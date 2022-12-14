/**
 * @file   jtag_manager.cpp
 *  Manages the communication with board over JTAG
 * @author Adel
 */

#include "jtag_manager.hxx"

#define REFRESH_RATE 5

JtagFT2232::JtagFT2232()
{
    mProbeName = "";
}

JtagFT2232::~JtagFT2232()
{
    deinit();
}

void JtagFT2232::initializeDriver(std::string probe_name)
{
    LOG_F(1, "Initializing the Driver for the probe : %s", probe_name.c_str());
    mProbeName = probe_name;
    
    // Initialization of mJc
    mJc = jtagcore_init();

    if (mJc)
    {
        // Scan available probes
        mProbeId = getAvailableProbes(mJc, probe_name);
        LOG_F(1, "Probe ID is : %d", mProbeId);
        // Selects the needed probe
        if (jtagcore_select_and_open_probe(mJc, mProbeId) >= 0) // Probe: FT2232H_MM A FT12345A ID=0
        {
            // Scan and initialize JTAG chain
            jtagcore_scan_and_init_chain(mJc);
        }

        // Gets the ID of the board
        printJtagDevices(mJc);
    }

    else
    {
        LOG_F(ERROR, "Couldn't initialise jc");
        exit(1);
    }
}

void JtagFT2232::initializeDevice(std::string probe_name, std::string bsdl_name, int device_no)
{
    // Loads the BSDL file
    jtagcore_loadbsdlfile(mJc, bsdl_name.c_str(), device_no);
    device_loaded++;
    // Enables test mode
    jtagcore_set_scan_mode(mJc, device_no, JTAG_CORE_EXTEST_SCANMODE);
    if(device_loaded == device_to_load)
    {
        jtagcore_push_and_pop_chain(mJc, JTAG_CORE_WRITE_ONLY);
        if(!auto_refresh_started)
            {
                auto_refresh_started = true;
                push_and_pop_auto_refresh = new std::thread(&JtagFT2232::auto_refresh_push_and_pop, this);
            }
    }
    
    // Prints all the pins on the board (verbosity 6 is needed)
    printPins(mJc, device_no);
}

int JtagFT2232::getProbeId()
{
    return mProbeId;
}

std::string JtagFT2232::getProbeName()
{
    return mProbeName;
}

jtag_core *JtagFT2232::getJc()
{
    return mJc;
}

char *JtagFT2232::getIdStr(int numberofdevice, jtag_core *mJc)
{
    int i;
    unsigned long idcode = 0;
    char *stringbuffer;
    char tempstr[DEFAULT_BUFLEN];

    stringbuffer = 0;

    stringbuffer = malloc(256 * numberofdevice);
    if (stringbuffer)
    {
        memset(stringbuffer, 0, 256 * numberofdevice);
        for (i = 0; i < numberofdevice; i++)
        {
            idcode = jtagcore_get_dev_id(mJc, i);
            sprintf(tempstr, "Device %d : 0x%.8lX - ", i, idcode);

            bsdlIdStr(idcode, &tempstr[strlen(tempstr)]);

            strcat(stringbuffer, tempstr);
            strcat(stringbuffer, "\n");
        }
    }

    return stringbuffer;
}

void JtagFT2232::bsdlIdStr(unsigned long id, char *str)
{
    int i;

    str[0] = 0;

    for (i = 0; i < 32; i++)
    {
        if ((0x80000000 >> i) & id)
        {
            strcat(str, "1");
        }
        else
        {
            strcat(str, "0");
        }
        if (i == 3)
            strcat(str, " ");
        if (i == 19)
            strcat(str, " ");
        if (i == 30)
            strcat(str, " ");
    }

    str[i] = 0;
}

int JtagFT2232::printJtagDevices(jtag_core *mJc)
{
    int ret;
    char *tempstring2;
    char tempstring[DEFAULT_BUFLEN];
    VLOG_SCOPE_F(6, "Print jtag devices");

    // Counts the peripherals on JTAG chain and prints the ID of the board
    ret = jtagcore_get_number_of_devices(mJc);
    if (ret > 0)
    {
        tempstring2 = getIdStr(ret, mJc);
        if (tempstring)
        {
            VLOG_F(2, "%s", tempstring2);
            free(tempstring2);
        }
        return 0;
    }
    else
    {
        LOG_F(ERROR, "No Device Found");
        exit(0);
    }
}

void JtagFT2232::printPins(jtag_core *mJc, int device)
{
    int nbOfPins;
    char tmp_name[128];
    VLOG_SCOPE_F(6, "pin names");
    nbOfPins = jtagcore_get_number_of_pins(mJc, device);
    LOG_F(5, "no of pin : %d", nbOfPins);
    for (int i = 0; i < nbOfPins; i++)
    {
        tmp_name[0] = 0;
        jtagcore_get_pin_properties(mJc, device, i, tmp_name, sizeof(tmp_name), 0);
        VLOG_F(6, "i = %d : %s", i, tmp_name);
    }
}

int JtagFT2232::getAvailableProbes(jtag_core *mJc, std::string probe_name)
{
    int i = 0;
    int good_id = -1;
    char tempstring[DEFAULT_BUFLEN - 7];
    char idstring[DEFAULT_BUFLEN];

    std::string good_probe = "Probe: " + probe_name;
    int nb_of_probes = jtagcore_get_number_of_probes(mJc, 0);

    if (nb_of_probes == 0)
    {
        LOG_F(ERROR, "No probes detected");
        exit(0);
    }

    while (i < nb_of_probes)
    {
        jtagcore_get_probe_name(mJc, PROBE_ID(0, i), tempstring);
        sprintf(idstring, "Probe: %s", tempstring);

        // Check if there is a space at the start and the end of the probe name
        checkSpaceOnProbeLimit(tempstring);

        VLOG_F(0, "probe name available with id %d : \"%s\"", i, idstring);
        if (strcmp(idstring, good_probe.c_str()) == 0)
        {
            LOG_F(3, "Probe found!");

            good_id = i;
        }
        i++;
    }

    if (good_id == -1)
    {
        LOG_F(ERROR, "%s not found", good_probe.c_str());
        exit(0);
    }

    return good_id;
}

void JtagFT2232::checkSpaceOnProbeLimit(char *probe_name)
{
    
    if (probe_name[0] == ' ')
    {
        LOG_F(WARNING, "There is a space at the begin of the probe name that we are trying to connect. Are you aware of it ?");
        LOG_F(WARNING, "Continuing...");
    }
    if (probe_name[strlen(probe_name)-1] == ' ')
    {
        LOG_F(WARNING, "There is a space at the end of the probe name that we are trying to connect. Are you aware of it ?");
        LOG_F(WARNING, "Continuing...");
    }
}

int JtagFT2232::getNumberOfProbes()
{
    return jtagcore_get_number_of_probes(mJc, 0);
}

std::string JtagFT2232::getProbeName(int id_of_probe)
{
    char tempstring[DEFAULT_BUFLEN - 7];

    jtagcore_get_probe_name(mJc, id_of_probe, tempstring);

    return tempstring;
}

int JtagFT2232::getNumberOfDevices(int id_of_probe)
{
    if (jtagcore_select_and_open_probe(mJc, id_of_probe) >= 0) // Probe: FT2232H_MM A FT12345A ID=0
        {
            // Scan and initialize JTAG chain
            jtagcore_scan_and_init_chain(mJc);
        }

    return jtagcore_get_number_of_devices(mJc);
}

void JtagFT2232::auto_refresh_push_and_pop()
{
    while (!kill_auto_refresh)
    {
        jtagcore_push_and_pop_chain(mJc, JTAG_CORE_WRITE_READ);
        std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_RATE));
    }
}

void JtagFT2232::deinit()
{
    if(auto_refresh_started)
    {
        kill_auto_refresh = true;
        push_and_pop_auto_refresh->join();
    }
    jtagcore_deinit(mJc);
}
