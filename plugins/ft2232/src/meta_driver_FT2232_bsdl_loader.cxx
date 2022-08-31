#include "meta_driver_FT2232_bsdl_loader.hxx"
#include "meta_driver_FT2232_boundary_scan.hxx"
#include "/panduza-cxx-platform/src/base64/base64.hxx"
#include <fstream>

class MetaDriverFT2232BoundaryScan;

MetaDriverFT2232BsdlLoader::MetaDriverFT2232BsdlLoader(MetaDriverFT2232BoundaryScan* meta_driver_ft2232_boundary_scan_instance)
{
    // Save the parent instance
    mMetaDriverFT2232BoundaryScanInstance = meta_driver_ft2232_boundary_scan_instance;
}

// ============================================================================
//
void MetaDriverFT2232BsdlLoader::setup()
{
    // Subscribe to the different topic needed direction and value separated because of retained not coming in the good order
    subscribe(getBaseTopic() + "/Bsdl_File/cmds/#", 0);
    subscribe(getBaseTopic() + "/Bsdl_File/atts/content", 0);
    subscribe(getBaseTopic() + "/Bsdl_File/atts/data", 0);

    LOG_F(ERROR, "%s.bsdl",getDriverName().c_str());
    // Try to open the BSDL
    std::ifstream bsdl_file(getDriverName() + ".bsdl", std::ifstream::binary);

    // If it does open, then start the launch of Ios
    if (bsdl_file.is_open())
    {
        LOG_F(INFO, "BSDL File found, loading the BSDL...");
        mMetaDriverFT2232BoundaryScanInstance->setBSDLName(getDriverName() + ".bsdl");
        mMetaDriverFT2232BoundaryScanInstance->setProbeName(getProbeName());
        mMetaDriverFT2232BoundaryScanInstance->startIo();
        
        bsdl_file.close();
    }
}

// ============================================================================
//
void MetaDriverFT2232BsdlLoader::sendInfo()
{
    // Create the info payload
    Json::Value info;
    info["type"] = "file";
    info["version"] = "1.0";

    LOG_F(4, "Info sent is : %s", info.toStyledString().c_str());

    // publish the message info to the mqtt server for the pin
    publish(getBaseTopic() + "/Bsdl_File/info", info, 0, false);
}

// ============================================================================
//
void MetaDriverFT2232BsdlLoader::message_arrived(mqtt::const_message_ptr msg)
{

    loguru::set_thread_name("Sub callback");
    Json::Value root;
    Json::Reader reader;
    std::string SubDir, val, driver = getDriverName() + "/";
    std::size_t start;

    // If the message topid is "pza" and the payload is "*"
    if ((msg->get_topic().compare("pza") == 0) && (msg->get_payload().compare("*") == 0))
    {
        LOG_F(3, "Sending infos");
        sendInfo();
    }
    else
    {
        Json::Value parsedMsg = parseMsg(msg->get_payload());

        if (msg->get_topic().find("/cmds/content/set") != std::string::npos)
        {
            int content_size = parsedMsg["data"].asString().size();
            parsedMsg["mime"] = "text/plain";
            
            // Get CRC32 encoded message and gets its checksum
            boost::crc_32_type crc32;
            crc32.process_bytes(parsedMsg["data"].asString().data(),content_size);
            std::stringstream crc32hex;
            crc32hex << std::hex << crc32.checksum();

            // Create one of the payload for atts
            Json::Value payload;
            payload["size"] = content_size;
            payload["crc"] = crc32hex.str();
            
            // Send payloads to atts
            publish(getBaseTopic() + "/Bsdl_File/atts/metadata", payload, 0, true);
            publish(getBaseTopic() + "/Bsdl_File/atts/content", parsedMsg, 0, true);

            // Get encoded message
            std::string encoded_message = parsedMsg["data"].asString();

            // Decode the payload
            mDecodedBSDL = base64_decode(encoded_message);

            // Create BSDL file and write the decoded content
            mBSDLName = getDriverName() +".bsdl";
            std::ofstream BSDLFile(mBSDLName);
            BSDLFile << mDecodedBSDL;
            BSDLFile.close();

            // Some variables to Boundary Scan instance
            mMetaDriverFT2232BoundaryScanInstance->setBSDLName(mBSDLName);
            mMetaDriverFT2232BoundaryScanInstance->setProbeName(getProbeName());

            // Start the Ios
            mMetaDriverFT2232BoundaryScanInstance->startIo();
        }
    }
}

// ============================================================================
//
void MetaDriverFT2232BsdlLoader::test()
{
    loguru::set_thread_name("MetaDriverFT2232BsdlLoader");
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        LOG_F(9,"MetaDriverFT2232BsdlLoader");
    }
}