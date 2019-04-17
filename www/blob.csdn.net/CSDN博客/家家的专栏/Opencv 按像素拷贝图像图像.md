# Opencv 按像素拷贝图像图像 - 家家的专栏 - CSDN博客





2015年04月05日 13:51:41[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1462








    cv::Mat gridimg(96,96,CV_8UC3,cv::Scalar(128,128,128)); 



   for (int c=0; c<mat.channels(); c++){

    for (int h=0; h<mat.rows; h++) {

    for (int w=0; w<mat.cols; w++) {

              gridimg.at<cv::Vec3b>(h+hOffset, w+wOffset)[c] =

                       mat.at<cv::Vec3b>(h, w)[c];

  //         LOG(INFO) << "gridimg:" << static_cast<int>(gridimg.at<cv::Vec3b>(h+hOffset, w+wOffset)[c]);

  //         LOG(INFO) << "mat:" << static_cast<int>(mat.at<cv::Vec3b>(h, w)[c]);

        }

      }

    }
            


