
# OpenCV-Python基本功能 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[OpenCV-Python基本功能](https://www.cnblogs.com/eniac1946/p/8259346.html)
|Posted on|2018-01-10 16:33|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8259346)|[收藏](#)
|一、图像读取/保存
|1|import|cv2|2|3|img = cv2.imread(|"|name.png|"|)|4|5|cv2.imwrite(|'|save.jpg|'|, img)|6|7|\#|显示图像|8|cv2.imshow(|"|Origin|"|, img)|9|10|\#|交互控制|11|cv2.waitKey(0)|12|cv2.destroyAllWindows()
|imread的函数原型是：Mat imread( const string& filename, int flags=1 );
|第二个参数意义：
|IMREAD_UNCHANGED ：不进行转化，比如保存为了16位的图片，读取出来仍然为16位。
|IMREAD_GRAYSCALE ：进行转化为灰度图，比如保存为了16位的图片，读取出来为8位，类型为CV_8UC1。
|IMREAD_COLOR ：进行转化为三通道图像。
|IMREAD_ANYDEPTH ：如果图像深度为16位则读出为16位，32位则读出为32位，其余的转化为8位。
|IMREAD_ANYCOLOR ：
|IMREAD_LOAD_GDAL ：使用GDAL驱动读取文件，GDAL(Geospatial Data Abstraction Library)是一个在X/
|自己测了一下，然后总结如下：
|flag=-1时，8位深度，原通道
|flag=0，8位深度，1通道
|flag=1,   8位深度  ，3通道
|flag=2，原深度，1通道
|flag=3,  原深度，3通道
|flag=4，8位深度 ，3通道
|二、HSV格式：
|[https://zhuanlan.zhihu.com/p/24425116](https://zhuanlan.zhihu.com/p/24425116)
|基本图像处理
|色调、饱和度、明度
|缩放，裁剪和补边
|图像的仿射变换：缩放，旋转，剪切，翻转和平移
|视频功能：随机裁剪、随机旋转、随机颜色和明暗
|色调，明暗，直方图和Gamma曲线
|HSV分别是|色调（Hue）|，|饱和度（Saturation）|和|明度（Value）|。在HSV空间中进行调节就避免了直接在RGB空间中调节是还需要考虑三个通道的相关性。OpenCV中H的取值是[0, 180)，其他两个通道的取值都是[0, 256)，
|1|\#|通过cv2.cvtColor把图像从BGR转换到HSV|2|img_hsv =|cv2.cvtColor(img, cv2.COLOR_BGR2HSV)|3|4|\#|H空间中，绿色比黄色的值高一点，所以给每个像素+15，黄色的树叶就会变绿|5|turn_green_hsv =|img_hsv.copy()|6|turn_green_hsv[:, :, 0] = (turn_green_hsv[:, :, 0]+15) % 180|7|turn_green_img =|cv2.cvtColor(turn_green_hsv, cv2.COLOR_HSV2BGR)|8|cv2.imwrite(|'|turn_green.jpg|'|, turn_green_img)|9|10|\#|减小饱和度会让图像损失鲜艳，变得更灰|11|colorless_hsv =|img_hsv.copy()|12|colorless_hsv[:, :, 1] = 0.5 * colorless_hsv[:, :, 1|]|13|colorless_img =|cv2.cvtColor(colorless_hsv, cv2.COLOR_HSV2BGR)|14|cv2.imwrite(|'|colorless.jpg|'|, colorless_img)|15|16|\#|减小明度为原来一半|17|darker_hsv =|img_hsv.copy()|18|darker_hsv[:, :, 2] = 0.5 * darker_hsv[:, :, 2|]|19|darker_img =|cv2.cvtColor(darker_hsv, cv2.COLOR_HSV2BGR)|20|cv2.imwrite(|'|darker.jpg|'|, darker_img)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
