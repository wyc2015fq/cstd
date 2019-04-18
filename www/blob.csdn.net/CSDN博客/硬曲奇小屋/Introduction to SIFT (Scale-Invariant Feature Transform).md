# Introduction to SIFT (Scale-Invariant Feature Transform) - 硬曲奇小屋 - CSDN博客





2017年10月24日 16:24:49[HardCookies](https://me.csdn.net/james_616)阅读数：114标签：[sift](https://so.csdn.net/so/search/s.do?q=sift&t=blog)
个人分类：[opencv](https://blog.csdn.net/james_616/article/category/7246002)









## Notes

This main page comes from the [OpenCV-Python Tutorials](https://docs.opencv.org/3.1.0/da/df5/tutorial_py_sift_intro.html). 

I converted the code from Python to C++ and commented the uesage relating functions.

## Theory 

In last couple of chapters, we saw some corner detectors like Harris etc. They are rotation-invariant, which means, even if the image is rotated, we can find the same corners. It is obvious because corners remain corners in rotated image also. But what about scaling? A corner may not be a corner if the image is scaled. For example, check a simple image below. A corner in a small image within a small window is flat when it is zoomed in the same window. So Harris corner is not scale invariant.



![sift_scale_invariant.jpg](https://docs.opencv.org/3.1.0/sift_scale_invariant.jpg)


 So, in 2004, **D.Lowe**, University of British Columbia, came up with a new algorithm, Scale Invariant Feature Transform (SIFT) in his paper, **Distinctive Image Features from Scale-Invariant Keypoints**, which extract keypoints and compute its descriptors. *(This paper is easy to understand and considered to be best material available on SIFT. So this explanation is just a short summary of this paper)*.

There are mainly four steps involved in SIFT algorithm. We will see them one-by-one.

### 1. Scale-space Extrema Detection

From the image above, it is obvious that we can’t use the same window to detect keypoints with different scale. It is OK with small corner. But to detect larger corners we need larger windows. For this, scale-space filtering is used. In it, Laplacian of Gaussian is found for the image with various  scale.

But this LoG is a little costly, so SIFT algorithm uses Difference of Gaussians which is an approximation of LoG. Difference of Gaussian is obtained as the difference of Gaussian blurring of an image with two different . This process is done for different octaves of the image in Gaussian Pyramid. It is represented in below image:



![sift_dog.jpg](https://docs.opencv.org/3.1.0/sift_dog.jpg)


 Once this DoG are found, images are searched for local extrema over scale and space. For eg, one pixel in an image is compared with its 8 neighbours as well as 9 pixels in next scale and 9 pixels in previous scales. If it is a local extrema, it is a potential keypoint. It basically means that keypoint is best represented in that scale. It is shown in below image:



![sift_local_extrema.jpg](https://docs.opencv.org/3.1.0/sift_local_extrema.jpg)


 Regarding different parameters, the paper gives some empirical data which can be summarized as, number of octaves = 4, number of scale levels = 5, initial  etc as optimal values.

### 2. Keypoint Localization

Once potential keypoints locations are found, they have to be refined to get more accurate results. They used Taylor series expansion of scale space to get more accurate location of extrema, and if the intensity at this extrema is less than a threshold value (0.03 as per the paper), it is rejected. This threshold is called **contrastThreshold** in OpenCV

DoG has higher response for edges, so edges also need to be removed. For this, a concept similar to Harris corner detector is used. They used a 2x2 Hessian matrix (H) to compute the pricipal curvature. We know from Harris corner detector that for edges, one eigen value is larger than the other. So here they used a simple function,

If this ratio is greater than a threshold, called **edgeThreshold** in OpenCV, that keypoint is discarded. It is given as 10 in paper.

So it eliminates any low-contrast keypoints and edge keypoints and what remains is strong interest points.

### 3. Orientation Assignment

Now an orientation is assigned to each keypoint to achieve invariance to image rotation. A neigbourhood is taken around the keypoint location depending on the scale, and the gradient magnitude and direction is calculated in that region. An orientation histogram with 36 bins covering 360 degrees is created. (It is weighted by gradient magnitude and gaussian-weighted circular window with  equal to 1.5 times the scale of keypoint. The highest peak in the histogram is taken and any peak above 80% of it is also considered to calculate the orientation. It creates keypoints with same location and scale, but different directions. It contribute to stability of matching.

### 4. Keypoint Descriptor

Now keypoint descriptor is created. A 16x16 neighbourhood around the keypoint is taken. It is devided into 16 sub-blocks of 4x4 size. For each sub-block, 8 bin orientation histogram is created. So a total of 128 bin values are available. It is represented as a vector to form keypoint descriptor. In addition to this, several measures are taken to achieve robustness against illumination changes, rotation etc.

### 5. Keypoint Matching

Keypoints between two images are matched by identifying their nearest neighbours. But in some cases, the second closest-match may be very near to the first. It may happen due to noise or some other reasons. In that case, ratio of closest-distance to second-closest distance is taken. If it is greater than 0.8, they are rejected. It eliminaters around 90% of false matches while discards only 5% correct matches, as per the paper.

So this is a summary of SIFT algorithm. For more details and understanding, reading the original paper is highly recommended. Remember one thing, this algorithm is patented. So this algorithm is included in [the opencv contrib repo](https://github.com/Itseez/opencv_contrib)

## SIFT in OpenCV 

So now let’s see SIFT functionalities available in OpenCV. Let’s start with keypoint detection and draw them. First we have to construct a SIFT object. We can pass different parameters to it which are optional and they are well explained in docs. 

```cpp
#include <stdio.h>
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

void readme();

/* @function main */
int main( int argc, char** argv )
{
  if( argc != 3 )
  { readme(); return -1; }
  Mat img_1 = imread( argv[1], IMREAD_GRAYSCALE );
  Mat img_2 = imread( argv[2], IMREAD_GRAYSCALE );
  if( !img_1.data || !img_2.data )
  { cout<< " --(!) Error reading images " << std::endl; return -1; }
  //-- Step 1: Detect the keypoints using SIFT Detector
  Ptr<SIFT> detector = SIFT::create();
  vector<KeyPoint> keypoints_1, keypoints_2;
  detector->detect( img_1, keypoints_1 );
  detector->detect( img_2, keypoints_2 );
  //-- Draw keypoints
  Mat img_keypoints_1; Mat img_keypoints_2;
  drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
  drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
  //-- Show detected (drawn) keypoints
  imshow("Keypoints 1", img_keypoints_1 );
  imshow("Keypoints 2", img_keypoints_2 );
  cout << "Keypoints 1 numnber: " << keypoints_1.size() << endl;
  cout << "Keypoints 2 numnber: " << keypoints_2.size() << endl;
  waitKey(0);
  return 0;
  }

  /* @function readme */
  void readme()
  { std::cout << " Usage: ./SIFT_detector <img1> <img2>" << std::endl; }
```

Build and run 
`./SIFT_detector box.png box_in_scene.png`

We will get the keypoints marked of the above 2 images. 

![这里写图片描述](https://img-blog.csdn.net/20171024161813540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

box.png




![这里写图片描述](https://img-blog.csdn.net/20171024161938624?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

box_in_scene.png


We can also get the keypoints number of the 2 images. 

```
Keypoints 1 numnber: 604 

Keypoints 2 numnber: 969
```

We first create a SIFT detector, and then use the detector detecting the keypoints of two images. So we get the *keypoints_1* and *keypoints_2* then we draw them using **drawKeypoints** . In the end, we show the keypoints and print the number of them. 

## Additional Resources 

[Lowe: Distinctive image features from scale-invariant keypoints](https://www-inst.eecs.berkeley.edu/~ee225b/fa12/readings/sift-lowe.pdf)
[OpenCV: SIFT Class Reference](https://docs.opencv.org/3.1.0/d5/d3c/classcv_1_1xfeatures2d_1_1SIFT.html)
[OpenCV: Drawing Function of Keypoints and Matches](https://docs.opencv.org/3.1.0/d4/d5d/group__features2d__draw.html#gab958f8900dd10f14316521c149a60433)





