#ifndef _METADRIVERFT2232BOUNDARYSCAN_
#define _METADRIVERFT2232BOUNDARYSCAN_
#pragma once

#include "../../../headers/meta_driver.hxx"
#include "ft2232_boundary_scan/jtag_core/jtag_core.hxx"
#include "ft2232_boundary_scan/jtag_manager.hxx"
#include "meta_driver_FT2232_io.hxx"
#include "meta_driver_group_info.hxx"

#include <boost/filesystem.hpp>
#include <thread>
#include <boost/config.hpp>
#include <sstream>

class Metaplatform;
class JtagFT2232;

/// Main instance that will handle functionalities of the plugin
class MetaDriverFT2232BoundaryScan : public MetaDriver
{
public:
    /// Destructor
    ~MetaDriverFT2232BoundaryScan() { LOG_F(9, "Meta Driver Io Fake Destructor"); }

    /// Constructor with parent pointer @param meta_platform_interface Meta Platform object
    MetaDriverFT2232BoundaryScan(Metaplatform *meta_platform_interface);

    /// Setup the meta driver
    void setup();

    /// Start the creation of Io Meta Driver
    void startIo();

    /// Return JtagManager Object @return Shared ptr of the Jtag connection
    std::shared_ptr<JtagFT2232> getJtagManager();

    /// Create a Jtag manager and initialize it @param probe_name Name of the probe @return Shared ptr of the Jtag connection
    std::shared_ptr<JtagFT2232> createJtagManager(std::string probe_name);

    /// Send Info function
    void sendInfo();

    /// set the BSDL name @param bsdl_file_name Name of the BSDL file
    void setBSDLName(std::string bsdl_file_name) { mBSDLName = bsdl_file_name; }

    /// Create group info meta driver that will return info about the boundary scan
    void createGroupInfoMetaDriver();

    Json::Value generateAutodetectInfo();

    void addAllIoPins();

    void loadBSDLIdCode();

private:
    std::string mBSDLName;
    std::string mProbeName;
    int mDeviceNo;
    std::string mIdcode;

    Metaplatform *mMetaplatformInstance;

    // static std::list<std::shared_ptr<JtagFT2232>> JTAG_MANAGERS;
    static std::shared_ptr<JtagFT2232> mJtagManager;
    static bool mJtagManagerLoaded;
    static std::map<std::string,std::string> mBSDLFileIdCode;
    static bool mBSDLFileIdCodeLoaded;

    Json::Value mInterfaceTree;
    Json::Value mRepeatedJson;
};

/// Boundary Scan Factory
class MetaDriverFactoryFT2232BoundaryScan : public MetaDriverFactory
{
public:
    /// Constructor
    MetaDriverFactoryFT2232BoundaryScan(){};

    /// Create the meta driver boundary scan @param arg instance of the platform @return shared pointer of the BoundaryScan Meta Driver
    std::shared_ptr<MetaDriver> createDriver(void *arg);
};

#endif