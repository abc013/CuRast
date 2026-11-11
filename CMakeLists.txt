cmake_minimum_required(VERSION 3.18)

# --------------------------------------------------------
# # Main Project
# --------------------------------------------------------
set(TARGET_NAME CuRast)
project(${TARGET_NAME} LANGUAGES CXX CUDA)
set(CMAKE_CXX_STANDARD 23)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(cmake/common.cmake)

# --------------------------------------------------------
# ## Source files (C++ and CUDA)
# --------------------------------------------------------
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS 
	src/*.[ch]pp
	src/*.cu
	src/kernels/textureTools.cu
	src/kernels/lines.cu
)
add_executable(${TARGET_NAME} ${PROJECT_SOURCES})
if (MSVC)
	target_compile_options(${TARGET_NAME} PRIVATE
		$<$<COMPILE_LANGUAGE:CXX>:/Zc:preprocessor>
		$<$<COMPILE_LANGUAGE:CUDA>:-Xcompiler /Zc:preprocessor>
	)
endif()
target_compile_definitions(${TARGET_NAME} PUBLIC
	IMGUI_DEFINE_MATH_OPERATORS
	_HAS_STD_BYTE=0
)
if (WIN32)
	target_compile_definitions(${TARGET_NAME} PUBLIC
		VK_USE_PLATFORM_WIN32_KHR
		NOMINMAX
	)

	set_property(TARGET ${TARGET_NAME} PROPERTY
		VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
	)
	set_property(DIRECTORY PROPERTY
		VS_STARTUP_PROJECT ${TARGET_NAME}
	)
endif()

# Include paths
target_include_directories(${TARGET_NAME} PRIVATE
	./src
	./libs
)

set(TURBOJPEG_LIBRARY "${CMAKE_SOURCE_DIR}/libs/jpg/turbojpeg-static.lib")
add_subdirectory(libs/laszip)
add_subdirectory(libs/meshoptimizer)
target_link_libraries(${TARGET_NAME} PRIVATE 
	${TURBOJPEG_LIBRARY}
	laszip
	meshoptimizer
)

# Add other third-party libs
ADD_GLFW(${TARGET_NAME})
ADD_CUDA(${TARGET_NAME})
ADD_VULKAN(${TARGET_NAME})
ADD_GLM(${TARGET_NAME})
ADD_IMGUI(${TARGET_NAME})

# --------------------------------------------------------
# ## CUDA configuration
# --------------------------------------------------------
# Choose GPU architectures (adjust for your machine)
set_target_properties(${TARGET_NAME} PROPERTIES CUDA_ARCHITECTURES "86")

# Enable separable compilation (for multi-file CUDA builds)
set_target_properties(${TARGET_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

# --------------------------------------------------------
# # Second project
# --------------------------------------------------------
add_executable(PlyToGlb 
	src/tools/PlyToGlb.cpp
	src/unsuck_platform_specific.cpp
)
target_include_directories(PlyToGlb PRIVATE
	./src
	./libs
	./libs/glm
)