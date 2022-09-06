/**
 * @file   jtag_manager.hpp
 * @brief  Manages the communication with board over JTAG
 * @author Adel
 */

#ifndef _JTAGFT2232_
#define _JTAGFT2232_

#include <iostream>
#include <cstring>
#include "/panduza-cxx-platform/src/loguru/loguru.hxx"
#include "jtag_core/jtag_core.hxx"

/// @brief JTAG Class, responsible of initializing and creating an object to communicate with the board
class JtagFT2232
{
public:
    /// @brief Default constructor
    JtagFT2232();

    /// @brief Initialize the driver to connect to the probe @param probe_name Name of the probe used @param bsdl_name Name of the bsdl file
    void initializeDriver(std::string probe_name, std::string bsdl_name);

    /// @brief Getter of Jc object @return jtag_core object
    jtag_core *getJc();

    /// @brief Gets the ID of the board @param numberofdevice Number of device @param jc jtag_core object @return get the IDs of the device connected in char format
    char *getIdStr(int numberofdevice, jtag_core *jc);

    /// @brief Prints the available probes and returns the one needed @param jc jtag_core object @param probe_name Name of the probe @return Number of pins (int)
    int getAvailableProbes(jtag_core *jc, std::string probe_name);

    /// @brief Prints the pins names connected to the probe "Id" @param jc Jtag_core object @param device Device number to select
    void printPins(jtag_core *jc, int device);

    /// @brief Gets the ID of the probe @return Probe Id (int)
    int getProbeId();

    /// @brief Return the probe name @return Probe name
    std::string getProbeName();

    /// @brief Prints the ID of the board @param jc Jtag_core object @return ...
    int printJtagDevices(jtag_core *jc);

    /// @brief Formats the string ID @param id id of the device @param str ...
    void bsdlIdStr(unsigned long id, char *str);

    /// @brief Deinit the driver
    void deinit();
    
private:
    bool mJtagDriverLoaded;
    /// Member to communicate over JTAG
    jtag_core *mJc;

    /// Id of the connected probe
    int mProbeId;

    std::string mProbeName;
    
};

#endif