project(Menu)

list(APPEND srcs
     Menu.cpp
     SimpleListMenuImpl.cpp
     )

# list(APPEND extra_includes
#      ${PROJECT_SOURCE_DIR}/../common
#      )

add_library(Menu SHARED
            ${srcs}
            )
target_include_directories(Menu INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           )

target_link_libraries(Menu
                      common
                      )

# target_include_directories(Menu PUBLIC
#                            ${extra_includes}
#                            )
