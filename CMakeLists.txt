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

message(STATUS "Curl include dirs: ${CURL_INCLUDE_DIR}")
message(STATUS "CUrl libraries: ${CURL_LIBRARY}")
add_executable(FinanceManager main.cpp)
target_link_libraries(FinanceManager ${wxWidgets_LIBRARIES} CURL::libcurl onedrive)

