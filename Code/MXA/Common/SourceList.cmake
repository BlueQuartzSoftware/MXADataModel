if (MSVC)
    SET (MXA_WIN_Common_SRCS
      ${MXA_CODE_DIR}/MXA/Common/IO/Private/MXAPrivateWindowsFileReader.cpp
      ${MXA_CODE_DIR}/MXA/Common/IO/Private/MXAPrivateWindowsFileWriter.cpp
    )
    SET (MXA_WIN_Common_HEADERS
      ${MXA_CODE_DIR}/MXA/Common/IO/Private/MXAPrivateWindowsFileReader.h
      ${MXA_CODE_DIR}/MXA/Common/IO/Private/MXAPrivateWindowsFileWriter.h
    )
    IDE_SOURCE_PROPERTIES( "MXA/Common/IO/Private" "${MXA_WIN_Common_HEADERS}" "${MXA_WIN_Common_SRCS}")
else(MSVC)
    SET (MXA_UNIX_Common_SRCS
      ${MXA_CODE_DIR}/MXA/Common/IO/Private/MXAPrivateFileReader.cpp
      ${MXA_CODE_DIR}/MXA/Common/IO/Private/MXAPrivateFileWriter.cpp
    )
    SET (MXA_UNIX_Common_HEADERS
      ${MXA_CODE_DIR}/MXA/Common/IO/Private/MXAPrivateFileReader.h
      ${MXA_CODE_DIR}/MXA/Common/IO/Private/MXAPrivateFileWriter.h
    )
    IDE_SOURCE_PROPERTIES( "MXA/Common/IO/Private" "${MXA_UNIX_Common_HEADERS}" "${MXA_UNIX_Common_SRCS}")
endif()



SET (MXA_Common_SRCS

)

SET (MXA_Common_HEADERS 
    ${MXA_CODE_DIR}/MXA/Common/LogTime.h
    ${MXA_CODE_DIR}/MXA/Common/DLLExport.h
    ${MXA_CODE_DIR}/MXA/Common/MXAEndian.h
    ${MXA_CODE_DIR}/MXA/Common/MXATypeDefs.h
    ${MXA_CODE_DIR}/MXA/Common/MXAErrorDefinitions.h
    ${MXA_CODE_DIR}/MXA/Common/MXASetGetMacros.h
    ${MXA_CODE_DIR}/MXA/Common/IO/MXAFileReader64.h
    ${MXA_CODE_DIR}/MXA/Common/IO/MXAFileWriter64.h
    ${MXA_CODE_DIR}/MXA/Common/Cast/Cast.h
    ${MXA_CODE_DIR}/MXA/Common/Win32Defines.h
)


IDE_SOURCE_PROPERTIES( "MXA/Common" "${MXA_Common_HEADERS}" "${MXA_Common_SRCS}")

SET (MXA_Common_SRCS
    ${MXA_WIN_Common_SRCS}
    ${MXA_UNIX_Common_SRCS}
)

SET (MXA_Common_HEADERS 
    ${MXA_Common_HEADERS}
    ${MXA_WIN_Common_HEADERS}
    ${MXA_UNIX_Common_HEADERS}
)

if ( ${MXA_INSTALL_FILES} EQUAL 1 )

    INSTALL (FILES  ${MXA_CODE_DIR}/MXA/Common/IO/MXAFileReader64.h
                    ${MXA_CODE_DIR}/MXA/Common/IO/MXAFileWriter64.h
             DESTINATION include/MXA/Common/IO
             COMPONENT Headers           
    )
    
    INSTALL (FILES  ${MXA_WIN_Common_HEADERS}
                    ${MXA_UNIX_Common_HEADERS}
             DESTINATION include/MXA/Common/IO/Private
             COMPONENT Headers           
    )
    
    INSTALL (FILES   ${MXA_CODE_DIR}/MXA/Common/Cast/Cast.h
             DESTINATION include/MXA/Common/Cast
             COMPONENT Headers           
    )
endif()

