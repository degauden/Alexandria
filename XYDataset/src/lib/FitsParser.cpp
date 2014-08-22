/**
 * @file src/lib/FitsParser.cpp
 *
 * @date May 13, 2014
 * @author Nicolas Morisset
 */

#include <boost/regex.hpp>
#include <fstream>
#include <iostream>
#include <CCfits/CCfits>

#include "ElementsKernel/ElementsException.h"
#include "ChTable/FitsReader.h"
#include "XYDataset/FitsParser.h"
#include "StringFunctions.h"

using boost::regex;
using boost::regex_match;

namespace Euclid {
namespace XYDataset {

//
// Get dataset name from a FITS file
//
std::string FitsParser::getName(const std::string& file) {

  std::string dataset_name{};
  std::ifstream sfile(file);

  // Check file exists
  if (!sfile) {
    throw ElementsException() << "File not found : " << file;
  }

  // Read first HDU
  std::unique_ptr<CCfits::FITS> fits { new CCfits::FITS(file, CCfits::RWmode::Read)};
  CCfits::ExtHDU& table_hdu = fits->extension(1);

  table_hdu.readAllKeys();
  auto keyword_map = table_hdu.keyWord();
  auto iter=keyword_map.find(m_name_keyword);
  if (iter != keyword_map.end()) {
    iter->second->value(dataset_name);
  }
  else {
    // Dataset name is the filename without extension and path
    std::string str {};
    str          = removeAllBeforeLastSlash(file);
    dataset_name = removeExtension(str);
  }
  return (dataset_name);
}

//
// Get dataset from a FITS file
//
std::unique_ptr<XYDataset> FitsParser::getDataset(const std::string& file) {

  std::unique_ptr<XYDataset> dataset_ptr {};
  std::ifstream sfile(file);

  CCfits::FITS::setVerboseMode(true);

  // Check file exists
  if (sfile) {
    std::unique_ptr<CCfits::FITS> fits { new CCfits::FITS(file, CCfits::RWmode::Read)};
    try {
      const CCfits::ExtHDU& table_hdu = fits->extension(1);
      // Read first HDU
      Euclid::ChTable::FitsReader fits_reader {};
      auto table = fits_reader.read(table_hdu);

      // Put the Table data into vector pair
      std::vector<std::pair<double, double>> vector_pair;
      for (auto row : table) {
          vector_pair.push_back(std::make_pair( boost::get<double>(row[0]), boost::get<double>(row[1]) ));
      }
      dataset_ptr = std::unique_ptr<XYDataset> { new XYDataset(vector_pair) };
    }
    catch(CCfits::FitsException& fits_except){
      throw ElementsException() << "FitsException catched! File: " << file;
    } // Eof try-catch
  } // Eof if

 return(dataset_ptr);
}

} // XYDataset namespace
} // end of namespace Euclid



