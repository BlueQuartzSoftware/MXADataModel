#include <HDF5/H5TiffImportDelegateFactory.h>
#include <HDF5/H5TiffImportDelegate.h>



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5TiffImportDelegateFactory::H5TiffImportDelegateFactory() :
  _fileNotFoundIsError(false),
  _importAsGrayScale(false)
{
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
H5TiffImportDelegateFactory::~H5TiffImportDelegateFactory()
{
  
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
IImportDelegatePtr H5TiffImportDelegateFactory::newDataImportDelegate (const std::string &className )
{
  IImportDelegatePtr delegate; // Creates a Null Shared Pointer
  if ( className.compare( H5TiffImportDelegateFactory_Detail::ClassName ) == 0)
  {
    H5TiffImportDelegate* d = new H5TiffImportDelegate();
    delegate.reset ( d );
    d->setFileNotFoundIsError(this->_fileNotFoundIsError);
    d->setImportAsGrayScale(this->_importAsGrayScale);
  }
  return delegate;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
std::string H5TiffImportDelegateFactory::delegateClassName()
{
  return H5TiffImportDelegateFactory_Detail::ClassName;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5TiffImportDelegateFactory::setFileNotFoundIsError(bool value)
{
  this->_fileNotFoundIsError = value;
}

// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void H5TiffImportDelegateFactory::setImportAsGrayScale(bool value)
{
  this->_importAsGrayScale = value;
}
