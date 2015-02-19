/**
 * @file src/lib/AsciiWriterHelper.h
 * @date April 16, 2014
 * @author Nikolaos Apostolakos
 */

#ifndef TABLE_ASCIIWRITERHELPER_H
#define TABLE_ASCIIWRITERHELPER_H

#include <vector>
#include <typeindex>

#include "ElementsKernel/Export.h"


#include "Table/Table.h"

namespace Euclid {
namespace Table {

/**
 * @brief
 * Converts a type to its string representation
 *
 * @param type The type to convert
 * @return The string representation
 * @throws Elements::Exception
 *    if the given type is not supported
 */
ELEMENTS_API std::string typeToKeyword(std::type_index type);

/**
 * @brief
 * Calculates the sizes in characters each column of the table needs
 * @details
 * The size is calculated as the size of the longest column entry (including type
 * and name) plus one to ensure separation of the values.
 *
 * @param table The table
 * @return  the sizes of the columns
 */
ELEMENTS_API std::vector<size_t> calculateColumnLengths(const Table& table);

}
} // end of namespace Euclid

#endif /* TABLE_ASCIIWRITERHELPER_H */

