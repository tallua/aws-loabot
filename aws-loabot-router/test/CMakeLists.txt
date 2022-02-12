cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(CMAKE_CXX_FLAGS -pthread)
project(aws-loabot-router-test)



## dependency: gtest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG origin/main
)
FetchContent_MakeAvailable(googletest)

# project
enable_testing()

add_executable(${PROJECT_NAME}
    ../src/validate.cpp
    ../src/message.cpp
    ../src/router.cpp
    ../src/loabot_router.cpp
    ./test_main.cpp
    ./test_router.cpp
    ./test_validate.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ../include)
target_link_libraries(${PROJECT_NAME}
    PRIVATE AWS::aws-lambda-runtime
    PRIVATE ${AWSSDK_LINK_LIBRARIES}
    PRIVATE sodium
    PRIVATE gtest
)

target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_17")
target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Wall -Werror -Wextra)

install(TARGETS ${PROJECT_NAME})

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})