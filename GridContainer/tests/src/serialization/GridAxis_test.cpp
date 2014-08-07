/** 
 * @file tests/src/serialization/GridAxis_test.cpp
 * @date June 27, 2014
 * @author Nikolaos Apostolakos
 */

#include <sstream>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "GridContainer/GridAxis.h"
#include "GridContainer/serialization/GridAxis.h"
#include "DefaultConstructibleClass.h"
#include "NonDefaultConstructibleClass.h"

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (AxisInfo_serialization_test)

//-----------------------------------------------------------------------------
// Test serialization of GridAxis with fundamental knot values
//-----------------------------------------------------------------------------
    
BOOST_AUTO_TEST_CASE(fundamentalKnotValues) {
  
  // Given
  std::string name = "AxisName";
  std::vector<double> knots {0., 3.4, 12E-15};
  GridContainer::GridAxis<double> axis_info {name, knots};
  
  // When
  std::stringstream stream {};
  boost::archive::binary_oarchive boa {stream};
  // We write to the stream a pointer to enable the non-default constructor
  // functionality of boost serialization
  GridContainer::GridAxis<double>* axis_info_ptr = &axis_info;
  boa << axis_info_ptr;
  GridContainer::GridAxis<double>* result;
  boost::archive::binary_iarchive bia {stream};
  bia >> result;
  // We use a unique_ptr for the memory management
  std::unique_ptr<GridContainer::GridAxis<double>> result_ptr {result};
  
  // Then
  BOOST_CHECK_EQUAL(result_ptr->name(), name);
  BOOST_CHECK_EQUAL_COLLECTIONS(result_ptr->begin(), result_ptr->end(), knots.begin(), knots.end());
  
}

//-----------------------------------------------------------------------------
// Test serialization of GridAxis with default constructible knot values
//-----------------------------------------------------------------------------
    
BOOST_AUTO_TEST_CASE(defaultConstructibleKnotValues) {
  
  // Given
  std::string name = "AxisName";
  std::vector<DefaultConstructibleClass> knots {};
  knots.push_back(DefaultConstructibleClass{});
  knots.back().value = 0.;
  knots.push_back(DefaultConstructibleClass{});
  knots.back().value = 3.4;
  knots.push_back(DefaultConstructibleClass{});
  knots.back().value = 12E-15;
  GridContainer::GridAxis<DefaultConstructibleClass> axis_info {name, knots};
  
  // When
  std::stringstream stream {};
  boost::archive::binary_oarchive boa {stream};
  // We write to the stream a pointer to enable the non-default constructor
  // functionality of boost serialization
  GridContainer::GridAxis<DefaultConstructibleClass>* axis_info_ptr = &axis_info;
  boa << axis_info_ptr;
  GridContainer::GridAxis<DefaultConstructibleClass>* result;
  boost::archive::binary_iarchive bia {stream};
  bia >> result;
  // We use a unique_ptr for the memory management
  std::unique_ptr<GridContainer::GridAxis<DefaultConstructibleClass>> result_ptr {result};
  
  // Then
  BOOST_CHECK_EQUAL(result_ptr->name(), name);
  BOOST_CHECK_EQUAL(result_ptr->size(), knots.size());
  auto result_iter = result_ptr->begin();
  auto knots_iter = knots.begin();
  while (result_iter != result_ptr->end()) {
    BOOST_CHECK_EQUAL(result_iter->value, knots_iter->value);
    ++result_iter;
    ++ knots_iter;
  }
  
}

//-----------------------------------------------------------------------------
// Test serialization of GridAxis with non default constructible knot values
//-----------------------------------------------------------------------------
    
BOOST_AUTO_TEST_CASE(nonDefaultConstructibleKnotValues) {
  
  // Given
  std::string name = "AxisName";
  std::vector<NonDefaultConstructibleClass> knots {};
  knots.push_back(NonDefaultConstructibleClass{0.});
  knots.push_back(NonDefaultConstructibleClass{3.4});
  knots.push_back(NonDefaultConstructibleClass{12E-15});
  GridContainer::GridAxis<NonDefaultConstructibleClass> axis_info {name, knots};
  
  // When
  std::stringstream stream {};
  boost::archive::binary_oarchive boa {stream};
  // We write to the stream a pointer to enable the non-default constructor
  // functionality of boost serialization
  GridContainer::GridAxis<NonDefaultConstructibleClass>* axis_info_ptr = &axis_info;
  boa << axis_info_ptr;
  GridContainer::GridAxis<NonDefaultConstructibleClass>* result;
  boost::archive::binary_iarchive bia {stream};
  bia >> result;
  // We use a unique_ptr for the memory management
  std::unique_ptr<GridContainer::GridAxis<NonDefaultConstructibleClass>> result_ptr {result};
  
  // Then
  BOOST_CHECK_EQUAL(result_ptr->name(), name);
  BOOST_CHECK_EQUAL(result_ptr->size(), knots.size());
  auto result_iter = result_ptr->begin();
  auto knots_iter = knots.begin();
  while (result_iter != result_ptr->end()) {
    BOOST_CHECK_EQUAL(result_iter->value, knots_iter->value);
    ++result_iter;
    ++ knots_iter;
  }
  
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()
