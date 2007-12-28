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
#ifndef _IFileReader_h_
#define _IFileReader_h_

#include <Common/MXATypeDefs.h>

/**
* @class IFileReader IFileReader.h Testing/IFileReader.h
* @brief 
* @author mjackson
* @date Dec 19, 2007
* @version $Revision: 1.1 $
*/
class IFileReader 
{
  public:
    IFileReader(){};
    virtual ~IFileReader(){};
      
    virtual int32 readFromFile(IDataFilePtr dataFile) = 0;
    
  protected:  
  
  private:
      IFileReader(const IFileReader&);    //Not Implemented
      void operator=(const IFileReader&); //Not Implemented
  
};

#endif //_IFileReader_h_



