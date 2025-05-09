cmake_minimum_required(VERSION 4.0)
project(FinanceManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(BUILD_SHARED_LIBS OFF)

cmake_policy(SET CMP0074 NEW)
add_compile_options(/utf-8 /bigobj)



find_package(wxWidgets REQUIRED COMPONENTS core base aui)
find_package(ZLIB REQUIRED)

# set(CURL_USE_LIBIDN2 OFF CACHE BOOL "" FORCE)
# set(CURL_USE_LIBSSH2 OFF CACHE BOOL "" FORCE)
# set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
# set(USE_NGHTTP2 OFF CACHE BOOL "" FORCE)

# add_subdirectory(libraries/curl)

find_package(Sodium REQUIRED)


set(CPR_BUILD_TESTS OFF)
set(CPR_BUILD_EXAMPLES OFF)
set(CPR_BUILD_PACKAGE OFF)
add_subdirectory(libraries/cpr)



include(${wxWidgets_USE_FILE})
include_directories(
    ${CMAKE_BINARY_DIR}/_deps/curl-src/include
    ${CMAKE_SOURCE_DIR}/libraries/json/include
    ${CMAKE_SOURCE_DIR}/libraries/asio/include
    ${CMAKE_SOURCE_DIR}/libraries/crow/include
    ${wxWidgets_INCLUDE_DIRS}
    ${SODIUM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/libraries/cpr/include
)

add_subdirectory(src)

if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

if(WIN32)
    add_executable(FinanceManager main.cpp resources.rc)
elseif(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE "FinanceManager.icns")
    set_source_files_properties(${CMAKE_SOURCE_DIR}/resources/FinanceManager.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable(FinanceManager MACOSX_BUNDLE main.cpp resources.rc)
    target_sources(FinanceManager PRIVATE ${CMAKE_SOURCE_DIR}/resources/FinanceManager.icns)
else()
    add_executable(FinanceManager main.cpp)
endif()

target_link_libraries(FinanceManager ${wxWidgets_LIBRARIES} ${SODIUM_LIBRARIES} cpr cloud_storage icons models ui)

add_custom_command(TARGET FinanceManager POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SODIUM_LIB_DIR}/libsodium.dll"
        $<TARGET_FILE_DIR:FinanceManager>/libsodium.dll
)