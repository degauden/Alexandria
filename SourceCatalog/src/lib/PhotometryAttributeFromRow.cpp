/**
 * @file src/lib/PhotometryAttributeFromRow.cpp
 *
 * @date Apr 17, 2014
 * @author dubath
 */

#include <typeindex>
#include "SourceCatalog/SourceAttributes/PhotometryAttributeFromRow.h"
#include "ElementsKernel/Exception.h"
#include "ElementsKernel/Real.h"

using namespace std;

namespace Euclid {
namespace SourceCatalog {

PhotometryAttributeFromRow::PhotometryAttributeFromRow(
    std::shared_ptr<Euclid::Table::ColumnInfo> column_info_ptr,
    const vector<pair<string, std::pair<string, string>>>& filter_name_mapping,
    const double missing_photometry_flag) :
    m_missing_photometry_flag(missing_photometry_flag) {

  unique_ptr<size_t> flux_column_index_ptr;
  unique_ptr<size_t> error_column_index_ptr;

  for (auto filter_name_pair : filter_name_mapping) {
    flux_column_index_ptr = column_info_ptr->find(filter_name_pair.second.first);
    error_column_index_ptr = column_info_ptr->find(filter_name_pair.second.second);

    if (flux_column_index_ptr == nullptr || type_index(typeid(double)) != column_info_ptr->getType(*(flux_column_index_ptr)) ) {
      throw Elements::Exception() << "Column info does not have the expected flux column of double type";
    }
    if (error_column_index_ptr == nullptr || type_index(typeid(double)) != column_info_ptr->getType(*(error_column_index_ptr)) ) {
      throw Elements::Exception() << "Column info does not have the expected flux column of double type";
    }
    m_table_index_vector.push_back(make_pair(*(flux_column_index_ptr), *(error_column_index_ptr)));
  }

  // create and filled the shared pointer to the filter name vector
  m_filter_name_vector_ptr = shared_ptr<vector<string>>{new vector<string>{} };
  for(auto a_filter_name_map: filter_name_mapping) {
    m_filter_name_vector_ptr->push_back(a_filter_name_map.first);
  }

}

PhotometryAttributeFromRow::~PhotometryAttributeFromRow() {
  // @todo Auto-generated destructor stub
}

unique_ptr<Attribute> PhotometryAttributeFromRow::createAttribute(
    const Euclid::Table::Row& row) {

  vector<FluxErrorPair> photometry_vector {};

  for (auto& filter_index_pair : m_table_index_vector) {
    Euclid::Table::Row::cell_type flux_cell = row[filter_index_pair.first];
    Euclid::Table::Row::cell_type error_cell = row[filter_index_pair.second];

    // Set the photometry flag if any
    if (boost::get<double>(flux_cell) == m_missing_photometry_flag) {
      photometry_vector.push_back(FluxErrorPair {boost::get<double>(flux_cell), boost::get<double>(error_cell), true,false } );
    }
    else {
      // Test for upper limit
      bool is_upper_limit=false;
      double error =  boost::get<double>(error_cell);
      if (error<0){
        error*=-1;
        is_upper_limit=true;
      }
      photometry_vector.push_back(FluxErrorPair {boost::get<double>(flux_cell), error, false, is_upper_limit } );
    }
  }//Eof for

  unique_ptr<Attribute> photometry_ptr { new Photometry{m_filter_name_vector_ptr, photometry_vector } };

  return move(photometry_ptr);
}

} // namespace SourceCatalog
} // end of namespace Euclid

