cmake_minimum_required(VERSION 3.3.2)

project(md5sum C)

set(EXECUTABLE_NAME md5sum)

set(CMAKE_BUILD_TYPE Release)

file(GLOB DIR_SRCS src/*.c)

add_executable(${EXECUTABLE_NAME} ${DIR_SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC
                                           ${PROJECT_SOURCE_DIR}
                                           ${PROJECT_SOURCE_DIR}/src)

install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION bin)
