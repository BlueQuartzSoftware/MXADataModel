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
#include <MXA/Common/MXATypeDefs.h>
#include <MXA/Common/LogTime.h>
#include <MXA/HDF5/H5MXADataFile.h>
#include <MXA/HDF5/H5MXAUtilities.h>
#include <Tests/DataFileGenerator.h>
#include <Examples/ExampleFileLocations.h>

#include <hdf5.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  std::cout << logTime() << "----- Running GetData Example ------------- " << std::endl;

  herr_t err = 1;
  // Generate a Data file to use
  std::string outputFile(Examples::GetDataExampleFile);
  DataFileGenerator dfg;
  dfg.setFilePath(outputFile);
  err = dfg.generate();
  if (err < 0)
  {
    return EXIT_FAILURE;
  }


  //First load the Data file
  IDataFile::Pointer dataFile = H5MXADataFile::OpenFile(Examples::GetDataExampleFile, true);
  IDataModel::Pointer modelPtr = dataFile->getDataModel();
  hid_t fileId = dataFile.get()->getFileId();
  if (fileId < 0)
  {
    std::cout << logTime() << "Error: FileId was not valid." << std::endl;
    return -1;
  }
  // Now we can interrogate the model to get some information
  IDataDimension::Container dims = modelPtr->getDataDimensions();
  MXADataDimension* dim = NULL;
  //Loop over the Data Dimensions and print each one to std::cout
  for (IDataDimension::Container::iterator iter = dims.begin(); iter != dims.end(); ++iter )
  {
    dim = static_cast<MXADataDimension*>((*(iter)).get() );
    if (NULL == dim)
    {
      std::cout << logTime() << "Error: Dimension was NULL. " << std::endl;
      break;
    }
  }

  // We know that the data dimensions have ranges of 1->2 and 1->3, so lets get the data for the first index of each one.
  std::vector<int32_t> indices;
  indices.push_back(1); indices.push_back(1);
  // We also know the exact path to the Data Record, so lets use it to retrieve the Data Record from the Model
  IDataRecord::Pointer record = modelPtr->getDataRecordByNamedPath(DataGen::TableRec + "/" + DataGen::Float32Rec);
  if (NULL == record.get() )
  {
    std::cout << logTime() << "Error getting '2D Array' Data Record" << std::endl;
    return -1;
  }

  // Have the DataModel generate the proper internal path relative to the root level and extending to the dataset
  std::string dsetPath = H5MXAUtilities::generateH5PathToDataset(modelPtr, indices, record );

  std::vector<float> data; // This will hold our data. The next call will call 'clear' and 'resize' the vector as needed
  err = H5Lite::readVectorDataset(fileId, dsetPath, data);
  if (err < 0)
  {
    std::cout << logTime() << "Error Retrieving Dataset at " << dsetPath << std::endl;
    return -1;
  }
  //Print the data:
  for (std::vector<float>::iterator iter = data.begin(); iter != data.end(); ++iter )
  {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;

  //If you know nothing of the datatype then you will have to interrogate the dataset
  // to find out...
  //uint32_t type_size = 0;
 // H5T_class_t type_class = -1;
 // err = H5Lite::getDatasetInfo(fileId, dsetPath, dims, type_class, type_size);
  if (err < 0)
  {
    std::cout << logTime() << "Error Retrieving Dataset info for " << dsetPath << std::endl;
    return -1;
  }

  std::cout << logTime() << "------------- GetData Example Complete ------------ " << std::endl;

  return 0;
}
