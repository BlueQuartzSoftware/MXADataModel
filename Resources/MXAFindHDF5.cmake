# This will find the HDF5 libraries and define the following variables:
#  HDF5_FOUND - Is the base HDF5 library and include file found
#  HDF5_INCLUDE_DIR - The Include directory containing all the HDF5 include files
#  HDF5_LIBRARY - The Actual HDF5 library
#  HDF5_HL_FOUND - Is the High Level HDF5 API Found
#  HDF5_HL_INCLUDE_DIR - The High Level Include Directory
#  HDF5_HL_LIBRARY - The Actual High Level Library
#  HDF5_USE_HIGH_LEVEL - Set this to TRUE if you need to link against the HDF5 High level APIs.
#  HDF5_INSTALL - This is an Environment variable that can be used to help find the HDF5 libraries and Include Directories
#  HDF5_LIBRARIES - The List of HDF5 libraries that were found. This variable can be used in a LINK_LIBRARIES(...) command
#  HDF5_DEBUG_LIBRARY - Debug Version of HDF5 library
#  HDF5_RELEASE_LIBRARY - Release Version of HDF5 library

MESSAGE(STATUS "Finding HDF5 Library... " )

#-- Clear the Library List 
SET (HDF5_LIBRARIES "")
#-- Clear the include Directories
SET (HDF5_INCLUDE_DIRS "")

# ------------------ START FIND HDF5 LIBS --------------------------------------
SET (HDF5_FOUND "NO")
SET (HDF5_HL_FOUND "NO")
SET (HDF5_INSTALL  $ENV{HDF5_INSTALL})

SET(HDF5_INCLUDE_SEARCH_DIRS
  $ENV{HDF5_INSTALL}/include
)

SET (HDF5_LIB_SEARCH_DIRS
  $ENV{HDF5_INSTALL}/lib
)

# -- Find the Include directory for HDF5
FIND_PATH(HDF5_INCLUDE_DIR 
  NAMES hdf5.h
  PATHS ${HDF5_INCLUDE_SEARCH_DIRS}
  NO_DEFAULT_PATH
)

IF (WIN32)
    SET (HDF5_SEARCH_DEBUG_NAMES "hdf5_D;libhdf5_D;hdf5ddll")
    SET (HDF5_SEARCH_RELEASE_NAMES "hdf5;libhdf5;hdf5dll")
ELSE (WIN32)
    SET (HDF5_SEARCH_DEBUG_NAMES "hdf5")
    SET (HDF5_SEARCH_RELEASE_NAMES "hdf5")
ENDIF(WIN32)

# Look for the library.
FIND_LIBRARY(HDF5_DEBUG_LIBRARY 
  NAMES ${HDF5_SEARCH_DEBUG_NAMES}
  PATHS ${HDF5_LIB_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )
  
FIND_LIBRARY(HDF5_RELEASE_LIBRARY 
  NAMES ${HDF5_SEARCH_RELEASE_NAMES}
  PATHS ${HDF5_LIB_SEARCH_DIRS} 
  NO_DEFAULT_PATH
  )
 
# MESSAGE(STATUS "HDF5_INCLUDE_SEARCH_DIRS: ${HDF5_INCLUDE_SEARCH_DIRS}")
# MESSAGE(STATUS "HDF5_LIB_SEARCH_DIRS: ${HDF5_LIB_SEARCH_DIRS}")
# MESSAGE(STATUS "HDF5_INCLUDE_DIR: ${HDF5_INCLUDE_DIR}")
# MESSAGE(STATUS "HDF5_DEBUG_LIBRARY: ${HDF5_DEBUG_LIBRARY}")
# MESSAGE(STATUS "HDF5_RELEASE_LIBRARY: ${HDF5_RELEASE_LIBRARY}")


IF ( CMAKE_BUILD_TYPE MATCHES Debug )
    SET (HDF5_LIBRARY ${HDF5_DEBUG_LIBRARY} )
ELSE ( CMAKE_BUILD_TYPE MATCHES Debug )
    SET (HDF5_LIBRARY ${HDF5_RELEASE_LIBRARY} )
ENDIF ( CMAKE_BUILD_TYPE MATCHES Debug )


IF (HDF5_INCLUDE_DIR)
  IF (HDF5_LIBRARY)
    SET (HDF5_FOUND "YES")
    SET (HDF5_LIBRARIES ${HDF5_LIBRARY} ${HDF5_LIBRARIES} )
    INCLUDE_DIRECTORIES( ${HDF5_INCLUDE_DIR} )
  ELSE (HDF5_LIBRARY)
    SET (HDF5_FOUND "NO")
  ENDIF (HDF5_LIBRARY)
ENDIF (HDF5_INCLUDE_DIR)


# ------------------ START FIND HDF5 HL LIBS -----------------------------------
IF ( HDF5_USE_HIGH_LEVEL )

  FIND_PATH(HDF5_HL_INCLUDE_DIR 
    NAMES H5LT.h
    PATHS ${HDF5_INCLUDE_SEARCH_DIRS}
    #  NO_DEFAULT_PATH
  )
  
  FIND_LIBRARY(HDF5_HL_LIBRARY 
    NAMES hdf5_hl
    PATHS ${HDF5_LIB_SEARCH_DIRS}
    #  NO_DEFAULT_PATH
  )

  IF (HDF5_HL_INCLUDE_DIR )
    IF (HDF5_HL_LIBRARY)
      SET (HDF5_HL_FOUND "YES")
      SET (HDF5_LIBRARIES ${HDF5_HL_LIBRARY} ${HDF5_LIBRARIES} )
      INCLUDE_DIRECTORIES( ${HDF5_HL_INCLUDE_DIR} )
    ELSE (HDF5_HL_LIBRARY)
      SET (HDF5_HL_FOUND "NO")
    ENDIF (HDF5_HL_LIBRARY)
  ENDIF (HDF5_HL_INCLUDE_DIR)
ENDIF ( HDF5_USE_HIGH_LEVEL )

