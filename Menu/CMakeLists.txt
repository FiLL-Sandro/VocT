project(Menu)

list(APPEND srcs
     Menu.cpp
     )

add_library(Menu SHARED
            ${srcs}
            )
target_include_directories(Menu INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           )

target_link_libraries(Menu
                      common
                      )
