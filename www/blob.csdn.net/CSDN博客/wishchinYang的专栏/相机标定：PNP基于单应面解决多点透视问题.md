# 相机标定：PNP基于单应面解决多点透视问题 - wishchinYang的专栏 - CSDN博客
2016年05月18日 19:18:51[wishchin](https://me.csdn.net/wishchin)阅读数：2482
![](https://img-blog.csdn.net/20170813215207481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170813215207481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        利用二维视野内的图像，求出三维图像在场景中的位姿，这是一个三维透视投影的反向求解问题。常用方法是PNP方法，需要已知三维点集的原始模型。
        本文做了大量修改，如有不适，请移步原文：  文章：[张正友相机标定&OpenCV实现&程序评价&矫正流程解析](http://blog.csdn.net/dcrmg/article/details/52939318)
        文章：[相机标定原理介绍](http://www.cnblogs.com/Jessica-jie/p/6596450.html)----[相机标定](http://blog.csdn.net/honyniu/article/details/51004397)---
**相机模型**
        根据光学成像的基本原理，针孔相机在定焦时候有固定的投射关系，这个投射关系是相机参数的大致决定因素。但是对于现实中的相机来说，相机参数会与理想模型有些偏差，涉及到几个因素：第一、相机安装导致光轴不准；第二、使用光学透镜导致成像畸变；第三、若实现变焦功能则整个模型参数都会发生改变；第四、若是双目相机，相机安装导致光轴不平行。
        且对于工业制品，相机的焦距长度只能精确到其标称值的4%，只有对焦到无穷远处时相机时焦距长度才是固定的。
        所以直接根据投射关系来推算相机参数几乎是不准确的，因此对相机需要参数进行更细致地描述，由此引发了相机标定过程，使用外在的标准几何模型，来直接推算相机的内部参数。
        相机标定的基本原理仍然是透视法则即小孔成像原理，标准几何模型一般使用平面点集根据所谓的单应性来推算相机的单应矩阵，进而解算出内参。
**相机标定**
        可惜了，只标定过一次之后便弃之不用，且只是标定过一个相机，所以标定方法将近生疏。
拿来主义就是方便，直接抄，省得想逻辑完备性！看知乎：[机器视觉的相机标定到底在标定什么](https://www.zhihu.com/question/29448299)？
        所以相机标定得到的内参仅仅是对相机物理特性的【近似】，这一点有些人可能一辈子都没办法意识到。
        传统相机标定假设相机是小孔成像模型，一般使用两种畸变来模拟镜片的物理畸变。但实际相机的物理特性很可能没办法通过上述假设来得到完全的拟合。所以需要意识到，每一次相机标定仅仅只是对物理相机模型的一次近似，再具体一点来说，每一次标定仅仅是对相机物理模型在采样空间范围内的一次近似。
        所以当成像物体所在的空间跟相机标定时的采样空间不一样的时候，你可能永远都没办法得到足够的精度，当你大幅改变相机与成像物体的距离的时候，你最好重新标定相机。如果你想在一个空间里得到更高的精度，你可以在空间里分层多次标定，实际计算的时候通过其他方式得到成像距离，从而选择合适的标定参数。
        传统标定方法需要相机拍摄一个三维标定靶，但仍然估计不出相机畸变。
**张正友标定法  **
       文章：[张正友相机标定&OpenCV实现&程序评价&矫正流程解析](http://blog.csdn.net/dcrmg/article/details/52939318)
       文章：[张正友相机标定的原理与实现](http://blog.csdn.net/lsh_2013/article/details/47617909)
       1999年，微软研究院的张正友提出了基于移动平面模板的相机标定方法。此方法是介于传统标定方法和自标定方法之间的一种方法，传统标定方法虽然精度高设备有较高的要求，其操作过程也比较繁琐，自标定方法的精度不高，张正友标定算法克服了这两者的缺点同时又兼备二者的优点，因此对办公、家庭的场合使用的桌面视觉系统(DVS)很适合。
       张的文章题目叫做 A Flexible New Technique for Camera Calibration，如果使用高精度的测量仪器制作一个精度极高的参照物，然后把每个点的坐标输入程序进去，这当然是可以的，更经典的标定方法需要制作一个三维的标定物体以避免退化的情况，但是这些没有办法体现flexible。
       标定方法使用了立体视觉平面单应性的情景，一般图像标定使用三张图像实现平面就可以，即需要对一个标定平面在一个空间位置进行至少三个角度的采样。
**平面单应性应用**
        使用相机从不同角度观测平面上的点集合，满足了重建的单应性场景。根据单应性的约束，可以求取相机的单应矩阵。用于标定过程，可以求取相机的本质矩阵。
![](https://img-blog.csdn.net/20170614154342022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
约束关系为：
![](https://img-blog.csdn.net/20170614154442678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**平面的使用**
        标定方法建议使用标定板，保持标定板的平面性质是重要的。一般方法使用一个工业标准标定板，现阶段合适的方法是使用一个LCD显示器，工业级的显示器一般能保证标定图片的平面性。
**标定输出**
         2. 外参矩阵：现实世界点P( 世界坐标 )是怎样经过旋转R和平移t，然后落到另一个现实世界点( 摄像机坐标系 )上。
         1. 内参矩阵：点P在2的基础上，是如何继续经过摄像机的镜头、并通过针孔成像透视变换成为像素点（图像坐标系）。
        3. 畸变矩阵：上面那个像素点并没有落在理论计算该落在的位置上，还tm产生了一定的偏移和变形！！！
         好了，到这里是不是明白了一点？上述3点的每一个转换，都有已经有成熟的数学描述，通过计算，我们完全可以精确地重现现实世界的任意一个点到其数字图像上对应像素点的投影过程。
         对于双目视觉系统，通过立体标定还能进一步得到下面的参数:
        4.结构参数。告诉你右摄像机是怎样相对于左摄像机经过旋转和平移达到现在的位置。通过结构参数，便能把左右摄像机获取的图像的每一个像素点之间的关系用数学语言定量描述，保证两个相机都处于我们“可求”的状态。
**标定流程**
       标定流程按照这张图里面的原理
![](https://img-blog.csdn.net/20170731180139778?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
             文章：[张正友相机标定的原理与实现](http://blog.csdn.net/lsh_2013/article/details/47617909)
**标定程序的完整流程：**
    1、**准备输入棋盘格图片**：对一个标定板在不同位置、不同角度、不同姿态下拍摄，至少三张。一般使用黑白棋盘格。    
    2、**对棋盘格图片，提取角点信息**：对棋盘格图像检测角点，使用findChessboardCorners检测函数，专门检测棋盘格内角点。
    3、进行再次计算，**提取亚像素级角点信息**：提高精度，使用亚像素级角点寻找方法。
    4、**对相机进行标定-使用标定流程**：使用启动标定流程，可是直接使用OpenCV的calibrateCamera函数,输出内参外参........................................
    5、对标定结果进行评价
    6、查看标定结果，利用标定结果对棋盘格图进行矫正
**标定的具体流程是：**
    1)把被取的**十个点**的世界坐标(齐次坐标)进行转置。
    2)对单应性矩阵求解并优化。
    3)把六幅图的单应矩阵求解出来后求解出6向量(B矩阵)。因为每个单应矩阵可以得到两个方程，通过循环对矩阵y 赋值后，再对y进行正交分解即可得到6向量。进而得到相机的内参矩阵。
    4)先求解出相机的外参，然后对畸变系数进行求解，得到相机坐标（Xc, Yc, Zc）。
    5)调用函数对内参和畸变系数进行优化，并显示优化后的结果。然后根据优化后的结果求解外参矩阵。
    6)从旋转矩阵中分解出独立变量(三个坐标的转角)，再得到平移矩阵，最后把它们和内参、畸变系数一起优化进行最终优化。
 OpenCV代码：
```cpp
#include "opencv2/core/core.hpp"  
    #include "opencv2/imgproc/imgproc.hpp"  
    #include "opencv2/calib3d/calib3d.hpp"  
    #include "opencv2/highgui/highgui.hpp"  
    #include <iostream>  
    #include <fstream>  
      
    using namespace cv;
   using namespace std;
            double coff = 0.0;
            ifstream fin("calibdata.txt"); /* 标定所用图像文件的路径 */
            ofstream fout("caliberation_result.txt");  /* 保存标定结果的文件 */
            //读取每一幅图像，从中提取出角点，然后对角点进行亚像素精确化   
            cout << "开始提取角点………………";
            int image_count = 0;  /* 图像数量 */
            Size image_size;  /* 图像的尺寸 */
            Size board_size = Size(4, 6);    /* 标定板上每行、列的角点数 */
            std::vector<Point2f> image_points_buf;  /* 缓存每幅图像上检测到的角点 */
            std::vector<std::vector<Point2f>> image_points_seq; /* 保存检测到的所有角点 */
            string filename;
            int count = -1;//用于存储角点个数。  
            while (getline(fin, filename))
            {
                image_count++;
                // 用于观察检验输出  
                cout << "image_count = " << image_count << endl;
                /* 输出检验*/
                cout << "-->count = " << count;
                cv::Mat imageInput = imread(filename);
                if (image_count == 1)  //读入第一张图片时获取图像宽高信息  
                {
                    image_size.width = imageInput.cols;
                    image_size.height = imageInput.rows;
                    cout << "image_size.width = " << image_size.width << endl;
                    cout << "image_size.height = " << image_size.height << endl;
                }
                /* 提取角点 */
                if (0 == findChessboardCorners(imageInput, board_size, image_points_buf))
                {
                    cout << "can not find chessboard corners!\n"; //找不到角点  
                    exit(1);
                }
                else
                {
                    cv::Mat view_gray;
                    cvtColor(imageInput, view_gray, CV_RGB2GRAY);
                    /* 亚像素精确化 */
                    find4QuadCornerSubpix(view_gray, image_points_buf, Size(5, 5)); //对粗提取的角点进行精确化  
                    //cornerSubPix(view_gray,image_points_buf,Size(5,5),Size(-1,-1),TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,30,0.1));  
                    image_points_seq.push_back(image_points_buf);  //保存亚像素角点  
                    /* 在图像上显示角点位置 */
                    drawChessboardCorners(view_gray, board_size, image_points_buf, false); //用于在图片中标记角点  
                    imshow("Camera Calibration", view_gray);//显示图片  
                    waitKey(500);//暂停0.5S         
                }
            }
            int total = image_points_seq.size();
            cout << "total = " << total << endl;
            int CornerNum = board_size.width*board_size.height;  //每张图片上总的角点数  
            for (int ii = 0; ii < total; ii++)
            {
                if (0 == ii%CornerNum)// 24 是每幅图片的角点个数。此判断语句是为了输出 图片号，便于控制台观看   
                {
                    int i = -1;
                    i = ii / CornerNum;
                    int j = i + 1;
                    cout << "--> 第 " << j << "图片的数据 --> : " << endl;
                }
                if (0 == ii % 3)  // 此判断语句，格式化输出，便于控制台查看  
                {
                    cout << endl;
                }
                else
                {
                    cout.width(10);
                }
                //输出所有的角点  
                cout << " -->" << image_points_seq[ii][0].x;
                cout << " -->" << image_points_seq[ii][0].y;
            }
            cout << "角点提取完成！\n";
            //以下是摄像机标定  
            cout << "开始标定………………";
            /*棋盘三维信息*/
            Size square_size = Size(10, 10);  /* 实际测量得到的标定板上每个棋盘格的大小 */
            std::vector<std::vector<Point3f>> object_points; /* 保存标定板上角点的三维坐标 */
            /*内外参数*/
            cv::Mat cameraMatrix = cv::Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 摄像机内参数矩阵 */
            std::vector<int> point_counts;  // 每幅图像中角点的数量  
            cv::Mat distCoeffs = cv::Mat(1, 5, CV_32FC1, Scalar::all(0)); /* 摄像机的5个畸变系数：k1,k2,p1,p2,k3 */
            std::vector<cv::Mat> tvecsMat;  /* 每幅图像的旋转向量 */
            std::vector<cv::Mat> rvecsMat; /* 每幅图像的平移向量 */
            /* 初始化标定板上角点的三维坐标 */
            int i, j, t;
            for (t = 0; t < image_count; t++)
            {
                std::vector<Point3f> tempPointSet;
                for (i = 0; i < board_size.height; i++)
                {
                    for (j = 0; j < board_size.width; j++)
                    {
                        Point3f realPoint;
                        /* 假设标定板放在世界坐标系中z=0的平面上 */
                        realPoint.x = i*square_size.width;
                        realPoint.y = j*square_size.height;
                        realPoint.z = 0;
                        tempPointSet.push_back(realPoint);
                    }
                }
                object_points.push_back(tempPointSet);
            }
            /* 初始化每幅图像中的角点数量，假定每幅图像中都可以看到完整的标定板 */
            for (i = 0; i < image_count; i++)
            {
                point_counts.push_back(board_size.width*board_size.height);
            }
            /* 开始标定 */
            calibrateCamera(object_points, image_points_seq, image_size, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);
            cout << "标定完成！\n";
            //对标定结果进行评价  
            cout << "开始评价标定结果………………\n";
            double total_err = 0.0; /* 所有图像的平均误差的总和 */
            double err = 0.0; /* 每幅图像的平均误差 */
            std::vector<Point2f> image_points2; /* 保存重新计算得到的投影点 */
            cout << "\t每幅图像的标定误差：\n";
            fout << "每幅图像的标定误差：\n";
            for (i = 0; i < image_count; i++)
            {
                std::vector<Point3f> tempPointSet = object_points[i];
                /* 通过得到的摄像机内外参数，对空间的三维点进行重新投影计算，得到新的投影点 */
                projectPoints(tempPointSet, rvecsMat[i], tvecsMat[i], cameraMatrix, distCoeffs, image_points2);
                /* 计算新的投影点和旧的投影点之间的误差*/
                std::vector<Point2f> tempImagePoint = image_points_seq[i];
                cv::Mat tempImagePointMat = cv::Mat(1, tempImagePoint.size(), CV_32FC2);
                cv::Mat image_points2Mat = cv::Mat(1, image_points2.size(), CV_32FC2);
                for (int j = 0; j < tempImagePoint.size(); j++)
                {
                    image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
                    tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
                }
                err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
                total_err += err /= point_counts[i];
                std::cout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
                fout << "第" << i + 1 << "幅图像的平均误差：" << err << "像素" << endl;
            }
            std::cout << "总体平均误差：" << total_err / image_count << "像素" << endl;
            fout << "总体平均误差：" << total_err / image_count << "像素" << endl << endl;
            std::cout << "评价完成！" << endl;
            //保存定标结果      
            std::cout << "开始保存定标结果………………" << endl;
            cv::Mat rotation_Matrix = cv::Mat(3, 3, CV_32FC1, Scalar::all(0)); /* 保存每幅图像的旋转矩阵 */
            fout << "相机内参数矩阵：" << endl;
            fout << cameraMatrix << endl << endl;
            fout << "畸变系数：\n";
            fout << distCoeffs << endl << endl << endl;
            for (int i = 0; i < image_count; i++)
            {
                fout << "第" << i + 1 << "幅图像的旋转向量：" << endl;
                fout << tvecsMat[i] << endl;
                /* 将旋转向量转换为相对应的旋转矩阵 */
                Rodrigues(tvecsMat[i], rotation_cv::Matrix);
                fout << "第" << i + 1 << "幅图像的旋转矩阵：" << endl;
                fout << rotation_Matrix << endl;
                fout << "第" << i + 1 << "幅图像的平移向量：" << endl;
                fout << rvecsMat[i] << endl << endl;
            }
            std::cout << "完成保存" << endl;
            fout << endl;
            /************************************************************************
            显示定标结果
            *************************************************************************/
            cv::Mat mapx = cv::Mat(image_size, CV_32FC1);
            cv::Mat mapy = cv::Mat(image_size, CV_32FC1);
            cv::Mat R = cv::Mat::eye(3, 3, CV_32F);
            std::cout << "保存矫正图像" << endl;
            string imageFileName;
            std::stringstream StrStm;
            for (int i = 0; i != image_count; i++)
            {
                std::cout << "Frame #" << i + 1 << "..." << endl;
                initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, image_size, CV_32FC1, mapx, mapy);
                StrStm.clear();
                imageFileName.clear();
                string filePath = "chess";
                StrStm << i + 1;
                StrStm >> imageFileName;
                filePath += imageFileName;
                filePath += ".bmp";
                cv::Mat imageSource = imread(filePath);
                cv::Mat newimage = imageSource.clone();
                //另一种不需要转换矩阵的方式  
                //undistort(imageSource,newimage,cameracv::Matrix,distCoeffs);  
                remap(imageSource, newimage, mapx, mapy, INTER_LINEAR);
                StrStm.clear();
                filePath.clear();
                StrStm << i + 1;
                StrStm >> imageFileName;
                imageFileName += "_d.jpg";
                imwrite(imageFileName, newimage);
            }
            std::cout << "保存结束" << endl;
            return coff;
    }
```
**后记**：
       相机标定其实是一个体力活，所以标定一次之后几乎再也没有自行标定过..............................
       此后，建议使用平板电脑，不过大多数平板电脑不适特别的平。
