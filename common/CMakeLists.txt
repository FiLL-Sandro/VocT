project(common)

list(APPEND srcs
     Log.cpp
     utility.cpp
     _thread.cpp
     )

add_library(common SHARED
            ${srcs}
            )
target_include_directories(common INTERFACE
                           ${CMAKE_CURRENT_SOURCE_DIR}
                           )
