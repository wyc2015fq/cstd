
//局部二值化算法Niblack OpenCV实现

//Niblack 算法的应用于文本图像二值化领域较多，算是比较经典的局部二值化处理方法，其局部二值化方法的提出也很有借鉴意义，包括后来的一些对其改进方法，Sauvola 算法、Nick 算法，
//核心思想是：根据图像像素点的邻域内的平均灰度和标准偏差来构造一个阈值曲面进行二值化处理。其阈值计算如下：

//point 1：计算窗口内像素均值；
//point 2：计算窗口内像素标准差。
//这两个点的计算方法因人而异。

void niBlackThreshold(InputArray _src, OutputArray _dst, double maxValue,
  int type, int blockSize, double delta)
{
  // Input grayscale image
  Mat src = _src.getMat();
  CV_Assert(src.channels() == 1);
  CV_Assert(blockSize % 2 == 1 && blockSize > 1);
  type &= THRESH_MASK;

  // Compute local threshold (T = mean + k * stddev)
  // using mean and standard deviation in the neighborhood of each pixel
  // (intermediate calculations are done with floating-point precision)
  Mat thresh;
  {
    // note that: Var[X] = E[X^2] - E[X]^2
    Mat mean, sqmean, stddev;
    boxFilter(src, mean, CV_32F, Size(blockSize, blockSize),
      Point(-1, -1), true, BORDER_REPLICATE);
    sqrBoxFilter(src, sqmean, CV_32F, Size(blockSize, blockSize),
      Point(-1, -1), true, BORDER_REPLICATE);
    sqrt(sqmean - mean.mul(mean), stddev);
    thresh = mean + stddev * static_cast<float>(delta);
    thresh.convertTo(thresh, src.depth());
  }

  // Prepare output image
  _dst.create(src.size(), src.type());
  Mat dst = _dst.getMat();
  CV_Assert(src.data != dst.data);  // no inplace processing

  // Apply thresholding: ( pixel > threshold ) ? foreground : background
  Mat mask;
  switch (type)
  {
  case THRESH_BINARY:      // dst = (src > thresh) ? maxval : 0
  case THRESH_BINARY_INV:  // dst = (src > thresh) ? 0 : maxval
    compare(src, thresh, mask, (type == THRESH_BINARY ? CMP_GT : CMP_LE));
    dst.setTo(0);
    dst.setTo(maxValue, mask);
    break;
  case THRESH_TRUNC:       // dst = (src > thresh) ? thresh : src
    compare(src, thresh, mask, CMP_GT);
    src.copyTo(dst);
    thresh.copyTo(dst, mask);
    break;
  case THRESH_TOZERO:      // dst = (src > thresh) ? src : 0
  case THRESH_TOZERO_INV:  // dst = (src > thresh) ? 0 : src
    compare(src, thresh, mask, (type == THRESH_TOZERO ? CMP_GT : CMP_LE));
    dst.setTo(0);
    src.copyTo(dst, mask);
    break;
  default:
    CV_Error(CV_StsBadArg, "Unknown threshold type");
    break;
  }
}

