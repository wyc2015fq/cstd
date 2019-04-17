# 在YUV图像上根据背景色实现OSD反色 - DoubleLi - 博客园






> 
**所谓的OSD**其实就是在视频图像上叠加一些字符信息，比如时间，地点，通道号等， 
在图像上叠加OSD通常有两种方式： 一种是在前端嵌入式设备上，在图像数据上叠加OSD， 
这样客户端这边只需解码显示数据即可。另一种是PC客户端在接收到前端设备图像，解码之后，进行叠加。这两种都是比较常见的方式。

OSD具有字符型(Font-Based)和位图型(Bit-Map)两种类型。

**字符型OSD：**为了节约显示缓存，早期及低成本的解决方案中使用字符型OSD发生器，其原理是将OSD中显示内容按照特定的格式(12×18、12×16等)进行分割成块，例如数字0-9、字母a-z、常用的亮度、对比度符号等，并把这些内容固化在ROM或Flash中，在显示缓存中仅存放对应的索引号，这样的“字典”结构可以大幅度减少显示缓存的需求。

**位图OSD**：通过对最终显示内容上特定区域的每个像素点进行改变，直接将OSD信息叠加到最终的显示画面上，其按像素进行控制的方式可以保证具有多色及足够的表现能力。 
最近做一个网络播放器, 有在播放器实时叠加OSD这个需求,正好借这个机会研究了一下位


#### 最近做一个网络播放器, 有在播放器实时叠加OSD这个需求,正好借这个机会研究了一下。

### 先说下大体流程，
- 首先，播放SDK，通过网络模块接收前端视频流（经过压缩的数据），然后进行解压，得到一帧完整的YUV图像，
- 然后，我们在内存中创建一个设备无关的位图，并指定图像数据背景色为白色，字体为黑色。通过DrawTextW将字体画到内存DC上，
- 之后，通过GetDIBit将位图的二进制位复制到与设备无关的位图buffer里， 然后扫描此位图的每一个像素点，判断每个像素点的R,G,B三个分量之和 ，如果大于384 设置该像素为RGB(255，255，255)， 否则设置为RGB(0,0,0),（384表示灰度）
- 然后根据图像的宽高，创建一个通明通道数组，通过遍历之前得到的设备无关位图buffer，获取每个像素点的R分量，如果R等于0，则设置通明通道数组中对应的值为1， 表示该像素点上需要绘制字体（换句话说，该像素点不是透明色） 
这样我们就记住了临时图像上OSD文字每个像素的位置。
- 接下来，我们将构造出来的bmp位图数据进行转换，转换成YUV420数据，存储在 pOSDYuvBuffer中
- 
**下面这一步，就是最主要的地方， 即计算OSD反色的算法，**

我们遍历透明通道数组， 若值等于1， 则说明该像素点是字体，需要绘制， 那么，我们就在源图像（解码后的YUV图像）上找到位置想对应的点。并以该点为中心，计算出一个13*13的矩形区域，此区域作为背景参考区， 遍历该矩形区域 并计算该区域的 Y分量平均值，如果平均值大于128 说明该背景区是亮色，那么，我们设置pOSDYuvBuffer相应像素点的Y分量为1（背景亮，则osd字体为黑色，反之，若背景区为暗色，则设置osd字体像素点的Y为255）

- 
这样扫描结束之后， 就实现了 pOSDYuvBuffer中的OSD字体颜色，根据背景色的反色。然后将我们构造出来的临时图像 叠加到源图像上即可。

- 至于叠加操作，其实很简单。 同样扫描通明通道数据，如果发现不是透明色，直接将pOSDYuvBuffer中的YUV复制到 源图像相应位置即可。

### 下面是流程：

![这里写图片描述](http://img.blog.csdn.net/20161017183848825)

### 反色计算算法 图

![这里写图片描述](http://img.blog.csdn.net/20161017183950576)

```
int posAx=0, posAy=0;
int posDx=0 ,posDy=0;
int nBKColor = 0;
for(i = 0; i < m_OSDHeigth; i++)
{
    for(j = 0; j < m_OSDWidth; j++)
    {
        if(  j+_dwStrPosX>=_VideoWidth || i+_dwStrPosY>=_VideoHeight )
            continue;
    // 找到字符 X
        if( m_palpha[i*m_OSDWidth + j] )// 找到x映射在在源图像上的13*13 背景块
        {
            posAx = _dwStrPosX+j - 3;   // 计算背景色块 A的坐标
            if ( posAx<0 )
                posAx=0;

            posAy = _dwStrPosY+i -3;
            if ( posAy<0 )
                posAy =0;


            posDx =  _dwStrPosX+j + 3;      // 计算背景色块 点D的坐标
            if ( posDx >_VideoWidth)
                posDx =_VideoWidth;

            posDy = _dwStrPosY+i +3;
            if ( posDy >_VideoHeight)
                posDy =_VideoHeight;

            // / 计算背景色块 长,宽
            int height_ = posDy-posAy;
            int width_  = posDx-posAx;

            UINT16* pTempDesY = (UINT16*)dest_buf + posAx + posAy*_VideoWidth;
                int nTemp = 0;
                for ( m=0; m< height_; m++ )//height
                {
                    for ( n=0; n<width_;n++ )
                    {
                        nTemp += ( (*pTempDesY) >>2 );
                        pTempDesY++;
                    }
                    pTempDesY = (UINT16*)dest_buf + posAx + posAy*_VideoWidth + m*_VideoWidth;
                }

                // 计算Y 的平均值
                nBKColor = nTemp/(height_*width_);
                if ( nBKColor >128 )
                    *pY = 0;
                else 
                    *pY = ( 255<<2);
            }

            pDesY++;
                pY++;
        }//j
        pDesY = ((UINT16*)dest_buf + _dwStrPosX + _dwStrPosY*_VideoWidth) + i*_VideoWidth;
}//i
```

### 效果图：

![这里写图片描述](http://img.blog.csdn.net/20161017191320856)

#### 扫描下方二维码。关注 【音视频开发训练营】

![这里写图片描述](http://img.blog.csdn.net/20161017191542282)

from:http://blog.csdn.net/machh/article/details/52840886









