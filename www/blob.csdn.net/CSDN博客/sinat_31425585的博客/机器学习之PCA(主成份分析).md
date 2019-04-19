# 机器学习之PCA(主成份分析) - sinat_31425585的博客 - CSDN博客
2015年11月13日 20:19:19[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：504

## 1.PCA的思想：
在尽可能的保留原始信息的同时，通过投影变换， 将高维空间的样本数据投影到低维的空间中。 
这是几个意思嘞？举个例子说一下： 
![这里写图片描述](https://img-blog.csdn.net/20151112103449533)
如图所示，黑色的坐标系X0Y表示原始的坐标系，红色的坐标系XOY表示经过一定的变换（旋转，平移）得到的新的坐标系。在原始的坐标系下表示A，B两点坐标的形式为（1，2），（2，2），也即是说在原始的坐标系下，想表示A，B需要一个2维的坐标表示。而在新的坐标系下，通过观察，我们可以知道，A，B两个点纵坐标相对于横坐标而言，非常的小，所以，在这里我们可以做近似处理，用A，B在新的坐标系下的横坐标近似表示A,B两个点。这就是PCA的思想。 
**将高维空间的样本进行投影变换（包括旋转，平移等），形成一个新的高维空间（这个时候，样本的维数没有发生改变），在新的高维空间中，样本的坐标中，部分坐标相对于其余的坐标较大，我们选取这些较大的坐标（这就是所谓的主成份）近似的表示原样本，舍弃那些较小的坐标（这些坐标包含的信息量太小，可以忽略）。**
  那么问题来了，这个新的坐标系怎么寻找？怎么判断找到的坐标系是最优的？ 
## 2.新的坐标系确定 
我们在画一个坐标系的时候，首先第一步需要做的事情就是确定坐标系的原点，所以在寻找新的坐标系的时候，第一步就是找寻新的坐标系原点。 
### 2.1 坐标原点的确定 
我们确定原点的方法是将所有的样本进行投影，让他们投影到一个点，使得这个点对应的向量![这里写图片描述](https://img-blog.csdn.net/20151112110601553)到这n个样本的距离平方和![这里写图片描述](https://img-blog.csdn.net/20151112111002350)最小： 
![这里写图片描述](https://img-blog.csdn.net/20151112111646423)
用![这里写图片描述](https://img-blog.csdn.net/20151112111825362)表示样本的平均值，即![这里写图片描述](https://img-blog.csdn.net/20151112112115945)，所以
![这里写图片描述](https://img-blog.csdn.net/20151112113236026)
通过观察可以得到，当`![这里写图片描述](https://img-blog.csdn.net/20151112120814957)时取最小，即原点为m
### 2.2 投影坐标系坐标轴确定 
这里，以一个坐标轴为例 
![这里写图片描述](https://img-blog.csdn.net/20151112124901538) 上面的推导可以得出一个结论：当我们按照特征向量进行投影时，能够最小化近似处理所带来的误差，所以我们选取的其余坐标轴方向为剩余的特征值对应的特征向量，这样就能在尽量保存原始样本对应的信息量的同时，实现对样本的降维。 . 
## 3.主成份分析的一般步骤： 
### 1）求样本的平均值 
![这里写图片描述](https://img-blog.csdn.net/20151112125026935)
### 2）求散布矩阵 
![这里写图片描述](https://img-blog.csdn.net/20151111213856412)
### 3）求散布矩阵的特征值和特征向量 
![这里写图片描述](https://img-blog.csdn.net/20151111214319991)
### 4）主成份的选取 
![这里写图片描述](https://img-blog.csdn.net/20151111214932753)
  这里的向量e是特征值对应的特征向量，选取的原则为：优先选择特征值大的特征向量。 
### 5）投影后样本的计算 
![这里写图片描述](https://img-blog.csdn.net/20151111215846576)
这个式子表示，每个样本要投影到d个方向。
最后，贴一个基于opencv特征脸的代码，代码来自于satya Mallick，[EigenFace](https://github.com/spmallick/learnopencv/tree/master/EigenFace)
```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
using namespace cv;
using namespace std;
#define MAX_SLIDER_VALUE 255
#define NUM_EIGEN_FACES 10
// Weights for the different eigenvectors
int sliderValues[NUM_EIGEN_FACES];
// Matrices for average (mean) and eigenvectors
Mat averageFace;
vector<Mat> eigenFaces;
// Read jpg files from the directory
void readImages(string dirName, vector<Mat> &images)
{
  
  cout << "Reading images from " << dirName;
  // Add slash to directory name if missing
  if (!dirName.empty() && dirName.back() != '/')
    dirName += '/';
  
  DIR *dir;
  struct dirent *ent;
  int count = 0;
  
  //image extensions
  string imgExt = "jpg";
  vector<string> files;
  
  if ((dir = opendir (dirName.c_str())) != NULL)
  {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL)
    {
      if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0 )
      {
        continue;
      }
      string fname = ent->d_name;
      
      if (fname.find(imgExt, (fname.length() - imgExt.length())) != std::string::npos)
      {
        string path = dirName + fname;
        Mat img = imread(path);
        if(!img.data)
        {
          cout << "image " << path << " not read properly" << endl;
        }
        else
        { 
          // Convert images to floating point type
          img.convertTo(img, CV_32FC3, 1/255.0);
          images.push_back(img);
          
          // A vertically flipped image is also a valid face image.
          // So lets use them as well.
          Mat imgFlip;
          flip(img, imgFlip, 1);
          images.push_back(imgFlip);
        }
      }
    }
    closedir (dir);
  }
  // Exit program if no images are found
  if(images.empty())exit(EXIT_FAILURE);
  
  cout << "... " << images.size() / 2 << " files read"<< endl;
  
}
// Create data matrix from a vector of images
static  Mat createDataMatrix(const vector<Mat> &images)
{
  cout << "Creating data matrix from images ...";
  
  // Allocate space for all images in one data matrix.
  // The size of the data matrix is
  //
  // ( w  * h  * 3, numImages )
  //
  // where,
  //
  // w = width of an image in the dataset.
  // h = height of an image in the dataset.
  // 3 is for the 3 color channels.
  
  
  Mat data(static_cast<int>(images.size()), images[0].rows * images[0].cols * 3, CV_32F);
  
  // Turn an image into one row vector in the data matrix
  for(unsigned int i = 0; i < images.size(); i++)
  {
    // Extract image as one long vector of size w x h x 3
    Mat image = images[i].reshape(1,1);
    
    // Copy the long vector into one row of the destm
    image.copyTo(data.row(i));
    
  }
  
  cout << " DONE" << endl;
  return data;
}
// Calculate final image by adding weighted
// EigenFaces to the average face.
void createNewFace(int ,void *)
{
  // Start with the mean image
  Mat output = averageFace.clone();
  
  // Add the eigen faces with the weights
  for(int i = 0; i < NUM_EIGEN_FACES; i++)
  {
    // OpenCV does not allow slider values to be negative.
    // So we use weight = sliderValue - MAX_SLIDER_VALUE / 2
    double weight = sliderValues[i] - MAX_SLIDER_VALUE/2;
    output = output + eigenFaces[i] * weight;
  }
  resize(output, output, Size(), 2, 2);
  
  imshow("Result", output);
  
}
// Reset slider values
void resetSliderValues(int event, int x, int y, int flags, void* userdata)
{
  if (event == EVENT_LBUTTONDOWN)
  {
    for(int i = 0; i < NUM_EIGEN_FACES; i++)
    {
      sliderValues[i] = 128;
      setTrackbarPos("Weight" + to_string(i), "Trackbars", MAX_SLIDER_VALUE/2);
    }
    
    createNewFace(0,0);
    
  }
}
int main(int argc, char **argv)
{
  // Directory containing images
  string dirName = "images/";
  
  // Read images in the directory
  vector<Mat> images; 
  readImages(dirName, images);
  
  // Size of images. All images should be the same size. 
  Size sz = images[0].size(); 
  
  // Create data matrix for PCA.
  Mat data = createDataMatrix(images);
  
  // Calculate PCA of the data matrix
  cout << "Calculating PCA ...";
  PCA pca(data, Mat(), PCA::DATA_AS_ROW, NUM_EIGEN_FACES);
  cout << " DONE"<< endl;
  
  // Extract mean vector and reshape it to obtain average face
  averageFace = pca.mean.reshape(3,sz.height);
  
  // Find eigen vectors.
  Mat eigenVectors = pca.eigenvectors;
  
  // Reshape Eigenvectors to obtain EigenFaces
  for(int i = 0; i < NUM_EIGEN_FACES; i++)
  {
      Mat eigenFace = eigenVectors.row(i).reshape(3,sz.height);
      eigenFaces.push_back(eigenFace);
  }
  
  // Show mean face image at 2x the original size
  Mat output;
  resize(averageFace, output, Size(), 2, 2);
  
  namedWindow("Result", CV_WINDOW_AUTOSIZE);
  imshow("Result", output);
  
  // Create trackbars
  namedWindow("Trackbars", CV_WINDOW_AUTOSIZE);
  for(int i = 0; i < NUM_EIGEN_FACES; i++)
  {
    sliderValues[i] = MAX_SLIDER_VALUE/2;
    createTrackbar( "Weight" + to_string(i), "Trackbars", &sliderValues[i], MAX_SLIDER_VALUE, createNewFace);
  }
  
  // You can reset the sliders by clicking on the mean image.
  setMouseCallback("Result", resetSliderValues);
  
  cout << "Usage:" << endl 
  << "\tChange the weights using the sliders" << endl
  << "\tClick on the result window to reset sliders" << endl
  << "\tHit ESC to terminate program." << endl;
  waitKey(0);
  destroyAllWindows(); 
}
```
