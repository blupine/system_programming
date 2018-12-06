cmake_minimum_required (VERSION 2.8)
 
set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/${CMAKE_MODULE_PATH}")
find_package(raspicam REQUIRED)
 
add_executable (main main.cpp)
 
target_link_libraries (main ${raspicam_LIBS})


