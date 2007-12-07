

#ifndef MXA_BMP_IO_H
#define MXA_BMP_IO_H


//-- MXA Includes
#include <Common/MXAEndian.h>
#include <Common/MXATypes.h>
#include <Common/DLLExport.h>
#include <Common/IO/Reader64.h>
#include <BMPIO/MXABmpHeaders.h>

//-- C Includes
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <string.h>

#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<Reader64>    Reader64Ptr;

// The following is the function return type. Use this to
// get information about how the loading operation went.

enum LOAD_TEXTUREBMP_RESULT {
  // The texture loading operation succeeded.
  LOAD_TEXTUREBMP_SUCCESS=0,
  // The file could not be found or it could not be opened for reading.
  LOAD_TEXTUREBMP_COULD_NOT_FIND_OR_READ_FILE,
  // The file does not comply with the specification.
  LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT,
  // OpenGL could not accept the texture. You proably used a internal
  // format not accepted by your OpenGL implementation or you may have run 
  // out of available texture names.
  LOAD_TEXTUREBMP_OPENGL_ERROR,
  // The system ran out of heap memory during the texture load.
  LOAD_TEXTUREBMP_OUT_OF_MEMORY
};

/**
* @class MXABmpIO MXABmpIO.h BMPIO/MXABmpIO.h
* @brief This class Handles the Reading of Windows Bitmap files (.bmp)
* @author Herb Mullens
* @date Oct 2007
* @version $Revision: 1.10 $
*/
class MXA_EXPORT MXABmpIO
{
public:
  MXABmpIO();
  virtual ~MXABmpIO();
  
  uint8* bitmapData;
  int32 getHeight();
  int32 getWidth();
  int32 getNumberOfChannels();
  LOAD_TEXTUREBMP_RESULT loadBMPData(const char*);
  void convertToGrayscale();
  bool isGrayscaleImage();
  void copyDataArray(std::vector<uint8> &buffer);
protected:

private:
  int32 width;
  int32 height;
  int32 numChannels;
  //FILE* file;
  Reader64Ptr _reader64Ptr;
  MXABMPFileHeader fileHeader;
  MXABMPDIBHeader dibHeader;
  bool isGrayscale;

  int32 bytesRead;
  // Palette used for paletted images during load.
  uint8 palette[3][256];

  int32 read32BitValue();
  int16 read16BitValue();
  uint8 read8BitValue();
  
  LOAD_TEXTUREBMP_RESULT readFileHeader();
  LOAD_TEXTUREBMP_RESULT readInfoHeader();
  LOAD_TEXTUREBMP_RESULT readPalette();
  LOAD_TEXTUREBMP_RESULT readBitmapData(uint8*);
  LOAD_TEXTUREBMP_RESULT readBitmapData1Bit(uint8*);
  LOAD_TEXTUREBMP_RESULT readBitmapData4Bit(uint8*);
  LOAD_TEXTUREBMP_RESULT readBitmapData8Bit(uint8*);
  LOAD_TEXTUREBMP_RESULT readBitmapData24Bit(uint8*);
  bool handleEscapeCode(int, int*, int*, LOAD_TEXTUREBMP_RESULT*);
  void flipBitmap();
  
  MXABmpIO(const MXABmpIO&);    //Not Implemented
  void operator=(const MXABmpIO&);  //Not Implemented
  
  
};
  
  
#endif
