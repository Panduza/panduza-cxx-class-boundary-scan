#ifndef _PLUGINENTRYPOINT_
#define _PLUGINENTRYPOINT_
#pragma once

#include <boost/config.hpp>
#include "/panduza-cxx-platform/src/meta_driver.hxx"

/// Entrypoint of the plugin template
class BOOST_SYMBOL_VISIBLE PluginEntrypoint
{
public:
    /// virtual function of the function that will return the driver name and factory of the plugin
    virtual std::map<std::string, MetaDriverFactory *> getInformationAndFactory() = 0;
};

#endif