
#include "cstd.h"
#include "img/imgopt.inl"

typedef struct eigen_faces_t {
  int _num_components;
  double _threshold;
  img_t* _projections;
  img_t _labels[1];
  img_t _eigenvectors[1];
  img_t _eigenvalues[1];
  img_t _mean[1];
} eigen_faces_t;


#include "cv1/matrix.inl"

// Turk, M., and Pentland, A. "Eigenfaces for recognition.". Journal of
// Cognitive Neuroscience 3 (1991), 71¨C86.

int Eigenfaces_init(eigen_faces_t* s, int num_components DEFAULT(0), double threshold DEFAULT(DBL_MAX) ) {
  s->_num_components = num_components;
  s->_threshold = threshold;
  return 0;
}
//------------------------------------------------------------------------------
// Eigenfaces
//------------------------------------------------------------------------------
// data 64F n*d
int Eigenfaces_train(eigen_faces_t* s, const img_t* data, const int* labels) {
  // observations in row
  //img_t data = asRowMatrix(_src, CC_64F);
  // number of samples
  int n = data->h;
  // assert there are as much samples as labels
  // clip number of components to be valid
  if((s->_num_components <= 0) || (s->_num_components > n))
    s->_num_components = n;
  
#if 0
  // perform the PCA
  PCA pca(data, img_t(), PCA::DATA_AS_ROW, _num_components);
  // copy the PCA results
  _mean = pca.mean.reshape(1,1); // store the mean vector
  _eigenvalues = pca.eigenvalues.clone(); // eigenvalues by row
  transpose(pca.eigenvectors, _eigenvectors); // eigenvectors by column
  // store labels for prediction
  _labels = labels.clone();
  // save projections
  for(int sampleIdx = 0; sampleIdx < data.rows; sampleIdx++) {
    img_t p = LDA::subspaceProject(_eigenvectors, _mean, data.row(sampleIdx));
    _projections.push_back(p);
  }
#endif
  return 0;
}

#if 0
void Eigenfaces::predict(InputArray _src, Ptr<PredictCollector> collector) const {
    // get data
    img_t src = _src.getMat();
    // make sure the user is passing correct data
    if(_projections.empty()) {
        // throw error if no data (or simply return -1?)
        String error_message = "This Eigenfaces model is not computed yet. Did you call Eigenfaces::train?";
        CC_Error(Error::StsError, error_message);
    } else if(_eigenvectors.rows != static_cast<int>(src.total())) {
        // check data alignment just for clearer exception messages
        String error_message = format("Wrong input image size. Reason: Training and Test images must be of equal size! Expected an image with %d elements, but got %d.", _eigenvectors.rows, src.total());
        CC_Error(Error::StsBadArg, error_message);
    }
    // project into PCA subspace
    img_t q = LDA::subspaceProject(_eigenvectors, _mean, src.reshape(1, 1));
    collector->init(_projections.size());
    for (size_t sampleIdx = 0; sampleIdx < _projections.size(); sampleIdx++) {
        double dist = norm(_projections[sampleIdx], q, NORM_L2);
        int label = _labels.at<int>((int)sampleIdx);
        if (!collector->collect(label, dist))return;
    }
}


#endif
