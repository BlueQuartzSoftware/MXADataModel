SET (MXA_BMPIO_SRCS
    ${MXA_SOURCE_DIR}/MXA/BMPIO/MXABmpIO.cpp
)

SET (MXA_BMPIO_HEADERS
  ${MXA_SOURCE_DIR}/MXA/BMPIO/MXABmpIO.h
  ${MXA_SOURCE_DIR}/MXA/BMPIO/MXABmpHeaders.h
)

IDE_SOURCE_PROPERTIES( "MXA/BMPIO" "${MXA_BMPIO_HEADERS}" "${MXA_BMPIO_SRCS}")
