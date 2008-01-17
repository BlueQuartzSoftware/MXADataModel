#ifndef _XMLMXAAttributeWriter_h_
#define _XMLMXAAttributeWriter_h_

#include <Common/MXATypeDefs.h>
#include <Core/MXAAbstractAttribute.h>
#include <hdf5.h>
/**
* @class XMLMXAAttributeWriter XMLMXAAttributeWriter.h XML/XMLMXAAttributeWriter.h
* @brief 
* @author mjackson
* @date Jan 15, 2008
* @version $Revision: 1.1 $
*/

class MXA_EXPORT XMLMXAAttributeWriter 
{
  public:
    XMLMXAAttributeWriter(boost::shared_ptr<std::ofstream> ofstreamPtr) :
      _ofstreamPtr(ofstreamPtr)
      {}
    
    virtual ~XMLMXAAttributeWriter() {} 
    
    // -----------------------------------------------------------------------------
    //  Does indenting for pretty printing the XML
    // -----------------------------------------------------------------------------
      std::string indent(int depth)
      {
        const int32 indentSize = 2;
        return std::string(indentSize * depth, ' ');
      }
      
      // -----------------------------------------------------------------------------
      //  
      // -----------------------------------------------------------------------------
    int32 writeAttribute(MXAAbstractAttributePtr attribute)
    {
      hid_t typeId = attribute->getDataType();
      herr_t err = -1;
      if (typeId == H5T_NATIVE_FLOAT)
      {
        err = this->writeAttributeData<float32>(attribute);
      }
      else if (typeId == H5T_NATIVE_DOUBLE)
      {
        err = this->writeAttributeData<float64>(attribute);
      }

      else if (typeId == H5T_NATIVE_INT8)
      {
        err = this->writeAttributeData<int8>(attribute);
      }
      else if (typeId == H5T_NATIVE_UINT8)
      {
        err = this->writeAttributeData<uint8>(attribute);
      }

      else if (typeId == H5T_NATIVE_INT16)
      {
        err = this->writeAttributeData<int16>(attribute);
      }
      else if (typeId == H5T_NATIVE_UINT16)
      {
        err = this->writeAttributeData<uint16>(attribute);
      }

      else if (typeId == H5T_NATIVE_INT32)
      {
        err = this->writeAttributeData<int32>(attribute);
      }
      else if (typeId == H5T_NATIVE_UINT32)
      {
        err = this->writeAttributeData<uint32>(attribute);
      }

      else if (typeId == H5T_NATIVE_INT64)
      {
        err = this->writeAttributeData<int64>(attribute);
      }
      else if (typeId == H5T_NATIVE_UINT64)
      {
        err = this->writeAttributeData<uint64>(attribute);
      }
      
      else if (typeId == H5T_STRING)
      {
        err = this->writeStringAttributeData(attribute);
      }
      else 
      {
        std::cout << logTime() << "typeId for Attribute could not be determined." << "\n      " << "Source File: " << __FILE__ << "(" << __LINE__ << ")" << std::endl;
      }
      return err;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    template<typename T>
    int32 writeAttributeData(MXAAbstractAttributePtr attribute)
    {
      std::ofstream &stream = *(_ofstreamPtr.get());
      T* value = static_cast<T*> (attribute->getVoidPointer(0) );
      uint64 nElements = attribute->getNumberOfElements();
      int32 size = attribute->getNumberOfDimensions();
 
      std::string sType = H5Lite::HDFTypeForPrimitiveAsStr(value[0]);
      stream << indent(5) << "<UserMetaData key=\"" << attribute->getAttributeKey() << "\" dims=\"" ;
      std::vector<uint64> dims(size, 0);
      attribute->getDimensions( &(dims.front() ) );
   
      // Write the values of each dimension to the file
      for (int i = 0; i < size; ++i)
      {
        stream << dims[i];
        if (i < size - 1)
        {
          stream << " ";
        }
      }
      stream << "\" type=\"" << sType << "\">";
      
      // Now Write the data
      for(uint64 i = 0; i < nElements; ++i) 
      {
       if ( i%dims[0] == 0)
        {
          stream << "\n" << indent(5);
        }
        if (sizeof(T) != 1 )
        {
          stream  << value[i];
        }
        else
        {
          stream  << static_cast<int32>(value[i]);
        } 
        stream << " ";
       
      }
      stream << "\n"<< indent(5) << "</UserMetaData>\n";
      return 1;
    }
    
// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
    int32 writeStringAttributeData(MXAAbstractAttributePtr attribute)
    {
      std::ofstream &stream = *(_ofstreamPtr.get());
      stream << indent(5) << "<UserMetaData key=\"" << attribute->getAttributeKey() << "\" dims=\"1\" type=\"H5T_STRING\">";
      char* s = static_cast<char*>(attribute->getVoidPointer(0));
      uint64 nElements = attribute->getNumberOfElements();
      if (s[nElements-1] == 0) // Null Terminated
      {
        nElements -= 1;
      }
      std::string str(s, nElements);
      
      stream << str << "</UserMetaData>" << std::endl;
      return 1;
    }
    
  protected:  
  
  private:
    boost::shared_ptr<std::ofstream> _ofstreamPtr;
    
    XMLMXAAttributeWriter(const XMLMXAAttributeWriter&);    //Not Implemented
    void operator=(const XMLMXAAttributeWriter&); //Not Implemented
  
};

#endif //_XMLMXAAttributeWriter_h_



