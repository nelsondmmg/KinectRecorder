cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

project(KinectRecorder CXX)

set(sources

    main.cpp
    mainform.cpp
)

set(library_sources

    filecapture.cpp
    kinectcapture.cpp
    luxframe.hpp
    icapture.hpp
)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)
find_package(OpenCV REQUIRED)

add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Concurrent_DEFINITIONS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Concurrent_INCLUDE_DIRS})

set(headers mainform.hpp)
qt5_wrap_cpp(headers_moc ${headers})
qt5_wrap_ui(headers_ui ${ui_files})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Concurrent_EXECUTABLE_COMPILE_FLAGS}")

include(CheckCXXCompilerFlag)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  CHECK_CXX_COMPILER_FLAG("-std=c++11" STDCPP11_SUPPORTED)
  if(STDCPP11_SUPPORTED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" STDCPP0X_SUPPORTED)
    if(STDCPP0X_SUPPORTED)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
      message(FATAL_ERROR "Compiler doesn't appear to support C++11 or at least C++0x")
    endif()
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

add_library(kinectCapture SHARED ${library_sources})
add_executable(kinectRecorder ${sources} ${headers_moc} ${headers_ui})

target_link_libraries(
    kinectCapture

    ${OpenCV_LIBS}
)

target_link_libraries(
    kinectRecorder
    kinectCapture
    ${Qt5Concurrent_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
)
