// 任意半径中值滤波（扩展至百分比滤波器）O(1)时间复杂度算法
// 图像镜像扩展，上下左右各扩r各像素
// b指向
int imext(int h, int w, const unsigned char* a, int al, unsigned char* b, int bl, int cn, int r)
{
  int i = 0, j = 0, i1 = 0, j1 = 0;

  for (i = -r; i < h + r;) {
    //i1=i<0?-i: i>=h?(h+h-i);
    //memcpy();
  }

  return 0;
}

// 黄氏算法
// Input: Image X of size m*n, kernel radius r
// Output: Image Y OF the same size as X
//   Initialize kernel histogram H
//   for i=1 to m do
//     for j=1 to n do
//       for k=-r to r do
//         Remove X[i+k,j-r-1] from H
//         Add x[i+k,j+r] to H
//       end for
//       Y[i,j]<-median(H)
//     end for
//   end for
int MedianFiltering_Huang(int h, int w, const unsigned char* a, int al, unsigned char* b, int bl, int r)
{
  int i, j, k, i1, j1;
  int H[256] = {0};
  int H_line[256] = {0};

  for (i = 0; i < r; ++i) {
    for (j = 0; j < r; ++j) {
      H_line[a[i * al + j]]++;
    }
  }

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      for (k = -r; k <= r; ++k) {
        i1 = i + k;

        if (i1 >= 0 && i1 < h) {
          j1 = j - r - 1;

          if (j1 >= 0 && j1 < w) {
            H_line[a[i1 * al + j1]]--;
          }

          j1 = j + r;

          if (j1 >= 0 && j1 < w) {
            H_line[a[i1 * al + j1]]++;
          }
        }
      }
    }
  }

  return 0;
}
// 针对8位灰度图像
// （1）、对核最右侧的列直方图执行一次加法。
// （2）、对同一列直方图执行一次减法，去除多余的像素信息。
// （3）、将更新后的列直方图数据加到核直方图中，这进行了256次加法。
// （4）、将无效的列直方图数据从核直方图中减去，这需要256次减法。
// （5）、为找到核直方图的中值，平均需要128次比较和127次加法。

// Input: Image X of size m*n, kernel radius r
// Output: Image Y OF the same size as X
//   Initialize kernel histogram H and column histograms h[1...n]
//   for i=1 to m do
//     for j=1 to n do
//       Remove X[i-r-1,j+r] from h[j+r]
//       Add x[i+r,j+r] to h[j+r]
//       H<-H+h[j+r]-h[j-r-1]
//       H<-H+h[j+r]-h[j-r-1]
//       Y[i,j]<-median(H)
//     end for
//   end for

