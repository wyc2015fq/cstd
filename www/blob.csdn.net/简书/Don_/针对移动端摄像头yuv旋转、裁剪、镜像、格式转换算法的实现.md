# 针对移动端摄像头yuv旋转、裁剪、镜像、格式转换算法的实现 - 简书


# 针对移动端摄像头yuv旋转、裁剪、镜像、格式转换算法的实现



## 存在问题

移动端录像在yuv数据上存在如下问题：
- 无论android还是ios都不能直接从摄像头取出颜色空间为i420的数据，所以在编码前需要进行格式转换。
- 而且由于所取图像得分辨率必须是摄像头所提供分辨率中得一组，所以有可能需要裁剪。
- 另外由于

(1)想让无论用户哪个方向拿手机所录的视频内容永远“头朝上”

( 2)摄像头默认返回图像为横屏图像（宽大于长）

所以需要旋转。- 前置摄像头需要镜像。

## 算法实现

1.**格式转换**

nv21转成i420。可以通过摄像头设置将所采集数据设置为YUVNV21格式。
`void NV21ToI420(uint8_t* dstyuv,uint8_t* data, int imageWidth, int imageHeight) { int Ustart =imageWidth*imageHeight; int i,j; int uWidth = imageWidth/2; int uHeight = imageWidth/2; //y memcpy(dstyuv,data,imageWidth*imageHeight); int tempindex = 0 ; int srcindex= 0; //u for(i= 0 ;i <uHeight;i++) { for(j = 0;j <uWidth ;j++ ) { dstyuv[Ustart+tempindex+j]= data[Ustart+(srcindex<<1)+1]; srcindex++; } tempindex+= uWidth; } //v for (i = 0; i < uHeight;i++) { for (j = 0; j < uWidth;j++) { dstyuv[Ustart+tempindex + j] = data[Ustart + (srcindex << 1 )]; srcindex++; } tempindex+= uWidth; } }`
其实就是改变了uv的位置。

I420: YYYYYYYY UU VV

NV21: YYYYYYYY VUVU
2.**裁剪**
`//crop yuv data int crop_yuv (char* data, char*dst, intwidth, intheight, int goalwidth, int goalheight) { int i, j; int h_div = 0, w_div = 0; w_div= (width - goalwidth) / 2; if (w_div % 2) w_div--; h_div= (height - goalheight) / 2; if (h_div % 2) h_div--; //u_div = (height-goalheight)/4; int src_y_length = width *height; int dst_y_length =goalwidth * goalheight; for (i = 0; i <goalheight; i++) for (j = 0; j <goalwidth; j++) { dst[i* goalwidth + j] = data[(i + h_div) * width + j + w_div]; } int index = dst_y_length; int src_begin =src_y_length + h_div * width / 4; int src_u_length =src_y_length / 4; int dst_u_length =dst_y_length / 4; for (i = 0; i <goalheight / 2; i++) for (j = 0; j <goalwidth / 2; j++) { int p = src_begin + i *(width >> 1) + (w_div >> 1) + j; dst[index]= data[p]; dst[dst_u_length+ index++] = data[p + src_u_length]; } return 0; }`

3.** 旋转**

分为四个方向 。以顺时针270度为例作图。

旋转前：




![](https://upload-images.jianshu.io/upload_images/1802307-dd16d236560660fb.png)





旋转后：





![](https://upload-images.jianshu.io/upload_images/1802307-604ab4afafcf5ec1.png)




u值的第i 行j列 对应原 数据的下标为：ustart+uw*j-i;

去除index的乘除法运算后算法：
**(1) i420 顺时针 270度**
`int rotateYUV420Degree270(uint8_t* dstyuv,uint8_t* srcdata, int imageWidth, int imageHeight) { int i = 0, j = 0; int index = 0; int tempindex = 0; int div = 0; for (i = 0; i <imageHeight; i++) { div= i +1; tempindex= 0; for (j = 0; j <imageWidth; j++) { tempindex+= imageWidth; dstyuv[index++]= srcdata[tempindex-div]; } } int start =imageWidth*imageHeight; int udiv = imageWidth *imageHeight / 4; int uWidth = imageWidth /2; int uHeight = imageHeight /2; index= start; for (i = 0; i < uHeight;i++) { div= i +1; tempindex= start; for (j = 0; j < uWidth;j++) { tempindex += uWidth; dstyuv[index]= srcdata[tempindex-div]; dstyuv[index+udiv]= srcdata[tempindex-div+udiv]; index++; } } return 0; }`
**(2)i420 顺时针旋转 180**
`int rotateYUV420Degree180(uint8_t* dstyuv,uint8_t* srcdata, int imageWidth, int imageHeight) { int i = 0, j = 0; int index = 0; int tempindex = 0; int ustart = imageWidth \*imageHeight; tempindex= ustart; for (i = 0; i <imageHeight; i++) { tempindex-= imageWidth; for (j = 0; j <imageWidth; j++) { dstyuv[index++] = srcdata[tempindex + j]; } } int udiv = imageWidth *imageHeight / 4; int uWidth = imageWidth /2; int uHeight = imageHeight /2; index= ustart; tempindex= ustart+udiv; for (i = 0; i < uHeight;i++) { tempindex-= uWidth; for (j = 0; j < uWidth;j++) { dstyuv[index]= srcdata[tempindex + j]; dstyuv[index+ udiv] = srcdata[tempindex + j + udiv]; index++; } } return 0; }`
**(3)顺时针 90度**
`int rotateYUV420Degree90(uint8_t* dstyuv,uint8_t* srcdata, int imageWidth, int imageHeight) { int i = 0, j = 0; int index = 0; int tempindex = 0; int div = 0; int ustart = imageWidth *imageHeight; for (i = 0; i <imageHeight; i++) { div= i; tempindex= ustart; for (j = 0; j <imageHeight; j++) { tempindex-= imageWidth; dstyuv[index++]= srcdata[tempindex + div]; } } int udiv = imageWidth *imageHeight / 4; int uWidth = imageWidth /2; int uHeight = imageHeight /2; index= ustart; for (i = 0; i < uHeight;i++) { div= i ; tempindex= ustart+udiv; for (j = 0; j < uWidth;j++) { tempindex-= uWidth; dstyuv[index]= srcdata[tempindex + div]; dstyuv[index+ udiv] = srcdata[tempindex + div + udiv]; index++; } } return 0; }`

4.** 镜像**
`//mirro 原址的 void Mirror(uint8_t\* yuv_temp, int nw, int nh, int w, int h) { int deleteW = (nw - h) / 2; int deleteH = (nh - w) / 2; int i, j; int a, b; uint8_ttemp; //mirror y for (i = 0; i < h; i++){ a= i \* w; b= (i + 1) \* w - 1; while (a < b) { temp= yuv_temp[a]; yuv_temp[a]= yuv_temp[b]; yuv_temp[b]= temp; a++; b--; } } //mirror u int uindex = w * h; for (i = 0; i < h / 2;i++) { a = i\ * w / 2; b= (i + 1) \* w / 2 - 1; while (a < b) { temp= yuv_temp[a + uindex]; yuv_temp[a+ uindex] = yuv_temp[b + uindex]; yuv_temp[b+ uindex] = temp; a++; b--; } } //mirror v uindex= w * h / 4 * 5; for (i = 0; i < h / 2;i++) { a= i\* w / 2; b= (i + 1) \* w / 2 - 1; while (a < b) { temp= yuv_temp[a + uindex]; yuv_temp[a+ uindex] = yuv_temp[b + uindex]; yuv_temp[b+ uindex] = temp; a++; b--; } } }`
## 算法优化

如果从摄像头取出数据，这样一步步的历遍，在低配手机上是满足不了需求的。其实这三个步骤中有很多中间步骤是可以省去的，比如：将a放到b 位置，再将b位置上的数据取出放到c位置，那么可以直接将a放到c位置。

所以将旋转、裁剪、格式转换三个问题所用的算法整合（未整合进去镜像）。结果如下：

**(1)处理不用旋转的图像，格式转换加裁剪
`void detailPic0(uint8_t\* d, uint8_t\* yuv_temp, int nw, int nh, int w, int h) { int deleteW = (nw - w) / 2; int deleteH = (nh - h) / 2; //处理y 旋转加裁剪 int i, j; int index = 0; for (j = deleteH; j < nh- deleteH; j++) { for (i = deleteW; i < nw- deleteW; i++) yuv_temp[index++]= d[j \* nw + i]; } //处理u index= w \* h; for (i = nh + deleteH / 2;i < nh / 2 \* 3 - deleteH / 2; i++) for (j = deleteW + 1; j< nw - deleteW; j += 2) yuv_temp[index++]= d[i \* nw + j]; //处理v 旋转裁剪加格式转换 for (i = nh + deleteH / 2;i < nh / 2 \* 3 - deleteH / 2; i++) for (j = deleteW; j < nw- deleteW; j += 2) yuv_temp[index++]= d[i \* nw + j]; }`
**(2)格式转换、裁剪加旋转90度
`void detailPic90(uint8_t\* d, uint8_t\* yuv_temp, int nw, int nh, int w, int h) { int deleteW = (nw - h) / 2; int deleteH = (nh - w) / 2; int i, j; for (i = 0; i < h; i++){ for (j = 0; j < w; j++){ yuv_temp[(h- i) \* w - 1 - j] = d[nw \* (deleteH + j) + nw - deleteW -i]; } } int index = w \* h; for (i = deleteW + 1; i< nw - deleteW; i += 2) for (j = nh / 2 \* 3 -deleteH / 2; j > nh + deleteH / 2; j--) yuv_temp[index++]= d[(j - 1) \* nw + i]; for (i = deleteW; i < nw- deleteW; i += 2) for (j = nh / 2 \* 3 -deleteH / 2; j > nh + deleteH / 2; j--) yuv_temp[index++]= d[(j - 1) \* nw + i]; }`

**(3)格式转换、裁剪加旋转180度**
`void detailPic180(uint8_t\* d, uint8_t\* yuv_temp, int nw, int nh, int w, int h) { int deleteW = (nw - w) / 2; int deleteH = (nh - h) / 2; //处理y 旋转加裁剪 int i, j; int index = w \* h; for (j = deleteH; j < nh- deleteH; j++) { for (i = deleteW; i < nw- deleteW; i++) yuv_temp[--index]= d[j \* nw + i]; } //处理u index= w \* h \* 5 / 4; for (i = nh + deleteH / 2;i < nh / 2 \* 3 - deleteH / 2; i++) for (j = deleteW + 1; j< nw - deleteW; j += 2) yuv_temp[--index]= d[i \* nw + j]; //处理v index= w \* h \* 3 / 2; for (i = nh + deleteH / 2;i < nh / 2 \* 3 - deleteH / 2; i++) for (j = deleteW; j < nw- deleteW; j += 2) yuv_temp[--index]= d[i \* nw + j]; }`

**(4)格式转换、裁剪加旋转270度**
`void detailPic270(uint8_t\* d, uint8_t\* yuv_temp, int nw, int nh, int w, int h) { int deleteW = (nw - h) / 2; int deleteH = (nh - w) / 2; int i, j; //处理y 旋转加裁剪 for (i = 0; i < h; i++){ for (j = 0; j < w; j++){ yuv_temp[i\* w + j] = d[nw \* (deleteH + j) + nw - deleteW - i]; } } //处理u 旋转裁剪加格式转换 int index = w \* h; for (i = nw - deleteW - 1;i > deleteW; i -= 2) for (j = nh + deleteH / 2;j < nh / 2 \* 3 - deleteH / 2; j++) yuv_temp[index++]= d[(j) \* nw + i]; //处理v 旋转裁剪加格式转换 for (i = nw - deleteW - 2;i >= deleteW; i -= 2) for (j = nh + deleteH / 2;j < nh / 2 \* 3 - deleteH / 2; j++) yuv_temp[index++]= d[(j) \* nw + i]; }`

**注：以上算法消除index的乘法后效果肯定会更好。**











