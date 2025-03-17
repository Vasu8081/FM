add_library(cloud_storage STATIC onedrive.cpp)

target_link_libraries(cloud_storage ${wxWidgets_LIBRARIES} CURL::libcurl)