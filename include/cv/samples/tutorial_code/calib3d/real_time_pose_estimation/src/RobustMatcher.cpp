/*
 * RobustMatcher.cpp
 *
 *  Created on: Jun 4, 2014
 *      Author: eriba
 */

#include "RobustMatcher.h"
#include <time.h>

#include <opencv2/features2d/features2d.hpp>

RobustMatcher::~RobustMatcher()
{
  // TODO Auto-generated destructor stub
}

void RobustMatcher::computeKeyPoints( const CvMat& image, std::vector<KeyPoint>& keypoints)
{
  detector_->detect(image, keypoints);
}

void RobustMatcher::computeDescriptors( const CvMat& image, std::vector<KeyPoint>& keypoints, CvMat& descriptors)
{
  extractor_->compute(image, keypoints, descriptors);
}

int RobustMatcher::ratioTest(std::vector<std::vector<DMatch> > &matches)
{
  int removed = 0;
  // for all matches
  for ( std::vector<std::vector<DMatch> >::iterator
        matchIterator= matches.begin(); matchIterator!= matches.end(); ++matchIterator)
  {
    // if 2 NN has been identified
    if (matchIterator->size() > 1)
    {
      // check distance ratio
      if ((*matchIterator)[0].distance / (*matchIterator)[1].distance > ratio_)
      {
        matchIterator->clear(); // remove match
        removed++;
      }
    }
    else
    { // does not have 2 neighbours
      matchIterator->clear(); // remove match
      removed++;
    }
  }
  return removed;
}

void RobustMatcher::symmetryTest( const std::vector<std::vector<DMatch> >& matches1,
                     const std::vector<std::vector<DMatch> >& matches2,
                     std::vector<DMatch>& symMatches )
{

  // for all matches image 1 -> image 2
   for (std::vector<std::vector<DMatch> >::const_iterator
       matchIterator1 = matches1.begin(); matchIterator1 != matches1.end(); ++matchIterator1)
   {

      // ignore deleted matches
      if (matchIterator1->empty() || matchIterator1->size() < 2)
         continue;

      // for all matches image 2 -> image 1
      for (std::vector<std::vector<DMatch> >::const_iterator
          matchIterator2 = matches2.begin(); matchIterator2 != matches2.end(); ++matchIterator2)
      {
        // ignore deleted matches
        if (matchIterator2->empty() || matchIterator2->size() < 2)
           continue;

        // Match symmetry test
        if ((*matchIterator1)[0].queryIdx ==
            (*matchIterator2)[0].trainIdx &&
            (*matchIterator2)[0].queryIdx ==
            (*matchIterator1)[0].trainIdx)
        {
            // add symmetrical match
            symMatches.push_back(
              DMatch((*matchIterator1)[0].queryIdx,
                         (*matchIterator1)[0].trainIdx,
                         (*matchIterator1)[0].distance));
            break; // next match in image 1 -> image 2
        }
      }
   }

}

void RobustMatcher::robustMatch( const CvMat& frame, std::vector<DMatch>& good_matches,
              std::vector<KeyPoint>& keypoints_frame, const CvMat& descriptors_model )
{

  // 1a. Detection of the ORB features
  this->computeKeyPoints(frame, keypoints_frame);

  // 1b. Extraction of the ORB descriptors
  CvMat descriptors_frame;
  this->computeDescriptors(frame, keypoints_frame, descriptors_frame);

  // 2. Match the two image descriptors
  std::vector<std::vector<DMatch> > matches12, matches21;

  // 2a. From image 1 to image 2
  matcher_->knnMatch(descriptors_frame, descriptors_model, matches12, 2); // return 2 nearest neighbours

  // 2b. From image 2 to image 1
  matcher_->knnMatch(descriptors_model, descriptors_frame, matches21, 2); // return 2 nearest neighbours

  // 3. Remove matches for which NN ratio is > than threshold
  // clean image 1 -> image 2 matches
  ratioTest(matches12);
  // clean image 2 -> image 1 matches
  ratioTest(matches21);

  // 4. Remove non-symmetrical matches
  symmetryTest(matches12, matches21, good_matches);

}

void RobustMatcher::fastRobustMatch( const CvMat& frame, std::vector<DMatch>& good_matches,
                                 std::vector<KeyPoint>& keypoints_frame,
                                 const CvMat& descriptors_model )
{
  good_matches.clear();

  // 1a. Detection of the ORB features
  this->computeKeyPoints(frame, keypoints_frame);

  // 1b. Extraction of the ORB descriptors
  CvMat descriptors_frame;
  this->computeDescriptors(frame, keypoints_frame, descriptors_frame);

  // 2. Match the two image descriptors
  std::vector<std::vector<DMatch> > matches;
  matcher_->knnMatch(descriptors_frame, descriptors_model, matches, 2);

  // 3. Remove matches for which NN ratio is > than threshold
  ratioTest(matches);

  // 4. Fill good matches container
  for ( std::vector<std::vector<DMatch> >::iterator
         matchIterator= matches.begin(); matchIterator!= matches.end(); ++matchIterator)
  {
    if (!matchIterator->empty()) good_matches.push_back((*matchIterator)[0]);
  }

}
