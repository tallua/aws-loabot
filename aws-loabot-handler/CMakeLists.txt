cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
project(aws-loabot-handler)

## dependency: aws-lambda-runtime
find_package(aws-lambda-runtime)

## dependency: aws-sdk-cpp
find_package(AWSSDK REQUIRED COMPONENTS core)

## dependecy: gumbo
add_library(gumbo STATIC IMPORTED)
set_target_properties(gumbo PROPERTIES IMPORTED_LOCATION /usr/local/lib/libgumbo.a)
set_target_properties(gumbo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /usr/local/include/)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/loabot_handler.cpp
    src/loabot_fetch.cpp
    src/loabot_discord.cpp
    src/loabot_response.cpp
    src/discord_client.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ./include)
target_link_libraries(${PROJECT_NAME} PRIVATE AWS::aws-lambda-runtime ${AWSSDK_LINK_LIBRARIES} gumbo)
target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_17")
target_compile_options(${PROJECT_NAME} PRIVATE -pedantic -Wall -Werror -Wextra)

aws_lambda_package_target(${PROJECT_NAME} NO_LIBC)
