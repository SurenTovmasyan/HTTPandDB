cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE main.cpp src/Config.cpp ~/Work/HTTP\ and\ DB\/general/src/User.cpp src/HTTP_Client.cpp)

include_directories(libs/cpp-httplib inlude)

add_executable(${PROJECT_NAME} ${SOURCE})

find_package(nlohmann_json CONFIG REQUIRED)