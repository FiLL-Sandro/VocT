project(common)

list(APPEND srcs
     Log.cpp
     utility.cpp
     )

add_library(common SHARED
            ${srcs}
            )
