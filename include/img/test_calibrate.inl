
// OpenCV: 摄像机标定原理

int test_calibrate()
{
  int cube_length = 7;

  CvCapture* capture;

  capture = cvCreateCameraCapture(0);

  if (capture == 0) {
    printf("无法捕获摄像头设备！\n\n");
    return 0;
  }
  else {
    printf("捕获摄像头设备成功！！\n\n");
  }

  IplImage* frame;

  cvNamedWindow("摄像机帧截取窗口", 1); //cvNamedWindow()函数用于在屏幕上创建一个窗口，将被显示的图像包含于该窗口中。函数的第一个参数指定了该窗口的窗口标题,如果要使用HighGUI库所提供的其他函数与该窗口进行交互时，我们将通过该参数值引用这个窗口。

  printf("按“C”键截取当前帧并保存为标定图片...\n按“Q”键退出截取帧过程...\n\n");

  int number_image = 1;
  char* str1;
  str1 = ".jpg";
  char filename[20] = "";

  while (true) {
    frame = cvQueryFrame(capture); // 从摄像头或者文件中抓取并返回一帧

    if (!frame) {
      break;
    }

    cvShowImage("摄像机帧截取窗口", frame); //图像显示


    if (cvWaitKey(10) == 'c') {
      sprintf_s(filename, "%d.jpg", number_image); // int sprintf_s( char *buffer, size_t sizeOfBuffer, const char *format [, argument] ... );


      这个函数的主要作用是将若干个argument按照format格式存到buffer中



      cvSaveImage(filename, frame); //保存
      cout << "成功获取当前帧，并以文件名" << filename << "保存...\n\n";
      printf("按“C”键截取当前帧并保存为标定图片...\n按“Q”键退出截取帧过程...\n\n");
      number_image++;
    }
    else if (cvWaitKey(10) == 'q') {
      printf("截取图像帧过程完成...\n\n");
      cout << "共成功截取" << --number_image << "帧图像！！\n\n";
      break;
    }
  }

  cvReleaseImage(&frame); //释放图像

  cvDestroyWindow("摄像机帧截取窗口");

  IplImage* show;
  cvNamedWindow("RePlay", 1);

  int a = 1;
  int number_image_copy = number_image;

  CvSize board_size = cvSize(7, 7); // Cvsizes:OpenCV的基本数据类型之一。表示矩阵框大小，以像素为精度。与CvPoint结构类似，但数据成员是integer类型的width和height。
  //cvSize是

  int board_width = board_size.width;
  int board_height = board_size.height;
  int total_per_image = board_width * board_height;
  CvPoint2D32f* image_points_buf = new CvPoint2D32f[total_per_image];
  CvMat* image_points = cvCreateMat(number_image * total_per_image, 2, CC_32FC1); //图像坐标系
  CvMat* object_points = cvCreateMat(number_image * total_per_image, 3, CC_32FC1); //世界坐标系
  CvMat* point_counts = cvCreateMat(number_image, 1, CC_32SC1); //
  CvMat* intrinsic_matrix = cvCreateMat(3, 3, CC_32FC1); //
  CvMat* distortion_coeffs = cvCreateMat(5, 1, CC_32FC1);

  int count;
  int found;
  int step;
  int successes = 0;

  while (a <= number_image_copy) {
    sprintf_s(filename, "%d.jpg", a);
    show = cvLoadImage(filename, -1);

    found = cvFindChessboardCorners(show, board_size, image_points_buf, &count,
        CC_CALIB_CB_ADAPTIVE_THRESH | CC_CALIB_CB_FILTER_QUADS);

    if (found == 0) {
      cout << "第" << a << "帧图片无法找到棋盘格所有角点!\n\n";
      cvNamedWindow("RePlay", 1);
      cvShowImage("RePlay", show);
      cvWaitKey(0);

    }
    else {
      cout << "第" << a << "帧图像成功获得" << count << "个角点...\n";

      cvNamedWindow("RePlay", 1);

      IplImage* gray_image = cvCreateImage(cvGetSize(show), 8, 1); //创建头并分配数据IplImage* cvCreateImage( CvSize size, int depth, int channels ); depth 图像元素的位深度

      cvCvtColor(show, gray_image, CC_BGR2GRAY); // cvCvtColor(...),是Opencv里的颜色空间转换函数，可以实现rgb颜色向HSV,HSI等颜色空间的转换，也可以转换为灰度图像。

      cout << "获取源图像灰度图过程完成...\n";
cvFindCornerSubPix(gray_image, image_points_buf, count, cvSize(11, 11), cvSize(-1, -1), 由于非常接近P的像素产生了很小的特征值，所以这个自相关矩阵并不总是可逆的。为了解决这个问题，一般可以简单地剔除离P点非常近的像素。输入参数: ero_zone定义了一个禁区(与win相似，但通常比win小)，这个区域在方程组以及自相关矩阵中不被考虑。如果不需要这样一个禁区，则zero_zone应设置为cvSize(-1， -1)0
          cvTermCriteria(CC_TERMCRIT_EPS + CC_TERMCRIT_ITER, 30, 0.1));
      cout << "灰度图亚像素化过程完成...\n";
      cvDrawChessboardCorners(show, board_size, image_points_buf, count, found);
      cout << "在源图像上绘制角点过程完成...\n\n";
      cvShowImage("RePlay", show);
      cvWaitKey(0);
    }

    if (total_per_image == count) {
      step = successes * total_per_image;

      for (int i = step, j = 0; j < total_per_image; ++i, ++j) {
        CC_MAT_ELEM(*image_points, float, i, 0) = image_points_buf[j].x; // opencv中用来访问矩阵每个元素的宏，这个宏只对单通道矩阵有效，多通道CC_MAT_ELEM( matrix, elemtype, row, col )参数　　matrix：要访问的矩阵　　elemtype：矩阵元素的类型　　row：所要访问元素的行数　　col：所要访问元素的列数

        CC_MAT_ELEM(*image_points, float, i, 1) = image_points_buf[j].y; // 求完每个角点横纵坐标值都存在image_point_buf里
        CC_MAT_ELEM(*object_points, float, i, 0) = (float)(j / cube_length);
        CC_MAT_ELEM(*object_points, float, i, 1) = (float)(j % cube_length);
        CC_MAT_ELEM(*object_points, float, i, 2) = 0.0f;
      }

      CC_MAT_ELEM(*point_counts, int, successes, 0) = total_per_image;
      successes++;
    }

    a++;
  }

  cvReleaseImage(&show);
  cvDestroyWindow("RePlay");
  cout << "*********************************************\n";
  cout << number_image << "帧图片中，标定成功的图片为" << successes << "帧...\n";
  cout << number_image << "帧图片中，标定失败的图片为" << number_image - successes << "帧...\n\n";
  cout << "*********************************************\n\n";

  cout << "按任意键开始计算摄像机内参数...\n\n";


  CvCapture* capture1;
  capture1 = cvCreateCameraCapture(0);
  IplImage* show_colie;
  show_colie = cvQueryFrame(capture1);


  CvMat* object_points2 = cvCreateMat(successes * total_per_image, 3, CC_32FC1); // OpenCV 中重要的矩阵变换函数，使用方法为cvMat* cvCreateMat ( int rows, int cols, int type ); 这里type可以是任何预定义类型，预定义类型的结构如下：CC_<bit_depth> (S|U|F)C<number_of_channels>。

  CvMat* image_points2 = cvCreateMat(successes * total_per_image, 2, CC_32FC1);
  CvMat* point_counts2 = cvCreateMat(successes, 1, CC_32SC1);

  for (int i = 0; i < successes * total_per_image; ++i) {
    CC_MAT_ELEM(*image_points2, float, i, 0) = CC_MAT_ELEM(*image_points, float, i, 0); //用来存储角点提取成功的图像的角点
    CC_MAT_ELEM(*image_points2, float, i, 1) = CC_MAT_ELEM(*image_points, float, i, 1);
    CC_MAT_ELEM(*object_points2, float, i, 0) = CC_MAT_ELEM(*object_points, float, i, 0);
    CC_MAT_ELEM(*object_points2, float, i, 1) = CC_MAT_ELEM(*object_points, float, i, 1);
    CC_MAT_ELEM(*object_points2, float, i, 2) = CC_MAT_ELEM(*object_points, float, i, 2);
  }

  for (int i = 0; i < successes; ++i) {
    CC_MAT_ELEM(*point_counts2, int, i, 0) = CC_MAT_ELEM(*point_counts, int, i, 0);
  }


  cvReleaseMat(&object_points);
  cvReleaseMat(&image_points);
  cvReleaseMat(&point_counts);


  CC_MAT_ELEM(*intrinsic_matrix, float, 0, 0) = 1.0f;
  CC_MAT_ELEM(*intrinsic_matrix, float, 1, 1) = 1.0f;


  cvCalibrateCamera2(object_points2, image_points2, point_counts2, cvGetSize(show_colie),
      intrinsic_matrix, distortion_coeffs, NULL, NULL, 0);

  cout << "摄像机内参数矩阵为：\n";
  cout << CC_MAT_ELEM(*intrinsic_matrix, float, 0, 0) << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 0, 1)
      << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 0, 2)
      << "\n\n";
  cout << CC_MAT_ELEM(*intrinsic_matrix, float, 1, 0) << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 1, 1)
      << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 1, 2)
      << "\n\n";
  cout << CC_MAT_ELEM(*intrinsic_matrix, float, 2, 0) << "    " << CC_MAT_ELEM(*intrinsic_matrix, float, 2, 1)
      << "          " << CC_MAT_ELEM(*intrinsic_matrix, float, 2, 2)
      << "\n\n";

  cout << "畸变系数矩阵为：\n";
  cout << CC_MAT_ELEM(*distortion_coeffs, float, 0, 0) << "    " << CC_MAT_ELEM(*distortion_coeffs, float, 1, 0)
      << "    " << CC_MAT_ELEM(*distortion_coeffs, float, 2, 0)
      << "    " << CC_MAT_ELEM(*distortion_coeffs, float, 3, 0)
      << "    " << CC_MAT_ELEM(*distortion_coeffs, float, 4, 0)
      << "\n\n";

  cvSave("Intrinsics.xml", intrinsic_matrix);
  cvSave("Distortion.xml", distortion_coeffs);

  cout << "摄像机矩阵、畸变系数向量已经分别存储在名为Intrinsics.xml、Distortion.xml文档中\n\n";

  CvMat* intrinsic = (CvMat*)cvLoad("Intrinsics.xml");
  CvMat* distortion = (CvMat*)cvLoad("Distortion.xml");

  IplImage* mapx = cvCreateImage(cvGetSize(show_colie), IMG_DEPTH_32F, 1);
  IplImage* mapy = cvCreateImage(cvGetSize(show_colie), IMG_DEPTH_32F, 1);

  cvInitUndistortMap(intrinsic, distortion, mapx, mapy);

  cvNamedWindow("原始图像", 1);
  cvNamedWindow("非畸变图像", 1);

  cout << "按‘E’键退出显示...\n\n";

  while (show_colie) {
    IplImage* clone = cvCloneImage(show_colie);
    cvShowImage("原始图像", show_colie);
    cvRemap(clone, show_colie, mapx, mapy);
    cvReleaseImage(&clone);
    cvShowImage("非畸变图像", show_colie);

    if (cvWaitKey(10) == 'e') {
      break;
    }

    show_colie = cvQueryFrame(capture1);
  }

  return 0;
}

//
//     各标定步骤实现方法
//       1 计算标靶平面与图像平面之间的映射矩阵
//       计算标靶平面与图像平面之间的映射矩阵，计算映射矩阵时不考虑摄像机的成像模型，只是根据平面标靶坐标点和对应的图像坐标点的数据，利用最小二乘方法计算得到[ [ix] ]
//       .2 求解摄像机参数矩阵
//       由计算得到的标靶平面和图像平面的映射矩阵得到与摄像机内部参数相关的基本方程关系，求解方程得到摄像机内部参数，考虑镜头的畸变模型，将上述解方程获
//       得的内部参数作为初值，进行非线性优化搜索，从而计算出所有参数的准确值 [[x] ]
//       .3 求解左右两摄像机之间的相对位置关系
//       设双目视觉系统左右摄像机的外部参数分别为Rl, Tl,与Rr, Tr,，即Rl, Tl表示左摄像机与世界坐标系的相对位置，Rr, Tr表示右摄像机与世界坐标系的相对位置 [[xi] ]。因此，对于空间任意一点，如果在世界坐标系、左摄像机坐标系和右摄像机坐标系中的坐标分别为Xw,, Xl , Xr，则有:Xl=RlXw+Tl ;Xr=RrXw+Tr .因此，两台摄像机之间的相对几何关系可以由下式表示R=RrRl-1 ;T=Tr- RrRl-1Tl
//       在实际标定过程中，由标定靶对两台摄像机同时进行摄像标定，以分别获得两台摄像机的内、外参数，从而不仅可以标定出摄像机的内部参数，还可以同时标定出双目视觉系统的结构参数 [xii] 。由单摄像机标定过程可以知道，标定靶每变换一个位置就可以得到一组摄像机外参数:Rr,Tr,与Rl, Tl，因此，由公式R=RrRl-1 ;T=Tr- RrRl-1Tl，可以得到一组结构参数R和T
