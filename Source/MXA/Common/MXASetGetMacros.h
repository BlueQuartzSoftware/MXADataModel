///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2009, Michael A. Jackson. BlueQuartz Software
//  All rights reserved.
//  BSD License: http://www.opensource.org/licenses/bsd-license.html
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _MXASetGetMacros_h_
#define _MXASetGetMacros_h_

#include <string>
#include <iostream>

//-- Boost includes
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

//-- Boost Includes
#include <boost/shared_ptr.hpp>

/**
 * @brief Creates a static method that returns a NULL pointer wrapped in a
 * boost::shared_ptr<>
 * @param thisClass The name of the class.
 */
#define MXA_NULL_SHARED_POINTER(thisClass)\
  static Pointer NullPointer(void)\
  { \
    return Pointer(static_cast<thisClass*>(NULL));\
  }

/**
 * @brief Creates some basic typedefs that can be used throughout the code to
 * reference the class.
 */
#define MXA_SHARED_POINTERS(thisClass)\
  typedef thisClass                      Self;\
  typedef boost::shared_ptr<Self>        Pointer;\
  typedef boost::shared_ptr<const Self>  ConstPointer;\
  MXA_NULL_SHARED_POINTER(thisClass);

/**
 * @brief Implements a Static 'New' Method for a class
 */
#define MXA_STATIC_NEW_MACRO(thisClass) \
static Pointer New(void) \
{ \
  Pointer sharedPtr (new thisClass); \
  return sharedPtr; \
}

/** Macro used to add standard methods to all classes, mainly type
 * information. */
#define MXA_TYPE_MACRO(thisClass) \
    virtual const char *GetNameOfClass() const \
        {return #thisClass;}

//------------------------------------------------------------------------------
// Macros for Properties
/**
* @brief Creates a std::string constant for the Property so that the property
* can be retrieved by name.
*/
#define MXA_PROPERTY_CONSTANT(prpty) \
  const std::string prpty ( #prpty );

/**
* @brief Creates a "setter" method to set the property.
*/
#define MXA_SET_PROPERTY(type, prpty, varname) \
  void set##prpty(type value) { this->varname = value; }

/**
* @brief Creates a "getter" method to retrieve the value of the property.
*/
#define MXA_GET_PROPERTY(type, prpty, varname) \
  type get##prpty() { return varname; }

/**
* @brief Convenience macro to create both the setter and getter methods.
*/
#define MXA_PROPERTY(type, prpty, varname) \
  MXA_SET_PROPERTY(type, prpty, varname)\
  MXA_GET_PROPERTY(type, prpty, varname)

/**
* @brief Convenience macro to create both the setter and getter methods in addition
* to the instance variable.
*/
#define MXA_INSTANCE_PROPERTY(type, prpty, varname)\
  private:\
      type   varname;\
  public:\
    MXA_SET_PROPERTY(type, prpty, varname)\
    MXA_GET_PROPERTY(type, prpty, varname)



/**
* @brief Creates a "setter" method to set the property.
*/
#define MXA_SET_STRING_PROPERTY( prpty, varname) \
  void set##prpty(const std::string &value) { this->varname = value; }

/**
* @brief Creates a "getter" method to retrieve the value of the property.
*/
#define MXA_GET_STRING_PROPERTY( prpty, varname) \
  std::string get##prpty() { return varname; }

/**
 * @brief Creates setters and getters in the form of 'setXXX()' and 'getXXX()' methods
 */
#define MXA_STRING_PROPERTY(prpty, varname)\
  MXA_SET_STRING_PROPERTY(prpty, varname)\
  MXA_GET_STRING_PROPERTY(prpty, varname)

#define MXA_INSTANCE_STRING_PROPERTY(prpty, varname)\
  private:\
  std::string      varname;\
  public:\
  MXA_SET_STRING_PROPERTY(prpty, varname)\
  MXA_GET_STRING_PROPERTY(prpty, varname)

/**
* @brief Creates an if conditional where the key is tested against the values constant
* and if a match found then the property value is set
* @param name_space The Namespace for the constants
* @param type The primitive type of the variable
* @param prpty the name of the property, usually with CamelCase spelling
* @param key The Key used for the property
* @param value The value of the property
*/
#define SET_PROPERTY_BODY(name_space, type, prpty, key, value) \
  if (name_space::prpty.compare(key) == 0) { \
    try { \
      this->set##prpty(boost::lexical_cast<type>(value)); return 1; \
    }  catch(boost::bad_lexical_cast &excp) { \
      std::cout << excp.what() << std::endl; \
      std::cout << "Could not convert value '" << value << "' to type '" << #type << "' for property '" << #prpty << "'" << std::endl; \
    } \
  }

/**
* @brief Creates an if condition where the key is tested against the values constant
* and if a match found the the property is returned using the 'value' argument
* @param name_space The Namespace for the constants
* @param type The primitive type of the variable
* @param prpty the name of the property, usually with CamelCase spelling
* @param varname The internal variable name used to store the property
* @param key The Key used for the property
* @param value The value of the property
*/
#define GET_PROPERTY_BODY(name_space, type, prpty, varname, key, value)\
  if (name_space::prpty.compare(key) == 0) {  \
  try { value = boost::any_cast<type>(varname); return 1;} \
  catch(boost::bad_any_cast &) { std::cout << "Could not cast value '" << value << "' to type '" << #type << "' for property '" << #prpty << "'" << std::endl; } }

//
////////////////////////////////////////////////////////////////////////////////






#endif /* _MXASetGetMacros_h_  */
