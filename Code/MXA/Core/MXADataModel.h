//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, Mike Jackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _MXADATAMODEL_H_
#define _MXADATAMODEL_H_


//------- Utility Methods
//TODO: Method to copy the model from a file and write it to a new file
//TODO: Static Method to Read from the various types of files (text, xml, H5) and create the model


//-- MXA Headers

//#include <MXA/Common/MXASetGetMacros.h>
#include <MXA/Base/IDataFileIO.h>
#include <MXA/Base/IDataModel.h>
//#include <MXA/Base/IMXAArray.h>
#include <MXA/Base/IRequiredMetaData.h>
#include <MXA/Base/IDataDimension.h>
#include <MXA/Base/IDataRecord.h>



//-- STL Headers
#include <string>
#include <map>

typedef std::map<std::string, std::string>        MXARequiredMetaData;

/**
 * @brief Main class used to work with the DataModel paradigm.
 * @author Mike Jackson
 * @date March 2007
 * @version 1.36
 * @class MXADataModel Core/MXADataModel.h Core/MXADataModel.h
 */
class MXA_EXPORT MXADataModel : public IDataModel
{

 public:
//   MXADataModel();
   virtual ~MXADataModel();

   MXA_SHARED_POINTERS(MXADataModel);
   MXA_TYPE_MACRO(MXADataModel);

  /**
   * @brief Static method to create a new blank model.
   * @return A boost shard_ptr to the newly created model
   */
  static MXADataModel::Pointer New(float modelVersion = 0.4f,
                                  const std::string &type = MXA::MXACurrentFileType,
                                  const std::string &dataRoot= "Data");

  /**
   * @brief Static method to create a new by creating a deep copy of
   * another model.
   * @return A boost shard_ptr to the newly created model
   */
   static MXADataModel::Pointer New(IDataModel::Pointer model);

  /**
   * @brief Validates that all the required Meta data is not empty
   * @param requiredMetaData
   * @param message String to store messages regarding missing field
   * @return Error - Negative is error. Zero or Positive is success
   */
  static int32_t validateRequiredMetaData(MXARequiredMetaData &requiredMetaData, std::string &message);

  /**
   * @brief Returns the MXA File version that the model adheres to
   * @return MXA API Version
   */
  float getModelVersion();
  void setModelVersion(float version);

  /**
   * @brief Returns the type of file, should be MXA
   * @return The Model Type. Should be MXA::CurrentModelType
   */
  std::string getModelType();

  /**
   * @brief Sets the file type. Should be MHD or MXA
   * @param modelType The Model Type for this model. Typically it should be MXA::CurrentModelType
   */
  void setModelType(const std::string &modelType);

  /**
   * @brief Sets the model type and version to the latest supported by this API
   */
  void setDefaultTypeAndVersion();

  /**
   * @brief Sets the "Data Root"
   * @param  dataRoot
   */
  void setDataRoot(const std::string &dataRoot);

  /**
   * @brief Returns the Data Root Value
   * @return
   */
  std::string getDataRoot();

  /**
   * @brief Adds a Data Dimension to the end of the Data Dimension data structure
   * thus making the index of the added dimension the largest of all the dimensions.
   * @param dimension The IDataDimension to be added to the list of dimensions.
   */
  void addDataDimension(IDataDimension::Pointer dimension);

  /**
   * @brief Creates and adds a new Data Dimension to the Model
   * @param name The name for the Data Dimensions
   * @param altName An Alternate Name for the Data Dimension
   * @param count The total number of
   * @param startValue The value this dimension starts at
   * @param endValue The ending value (inclusive) this dimension ends at
   * @param increment The value to increment the dimension when iterating
   * @param uniform Are the values uniform across the start to end values
   * @return A boost::shared_ptr to the newly created Data Dimension
   */
  IDataDimension::Pointer addDataDimension(std::string name, std::string altName,
      int32_t count, int32_t startValue,
      int32_t endValue, int32_t increment, int32_t uniform);

  /**
   * @brief Inserts a Data Dimension at a given index position. If Needed the
   * internal data structure holding the Data Dimenions will be expanded by adding
   * NULL data dimension pointers into the data structure.
   * @param dimension The Data Dimension to Insert
   * @param index The index to insert the Data Dimension at
   * @return Error condition
   */
  int32_t insertDataDimension(IDataDimension::Pointer dimension, int32_t index);

  /**
   * @brief This method will remove any NULL Data Dimensions from the internal
   * data structure that holds the list of Data Dimensions
   */
  void squeezeDataDimensions();

  /**
   * @brief Removes a data dimension by index
   * @param index
   */
  int32_t removeDataDimension(int32_t index);

  /**
   * @brief Removed a Data Dimension by Name
   * @param dimensionName The 'name' property of the Data Dimension to remove
   * @return Error: Negative is error condition
   */
  int32_t removeDataDimension(const std::string &dimensionName);

  /**
   * @brief moves the data dimension at index 'fromIndex' to another index
   * 'newIndex'. All Data Dimensions have new indices assigned to them after
   * this operation.
   * @param fromIndex The current index of the data dimension to move
   * @param toIndex The new index to place the data dimension at
   * @return Error condition
   */
  int32_t moveDataDimension(int32_t fromIndex, int32_t toIndex);

  /**
   * @brief Swaps a pair of Data Dimensions in the index list
   * @param index1 The first Data Dimension
   * @param index2 The second Data Dimension to swap with the first
   * @return Error Condition
   */
  int32_t swapDataDimensions(int32_t index1, int32_t index2);

  /**
   * @brief Returns the vector of Data Dimenions
   * @return
   */
  IDataDimension::Container& getDataDimensions();

  /**
   * @brief Returns a Data dimension object given an index
   * @param index
   * @return
   */
  IDataDimension::Pointer getDataDimension(int32_t index);

  /**
   * @brief Returns a Data Dimension object given the name of the data dimension
   * @param dimName
   * @return
   */
  // IDataDimension* getDataDimension(std::string dimName);
  IDataDimension::Pointer getDataDimension(std::string dimName);

  /**
   * @brief
   * @return The Number of data dimensions
   */
  int32_t getNumberOfDataDimensions();

  /**
   * @brief Adds a Data Record given a DataRecord shared pointer
   * @param record
   */
  void addDataRecord(IDataRecord::Pointer record);

  /**
   * @brief Adds a Data Record to the model with the given parent record as the data
   * records parent
   * @param record The data record to add to the model
   * @param parent The parent of the data record
   * @return
   */
  void addDataRecord(IDataRecord::Pointer record, IDataRecord::Pointer parent);

  /**
   * @brief Removes the Data Record from the Model
   * @param record The record to remove from the Data model
   * @return Error Condition
   */
  int32_t removeDataRecord(IDataRecord::Pointer record);

  /**
   * @brief Returns the Hierarchy of Data Records
   * @return
   */
  IDataRecord::Container& getDataRecords();

  /**
   * @brief Returns a Data Record object from the given path
   * @param path The path to the data record
   * @param parent The parent data record
   * @return
   */
  IDataRecord::Pointer getDataRecordByNamedPath(const std::string &path, IDataRecord* parent=NULL);

  /**
   * @brief Returns a Data Record based on an internal path representation
   * @param path
   * @param parent
   * @return
   */
  IDataRecord::Pointer getDataRecordByInternalPath(const std::string &path, IDataRecord* parent=NULL);


  /**
   * @brief Sets the required meta data fields
   * @param researcherName
   * @param dateCreated
   * @param datasetName
   * @param description
   * @param distributionRights
   * @param releaseNumber
   * @param pedigree
   * @param derivedSrcFile
   * @return
   */
  int32_t setRequiredMetaData( std::string researcherName,
      std::string dateCreated,
      std::string datasetName,
      std::string description,
      std::string distributionRights,
      std::string releaseNumber,
      std::string pedigree,
      std::string derivedSrcFile);

  /**
   * @brief Sets the Required Meta Data Fields given a std::map object
   * @param metadata
   * @return
   */
  int32_t setRequiredMetaData(std::map<std::string, std::string> &metadata);

  /**
   * @brief Sets the required meta data
   * @param metaData IRequiredMetaData::Pointer object
   */
  int32_t setRequiredMetaData(IRequiredMetaData::Pointer metaData);

  /**
   * @brief Returns the RequiredMeta Data in the provided std::map
   * @return IRequiredMetaData::Pointer
   */
  IRequiredMetaData::Pointer getRequiredMetaData();

  /**
    * @brief Adds User defined Meta data to the model
    * @param attributeKey The attribute key name
    * @param umd The Key/Value pair to append to the model
   */
  void addUserMetaData (const std::string &attributeKey, IMXAArray::Pointer umd);

  /**
   * @brief Removes specific user meta-data entry
   * @param attributeKey The user meta-data to remove
   */
    void removeUserMetaData(const std::string &attributeKey);

  /**
   * @brief Returns a specific User meta data item
   * @param attributeKey The value of the attribute key
   */
  IMXAArray::Pointer getUserMetaData(const std::string &attributeKey);

  /**
   * @brief Sets all the user defined meta data for this model.Any previously
     * defined meta data will be erased.
   * @param attributes Vector of MXAAbstractAttributePtr objects
   */
  void setUserMetaData( MXAAbstractAttributes &attributes);


  /** @brief Returns the Data Structure used to hold the user defined Meta Data */
  MXAAbstractAttributes getUserMetaData();

// ------------------------------------------------------------------------------
// Arbitrary Support Files methods

  /**
   * @brief Adds a SupportFile Reference to the model
   * @param supportFile An ISupportFile object or subclass
   * @param updateIndex Update the internal index of the supportFile Object after
   * it is added to the model
   */
  void addSupportFile(ISupportFile::Pointer supportFile, bool updateIndex = false);

  /**
   * @brief Returns the list of SupportFile objects from the model.
   * @return The list of Support files for this model.
   */
  ISupportFile::Container getSupportFiles();

  /**
   * @brief Returns a specific ISupportFile instance from the model or a NULL
   * wrapped pointer in case nothing was found or index is out of bounds.
   * @param index The index (zero based) of the ISupportFile instance to return
   * @return Boost Shared Pointer wrapping an ISupportFile subclass instance or NULL
   * if nothing was found.
   */
  ISupportFile::Pointer getSupportFile(int index);

// ------------------------------------------------------------------------------
// Printing the model methods
  /**
   * @brief Prints the Data Model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printModel(std::ostream &os, int32_t indent);

  /**
   * @brief Prints the Data Records section of the model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printDataRecords(std::ostream &os, int32_t indent);

  /**
   * @brief Prints the Data Dimension section of the model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printDataDimensions(std::ostream &os, int32_t indent);

  /**
   * @brief Prints the Required Meta Data section of the model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printRequiredMetaData(std::ostream &os, int32_t indent);

  /**
   * @brief Prints the User Meta Data section of the model to the provided std::ostream
   * @param os
   * @param indent The number of spaces to use for indenting
   * @return
   */
  void printUserMetaData(std::ostream &os, int32_t indent);


   /**
    * @brief Performs some basic checks to make sure the model is valid.
    * @param message String to store messages relating to errors/omissions about the model
    */
   bool isValid(std::string &message);

  protected:
    MXADataModel();

  private:
   MXADataModel(const MXADataModel&);   //Copy Constructor Not Implemented
   void operator=(const MXADataModel&); //Copy Assignment Not Implemented

   float _fileVersion;
   std::string _fileType;

   //Holds the 'path' to the root of the actual data in the data portion of the file
   std::string    _dataRoot;
   // Holds a vector of data dimensions
   IDataDimension::Container _dataDimensions;
   //Holds a vector of hierarchacally ordered Data Records
   IDataRecord::Container    _dataRecords;
   //Fields to hold the Required Meta Data Fields
   IRequiredMetaData::Pointer _requiredMetaData;
   // Holds the arbitrary User Meta Data
   MXAAbstractAttributes _userMetaData;
   // Holds the SUpport files references
   ISupportFile::Container         _supportFiles;
};


#endif /* _MXADATAMODEL_H_ */
