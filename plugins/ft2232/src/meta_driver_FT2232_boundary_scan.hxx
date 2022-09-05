#ifndef _METADRIVERFT2232BOUNDARYSCAN_
#define _METADRIVERFT2232BOUNDARYSCAN_
#pragma once

// #include "ft2232_boundary_scan/jtag_manager.hxx"
#include "/panduza-cxx-platform/src/meta_driver.hxx"
#include "ft2232_boundary_scan/jtag_core/jtag_core.hxx"
#include "ft2232_boundary_scan/jtag_manager.hxx"
#include "meta_driver_FT2232_bsdl_loader.hxx"
#include "meta_driver_FT2232_io.hxx"
#include "meta_driver_group_info.hxx"

#include <thread>
#include <boost/config.hpp>

class Io;
class Metaplatform;

/// @brief Main instance that will handle functionalities of the plugin
class MetaDriverFT2232BoundaryScan : public MetaDriver
{
public:
    /// Destructor
    ~MetaDriverFT2232BoundaryScan() { LOG_F(9, "Meta Driver Io Fake Destructor"); }

    /// Constructor with parent pointer
    MetaDriverFT2232BoundaryScan(Metaplatform *meta_platform_interface){ mMetaplatformInstance = meta_platform_interface; }

    /// Setup the meta driver
    void setup();

    /// Start the creation of Io Meta Driver
    void startIo();

    /// Return JtagManager Object
    std::shared_ptr<JtagFT2232> getJtagManager();

    /// Create a Jtag manager and initialize it
    std::shared_ptr<JtagFT2232> createJtagManager(std::string probe_name, std::string bsdl_name);

    /// Send Info function
    void sendInfo();

    /// set the BSDL name
    void setBSDLName(std::string bsdl_file_name) { mBSDLName = bsdl_file_name; }

    /// Create group info meta driver
    void createGroupInfoMetaDriver();

private:
    std::string mBSDLName;
    std::string mMode;
    std::string mProbeName;

    static std::mutex mSetupMutex;

    Metaplatform *mMetaplatformInstance;

    // static std::list<std::shared_ptr<JtagFT2232>> JTAG_MANAGERS;
    std::shared_ptr<JtagFT2232> mJtagManager;
    bool mJtagManagerLoaded = false;

    Json::Value mInterfaceTree;
};

/// @brief Boundary Scan Factory
class MetaDriverFactoryFT2232BoundaryScan : public MetaDriverFactory
{
public:
    /// Constructor
    MetaDriverFactoryFT2232BoundaryScan(){};

    /// Create the meta driver boundary scan
    std::shared_ptr<MetaDriver> createDriver(void *arg);
};

#endif