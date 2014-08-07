/** 
 * @file GridContainer/GridCellManagerTraits.h
 * @date May 13, 2014
 * @author Nikolaos Apostolakos
 */

#ifndef GRIDCONTAINER_DATAMANAGERTRAITS_H
#define	GRIDCONTAINER_DATAMANAGERTRAITS_H

#include <vector>
#include <memory>

namespace GridContainer {

/**
 * @class GridCellManagerTraits
 * 
 * @brief Class used by the GridContainer to access the different DataManagers
 * 
 * @details
 * To reduce the requirements of the different GridCellManager which are used by
 * the GridContainer to store the data, the GridContainer class does not directly access the
 * GridCellManager instances, but it uses this trait to redirect all the operations.
 * The default implementation of the trait simply redirects the operations,
 * but, if the API of a manager does not fit the trait, this default behavior
 * can be overridden by declaring a specialization of the trait.
 * 
 * @tparam GridCellManager the manager which keeps the GridContainer data
 */
template<typename GridCellManager>
struct GridCellManagerTraits {
  
  /// The type of the data kept by the GridCellManager
  typedef typename GridCellManager::data_type data_type;
  
  /// The iterator type which is used to iterate through the data kept in the
  /// data manager
  typedef typename GridCellManager::iterator iterator;
  
  /**
   * Factory which creates a GridCellManager instance with the given number of
   * managed data, which all are set to a default value. The default
   * implementation will try to use a constructor with the size as parameter.
   * 
   * @param size The number of data the manager will contain
   * @return A unique_ptr to the newly constructed GridCellManager
   */
  static std::unique_ptr<GridCellManager> factory(size_t size);
  
  /**
   * Returns the number of data managed by the given GridCellManager. Defaults on
   * calling the constant version of method size() on the GridCellManager instance.
   * 
   * @param data_manager The GridCellManager to get the size of
   * @return The number of data managed by the GridCellManager
   */
  static size_t size(const GridCellManager& data_manager);
  
  /**
   * Returns an iterator pointing to the first element managed by the
   * GridCellManager. Defaults on calling the begin() method of the GridCellManager
   * instance.
   * 
   * @param data_manager the data manager
   * @return An iterator at the first element
   */
  static iterator begin(GridCellManager& data_manager);
  
  /**
   * Returns an iterator pointing right after the last element managed by the
   * GridCellManager. Defaults on calling the end() method of the GridCellManager
   * instance.
   * 
   * @param data_manager the GridCellManager
   * @return An iterator right after the last element
   */
  static iterator end(GridCellManager& data_manager);
  
  /// Flag which indicates if the GridCellManager is boost serializable. By default
  /// it is set to false. Note that Matrices which use DataManagers which have
  /// this flag set to false cannot be serialized.
  static const bool enable_boost_serialize = false;
  
}; // end of GridCellManagerTraits


/**
 * Specialization of the GridCellManagerTraits for vector DataManagers. It uses
 * all the default operations but it changes the serialization flag to true
 * to declare that vector GridCellManager%s can be serialized. Note that the type
 * T of the data managed has to also be serializable.
 * 
 * @tparam T the type of the data kept by the vector
 */
template<typename T>
struct GridCellManagerTraits<std::vector<T>> {
  
  /// The type of the data kept by the GridCellManager
  typedef T data_type;
  
  /// The iterator type which is used to iterate through the data kept in the
  /// data manager
  typedef typename std::vector<T>::iterator iterator;
  
  /// Returns a vector containing "size" default constructed elements
  static std::unique_ptr<std::vector<T>> factory(size_t size);
  
  /// Returns the size of the vector
  static size_t size(const std::vector<T>& vector);
  
  /// Returns an iterator at the first element of the vector
  static iterator begin(std::vector<T>& vector);
  
  /// Returns an iterator right after the last element of the vector
  static iterator end(std::vector<T>& vector);
  
  /// Enables boost serialization of Matrices using vector%s as GridCellManager%s
  static const bool enable_boost_serialize = true;
  
}; // end of GridCellManagerTraits vector specialization

} // end of namespace GridContainer

#include "GridContainer/_impl/GridCellManagerTraits.icpp"

#endif	/* GRIDCONTAINER_DATAMANAGERTRAITS_H */
