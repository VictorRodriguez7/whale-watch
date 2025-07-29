cmake_minimum_required(VERSION 3.16)
project(WhaleWatch)

set(CMAKE_CXX_STANDARD 17)

# Enable vcpkg toolchain integration if available
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Collect source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(whale-watch ${SOURCES})

# Find dependencies via vcpkg
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(PostgreSQL REQUIRED)  
find_package(libpqxx CONFIG REQUIRED)    
find_package(Boost REQUIRED COMPONENTS multiprecision)

# Include directories (vcpkg handles most includes automatically)
target_include_directories(whale-watch PRIVATE
    ${Boost_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(whale-watch PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
    PostgreSQL::PostgreSQL
    libpqxx::pqxx
    Boost::multiprecision
)
