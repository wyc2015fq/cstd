
#ifndef __SVRDYNAMICLINREGRESSORS_h_
#define __SVRDYNAMICLINREGRESSORS_h_

// Collection of linear SVR regressors for AU prediction that uses per person face nomalisation with the help of a running median
class SVR_dynamic_lin_regressors
{

public:

  SVR_dynamic_lin_regressors()
  {}

  // Predict the AU from HOG appearance of the face
  void Predict(std::vector<double>& predictions, std::vector<std::string>& names, const Mat_<double>& descriptor, const Mat_<double>& geom_params, const Mat_<double>& running_median, const Mat_<double>& running_median_geom);

  // Reading in the model (or adding to it)
  void Read(std::ifstream& stream, const std::vector<std::string>& au_names);

  std::vector<std::string> GetAUNames() {
    return AU_names;
  }

private:

  // The names of Action Units this model is responsible for
  std::vector<std::string> AU_names;

  // For normalisation
  Mat_<double> means;

  // For actual prediction
  Mat_<double> support_vectors;
  Mat_<double> biases;

};
//===========================================================================

#endif
