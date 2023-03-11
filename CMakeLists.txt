cmake_minimum_required(VERSION 3.3)
project(GravSimulation)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(INC ./include)
set(SRC ./src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -fpermissive")
set(SOURCE_FILES ${INC}/glmatrix.h ${SRC}/glmatrix.c)

include_directories(${INC})

## libs
#add_library(FREE_GLUT_LIB STATIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/libfreeglut_static.a)

# exe
add_executable(GravSimulation ${SRC}/shapes.c ${SOURCE_FILES})

# linking
target_link_libraries(GravSimulation ${CMAKE_CURRENT_SOURCE_DIR}/lib/libfreeglut_static.a)
