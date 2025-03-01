add_library(onedrive STATIC onedrive.cpp)

target_link_libraries(onedrive ${wxWidgets_LIBRARIES} CURL::libcurl)