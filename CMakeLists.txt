cmake_minimum_required(VERSION 3.16)
project(WhaleWatch)

set(CMAKE_CXX_STANDARD 17)

# vcpkg toolchain integration
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(whale-watch ${SOURCES})

find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(whale-watch PRIVATE cpr::cpr nlohmann_json::nlohmann_json)
