/** 
 * @file src/lib/interpolation/linear.cpp
 * @date February 20, 2014
 * @author Nikolaos Apostolakos
 */

#include "ElementsKernel/ElementsException.h"
#include "ChMath/interpolation/interpolation.h"
#include "ChMath/function/Polynomial.h"
#include "ChMath/function/Piecewise.h"

namespace ChMath {

std::unique_ptr<Function> linearInterpolation(const std::vector<double>& x, const std::vector<double>& y) {
  if (x.size() != y.size()) {
    throw ElementsException("Input X and Y vectors must have the same size");
  }
  for (auto iter=(x.begin()+1); iter!=x.end(); ++iter) {
    if (*iter <= *(iter-1)) {
      throw ElementsException("Only strictly increasing X values allowed");
    }
  }
  std::vector<std::shared_ptr<Function>> functions {};
  for (size_t i=0; i<x.size()-1; i++) {
    double coef1 = (y[i+1]-y[i]) / (x[i+1]-x[i]);
    double coef0 = y[i] - coef1*x[i];
    functions.push_back(std::shared_ptr<Function>(new Polynomial{{coef0,coef1}}));
  }
  return std::unique_ptr<Function>(new Piecewise{x, std::move(functions)});
}

} // End of ChMath