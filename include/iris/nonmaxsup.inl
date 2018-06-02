
int nonmaxsup(const img_t* inimage, const img_t* orient, double radius, img_t* im)
{
  int i, row, col, rows, cols, iradius;
  double xoff[180], yoff[180], hfrac[180], vfrac[180];
  //if (imsize(inimage) != imsize(orient)) {
  //  error("image and orientation image are of different sizes");
  //}

  if (radius < 1) {
    printf("radius must be >= 1");
    return 0;
  }

  rows = inimage->h, cols = inimage->w;
  imsetsize(im, rows, cols, sizeof(double), 1);       // Preallocate memory for output image for speed
  iradius = (int)ceil(radius);

  // Precalculate x and y offsets relative to centre pixel for each orientation angle
  // 预先计算与每个方向角度的中心像素相关的x和y补偿

  // angle = [0:180] * PI/180; // Array of angles in 1 degree increments (but in radians).
  //angle = (0:180) * PI/180;   // 幅度方向的角度构成数组，步长为一
  // 指定来自参考位置上的半径与角度点上的x,y的补偿
  for (i = 0; i < 180; ++i) {
    double angle = i * PI / 180;
    xoff[i] = radius * cos(angle); // x and y offset of points at specified radius and angle
    yoff[i] = radius * sin(angle); // from each reference position.

    // （与整数部分相关的小数部分补偿）
    hfrac[i] = xoff[i] - floor(xoff[i]); // Fractional offset of xoff relative to integer location
    vfrac[i] = yoff[i] - floor(yoff[i]); // Fractional offset of yoff relative to integer location
  }

  //orient = im fix(orient)+1;     // Orientations start at 0 degrees but arrays start
  // with index 1.（方向从0开始，而数组从索引1开始）

  // Now run through the image interpolating grey values on each side
  // of the centre pixel to be used for the non-maximal suppression.

  for (row = (iradius); row < (rows - iradius); ++row) {
    for (col = (iradius); col < (cols - iradius); ++col) {
      int im_step = im->w;
      double dor = orient->tt.f8[orient->w * row + col]; // Index into precomputed arrays（索引编入预先计算的矩阵）
#define FIX_1(x)  (int)((x>0) ? (x) : (x+0.9999999999))
      int fx, cx, fy, cy, or = FIX_1(dor);
#undef FIX_1
      double x, y, tl, tr, bl, br, upperavg, loweravg, v1, v2;
      // x，y在关键点的一边的定位
      x = col + xoff[ or ];   // x, y location on one side of the point in question
      y = row - yoff[ or ];

      // 得到围绕x，y的整数象素定位
      fx = (int)floor(x);          // Get integer pixel locations that surround location x,y
      cx = (int)ceil(x);
      fy = (int)floor(y);
      cy = (int)ceil(y);
      tl = inimage->tt.f8[inimage->w * fy + fx]; // Value at top left integer pixel location.
      tr = inimage->tt.f8[inimage->w * fy + cx]; // top right
      bl = inimage->tt.f8[inimage->w * cy + fx]; // bottom left
      br = inimage->tt.f8[inimage->w * cy + cx]; // bottom right

      // 使用双线性插值估计x，y处的值
      upperavg = tl + hfrac[ or ] * (tr - tl); // Now use bilinear interpolation to
      loweravg = bl + hfrac[ or ] * (br - bl); // estimate value at x,y
      v1 = upperavg + vfrac[ or ] * (loweravg - upperavg);

      if (inimage->tt.f8[inimage->w * row + col] > v1) { // We need to check the value on the other side...

        // 讨论另外一边的值的情况
        // x，y在关键点另外一边的位置
        x = col - xoff[ or ];   // x, y location on the `other side' of the point in question
        y = row + yoff[ or ];

        fx = (int)floor(x);
        cx = (int)ceil(x);
        fy = (int)floor(y);
        cy = (int)ceil(y);
        tl = inimage->tt.f8[inimage->w * fy + fx]; // Value at top left integer pixel location.
        tr = inimage->tt.f8[inimage->w * fy + cx]; // top right
        bl = inimage->tt.f8[inimage->w * cy + fx]; // bottom left
        br = inimage->tt.f8[inimage->w * cy + cx]; // bottom right
        upperavg = tl + hfrac[ or ] * (tr - tl);
        loweravg = bl + hfrac[ or ] * (br - bl);
        v2 = upperavg + vfrac[ or ] * (loweravg - upperavg);

        if (inimage->tt.f8[inimage->w * row + col] > v2) {        // This is a local maximum.局部最大值
          im->tt.f8[im_step * row + col] = inimage->tt.f8[inimage->w * row + col]; // Record value in the output image.输出图像中记录该值.
        }

      }
    }
  }

  return 0;
}
