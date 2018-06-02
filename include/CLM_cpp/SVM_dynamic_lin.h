

#ifndef __SVMDYNAMICLIN_h_
#define __SVMDYNAMICLIN_h_

// Collection of linear SVR regressors for AU prediction
class SVM_dynamic_lin
{

public:

  SVM_dynamic_lin()
  {}

  // Predict the AU from HOG appearance of the face
  void Predict(std::vector<double>& predictions, std::vector<std::string>& names, const Mat_<double>& fhog_descriptor, const Mat_<double>& geom_params, const Mat_<double>& running_median, const Mat_<double>& running_median_geom);

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

  std::vector<double> pos_classes;
  std::vector<double> neg_classes;

};
//===========================================================================

#endif
