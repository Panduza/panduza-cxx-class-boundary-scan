#ifndef _METADRIVERFT2232BSDLLOADER_
#define _METADRIVERFT2232BSDLLOADER_
#pragma once

#include <iostream>
#include <boost/crc.hpp>
#include <sstream>
#include "/panduza-cxx-platform/src/meta_driver.hxx"

class MetaDriverFT2232BoundaryScan;

/// @brief Manage the bsdl file loading via mqtt
class MetaDriverFT2232BsdlLoader : public MetaDriver
{
public:
    /// @brief Constructor @param meta_driver_ft2232_boundary_scan_instance Meta Driver Boundary Scan object
    MetaDriverFT2232BsdlLoader(MetaDriverFT2232BoundaryScan *meta_driver_ft2232_boundary_scan_instance);

    /// @brief Setup the meta driver
    void setup();

    /// @brief Send info
    void sendInfo();

    /// @brief launched when a message arrived on subscribed topics @param msg Receiver message from the MQTT broker
    void message_arrived(mqtt::const_message_ptr msg);

private:
    std::string mDecodedBSDL;
    std::string mBSDLName;

    MetaDriverFT2232BoundaryScan *mMetaDriverFT2232BoundaryScanInstance;
};

#endif