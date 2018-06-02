
#include "highgui.h"

#include "features2d/features2d.h"
#include "features2d/features2d.inl"

// sift特征点的检测，也可以使用下面代码

void test_sift()
{
  img_t im[1] = {0};
  img_t gry[1] = {0}
      img_t descriptors;
  vector<KeyPoint> keypoints;

  imread("test.bmp", 1, 1, im);
  imread("test.bmp", 1, 1, gry);

  SiftFeatureDetector sift2(0.06f, 10.0);
  sift2.detect(iamgeGray, keypoints);

  drawKeypoints(image, keypoints, image, Scalar(255, 0, 255));
  imshow("test", image);

  waitKey();
}

#if 0
void _tmain()
{
  img_t input1 = imread("image1.jpg", 1);
  img_t input2 = imread("image2.jpg", 1);
  SiftFeatureDetector detector;
  vector<KeyPoint> keypoint1, keypoint2;
  detector.detect(input1, keypoint1);

  img_t output1;
  drawKeypoints(input1, keypoint1, output1);
  imshow("sift_result1.jpg", output1);
  imwrite("sift_result1.jpg", output1);

  img_t output2;
  SiftDescriptorExtractor extractor;
  img_t descriptor1, descriptor2;
  BruteForceMatcher<L2<float>> matcher;

  vector<DMatch> matches;
  img_t img_matches;
  detector.detect(input2, keypoint2);
  drawKeypoints(input2, keypoint2, output2);

  imshow("sift_result2.jpg", output2);
  imwrite("sift_result2.jpg", output2);

  extractor.compute(input1, keypoint1, descriptor1);
  extractor.compute(input2, keypoint2, descriptor2);

  matcher.match(descriptor1, descriptor2, matches);

  drawMatches(input1, keypoint1, input2, keypoint2, matches, img_matches);
  imshow("matches", img_matches);
  imwrite("matches.jpg", img_matches);

  waitKey();
}

#endif
