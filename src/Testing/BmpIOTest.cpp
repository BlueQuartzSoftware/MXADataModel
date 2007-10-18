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
#include <Common/MXATypes.h>

//-- Boost Unit Testing Framework
#include <boost/test/unit_test.hpp>
using namespace boost::unit_test;
#include <BMPIO/MXABmpIO.h>



// -----------------------------------------------------------------------------
//  
// -----------------------------------------------------------------------------
void TestBMPIO()
{
  // Insert Test Code here.
  /**************Test stub I was using to develop class, 
  ***************needs modified for use with test framework
	MXABmpIO* reader = new MXABmpIO();
	cout<<"New MXABmpIO class created\n";
	LOAD_TEXTUREBMP_RESULT res = reader->loadBMPData("/Users/hmullens/Desktop/RoboMetData/mosaic_test3_0816_10.bmp");
	
	switch(res)
  	{
  	case LOAD_TEXTUREBMP_SUCCESS:
  		{
			cout<<"Bitmap loaded successfully\n";
			cout<<"Dimensions: "<<reader->getWidth()<<" x "<<reader->getHeight()<<endl;
			cout<<"Number of channels: "<<reader->getNumberOfChannels()<<endl;
			break;
  		}
  	case LOAD_TEXTUREBMP_COULD_NOT_FIND_OR_READ_FILE:
    	{
    		cout<<"File not found or could not be opened\n";
    		break;
    	}
  	case LOAD_TEXTUREBMP_ILLEGAL_FILE_FORMAT:
    	{
    		cout<<"File format not a valid BMP file\n";
    		break;
    	}
    case LOAD_TEXTUREBMP_OPENGL_ERROR:
    	{
    		cout<<"OPENGL ERROR\n";
    		break;
    	}
    case LOAD_TEXTUREBMP_OUT_OF_MEMORY:
    	{
    		cout<<"System ran out of memory when allocating array for bitmap data\n";
    		break;
    	}
  	default: 
    	{
    		cout<<"Unhandled error occurred"<<endl<<flush;
    		break;
    	}
  }
  */
  
  
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] ) {
    test_suite* test= BOOST_TEST_SUITE( "Bitmap IO Testing" );
    
    test->add( BOOST_TEST_CASE( &TestBMPIO), 0);

    return test; 
}