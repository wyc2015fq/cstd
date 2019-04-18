# Android平台使用MediaCodec设置码率无效(跟实际码率偏差很大)的问题解决 - 爱写代码的自由人 - CSDN博客





2018年07月21日 16:43:27[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：1864








Android从4.1系统开始，引进了MediaCodec API，支持对图像进行硬编码，但很多读者刚开始使用这套API遇到很多问题，特别是不知道如何正确设置编码器的参数。设置MediaCodec编码器涉及如下几个重要的参数：码率模式（固定码率、可变码率），码率，帧率、I帧间隔，H264的Profile和Level，下面是一段初始化参数的代码：

```
public AvcEncode(int width, int height, int framerate, int bitrate, int i_frame_interval) {
		this.width = width;
		this.height = height;
		yuv420 = new byte[width * height * 3 / 2];
		m_FrameRate = framerate;
		m_Bitrate = bitrate;
		
		try{
			mediaCodec = MediaCodec.createEncoderByType("video/avc");
			MediaFormat mediaFormat = null;
		
			mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
			
			mediaFormat.setInteger(MediaFormat.KEY_BITRATE_MODE, MediaCodecInfo.EncoderCapabilities.BITRATE_MODE_CBR);
			mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitrate);
			mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, framerate);
			
			// 根据手机设置不同的颜色参数
			mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar);
			// mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedPlanar);

			mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, i_frame_interval);

			//mediaFormat.setInteger(MediaFormat.KEY_PROFILE, MediaCodecInfo.CodecProfileLevel.AVCProfileHigh); //某些设备不支持设置Profile和Level，而应该采用默认设置
			//mediaFormat.setInteger("level", MediaCodecInfo.CodecProfileLevel.AVCLevel41); // Level 4.1

			
			mediaCodec.configure(mediaFormat, null, null,MediaCodec.CONFIGURE_FLAG_ENCODE);
			
			m_outData = new byte[width*height*2];
			
		}catch(Exception e){
			e.printStackTrace();
		}

	}
```

安卓默认的码率模式是可变编码的，即码率会根据场景变化有比较大的波动，而固定码率则使码率恒定在一个范围。虽然我们可以 通过MediaFormat.KEY_BITRATE_MODE来设置CBR模式，但在很多设备上是设置无效的。

     第二，关于设置码率的，很多人遇到设置的码率跟实际偏差很大，比如设置的是500K码率，但是实际产生的文件的码率是几M的，其实这个问题是跟帧率有关系的，如果把帧率设置为25，即mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, 25); 则产生的码率跟实际的就很接近了，这个是我测试后得出的结果。我刚开始设置的帧率是15，因为我手机采集图像的帧率每秒最高为15帧，结果实际的文件的体积很大，后来改成25Fps，码率就正常了。

   另外，说一下Profile的设置，网上说，在Android7.0以下，设置Profile是无效的，因为Android系统内部写死了参数，编码出来的只能是Baseline，除非你用的系统改过了这个Bug，否则设置了无效甚至会导致Configure参数失败。



