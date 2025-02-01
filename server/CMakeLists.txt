cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES main.cpp src/Config.cpp src/DataBase.cpp ~/Work/HTTP\ and\ DB\/general/src/User.cpp src/HTTP_Server.cpp)

include_directories(libs/cpp-httplib /usr/include/mysql include)

link_directories(/usr/lib/mysql)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(server mysqlcppconn)