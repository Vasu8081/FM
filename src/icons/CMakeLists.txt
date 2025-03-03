include(${wxWidgets_USE_FILE})

file(GLOB MAT_ICONS_SRC "*.cpp")

add_library(icons STATIC ${MAT_ICONS_SRC})

target_link_libraries(icons ${wxWidgets_LIBRARIES})
