#ifndef _PLUGINENTRYPOINT_
#define _PLUGINENTRYPOINT_
#pragma once

#include <boost/config.hpp>
#include "meta_driver.hxx"

/// Entrypoint for the boundary scan plugin
class BOOST_SYMBOL_VISIBLE PluginEntrypoint
{
public:
    /// return the driver name and factory of the plugin @return Boundary Scan Factory information as a Map data
    virtual std::map<std::string, MetaDriverFactory *> getInformationAndFactory() = 0;
};

#endif