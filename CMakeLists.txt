cmake_minimum_required(VERSION 3.10)
project(FinanceManager)

set(CMAKE_CXX_STANDARD 17)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)
if (NOT wxWidgets_FOUND)
    message(FATAL_ERROR "wxWidgets not found. Please install wxWidgets and try again.")
endif()

include(${wxWidgets_USE_FILE})

include_directories(
    ${CMAKE_SOURCE_DIR}/libraries/json/include
    ${wxWidgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/include
)

add_subdirectory(src)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

if(WIN32)
    add_executable(FinanceManager main.cpp resources.rc)
elseif(APPLE)
    add_executable(FinanceManager MACOSX_BUNDLE main.cpp resources.rc)
else()
    add_executable(FinanceManager main.cpp)
endif()

message(STATUS "Curl include dirs: ${CURL_INCLUDE_DIR}")
message(STATUS "CUrl libraries: ${CURL_LIBRARY}")

target_link_libraries(FinanceManager ${wxWidgets_LIBRARIES} CURL::libcurl onedrive models)