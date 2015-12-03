
cmake_minimum_required(VERSION 3.0)

if(NOT CONFIGURED_ONCE)
	set(CMAKE_BUILD_TYPE  "Debug"  CACHE STRING "Debug Release RelWithDebInfo MinSizeRel" FORCE)
	set(BUILD_CPU_ARCH    "x64"    CACHE STRING "x86 x64") # x64 by default UNIX, x86 by default in WIN32 !
	set(BUILD_SHARED_LIBS "SHARED" CACHE STRING "STATIC SHARED")
endif()

if( NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND
	NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release" AND
	NOT ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo" AND
	NOT ${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
	message(FATAL_ERROR "Bad CMAKE_BUILD_TYPE variable value (Debug Release RelWithDebInfo MinSizeRel)")
endif()

if( NOT ${BUILD_CPU_ARCH} STREQUAL "x86" AND
	NOT ${BUILD_CPU_ARCH} STREQUAL "x64")
	message(FATAL_ERROR "Bad BUILD_CPU_ARCH variable value (x86 x64)")
endif()

if( NOT ${BUILD_SHARED_LIBS} STREQUAL "STATIC" AND
	NOT ${BUILD_SHARED_LIBS} STREQUAL "SHARED")
	message(FATAL_ERROR "Bad BUILD_SHARED_LIBS variable value (STATIC SHARED)")
endif()

message(STATUS "CMAKE_BUILD_CONFIG: ${CMAKE_BUILD_TYPE} (${BUILD_CPU_ARCH}) - ${BUILD_SHARED_LIBS} Lib")

# ----------------------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS         "-Wall -Werror -fmax-errors=5 -std=c++11 -DGLFW_DLL")
set(CMAKE_CXX_FLAGS_DEBUG   "-gdwarf-2 -O2 -DDEBUG_ON") # -g
set(CMAKE_CXX_FLAGS_RELEASE "-s -O2")

# API Visibility in Lib
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}_${BUILD_CPU_ARCH}/lib")
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

set(LIBRARY_NAME JuEngine)
file(GLOB_RECURSE ENGINE_SRC_INL JuEngine/*.inl)
file(GLOB_RECURSE ENGINE_SRC JuEngine/*.*pp)

if(${BUILD_CPU_ARCH} STREQUAL "x64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DJUENGINE_COMPILE_DLL")

if(UNIX)
	set(LINK_FLAGS
		"-lGL" # OpenGL Lib
		#"-static-libgcc"
		#"-static-libstdc++"
		"-Wl,-rpath,./Libraries"
	)

	set(GLFW_PATH "/home/judelco/Projects/Libraries/glfw-3.1.2")
	set(GLEW_PATH "/home/judelco/Projects/Libraries/glew-1.13.0")
	set(GLM_PATH "/home/judelco/Projects/Libraries/glm-0.9.7.1")
	set(BULLET3_PATH "/home/judelco/Projects/Libraries/bullet3-20151125")
	set(ASSIMP_PATH "/home/judelco/Projects/Libraries/assimp-3.1.1_20151125")
	set(ATB_PATH "/home/judelco/Projects/Libraries/AntTweakBar-1.1.6")
elseif(WIN32)
	set(LINK_FLAGS "-lopengl32")

	if("${BUILD_CPU_ARCH}" STREQUAL "x86")
		set(LINK_FLAGS "${LINK_FLAGS}         -lwinmm           -static-libgcc -static-libstdc++")
	else() # x64
		set(LINK_FLAGS "${LINK_FLAGS} -lglu32 -lwinmm -lmingw32 -static-libgcc -static-libstdc++")
	endif()

	set(GLFW_PATH "C:/Programacion/CppLibs/GLFW-3.1.1-${BUILD_CPU_ARCH}")
	set(GLEW_PATH "C:/Programacion/CppLibs/GLEW-1.12.0-${BUILD_CPU_ARCH}")
	set(GLM_PATH "C:/Programacion/CppLibs/GLM-0.9.6.1")
	set(BULLET3_PATH "C:/Programacion/CppLibs/Bullet-3_20140417-${BUILD_CPU_ARCH}")
	set(ASSIMP_PATH "C:/Programacion/CppLibs/Assimp-3.1.1_20150222-${BUILD_CPU_ARCH}")
	set(ATB_PATH "C:/Programacion/CppLibs/AntTweakBar-1.16-${BUILD_CPU_ARCH}")
endif()

# ----------------------------------------------------------------------------------------------

project(JuEngine CXX)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${GLFW_PATH}/include
	${GLEW_PATH}/include
	${GLM_PATH}/include
	${BULLET3_PATH}/include
	${ASSIMP_PATH}/include
	${ATB_PATH}/include
)

link_directories(
	${GLFW_PATH}/lib
	${GLEW_PATH}/lib
	${BULLET3_PATH}/lib
	${ASSIMP_PATH}/lib
	${ATB_PATH}/lib
)

add_library(${LIBRARY_NAME} ${BUILD_SHARED_LIBS}
	${ENGINE_SRC_INL}
	${ENGINE_SRC}
)

# Unset 'lib' prefix
# set_target_properties(${LIBRARY_NAME} PROPERTIES PREFIX "")

target_link_libraries(${LIBRARY_NAME}
	glfw
	${LINK_FLAGS}
	GLEW
	assimp
	LinearMath BulletCollision BulletDynamics # Bullet3 Libs
	AntTweakBar
)

# ----------------------------------------------------------------------------------------------

#install(TARGETS ${LIBRARY_NAME} DESTINATION "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}_${BUILD_CPU_ARCH}/lib")

macro(INSTALL_HEADERS HEADER_LIST)
	foreach(HEADER ${${HEADER_LIST}})
		string(REGEX MATCH "(.*)[/\\]" HEADER_PATH ${HEADER})
		string(REGEX REPLACE "${CMAKE_SOURCE_DIR}" "" HEADER_PATH ${HEADER_PATH})
		install(FILES ${HEADER} DESTINATION "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}_${BUILD_CPU_ARCH}/include/${HEADER_PATH}")
	endforeach(HEADER)
endmacro(INSTALL_HEADERS)

file(GLOB_RECURSE ENGINE_SRC_HEADERS JuEngine/*.hpp)

INSTALL_HEADERS(ENGINE_SRC_HEADERS)
INSTALL_HEADERS(ENGINE_SRC_INL)

# ----------------------------------------------------------------------------------------------

set(CONFIGURED_ONCE TRUE CACHE INTERNAL "Flag - CMake has configured at least once")
