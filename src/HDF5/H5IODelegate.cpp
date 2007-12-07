
//-- MXA Includes
#include "H5IODelegate.h"
#include <Common/LogTime.h>
#include <HDF5/H5Lite.h>
#include <HDF5/H5DataModelReader.h>
#include <HDF5/H5DataModelWriter.h>
#include <HDF5/H5Utilities.h>

//-- Boost Filesystem Headers
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5IODelegate::H5IODelegate() :
  _fileId(-1),
  _openFile("")
{
#ifndef DEBUG
#warning HDF Error Handlers are OFF
  HDF_ERROR_HANDLER_OFF;
#endif
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5IODelegate::~H5IODelegate()
{
  if (_fileId > 0) {
    //std::cout << "H5IODelegate::~H5IODelegate()  Closing file.." << std::endl;   
    this->closeMXAFile();
  }
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
MXATypes::MXAError H5IODelegate::writeModelToFile(const std::string &fileName, 
                                                  IDataModel* model, 
                                                  bool closeWhenFinished,
                                                  bool deleteExisting)
{
  int32 success = -1;
  bool didDeleteFile = true;
  //_fileId = -1;
  if (deleteExisting == true)
  {
    if (_fileId > 0 && (this->_openFile.compare(fileName) == 0 ) )
    {
      this->closeMXAFile(); // Close the file first if it is open.
    }
    // Now delete the file if it really exists
    if (true == boost::filesystem::exists(fileName) )
    {
      didDeleteFile = boost::filesystem::remove(fileName);
      if (false == didDeleteFile)
      {
        std::cout << "H5IODelegate::writeModelToFile: The file could not be deleted\n  " << fileName << std::endl;
        return -20;
      }
    }
  }
  
  // Model file is NOT open and the filenames do NOT match
  if (_fileId < 0 && (this->_openFile.compare(fileName) != 0))
  {
    // Named file is not open, try opening it first
    HDF_ERROR_HANDLER_OFF
    _fileId = H5Fopen(fileName.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
    HDF_ERROR_HANDLER_ON
    if (_fileId < 0) // The data file does NOT exist-Create it and write the model
    {
      //Create the HDF File
      _fileId = H5Fcreate(fileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
      if (_fileId < 0)
      {
        std::cout << "Error Creating new MXA File" << std::endl;
        return _fileId;
      }
    }
    this->_openFile = fileName;
  }
  
  // A different File is open
  if (_fileId > 0 && (this->_openFile.compare(fileName) != 0) )
  {
#if DEBUG
    std::cout << "H5IODelegate::writeModelToFile - Error: This method was called with a different filename " 
    << " than the currently open data file has.\nPlease use H5IODelegate.closeMXAFile() first to close the" 
    << " current file, then call this method or \ncreate an IFileIODelegate object on the stack and pass that object as an argument." << std::endl;
#endif
    return -1010;
  }

  // A file IS open, and it is the same as the parameter fileName
  if (_fileId > 0 && (this->_openFile.compare(fileName) == 0 ) )
  {
    // Write or Overwrite the model information
    H5DataModelWriter writer(this, model);
    success = writer.writeModelToFile(_fileId);
    //Close the file as we are done with it.
    if (closeWhenFinished)
    {
      closeMXAFile();
    }
    else
    {
      this->_openFile = fileName;
    }
  }
  return success;
  
}

// -----------------------------------------------------------------------------
//  Opens the file and then reads the data model from the file. Then closes the
//  when reading is complete.
// -----------------------------------------------------------------------------
MXATypes::MXAError H5IODelegate::readModelFromFile(const std::string &fileName, 
                                                  IDataModel* model, 
                                                  bool closeWhenFinished,
                                                  bool openReadOnly)
{
  //Open the HDF File
  _fileId = openMXAFile(fileName, openReadOnly);
  if (_fileId < 0)
  {
    return _fileId;
  }
  //Instantiate the H5DataModelReader object
  H5DataModelReader reader(this, model);
  herr_t error = reader.readDataModel(_fileId);
  //Close the file as we are done with it.
  if (closeWhenFinished)
  {
    closeMXAFile();
  }
  else
  {
    this->_openFile = fileName;
  }
  return error;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
bool H5IODelegate::supportedMXAFileVersion(float version)
{
  if (version == MXA::MXACurrentFileVersion) {
    return true;
  } else {
    return false;
  }
}

// -----------------------------------------------------------------------------
// Returns true if the file referred to by fileId is an HDF5
//  compliant file (has the correct file type and version)
// -----------------------------------------------------------------------------
bool H5IODelegate::isMXAFile(hid_t fileId)
{
  herr_t err=0;
  
  // Check the Type
  std::string ftype;
  err = H5Lite::readStringAttribute(fileId, MXA::DataModelPath, MXA::ModelType, ftype);
  
  if ( (ftype.compare(MXA::MXACurrentFileType)) != 0 ) {
    std::cout << "NOT an MXA formatted HDF5 File. Type should be " << MXA::MXACurrentFileType << " but I found:" << ftype << std::endl;
    return false;
  }
  if (err < 0)
  {
    std::cout << logTime() << "H5IODelegate::isMXAFile: Error Getting the FileType. Look for other log messages" << err << std::endl;
    return false;
  }
  // Check the Version
  float32 fileVersion;
  err = H5Lite::readScalarAttribute(fileId, MXA::DataModel, MXA::ModelVersion, fileVersion);
  
  if (! supportedMXAFileVersion(fileVersion)) {
    std::cout << "Not a supported MXA Formatted HDF5 File. File Version should be " << MXA::MXACurrentFileVersion << " but I found: "  << fileVersion << std::endl;
    return false;
  }
  if (err < 0)
  {
    std::cout << logTime() << "H5IODelegate::isMXAFile: Error Getting the File Version. Look for other log messages" << err << std::endl;
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------
// Returns true if the file referred to by filename is an MXA
//  compliant file (has the correct file type and version)
// -----------------------------------------------------------------------------
bool H5IODelegate::isMXAFile(const std::string &fileName)
{
  hid_t fileId = openMXAFile(fileName, true);
  if (fileId < 0) {
    return false;
  }
  closeMXAFile();
  return true;
}


// -----------------------------------------------------------------------------
// Opens and returns an hdf file identifier
// -----------------------------------------------------------------------------
hid_t H5IODelegate::openMXAFile(std::string filename, bool readOnly)
{
  // Suspend the HDF Error Handlers
  herr_t (*_oldHDF_error_func)(void *);
  void *_oldHDF_error_client_data;

  // Store the current HDF error handlers
  H5Eget_auto(&_oldHDF_error_func, &_oldHDF_error_client_data);

  // Turn off error handling
  H5Eset_auto(NULL, NULL);

  if (readOnly) {
    _fileId = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  } else {
    _fileId = H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
  }

  // Turn the HDF Error handlers back on
  H5Eset_auto(_oldHDF_error_func, _oldHDF_error_client_data);

  if (_fileId < 0) {
    return _fileId;
  }


  // Make sure this is an HDF5 file
  if (! isMXAFile(_fileId)) {
    closeMXAFile();
    return (hid_t) -1;
  } 
  this->_openFile = filename;
  return _fileId;
}


// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5IODelegate::closeMXAFile() 
{
  if (_fileId < 0) {  // fileId isn't open
    return;
  }
  
  // Get the number of open identifiers of all types
  //  except files
  int32 num_open = H5Fget_obj_count(_fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |
			      H5F_OBJ_DATATYPE | H5F_OBJ_ATTR);
  if (num_open > 0) {
    std::cout << "WARNING: Some IDs weren't closed. Closing them."  << std::endl;
    std::vector<hid_t> attr_ids(num_open, 0);
    H5Fget_obj_ids(_fileId, H5F_OBJ_DATASET | H5F_OBJ_GROUP |
		   H5F_OBJ_DATATYPE | H5F_OBJ_ATTR, 
		   num_open, &(attr_ids.front()) );
    for (int i=0; i<num_open; i++) {
      H5Utilities::closeHDF5Object(attr_ids[i]);
    }
  }
    
  herr_t err = H5Fclose(_fileId);
  if (err < 0) {
    std::cout << logTime() << "H5IODelegate::closeMXAFile(): H5Fclose() caused error " << err << std::endl;
  }
  this->_fileId = -1;
  this->_openFile = "";
}



