#ifndef _METADRIVERGROUPINFO
#define _METADRIVERGROUPINFO
#pragma once

#include "/panduza-cxx-platform/src/meta_driver.hxx"
#include <jsoncpp/json/json.h>

/// Meta driver for group info
class MetaDriverGroupInfo : public MetaDriver
{
public:
    /// Constructor @param payload of the boundary scan info
    MetaDriverGroupInfo(Json::Value payload);

    /// Setup the meta driver
    void setup();

    /// Send info
    void sendInfo();

    /// launched when a message arrived on subscribed topics @param msg payload received with topic
    void message_arrived(mqtt::const_message_ptr msg);

private:
    Json::Value mDataPayload;

};

#endif