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
#ifndef DATASOURCEPATHBUILDER_H_
#define DATASOURCEPATHBUILDER_H_

#include <Common/MXATypes.h>
#include <Common/MXATypeDefs.h>
#include <Utilities/IStringSection.h>

/**
* @class DataSourcePathBuilder DataSourcePathBuilder.h Utilities/DataSourcePathBuilder.h
* @brief Utility class to help build up parts of a path
* @author Mike Jackson
* @date Nov 2007
* @version $Revision: 1.2 $
*/
class MXA_EXPORT DataSourcePathBuilder
{

  public:
    DataSourcePathBuilder();
    virtual ~DataSourcePathBuilder();

    /**
     * @brief Adds another section to the path
     * @param part The part to add
     */
    void addSection(IStringSectionPtr part);

    /**
     * @brief Clears the sections
     */
    void clearSections();

  private:
    IStringSections _parts;

    DataSourcePathBuilder(const DataSourcePathBuilder&);    // Copy Constructor Not Implemented
    void operator=(const DataSourcePathBuilder&);  // Operator '=' Not Implemented
};

#endif /*DATASOURCEPATHBUILDER_H_*/
