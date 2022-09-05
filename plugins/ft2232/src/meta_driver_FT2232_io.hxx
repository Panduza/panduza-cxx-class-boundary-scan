#ifndef _METADRIVERFT2232IO_
#define _METADRIVERFT2232IO_
#pragma once

#include "/panduza-cxx-platform/src/meta_driver.hxx"
#include "ft2232_boundary_scan/jtag_core/jtag_core.hxx"
#include "ft2232_boundary_scan/jtag_manager.hxx"

class Io;
class JtagFT2232;

class MetaDriverFT2232Io : public MetaDriver
{
public:
    /// Destructor
    ~MetaDriverFT2232Io();

    /// Constructor @param Jtag connexion
    MetaDriverFT2232Io(std::shared_ptr<JtagFT2232> jc);

    /// Setup the instance
    void setup();

    /// Sets a pin direction to input and reads its value @param Io : Io object of the  @return int : input state
    int readInputState();

    /// Sets the state of an Io @param Io 
    void setState(int state);

    /// Sets the saved state of an Io
    void setSavedState(int save);

    /// Sets an Io direction to output and sets its value to 1 (to the board)
    void setOutputOn();

    /// Sets an Io direction to output and sets its value to 0  (to the board)
    void setOutputOff();

    void setDirection(std::string direction);

    /// Getter of Jc object
    jtag_core *getJc() const { return mJc; };

    /// Pin getter @return Reference of the Pin
    Io *getPin();

    /// Publishes state of an Io @param Io : State of this Io is published
    int publishState();

    /// Publishes direction of an Io @param Io : State of this Io is published
    int publishDirection();

    /// Check the input of the board, this is for the thread
    void checkInput();

    /// Launched when a message arrive on the subscribed topic
    void message_arrived(mqtt::const_message_ptr msg);

    /// Send info when asked
    void sendInfo();

    /// Create an alternative thread
    std::shared_ptr<std::thread> createAlternativeThread() { return std::make_shared<std::thread>(&MetaDriverFT2232Io::checkInput, this); }

    /// Edits an Io in the vector @param name : Name of the Io to edit
    void editPin(std::string name);

private:

    std::shared_ptr<JtagFT2232> mJtagManager;
    jtag_core *mJc;
    std::string mProbeId;
    bool first_start = false;

    //IO VARIABLES

    std::string mPinName;
    int mId;
    int mState;
    std::string mDirection;
    int mSavedState;
    int mReadState;

    Json::Value mStatePayload;
    Json::Value mDirectionPayload;

    /// A Mutex to synchronize the publishes
    static std::mutex mPubMutex;
    static std::mutex mCheckInputMutex;
    static std::mutex mMessageMutex;

    std::thread *mAlternativeThread;
    bool mKillThread = false;
};

#endif