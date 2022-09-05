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

/// JTAG Class, responsible of initializing and creating an object to communicate with board
class JtagFT2232
{
public:
    /// Default constructor
    JtagFT2232();

    /// Initialize the driver to connect to the probe
    void initializeDriver(std::string probe_name, std::string bsdl_name);

    /// Getter of Jc object
    jtag_core *getJc();

    /// Gets the ID of the board
    char *getIdStr(int numberofdevice, jtag_core *jc);

    /// Prints the available probes and returns the one needed
    int getAvailableProbes(jtag_core *jc, std::string probe_name);

    /// Prints the pins names connected to the probe "Id"
    void printPins(jtag_core *jc, int device);

    /// Gets the ID of the probe
    int getProbeId();

    /// Return the probe name
    std::string getProbeName();

    /// Prints the ID of the board
    int printJtagDevices(jtag_core *jc);

    /// Formats the string ID
    void bsdlIdStr(unsigned long id, char *str);

    /// Deinit the driver
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