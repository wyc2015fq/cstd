/*
 * RobustMatcher.h
 *
 *  Created on: Jun 4, 2014
 *      Author: eriba
 */

#ifndef ROBUSTMATCHER_H_
#define ROBUSTMATCHER_H_

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

class RobustMatcher {
public:
  RobustMatcher() : ratio_(0.8f)
  {
    // ORB is the default feature
    detector_ = ORB::create();
    extractor_ = ORB::create();

    // BruteFroce matcher with Norm Hamming is the default matcher
    matcher_ = makePtr<BFMatcher>((int)NORM_HAMMING, false);

  }
  virtual ~RobustMatcher();

  // Set the feature detector
  void setFeatureDetector(const Ptr<FeatureDetector>& detect) {  detector_ = detect; }

  // Set the descriptor extractor
  void setDescriptorExtractor(const Ptr<DescriptorExtractor>& desc) { extractor_ = desc; }

  // Set the matcher
  void setDescriptorMatcher(const Ptr<DescriptorMatcher>& match) {  matcher_ = match; }

  // Compute the keypoints of an image
  void computeKeyPoints( const img_t& image, std::vector<KeyPoint>& keypoints);

  // Compute the descriptors of an image given its keypoints
  void computeDescriptors( const img_t& image, std::vector<KeyPoint>& keypoints, img_t& descriptors);

  // Set ratio parameter for the ratio test
  void setRatio( float rat) { ratio_ = rat; }

  // Clear matches for which NN ratio is > than threshold
  // return the number of removed points
  // (corresponding entries being cleared,
  // i.e. size will be 0)
  int ratioTest(std::vector<std::vector<DMatch> > &matches);

  // Insert symmetrical matches in symMatches vector
  void symmetryTest( const std::vector<std::vector<DMatch> >& matches1,
                     const std::vector<std::vector<DMatch> >& matches2,
                     std::vector<DMatch>& symMatches );

  // Match feature points using ratio and symmetry test
  void robustMatch( const img_t& frame, std::vector<DMatch>& good_matches,
                      std::vector<KeyPoint>& keypoints_frame,
                      const img_t& descriptors_model );

 // Match feature points using ratio test
 void fastRobustMatch( const img_t& frame, std::vector<DMatch>& good_matches,
                       std::vector<KeyPoint>& keypoints_frame,
                       const img_t& descriptors_model );

private:
  // pointer to the feature point detector object
  Ptr<FeatureDetector> detector_;
  // pointer to the feature descriptor extractor object
  Ptr<DescriptorExtractor> extractor_;
  // pointer to the matcher object
  Ptr<DescriptorMatcher> matcher_;
  // max ratio between 1st and 2nd NN
  float ratio_;
};

#endif /* ROBUSTMATCHER_H_ */
