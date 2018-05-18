CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(LLTableVisualizer)

SET(CMAKE_CXX_STANDARD 17)

ADD_EXECUTABLE(LLTableVisualizer main.cpp)

IF (NOT TARGET LLTableBuilderLibrary)
	ADD_SUBDIRECTORY(LLTableBuilderLibrary)
ENDIF ()
IF (NOT TARGET TokenLibrary)
	ADD_SUBDIRECTORY(TokenLibrary)
ENDIF ()

TARGET_LINK_LIBRARIES(LLTableVisualizer LLTableBuilderLibrary TokenLibrary)
