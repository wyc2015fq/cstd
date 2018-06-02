/******************************************
void wbGW(float** rgbImg, float** chImg, int rows, int cols)
功能：
  将rbg图按灰度世界理论算法(GW)做白平衡调整
参数：
  float** rgbImg - 图像的rgb数据，[rows][cols*3]
  float** chImg - 输出数据，白平衡处理后的图像数据，[rows][cols*3]
  int rows - 图像的行数
  int cols - 图像的列数

时间：
  2011.9.20
*******************************************/
int wbGW(int rows, int cols, ucahr* rgbImg, int rgbstep, ucahr* chImg, int chstep)
{
  int i = 0;
  int j = 0;

  //1）求R、G、B三色分量的平均值
  float aveR = 0.0f;        //记录R分量的平均值
  float aveG = 0.0f;        //记录G分量的平均值
  float aveB = 0.0f;        //记录B分量的平均值
  float k = 0.0f;          //记录aveR、aveG、aveB的平均值

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      aveR = aveR + rgbImg[i][3 * j];
      aveG = aveG + rgbImg[i][3 * j + 1];
      aveB = aveB + rgbImg[i][3 * j + 2];
    }
  }

  aveR = aveR / (rows * cols);
  aveG = aveG / (rows * cols);
  aveB = aveB / (rows * cols);
  k = (aveR + aveG + aveB) / 3;

  //3）求白平衡调整后的图像数据
  for (i = 0; i < rows; i++) {
    uchar* chLine = chImg + i * chstep;

    for (j = 0; j < cols; j++) {
      int r, g, b;
      r =  rgbImg[i][3 * j]  * k / aveR;  //调整后的R分量
      g =  rgbImg[i][3 * j + 1] * k / aveG; //调整后的R分量
      b =  rgbImg[i][3 * j + 2] * k / aveB; //调整后的R分量

      if (r > 255) {  //超过255，按255算
        r = 255;
      }

      if (g > 255) {
        g = 255;
      }

      if (b > 255) {
        b = 255;
      }

      chLine[3 * j] = r;
      chLine[3 * j + 1] = g;
      chLine[3 * j + 2] = b;
    }
  }

  return 0;
}
