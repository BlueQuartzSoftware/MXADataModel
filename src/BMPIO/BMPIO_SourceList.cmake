SET (MXA_BMPIO_SRCS
    ${MXA_SOURCE_DIR}/src/BMPIO/MXABmpIO.cpp
)

SET (MXA_BMPIO_HEADERS
  ${MXA_SOURCE_DIR}/src/BMPIO/MXABmpIO.h
  ${MXA_SOURCE_DIR}/src/BMPIO/MXABmpHeaders.h
)

MXA_SOURCE_PROPERTIES(BMPIO "${MXA_BMPIO_HEADERS}" "${MXA_BMPIO_SRCS}")
