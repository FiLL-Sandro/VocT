project(Dictionary)

list(APPEND srcs
     Dictionary.cpp
     )

add_library(Dictionary SHARED
            ${srcs}
            )
target_include_directories(Dictionary INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           )

target_link_libraries(Dictionary
                      common
                      )
