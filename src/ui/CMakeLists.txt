add_library(ui STATIC 
                account/add_account.cpp
                account/modify_account.cpp
                account/view_account.cpp
)

target_link_libraries(ui ${wxWidgets_LIBRARIES} CURL::libcurl)