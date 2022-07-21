cmake_minimum_required(VERSION 3.12)
project(OpenGL LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
#head file
set(THIRD_PARTY_H ${PROJECT_SOURCE_DIR}/include)
include_directories(${THIRD_PARTY_H})

#link file
set(LIB_PATH ${PROJECT_SOURCE_DIR}/lib)
file(GLOB_RECURSE LIB_FILES
	"${LIB_PATH}/*.lib"
)
message("............ LIB_PATH: ${LIB_PATH}")
message("............ LIB_FILES: ${LIB_FILES}")
link_directories(${LIB_PATH})

aux_source_directory(./src DIR_SRCS)
add_executable(hello_tri ${DIR_SRCS})
target_link_libraries(hello_tri ${LIB_FILES})