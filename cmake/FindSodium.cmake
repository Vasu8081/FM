# Detect platform
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SODIUM_ARCH "x64")
    else()
        set(SODIUM_ARCH "win32")
    endif()
endif()

# Select Debug/Release based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SODIUM_BUILD "Debug")
else()
    set(SODIUM_BUILD "Release")
endif()

# Set toolchain (default to v143)
set(SODIUM_TOOLCHAIN "v143") # Change to v142 if needed

# Set linking type (choose dynamic or static)
set(SODIUM_LINKING "dynamic")  # Set to "dynamic" for shared library

# Define paths
set(SODIUM_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libraries/libsodium/include)
message(STATUS "Sodium include dir: ${SODIUM_INCLUDE_DIR}")
set(SODIUM_LIB_DIR ${CMAKE_SOURCE_DIR}/libraries/libsodium/${SODIUM_ARCH}/${SODIUM_BUILD}/${SODIUM_TOOLCHAIN}/${SODIUM_LINKING})
message(STATUS "Sodium lib dir: ${SODIUM_LIB_DIR}")

# Find the library (pointing to the directory, not the full file path)
find_library(SODIUM_LIB NAMES sodium PATHS ${SODIUM_LIB_DIR} NO_DEFAULT_PATH)

# Standard CMake package handling
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sodium DEFAULT_MSG SODIUM_INCLUDE_DIR SODIUM_LIB)

if(SODIUM_FOUND)
    set(SODIUM_LIBRARIES ${SODIUM_LIB})
    set(SODIUM_INCLUDE_DIRS ${SODIUM_INCLUDE_DIR})
endif()

mark_as_advanced(SODIUM_INCLUDE_DIR SODIUM_LIB)
