cmake_minimum_required(VERSION 3.7)
PROJECT(vivig)



# Build configs
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# OpenGL
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL included.")

# GLM
add_subdirectory(lib/glm/glm)
include_directories("${CMAKE_SOURCE_DIR}/lib/glm")

# GLFW
add_subdirectory(lib/glfw)
target_link_libraries(vivig glfw)
target_link_libraries(vivig OpenGL::GL)

# GLEW (static)
include_directories("${PROJECT_SOURCE_DIR}/lib/glew/include")

# DEFINE GLEW_STATIC in cmake VS solution preprocessor definitions
# add below output to VS solution linker library directories
message(STATUS "${CMAKE_SOURCE_DIR}/lib/glew/lib/Release/x64")

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
)
