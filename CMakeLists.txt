cmake_minimum_required(VERSION 3.25)
project(xcleanup_test LANGUAGES C)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR})

ADD_CUSTOM_TARGET(leak-check
  COMMAND valgrind -s --leak-check=full --track-fds=yes ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
)

add_executable(${PROJECT_NAME}
    main.c
)
