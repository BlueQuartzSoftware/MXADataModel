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
#ifndef _H5MXADataFile_h_
#define _H5MXADataFile_h_

#include <MXA/Base/IDataset.h>
#include <MXA/Base/IDataModel.h>
#include <MXA/Base/IDataFile.h>



/**
 * @class H5MXADataFile H5MXADataFile.h Core/H5MXADataFile.h
 * @brief
 * @author mjackson
 * @date Dec 17, 2007
 * @version $Revision: 1.2 $
 */
class MXA_EXPORT H5MXADataFile : public IDataFile
{
  public:
    MXA_SHARED_POINTERS(H5MXADataFile)

    static IDataFile::Pointer OpenFile(const std::string &filename, bool readOnly);

    static IDataFile::Pointer CreateFileWithModel(const std::string &filename, IDataModel::Pointer model);

    static IDataFile::Pointer CreateEmptyFile(const std::string &filename);



    virtual ~H5MXADataFile();

    /**
     * @brief returns the name of the file that is actively being read from or
     * written to.
     */
    std::string getFilename();

    IDataModel::Pointer getDataModel();

    // -----------------------------------------------------------------------------
    //  Basic File Operations

    /**
     * @brief Creates a new data file, first deleting and existing file with the same name
     * @return Error code < 0 is error. 0 or positive is Success
     */
    int32_t createFile();

    /**
     * @brief Opens an existing data file.
     * @return Error code < 0 is error. 0 or positive is Success
     */
    int32_t openFile(bool readOnly);

    /**
     * @brief Closes the data file. Any further access to the file will fail.
     * @return Error code < 0 is error. 0 or positive is Success
     */
    int32_t closeFile(bool saveModel);

    /**
     * @brief Checks if the file version of the data file is with in the bounds of the library to read/parse the data model
     */
    float supportedMXAFileVersion();

    /**
     * @brief Returns if the file is currently in the "open" state. If this file
     * is OPEN then file operations (Read/Write data or model) can be performed
     * on the file
     */
    bool isFileOpen();

    /**
     * @brief Was the file opened in ReadOnly mode
     */
    bool isReadOnly();

    // -----------------------------------------------------------------------------
    //  Data Model Related Methods

    /**
     * @brief Writes the Data Model into the underlying data file overwriting any
     * existing model. This may have adverse effects on the ability to retrieve
     * data from the data file if the data model differs in structure from the original
     * data model
     * @return Error code < 0 is error. 0 or positive is Success
     */
    int32_t saveDataModel();

    // -----------------------------------------------------------------------------
    //  Writing data related methods

    int32_t writeData ( const IDataset::Pointer dataset);

    int32_t readData (const IDataset::Pointer dataset);

    int32_t getFileId();

  protected:
    H5MXADataFile(const std::string &filename);
    H5MXADataFile(const std::string &filename, IDataModel::Pointer model);

    /**
     * @brief Writes the data model to the data file. Any existing model is over-written
     * with the new/modified model
     * @return Error code < 0 is error. 0 or positive is Success
     */
    int32_t _writeDataModel();

    /**
     * @brief Loads the data model from the data file. If a data model can not be
     * loaded from the file, then a NULL wrapped model will be returned.
     * @return Error code < 0 is error. 0 or positive is Success
    */
    int32_t _readDataModel();

    void _setWeakPointer(boost::weak_ptr<IDataFile> weakPtr);


  private:
    std::string                   _filename;
    hid_t                         _fileId;
    bool                          _isFileOpen;
    bool                          _isReadOnly;
    IDataModel::Pointer                 _dataModel;
    boost::weak_ptr<IDataFile>    _weakPtr;


    H5MXADataFile(const H5MXADataFile&); //Not Implemented
    void operator=(const H5MXADataFile&); //Not Implemented

};

#endif //_H5MXADataFile_h_

