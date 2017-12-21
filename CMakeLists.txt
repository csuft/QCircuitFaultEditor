CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(QCircuitEditor CXX)

################################################################################
# Compiler and linker options
ADD_DEFINITIONS(-Wall -DWIN32_LEAN_AND_MEAN -DUNICODE)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON) 
SET(CMAKE_CXX_STANDARD 11) 

################################################################################
# Sources and libraries
FILE(GLOB THIRDPARTY "${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/*")
FILE(GLOB CONTROLS "${CMAKE_CURRENT_SOURCE_DIR}/src/controls/*")
FILE(GLOB EDITOR "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/*")
FILE(GLOB MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main/*")
FILE(GLOB UTILS "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*") 
 
SET(RESOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/images/apprc.rc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/circuitfaulteditor.qrc) 


FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Xml Gui Sql Network PrintSupport REQUIRED)  
  
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}) 
################################################################################
# Compile target
ADD_EXECUTABLE ( ${PROJECT_NAME} 
				${THIRDPARTY} 
				${CONTROLS} 
				${EDITOR}
				${MAIN}
				${UTILS}
				${RESOURCES})
# https://www.kdab.com/using-cmake-with-qt-5/
qt5_use_modules(${PROJECT_NAME} Core Widgets Gui Xml Sql Network PrintSupport)  