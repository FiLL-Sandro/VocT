project(Receiver)

list(APPEND srcs
     Receiver.cpp
     )

add_library(Receiver SHARED
            ${srcs}
            )
target_include_directories(Receiver INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           )

target_link_libraries(Receiver
                      common
                      )
