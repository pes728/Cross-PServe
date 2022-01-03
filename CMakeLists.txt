cmake_minimum_required(VERSION 3.8)
project(PServe VERSION 0.1.0)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()


include_directories(Pserve/include thirdParty/imgui thirdParty/imgui/backends thirdParty/glm)

file(GLOB SOURCES PServe/src/*.cpp 
thirdParty/imgui/imgui*.cpp 
thirdParty/imgui/backends/imgui_impl_opengl2.cpp 
thirdParty/imgui/backends/imgui_impl_glfw.cpp)

add_executable(PServe ${SOURCES})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(thirdParty/glfw-3.3.6)
target_link_libraries(PServe glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(PServe OpenGL::GL)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
