cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(KinectRecorder CXX)

set(sources

	main.cpp
	mainform.cpp
)

set(library_sources

	filecapture.cpp
	icapture.cpp
	kinectcapture.cpp
	luxframe.cpp

)


find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL QtXml QtTest)
include(${QT_USE_FILE})

find_package(OpenCV REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# append this library's includes for use in higher-level directories
if (DEFINED GLOBAL_LIBS)
	set(GLOBAL_LIBS ${GLOBAL_LIBS} kinectCapture; PARENT_SCOPE)
endif()

set(headers mainform.hpp)
qt4_wrap_cpp(headers_moc ${headers})
qt4_wrap_ui(headers_ui ${ui_files})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-std=c++11")

add_library(kinectCapture SHARED ${library_sources})

add_executable(kinectRecorder ${sources} ${headers_moc} ${headers_ui})

target_link_libraries(
	kinectCapture

	${QT_LIBRARIES}
	${OpenCV_LIBS}
)

target_link_libraries(kinectRecorder kinectCapture)
