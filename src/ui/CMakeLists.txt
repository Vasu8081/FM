add_library(ui STATIC 
                add_model.cpp
)

target_link_libraries(ui ${wxWidgets_LIBRARIES} CURL::libcurl)