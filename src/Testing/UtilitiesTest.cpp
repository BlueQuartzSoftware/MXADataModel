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

#include <MXAConfiguration.h>

#include <Common/MXATypeDefs.h>
#include <Common/MXATypes.h>
#include <Utilities/DataSourcePathBuilder.h>
#include <Utilities/IStringSection.h>
#include <Utilities/DataSourcePathTextSection.h>
#include <Utilities/DataSourcePathIndexSection.h>

#include <iostream>

#include <boost/test/unit_test.hpp>

using boost::unit_test::test_suite;


void DataSourcePathTest()
{
#if 0
  // Scope for each test
  {
  DataSourcePathBuilder builder;
  
  IStringSectionPtr f0 ( new DataSourcePathTextSection("/Users/mjackson/Data/Timestep-") );
  builder.addSection(f0);
  
  IStringSectionPtr f1 ( new DataSourcePathIndexSection(0, '0', 4, DataSourcePathSection::IntegerType) );
  builder.addSection(f1);
  
  IStringSectionPtr f3 ( new DataSourcePathTextSection("/slice_"));
  builder.addSection(f3);
  
  IStringSectionPtr f4 ( new DataSourcePathIndexSection(1, '0', 3, DataSourcePathSection::IntegerType) );
  builder.addSection(f4);
  
  IStringSectionPtr f5 ( new DataSourcePathTextSection(".data") );
  builder.addSection(f5);
  int8 ok = false;
  std::vector<uint32> dims;
  dims.push_back(23);
  dims.push_back(234);
  std::string path = builder.build(dims, ok);
   //std::cout << "Built Path: " << path << std::endl;
  BOOST_REQUIRE( path.compare("/Users/mjackson/Data/Timestep-0023/slice_234.data") == 0);
  BOOST_REQUIRE(ok == true);
  }
  
  // Scope for each test - This test will short the values of the dimensions so there 
  // is not enough values in the dimension vector. This should produce an empty string
  {
    DataSourcePathBuilder builder;
    
    IStringSectionPtr f0 ( new DataSourcePathTextSection("/Users/mjackson/Data/Timestep-") );
    builder.addSection(f0);
    
    IStringSectionPtr f1 ( new DataSourcePathIndexSection(0, '0', 4, DataSourcePathSection::IntegerType) );
    builder.addSection(f1);
    
    IStringSectionPtr f3 ( new DataSourcePathTextSection("/slice_"));
    builder.addSection(f3);
    
    IStringSectionPtr f4 ( new DataSourcePathIndexSection(1, '0', 3, DataSourcePathSection::IntegerType) );
    builder.addSection(f4);
    
    IStringSectionPtr f5 ( new DataSourcePathTextSection(".data") );
    builder.addSection(f5);
    
    std::vector<uint32> dims;
    dims.push_back(23);
    //dims.push_back(234);
    int8 ok = true;
    std::string path = builder.build(dims, ok);
   // std::cout << "Built Path: " << path << std::endl;
    BOOST_REQUIRE( ok == false);
  }
#endif
}



// -----------------------------------------------------------------------------
//  Use Boost unit test framework
// -----------------------------------------------------------------------------
test_suite* init_unit_test_suite( int32 /*argc*/, char* /*argv*/[] ) 
{
  
    test_suite* test= BOOST_TEST_SUITE( "Utility Tests" );
    test->add( BOOST_TEST_CASE( &DataSourcePathTest), 0);

    
    return test; 
}

