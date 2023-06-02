cmake_minimum_required(VERSION 3.10)
project(VocT
        VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# list(APPEND extra_libs
#      common
#      )

# list(APPEND extra_includes
#      ${PROJECT_BINARY_DIR}
#      ${PROJECT_SOURCE_DIR}/../common
#      )

add_subdirectory(common)
add_subdirectory(Menu)
