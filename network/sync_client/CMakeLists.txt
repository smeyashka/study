cmake_minimum_required(VERSION 3.18)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

set(Boost_USE_STATIC_LIBS OFF)

set(PROJECT_NAME crazy_client)

project(${PROJECT_NAME})

find_package(Boost COMPONENTS program_options system thread REQUIRED)
if(NOT Boost_FOUND)
	message(SEND_ERROR "Failed to find boost components.")
	return()
else()
	include_directories(${Boost_INCLUDE_DIRS})
endif()


set(SOURCE src/main.cpp src/client.cpp)


add_executable(
  ${PROJECT_NAME}
  ${SOURCE}
)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})


