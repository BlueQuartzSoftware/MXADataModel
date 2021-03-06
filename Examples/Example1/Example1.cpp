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

/**
 * This example demonstrates the following:
 *    Programmitically Creating a Data Model
 *    Exporting the Model to an XML File
 *    Retrieving a list of the Data Dimensions
 *    Retrieving a list of the Data Records
 *    Saving the Model to an HDF5 File
 *    Writing some sample data to the HDF5 file
 *    Shows 2 different ways to get the value from a User Defined Meta Data Object
 */

//-- MXA Includes
#include "MXA/MXA.h"
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/LogTime.h>
#include <MXA/Core/MXADataDimension.h>
#include <MXA/Core/MXADataRecord.h>
#include <MXA/Core/MXADataModel.h>
#include "MXA/Core/MXADataModelWriter.hpp"
#include <MXA/HDF5/H5Lite.h>
#include <MXA/HDF5/H5Utilities.h>
#include <MXA/HDF5/H5MXAUtilities.h>
#include <MXA/HDF5/H5MXADataFile.h>
#include <MXA/DataWrappers/MXAArrayTemplate.hpp>
#include <MXA/XML/XMLFileUtilities.hpp>
#include <Examples/ExampleFileLocations.h>

// HDF5 Include
#include <hdf5.h>


// Declare methods
void listDataDimensions(MXADataModel* model);
void listDataRecords(MXADataModel* model);
void captureSampleImage(std::vector<uint8_t> &imageBuffer);
void listUserMetaData(IDataFile::Pointer dataFile);

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int main(int argc, char **argv) {
  std::cout << "Starting Example 1." << std::endl;

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
  MXADataRecord::Pointer cameraImage = MXADataRecord::New(1, "Camera", "Camera Image");

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

  // Add some user defined Meta Data to the model
  float value = 12.234234f;
  IMXAArray::Pointer umd = MXAArrayTemplate<float>::CreateSingleValueArray(value);
  model->addUserMetaData("Float32 User Meta Data", umd);

  int32_t iMDValue = 34212;
  IMXAArray::Pointer iUmd = MXAArrayTemplate<int32_t>::CreateSingleValueArray(iMDValue);
  model->addUserMetaData("Int32 User Meta Data", iUmd);

  int32_t err = MXAXMLModelFileWriter::writeModel(modelPtr, Examples::Example1_XMLFile);

   if (err < 0)
   {
     std::cout << "Error writing model to an xml file" << std::endl;
     return -1;
   }

  // List the Data Dimensions of the model
  listDataDimensions(model);

  // List the Data Records in the model
  listDataRecords(model);

  //Write the model to a new HDF5 file, deleting any existing file and
  // allowing the Hdf5 file to remain open for further processing
  IDataFile::Pointer dataFile = H5MXADataFile::CreateFileWithModel(Examples::Example1File, modelPtr);

  if (NULL == dataFile.get() )
  {
    std::cout << "Error Writing Model to HDF5 File" << std::endl;
    return EXIT_FAILURE;
  }

  //List the User Meta Data
  listUserMetaData(dataFile);

  //Lets store some data into the HDF5 File. In our experiment we are recording the time
  // in 1 minute intervals for 10 minutes and also incrementing the pressure by
  // 200 KPa starting at 200 and ending at 800 KPa. At each combination of those
  // values we are taking the temperature and capturing an image of our sample
  hid_t fileId = dataFile->getFileId();//We need the HDF5 indentifier for the open file
  std::vector<int32_t> indices(2, 0); // we keep this for re-use during the loop
  std::string temperaturePath;
  std::string cameraImagePath;
  std::string::size_type pos = 0;
  float temperature = 1200.0f;
  std::vector<uint8_t> image;
  err = 0;
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
      pos = temperaturePath.find_last_of("/");
      std::string parentPath ( temperaturePath.substr(0, pos) );
      // Make sure the path to the dataset in the HDF5 file is already created.
      H5Utilities::createGroupsFromPath(parentPath, fileId);
      // Write the temperature value to the HDF5 File
      err = H5Lite::writeScalarDataset(fileId, temperaturePath, temperature);
      if (err < 0)
      {
        std::cout << "Error writing temperature dataset for (t,p):" << t << "," << p << std::endl;
        break;
      }
      captureSampleImage(image);
      err = H5Lite::writeVectorDataset(fileId, cameraImagePath, dims, image);
      if (err < 0)
      {
        std::cout << "Error writing image dataset for (t,p):" << t << "," << p << std::endl;
        break;
      }
    }
  }

  std::cout << "... Ending Example 1" << std::endl;
  return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void captureSampleImage(std::vector<uint8_t> &imageBuffer)
{
  imageBuffer.clear();  // Clear the Array first
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      imageBuffer.push_back(i*j);
    }
  }

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void listUserMetaData(IDataFile::Pointer dataFile)
{
  IDataModel::Pointer modelPtr = dataFile->getDataModel();
  MXAAbstractAttributes userMetaData = modelPtr->getUserMetaData();

  float* fAttr = NULL;
  IMXAArray::Pointer attr;
  std::string key;
  //std::string value;
  for (MXAAbstractAttributes::iterator iter = userMetaData.begin(); iter != userMetaData.end(); ++iter ) {
    key = (*iter).first;
    attr = (*iter).second;
    if (key.compare("Int32 User Meta Data") == 0)
    {
      // This works because we have a-priori knowledge of the type of data stored
      int32_t* valuePtr = static_cast<int32_t*>( attr->getVoidPointer(0) );
      std::cout << "Value is: " << *valuePtr << std::endl;
    }

    if (key.compare("Float32 User Meta Data") == 0)
    {
      // This works because we have a-priori knowledge of the type of data stored
      fAttr = static_cast<float*>(attr->getVoidPointer(0) );
      std::cout << "Value is: " << *fAttr << std::endl;
    }
  }
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void listDataDimensions(MXADataModel* model)
{
  //We can now get a list of the Data Dimensions and print out various properties for each
   IDataDimension::Container dims = model->getDataDimensions();
   MXADataDimension* dim = NULL; // Use a Pointer to make the code a bit easier to read
   for (IDataDimension::Container::iterator iter = dims.begin(); iter != dims.end(); ++iter )
   {
     dim = static_cast<MXADataDimension*>((*(iter)).get() );
     if (NULL == dim)
     {
       std::cout << logTime() << "Error: Dimension was NULL. " << std::endl;
       break;
     }
     std::cout << "Data Dimension:[" << dim->getIndex() << "]: " << dim->getDimensionName() << std::endl;
   }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void listDataRecords(MXADataModel* model)
{
  // We can get a list of Data Records and print out the top level records.
  // Note that Data Records are stored in a tree structure, so without any type of
  // tree traversal, this code will only print the top level records.
  IDataRecord::Container records = model->getDataRecords();
  MXADataRecord* rec = NULL; //Create a convenience pointer
  for (IDataRecord::Container::iterator iter = records.begin(); iter != records.end(); ++iter ) {
    rec = static_cast<MXADataRecord*>( (*(iter)).get() );
    std::cout << "Data Record: " << rec->getRecordName() << std::endl;
  }
}


