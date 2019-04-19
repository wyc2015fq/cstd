# Android 用MediaCodec实现视频硬解码 - u013366022的专栏 - CSDN博客
2014年05月14日 14:49:35[slitaz](https://me.csdn.net/u013366022)阅读数：699
转自：  http://blog.csdn.net/halleyzhang3/article/details/11473961
本文向你讲述如何用android标准的API （MediaCodec）实现视频的硬件编解码。例程将从摄像头采集视频开始，然后进行H264编码，再解码，然后显示。我将尽量讲得简短而清晰，不展示那些不相关的代码。但是，我不建议你读这篇文章，也不建议你开发这类应用，而应该转而开发一些戳鱼、打鸟、其乐融融的程序。好吧，下面的内容是写给那些执迷不悟的人的，看完之后也许你会同意我的说法：Android只是一个玩具，很难指望它来做靠谱的应用。
1、从摄像头采集视频
      可以通过摄像头Preview的回调，来获取视频数据。
      首先创建摄像头，并设置参数：
**[java]**[view
 plain](http://blog.csdn.net/halleyzhang3/article/details/11473961#)[copy](http://blog.csdn.net/halleyzhang3/article/details/11473961#)
-                      cam = Camera.open();  
- cam.setPreviewDisplay(holder);                    
- Camera.Parameters parameters = cam.getParameters();  
- parameters.setFlashMode("off"); // 无闪光灯
- parameters.setWhiteBalance(Camera.Parameters.WHITE_BALANCE_AUTO);  
- parameters.setSceneMode(Camera.Parameters.SCENE_MODE_AUTO);  
- parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);   
- parameters.setPreviewFormat(ImageFormat.YV12);       
- parameters.setPictureSize(camWidth, camHeight);  
- parameters.setPreviewSize(camWidth, camHeight);  
- //这两个属性 如果这两个属性设置的和真实手机的不一样时，就会报错
- cam.setParameters(parameters);            
宽度和高度必须是摄像头支持的尺寸，否则会报错。要获得所有支持的尺寸，可用getSupportedPreviewSizes，这里不再累述。据说所有的参数必须设全，漏掉一个就可能报错，不过只是据说，我只设了几个属性也没出错。    然后就开始Preview了：
**[java]**[view
 plain](http://blog.csdn.net/halleyzhang3/article/details/11473961#)[copy](http://blog.csdn.net/halleyzhang3/article/details/11473961#)
- buf = newbyte[camWidth * camHeight * 3 / 2];  
- cam.addCallbackBuffer(buf);  
- cam.setPreviewCallbackWithBuffer(this);           
- cam.startPreview();   
  setPreviewCallbackWithBuffer是很有必要的，不然每次回调系统都重新分配缓冲区，效率会很低。
    在onPreviewFrame中就可以获得原始的图片了（当然，this 肯定要 implements PreviewCallback了）。这里我们是把它传给编码器：
**[java]**[view
 plain](http://blog.csdn.net/halleyzhang3/article/details/11473961#)[copy](http://blog.csdn.net/halleyzhang3/article/details/11473961#)
- publicvoid onPreviewFrame(byte[] data, Camera camera) {  
- if (frameListener != null) {  
-         frameListener.onFrame(data, 0, data.length, 0);  
-     }  
-     cam.addCallbackBuffer(buf);  
- }  
2、编码
    首先要初始化编码器：
**[java]**[view
 plain](http://blog.csdn.net/halleyzhang3/article/details/11473961#)[copy](http://blog.csdn.net/halleyzhang3/article/details/11473961#)
-       mediaCodec = MediaCodec.createEncoderByType("Video/AVC");  
- MediaFormat mediaFormat = MediaFormat.createVideoFormat(type, width, height);  
- mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, 125000);  
- mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, 15);  
- mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar);  
- mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 5);  
- mediaCodec.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);  
- mediaCodec.start();  
    然后就是给他喂数据了，这里的数据是来自摄像头的：
**[java]**[view
 plain](http://blog.csdn.net/halleyzhang3/article/details/11473961#)[copy](http://blog.csdn.net/halleyzhang3/article/details/11473961#)
- publicvoid onFrame(byte[] buf, int offset, int length, int flag) {  
-    ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();  
-    ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();  
- int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);  
- if (inputBufferIndex >= 0)  
-        ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];  
-        inputBuffer.clear();  
-        inputBuffer.put(buf, offset, length);  
-        mediaCodec.queueInputBuffer(inputBufferIndex, 0, length, 0, 0);  
-    }  
-    MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();  
- int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo,0);  
- while (outputBufferIndex >= 0) {  
-        ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];  
- if (frameListener != null)  
-            frameListener.onFrame(outputBuffer, 0, length, flag);  
-        mediaCodec.releaseOutputBuffer(outputBufferIndex, false);  
-        outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);  
-    }   
先把来自摄像头的数据喂给它，然后从它里面取压缩好的数据喂给解码器。
3、解码和显示
     首先初始化解码器：
**[java]**[view
 plain](http://blog.csdn.net/halleyzhang3/article/details/11473961#)[copy](http://blog.csdn.net/halleyzhang3/article/details/11473961#)
- mediaCodec = MediaCodec.createDecoderByType("Video/AVC");  
- MediaFormat mediaFormat = MediaFormat.createVideoFormat(mime, width, height);  
- mediaCodec.configure(mediaFormat, surface, null, 0);  
- mediaCodec.start();  
             这里通过给解码器一个surface，解码器就能直接显示画面。
     然后就是处理数据了：
**[java]**[view
 plain](http://blog.csdn.net/halleyzhang3/article/details/11473961#)[copy](http://blog.csdn.net/halleyzhang3/article/details/11473961#)
- publicvoid onFrame(byte[] buf, int offset, int length, int flag) {  
-         ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();  
- int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);  
- if (inputBufferIndex >= 0) {  
-             ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];  
-             inputBuffer.clear();  
-             inputBuffer.put(buf, offset, length);  
-             mediaCodec.queueInputBuffer(inputBufferIndex, 0, length, mCount * 1000000 / FRAME_RATE, 0);  
-                    mCount++;  
-         }  
- 
-        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();  
- int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo,0);  
- while (outputBufferIndex >= 0) {  
-            mediaCodec.releaseOutputBuffer(outputBufferIndex, true);  
-            outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);  
-        }  
- }  
        queueInputBuffer第三个参数是时间戳，其实怎么写都无所谓，只要是按时间线性增加的就可以，这里就随便弄一个了。后面一段的代码就是把缓冲区给释放掉，因为我们直接让解码器显示，就不需要解码出来的数据了，但是必须要这么释放一下，否则解码器始终给你留着，内存就该不够用了。
好了，到现在，基本上就可以了。如果你运气够好，现在就能看到视频了，比如在我的三星手机上这样就可以了。但是，我试过几个其他平台，多数都不可以，总是有各种各样的问题，如果要开发一个不依赖平台的应用，还有很多的问题要解决。说说我遇到的一些情况：
1、视频尺寸
     一般都能支持176X144/352X288这种尺寸，但是大一些的，640X480就有很多机子不行了，至于为什么，我也不知道。当然，这个尺寸必须和摄像头预览的尺寸一致，预览的尺寸可以枚举一下。
2、颜色空间
    根据ANdroid SDK文档，确保所有硬件平台都支持的颜色，在摄像头预览输出是YUV12，在编码器输入是COLOR_FormatYUV420Planar，也就是前面代码中设置的那样。       不过，文档终究是文档，否则安卓就不是安卓。
    在有的平台上，这两个颜色格式是一样的，摄像头的输出可以直接作为编码器的输入。也有的平台，两个是不一样的，前者就是YUV12，后者等于I420，需要把前者的UV分量颠倒一下。下面的代码效率不高，可供参考。
**[java]**[view
 plain](http://blog.csdn.net/halleyzhang3/article/details/11473961#)[copy](http://blog.csdn.net/halleyzhang3/article/details/11473961#)
- byte[] i420bytes = null;  
- privatebyte[] swapYV12toI420(byte[] yv12bytes, int width, int height) {  
- if (i420bytes == null)  
-         i420bytes = newbyte[yv12bytes.length];  
- for (int i = 0; i < width*height; i++)  
-         i420bytes[i] = yv12bytes[i];  
- for (int i = width*height; i < width*height + (width/2*height/2); i++)  
-         i420bytes[i] = yv12bytes[i + (width/2*height/2)];  
- for (int i = width*height + (width/2*height/2); i < width*height + 2*(width/2*height/2); i++)  
-         i420bytes[i] = yv12bytes[i - (width/2*height/2)];  
- return i420bytes;  
- }  
      这里的困难是，我不知道怎样去判断是否需要这个转换。据说，Android 4.3不用再从摄像头的PreView里面取图像，避开了这个问题。这里有个例子，虽然我没读，但看起来挺厉害的样子，应该不会有错吧（觉厉应然）。[http://bigflake.com/mediacodec/CameraToMpegTest.java.txt](http://bigflake.com/mediacodec/CameraToMpegTest.java.txt)
3、输入输出缓冲区的格式
    SDK里并没有规定格式，但是，这种情况H264的格式基本上就是附录B。但是，也有比较有特色的，它就是不带那个StartCode，就是那个0x000001，搞得把他编码器编出来的东西送给他的解码器，他自己都解不出来。还好，我们可以自己加。
**[java]**[view
 plain](http://blog.csdn.net/halleyzhang3/article/details/11473961#)[copy](http://blog.csdn.net/halleyzhang3/article/details/11473961#)
- ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];  
- byte[] outData = newbyte[bufferInfo.size + 3];  
-        outputBuffer.get(outData, 3, bufferInfo.size);  
- if (frameListener != null) {  
- if ((outData[3]==0 && outData[4]==0 && outData[5]==1)  
-     || (outData[3]==0 && outData[4]==0 && outData[5]==0 && outData[6]==1))  
-     {  
-         frameListener.onFrame(outData, 3, outData.length-3, bufferInfo.flags);  
-     }  
- else
-     {  
-      outData[0] = 0;  
-      outData[1] = 0;  
-      outData[2] = 1;  
-         frameListener.onFrame(outData, 0, outData.length, bufferInfo.flags);  
-     }  
- }  
4、有时候会死在dequeueInputBuffer(-1)上面
根据SDK文档，dequeueInputBuffer 的参数表示等待的时间（毫秒），-1表示一直等，0表示不等。按常理传-1就行，但实际上在很多机子上会挂掉，没办法，还是传0吧，丢帧总比挂掉好。当然也可以传一个具体的毫秒数，不过没什么大意思吧。
在遇到上述的问题之后，我给出了我的感慨：Android是一个玩具。
