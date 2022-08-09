#include <iostream>
#include <boost/config.hpp>
#include <boost/dll/alias.hpp>
#include <boost/shared_ptr.hpp>
#include "entrypoint.hxx"
#include "meta_driver.hxx"
#include "src/meta_driver_FT2232_boundary_scan.hxx"

namespace boundary_scan
{

    class BoundaryscanEntrypoint : public Entrypoint
    {
    public:
        std::map<std::string, MetaDriverFactory *> getInformationAndFactory()
        {
            std::map<std::string, MetaDriverFactory *> Factory;
            Factory["Scan_service"] = new MetaDriverFactoryFT2232BoundaryScan();
            return Factory;
        }

        // Factory method
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