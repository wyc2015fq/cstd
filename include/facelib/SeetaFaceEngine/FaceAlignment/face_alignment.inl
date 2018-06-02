
#include "face_alignment.h"

#include <string>
#include <math.h>
#include "cfan.h"

namespace seeta {
  /** A constructor with an optional argument specifying path of the model file.
   *  If called with no argument, the model file is assumed to be stored in the
   *  the working directory as "seeta_fa_v1.1.bin".
   *
   *  @param model_path Path of the model file, either absolute or relative to
   *  the working directory.
   */
  FaceAlignment::FaceAlignment(const char * model_path){
    facial_detector = new CCFAN();
    if (model_path == NULL)
      model_path = "seeta_fa_v1.1.bin";
    facial_detector->InitModel(model_path);
  }

  /** Detect five facial landmarks, i.e., two eye centers, nose tip and two mouth corners.
   *  @param gray_im A grayscale image
   *  @param face_info The face bounding box
   *  @param[out] points The locations of detected facial points
   */
  bool FaceAlignment::PointDetectLandmarks(ImageData gray_im, FaceInfo face_info, FacialLandmark *points)
  {
    if (gray_im.num_channels != 1) {
      return false;
    }
    int pts_num = 5;
    float *facial_loc = new float[pts_num * 2];
    facial_detector->FacialPointLocate(gray_im.data, gray_im.width, gray_im.height, face_info, facial_loc);

    for (int i = 0; i < pts_num; i++) {
      points[i].x = facial_loc[i * 2];
      points[i].y = facial_loc[i * 2 + 1];
    }

    delete[]facial_loc;
    return true;
  }

  /** A Destructor which should never be called explicitly.
   *  Release all dynamically allocated resources.
   */
  FaceAlignment::~FaceAlignment() {
    if (facial_detector != NULL) {
      delete facial_detector;
      facial_detector = NULL;
    }
  }
}  // namespace seeta
