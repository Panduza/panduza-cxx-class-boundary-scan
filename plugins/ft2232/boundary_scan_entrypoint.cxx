#include <iostream>
#include <boost/config.hpp>
#include <boost/dll/alias.hpp>
#include <boost/shared_ptr.hpp>
#include "../../headers/entrypoint.hxx"
#include "src/meta_driver_FT2232_boundary_scan.hxx"

namespace boundary_scan
{
    
    class BoundaryscanEntrypoint : public PluginEntrypoint
    {
    public:
        /// @brief Function to return the information and the factory of the Boundary Scan plugin
        /// @return Name of the driver and object of the Boundary Scan factory as a map
        std::map<std::string, MetaDriverFactory *> getInformationAndFactory()
        {
            std::map<std::string, MetaDriverFactory *> Factory;
            Factory["Scan_service"] = new MetaDriverFactoryFT2232BoundaryScan();
            return Factory;
        }

        /// Create the entrypoint object for boundary scan @return Shared pointer of the Entrypoint class
        static boost::shared_ptr<BoundaryscanEntrypoint> create()
        {
            return boost::shared_ptr<BoundaryscanEntrypoint>(
                new BoundaryscanEntrypoint()
            );
        }
    };

    BOOST_DLL_ALIAS(
        boundary_scan::BoundaryscanEntrypoint::create,
        get_factory
    )
}