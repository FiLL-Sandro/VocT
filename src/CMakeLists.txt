project(src)

add_executable(VocT
               main.cpp
               )
target_include_directories(VocT INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           )

target_link_libraries(VocT
                      common
                      Menu
                      Receiver
                      Dictionary
                      )
