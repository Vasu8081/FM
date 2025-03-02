add_library(models STATIC transactions.cpp)

target_link_libraries(models ${wxWidgets_LIBRARIES} CURL::libcurl)