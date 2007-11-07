# - Find expat
# Find the native EXPAT headers and libraries.
#
#  EXPAT_INCLUDE_DIRS - where to find expat.h, etc.
#  EXPAT_LIBRARIES    - List of libraries when using expat.
#  EXPAT_FOUND        - True if expat found.

# Look for the header file.
SET(EXPAT_INCLUDE_SEARCH_DIRS
  $ENV{EXPAT_INSTALL}/include
)

SET (EXPAT_LIB_SEARCH_DIRS
  $ENV{EXPAT_INSTALL}/lib
  )

FIND_PATH(EXPAT_INCLUDE_DIR 
  NAMES expat.h
  PATHS ${EXPAT_INCLUDE_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )
  
# MARK_AS_ADVANCED(EXPAT_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(EXPAT_LIBRARY 
  NAMES expat 
  PATHS ${EXPAT_LIB_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )
# MARK_AS_ADVANCED(EXPAT_LIBRARY)

# Copy the results to the output variables.
IF(EXPAT_INCLUDE_DIR AND EXPAT_LIBRARY)
  SET(EXPAT_FOUND 1)
  SET(EXPAT_LIBRARIES ${EXPAT_LIBRARY})
  SET(EXPAT_INCLUDE_DIRS ${EXPAT_INCLUDE_DIR})
ELSE(EXPAT_INCLUDE_DIR AND EXPAT_LIBRARY)
  SET(EXPAT_FOUND 0)
  SET(EXPAT_LIBRARIES)
  SET(EXPAT_INCLUDE_DIRS)
ENDIF(EXPAT_INCLUDE_DIR AND EXPAT_LIBRARY)

# Report the results.
IF(NOT EXPAT_FOUND)
  SET(EXPAT_DIR_MESSAGE
    "EXPAT was not found. Make sure EXPAT_LIBRARY and EXPAT_INCLUDE_DIR are set.")
  IF(NOT EXPAT_FIND_QUIETLY)
    MESSAGE(STATUS "${EXPAT_DIR_MESSAGE}")
  ELSE(NOT EXPAT_FIND_QUIETLY)
    IF(EXPAT_FIND_REQUIRED)
      # MESSAGE(FATAL_ERROR "${EXPAT_DIR_MESSAGE}")
      MESSAGE(FATAL_ERROR " WTF. Could not file expat")
    ENDIF(EXPAT_FIND_REQUIRED)
  ENDIF(NOT EXPAT_FIND_QUIETLY)
ENDIF(NOT EXPAT_FOUND)
