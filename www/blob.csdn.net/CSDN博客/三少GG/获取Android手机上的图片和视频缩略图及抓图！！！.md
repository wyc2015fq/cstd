# 获取Android手机上的图片和视频缩略图及抓图！！！ - 三少GG - CSDN博客
2011年12月09日 18:33:54[三少GG](https://me.csdn.net/scut1135)阅读数：3474
a. BufferedInputStream ?       byte[] buffer = new byte[1024];
b.
# [Android 带缓冲的播放器](http://www.eoeandroid.com.sixxs.org/thread-79148-1-1.html)
[http://www.eoeandroid.com.sixxs.org/thread-79148-1-1.html](http://www.eoeandroid.com.sixxs.org/thread-79148-1-1.html)
[http://blog.csdn.net/chenjie19891104/article/details/6338202](http://blog.csdn.net/chenjie19891104/article/details/6338202)
[http://blog.csdn.net/chenjie19891104/article/details/6331442](http://blog.csdn.net/chenjie19891104/article/details/6331442)
[http://blog.csdn.net/kickxxx/article/details/6977891](http://blog.csdn.net/kickxxx/article/details/6977891)
[http://stackoverflow.com.sixxs.org/questions/5343730/mediaplayer-stutters-at-start-of-mp3-playback/5432091#5432091](http://stackoverflow.com.sixxs.org/questions/5343730/mediaplayer-stutters-at-start-of-mp3-playback/5432091#5432091)
[http://blog.csdn.net/faithsws/article/details/6668696](http://blog.csdn.net/faithsws/article/details/6668696)
[http://edu.3gmgc.com/bbs/forum.php?mod=viewthread&tid=13308&highlight=mediaplayer](http://edu.3gmgc.com/bbs/forum.php?mod=viewthread&tid=13308&highlight=mediaplayer)
[http://www.eoeandroid.com.sixxs.org/thread-66935-1-1.html](http://www.eoeandroid.com.sixxs.org/thread-66935-1-1.html)
![](http://hi.csdn.net/attachment/201112/10/0_1323505576gVe1.gif)
写在前面的话：
研究ThumbnailUtilscreatVideoThumbnail方法发现，其实取任意时间点截图的功能已经实现，只要延伸一个接口出来就ok了。
可以重载ThumbnailUtils里面的creatVideoThumbnail方法，添加一个参数表示需要截取的视频图片的时间点，基本上是跟踪原有的creatVideoThumbnail方法就可以修改了。
如何重载？
MediaMetadataRetriever retriever = new MediaMetadataRetriever();
        try {
            retriever.setMode(MediaMetadataRetriever.MODE_CAPTURE_FRAME_ONLY);
            retriever.setDataSource(filePath);
            bitmap = retriever.captureFrame();
          }
MediaMetadataRetriever是一个未公开的接口      现在急用，还请大虾明示...:Q
需要到Framework里面去修改扩展，你只需要顺着ThumbnailUtils的creatVideoThumbnail方法反向跟踪一下，就能找到设置时间点的地方。源代码是根据一个算法来计算这个时间点，我想他原本的目的是不想截图截下来的是一个“黑板”或类似功能。
能讲清楚点，计算时间点的算法在哪里，给个文件路径
/trunk/frameworks/base/include/media/IMediaMetadataRetriever.h
/trunk/frameworks/base/include/media/MediaMetadataRetrieverInterface.h
/trunk/frameworks/base/include/media/mediametadataretriever.h
/trunk/frameworks/base/media/java/android/media/MediaMetadataRetriever.java
/trunk/frameworks/base/media/java/android/media/ThumbnailUtils.java
/trunk/frameworks/base/media/jni/android_media_MediaMetadataRetriever.cpp
/trunk/frameworks/base/media/libmedia/IMediaMetadataRetriever.cpp
/trunk/frameworks/base/media/libmedia/mediametadataretriever.cpp
/trunk/frameworks/base/media/libmediaplayerservice/MetadataRetrieverClient.cpp
/trunk/frameworks/base/media/libmediaplayerservice/MetadataRetrieverClient.h
/trunk/frameworks/base/media/libstagefright/StagefrightMetadataRetriever.cpp
/trunk/frameworks/base/media/libstagefright/include/StagefrightMetadataRetriever.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
大家都知道Android从1.5开始刚插入SD卡时系统会调用MediaScanner服务进行后台扫描，索引新的歌曲、图片和视频等信息，如果我们需要快速提取图片和视频缩略图可以直接访问 android.provider.MediaStore.Images.Thumbnails 和android.provider.MediaStore.Video.Thumbnails这两个[数据库](http://www.2cto.com/database/)，即可查询出来缩略图
  如何判断文件呢? 可以通过Cursor遍历数据库，对比INTERNAL_CONTENT_URI字段的值，这是一个Uri我们可以转成String，这里保存着Android手机SD卡上的多媒体文件完整路径，
  有关具体的缩略图可以通过getThumbnail(ContentResolver cr, long origId, int kind, BitmapFactory.Options options) 或getThumbnail(ContentResolver cr, long origId, long groupId, int kind, BitmapFactory.Options options) 方法获取，这两种方法返回Bitmap类型，而缩略图的分辨率可以从HEIGHT和WIDTH两个字段提取，在Android上缩略图分为两种，通过读取KIND字段来获得，分别为MICRO_KIND和MINI_KIND
 分别为微型和迷你两种缩略模式，前者的分辨率更低。这样我们平时获取文件系统的某个图片预览时，可以直接调用系统缩略图，而不用自己重新计算。
  最后Android123提示大家，**缩略图保存在SD卡的DCIM目录，里面的.thumbnails是图片的，而.video_thumbnails是视频的，这两个文件夹为隐藏属性，一般的文件管理器都可以看到。adb shell      ls -l**
从Android2.2开始系统新增了一个缩略图ThumbnailUtils类，位于framework的android.media.ThumbnailUtils位置，可以帮助我们从mediaprovider中获取系统中的视频或图片文件的缩略图，该类提供了三种静态方法可以直接调用获取。
1. static Bitmap createVideoThumbnail(String filePath, int kind)   //获取视频文件的缩略图，第一个参数为视频文件的位置，比如/sdcard/android123.3gp，而第二个参数可以为MINI_KIND或MICRO_KIND最终和分辨率有关
2. static Bitmap extractThumbnail(Bitmap source, int width, int height, int options) //直接对Bitmap进行缩略操作，最后一个参数定义为OPTIONS_RECYCLE_INPUT，来回收资源
3. static Bitmap extractThumbnail(Bitmap source, int width, int height) // 这个和上面的方法一样，无options选项
最后Android开发网再次提醒大家，ThumbnailUtils类是API Level从8或更高才开始支持的。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://www.linuxidc.com/Linux/2011-08/40109p2.htm](http://www.linuxidc.com/Linux/2011-08/40109p2.htm)
- /**
-      * Create a video thumbnail for a video. May return null if the video is
-      * corrupt or the format is not supported.
-      *
-      * @param filePath the path of video file
-      * @param kind could be MINI_KIND or MICRO_KIND
-      */
- **public****static** Bitmap createVideoThumbnail(String filePath, **int** kind) {  
-         Bitmap bitmap = **null**;  
-         MediaMetadataRetriever retriever = **new** MediaMetadataRetriever();  
- **try** {  
-             retriever.setMode(MediaMetadataRetriever.MODE_CAPTURE_FRAME_ONLY);  
-             retriever.setDataSource(filePath);  
-             bitmap = retriever.captureFrame();  
-         } **catch** (IllegalArgumentException ex) {  
- // Assume this is a corrupt video file
-         } **catch** (RuntimeException ex) {  
- // Assume this is a corrupt video file.
-         } **finally** {  
- **try** {  
-                 retriever.release();  
-             } **catch** (RuntimeException ex) {  
- // Ignore failures while cleaning up.
-             }  
-         }  
- **if** (kind == Images.Thumbnails.MICRO_KIND && bitmap != **null**) {  
-             bitmap = extractThumbnail(bitmap,  
-                     TARGET_SIZE_MICRO_THUMBNAIL,  
-                     TARGET_SIZE_MICRO_THUMBNAIL,  
-                     OPTIONS_RECYCLE_INPUT);  
-         }  
- **return** bitmap;  
-     }  
++++++++++++++++++++++++++++++++++
### [为多媒体文件生成缩略图](http://jacky-zhang.iteye.com/blog/836325)
1、Video 
对于视频，取第一帧作为缩略图，也就是怎样从filePath得到一个Bitmap对象。 
Java代码  ![收藏代码](http://jacky-zhang.iteye.com/images/icon_star.png)
- private Bitmap createVideoThumbnail(String filePath) {   
-         Bitmap bitmap = null;   
-         MediaMetadataRetriever retriever = new MediaMetadataRetriever();   
- try {   
-             retriever.setMode(MediaMetadataRetriever.MODE_CAPTURE_FRAME_ONLY);   
-             retriever.setDataSource(filePath);   
-             bitmap = retriever.captureFrame();   
-         } catch(IllegalArgumentException ex) {   
- // Assume this is a corrupt video file 
-         } catch (RuntimeException ex) {   
- // Assume this is a corrupt video file. 
-         } finally {   
- try {   
-                 retriever.release();   
-             } catch (RuntimeException ex) {   
- // Ignore failures while cleaning up. 
-             }   
-         }   
- return bitmap;   
-     }   
Android提供了MediaMetadataRetriever，由JNI(media_jni)实现。 
看得出MediaMetadataRetriever主要有两个功能：MODE_GET_METADATA_ONLY和MODE_CAPTURE_FRAME_ONLY 
这里设mode为MODE_CAPTURE_FRAME_ONLY，调用captureFrame取得一帧。 
另外还有两个方法可以用： 
extractMetadata 提取文件信息，ARTIST、DATE、YEAR、DURATION、RATING、FRAME_RATE、VIDEO_FORMAT 
和extractAlbumArt 提取专辑信息，这个下面的音乐文件可以用到。 
