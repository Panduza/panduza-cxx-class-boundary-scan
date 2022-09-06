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

class Metaplatform;

/// @brief Main instance that will handle functionalities of the plugin
class MetaDriverFT2232BoundaryScan : public MetaDriver
{
public:
    /// @brief Destructor
    ~MetaDriverFT2232BoundaryScan() { LOG_F(9, "Meta Driver Io Fake Destructor"); }

    /// @brief Constructor with parent pointer @param meta_platform_interface Meta Platform object
    MetaDriverFT2232BoundaryScan(Metaplatform *meta_platform_interface){ mMetaplatformInstance = meta_platform_interface; }

    /// @brief Setup the meta driver
    void setup();

    /// @brief Start the creation of Io Meta Driver
    void startIo();

    /// @brief Return JtagManager Object @return Shared ptr of the Jtag connection
    std::shared_ptr<JtagFT2232> getJtagManager();

    /// @brief Create a Jtag manager and initialize it @param probe_name Name of the probe @param bsdl_name Name of the BSDL file @return Shared ptr of the Jtag connection
    std::shared_ptr<JtagFT2232> createJtagManager(std::string probe_name, std::string bsdl_name);

    /// @brief Send Info function
    void sendInfo();

    /// @brief set the BSDL name @param bsdl_file_name Name of the BSDL file
    void setBSDLName(std::string bsdl_file_name) { mBSDLName = bsdl_file_name; }

    /// @brief Create group info meta driver that will return info about the boundary scan
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
    /// @brief Constructor
    MetaDriverFactoryFT2232BoundaryScan(){};

    /// @brief Create the meta driver boundary scan @param arg instance of the platform @return shared pointer of the BoundaryScan Meta Driver
    std::shared_ptr<MetaDriver> createDriver(void *arg);
};

#endif