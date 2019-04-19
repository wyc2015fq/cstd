# 【我的解决方案】android2.3 播放网络视频时，截取任意时间点图像且保存下来 - 三少GG - CSDN博客
2011年12月11日 21:53:35[三少GG](https://me.csdn.net/scut1135)阅读数：3146
** MediaMetadataRetriever**
/**
* MediaMetadataRetriever class provides a unified interface for retrieving
* frame and meta data from an input media file. MediaMetadataRetriever
* 提供了一个统一的检索界面 ,从输入媒体文件的帧和元数据
该类大量方法采用JNI实现可以暂时跳过本类 ，该类最重要的最后声明的那些静态标记 {@hide}
*/
public class MediaMetadataRetriever {
static {
System.loadLibrary("media_jni");
native_init();
}
**可用代码为：**

+++++++++++++++++++++++++++++++++++
参考：
# [Android2.2缩略图类ThumbnailUtils](http://www.cnblogs.com/trust/archive/2011/01/20/1939840.html)
提供了四个对外的静态接口 
1. Bitmap android.media.ThumbnailUtils.createImageThumbnail(String filePath, int kind) 
This method first examines if the thumbnail embedded in EXIF is bigger than our target size. If not, then it'll create a thumbnail from original image. Due to efficiency consideration, we want to let MediaThumbRequest avoid calling this method twice for both
 kinds, so it only requests for MICRO_KIND and set saveImage to true. This method always returns a "square thumbnail" for MICRO_KIND thumbnail. 
Parameters: 
    filePath the path of image file 
    kind could be MINI_KIND or MICRO_KIND 
Returns: 
    Bitmap 
@hide 
    This method is only used by media framework and media provider internally. 
2. Bitmap android.media.ThumbnailUtils.createVideoThumbnail(String filePath, int kind) 
返回视频缩略图，如果视频损坏或格式不被支持那么都将返回为空 
filePath：视频文件的位置 
kind：Images.Thumbnails.MICRO_KIND或者Images.Thumbnails.MINI_KIND最终和分辨率有关 
3. Bitmap android.media.ThumbnailUtils.extractThumbnail(Bitmap source, int width, int height) 
创建一个指定大小居中的缩略图 
4. Bitmap android.media.ThumbnailUtils.extractThumbnail(Bitmap source, int width, int height, int options) 
创建一个指定大小居中的缩略图，如果options定义为OPTIONS_RECYCLE_INPUT，则回收资源 
ThumbnailUtils类是API Level从8或更高才开始支持的，不向下兼容
[**怎样为多媒体文件生成缩略图**
-](http://dev.10086.cn/cmdn/wiki/index.php?edition-view-6258-1.html)
