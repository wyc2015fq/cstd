# MFC 缩放和显示IplImage - wishchinYang的专栏 - CSDN博客
2013年07月17日 11:20:45[wishchin](https://me.csdn.net/wishchin)阅读数：1260
序言：使用OpenCV嵌入MFC的框内，图像大小不能和框大小进行匹配，因此需要缩放，使图像适用于MFC框。
后来找到了一种新的方法，此方案貌似u已经废弃。
（1）.在MFC中显示图片
```cpp
void CAviTestDlg::ShowImage( IplImage* img, UINT ID )    // ID 是Picture Control控件的ID号
{
    CDC* pDC = GetDlgItem( ID ) ->GetDC();        // 获得显示控件的 DC wishchin
    HDC hDC = pDC ->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作
    CRect rect;
    GetDlgItem(ID) ->GetClientRect( &rect );
    int rw = rect.right - rect.left;            // 求出图片控件的宽和高
    int rh = rect.bottom - rect.top;
    int iw = img->width;                        // 读取图片的宽和高
    int ih = img->height;
    int tx = (int)(rw - iw)/2;                    // 使图片的显示位置正好在控件的正中
    int ty = (int)(rh - ih)/2;
    SetRect( rect, tx, ty, tx+iw, ty+ih );
    CvvImage cimg;
    cimg.CopyOf( img );                            // 复制图片
    cimg.DrawToHDC( hDC, &rect );                // 将图片绘制到显示控件的指定区域内  参数不对？
    ReleaseDC( pDC );//cimg.DrawToHDC( hDC, &rect );
}
```
  对于cvvImage取消后，怎么显示的问题，还在考虑之中！！！
后记：还是直接把HighGUI嵌入到MFC里面就可以了！或者直接Copy矩阵数据。
（2）.在MFC中缩放图片
TheImage作为全局变量，首先作为适应框进行初始化！
```cpp
void CAviTestDlg::ResizeImage(IplImage* img)
{
    // 读取图片的宽和高
    int w = img->width;
    int h = img->height;
    // 找出宽和高中的较大值者
    int max = (w > h)? w: h;
    // 计算将图片缩放到TheImage区域所需的比例因子
    float scale = (float) ( (float) max / 320.0f );
    // 缩放后图片的宽和高
    int nw = (int)( w/scale );
    int nh = (int)( h/scale );
    // 为了将缩放后的图片存入 TheImage 的正中部位，需计算图片在 TheImage 左上角的期望坐标值
    int tlx = (nw > nh)? 0: (int)(320-nw)/2;                //修改参数因子 wishchin
    int tly = (nw > nh)? (int)(240-nh)/2: 0;
    // 设置 TheImage 的 ROI 区域，用来存入图片 img
    cvSetImageROI( TheImage, cvRect( tlx, tly, nw, nh) );
    // 对图片 img 进行缩放，并存入到 TheImage 中
    cvResize( img, TheImage );
    // 重置 TheImage 的 ROI 准备读入下一幅图片
    cvResetImageROI( TheImage );
}
```
