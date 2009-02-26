if (WIN32)
    SET (MXA_WIN_COMMON_SRCS
      ${MXA_SOURCE_DIR}/MXA/Common/IO/Private/MXAPrivateWindowsFileReader.cpp
      ${MXA_SOURCE_DIR}/MXA/Common/IO/Private/MXAPrivateWindowsFileWriter.cpp
    )
    SET (MXA_WIN_COMMON_HEADERS
      ${MXA_SOURCE_DIR}/MXA/Common/IO/Private/MXAPrivateWindowsFileReader.h
      ${MXA_SOURCE_DIR}/MXA/Common/IO/Private/MXAPrivateWindowsFileWriter.h
    )
else(WIN32)
    SET (MXA_UNIX_COMMON_SRCS
      ${MXA_SOURCE_DIR}/MXA/Common/IO/Private/MXAPrivateFileReader.cpp
      ${MXA_SOURCE_DIR}/MXA/Common/IO/Private/MXAPrivateFileWriter.cpp
    )
    SET (MXA_UNIX_COMMON_HEADERS
      ${MXA_SOURCE_DIR}/MXA/Common/IO/Private/MXAPrivateFileReader.h
      ${MXA_SOURCE_DIR}/MXA/Common/IO/Private/MXAPrivateFileWriter.h
    )
endif()



SET (MXA_COMMON_SRCS
    ${MXA_WIN_COMMON_SRCS}
    ${MXA_UNIX_COMMON_SRCS}
)

SET (MXA_COMMON_HEADERS 
    ${MXA_WIN_COMMON_HEADERS}
    ${MXA_UNIX_COMMON_HEADERS}
    ${MXA_SOURCE_DIR}/MXA/Common/LogTime.h
    ${MXA_SOURCE_DIR}/MXA/Common/DLLExport.h
    ${MXA_SOURCE_DIR}/MXA/Common/MXAEndian.h
    ${MXA_SOURCE_DIR}/MXA/Common/MXATypeDefs.h
    ${MXA_SOURCE_DIR}/MXA/Common/MXATypes.h
    ${MXA_SOURCE_DIR}/MXA/Common/MXAErrorDefinitions.h
    ${MXA_SOURCE_DIR}/MXA/Common/MXASetGetMacros.h
    ${MXA_SOURCE_DIR}/MXA/Common/IO/MXAFileReader64.h
    ${MXA_SOURCE_DIR}/MXA/Common/IO/MXAFileWriter64.h
    ${MXA_SOURCE_DIR}/MXA/Common/Cast/Cast.h
    ${MXA_SOURCE_DIR}/MXA/Common/Win32Defines.h
)


MXA_SOURCE_PROPERTIES(Common "${MXA_COMMON_HEADERS}" "${MXA_COMMON_SRCS}")

INSTALL (FILES  ${MXA_SOURCE_DIR}/MXA/Common/IO/MXAFileReader64.h
                ${MXA_SOURCE_DIR}/MXA/Common/IO/MXAFileWriter64.h
         DESTINATION include/MXA/Common/IO
         COMPONENT Headers           
)

INSTALL (FILES  ${MXA_WIN_COMMON_HEADERS}
                ${MXA_UNIX_COMMON_HEADERS}
         DESTINATION include/MXA/Common/IO/Private
         COMPONENT Headers           
)

INSTALL (FILES   ${MXA_SOURCE_DIR}/MXA/Common/Cast/Cast.h
         DESTINATION include/MXA/Common/Cast
         COMPONENT Headers           
)

