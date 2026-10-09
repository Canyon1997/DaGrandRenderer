cmake_minimum_required(VERSION 3.25)
project(DaGrandRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(glad STATIC
	External/glad/src/glad.c
)

target_include_directories(glad PUBLIC
	${CMAKE_SOURCE_DIR}/External/glad/include
)

add_executable(DaGrandRenderer
		DaGrandRenderer/Source/main.cpp
		DaGrandRenderer/Source/Shader.cpp
		DaGrandRenderer/Source/Texture.cpp
		DaGrandRenderer/Header/stb_image.h
)

target_include_directories(DaGrandRenderer PRIVATE
		${CMAKE_SOURCE_DIR}/DaGrandRenderer/Header
)

target_link_libraries(DaGrandRenderer PRIVATE
		glad
		glfw
		glm::glm
		OpenGL::GL
)

target_compile_definitions(DaGrandRenderer PRIVATE
	SHADER_ROOT="${CMAKE_SOURCE_DIR}/DaGrandRenderer/Shader/"
	TEXTURE_ROOT="${CMAKE_SOURCE_DIR}/DaGrandRenderer/Texture/"
)
