cmake_minimum_required(VERSION 3.10)
project(FinanceManager)

set(CMAKE_CXX_STANDARD 20)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base aui)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)
find_package(cpr REQUIRED)
if (NOT wxWidgets_FOUND)
    message(FATAL_ERROR "wxWidgets not found. Please install wxWidgets and try again.")
endif()

include(${wxWidgets_USE_FILE})

include_directories(
    ${CMAKE_SOURCE_DIR}/libraries/json/include
    ${wxWidgets_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/libraries/cpr/include
    ${CMAKE_SOURCE_DIR}/libraries/asio/asio/include
    ${CMAKE_SOURCE_DIR}/libraries/Crow/include
)

add_subdirectory(src)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

if(WIN32)
    add_executable(FinanceManager main.cpp resources.rc)
elseif(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE "FinanceManager.icns")
    set_source_files_properties(${CMAKE_SOURCE_DIR}/FinanceManager.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(FinanceManager MACOSX_BUNDLE main.cpp resources.rc)
    target_sources(FinanceManager PRIVATE ${CMAKE_SOURCE_DIR}/FinanceManager.icns)
else()
    add_executable(FinanceManager main.cpp)
endif()

message(STATUS "Curl include dirs: ${CURL_INCLUDE_DIR}")
message(STATUS "CUrl libraries: ${CURL_LIBRARY}")

target_link_libraries(FinanceManager ${wxWidgets_LIBRARIES} CURL::libcurl cpr::cpr onedrive icons models ui)