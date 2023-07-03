cmake_minimum_required(VERSION 3.10)
project(VocT
        VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Werror
                    )

add_subdirectory(common)
add_subdirectory(Receiver)
# add_subdirectory(Menu)
# add_subdirectory(src)
