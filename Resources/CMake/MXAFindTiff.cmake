
# - Find TIFF library
# - Derived from the FindTiff.cmake that is included with cmake
# Find the native TIFF includes and library
# This module defines
#  TIFF_INCLUDE_DIR, where to find tiff.h, etc.
#  TIFF_LIBRARIES, libraries to link against to use TIFF.
#  TIFF_FOUND, If false, do not try to use TIFF.
# also defined, but not for general use are
#  TIFF_LIBRARY, where to find the TIFF library.
#  TIFF_LIBRARY_DEBUG - Debug version of tiff library
#  TIFF_LIBRARY_RELEASE - Release Version of tiff library

# MESSAGE (STATUS "Finding Tiff library and headers..." )

# Look for the header file.
SET(TIFF_INCLUDE_SEARCH_DIRS
  $ENV{TIFF_INSTALL}/include
)

SET (TIFF_LIB_SEARCH_DIRS
  $ENV{TIFF_INSTALL}/lib
  )

SET (TIFF_BIN_SEARCH_DIRS
  $ENV{TIFF_INSTALL}/bin
)

FIND_PATH(TIFF_INCLUDE_DIR 
  NAMES tiff.h
  PATHS ${TIFF_INCLUDE_SEARCH_DIRS} 
  NO_DEFAULT_PATH
)

IF (WIN32 AND NOT MINGW)
    SET (TIFF_SEARCH_DEBUG_NAMES "tiffdll_D;libtiff_D")
    SET (TIFF_SEARCH_RELEASE_NAMES "tiffdll;libtiff")
ELSE (WIN32 AND NOT MINGW)
    SET (TIFF_SEARCH_DEBUG_NAMES "tiff_debug")
    SET (TIFF_SEARCH_RELEASE_NAMES "tiff")
ENDIF(WIN32 AND NOT MINGW)

# Look for the library.
FIND_LIBRARY(TIFF_LIBRARY_DEBUG 
  NAMES ${TIFF_SEARCH_DEBUG_NAMES}
  PATHS ${TIFF_LIB_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )
  
FIND_LIBRARY(TIFF_LIBRARY_RELEASE 
  NAMES ${TIFF_SEARCH_RELEASE_NAMES}
  PATHS ${TIFF_LIB_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )

SET (TIFF_DUMP_PROG_NAME "tiffdump")
IF (WIN32)
    SET (TIFF_DUMP_PROG_NAME "tiffdump.exe")
ENDIF(WIN32)

FIND_PROGRAM(TIFF_DUMP_PROG
    NAMES ${TIFF_DUMP_PROG_NAME}
    PATHS ${TIFF_BIN_SEARCH_DIRS} 
    NO_DEFAULT_PATH
)
MARK_AS_ADVANCED(TIFF_DUMP_PROG)

 #MESSAGE(STATUS "TIFF_INCLUDE_DIR: ${TIFF_INCLUDE_DIR}")
 #MESSAGE(STATUS "TIFF_LIBRARY_DEBUG: ${TIFF_LIBRARY_DEBUG}")
 #MESSAGE(STATUS "TIFF_LIBRARY_RELEASE: ${TIFF_LIBRARY_RELEASE}")
 #MESSAGE(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

IF (NOT MXA_RESOURCES_DIR)
  GET_FILENAME_COMPONENT(MXA_RESOURCES_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
ENDIF (NOT MXA_RESOURCES_DIR)

# include the macro to adjust libraries
INCLUDE (${MXA_CMAKE_DIR}/MXAAdjustLibVars.cmake) 
_MXA_ADJUST_LIB_VARS(TIFF)

IF(TIFF_INCLUDE_DIR AND TIFF_LIBRARY)
  SET(TIFF_FOUND 1)
  SET(TIFF_LIBRARIES ${TIFF_LIBRARY})
  SET(TIFF_INCLUDE_DIRS ${TIFF_INCLUDE_DIR})
  IF (TIFF_LIBRARY_DEBUG)
    GET_FILENAME_COMPONENT(TIFF_LIBRARY_PATH ${TIFF_LIBRARY_DEBUG} PATH)
    SET(TIFF_LIB_DIR  ${TIFF_LIBRARY_PATH})
  ELSEIF(TIFF_LIBRARY_RELEASE)
    GET_FILENAME_COMPONENT(TIFF_LIBRARY_PATH ${TIFF_LIBRARY_RELEASE} PATH)
    SET(TIFF_LIB_DIR  ${TIFF_LIBRARY_PATH})
  ENDIF(TIFF_LIBRARY_DEBUG)
  
  IF (TIFF_DUMP_PROG)
    GET_FILENAME_COMPONENT(TIFF_BIN_PATH ${TIFF_DUMP_PROG} PATH)
    SET(TIFF_BIN_DIR  ${TIFF_BIN_PATH})
  ENDIF (TIFF_DUMP_PROG)
ELSE(TIFF_INCLUDE_DIR AND TIFF_LIBRARY)
  SET(TIFF_FOUND 0)
  SET(TIFF_LIBRARIES)
  SET(TIFF_INCLUDE_DIRS)
ENDIF(TIFF_INCLUDE_DIR AND TIFF_LIBRARY)

# Report the results.
IF(NOT TIFF_FOUND)
  SET(TIFF_DIR_MESSAGE
    "Tiff was not found. Make sure TIFF_LIBRARY and TIFF_INCLUDE_DIR are set or set the TIIF_INSTALL environment variable.")
  IF(NOT TIFF_FIND_QUIETLY)
    MESSAGE(STATUS "${TIFF_DIR_MESSAGE}")
  ELSE(NOT TIFF_FIND_QUIETLY)
    IF(TIFF_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Tiff was NOT found and is Required by this project")
    ENDIF(TIFF_FIND_REQUIRED)
  ENDIF(NOT TIFF_FIND_QUIETLY)
ENDIF(NOT TIFF_FOUND)

IF (TIFF_FOUND)
  INCLUDE(CheckSymbolExists)
  #############################################
  # Find out if TIFF was build using dll's
  #############################################
  # Save required variable
  SET(CMAKE_REQUIRED_INCLUDES_SAVE ${CMAKE_REQUIRED_INCLUDES})
  SET(CMAKE_REQUIRED_FLAGS_SAVE    ${CMAKE_REQUIRED_FLAGS})
  # Add TIFF_INCLUDE_DIR to CMAKE_REQUIRED_INCLUDES
  SET(CMAKE_REQUIRED_INCLUDES "${CMAKE_REQUIRED_INCLUDES};${TIFF_INCLUDE_DIRS}")

  CHECK_SYMBOL_EXISTS(TIFF_BUILT_AS_DYNAMIC_LIB "tif_config.h" HAVE_TIFF_DLL)

    IF (HAVE_TIFF_DLL STREQUAL "TRUE")
        SET (HAVE_TIFF_DLL "1")
    ENDIF (HAVE_TIFF_DLL STREQUAL "TRUE")

  # Restore CMAKE_REQUIRED_INCLUDES and CMAKE_REQUIRED_FLAGS variables
  SET(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES_SAVE})
  SET(CMAKE_REQUIRED_FLAGS    ${CMAKE_REQUIRED_FLAGS_SAVE})
  #
  #############################################
ENDIF (TIFF_FOUND)
