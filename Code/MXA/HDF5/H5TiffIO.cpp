/* ============================================================================
 * Copyright (c) 2011, Michael A. Jackson (BlueQuartz Software)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Jackson nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */


//-- MXA Headers
#include <MXA/HDF5/H5TiffIO.h>

#include <MXA/MXAVersion.h>
#include <MXA/Common/LogTime.h>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Image.h>

#include <MXA/Utilities/StringUtils.h>

#include <MXA/Base/ITiffTagExtractor.h>

// Must come after the above includes because MXATypes is already defining
// the base integer types that tiff.h will try to define.
//#include <tif_config.h>

//-- STL Headers
#include <iostream>
#include <math.h>

//-- C includes
#include <string.h>
// If you use LZW_COMPRESSION then any source buffer that gets written to a file
// WILL BE ALTERED by the tiff functions which can have bad side effects if you
// elect to reuse the buffer. Until a better API is designed to copy the image
// buffer then leave this set to ZERO.
#define USE_LZW_COMPRESSION 0



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5TiffIO::H5TiffIO(hid_t fileId) :
  _fileId(fileId)
{
  _tiff = NULL;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
H5TiffIO::~H5TiffIO()
{
  if ( NULL != _tiff)
  {
    (void) TIFFClose(_tiff);
  }
}

// -------------------------------------------------------------------------
// Read the tiff file
// -------------------------------------------------------------------------
herr_t H5TiffIO::importTiff(const std::string &filename,
                            hid_t groupId,
                            const std::string &datasetName,
                            bool asGrayscale)
{
  if (NULL != _tiff)
  {
    (void) TIFFClose(_tiff); // Close any open tiff file first
  }

  herr_t err = 0;

  _tiff = TIFFOpen(filename.c_str(), "r");
  if (_tiff == NULL) {
    std::cout << "Error Opening Tiff file with Absolute Path:\n    "
	      << filename << std::endl;
    return (herr_t) -1;
  }
  TIFFErrorHandler handler = 0;
  TIFFErrorHandler prevTifErrorHandler = TIFFSetWarningHandler(handler);

  if (asGrayscale) {
    err = _importGrayscaleTiffImage(_tiff, groupId, datasetName);
  } else {
    int32_t imageClass = _determineTiffImageClass(_tiff);

    switch (imageClass) {
      case GrayscaleTiffImage:
        err = _importGrayscaleTiffImage( _tiff, groupId, datasetName);
        break;
      case PaletteColorTiffImage:
        err = _importPaletteColorTiff( _tiff, groupId, datasetName);
        break;
      case RGBFullColorTiffImage:
        err = _importRGBFullColorTiff(_tiff, groupId, datasetName);
        break;
      default:
        std::cout << "importTiff ERROR: Unsupported TIFF image type:" << imageClass << std::endl;
        err = -1;
    }
  }

  handler = TIFFSetWarningHandler(prevTifErrorHandler);
  (void) TIFFClose(_tiff);
  _tiff = NULL;
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void H5TiffIO::extractTiffTagContents(ITiffTagExtractor* extractor)
{
  extractor->extractTagContents(_tiff);
}

// ---------------------------------------------------------------------
//  determines the TIFF Image Class type
// ---------------------------------------------------------------------
int H5TiffIO::_determineTiffImageClass(TIFF *in)
{
  uint16_t samplesperpixel = 0;
  uint16_t bitspersample = 0;
  uint16_t compression = 0;
  uint16_t photometric = 0;

  // Get determining fields
  TIFFGetField(in, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
  TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bitspersample);
  TIFFGetField(in, TIFFTAG_COMPRESSION, &compression);
  TIFFGetField(in, TIFFTAG_PHOTOMETRIC, &photometric);

//  std::cout << "TIFF Image Field Values: " << std::endl;
//  std::cout << "  SamplesPerPixel: " << samplesperpixel << std::endl;
//  std::cout << "  BitsPerSample:   " << bitspersample << std::endl;
//  std::cout << "  Compression:     " << compression << std::endl;
//  std::cout << "  Photometric:     " << photometric << std::endl;

  if (photometric == 6) {
    return YCbCrTiffImage;
  } else if (photometric == 5) {
    return SeparatedTiffImage;
  } else if (compression == 3 || compression == 4) {
    return FacsimileTiffImage;
  } else if (photometric == 3) {
    return PaletteColorTiffImage;
  } else if (photometric == 2) {
    return RGBFullColorTiffImage;
  } else if (bitspersample == 1) {
    return BilevelTiffImage;
  } else if (samplesperpixel == 1) {
    return GrayscaleTiffImage;
  }
  return UnknownTiffImage;
}



// ---------------------------------------------------------------------
// Reads a grayscale image into an HDF5 Image Dataset
// ---------------------------------------------------------------------
herr_t H5TiffIO::_importGrayscaleTiffImage(TIFF *in, hid_t groupId, const std::string &datasetName)
{
  unsigned char* raster;     /* retrieve RGBA image */
  uint32_t  width, height;    /* image width & height */
  int16_t samplesPerPixel = 0;
  herr_t err = 0;

  //std::cout << "Importing grayscale tiff image" << std::endl;

  TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(in, TIFFTAG_SAMPLESPERPIXEL, &samplesPerPixel);

  raster = (unsigned char*)_TIFFmalloc(width * height * 4);
  if (raster == 0) {
    TIFFError(TIFFFileName(in), "No space for raster buffer");
    return (-1);
  }

  // TIFFReadRGBAImageOriented converts non 8bit images including:
  //  Grayscale, bilevel, CMYK, and YCbCR transparently into 32bit RGBA
  //  samples

  /* Read the image in one chunk into an RGBA array */
  if (!TIFFReadRGBAImageOriented(in, width, height, (uint32_t*)(raster), ORIENTATION_TOPLEFT, 0)) {
    _TIFFfree(raster);
    return (-1);
  }

  // Collapse the data down to a single channel, that will end up
  //  being the grayscale values
  int32_t pixel_count = width * height;
  unsigned char *src, *dst;
 // unsigned char r,g,b;
  float R, G, B;
  // The collapse is done IN PLACE
  src = raster;
  dst = raster;
  while( pixel_count > 0 )
  {
    if (src[0] == src[1] && src[0] == src[2])
    {
      *(dst) = src[0];
    }
    else
    {
      R = src[0] * 0.299; G = src[1] * 0.587; B = src[2] * 0.144;
      *(dst) =  (unsigned char)(R + G + B);
    }
    dst++;
    src += 4; //skip ahead by 4 bytes because we read the raw array into an RGBA array.
    pixel_count--;
  }

  // Store byte array to HDF5 File
  err = H5Image::H5IMmake_image_8bit(groupId, datasetName, width, height, MXA::H5Image::UpperLeft, (unsigned char *)raster );
  if (err<0) {
    std::cout << "Error storing Image data with H5IM API:  datasetName: "
	      << datasetName << std::endl;
  }

  // Need to update the attributes to be correct for a grayscale image
  H5Lite::writeStringAttribute(groupId,
                         datasetName,
                         const_cast<std::string&>(MXA::H5Image::ImageSubclass),
                         const_cast<std::string&>(MXA::H5Image::ImageGrayScale) );
 // H5LTset_attribute_string(groupId, datasetName.c_str(), MXA::H5Image::ImageSubclass, MXA::H5Image::ImageGrayScale);
  uint32_t white_is_zero = 0;
  H5Lite::writeScalarAttribute(groupId,
                         datasetName,
                         const_cast<std::string&>(MXA::H5Image::ImageWhiteIsZero),
                         white_is_zero);

  // set the display origin
  //    "UL": (0,0) is at the upper left.
  //    "LL": (0,0) is at the lower left.
  //    "UR": (0,0) is at the upper right.
  //    "LR": (0,0) is at the lower right.
  err = H5Lite::writeStringAttribute(groupId,
                               datasetName,
                               const_cast<std::string&>(MXA::H5Image::DisplayOrigin),
                               const_cast<std::string&>(MXA::H5Image::UpperLeft) );
  if (err < 0) {
    std::cout << "Error setting display origin" << std::endl;
    _TIFFfree( raster );
    return -1;
  }


  _TIFFfree( raster );
  return err;
}



// ---------------------------------------------------------------------
// Reads an 8bit color palette image into an HDF5 Image Dataset
// ---------------------------------------------------------------------
herr_t H5TiffIO::_importPaletteColorTiff( TIFF *in, hid_t groupId, const std::string &datasetName)
{
  herr_t err = 0;

  if (false)
  {
    std::cout << DEBUG_OUT(logTime) << "Indexed Color Images are not supported." << std::endl;
    return -1;
  }

  uint32_t* raster;     /* retrieve RGBA image */
  uint32_t  width, height;    /* image width & height */
  uint16_t  bitspersample;

 // std::cout << "Importing 8bit color palette tiff image" << std::endl;

  TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(in, TIFFTAG_BITSPERSAMPLE, &bitspersample);

  raster = (uint32_t*)_TIFFmalloc(width * height * sizeof (uint32_t));
  if (raster == 0) {
    TIFFError(TIFFFileName(in), "No space for raster buffer");
    return (-1);
  }


  // TIFFReadRGBAImageOriented converts non 8bit images including:
  //  Grayscale, bilevel, CMYK, and YCbCR transparently into 32bit RGBA
  //  samples
  /* Read the image in one chunk into an RGBA array */
  if (!TIFFReadRGBAImageOriented(in, width, height, raster, ORIENTATION_TOPLEFT, 0))
  {
    _TIFFfree(raster);
    return (-1);
  }

  // Strip out the Alpha Components
  int32_t pixel_count = width * height;
  unsigned char *src, *dst;

  src = (unsigned char *) raster;
  dst = (unsigned char *) raster;
  while( pixel_count > 0 )
    {
      *(dst++) = *(src++);
      *(dst++) = *(src++);
      *(dst++) = *(src++);
      src++;
      pixel_count--;
    }

  // Store byte array to HDF5 File
  err = H5Image::H5IMmake_image_24bit(groupId, datasetName, width, height, H5IM_INTERLACE_PIXEL.c_str() , (unsigned char *)raster);
  if (err<0) {
    std::cout << "_importPaletteColorTiff: Error storing 24 bit true color Image data with H5IM API. datasetName: " << datasetName << std::endl;
  }
  _TIFFfree( raster );
  return err;
}

// ---------------------------------------------------------------------
// Reads a True Color image into an HDF5 24Bit Image data set
// ---------------------------------------------------------------------
herr_t H5TiffIO::_importRGBFullColorTiff(TIFF *in, hid_t groupId, const std::string &datasetName)
{
  uint32_t* raster;     /* retrieve RGBA image */
  uint32_t  width, height;    /* image width & height */
  herr_t err = -1;

  std::cout << "Importing rgb full color tiff image" << std::endl;

  TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);

  raster = (uint32_t*)_TIFFmalloc(width * height * sizeof (uint32_t));
  if (raster == 0) {
    TIFFError(TIFFFileName(in), "No space for raster buffer");
    return (0);
  }

  /* Read the image in one chunk into an RGBA array */
  if (!TIFFReadRGBAImageOriented(in, width, height, raster,
				 ORIENTATION_TOPLEFT, 0)) {
    _TIFFfree(raster);
    return (0);
  }

  /*
  ** Strip out the Alpha Components
  */
  int32_t pixel_count = width * height;
  unsigned char *src, *dst;

  src = (unsigned char *) raster;
  dst = (unsigned char *) raster;
  while( pixel_count > 0 )
    {
      *(dst++) = *(src++);
      *(dst++) = *(src++);
      *(dst++) = *(src++);
      src++;
      pixel_count--;
    }

  // Store byte array to HDF5 File
  err = H5Image::H5IMmake_image_24bit(groupId, datasetName, width, height,
			     H5IM_INTERLACE_PIXEL.c_str() , (unsigned char *)raster);
  if (err<0) {
    std::cout << "Error storing 24 bit true color Image data with H5IM API. datasetName: " << datasetName << std::endl;
  }
  _TIFFfree( raster );
  return err;
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int H5TiffIO::_determineTiffOutputImageClass(hid_t fileId, const string &img_dataset_name)
{
  herr_t err = 0;
  int32_t dimRank;

  err = H5Lite::getAttributeNDims(fileId, img_dataset_name, const_cast<std::string&>(MXA::H5Image::ImageSubclass), dimRank);
  if (err < 0) {
    return UnknownTiffImage;
  }

  size_t type_size;
  H5T_class_t class_type;
  hid_t attrType = -1;
  std::vector<hsize_t> dimensions;
  err = H5Lite::getAttributeInfo(fileId,
                                 img_dataset_name,
                                 const_cast<std::string&>(MXA::H5Image::ImageSubclass),
                                 dimensions, class_type, type_size, attrType );
  err = H5Tclose(attrType);
  if (err < 0) {
    return UnknownTiffImage;
  }

  if (class_type == H5T_STRING) {
    //char atStr[type_size];
    std::string atStr;
    err = H5Lite::readStringAttribute(fileId, img_dataset_name, MXA::H5Image::ImageSubclass, atStr);

    if (err < 0) {
      return UnknownTiffImage;
    }

    if (atStr == MXA::H5Image::ImageGrayScale) {
      return GrayscaleTiffImage;
    } else if (atStr == MXA::H5Image::ImageIndexed ) {
      return PaletteColorTiffImage;
    }
  }

  return UnknownTiffImage;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// While linking the palette to the dataset referred to by
//   groupId and datasetName, the HDF code opened an hid_t
//   to that dataset but didn't close it.  This is an
//  Attribute dataset - search through the list of open attr Ids
//   and close the one matching.
void H5TiffIO::_closePaletteCreatedDataset(hid_t fileId, hid_t groupId,
              string datasetName,
              int32_t num_attrs)
{
  hid_t *attr_ids;
  attr_ids = (hid_t *)malloc(num_attrs * sizeof(hid_t));
  H5Fget_obj_ids(fileId, H5F_OBJ_ATTR, num_attrs, attr_ids);
  ssize_t numchars;
  char *attrname;

  numchars = 1 + H5Iget_name(groupId, NULL, 0);
  attrname = (char *)malloc(numchars * sizeof(char));
  H5Iget_name(groupId, attrname, numchars);
  string gname = attrname;
  gname += "/" + datasetName;
  string aname;
  for (int i=0; i<num_attrs; i++) {
    if (attr_ids[i] > 0) {
      numchars = 1+ H5Iget_name(attr_ids[i], NULL, 0);
      attrname = (char *)malloc(numchars * sizeof(char));
      H5Iget_name(attr_ids[i], attrname, numchars);
      aname = attrname;
      free(attrname);
      if (aname == gname) {
  H5Aclose(attr_ids[i]);
  break;
      }
    }
  }
  free(attr_ids);
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t H5TiffIO::_findColorMapIndex(int max, int32_t imgR, int32_t imgG, int32_t imgB,
         unsigned char *colorMap)
{
  // note - colorMap is in the format for HDF5 which means it's a flat
  //  1dimensional array where the entries are:  [R1, G1, B1, R2, G2, B2,...]
  for (int i=0; i<max; i=i+3) {
    if (imgR == colorMap[i] &&
  imgG == colorMap[i+1] &&
  imgB == colorMap[i+2]) {
      return (int)i/3;
    }
  }
  return -1;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5TiffIO::exportTiff(hid_t fileId,
                            const std::string &filename,
                            const std::string &img_dataset_name)
{
  TIFF *out;
  herr_t err = 0;

  hsize_t width, height, planes;
  hssize_t numpalettes;
  std::string interlace("");
  err = H5Image::H5IMget_image_info(fileId, img_dataset_name, &width, &height, &planes, interlace, &numpalettes);
  if (err < 0) {
    std::cout << "Error getting Image information from hdf file" << std::endl;
    return err;
  }

  std::vector<uint8_t>::size_type dSize = static_cast<std::vector<uint8_t>::size_type> (width * height);
  std::vector<uint8_t> data(dSize);
  err = H5Image::H5IMread_image(fileId, img_dataset_name, &(data.front() ) );
  if (err < 0) {
    std::cout << "Error reading image data" << std::endl;
    return err;
  }

  out = TIFFOpen(filename.c_str(), "w");
  if (out == NULL) {
    std::cout << "Could not open output tiff for writing" << std::endl;
    return -1;
  }

  int32_t imageClass = _determineTiffOutputImageClass(fileId, img_dataset_name);
  char h5_filename[1024];
  ::memset(h5_filename, 0,1024);  // Zero everything out
  size_t filename_size = 1024;
  err = H5Fget_name(fileId, h5_filename, filename_size);



  switch (imageClass) {
    case GrayscaleTiffImage:
      err = _exportGrayScaleTiff(out, &(data.front() ), width, height, h5_filename, img_dataset_name);
      break;
    case PaletteColorTiffImage:
      //err = _export8BitTiff(out, fileId, img_dataset_name, &(data.front() ), width, height, numpalettes);
      err = -1;
      std::cout << "PaletteColorTiffImage currently not supported for Export" << std::endl;
      break;
    case RGBFullColorTiffImage:
      err = _exportRGBFullColorTiff(out, fileId, img_dataset_name, width, height, planes, numpalettes, interlace);
      break;
    default:
      std::cout << "importTiff ERROR: Unsupported TIFF image type:"
  	      << imageClass << std::endl;
      err = -1;
  }

  (void) TIFFClose(out);
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5TiffIO::_exportRGBFullColorTiff(TIFF* out,
                              hid_t fileId,
                              const std::string &img_dataset_name,
                              hsize_t width,
                              hsize_t height,
                              hsize_t planes,
                              hssize_t numpalettes,
                              const std::string &interlace)
{
  herr_t err = -1;
  std::cout << "_exportRGBFullColorTiff NOT Implemented." << std::endl;
  return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
herr_t H5TiffIO::_exportGrayScaleTiff(TIFF *image,
                                     uint8_t* data,
                                     hsize_t width,
                                     hsize_t height,
                                     const std::string &documentName,
                                     const std::string &imageDescription)
{
  herr_t err = 0;

  // set the basic values
  err = TIFFSetField(image, TIFFTAG_IMAGEWIDTH, (int) width);
  err = TIFFSetField(image, TIFFTAG_IMAGELENGTH, (int) height);
  err = TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 8);
  err = TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);
  err = TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, (int) height); // 1 strip


  std::string datetime = tifDateTime();
  err = TIFFSetField(image, TIFFTAG_DATETIME, datetime.c_str());
  // String based tags
  if (documentName.length() > 0) {
    err = TIFFSetField(image, TIFFTAG_DOCUMENTNAME, documentName.c_str());
  }
  if (imageDescription.length() > 0) {
    err = TIFFSetField(image, TIFFTAG_IMAGEDESCRIPTION, imageDescription.c_str());
  }

  err = TIFFSetField(image, TIFFTAG_ORIENTATION, 1);
  err = TIFFSetField(image, TIFFTAG_PHOTOMETRIC, 1);

#if USE_LZW_COMPRESSION
  err = TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
  err = TIFFSetField(image, TIFFTAG_PREDICTOR, PREDICTOR_HORIZONTAL);
#else
  err = TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
#endif

  // Insert Resolution Units here if possible

  std::string software ("MXADataModel Version ");
  software.append(MXA::Version::Complete()).append(" using ");
#ifdef PACKAGE_STRING
  software.append(PACKAGE_STRING);
#else
  software.append("libTif");
#endif

  err = TIFFSetField(image, TIFFTAG_SOFTWARE, software.c_str());

  err = TIFFSetField(image, TIFFTAG_HOSTCOMPUTER, MXADATAMODEL_SYSTEM);

  // Write the information to the file
  tsize_t area = static_cast<tsize_t>(width* height);
  err = TIFFWriteEncodedStrip(image, 0, data, area);
  return err;
}

