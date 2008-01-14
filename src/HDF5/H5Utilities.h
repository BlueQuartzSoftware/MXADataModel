///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007, mjackson
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//  This code was written under United States Air Force Contract number 
//                           FA8650-04-C-5229
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _HDF5UTILITIES_H_
#define _HDF5UTILITIES_H_


#include <Common/DLLExport.h>
#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <Core/MXAAttribute.h>
#include <HDF5/H5Lite.h>

// C++ Includes
#include <map>
#include <list>
#include <string>

//-- HDF Headers
#include <hdf5.h>

/**
 * @brief General Utilities for working with the HDF5 data files and API
 * @author Mike Jackson/Shawn Nicholson
 * @date March 2007
 * @version $Revision: 1.22 $
 */
class H5Utilities
{

public:
  
  virtual ~H5Utilities();

  
  #if 1
  enum CustomHDFDataTypes {
   MXA_GROUP = 1,
   MXA_DATASET = 2,
   MXA_TYPE = 4,
   MXA_LINK = 8,
   MXA_ANY = 15
  };
  #endif
  
  // -------------- HDF Indentifier Methods ----------------------------
  /**
  * @brief Retuirns the path to an object
  * @param objId The HDF5 id of the object
  * @param trim set to False to trim the path
  * @return  The path to the object relative to the objId
  */
  static MXA_EXPORT std::string getObjectPath(hid_t objId, bool trim=false);
 
  /**
  * @brief Returns the hdf object type
  * @param objId The hdf5 object id
  * @param objName The path to the data set
  * @param objType The type of the object
  * @return  Negative value on error
  */
  static MXA_EXPORT herr_t getObjectType(hid_t objId, const std::string &objName, int32 *objType);
 
  /**
  * @brief Retrieves the object name for a given index
  * @param fileId The hdf5 object id
  * @param idx The index to retrieve the name for
  * @param name The variable to store the name
  * @return Negative value is error
  */
  static MXA_EXPORT herr_t objectNameAtIndex(hid_t fileId, int32 idx, std::string &name);
 
  /**
  * @brief Returns if a given hdf5 object is a group
  * @param objId The hdf5 object that contains an object with name objName
  * @param objName The name of the object to check
  * @return True if the given hdf5 object id is a group
  */
  static MXA_EXPORT bool isGroup(hid_t objId, const std::string &objName);
  

  /**
  * @brief Opens an HDF5 object for hdf5 operations
  * @param locId the Object id of the parent
  * @param objectPath The path of the object to open
  * @return The hdf5 id of the opened object. Negative value is error.
  */
  static MXA_EXPORT hid_t openHDF5Object(hid_t locId, const std::string &objectPath);

  /**
  * @brief Closes the object id 
  * @param locId The object id to close
  * @return Negative value is error.
  */
  static MXA_EXPORT herr_t closeHDF5Object(hid_t locId);


  /**
  * @brief prints the class type of the given class
  * @param classT The Class Type to print
  */
  static MXA_EXPORT void printHDFClassType(H5T_class_t classT);

  // -------------- HDF Group Methods ----------------------------
  /**
  * @brief Returns a list of child hdf5 objects for a given object id
  * @param loc_id The parent hdf5 id
  * @param typeFilter A filter to apply to the list
  * @param names Variable to store the list
  * @return 
  */
  static MXA_EXPORT herr_t getGroupObjects(hid_t loc_id, int32 typeFilter, std::list<std::string>& names);
  
  /**
   * @brief Creates a HDF Group by checking if the group already exists. If the 
   * group already exists then that group is returned otherwise a new group is 
   * created.
   * @param loc_id The HDF unique id given to files or groups
   * @param group The name of the group to create. Note that this group name should
   * not be any sort of 'path'. It should be a single group.
   */
  static MXA_EXPORT hid_t createGroup(hid_t loc_id, const std::string &group);
  
  /**
   * @brief Given a path relative to the Parent ID, this method will create all
   * the intermediate groups if necessary.
   * @param pathToCheck The path to either create or ensure exists.
   * @param parent The HDF unique id for the parent
   * @return Error Condition: Negative is error. Positive is success.
   */
  static MXA_EXPORT herr_t  createGroupsFromPath(const std::string &pathToCheck, hid_t parent);
  
  /**
   * @brief Given a path relative to the Parent ID, this method will create all
   * the intermediate groups if necessary.
   * @param datasetPath The path to the dataset that you want to make all the intermediate groups for
   * @param parent The HDF unique id for the parent
   * @return Error Condition: Negative is error. Positive is success.
   */
  static MXA_EXPORT herr_t createGroupsForDataset(const std::string &datasetPath, hid_t parent);

  // -------------- HDF Attribute Methods ----------------------------
  /**
  * @brief Looks for an attribute with a given name
  * @param loc_id The objects Parent id
  * @param obj_name The name of the object
  * @param attr_name The attribute to look for (by name)
  * @return True if the attribute exists.
  */
  static MXA_EXPORT bool probeForAttribute(hid_t loc_id, 
                                            const std::string &obj_name, 
                                            const std::string &attr_name);


  /**
  * @brief Returns a list of all the attribute names
  * @param objId The parent object
  * @param names Variable to hold the list of attribute names
  * @return Negate value is error
  */
  static MXA_EXPORT herr_t getAllAttributeNames(hid_t objId, std::list<std::string> &names);

  /**
  * @brief Returns a list of all the attribute names 
  * @param objId The parent object
  * @param obj_name The name of the object whose attribute names you want a list
  * @param names Variable to hold the list of attribute names
  * @return Negative value is error
  */
  static MXA_EXPORT herr_t getAllAttributeNames(hid_t objId, const std::string &obj_name, 
                                                  std::list<std::string> &names);
#if 0
  THIS IS REALLY BROKEN. DO NOT USE THIS METHOD
  /**
  * @brief Returns a mapping of attribute names to attribute values. This is a pretty specialized 
  * method and should be used with great care. The outcome of the method is undefined for any type
  * of attribute except strings.
  * @param objId The hdf5 id of the parent object
  * @param obj_name The name of the object whose attributes you want a map of
  * @return A std::map<string, string> of attribute names to values
  */
  static MXA_EXPORT  herr_t H5Utilities::getAttributesMap(hid_t loc_id, 
                                                          const std::string &obj_name,
                                                          std::map<std::string, std::string> &attributes);
#endif  
  /**
   * @brief Returns a vector of IAttributes, one for each attribute of a given hdf5 object
   * @param fileId The parent hdf5 id
   * @param datasetPath The path to the hdf5 object whose attributes you want
   * @param attributes Variable to store the attributes
   * @return Negative value on error
   */
  static MXA_EXPORT herr_t readAllAttributes(hid_t fileId, 
                                             const std::string &datasetPath, 
                                             MXAAttributes &attributes);
  
/**
  * @brief Reads data from an Attribute into an IAttributePtr
  * @param locId The hdf5 object id of the parent
  * @param datasetPath The path to the data set containing the attributes you want
  * @param key The name of the attribute to read
  * @param dims The dimensions of the attribute
  * @return Boost shared pointer to the attribute
  */
  template<typename T>
  static MXAAttributePtr readPrimitiveAttribute( hid_t locId, 
                                  const std::string &datasetPath, 
                                 const std::string &key, 
                                 const std::vector<uint64> &dims)
  {
    herr_t err = -1;
    MXAAttributePtr ptr;
    if (dims.size() == 1 && dims.at(0) == 1) // One Dimensional Array with 1 element
    {
      T data;
      err = H5Lite::readScalarAttribute(locId, datasetPath, key, data);
      if (err >= 0) {   
        MXAAttributePtr attr = MXAAttribute::createAttribute(key, data);
        return attr;
      }
    } 
    else // Multi-Dimensional Data 
    {
      std::vector<T> data;
      err = H5Lite::readVectorAttribute(locId, datasetPath, key, data);
      if (err >= 0) {   
        MXAAttributePtr attr = MXAAttribute::createAttribute(key, data, dims);
        return attr;
      }
    }
    return ptr;
  }

  /**
   * @brief Creates an absolute path suitable for create an HDF5 data set.
   * @param model The data model to use
   * @param indices The indices to use for the Data dimensions
   * @param record The MXADataRecord to generate the path to
   * @return The hdf5 path
   */
  static std::string generateH5PathToDataset (  IDataModelPtr model, 
                                                std::vector<int32> &indices,  
                                                IDataRecordPtr record);
 
 /**
 * @brief Reads a dataset from an HDF5 data file into a newly allocated MXAAbstractData
 * derived object.
 * @param dataFile The IDataFile object to read from
 * @param datasetPath The internal HDF5 path to the data, relative to the root of the data file
 * @return
 */
  static MXAAbstractDataPtr readDataArray(IDataFilePtr dataFile, const std::string &datasetPath);
  
  /**
  * @brief Reads an attribute from an HDF5 data file into a newly allocated MXAAbstractAttribute
  * derived object.
  * @param dataFile The IDataFile object to read from
  * @param datasetPath The internal HDF5 path to the data, relative to the root of the data file
  * @param attributeKey The name of the attribute to read
  * @return
  */ 
  static MXAAbstractAttributePtr readAttributeArray(IDataFilePtr dataFile, const std::string &datasetPath, const std::string &attributeKey);
  
  
  
protected:
  H5Utilities() {}; //This is just a bunch of Static methods
  
private:
    H5Utilities(const H5Utilities&);   //Copy Constructor Not Implemented
    void operator=(const H5Utilities&); //Copy Assignment Not Implemented
};



#endif /* _HDF5UTILITIES_H_ */

