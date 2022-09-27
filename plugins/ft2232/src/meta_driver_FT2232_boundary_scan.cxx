/**
 * @file   meta_driver_FT2232_boundary_scan.cxx
 *  Manages and keeps informations about each IO
 * @author Valentin 
 */

#include "meta_driver_FT2232_boundary_scan.hxx"
#include "../../../headers/meta_platform.hxx"

std::shared_ptr<JtagFT2232> MetaDriverFT2232BoundaryScan::mJtagManager;
bool MetaDriverFT2232BoundaryScan::mJtagManagerLoaded = false;

// ============================================================================
//

void MetaDriverFT2232BoundaryScan::setup()
{
    // Transfert back the loguru verbose and file logging
    loguru::g_stderr_verbosity = mMetaplatformInstance->mLoguruVerbose;
    loguru::add_file("../logs/Platform.log", loguru::Append, loguru::Verbosity_MAX);
    loguru::add_file("../logs/BoundaryScan.log", loguru::Append, loguru::Verbosity_MAX);

    // Get Probe name
    mProbeName = getInterfaceTree()["settings"]["probe_name"].asString();
    
    // Create a unique name for the driver name when there is multiple driver with the same name
    mInterfaceTree = getInterfaceTree();
    int serial_start_index = mInterfaceTree["settings"]["probe_name"].asString().find(" FT") + 1;
    int probe_name_size = mInterfaceTree["settings"]["probe_name"].size();
    std::string probe_serial_no = mProbeName.substr( serial_start_index, probe_name_size - serial_start_index);
    mInterfaceTree["driver"] = mInterfaceTree["driver"].asString() + "_" + probe_serial_no;
    mDeviceNo = mInterfaceTree["settings"]["device_no"].asInt();

    // Create Meta Driver File
    std::shared_ptr<MetaDriver> meta_driver_file_instance = std::make_shared<MetaDriverFT2232BsdlLoader>(this);

    // Initialize the meta_driver file instance
    meta_driver_file_instance->initialize(getMachineName(), getBrokerName(), getBrokerAddr(), getBrokerPort(), mInterfaceTree);

    // Add the meta driver instance to the main meta driver list
    mMetaplatformInstance->addStaticDriverInstance(meta_driver_file_instance);
}

// ============================================================================
//

void MetaDriverFT2232BoundaryScan::startIo()
{
    // Kill all reloadable instances
    // mMetaplatformInstance->clearReloadableInterfaces();

    // If there is a jtagManager loaded, delete it and reset its flag
    // if (mJtagManagerLoaded)
    // {
    //     mJtagManager->deinit();
    //     mJtagManager.reset();
    //     mJtagManagerLoaded = false;
    // }

    // If there is no Jtag Manager, create it and pass a flag to true
    if (!mJtagManagerLoaded)
    {
        mJtagManager = createJtagManager(mProbeName, mBSDLName);
        mJtagManagerLoaded = true;
    }

    // Create the Group Info meta Driver where it will store the payload of the jtagManager infos...
    createGroupInfoMetaDriver();

    // get some variable and key point
    const Json::Value repeated_json = mInterfaceTree["repeated"];
    const std::string format = "%r";
    const size_t posFormat = mInterfaceTree["name"].asString().find(format);
    
    std::list<std::shared_ptr<MetaDriver>> io_list;
    // Loop into the repeated list of pins
    for (auto repeated_pin : repeated_json)
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


        // add the meta driver to the main list
        // mMetaplatformInstance->addReloadableDriverInstance(meta_driver_io_instance);
        io_list.emplace_back(meta_driver_io_instance);
    }
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
        mJtagManager = createJtagManager(mProbeName, mBSDLName);
        mJtagManagerLoaded = true;
    }
    mJtagManager->initializeDevice(mProbeName, mBSDLName, mDeviceNo);

    return mJtagManager;
}

// ============================================================================
//

std::shared_ptr<JtagFT2232> MetaDriverFT2232BoundaryScan::createJtagManager(std::string probe_name, std::string bsdl_name)
{
    // Create the Jtag manager with the probe name and bsdl name
    std::shared_ptr<JtagFT2232> jtagManager = std::make_shared<JtagFT2232>();
    jtagManager->initializeDriver(probe_name, bsdl_name);

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

    LOG_F(ERROR,"%d", probe_id);
    LOG_F(ERROR,"%s", probe_name.c_str());
    LOG_F(ERROR,"%d", nb_of_devices);

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
std::shared_ptr<MetaDriver> MetaDriverFactoryFT2232BoundaryScan::createDriver(void *arg)
{
    // get the parameter and give it to the meta driver constructor
    Metaplatform *meta_platform_instance = (Metaplatform *)arg;
    auto MetaDriverFactoryFT2232BoundaryScanInstance = std::make_shared<MetaDriverFT2232BoundaryScan>(meta_platform_instance);

    return MetaDriverFactoryFT2232BoundaryScanInstance;
}