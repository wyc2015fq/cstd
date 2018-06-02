


#ifndef __CCNF_PATCH_EXPERT_h_
#define __CCNF_PATCH_EXPERT_h_

//===========================================================================
/**
  A single Neuron response
*/
class CCNF_neuron
{

public:

  // Type of patch (0=raw,1=grad,3=depth, other types besides raw are not actually used now)
  int     neuron_type;

  // scaling of weights (needed as the energy of neuron might not be 1)
  double  norm_weights;

  // Weight bias
  double  bias;

  // Neural weights
  Mat weights;

  // can have neural weight dfts that are calculated on the go as needed, this allows us not to recompute
  // the dft of the template each time, improving the speed of tracking
  std::map<int, Mat > weights_dfts;

  // the alpha associated with the neuron
  double alpha;
  // The im_dft, integral_img, and integral_img_sq are precomputed images for convolution speedups (they get set if passed in empty values)
};

// Copy constructor
void CCNF_neuron_copy(CCNF_neuron* s, const CCNF_neuron& other) {
	s->weights = (other.weights.clone());
	s->neuron_type = other.neuron_type;
	s->norm_weights = other.norm_weights;
	s->bias = other.bias;
	s->alpha = other.alpha;

	for (std::map<int, Mat >::const_iterator it = other.weights_dfts.begin(); it != other.weights_dfts.end(); it++) {
		// Make sure the matrix is copied.
		s->weights_dfts.insert(std::pair<int, Mat>(it->first, it->second.clone()));
	}
}

//void CCNF_neuron_Response(CCNF_neuron* s, Mat& im, Mat& im_dft, Mat& integral_img, Mat& integral_img_sq, Mat& resp);
//===========================================================================
/**
A CCNF patch expert
*/
class CCNF_patch_expert
{
public:

  // Width and height of the patch expert support region
  int width;
  int height;

  // Collection of neurons for this patch expert
  std::vector<CCNF_neuron> neurons;

  // Information about the vertex features (association potentials)
  std::vector<int>        window_sizes;
  std::vector<Mat > Sigmas;
  std::vector<double>       betas;

  // How confident we are in the patch
  double   patch_confidence;

  // Default constructor
  CCNF_patch_expert() {
    ;
  }

  bool HasWindow(int window_size) {
	  for (size_t i = 0; i < window_sizes.size(); ++i)
	  {
		  if (window_sizes[i] == window_size)
			  return true;
	  }
	  return false;
  }
  // Copy constructor
  CCNF_patch_expert(const CCNF_patch_expert& other) {
	  this->window_sizes = (other.window_sizes);
	  this->betas = (other.betas);
	  this->neurons.resize(other.neurons.size());
	  for (int i=0; i<other.neurons.size(); ++i) {
	    CCNF_neuron_copy(&this->neurons[i], other.neurons[i]);
	  }
    this->width = other.width;
    this->height = other.height;
    this->patch_confidence = other.patch_confidence;

    // Copy the Sigmas in a deep way
    for (std::vector<Mat >::const_iterator it = other.Sigmas.begin(); it != other.Sigmas.end(); it++) {
      // Make sure the matrix is copied.
      this->Sigmas.push_back(it->clone());
    }

  }

  void Read(FILE* stream, const std::vector<int>& window_sizes, const std::vector<std::vector<Mat > >& sigma_components);

  // actual work (can pass in an image and a potential depth image, if the CCNF is trained with depth)
  void Response(const Mat& area_of_interest, Mat& response);

  // Helper function to compute relevant sigmas
  void ComputeSigmas(const std::vector<Mat >& sigma_components, int window_size);

};
//===========================================================================

#endif
