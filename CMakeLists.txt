cmake_minimum_required(VERSION 3.7)
project(ModelLoader)

set(CMAKE_CXX_STANDARD 11)

# Source files
set(SOURCE_FILES
        src/main.cpp
        include/GLAD/src/glad.c
        src/graphics/shader.cpp
        src/graphics/camera.cpp
        src/graphics/mesh.cpp
        src/graphics/model.cpp
)

set(HEADER_FILES
        src/graphics/shader.h
        src/graphics/camera.h
        src/graphics/mesh.h
        src/graphics/model.h)

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir}/build)

include_directories(include)

# Include library files
include_directories(include/GLAD/include)
include_directories(include/glm/include)
include_directories(include/inih/include)
include_directories(include/assimp/include)

link_directories(${CMAKE_SOURCE_DIR}/include/assimp/lib)

# Copy assets and shaders to the build directory
file(COPY assets DESTINATION ${dir}/build)
file(COPY shaders DESTINATION ${dir}/build)

# Add dependencies
add_subdirectory(dependencies)

# Create executable
add_executable(ModelLoader ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(ModelLoader ${CMAKE_SOURCE_DIR}/include/assimp/lib/assimp-vc143-mtd.lib)
target_link_libraries(ModelLoader glfw)

