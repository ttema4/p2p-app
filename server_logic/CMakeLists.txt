cmake_minimum_required(VERSION 3.29)

project(server_logic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

configure_file(config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)
configure_file(logs.txt ${CMAKE_BINARY_DIR}/logs.txt COPYONLY)

add_definitions(-DCONFIG_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\")
add_definitions(-DLOGS_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\")

find_package(Boost 1.65 REQUIRED)

if(NOT Boost_FOUND)
    include_directories(/opt/homebrew/opt/boost/include)
    link_directories(/opt/homebrew/opt/boost/lib)
endif()

add_executable(server_logic 
    logic_main.cpp
    logic_manager.cpp 
    data_processing.cpp 
    parser_connection_client.cpp 
    users_connections_server.cpp 
    json_tools.cpp
)

if(Boost_FOUND)
    target_link_libraries(server_logic PRIVATE Boost::boost)
endif()
