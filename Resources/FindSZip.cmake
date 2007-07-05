# - Find szlib
# Find the native SZLIB includes and library
#
#  SZLIB_INCLUDE_DIR - where to find szlib.h, etc.
#  SZLIB_LIBRARIES   - List of libraries when using szlib.
#  SZLIB_FOUND       - True if szlib found.


IF (SZLIB_INCLUDE_DIR)
  # Already in cache, be silent
  SET(SZLIB_FIND_QUIETLY TRUE)
ENDIF (SZLIB_INCLUDE_DIR)

FIND_PATH(SZLIB_INCLUDE_DIR szlib.h
  /usr/local/include
  /usr/include
)

SET(SZLIB_NAMES z szlib zdll)
FIND_LIBRARY(SZLIB_LIBRARY
  NAMES ${SZLIB_NAMES}
  PATHS /usr/lib /usr/local/lib
)

IF (SZLIB_INCLUDE_DIR AND SZLIB_LIBRARY)
   SET(SZLIB_FOUND TRUE)
    SET( SZLIB_LIBRARIES ${SZLIB_LIBRARY} )
ELSE (SZLIB_INCLUDE_DIR AND SZLIB_LIBRARY)
   SET(SZLIB_FOUND FALSE)
   SET( SZLIB_LIBRARIES )
ENDIF (SZLIB_INCLUDE_DIR AND SZLIB_LIBRARY)

IF (SZLIB_FOUND)
   IF (NOT SZLIB_FIND_QUIETLY)
      MESSAGE(STATUS "Found SZLIB: ${SZLIB_LIBRARY}")
   ENDIF (NOT SZLIB_FIND_QUIETLY)
ELSE (SZLIB_FOUND)
   IF (SZLIB_FIND_REQUIRED)
      MESSAGE(STATUS "Looked for Z libraries named ${SZLIBS_NAMES}.")
      MESSAGE(FATAL_ERROR "Could NOT find z library")
   ENDIF (SZLIB_FIND_REQUIRED)
ENDIF (SZLIB_FOUND)

MARK_AS_ADVANCED(
  SZLIB_LIBRARY
  SZLIB_INCLUDE_DIR
  )
