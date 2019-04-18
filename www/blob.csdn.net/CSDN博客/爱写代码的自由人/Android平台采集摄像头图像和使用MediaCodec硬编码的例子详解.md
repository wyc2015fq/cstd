# Android平台采集摄像头图像和使用MediaCodec硬编码的例子详解 - 爱写代码的自由人 - CSDN博客





2018年07月21日 16:20:10[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：1102








很多Android系统上的应用需要采集摄像头图像，并把图像编码成某种格式（比如H264），保存成文件或发送到网络。虽然有FFmpeg可以实现编码的功能，但是使用软编码一方面比较耗电，另一方面，对于CPU性能不是太强的ARM设备来说，软件编码肯定很占CPU资源，有些旧的机器甚至一编码就卡机。从Android 4.1系统起，引进了MediaCodec API，可以实现硬编码的功能，这解决了广大程序员编码视频的难题，但是这套API使用起来并不简单，有很多细节要注意，很容易掉进一些坑，下面我会讲解怎么越过那些坑，但是首先我先用代码说明一下怎么在Android上调用摄像头的API来采集图像。
- 采集摄像头图像

```
public class MainActivity extends Activity {
	private SurfaceView surfaceView;
	private Camera camera;
	private SurfaceHolder surfaceHolder;
	private Camera.Parameters mParameters = null;
	@SuppressLint("InlinedApi")
	private int mCameraId = Camera.CameraInfo.CAMERA_FACING_BACK;
	// 不同手机有不同的匹配颜色
	private int width = 1280;
	private int height = 720;
	private int mFrameRate = 25;
	private int bitrate = 800*1024;
	byte[] h264 = new byte[width * height * 3 / 2];
	byte[] mPreBuffer = null;
	
	private AvcEncode avcEncode;
	
	private Button button1;
	private Button button2;
    private TextView labelFrameRate;
    private TextView labelCPULevel;

	Timer timer = null;
	Boolean m_bRunning = false;
	private long mLastStartCountTime = 0;
	private int  mFrameCapturedCountInCycle = 0;
	
	static {
		// System.loadLibrary("gnustl_shared");
		System.loadLibrary("songstudio");
	}
	
	//EncodeVideo EncoderJni = new EncodeVideo(); //构建软解编码器
	
	@SuppressWarnings("deprecation")
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		setContentView(R.layout.activity_main);

		button1 = (Button) findViewById(R.id.btn_start_preview);
		button2 = (Button) findViewById(R.id.btn_stop_preview);
		
		labelFrameRate = (TextView) findViewById(R.id.label_framerate);
		labelCPULevel = (TextView) findViewById(R.id.label_cpu_level);
		

		avcEncode = new AvcEncode(width, height, mFrameRate, bitrate, 20);
		
		//EncoderJni.SetFilePath(FileOperator.fileName2);
		//EncoderJni.SetParameters(width, height, mFrameRate, bitrate, 25);
		
		surfaceView = (SurfaceView) findViewById(R.id.surfaceView);
		surfaceHolder = surfaceView.getHolder();
		surfaceHolder.setFixedSize(width, height);
		surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		surfaceHolder.addCallback(new SurfaceCallback());

//		 Sampler.getInstance().init(getApplicationContext(), 3000L);
//		 Sampler.getInstance().start();
		 
		button1.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// TODO 自动生成的方法存根
				//EncoderJni.OpenEncoder();
				avcEncode.startEncode();
				camera.startPreview();
			
       		 mLastStartCountTime = System.currentTimeMillis();
       		 mFrameCapturedCountInCycle = 0;
       		 
				if(timer != null){
					timer.cancel();
					timer = null;
				}
				
				timer = new Timer();
				timer.schedule(new TimerTask() {  
					    //int i=10;
					    	
					        @Override  
					        public void run() {  
					        
					        	if(m_bRunning)
					        	{
						            Message message = new Message();  
						            message.what = 1;  
						            handler.sendMessage(message); 
					        	}
					        	else
					        	{
					        		timer.cancel();
					        		System.out.println("timer canceled!");
					        		timer = null;
					        	}
					        }  //run
					    }, 
						500, 2000); 
				
				m_bRunning = true;
			}
			
		});

		button2.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				if(camera != null && m_bRunning){
					camera.stopPreview();
				}

				avcEncode.close();
				//EncoderJni.CloseEncoder();

				m_bRunning = false;
			}
		});
	}

	protected void OnDestroy() {
		if(camera != null){
			camera.stopPreview();
		}
		//EncoderJni.CloseEncoder();
		avcEncode.close();
		
		m_bRunning = false;
		
		super.onDestroy();
	}

    Handler handler = new Handler() {  
        public void handleMessage(Message msg) {  
          
        	long CurrentTime = System.currentTimeMillis();
        	 if(CurrentTime - mLastStartCountTime > 2000)
        	 {
        		 long nAverateFrameRate = mFrameCapturedCountInCycle*1000/(CurrentTime - mLastStartCountTime);
        		 labelFrameRate.setText(Integer.toString((int)nAverateFrameRate));
        		 
        		 mLastStartCountTime = CurrentTime;
        		 mFrameCapturedCountInCycle = 0;
        	 }
        	
        	
        	labelCPULevel.setText(Integer.toString((int)Sampler.getInstance().m_lastCPULevel));

            super.handleMessage(msg);  
        };  
    }; 
    
	private final class SurfaceCallback implements Callback, PreviewCallback {

		@SuppressLint("NewApi")
		public void surfaceCreated(SurfaceHolder holder) {
			// TODO 自动生成的方法存根
			try {
				if (mCameraId == Camera.CameraInfo.CAMERA_FACING_BACK) {
					camera = Camera.open();
				} else {
					camera = Camera.open(mCameraId);
				}
				camera.setPreviewDisplay(surfaceHolder); //是否预览显示
				//camera.setDisplayOrientation(90);
				mParameters = camera.getParameters();
				mParameters.setPreviewSize(width, height);
				mParameters.setPictureSize(width, height);
				
				// 此处颜色设置与后面要匹配 否则颜色会出现变化
				mParameters.setPreviewFormat(ImageFormat.YV12);
				camera.setParameters(mParameters);
				
			    int size = width * height * 3 / 2;
	            if (mPreBuffer == null) {
	                 mPreBuffer = new byte[size];
	            }
	            camera.addCallbackBuffer(mPreBuffer);
	 
				camera.setPreviewCallbackWithBuffer(this);
				// camera.startPreview();

			} catch (Exception e) {
				Log.e("TEST", "setPreviewDisplay fail " + e.getMessage());
			}
		}

		public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
			// TODO 自动生成的方法存根
		}

		public void surfaceDestroyed(SurfaceHolder holder) {
			// TODO 自动生成的方法存根
			camera.setPreviewCallback(null);
			camera.stopPreview();
			camera.release();
			camera = null;
			
			avcEncode.close();

		}

		public void onPreviewFrame(byte[] data, Camera camera) {

			avcEncode.offerEncode(data, h264);
			//avcEncode.CompressBuffer(data,  h264);
			
			mFrameCapturedCountInCycle++;
			 
			//EncoderJni.PushData(data, 0);
			
			camera.addCallbackBuffer(mPreBuffer);
		}

	}
}
```

Camera是一个专门用来连接和断开相机服务的类，用来处理相机相关的事件。上面的**SurfaceView**用于预览图像，并且重载了Camera.PreviewCallback接口的onPreviewFrame函数，用于获得捕捉到的图像，而图像的格式是根据mParameters.setPreviewFormat来指定，这是是设置为YV12；而预览的图像大小和采集的图像大小分别通过mParameters.setPreviewSize和mParameters.setPictureSize来设置。另外，我们还调用了addCallbackBuffer来重用Buffer，如果不调用这个API，则回调函数**onPreviewFrame**调用的时候每次都会分配一块内存传递图像数据，这样频繁分配内存可能会造成比较多的内存碎片，降低系统性能，如果调用了**addCallbackBuffer**对Buffer进行循环利用，系统内核调用回调时就不会重新分配内存，所以建议采集图像时还是尽量使用addCallbackBuffer方法（虽然有点麻烦）。

除了SurfaceView，我们还可以使用**SurfaceTexture**来获得图像流，虽然我们这个代码里没有用到。SurfaceTexture是从Android3.0（API 11）加入的一个新类，跟SurfaceView很像，可以从camera preview或者video decode里面获取图像流（image stream）。SurfaceView从camera读取到的预览（preview）图像流一定要输出到一个可见的（Visible）SurfaceView上，而SurfaceTexture在接收图像流后，不需要显示出来。所以在有些需求上，比如在Service后台里面获取camera预览帧进行处理就必须使用SurfaceTexture才能实现。关于SurfaceTexture的使用，大家可参考这篇博文：[https://blog.csdn.net/u012874222/article/details/70216700](https://blog.csdn.net/u012874222/article/details/70216700)。

2.使用MediaCodec编码

MediaCodec的使用在[Android Developer](https://developer.android.com/reference/android/media/MediaCodec.html)官网上有详细的说明。官网上的图能够很好的说明MediaCodec的使用方式。我们只需理解这个图，然后熟悉下MediaCodec的API就可以很快的上手使用MediaCodec来进行音视频的编解码工作了。

![](https://img-blog.csdn.net/20180721153252921?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pob3Vib3RvbmcyMDEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 我们还需要知道，MediaCodec总共有三种使用方法：

        同步数据处理（使用buffer arrays） 从Android4.1 api 16即可以使用；

        同步数据处理 （使用buffers ） 从Android5.0 api 21 即可以使用；

       异步数据处理（使用buffers ） 从Android5.0 api 21 即可以使用；

      同步与异步处理数据的主要不同点是：对于同步处理是循环的将待处理数据交给编解码器（不用理会编解码器是否已经准备好接收数据），而异步处理则是每次都会去等待编解码器已经能够处理数据时，才将待处理数据送给相应编解码器。

     第一种和第二种方法区别不大，而第一种方法适用性更广，因为对API的Level要求低。而第三种方法对性能要求比较高的场合可考虑使用，因为它的编码是异步的，编码操作是内部进行，所以不会卡住原先的线程，而如果把采集图像和编码都放在一个线程（就像方法一和方法二），则可能出现一个线程做的工作太多，导致效率低了。

   为了简单起见，我的例子使用了方法一。

   使用MediaCodec前要初始化编码器的格式，下面是初始化的代码：

```
public AvcEncode(int width, int height, int framerate, int bitrate, int i_frame_interval) {
		this.width = width;
		this.height = height;
		yuv420 = new byte[width * height * 3 / 2];
		m_FrameRate = framerate;
		m_Bitrate = bitrate;
		bConfigBitrate = false;
		
		try{
			mediaCodec = MediaCodec.createEncoderByType("video/avc");
			MediaFormat mediaFormat = null;
			if (CamcorderProfile.hasProfile(CamcorderProfile.QUALITY_720P)) {
				mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
			} else {
				mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
			}
			mediaFormat.setInteger(MediaFormat.KEY_BITRATE_MODE, MediaCodecInfo.EncoderCapabilities.BITRATE_MODE_CBR);
			mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitrate);
			mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, framerate);
			
			// 根据手机设置不同的颜色参数
			mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar);
			// mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedPlanar);
			mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 1);

			
			mediaCodec.configure(mediaFormat, null, null,MediaCodec.CONFIGURE_FLAG_ENCODE);
			
			m_outData = new byte[width*height*2];
			
		}catch(Exception e){
			e.printStackTrace();
		}

	}
```

这里有几个很重要的参数，包括：码率模式，码率，帧率，I帧间隔，码率的单位是比特的，例如：512000，表示码率是512KBps。I帧间隔是设置相邻两个I帧的间隔，这里的单位是秒，比如1秒，表示每隔1秒插入一个I帧。而Profile和Level这里没有设置，因为在Android7.0以下，设置这两个参数在很多设备上是无效的，如果设置了，可能会导致后面的configure函数失败。这里有必要说明一下，**很多读者遇到一个坑，就是设置的码率不生效，其实这个是跟设置的帧率有关系，如果设置的帧率为25（就算你手机的摄像头实际采集的帧率达不到这么高也没关系），则实际码率就跟设置的一样了。**

从摄像头采集到的图像数据在回调函数OnPreviewFrame中获得，里面调用了编码函数对图像进行编码，回调函数如下：

```
public void onPreviewFrame(byte[] data, Camera camera) {

			avcEncode.offerEncode(data, h264);
			//avcEncode.CompressBuffer(data,  h264);
			
			mFrameCapturedCountInCycle++;
			 
			//EncoderJni.PushData(data, 0);
			
			camera.addCallbackBuffer(mPreBuffer);
		}
```

 使用MediaCodec编码的函数如下：

```
public void offerEncode(byte[] input, byte[] output) {
	
		int pos = 0;
		swapYV12toI420(input, yuv420, width, height);
		
		try {


			int inputBufferIndex = mediaCodec.dequeueInputBuffer(0);

			if (inputBufferIndex >= 0) {
				ByteBuffer inputBuffer = mediaCodec.getInputBuffer(inputBufferIndex);
				inputBuffer.clear();
				inputBuffer.put(yuv420);
				mediaCodec.queueInputBuffer(inputBufferIndex, 0, yuv420.length,
						(System.currentTimeMillis() - start_timeStamp)*1000, 0);
			}
			else{
				return;
			}
			MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();

			int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 100000);
			
			while (outputBufferIndex >= 0) {
				ByteBuffer outputBuffer = mediaCodec.getOutputBuffer(outputBufferIndex);
				
				int n = m_outData.length;
				outputBuffer.get(m_outData, 0, bufferInfo.size);
            
                
				int nalu_type = (m_outData[4] & 0x1F);
				
				if (m_info != null) {
					System.arraycopy(m_outData, 0, output, pos, bufferInfo.size);
					pos += bufferInfo.size;
		
				} else {
					
					if (nalu_type ==  0x07) {
						
						m_info = new byte[bufferInfo.size];
						System.arraycopy(m_outData, 0, m_info, 0, bufferInfo.size);
					}
				}
				mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
				outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
			}//while
			
			if (output[4] == 0x65) { //key frame, 编码器生成关键帧时没有pps sps， 要加上 
				if(m_info != null){
					System.arraycopy(output, 0, yuv420, 0, pos);
					System.arraycopy(m_info, 0, output, 0, m_info.length);
					System.arraycopy(yuv420, 0, output, m_info.length, pos);
					pos += m_info.length;	
				}

			}
			if(pos > 0)
			   addToFile(output, pos);

			mFrameNum++;
		} catch (Throwable t) {
			t.printStackTrace();
		}
	
	}
```

这里，我们调用swapYV12toI420函数将输入的YUV图像数据拷贝到另外一块内存地址，并且将U和V两个平面交换，这里可能大家有点难理解，为什么要交换呢？因为如果直接将传入的YUV图像显示出来，其实色度是错乱的，但如果将U，V交换，则颜色就正常了。

  在代码中，我们还要关注两个细节，一个是要在开始编码的前几帧提取PPS和SPS，第二是每个I帧前面要插入SPS和PPS，上面的m_info数组存储的就是SPS和PPS的内容（一般这两个东西都是从同一帧编码出来）。

   好，硬编码的用法就讲完了，下面是例子工程的链接，大家可以下载例子App测试一下编码的效果。

[https://download.csdn.net/download/zhoubotong2012/10555784](https://download.csdn.net/download/zhoubotong2012/10555784)





