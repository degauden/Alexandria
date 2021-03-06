/*
 * Copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file src/lib/CatalogFromTable.cpp
 *
 * Created on: Apr 16, 2014
 *     Author: Pierre Dubath
 */
#include "SourceCatalog/CatalogFromTable.h"
#include "SourceCatalog/SourceAttributes/Photometry.h"
#include "Table/CastVisitor.h"
#include "Table/ColumnInfo.h"
#include <vector>

namespace Euclid {
namespace SourceCatalog {

CatalogFromTable::CatalogFromTable(std::shared_ptr<Euclid::Table::ColumnInfo>     column_info_ptr,
                                   const std::string&                             source_id_column_name,
                                   std::vector<std::shared_ptr<AttributeFromRow>> attribute_from_row_ptr_vector) {

  std::unique_ptr<size_t> source_id_index_ptr = column_info_ptr->find(source_id_column_name);
  if (source_id_index_ptr == nullptr) {
    throw Elements::Exception() << "Column info does not have the column " << source_id_column_name;
  }
  m_source_id_index = *(source_id_index_ptr);

  m_attribute_from_row_ptr_vector = std::move(attribute_from_row_ptr_vector);
}

CatalogFromTable::~CatalogFromTable() {
  // @todo Auto-generated destructor stub
}

Euclid::SourceCatalog::Catalog CatalogFromTable::createCatalog(const Euclid::Table::Table& input_table) {

  std::vector<Source> source_vector;

  // Figure out the type of the first row, and then assume all following
  // must be of the same
  CastSourceIdVisitor castVisitor;

  for (auto row : input_table) {

    auto source_id = boost::apply_visitor(castVisitor, row[m_source_id_index]);

    std::vector<std::shared_ptr<Attribute>> attribute_ptr_vector;

    for (auto& attribute_from_table_ptr : m_attribute_from_row_ptr_vector) {
      attribute_ptr_vector.push_back(attribute_from_table_ptr->createAttribute(row));
    }

    source_vector.push_back(Source{source_id, move(attribute_ptr_vector)});
  }

  return Catalog{source_vector};
}

}  // namespace SourceCatalog
}  // end of namespace Euclid
