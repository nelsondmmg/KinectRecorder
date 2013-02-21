cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(KinectRecorder CXX)

set(sources

	filecapture.cpp
	icapture.cpp
	kinectcapture.cpp
	luxframe.cpp
	main.cpp
	mainform.cpp
	mainwindow.cpp
)

set(ui_files mainwindow.ui)

find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL QtXml QtTest)
include(${QT_USE_FILE})

find_package(OpenCV REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/algorithms/)

file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
qt4_wrap_cpp(headers_moc ${headers})
qt4_wrap_ui(headers_ui ${ui_files})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-std=c++11")

#add_subdirectory(algorithms)

add_executable(kinectRecorder ${sources} ${headers_moc} ${headers_ui})

target_link_libraries(
	kinectRecorder

	${QT_LIBRARIES}
	${OpenCV_LIBS}
)
