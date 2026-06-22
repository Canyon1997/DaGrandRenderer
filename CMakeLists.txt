cmake_minimum_required(VERSION 3.25)
project(DaGrandRenderer)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_executable(DaGrandRenderer
		DaGrandRenderer/Source/main.cpp
		DaGrandRenderer/Source/Shader.cpp
		DaGrandRenderer/Source/Texture.cpp
		DaGrandRenderer/Header/stb_image.h
		External/glad/src/glad.c
)

target_include_directories(DaGrandRenderer
		PRIVATE
		${CMAKE_SOURCE_DIR}/External/glad/include
		${CMAKE_SOURCE_DIR}/External/glfw/include
		${CMAKE_SOURCE_DIR}/DaGrandRenderer/Header
		${CMAKE_SOURCE_DIR}/External/glm
)

target_link_libraries(DaGrandRenderer
		PRIVATE
		${CMAKE_SOURCE_DIR}/External/glfw/lib/glfw3.lib
		OpenGL::GL
)

target_compile_definitions(DaGrandRenderer PRIVATE
	SHADER_ROOT="${CMAKE_SOURCE_DIR}/DaGrandRenderer/Shader/"
	TEXTURE_ROOT="${CMAKE_SOURCE_DIR}/DaGrandRenderer/Texture/"
)
