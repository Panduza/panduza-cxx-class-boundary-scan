#ifndef _METADRIVERFT2232BSDLLOADER_
#define _METADRIVERFT2232BSDLLOADER_
#pragma once

#include <iostream>
#include <boost/crc.hpp>
#include <sstream>
#include "../../../headers/meta_driver.hxx"

class MetaDriverFT2232BoundaryScan;

/// Manage the bsdl file loading via mqtt
class MetaDriverFT2232BsdlLoader : public MetaDriver
{
public:
    /// Constructor @param meta_driver_ft2232_boundary_scan_instance Meta Driver Boundary Scan object
    MetaDriverFT2232BsdlLoader(MetaDriverFT2232BoundaryScan *meta_driver_ft2232_boundary_scan_instance);

    /// Setup the meta driver
    void setup();

    /// Send info
    void sendInfo();

    /// launched when a message arrived on subscribed topics @param msg Receiver message from the MQTT broker
    void message_arrived(mqtt::const_message_ptr msg);

    void checkBSDLFile(std::string bsdl_path);

private:
    std::string mDecodedBSDL;
    std::string mBSDLName;

    MetaDriverFT2232BoundaryScan *mMetaDriverFT2232BoundaryScanInstance;
};

#endif