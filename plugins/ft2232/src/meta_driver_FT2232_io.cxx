#include "meta_driver_FT2232_io.hxx"
// #include "meta_driver_FT2232_boundary_scan.hxx"

std::mutex MetaDriverFT2232Io::mJtagMutex;

MetaDriverFT2232Io::~MetaDriverFT2232Io()
{
    mKillThread = true;

    mAlternativeThread->join();
    delete mAlternativeThread;

    mKillThread = false;
}

MetaDriverFT2232Io::MetaDriverFT2232Io(std::shared_ptr<JtagFT2232> jc)
{
    mJtagManager = jc;
}

void MetaDriverFT2232Io::setup()
{
    mJc = mJtagManager->getJc();
    mProbeId = mJtagManager->getProbeId();

    // Get the name of the pin to use it on the function
    mPinName = getInterfaceTree()["settings"]["pin"].asString();
    mDeviceNo = getInterfaceTree()["settings"]["device_no"].asInt();
    LOG_F(4, "driver instance for pin : %s", mPinName.c_str());

    // Load the pin
    mId = jtagcore_get_pin_id(mJc, mDeviceNo, mPinName.data());
    mPinType = jtagcore_get_pintype(mJc, mDeviceNo, mPinName.data());
    mState = -1;
    mSavedState = -1;
    mReadState = 1;
    mDirection = "unknown";

    if(!getInterfaceTree()["group_name"].isNull())
    {
        setBaseTopic(getBaseTopic()+  "/" + mPinName);
    }
    else
    {
        setBaseTopic(getBaseTopic() + "_device_"+ std::to_string(mDeviceNo) +  "/" + mPinName);
    }

    // Subscribe to the different topic needed direction and value separated because of retained not coming in the good order
    subscribe(getBaseTopic() + "/cmds/#", 0);
    subscribe(getBaseTopic() + "/atts/direction", 0);
    subscribe(getBaseTopic() + "/atts/value", 0);

    mAlternativeThread = new std::thread(&MetaDriverFT2232Io::checkInput, this);
}

int MetaDriverFT2232Io::readInputState()
{
    // Return -1 if there is an error
    if (jtagcore_push_and_pop_chain(mJc, JTAG_CORE_WRITE_READ) < 0)
    {
        return -1;
    }

    // Return the actual state
    return jtagcore_get_pin_state(mJc, mDeviceNo, mId, JTAG_CORE_INPUT);
}

void MetaDriverFT2232Io::setState(int state)
{
    // Set the state of the Io
    mState = state;
}

void MetaDriverFT2232Io::setSavedState(int save)
{
    // Set the saved state of the Io
    mSavedState = save;
}

void MetaDriverFT2232Io::setOutputOn()
{
    // Set Output to On
    jtagcore_set_pin_state(mJc, mDeviceNo, mId, JTAG_CORE_OUTPUT, 1);
    jtagcore_set_pin_state(mJc, mDeviceNo, mId, JTAG_CORE_OE, 1);
    jtagcore_push_and_pop_chain(mJc, JTAG_CORE_WRITE_ONLY);
}

void MetaDriverFT2232Io::setOutputOff()
{
    // Set Output to Off
    jtagcore_set_pin_state(mJc, mDeviceNo, mId, JTAG_CORE_OUTPUT, 0);
    jtagcore_set_pin_state(mJc, mDeviceNo, mId, JTAG_CORE_OE, 0);
    jtagcore_push_and_pop_chain(mJc, JTAG_CORE_WRITE_ONLY);
}

int MetaDriverFT2232Io::publishState()
{
    std::string PUB_TOPIC_VALUE = getBaseTopic() + "/atts/value";
    mStatePayload["value"] = mState;

    publish(PUB_TOPIC_VALUE, mStatePayload, 0, true);

    return 0;
}

int MetaDriverFT2232Io::publishDirection()
{
    std::string PUB_TOPIC_DIRECTION = getBaseTopic() + "/atts/direction";
    mDirectionPayload["direction"] = mDirection;

    publish(PUB_TOPIC_DIRECTION, mDirectionPayload, 0, true);

    return 0;
}

void MetaDriverFT2232Io::checkInput()
{
    loguru::set_thread_name("MetaDriverIo");

    while (!mKillThread)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // Parse IO vector, search for IO input, keeps track of their actual state and checks for changes to publish them.
        if (mDirection == "in")
        {
            mJtagMutex.lock();
            //	If the pin state hasnt been alrady read, we read it and mark as read
            const int inputState = readInputState();

            //	If the saved state and actual state are different, we publish the state
            if ((mState != inputState) && (inputState >= 0))
            {
                setState(inputState);
                publishState();
            }
            mJtagMutex.unlock();
        }
    }
}

void MetaDriverFT2232Io::message_arrived(mqtt::const_message_ptr msg)
{
    loguru::set_thread_name("Sub callback");
    Json::Value root;
    Json::Reader reader;
    std::string SubDir, val, driver = getDriverName() + "/";
    std::size_t start;
    int SubVal;

    VLOG_SCOPE_F(2, "Message arrived in Boundary Scan");

    // Log the topic and payload
    LOG_F(3, "Topic: %s", msg->get_topic().c_str());
    LOG_F(3, "Payload: %s", msg->to_string().c_str());

    // if the topic is pza, send the info
    if ((msg->get_topic().compare("pza") == 0) && (msg->get_payload().compare("*") == 0))
    {
        LOG_F(3, "Sending infos");
        sendInfo();
    }
    else
    {
        // Parse the message, get the name of the IO from it and the object associated to that IO
        reader.parse(msg->to_string().c_str(), root);  

        // If message contains "direction", get it
        if (msg->get_topic().find("direction") != std::string::npos)
        {
            SubDir = root.get("direction", "").asString();
            // If the direction of the pin is different than the direction published in the topic
            if (SubDir != mDirection)
            {
                // Lock Jtag Connection Mutex
                mJtagMutex.lock();
                // If direction is "input" or "output", edit the object and publish its direction
                if (SubDir == "in")
                {
                    if (mPinType == 1 || mPinType == 3)
                    {
                        LOG_F(INFO, "Setting %s as input", mPinName.c_str());

                        mDirection = "in";
                        publishDirection();
                        setState(mSavedState);
                    }
                    else
                    {
                        LOG_F(WARNING, "This pin cannot be change to Input, please verify the pin characteristics");
                    }
                    
                }
                else if (SubDir == "out")
                {
                    if (mPinType == 2 || mPinType == 3)
                    {
                        LOG_F(INFO, "Setting %s as output", mPinName.c_str());

                    mDirection = "out";
                    publishDirection();
                    }
                    else
                    {
                        LOG_F(WARNING, "This pin cannot be change to Output, please verify the pin characteristics");
                    }
                    
                }
                else
                {
                    LOG_F(WARNING, "Expecting in or out as pin direction");
                }

                // Unlock Jtag Connection Mutex
                mJtagMutex.unlock();
            }
        }
        // If the message contains "value" and IO's direction is output, get the value
        if (((msg->get_topic().find("value")) != (std::string::npos)) && (mDirection == "out"))
        {
            val = root.get("value", "").asString();
            if (isStringANumber(val))
            {
                SubVal = stoi(val);
                LOG_F(3, "topic = %s, payload = %s", msg->get_topic().c_str(), msg->get_payload().c_str());
                LOG_F(3, "value : %d", SubVal);
                // If the state of the pin is different then the state published in the topic
                if (SubVal != mState)
                {
                    // Lock Jtag connection mutex
                    mJtagMutex.lock();
                    // If value = 0/1, edit the object and publish its state
                    if (SubVal == 0)
                    {
                        LOG_F(INFO, "Setting %s to state %d", mPinName.c_str(), 0);
                        setOutputOff();
                        setState(0);
                        publishState();
                    }
                    else if (SubVal == 1)
                    {
                        LOG_F(INFO, "Setting %s to state %d", mPinName.c_str(), 1);
                        setOutputOn();
                        setState(1);
                        publishState();
                    }
                    else
                    {
                        LOG_F(WARNING, "Expecting 0 or 1 as pin value");
                    }

                    //Unlock Jtag connection Mutex
                    mJtagMutex.unlock();
                }
            }
            else
            {
                LOG_F(WARNING,"The value received is not a integer. Please verify your payload.");
            }
        }
    }
}

void MetaDriverFT2232Io::sendInfo()
{
    // Create the info payload
    Json::Value info;
    info["type"] = "io";
    info["version"] = "1.0";

    LOG_F(4, "Info sent is : %s", info.toStyledString().c_str());

    // publish the message info to the mqtt server for the pin
    publish(getBaseTopic() + "/info", info, 0, false);
}

bool MetaDriverFT2232Io::isStringANumber(const std::string& string)
{
    return !string.empty() && std::find_if(string.begin(), 
        string.end(), [](unsigned char c) { return !std::isdigit(c); }) == string.end();
}