cmake_minimum_required(VERSION 2.8)

project(Object_detection_project)

set(OpenCV_DIR "C:\\opencv412_mieu\\build")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core SerialPort Network Widgets)

# Qt Flags
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Instruct CMake to get the resources
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME}
   src/main.cpp
   src/ui/widget.h
   src/ui/widget.cpp
   src/ui/widget.ui
   src/opencvworker/opencvworker.h
   src/opencvworker/opencvworker.cpp
    )


include_directories(${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/opencvworker
    )

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Core Threads::Threads ${OpenCV_LIBS})


# Define required c++ standard to C++11
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# set build type to Debug/Release
set(CMAKE_BUILD_TYPE "Debug")

# Set compile options, enable warnings
target_compile_options(${PROJECT_NAME} PRIVATE
$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: -Wall>
$<$<CXX_COMPILER_ID:MSVC>: /W3>
)
