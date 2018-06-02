

#include "AAM_Basic.inl"
#include "AAM_IC.inl"
#include "AAM_PAW.inl"
#include "AAM_PDM.inl"
#include "AAM_Shape.inl"
#include "AAM_TDM.inl"
#include "AAM_Util.inl"
#include "AAM_CAM.inl"
#include "AAM_VJFaceDetect.inl"
#include "train.inl"
#include "fit.inl"
#include "aamtest.inl"

int test_aam(int argc, char** argv) {
  train(argc, argv);
  return 0;
}
