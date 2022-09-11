#include <iostream>
#include <boost/config.hpp>
#include <boost/dll/alias.hpp>
#include <boost/shared_ptr.hpp>
#include "entrypoint.hxx"
#include "src/meta_driver_FT2232_boundary_scan.hxx"

/// Entrypoint Boundary Scan Namespace
namespace boundary_scan
{
    /// Entrypoint for the boundary scan plugin
    class BoundaryscanEntrypoint : public PluginEntrypoint
    {
    public:
        /// return the driver name and factory of the plugin
        std::map<std::string, MetaDriverFactory *> getInformationAndFactory()
        {
            std::map<std::string, MetaDriverFactory *> Factory;
            Factory["Scan_service"] = new MetaDriverFactoryFT2232BoundaryScan();
            return Factory;
        }

        /// Create the entrypoint object for boundary scan
        static boost::shared_ptr<BoundaryscanEntrypoint> create()
        {
            return boost::shared_ptr<BoundaryscanEntrypoint>(
                new BoundaryscanEntrypoint()
            );
        }
    };

    BOOST_DLL_ALIAS(
        boundary_scan::BoundaryscanEntrypoint::create, // <-- this function is exported with...
        get_factory                                          // <-- ...this alias name
    )

}