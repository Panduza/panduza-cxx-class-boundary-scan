/**
 * @file   jtag_manager.hpp
 *  Manages the communication with board over JTAG
 * @author Adel
 */

#ifndef _JTAGFT2232_
#define _JTAGFT2232_

#include <iostream>
#include <cstring>
#include "../../../../headers/loguru.hxx"
#include "jtag_core/jtag_core.hxx"
#include <mutex>
#include <thread>

/// JTAG Class, responsible of initializing and creating an object to communicate with the board
class JtagFT2232
{
public:
    /// Default constructor
    JtagFT2232();

    /// Default destructor
    ~JtagFT2232();

    /// Initialize the driver to connect to the probe @param probe_name Name of the probe used
    void initializeDriver(std::string probe_name);

    void initializeDevice(std::string probe_name, std::string bsdl_name, int device_no);

    /// Getter of Jc object @return jtag_core object
    jtag_core *getJc();

    /// Gets the ID of the board @param numberofdevice Number of device @param jc jtag_core object @return get the IDs of the device connected in char format
    char *getIdStr(int numberofdevice, jtag_core *jc);

    /// Prints the available probes and returns the one needed @param jc jtag_core object @param probe_name Name of the probe @return Number of pins (int)
    int getAvailableProbes(jtag_core *jc, std::string probe_name);

    /// Prints the pins names connected to the probe "Id" @param jc Jtag_core object @param device Device number to select
    void printPins(jtag_core *jc, int device);

    /// Gets the ID of the probe @return Probe Id (int)
    int getProbeId();

    /// Return the probe name @return Probe name
    std::string getProbeName();

    /// Prints the number of device connected to the probe @param jc Jtag_core object @return Number of devices connected 
    int printJtagDevices(jtag_core *jc);

    /// Formats the string ID @param id id of the device @param str ...
    void bsdlIdStr(unsigned long id, char *str);

    /// @brief Check if there is a space at the start or at the end of the probe name and warn the user if it is the case
    void checkSpaceOnProbeLimit(char *probe_name);
    
    /// Deinit the driver
    void deinit();

    void autodetectInitialization(){mJc = jtagcore_init();};

    int getNumberOfProbes();
    std::string getProbeName(int id_of_probe);

    int getNumberOfDevices(int id_of_probe);

    void set_devices_to_load(int value){device_to_load = value;};

    void auto_refresh_push_and_pop();
    
private:
    bool auto_refresh_started = false;
    bool kill_auto_refresh = false;
    int device_to_load = 0;
    int device_loaded = 0;
    /// Member to communicate over JTAG
    jtag_core *mJc;

    /// Id of the connected probe
    int mProbeId;

    std::string mProbeName;
    std::thread* push_and_pop_auto_refresh;
};

#endif