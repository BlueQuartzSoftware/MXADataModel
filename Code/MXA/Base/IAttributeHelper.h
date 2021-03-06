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
#ifndef _IATTRIBUTECONVERTER_H_
#define _IATTRIBUTECONVERTER_H_

#include "MXA/MXADLLExport.h"
#include <MXA/Base/IAttributeWriter.h>

/**
 * @brief Abstract Class that defines an interface for a helper class for Attributes. 
 * The duty of the helper class is to convert the actual attribute value to a string
 * 
 * @author Mike Jackson
 * @date Mach 2007
 * @version $Revision: 1.2 $
 *   MXADataModel/IAttributeHelper.h
 */
class MXA_EXPORT IAttributeHelper 
{
  public:
    IAttributeHelper() {};
    virtual ~IAttributeHelper() {};

    /**
     * @brief Converts the stored value into a string
     * @param sstream The stringstream to use for the conversion
     */
    virtual void convert(std::stringstream &sstream) = 0;

    /**
     * @brief Writes the stored value to a disk using the IAttributeWriter object
     * @param locationId A unique ID
     * @param datasetPath The Path to the dataset that this attribute belongs to
     * @param key The attribute Key
     * @param writer IAttributeWriter object to use for the writing
     * @return Error condition
     */
    virtual int32_t writeAttribute(int32_t locationId, std::string &datasetPath, std::string &key, IAttributeWriter &writer) = 0;
};

#endif /* _IATTRIBUTECONVERTER_H_ */

