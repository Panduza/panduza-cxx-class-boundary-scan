/**
 * @file   meta_driver_FT2232_boundary_scan.cxx
 *  Manages and keeps informations about each IO
 * @author Valentin 
 */

#include <iomanip>
#include "meta_driver_FT2232_boundary_scan.hxx"
#include "../../../headers/meta_platform.hxx"

std::shared_ptr<JtagFT2232> MetaDriverFT2232BoundaryScan::mJtagManager;
bool MetaDriverFT2232BoundaryScan::mJtagManagerLoaded = false;
std::map<std::string, std::string> MetaDriverFT2232BoundaryScan::mBSDLFileIdCode;
bool MetaDriverFT2232BoundaryScan::mBSDLFileIdCodeLoaded = false;

/// Constructor with parent pointer @param meta_platform_interface Meta Platform object
MetaDriverFT2232BoundaryScan::MetaDriverFT2232BoundaryScan(Metaplatform *meta_platform_interface)
{
    mMetaplatformInstance = meta_platform_interface;

    // Transfert back the loguru verbose and file logging
    loguru::g_stderr_verbosity = mMetaplatformInstance->mLoguruVerbose;
    loguru::add_file("../logs/Platform.log", loguru::Append, loguru::Verbosity_MAX);
    loguru::add_file("../logs/BoundaryScan.log", loguru::Append, loguru::Verbosity_MAX);
}

// ============================================================================
//

void MetaDriverFT2232BoundaryScan::setup()
{
    // Get Probe name
    mProbeName = getInterfaceTree()["settings"]["probe_name"].asString();
    
    // Create a unique name for the driver name when there is multiple driver with the same name
    mInterfaceTree = getInterfaceTree();

    mInterfaceTree["driver"] = mInterfaceTree["driver"].asString() + "_" + getProbeName();
    (mInterfaceTree["settings"]["device_no"].isNull()) ? mDeviceNo = -1 : mDeviceNo = mInterfaceTree["settings"]["device_no"].asInt();
    (mInterfaceTree["settings"]["idcode"].isNull()) ? mIdcode = "" : mIdcode = mInterfaceTree["settings"]["idcode"].asString();
    mProbeName = mInterfaceTree["settings"]["probe_name"].asString();

    // If there is no Jtag Manager, create it and pass a flag to true
    if (!mJtagManagerLoaded)
    {
        mJtagManager = getJtagManager();
        mJtagManagerLoaded = true;
    }
    if (!mBSDLFileIdCodeLoaded)
    {
        loadBSDLIdCode();
        mBSDLFileIdCodeLoaded = true;
    }

    if(mDeviceNo != -1)
    {
        int device_dec_id = jtagcore_get_dev_id(mJtagManager->getJc(), mDeviceNo);
        device_dec_id = device_dec_id & 0x0fffffff;

        std::stringstream hex_ss;
        hex_ss<< std::hex << device_dec_id;
        std::string device_hex_id ("0x" + hex_ss.str());

        findCorrespondingBsdlFile(device_hex_id);

        if(mBSDLName.empty())
        {
            LOG_F(ERROR, "No BSDL File found for device no : %d, it's idcode is : %s", mDeviceNo,device_hex_id.c_str());
            exit(1);
        }
    }
    else
    {
        if(mIdcode.empty())
        {
            LOG_F(ERROR,"No device number or idcode defined in the tree, ... exiting for now");
            exit(1);
        }
        findCorrespondingBsdlFile(mIdcode);
    }
   
    
    std::ifstream bsdl_file(mBSDLName, std::ifstream::binary);

    if (bsdl_file.is_open())
    {
        LOG_F(INFO, "BSDL File found, loading the BSDL...");
        startIo();
        bsdl_file.close();
    }
    else
    {
        LOG_F(ERROR, "No BSDL file found... exiting...");
        exit(1);
    }
}

void MetaDriverFT2232BoundaryScan::findCorrespondingBsdlFile(std::string idcode)
{
    for(auto idcode_line : mBSDLFileIdCode)
    {
        LOG_F(ERROR, "%s, \"%s\", \"%s\"", idcode_line.second.c_str(), idcode_line.first.c_str(), idcode.c_str());
        if(strcmp(idcode_line.first.c_str(),idcode.c_str()) == 0)
        {
            LOG_F(ERROR, "YEP");
            mBSDLName = idcode_line.second;
        }
    }
}

// ============================================================================
//

void MetaDriverFT2232BoundaryScan::startIo()
{
    // Kill all reloadable instances
    mMetaplatformInstance->clearReloadableInterfaces(getDriverName() + "_io_list_" + std::to_string(mDeviceNo));

    mJtagManager->initializeDevice(mProbeName, mBSDLName, mDeviceNo);

    // Create the Group Info meta Driver where it will store the payload of the jtagManager infos...
    createGroupInfoMetaDriver();

    // get some variable and key point
    mRepeatedJson = mInterfaceTree["repeated"];
    const std::string format = "%r";
    const size_t posFormat = mInterfaceTree["name"].asString().find(format);
    
    if(mRepeatedJson.isNull() || mRepeatedJson.size() == 0)
    {
        addAllIoPins();
    }

    std::list<std::shared_ptr<MetaDriver>> io_list;
    // Loop into the repeated list of pins
    for (auto repeated_pin : mRepeatedJson)
    {
        // Create a json with the good pin name
        Json::Value interface_json_copy = mInterfaceTree;
        interface_json_copy["settings"]["pin"] = interface_json_copy["settings"]["pin"].asString().replace(0, format.length(), repeated_pin.asString());
        interface_json_copy["name"] = interface_json_copy["name"].asString().replace(posFormat, format.length(), repeated_pin.asString());
        LOG_F(2, "loading driver for pin : %s", interface_json_copy["name"].asString().c_str());

        // Create the Meta Driver
        std::shared_ptr<MetaDriver> meta_driver_io_instance = std::make_shared<MetaDriverFT2232Io>(getJtagManager());

        // Initialize the meta Driver
        meta_driver_io_instance->initialize(getMachineName(), getBrokerName(), getBrokerAddr(), getBrokerPort(), interface_json_copy);


        // add the meta driver list to the main list
        io_list.emplace_back(meta_driver_io_instance);
    }

    LOG_F(INFO, "Adding IOs list with the key : %s", (getDriverName() + "_io_list_" + std::to_string(mDeviceNo)).c_str());
    std::map<std::string, std::list<std::shared_ptr<MetaDriver>>> io_map_entry {{getDriverName() + "_io_list_" + std::to_string(mDeviceNo), io_list}};
    // std::pair<std::string, std::list<std::shared_ptr<MetaDriver>>> io_map_entry (getProbeName(), io_list);
    
    mMetaplatformInstance->addReloadableDriverInstance(io_map_entry);
}

// ============================================================================
//

std::shared_ptr<JtagFT2232> MetaDriverFT2232BoundaryScan::getJtagManager()
{
    if (mJtagManagerLoaded == false)
    {
        mJtagManager = createJtagManager(mProbeName);
        mJtagManagerLoaded = true;
    }

    return mJtagManager;
}

// ============================================================================
//

std::shared_ptr<JtagFT2232> MetaDriverFT2232BoundaryScan::createJtagManager(std::string probe_name)
{
    // Create the Jtag manager with the probe name and bsdl name
    std::shared_ptr<JtagFT2232> jtagManager = std::make_shared<JtagFT2232>();
    jtagManager->initializeDriver(probe_name);

    return jtagManager;
}

// ============================================================================
//

void MetaDriverFT2232BoundaryScan::sendInfo() {}

// ============================================================================
//

void MetaDriverFT2232BoundaryScan::createGroupInfoMetaDriver()
{
    int probe_id = mJtagManager->getProbeId();
    std::string probe_name = mProbeName;
    int nb_of_devices = jtagcore_get_number_of_devices(mJtagManager->getJc());

    Json::Value payload;

    payload["probe_id"] = probe_id;
    payload["probe_name"] = probe_name;
    payload["nb_of_devices"] = nb_of_devices;

    std::shared_ptr<MetaDriver> meta_driver_group_info = std::make_shared<MetaDriverGroupInfo>(payload);

    // Initialize the meta Driver
    meta_driver_group_info->initialize(getMachineName(), getBrokerName(), getBrokerAddr(), getBrokerPort(), mInterfaceTree);

    // std::map<std::string,std::list<std::shared_ptr<MetaDriver>>> group_info_map_entry;
    std::list<std::shared_ptr<MetaDriver>> group_info_list;
    group_info_list.emplace_back(meta_driver_group_info);
    
    std::map<std::string, std::list<std::shared_ptr<MetaDriver>>> group_info_map_entry {{getDriverName()+"_group_info_" + std::to_string(mDeviceNo), group_info_list}};
    // add the meta driver to the main list
    mMetaplatformInstance->addReloadableDriverInstance(group_info_map_entry);    
}

// ============================================================================
//

Json::Value MetaDriverFT2232BoundaryScan::generateAutodetectInfo()
{
    Json::Value json;
    Json::Value template_json;
    Json::Value template_settings_json;
    Json::Value autodetect_settings_json;
    Json::Value autodetect_json;

    template_json["name"] = "%r";
    template_json["group_name"] = "??? (optional)";
    template_json["driver"] = "Scan_Service";
    template_json["settings"]["probe_name"] = "???";
    template_json["settings"]["device_no"] = "???";
    template_json["settings"]["bsdl_library"] = "???";
    template_json["settings"]["idcode"] = "???";
    template_json["settings"]["pin"] = "%r";
    template_json["repeated"] = Json::arrayValue;

    std::shared_ptr<JtagFT2232> jtagManager = std::make_shared<JtagFT2232>();
    jtagManager->autodetectInitialization();

    int number_of_probes = jtagManager->getNumberOfProbes();
    LOG_F(9, "Number of probes : %d", number_of_probes);

    for(int probe_id = 0; probe_id < number_of_probes; probe_id++)
    {
        std::string probe_name = jtagManager->getProbeName(probe_id);

        autodetect_json = template_json;
        autodetect_json["settings"]["probe_name"] = probe_name;
        
        int number_of_devices = jtagManager->getNumberOfDevices(probe_id);
        LOG_F(9, "Number of devices for probe %s : %d", probe_name.c_str(), number_of_devices);
        for(int device_id = 0; device_id < number_of_devices; device_id++)
        {
            autodetect_json["settings"]["device_no"] = device_id;

            json["autodetect"].append(autodetect_json);
        }
    }

    if(number_of_probes == 0)
    {
    json["autodetect"] = Json::arrayValue;
    }

    json["name"] = "BoundaryScan";
    json["version"] = "1.0";
    json["description"] = "Boundary Scan interface";
    json["template"] = template_json;

    return json;
}

void MetaDriverFT2232BoundaryScan::addAllIoPins()
{
    int number_of_pin = jtagcore_get_number_of_pins(mJtagManager->getJc(), mDeviceNo);
    LOG_F(ERROR, "Nb of pin : %d", number_of_pin);

    for(int i = 0; i < number_of_pin; i++)
    {
        char tmp_name[128];
        jtagcore_get_pin_properties(mJtagManager->getJc(), mDeviceNo, i, tmp_name, sizeof(tmp_name), 0);
        std::string pinName(tmp_name);
        
        int pin_type = jtagcore_get_pintype(mJtagManager->getJc(), mDeviceNo, pinName.data());

        if(pin_type > 0)
        {
            mRepeatedJson.append(pinName);
        }
    }
}

void MetaDriverFT2232BoundaryScan::loadBSDLIdCode()
{
    boost::filesystem::path bsdl_library = mInterfaceTree["settings"]["bsdl_library"].asString();

    if(boost::filesystem::is_directory(bsdl_library))
    {
        for (auto file : boost::filesystem::directory_iterator(bsdl_library))
        {
            std::ifstream bsdl_file(file.path().c_str(), std::ifstream::binary);
            if(bsdl_file.is_open())
            {
                bsdl_file.close();
                
                int bsdl_dec_id = jtagcore_get_bsdl_id(mJtagManager->getJc(), file.path().c_str());

                std::stringstream hex_ss;
                hex_ss<< std::hex << bsdl_dec_id;
                std::string bsdl_hex_id (hex_ss.str());

                mBSDLFileIdCode["0x" + bsdl_hex_id] = file.path().c_str();
            }
        }
    }
    else
    {
        LOG_F(ERROR, "The path given is not a directory");
    }
}

// ============================================================================
//
std::shared_ptr<MetaDriver> MetaDriverFactoryFT2232BoundaryScan::createDriver(void *arg)
{
    // get the parameter and give it to the meta driver constructor
    Metaplatform *meta_platform_instance = (Metaplatform *)arg;
    auto MetaDriverFactoryFT2232BoundaryScanInstance = std::make_shared<MetaDriverFT2232BoundaryScan>(meta_platform_instance);

    return MetaDriverFactoryFT2232BoundaryScanInstance;
}