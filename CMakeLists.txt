cmake_minimum_required(VERSION 3.2.3 FATAL_ERROR)
project(topdownthing)

find_package(PkgConfig REQUIRED)
pkg_search_module(SFML REQUIRED sfml-graphics)
pkg_search_module(SFML REQUIRED sfml-system)
pkg_search_module(SFML REQUIRED sfml-window)

pkg_searcH_module(CONFIG REQUIRED libconfig++)

include_directories(include)

set(CMAKE_CXX_STANDARD 14)

message("Enabling all warnings..")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(
	topdown
	src/main.cpp
	src/resource.cpp
	src/entity/player.cpp
	src/entity/enemy.cpp
)

target_link_libraries(topdown ${SFML_LIBRARIES} ${CONFIG_LIBRARIES})
