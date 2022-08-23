cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Scan LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -I/usr/include/jsoncpp")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

# specify sources
set(SOURCES 
	src/base64/base64.cxx
	src/ft2232_boundary_scan/jtag_manager.cxx
	entrypoint.cxx
	src/meta_driver_group_info.cxx
	src/meta_driver_FT2232_boundary_scan.cxx
	src/meta_driver_FT2232_bsdl_loader.cxx
	src/meta_driver_FT2232_io.cxx
	src/ft2232_boundary_scan/jtag_core/jtag_core.cxx
	src/ft2232_boundary_scan/jtag_core/script.cxx
	src/ft2232_boundary_scan/drivers/drv_loader.cxx
	src/ft2232_boundary_scan/drivers/ftdi_jtag/ftdi_jtag_drv.cxx
	src/ft2232_boundary_scan/bsdl_parser/bsdl_loader.cxx
	src/ft2232_boundary_scan/bsdl_parser/bsdl_strings.cxx
	src/ft2232_boundary_scan/io.cxx
	)

# set extra libraries needed
set(EXTRA_LIBS 
	dl
	ftd2xx 
	pthread
	boost_program_options
	boost_filesystem
	boost_system
	Metaplatform
)

# include mqtt libraries
set(MQTT_LIBRARIES
	paho-mqttpp3 
	paho-mqtt3a # Put back mqtt3as when possible
	paho-mqtt3c
)

# check for jsoncpp library
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

# add the executable
add_library(BoundaryScan SHARED ${SOURCES})

# link libraries with the executable
target_link_libraries(BoundaryScan PUBLIC ${EXTRA_LIBS} ${JSONCPP_LIBRARIES} ${MQTT_LIBRARIES})

# add the binary tree to the search path for include files
target_include_directories(BoundaryScan PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(BoundaryScan PUBLIC /usr/share/panduza-cxx/includes/paho.mqtt.c-src/src)
target_include_directories(BoundaryScan PUBLIC /usr/share/panduza-cxx/includes/paho.mqtt.cpp-src/src)

install(TARGETS BoundaryScan
		LIBRARY DESTINATION /usr/share/panduza-cxx/libraries
)