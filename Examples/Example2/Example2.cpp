///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2008, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
//-- MXA Includes
#include "MXA/MXA.h"
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/LogTime.h>
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/Core/MXADataModel.h>
#include <MXA/Core/MXADataModelWriter.hpp>
#include <MXA/XML/XMLFileUtilities.hpp>
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/HDF5/H5MXAUtilities.h>
#include <MXA/HDF5/H5Image.h>
#include <MXA/HDF5/H5MXADataFile.h>
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/DataWrappers/MXARGBImage.h>
#include <MXA/DataWrappers/MXA2DArray.hpp>
#include <MXA/HDF5/H5Dataset.h>
#include <Examples/ExampleFileLocations.h>


// HDF5 Include
#include <hdf5.h>

/**
 * This example shows how to retrieve data that is stored in a hdf5 based MXA
 * datafile. This example uses the same model layout as example #1 used and adds
 * a 'readData' method that shows how to use some of the predefined classes in
 * the MXA library that can fetch their data from an HDF5 data file
 */

// Declare methods
void listDataDimensions(MXADataModel* model);
void listDataRecords(MXADataModel* model);
IMXAArray::Pointer captureSampleImage();
IMXAArray::Pointer captureSampleSignal();
int32_t generateData();
int32_t readData();
void miscTest();
IMXAArray::Pointer readRGBImageFromFile(IDataFile::Pointer dataFile, const std::string &datasetPath);

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
std::cout << logTime() << "Example 2 Starting" << std::endl;
 if ( generateData() == EXIT_FAILURE)
 {
   return EXIT_FAILURE;
 }

 if (readData() == EXIT_FAILURE)
 {
   return EXIT_FAILURE;
 }

 std::cout << logTime() << "Example 2 Ending"  << std::endl;
 return EXIT_SUCCESS;
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t readData()
{
  // Open the data file in read-only mode
  IDataFile::Pointer dataFile = H5MXADataFile::OpenFile(Examples::Example2File, true);
  if (dataFile.get() == NULL)
  { // CHeck for a good open operation. If something failed during the open operation
    //  then the pointer will be NULL;
    return EXIT_FAILURE;
  }

  IDataModel::Pointer modelPtr = dataFile->getDataModel();
  //Now we need to get each MXADataDimension from the Model so we know what
  // values to use in our loop.

  IDataDimension::Pointer timeDim = modelPtr->getDataDimension(0);
  if (timeDim.get() == NULL)
  {
    return EXIT_FAILURE;
  }
  IDataDimension::Pointer pressureDim = modelPtr->getDataDimension(1);
  if (pressureDim.get() == NULL)
  {
    return EXIT_FAILURE;
  }

  int32_t err = 0;
  // Create an array with 2 values. This will hold the values for the current time and current
  //  pressure indexes. These indexes allow for the construction of the internal HDF5
  // path to the actual data.
  std::vector<int32_t> indices (2,0);
  IDataRecord::Pointer cameraRec = modelPtr->getDataRecordByNamedPath("Camera");
  IDataRecord::Pointer tempRec = modelPtr->getDataRecordByNamedPath("Temperature");
  if (cameraRec.get() == NULL || tempRec.get() == NULL)
  {
    return EXIT_FAILURE;
  }
  for (int t = timeDim->getStartValue(); t <= timeDim->getEndValue(); t+= timeDim->getIncrement())
  {
    indices[0] = t;
    for (int p = pressureDim->getStartValue(); p <= pressureDim->getEndValue(); p+=pressureDim->getIncrement() )
    {
      indices[1] = p;
      std::string cameraPath = H5MXAUtilities::generateH5PathToDataset(modelPtr, indices, cameraRec);
      std::string tempPath = H5MXAUtilities::generateH5PathToDataset(modelPtr, indices, tempRec);

      // We know the "Camera" signal was an RGBArray, so we create an MXARGBImage to hold the data;
      IMXAArray::Pointer image = readRGBImageFromFile(dataFile, cameraPath);
      if (err < 0) {
        std::cout << DEBUG_OUT(logTime) << "cameraPath: " << cameraPath << std::endl;
        return EXIT_FAILURE;
      }

      //We know the temperature was a scalar float value of size 32 bits
      //MXAArrayTemplate<float>* tempValue = MXAArrayTemplate<float>::New( 1 );

      IMXAArray* tempValue = H5Lite::readMXAArray(dataFile->getFileId(), tempPath);
      if (NULL == tempValue) {
        std::cout << DEBUG_OUT(logTime) << "tempPath: " << tempPath << std::endl;
        return EXIT_FAILURE;
      }

      // Now you can do something with the data... Print it, display it..

    }
  }
  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int32_t generateData()
{


  //Instatiate a new model using the predefined boost shared pointer type
  MXADataModel::Pointer modelPtr = MXADataModel::New();
  MXADataModel* model = modelPtr.get();

  //Define at what path in the HDF5 file the data will be stored
  model->setDataRoot("/Experimental Data");

  //Instantiate 2 Data Dimensions
  // The first dimension has 10 elements from 0 to 9 and increments by 1. Since this
  // is the first dimension we give it an index of 0
  int32_t index = 0;
  int32_t count = 10;
  int32_t start = 0;
  int32_t end = 9;
  int32_t increment = 1;
  int32_t uniform = 1;
  MXADataDimension::Pointer dim1 = MXADataDimension::New("Time", "Time (minutes)", index, count, start, end, increment, uniform);

  // The second dimension will have 4 elements ranging from 2 to 8 with an increment of 2;
  index = 1;
  count = 4;
  start = 200;
  end = 800;
  increment = 200;
  uniform = 1;
  MXADataDimension::Pointer dim2 = MXADataDimension::New("Pressure", "Press (kPa)", index, count, start, end, increment, uniform);

  //Next we need to add these dimensions to the model. Since we are using Boost shared pointers
  // the dimension objects are refcounted thus relieving us from having to worry about cleaning up
  // the memory allocations
  model->addDataDimension(dim1);
  model->addDataDimension(dim2);


  // Next we need to create a data record to hold one of the dependent variables for our experiment.
  // In our sample experiment we are going to measure the temperature and record an image of the sample.
  // The important argument is the 'luid' argument. These need to be unique within each group of Data Records.
  MXADataRecord::Pointer temp = MXADataRecord::New(0, "Temperature" , "Temp (K)");
  MXADataRecord::Pointer cameraImage = MXADataRecord::New(1, "Camera", "Camera Image(RGB)");

  // Next, add these Records to the Data Model
  model->addDataRecord(temp);
  model->addDataRecord(cameraImage);

  //Lastly a certain number of meta data fields are required to be set to non-empty values
  std::map<std::string, std::string> md;
  md[MXA::MXA_CREATOR_TAG] = "Mike Jackson"; // Who is performing the experiment
  md[MXA::MXA_DATE_TAG] = "2006:12:24 15:34.51"; // What date is the experiment being performed
  md[MXA::MXA_DSET_NAME_TAG] = "Testing Data Import"; // Give this specific experiment a name or other identifying tag
  md[MXA::MXA_DESCRIPTION_TAG] = "Application to test importing data to the data file"; // Give a short description of the experiment
  md[MXA::MXA_PEDIGREE_TAG] = MXA::MXA_PEDIGREE_ORIGINAL_VALUE; // Is the data being stored original data or was it imported from another source?
  md[MXA::MXA_DERIVED_SRC_TAG] = MXA::MXA_NOT_APPLICABLE_VALUE; // The data is original from the instrument so this tag is not needed
  md[MXA::MXA_RIGHTS_TAG] = MXA::MXA_RIGHTS_UNLIMITED_VALUE; // There are no limitations on the distribution of the data
  md[MXA::MXA_RELEASE_NUMBER_TAG] = "90312901291239012390"; // The Data has been through a local public affairs office which assigned the data this unique ID
  model->setRequiredMetaData(md);

  // So now our model should be valid. We can check the validity of the model with the following:
  std::string message;
  bool valid = model->isValid(message);
  if ( !valid )
  {
    std::cout << "Model was NOT valid. Exiting with Failure. Error message given was: \n" << message << std::endl;
    return EXIT_FAILURE;
  }

  //Write the model to a new HDF5 file, deleting any existing file and
  // allowing the Hdf5 file to remain open for further processing
  IDataFile::Pointer dataFile = H5MXADataFile::CreateFileWithModel(Examples::Example2File, modelPtr);

  if (NULL == dataFile.get() )
  {
    std::cout << "Error Writing Model to HDF5 File" << std::endl;
    return EXIT_FAILURE;
  }

  //Lets store some data into the HDF5 File. In our experiment we are recording the time
  // in 1 minute intervals for 10 minutes and also incrementing the pressure by
  // 200 KPa starting at 200 and ending at 800 KPa. At each combination of those
  // values we are taking the temperature and capturing an image of our sample
  std::vector<int32_t> indices(2, 0); // we keep this for re-use during the loop
  std::string temperaturePath;
  std::string cameraImagePath;
  float temperature = 1200.0f;



  MXAArrayTemplate<float>* temperatureArray = MXAArrayTemplate<float>::New(1);
  IMXAArray::Pointer temperaturePtr(temperatureArray); // Let boost clean up the memory when we are finished with it
  int32_t err = 0;
  // Define the height/width of our camera "image"
  std::vector<hsize_t> dims (2,0);
  dims[0] = 10;
  dims[1] = 10;
  for (int t = 0; t <= 9; ++t)
  {
    indices[0] = t;
    for (int p = 200; p <= 800; p+=200)
    {
      temperature += (float)p;
      indices[1] = p;
      temperaturePath = H5MXAUtilities::generateH5PathToDataset(modelPtr, indices, temp );
      cameraImagePath = H5MXAUtilities::generateH5PathToDataset(modelPtr, indices, cameraImage );

      temperatureArray->setValue(0, temperature);

      IDataset::Pointer tempDataset = H5Dataset::CreateDatasetPtr(temperaturePath, temperaturePtr);
       // Write the temperature value to the HDF5 File
      err = tempDataset->writeToFile(dataFile);
      if (err < 0)
      {
        std::cout << "Error writing temperature dataset for (t,p):" << t << "," << p << std::endl;
        break;
      }

      IMXAArray::Pointer imagePtr = captureSampleImage();
      MXARGBImage* image = dynamic_cast<MXARGBImage*>(imagePtr.get() );
      err = H5Image::H5IMmake_image_24bit(dataFile->getFileId(), cameraImagePath, image->getWidth(), image->getHeight(), MXA::H5Image::InterlacePixel, image->getPointer(0) );
      if (err < 0)
      {
        std::cout << "Error writing image dataset for (t,p):" << t << "," << p << std::endl;
        break;
      }
    }
  }
  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArray::Pointer captureSampleImage()
{
  IMXAArray::Pointer imagePtr;
  MXARGBImage* image = MXARGBImage::New( 256, 256);
  if (NULL == image)
  {
    return imagePtr;
  }
  imagePtr.reset(image);
  uint8_t* data = image->getPixelPointer(0,0);
  int32_t xDim = 256;
  int32_t yDim = 256;
  for (int i = 0; i < xDim; ++i) {
    for (int j = 0; j < yDim; ++j) {
      data[0] = i * j;
      data[1] = i;
      data[2] = j;
      data +=3; // move the pointer by 3 bytes
    }
  }
  return imagePtr;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
IMXAArray::Pointer captureSampleSignal()
{
  IMXAArray::Pointer imagePtr;
  MXA2DArray<uint8_t>* image = MXA2DArray<uint8_t>::New(256, 256);
  if (NULL == image)
  {
    return imagePtr;
  }
  imagePtr.reset(image);
  uint8_t* data = image->getPointer(0,0);
  int32_t xDim = 256;
  int32_t yDim = 256;
  for (int i = 0; i < xDim; ++i) {
    for (int j = 0; j < yDim; ++j) {
      data[0] = i * j;
      data++; // move the pointer by 3 bytes
    }
  }
  return imagePtr;
}



// -----------------------------------------------------------------------------
//  IDataFileIO Implementation (IFileReader)
// -----------------------------------------------------------------------------
IMXAArray::Pointer readRGBImageFromFile(IDataFile::Pointer dataFile, const std::string &datasetPath)
{
  MXARGBImage* nullImage = NULL;
  IMXAArray::Pointer imagePtr (nullImage);

  hid_t fileId = dataFile->getFileId();
  if (fileId < 0)
  {
    return imagePtr;
  }
 // herr_t err = -1;
  IMXAArray* image = H5Lite::readMXAArray(fileId, datasetPath );
  imagePtr.reset(image);
  return imagePtr;
}


