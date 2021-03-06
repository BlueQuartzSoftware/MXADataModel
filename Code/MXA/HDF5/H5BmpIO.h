///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, 2010 Michael A. Jackson for BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _H5BMPIO_H_
#define _H5BMPIO_H_

//-- MXA Headers
#include "MXA/MXADLLExport.h"
#include "MXA/MXA.h"
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/BMPIO/MXABmpIO.h>

//-- HDF Headers
#include <hdf5.h>

//-- STL Headers
#include <string>

using namespace std;


/**
* @brief General IO class for Bmp images
* @author Mike Jackson/Herb Mullens
* @date October 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT H5BmpIO
{
 public:
  explicit H5BmpIO(hid_t fileId);
  virtual ~H5BmpIO();

  /**
   * @brief imports a Bmp image into the MXADataModel
   *
   * @param filename The absolute path to the Bmp file
   * @param fileId The HDF Group/FileId to store the imported data
   * @param datasetName The Name of the HDF5 dataset to store the data
   * @param asGrayscale Should the Bmp be stored as a grayscale image
   * @return Error < 0 Error
   */
  herr_t importBmp(const std::string &filename,
                    hid_t fileId,
                    const std::string &datasetName,
                    bool asGrayscale = true);


  /**
   * @brief exports a BMP image from the MXADataModel
   *
   * @param fileId - the hdf fileID
   * @param filename - the output file to write the bmp to
   * @param datasetPath - the image dataset in the MXA HDF5 file
   */
  herr_t exportBmp(hid_t fileId,
                    const std::string &filename,
                    const std::string &datasetPath);



 protected:
   /**
    * @brief Stores a True Color image in the form of a Pixel interlaced buffer as a Grayscale image in the provided HDF5 data set
    * @param fileId The HDF5 file id
    * @param datasetName The name of the data set to store the RGB image into
    * @param reader The reader object for reading BMP files
    * @return Error Condition
    */
   herr_t _importGrayscaleBmpImage(hid_t fileId,
   								                 const std::string &datasetName,
                                   MXABmpIO &reader);

   /**
    * @brief Stores a True Color image in the form of a Pixel interlaced buffer as an RGB image in the provided HDF5 data set
    * @param fileId The HDF5 file id
    * @param datasetName The name of the data set to store the RGB image into
    * @param reader The reader object for reading BMP files
    * @return Error Condition
    */
   herr_t _importRGBFullColorBmp(hid_t fileId,
   								               const std::string &datasetName,
   								               MXABmpIO &reader);

 private:
   hid_t _fileId;

  H5BmpIO(const H5BmpIO&);   //Copy Constructor Not Implemented
  void operator=(const H5BmpIO&); //Copy Assignment Not Implemented

};



#endif /* _H5BMPIO_H_ */
