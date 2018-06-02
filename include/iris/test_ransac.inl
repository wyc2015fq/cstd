

// RanSac.cpp : 定义控制台应用程序的入口点。
//

#define RANSAC_TIMES 1000
#define RANSAC_THRESHOLD 0.2

//记录黑像素的个数

typedef struct _PixelPos {
  int x;
  int y;
} PixelPos;

int solve_ellipse(const double conic_params[6], double ellipse_params[5])
{
  double  a = conic_params[0], //a*x*x + b*x*y + c*y*y + d*x + e*y + f =0;
          b = conic_params[1],
          c = conic_params[2],
          d = conic_params[3],
          e = conic_params[4],
          f = conic_params[5];
  //orientation of ellipse;
  double theta = atan2(b, a - c) / 2;
  //scaled major/minor axes of ellipse;
  double ct = cos(theta);
  double st = sin(theta);
  double ap = a * ct * ct + b * ct * st + c * st * st;
  double cp = a * st * st - b * ct * st + c * ct * ct;

  //translations
  double cx = (2 * c * d - b * e) / (b * b - 4 * a * c);
  double cy = (2 * a * e - b * d) / (b * b - 4 * a * c);

  //scale factor
  double val = a * cx * cx + b * cx * cy + c * cy * cy;
  double scale_inv = val - f;

  if (scale_inv / ap <= 0 || scale_inv / cp <= 0) { //not ellipse;
    return 0;
  }

  ellipse_params[0] = sqrt(scale_inv / ap);      //(x-cx)^2/A^2+(y-cy)^2/B^2=0;
  ellipse_params[1] = sqrt(scale_inv / cp);
  ellipse_params[2] = cx;
  ellipse_params[3] = cy;
  ellipse_params[4] = theta;
  return 1;
}

int test_ellipse(double c0, double c1, double c2, double c3, double c4)
{
  double x, angle, eig_value0, eig_value1, cos_value, sin_value, sin_cos_value;
  double left_matrix[4], constant, e0, e1, temp, ratio;

  if (c3 == c4) {
    angle = PI / 4.0;  // 3.0*PI/4.0
  }

  else {
    ratio = c2 / (c3 - c4);
    x = atan(ratio);

    //if(ratio > 0.0f)
    angle = 0.5f * atan(ratio);
    //else
    //  angle = PI + 0.5f * atan(ratio);
  }


  sin_value = sin(angle);
  cos_value = cos(angle);
  sin_cos_value = 2.0f * sin_value * cos_value;

  left_matrix[0] = cos_value;
  left_matrix[1] = - sin_value;
  left_matrix[2] = sin_value;
  left_matrix[3] = cos_value;

  e0 = c0 * left_matrix[0] + c1 * left_matrix[1];
  e1 = c0 * left_matrix[2] + c1 * left_matrix[3];

  if (c2 == 0.0) {
    eig_value0 = c3;
    eig_value1 = c4;
  }

  else {
    eig_value1 = c3 - c2 * cos_value * cos_value / sin_cos_value;
    eig_value0 = eig_value1 + c2 / sin_cos_value;
  }

  constant = e0 * e0 / (4.0 * eig_value0) + e1 * e1 / (4.0 * eig_value1) - 1.0;

  temp = eig_value0 * constant;

  if ((eig_value0 != 0.0f) && (eig_value1 != 0.0f) && (eig_value0 != eig_value1) && (temp > 0.0f)) {
    return 1;
  }
  else {
    return 0;
  }
}

void Ransac(img_t* pload, img_t* pnew, int* x_cord, int* y_cord, int length, double& c0, double& c1, double& c2, double& c3, double& c4, double& c5, char* file_name_prefix2, int nums, int& min, uchar* loadimageData)
{
  if (nums == 20) {
    nums  =  nums;
  }

  _PixelPos data[5];
  int count[RANSAC_TIMES]; //计数
  memset(count, 0x00, sizeof(int)*RANSAC_TIMES);
  CvMat  mat_x, mat_b, mat_c;
  double  p_mat_x[25], p_mat_b[5], p_mat_c[5]; //要转化成数组
  min = length; //求最小值用
  int flag_linear = 0;
  int position = 0; //记录误差最小的位置
  double ellipse[5];
  memset(ellipse, 0x00, sizeof(double) * 5);
  //uchar *loadimageData = (uchar*)pload->imageData;
  int step = pload->widthStep;
  double a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;

  for (int i = 0; i < RANSAC_TIMES; i++) {
    int n = 0;

    //c0x+c1y+c2xy+c3pow(x,2)+c4pow(y,2) = -1
    for (int t = 0; t < 5; t++) {
      n = rand() % length;
      data[t].x = x_cord[n];
      data[t].y = y_cord[n];
      p_mat_x[t * 5] = x_cord[n]; //x
      p_mat_x[t * 5 + 1] = y_cord[n]; //y
      p_mat_x[t * 5 + 2] = x_cord[n] * y_cord[n]; //xy
      p_mat_x[t * 5 + 3] = x_cord[n] * x_cord[n]; //x^2
      p_mat_x[t * 5 + 4] = y_cord[n] * y_cord[n]; //y^2
      p_mat_b[t] = -1;
      p_mat_c[t] = 0;//保存结果
    }

    cvInitMatHeader(&mat_x, 5, 5, CV_64FC1, p_mat_x);
    cvInitMatHeader(&mat_b, 5, 1, CV_64FC1, p_mat_b);
    cvInitMatHeader(&mat_c, 5, 1, CV_64FC1, p_mat_c);

    flag_linear = cvSolve(&mat_x, &mat_b, &mat_c, CV_LU);

    if (flag_linear == 0) {
      continue;
    }

    double* temp = mat_c.data.db;  //结果保存下来。
    c0 = temp[0];  //x
    c1 = temp[1];  //y
    c2 = temp[2];  //xy
    c3 = temp[3];  //x^2
    c4 = temp[4];  //y^2
    c5 = 1;        //c5判断是不是锥体用六个参数。

    //int flag2 = test_ellipse(c,b,a,d,e);
    //if (!flag2)
    //{
    //  //continue;
    //  flag = flag;
    //}

    double dis_error = 0;

    for (int j = 0; j < length; j++) {
      dis_error = fabs(c0 * x_cord[j] + c1 * y_cord[j] + c2 * x_cord[j] * y_cord[j] + c3 * x_cord[j] * x_cord[j] + c4 * y_cord[j] * y_cord[j] + 1);

      if (dis_error > RANSAC_THRESHOLD) {
        count[i]++;
      }
    }

    double conic_params[6];

    //conic_params[0]*x*x
    //conic_params[1]*x*y
    //conic_params[2]*y*y
    //conic_params[3]*x
    //conic_params[4]*y
    //conic_params[5];
    if (min > count[i]) {
      min = count[i];
      conic_params[0] = c3;
      conic_params[1] = c2;
      conic_params[2] = c4;
      conic_params[3] = c0;
      conic_params[4] = c1;
      conic_params[5] = 1;  //con0=c0,con1 =c1,con2=c2,con3=c3;con4=c4;con5 =1;
      int flag = solve_ellipse(conic_params, ellipse);

      if (!flag) {  //看这六个数是不是圆锥
        continue;
      }
    }

  }   //50次循环结束。

  //XFORM xform;
  double theta = ellipse[4];
  double centerX = ellipse[2];
  double centerY = ellipse[3];

  double left  = (centerX - ellipse[0]/*+0.5*/) ;
  double right = (centerX + ellipse[0]/*+0.5*/) ;
  double top   = (centerY - ellipse[1]/*+0.5*/) ;
  double bottom = (centerY + ellipse[1]/*+0.5*/) ;
  //  cvEllipse(ellipse_image,cvPoint(ellipse_x1,ellipse_y1),cvSize(ellipse_ra1,ellipse_rb1),0,0,360,CV_RGB(0,0xff,0),4,CV_AA);
  //cvEllipse(pnew,cvPoint(cvRound(centerX),cvRound(centerY)),cvSize(abs( (bottom-top)/2.0 ),abs( (left-right)/2.0) ),0,0,360,CV_RGB(255,0,0),4,CV_AA);
  //pDC->Ellipse(left,top, right, bottom);

  //double y_value = 0;
  int y_floor1 = 0, y_floor2 = 0;
  int pixelposition = 0;

  //
  //double dis_error1 = 0;
  //double dis_error2 = 0;
  double y1 = 0, y2 = 0;
  //double square_item = 0;
  //double b_negative = 0;
  double b_square = 0;
  double a_square = 0;

  double y_value = 0;
  double x_square = 0;
  int y_floor = 0;
  ////double ellipse_value1 = 0,ellipse_value2 = 0;


  for (int x = 0; x < pnew->width; x++) { //自变量取值范围
    for (int y = 0; y < pnew->height; y++) {
      pixelposition = y * step + x;

      if (0 == loadimageData[pixelposition]) {
        ((uchar*)(pnew->imageData + pnew->widthStep * y))[3 * x] = 255;
        ((uchar*)(pnew->imageData + pnew->widthStep * y))[3 * x + 1] = 0;
        ((uchar*)(pnew->imageData + pnew->widthStep * y))[3 * x + 2] = 0;
      }
    }

    b_square = ((left - right) / 2.0) * ((left - right) / 2.0);
    a_square = ((bottom - top) / 2.0) * ((bottom - top) / 2.0);
    x_square = (x - centerX) * (x - centerX);
    y1 = sqrt((b_square * (a_square - x_square)) / a_square) + centerY;
    y2 = -sqrt((b_square * (a_square - x_square)) / a_square) + centerY;
    //b_negative = -1.0 * ( c2*x_cord[x]+c1 );
    //b_square = b_negative * b_negative;
    //square_item = (  b_square - 4.0 *c4*( c3 * x_cord[x] * x_cord[x] + c0 * x_cord[x] + 1 ) );//相当于b平方-4ac
    //if (c4 == 0)  // c4y^2 +(c2x+c1)y+c3x^2+c0x+1 = 0;
    //{
    //  break; //不是曲线
    //}
    //else if ( square_item < 0 )
    //{
    //  break;//方程无解

    //}
    //else
    //{
    //  square_item = sqrt(square_item);
    //  y1 = ( b_negative + (square_item ) ) / (2.0*c4);
    //  y2 = ( b_negative - ( square_item) ) / (2.0*c4);
    //}
    /*ellipse_value1 = ( 1 + c0*x_cord[x] + c1*y1+ c2*x_cord[x]*y1 + c3*x_cord[x]*x_cord[x] + c4*y1*y1);
    ellipse_value2 = ( 1 + c0*x_cord[x] + c1*y2+ c2*x_cord[x]*y2 + c3*x_cord[x]*x_cord[x] + c4*y2*y2);*/
    y_floor1 = cvRound(y1);
    y_floor2 = cvRound(y2);

    if (y_floor1 >= 0 && y_floor1 < pnew->height) {
      ((uchar*)(pnew->imageData + pnew->widthStep * y_floor1))[3 * x] = 0;
      ((uchar*)(pnew->imageData + pnew->widthStep * y_floor1))[3 * x + 1] = 0;
      ((uchar*)(pnew->imageData + pnew->widthStep * y_floor1))[3 * x + 2] = 255;
    }

    if (y_floor2 >= 0 && y_floor2 < pnew->height) {
      ((uchar*)(pnew->imageData + pnew->widthStep * y_floor2))[3 * x] = 0;
      ((uchar*)(pnew->imageData + pnew->widthStep * y_floor2))[3 * x + 1] = 0;
      ((uchar*)(pnew->imageData + pnew->widthStep * y_floor2))[3 * x + 2] = 255;
    }

  }

  delete []count;
  cvSaveImage(file_name_prefix2, pnew);
}




int test_ransac()
{
  int nRetCode = 0;

  // 初始化 MFC 并在失败时显示错误
  if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
    // TODO: 更改错误代码以符合您的需要
    _tprintf(_T("错误: MFC 初始化失败\n"));
    nRetCode = 1;
  }
  else {
    FILE* fp = NULL;
    fp = fopen("d:\\filename.txt", "wb");
    FILE* fd = NULL;
    fd = fopen("d:\\minvalue.txt", "wb");

    if (!fp) {
      return 0;
    }

    CString strSearchName       = L"d:/edgelink_gray/*.bmp";
    CString aimstrSearchName = L"d:/cut/*.bmp";

    WIN32_FIND_DATA FindFileData;
    WIN32_FIND_DATA aimFindFileData;
    HANDLE hFind = FindFirstFile(strSearchName, &FindFileData);
    HANDLE ahFind = FindFirstFile(aimstrSearchName, &aimFindFileData);
    TCHAR* t_str;
    char* str;

    int nums = 0;

    if (hFind != INVALID_HANDLE_VALUE && ahFind != INVALID_HANDLE_VALUE) {
      do {
        USES_CONVERSION;
        t_str = FindFileData.cFileName;
        str = T2A(t_str);
        char file_name_prefix[128]  = "d:/edgelink_gray/";
        char file_name_prefix2[128] = "d:/cut/";
        strcat(file_name_prefix, str);
        strcat(file_name_prefix2, str);
        img_t* pload = cvLoadImage(file_name_prefix, 0);
        img_t* pnew  = cvLoadImage(file_name_prefix2, 1);

        if (!pload || !pnew) {
          return 0;
        }

        int width  = pload->width;
        int height = pload->height;
        uchar* loadimageData = (uchar*)pload->imageData;
        uchar* newimagedata = (uchar*)pnew->imageData;
        int step = pload->widthStep;
        int newstep = pnew->widthStep;
        int pixelposition = 0;
        double c0 = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0, c5 = 0;; //出口还没有决定哪一个是长半轴，短半轴
        int min = 0;
        int* x_cord = new int[width * height];
        int* y_cord = new int[width * height];
        memset(x_cord, 0x00, sizeof(int)*width * height);
        memset(y_cord, 0x00, sizeof(int)*width * height);
        int blackpixels = 0;

        for (int x = 0; x < width; x++) {
          for (int y = 0; y < height; y++) {
            pixelposition = y * step + x;

            if (0 == loadimageData[pixelposition]) {  //记录黑色像素的位置
              x_cord[blackpixels] = x;
              y_cord[blackpixels] = y;
              blackpixels ++;
            }
          }
        }

        int length = blackpixels; //入口
        Ransac(pload, pnew, x_cord, y_cord, length, c0, c1, c2, c3, c4, c5, file_name_prefix2, nums, min, loadimageData);
        cvReleaseImage(&pload);
        cvReleaseImage(&pnew);
        //cout<<"filenum ="<<nums<<":"<<c0<<","<<c1<<","<<c2<<endl;
        fprintf(fp, "%d:", nums);
        fputs(file_name_prefix2, fp);
        fprintf(fp, "\r\n");
        fprintf(fd, "%d", min);
        fprintf(fd, "\r\n");
        nums++;
        delete []x_cord;
        delete []y_cord;
      }
      while (FindNextFile(hFind, &FindFileData) && FindNextFile(ahFind, &aimFindFileData));
    }

    fclose(fp);
    fclose(fd);
  }

  return nRetCode;
}










