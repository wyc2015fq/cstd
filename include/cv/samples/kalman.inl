
#include "cstd.h"

static inline IPOINT calcPoint(FPOINT center, double R, double angle)
{
  return iPOINT(center.x + (float)cos(angle), center.y + (float)-sin(angle))*(float)R;
}

int kalman()
{
  printf( "\nExample of c calls to OpenCV's Kalman filter.\n"
    "   Tracking of rotating point.\n"
    "   Rotation speed is constant.\n"
    "   Both state and measurements vectors are 1D (a point angle),\n"
    "   Measurement is the real point angle + gaussian noise.\n"
    "   The real and the estimated points are connected with yellow line segment,\n"
    "   the real and the measured points are connected with red line segment.\n"
    "   (if Kalman filter works correctly,\n"
    "    the yellow segment should be shorter than the red one).\n"
    "\n"
    "   Pressing any key (except ESC) will reset the tracking with a different speed.\n"
    "   Pressing ESC will stop the program.\n"
    );
  img_t im[10] = {0};
  int i=0;
  img_t* img = im+i++;
  imsetsize(img, 500, 500, 3, 1);
  KalmanFilter KF(2, 1, 0);
  img_t state(2, 1, CC_32F); /* (phi, delta_phi) */
  img_t processNoise(2, 1, CC_32F);
  img_t measurement = img_t::zeros(1, 1, CC_32F);
  char code = (char)-1;
  
  for(;;)
  {
    randn( state, Scalar::all(0), Scalar::all(0.1) );
    KF.transitionMatrix = (Mat_<float>(2, 2) << 1, 1, 0, 1);
    
    setIdentity(KF.measurementMatrix);
    setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
    setIdentity(KF.errorCovPost, Scalar::all(1));
    
    randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));
    
    for(;;)
    {
      CPoint2f center(img->cols*0.5f, img->rows*0.5f);
      float R = img->cols/3.f;
      double stateAngle = state.at<float>(0);
      Point statePt = calcPoint(center, R, stateAngle);
      
      img_t prediction = KF.predict();
      double predictAngle = prediction.at<float>(0);
      Point predictPt = calcPoint(center, R, predictAngle);
      
      randn( measurement, Scalar::all(0), Scalar::all(KF.measurementNoiseCov.at<float>(0)));
      
      // generate measurement
      measurement += KF.measurementMatrix*state;
      
      double measAngle = measurement.at<float>(0);
      Point measPt = calcPoint(center, R, measAngle);
      
      // plot points
#define drawCross( center, color, d )                                        \
  line( img, Point( center.x - d, center.y - d ),                          \
  Point( center.x + d, center.y + d ), color, 1, LINE_AA, 0); \
  line( img, Point( center.x + d, center.y - d ),                          \
      Point( center.x - d, center.y + d ), color, 1, LINE_AA, 0 )
      
      img = Scalar::all(0);
      drawCross( statePt, Scalar(255,255,255), 3 );
      drawCross( measPt, Scalar(0,0,255), 3 );
      drawCross( predictPt, Scalar(0,255,0), 3 );
      line( img, statePt, measPt, Scalar(0,0,255), 3, LINE_AA, 0 );
      line( img, statePt, predictPt, Scalar(0,255,255), 3, LINE_AA, 0 );
      
      if(theRNG().uniform(0,4) != 0)
        KF.correct(measurement);
      
      randn( processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
      state = KF.transitionMatrix*state + processNoise;
      
      imshow( "Kalman", img );
      code = (char)waitKey(100);
      
      if( code > 0 )
        break;
    }
    if( code == 27 || code == 'q' || code == 'Q' )
      break;
  }
  
  return 0;
}
