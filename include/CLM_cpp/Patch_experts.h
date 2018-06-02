
#ifndef __Patch_experts_h_
#define __Patch_experts_h_


//===========================================================================
/**
    Combined class for all of the patch experts
*/
class Patch_experts
{
public:

  // The collection of SVR patch experts (for intensity/grayscale images), the experts are laid out scale->view->landmark
  vector<vector<vector<Multi_SVR_patch_expert> > >  svr_expert_intensity;

  // The collection of SVR patch experts (for depth/range images), the experts are laid out scale->view->landmark
  vector<vector<vector<Multi_SVR_patch_expert> > >  svr_expert_depth;

  // The collection of LNF (CCNF) patch experts (for intensity images), the experts are laid out scale->view->landmark
  vector<vector<vector<CCNF_patch_expert> > >     ccnf_expert_intensity;

  // The node connectivity for CCNF experts, at different window sizes and corresponding to separate edge features
  vector<vector<Mat > >         sigma_components;

  // The available scales for intensity patch experts
  vector<double>              patch_scaling;

  // The available views for the patch experts at every scale (in radians)
  vector<vector<Vec3d> >               centers;

  // Landmark visibilities for each scale and view
  vector<vector<Mat > >          visibilities;

  // A default constructor
  Patch_experts() {
    ;
  }

  // A copy constructor
  Patch_experts(const Patch_experts& other): patch_scaling(other.patch_scaling), centers(other.centers), svr_expert_intensity(other.svr_expert_intensity), svr_expert_depth(other.svr_expert_depth), ccnf_expert_intensity(other.ccnf_expert_intensity) {

    int i, j;
    // Make sure the matrices are allocated properly
    this->sigma_components.resize(other.sigma_components.size());

    for (i = 0; i < other.sigma_components.size(); ++i) {
      this->sigma_components[i].resize(other.sigma_components[i].size());

      for (j = 0; j < other.sigma_components[i].size(); ++j) {
        // Make sure the matrix is copied.
        this->sigma_components[i][j] = other.sigma_components[i][j].clone();
      }
    }

    // Make sure the matrices are allocated properly
    this->visibilities.resize(other.visibilities.size());

    for (i = 0; i < other.visibilities.size(); ++i) {
      this->visibilities[i].resize(other.visibilities[i].size());

      for (j = 0; j < other.visibilities[i].size(); ++j) {
        // Make sure the matrix is copied.
        this->visibilities[i][j] = other.visibilities[i][j].clone();
      }
    }
  }

  // Returns the patch expert responses given a grayscale and an optional depth image.
  // Additionally returns the transform from the image coordinates to the response coordinates (and vice versa).
  // The computation also requires the current landmark locations to compute response around, the PDM corresponding to the desired model, and the parameters describing its instance
  // Also need to provide the size of the area of interest and the desired scale of analysis
  void Response(vector<Mat >& patch_expert_responses, Matx22d& sim_ref_to_img, Matx22d& sim_img_to_ref, const img_t* grayscale_image, const img_t* depth_image,
      const PDM& pdm, const Vec6d& params_global, const Mat& params_local, int window_size, int scale);

  // Getting the best view associated with the current orientation
  int GetViewIdx(const Vec6d& params_global, int scale) const;

  // The number of views at a particular scale
  inline int nViews(int scale = 0) const {
    return centers[scale].size();
  };

  // Reading in all of the patch experts
  void Read(char** intensity_svr_expert_locations, char** depth_svr_expert_locations, char** intensity_ccnf_expert_locations);




private:
  void Read_SVR_patch_experts(char* expert_location, std::vector<Vec3d>& centers, std::vector<Mat >& visibility, std::vector<std::vector<Multi_SVR_patch_expert> >& patches, double& scale);
  void Read_CCNF_patch_experts(char* patchesFileLocation, std::vector<Vec3d>& centers, std::vector<Mat >& visibility, std::vector<std::vector<CCNF_patch_expert> >& patches, double& patchScaling);


};


#endif
