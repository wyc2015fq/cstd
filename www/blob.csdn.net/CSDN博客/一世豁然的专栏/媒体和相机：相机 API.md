# 媒体和相机：相机 API - 一世豁然的专栏 - CSDN博客





2017年05月06日 10:45:49[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1888








Android框架包括对设备上可用的各种相机和相机功能的支持，可让您在应用程序中捕获图片和视频。 本文档讨论了一种快速，简单的图像和视频捕获方法，并概述了为用户创建自定义相机体验的高级方法。




注意：此页面描述了已被弃用的Camera类。 我们建议使用更新的camera2，它适用于Android 5.0（API级别21）或更高版本。 在我们的博客上阅读更多关于camera2的信息。








一、注意事项

在启用你的应用程序来使用Android设备上相机之前，您应该考虑一些关于您的应用程序如何使用此硬件功能的问题。


1、相机要求 - 使用相机对您的应用程序如此重要，您不希望将应用程序安装在没有相机的设备上？ 如果是这样，您应该在清单中声明相机要求。





2、快速拍照或定制相机 - 您的应用如何使用相机？ 您是否只想拍摄快速的图片或视频剪辑，或者您的应用程序是否提供使用相机的新方法？ 要获得快速捕捉或剪辑，请考虑使用现有的相机应用程序。 要开发定制的相机功能，请查看构建相机应用程序部分。




3、存储 - 您的应用程序生成的图像或视频是仅应用程序可见或共享的，以便其他应用程序（如Gallery或其他媒体和社交应用程序）可以使用它们？ 即使您的应用程序被卸载，您是否希望图片和视频可用？ 查看“保存媒体文件”部分，了解如何实现这些选项。







二、基础

Android框架支持通过android.hardware.camera2 API或摄像机Intent捕获图像和视频。 以下是相关类：


[android.hardware.camera2](https://developer.android.google.cn/reference/android/hardware/camera2/package-summary.html)


该软件包是用于控制设备摄像机的主要API。 当您构建相机应用程序时，可用于拍摄照片或视频。





[Camera](https://developer.android.google.cn/reference/android/hardware/Camera.html)


这个类是用于控制设备摄像机的旧版API。





[SurfaceView](https://developer.android.google.cn/reference/android/view/SurfaceView.html)


此类用于向用户呈现实时相机预览。





[MediaRecorder](https://developer.android.google.cn/reference/android/media/MediaRecorder.html)


该类用于从相机录制视频。





[Intent](https://developer.android.google.cn/reference/android/content/Intent.html)


MediaStore.ACTION_IMAGE_CAPTURE或MediaStore.ACTION_VIDEO_CAPTURE的意图操作类型可用于捕获图像或视频而不直接使用Camera对象。








三、清单声明

在使用Camera API开始开发应用程序之前，您应该确保您的清单具有适当的声明，以允许使用摄像头硬件和其他相关功能。





1、相机许可 - 您的应用程序必须要求使用设备摄像头的权限。


`<uses-permission android:name="android.permission.CAMERA" />`注意：如果您通过调用现有的相机应用程序来使用相机，则您的应用程序不需要请求此权限。





2、相机功能 - 您的应用程序还必须声明使用相机功能，例如：


`<uses-feature android:name="android.hardware.camera" />`





有关相机功能的列表，请参阅[清单功能参考](https://developer.android.google.cn/guide/topics/manifest/uses-feature-element.html#hw-features)。




将相机功能添加到您的清单会导致Google Play阻止将应用程序安装到不包含相机的设备或不支持您指定的相机功能。 有关使用基于功能的Google Play过滤功能的详细信息，请参阅Google Play和基于功能的过滤。



如果您的应用程序可以使用相机或相机功能进行正常操作，但不需要它，则应通过包含android：required属性并将其设置为false将其指定在清单中：



`<uses-feature android:name="android.hardware.camera" android:required="false" />`

3、存储权限 - 如果应用程序将图像或视频保存到设备的外部存储（SD卡），则还必须在清单中指定。




`<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />`

4、录音许可 - 对于使用视频捕获录制音频，您的应用程序必须请求音频捕获权限。




`<uses-permission android:name="android.permission.RECORD_AUDIO" />`

5、位置许可 - 如果您的应用程序使用GPS位置信息标记图像，则必须请求ACCESS_FINE_LOCATION权限。 请注意，如果您的应用定位到Android 5.0（API级别21）或更高版本，则还需要声明您的应用使用设备的GPS：





```
<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
...
<!-- Needed only if your app targets Android 5.0 (API level 21) or higher. -->
<uses-feature android:name="android.hardware.location.gps" />
```


有关获取用户位置的更多信息，请参阅[位置策略](https://developer.android.google.cn/guide/topics/location/strategies.html)。









四、使用现有的相机应用程序

使用Intent调用现有的Android相机应用程序，可以快速地在应用程序中拍摄图片或视频而不需要额外的代码。 培训课程描述了细节，[简单地拍摄照片](https://developer.android.google.cn/training/camera/photobasics.html)和[录制视频](https://developer.android.google.cn/training/camera/videobasics.html)。








五、构建相机应用程序

一些开发人员可能需要根据应用程序的外观进行定制的相机用户界面，或提供特殊功能。 编写自己的拍照代码可以为您的用户提供更引人注目的体验。





注意：以下指南是针对较旧的，已弃用的Camera API。 对于新的或高级的相机应用程序，建议使用较新的android.hardware.camera2 API。





为您的应用程序创建自定义相机界面的一般步骤如下：


1、检测和存取相机 - 创建代码以检查相机的存在并请求访问。




2、创建一个预览类 - 创建一个扩展SurfaceView的相机预览类，并实现SurfaceHolder接口。 该课程预览相机的实时图像。




3、构建预览布局 - 一旦你有相机预览类，创建一个视图布局，其中包含预览和所需的用户界面控件。




4、用于捕获的设置侦听器 - 连接您的界面控件的侦听器以响应用户操作（例如按下按钮）开始图像或视频捕获。




5、捕获并保存文件 - 设置捕获图片或视频的代码并保存输出。




6、释放相机 - 使用相机后，您的应用程序必须正确释放以供其他应用程序使用。




相机硬件是一种共享资源，必须仔细管理，以便您的应用程序不会与可能还要使用它的其他应用程序相冲突。 以下部分将讨论如何检测相机硬件，如何请求访问相机，如何捕获图片或视频以及如何在应用程序完成时释放相机。





注意：当您的应用程序完成使用它时，请记住通过调用Camera.release（）来释放Camera对象！ 如果您的应用程序没有正确地释放相机，则所有后续访问相机的尝试（包括您自己的应用程序）都将失败，并可能导致您或其他应用程序关闭。





一）、检测相机硬件

如果您的应用程序没有特别要求使用清单声明的相机，则应检查相机是否在运行时可用。 要执行此检查，请使用PackageManager.hasSystemFeature（）方法，如以下示例代码所示：




```java
/** Check if this device has a camera */
private boolean checkCameraHardware(Context context) {
    if (context.getPackageManager().hasSystemFeature(PackageManager.FEATURE_CAMERA)){
        // this device has a camera
        return true;
    } else {
        // no camera on this device
        return false;
    }
}
```


Android设备可以有多个摄像头，例如用于摄影的后置摄像头和用于视频通话的前置摄像头。 Android 2.3（API等级9）及更高版本允许您使用Camera.getNumberOfCameras（）方法检查设备上可用的摄像机数量。






二）、访问相机

如果您确定您的应用程序正在运行的设备有相机，则必须通过获取Camera的实例来请求访问它（除非您使用意图访问相机）。





要访问主摄像头，请使用Camera.open（）方法，并确保捕获任何异常，如以下代码所示：




```java
/** A safe way to get an instance of the Camera object. */
public static Camera getCameraInstance(){
    Camera c = null;
    try {
        c = Camera.open(); // attempt to get a Camera instance
    }
    catch (Exception e){
        // Camera is not available (in use or does not exist)
    }
    return c; // returns null if camera is unavailable
}
```


注意：使用Camera.open（）时，请始终检查异常情况。 如果相机使用或不存在，则无法检查异常会导致系统关闭应用程序。






在运行Android 2.3（API Level 9）或更高版本的设备上，您可以使用Camera.open（int）访问特定摄像机。 上面的示例代码将访问具有多个摄像头的设备上的第一个后置摄像头。





三）、检查相机功能

获取摄像机的权限后，您可以使用Camera.getParameters（）方法获取有关其功能的更多信息，并检查返回的Camera.Parameters对象以获得支持的功能。 当使用API Level 9或更高版本时，请使用Camera.getCameraInfo（）来确定相机是否在设备的正面或背面，以及图像的方向。





四）、创建一个预览类

为了让用户有效地拍照或录像，他们必须能够看到设备相机看到什么。 相机预览类是可以显示来自相机的实时图像数据的SurfaceView，因此用户可以对图片或视频进行构图和捕获。





以下示例代码演示如何创建可以包含在View布局中的基本相机预览类。 该类实现了SurfaceHolder.Callback，以捕获用于创建和销毁视图的回调事件，这是分配相机预览输入所需的。




```java
/** A basic Camera preview class */
public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback {
    private SurfaceHolder mHolder;
    private Camera mCamera;

    public CameraPreview(Context context, Camera camera) {
        super(context);
        mCamera = camera;

        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed.
        mHolder = getHolder();
        mHolder.addCallback(this);
        // deprecated setting, but required on Android versions prior to 3.0
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void surfaceCreated(SurfaceHolder holder) {
        // The Surface has been created, now tell the camera where to draw the preview.
        try {
            mCamera.setPreviewDisplay(holder);
            mCamera.startPreview();
        } catch (IOException e) {
            Log.d(TAG, "Error setting camera preview: " + e.getMessage());
        }
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        // empty. Take care of releasing the Camera preview in your activity.
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        // If your preview can change or rotate, take care of those events here.
        // Make sure to stop the preview before resizing or reformatting it.

        if (mHolder.getSurface() == null){
          // preview surface does not exist
          return;
        }

        // stop preview before making changes
        try {
            mCamera.stopPreview();
        } catch (Exception e){
          // ignore: tried to stop a non-existent preview
        }

        // set preview size and make any resize, rotate or
        // reformatting changes here

        // start preview with new settings
        try {
            mCamera.setPreviewDisplay(mHolder);
            mCamera.startPreview();

        } catch (Exception e){
            Log.d(TAG, "Error starting camera preview: " + e.getMessage());
        }
    }
}
```


如果要为相机预览设置特定尺寸，请在上述注释中指定的surfaceChanged（）方法中进行设置。 设置预览大小时，您必须使用getSupportedPreviewSizes（）中的值。 不要在setPreviewSize（）方法中设置任意值。






注意：随着Android 7.0（API级别24）及更高版本中引入了多窗口功能，即使在调用setDisplayOrientation（）之后，您也不能再假设预览的宽高比与您的活动相同。 根据窗口大小和宽高比，您可能需要使用信箱布局将宽大的相机预览适用于纵向导向的布局，反之亦然。





五）、在布局中放置预览

相机预览类，如上一节所示的示例，必须与其他用户界面控件一起放置在活动的布局中，以拍摄照片或视频。 本节介绍如何为预览构建基本布局和活动。





以下布局代码提供了一个非常基本的视图，可用于显示相机预览。 在这个例子中，FrameLayout元素是摄像机预览类的容器。 使用这种布局类型，以便附加的图片信息或控件可以覆盖在实况相机预览图像上。




```
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:orientation="horizontal"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    >
  <FrameLayout
    android:id="@+id/camera_preview"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:layout_weight="1"
    />

  <Button
    android:id="@+id/button_capture"
    android:text="Capture"
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:layout_gravity="center"
    />
</LinearLayout>
```


在大多数设备上，相机预览的默认方向是横向。 此示例布局指定了一个水平（横向）布局，下面的代码将应用程序的方向定义为横向。 为了简化渲染相机预览，您应该将应用程序的预览活动方向更改为横向，方法是将以下内容添加到清单中。





```
<activity android:name=".CameraActivity"
          android:label="@string/app_name"

          android:screenOrientation="landscape">
          <!-- configure this activity to use landscape orientation -->

          <intent-filter>
        <action android:name="android.intent.action.MAIN" />
        <category android:name="android.intent.category.LAUNCHER" />
    </intent-filter>
</activity>
```


注意：相机预览不必处于横向模式。 从Android 2.2（API 8级）开始，您可以使用setDisplayOrientation（）方法设置预览图像的旋转。 为了在用户重新定向手机时更改预览方向，在预览类的surfaceChanged（）方法中，首先使用Camera.stopPreview（）更改方向来停止预览，然后再次使用Camera.startPreview（）。






在相机视图的活动中，将您的预览类添加到上述示例中所示的FrameLayout元素。 您的相机活动还必须确保它在暂停或关闭时释放相机。 以下示例显示如何修改相机活动以附加“创建预览”类中显示的预览类。




```java
public class CameraActivity extends Activity {

    private Camera mCamera;
    private CameraPreview mPreview;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        // Create an instance of Camera
        mCamera = getCameraInstance();

        // Create our Preview view and set it as the content of our activity.
        mPreview = new CameraPreview(this, mCamera);
        FrameLayout preview = (FrameLayout) findViewById(R.id.camera_preview);
        preview.addView(mPreview);
    }
}
```


注意：上述示例中的getCameraInstance（）方法是指访问摄像机中显示的示例方法。






六）、捕捉图片

一旦建立了一个预览类和一个显示它的视图布局，就可以开始使用应用程序捕获图像了。 在应用程序代码中，您必须设置用户界面控件的侦听器，以便通过拍摄照片来响应用户操作。





为了检索图片，请使用Camera.takePicture（）方法。 该方法采用三个接收数据的参数。 为了以JPEG格式接收数据，您必须实现一个Camera.PictureCallback接口来接收图像数据并将其写入文件。 以下代码显示了Camera.PictureCallback界面的基本实现，用于保存从相机接收的图像。




```java
private PictureCallback mPicture = new PictureCallback() {

    @Override
    public void onPictureTaken(byte[] data, Camera camera) {

        File pictureFile = getOutputMediaFile(MEDIA_TYPE_IMAGE);
        if (pictureFile == null){
            Log.d(TAG, "Error creating media file, check storage permissions: " +
                e.getMessage());
            return;
        }

        try {
            FileOutputStream fos = new FileOutputStream(pictureFile);
            fos.write(data);
            fos.close();
        } catch (FileNotFoundException e) {
            Log.d(TAG, "File not found: " + e.getMessage());
        } catch (IOException e) {
            Log.d(TAG, "Error accessing file: " + e.getMessage());
        }
    }
};
```


触发通过调用Camera.takePicture（）方法捕获图像。 以下示例代码显示了如何从按钮View.OnClickListener调用此方法。





```java
// Add a listener to the Capture button
Button captureButton = (Button) findViewById(id.button_capture);
captureButton.setOnClickListener(
    new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            // get an image from the camera
            mCamera.takePicture(null, null, mPicture);
        }
    }
);
```


注意：以下示例中的mPicture成员引用了上面的示例代码。



注意：当您的应用程序完成使用它时，请记住通过调用Camera.release（）来释放Camera对象！ 有关如何释放相机的信息，请参阅释放相机。





七）、捕获视频

使用Android框架进行视频捕获需要仔细管理Camera对象并与MediaRecorder类进行协调。 使用Camera录制视频时，除Camera.open（）和Camera.release（）调用之外，还必须管理Camera.lock（）和Camera.unlock（）调用以允许MediaRecorder访问摄像机硬件。





注意：从Android 4.0（API级别14）开始，Camera.lock（）和Camera.unlock（）调用将自动为您管理。





与使用设备摄像头拍摄照片不同，拍摄视频需要非常特别的呼叫顺序。 您必须按照特定的执行顺序成功准备和捕获与应用程序的视频，具体如下。


1、打开摄像头 - 使用Camera.open（）获取摄像机对象的实例。




2、连接预览 - 使用Camera.setPreviewDisplay（）将SurfaceView连接到摄像机，准备实时摄像机图像预览。




3、开始预览 - 调用Camera.startPreview（）开始显示实时摄像头图像。




4、开始录制视频 - 必须完成以下步骤才能成功录制视频：

 1）、解锁相机 - 通过调用Camera.unlock（）来解锁MediaRecorder使用的相机。

 2）、配置MediaRecorder - 按照以下顺序调用以下MediaRecorder方法。 有关详细信息，请参阅MediaRecorder参考文档。

I、[setCamera()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setCamera(android.hardware.Camera))-
 将相机设置为用于视频捕获，使用应用程序当前的Camera实例。

II、[setAudioSource()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setAudioSource(int))-
 设置音频源，使用MediaRecorder.AudioSource.CAMCORDER。

III、[setVideoSource()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setVideoSource(int))-
 设置视频源，使用MediaRecorder.VideoSource.CAMERA。

IV、设置视频输出格式和编码。 对于Android 2.2（API 8级）及更高版本，请使用MediaRecorder.setProfile方法，并使用CamcorderProfile.get（）获取配置文件实例。 对于2.2之前的Android版本，您必须设置视频输出格式和编码参数：

·[setOutputFormat()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setOutputFormat(int))-
 设置输出格式，指定默认设置或MediaRecorder.OutputFormat.MPEG_4。

·[setAudioEncoder()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setAudioEncoder(int))-
 设置声音编码类型，指定默认设置或MediaRecorder.AudioEncoder.AMR_NB。

·[setVideoEncoder()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setVideoEncoder(int))-
 设置视频编码类型，指定默认设置或MediaRecorder.VideoEncoder.MPEG_4_SP。

V、[setOutputFile()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setOutputFile(java.lang.String))
 - 从“保存媒体文件”部分的示例方法中设置输出文件，使用getOutputMediaFile（MEDIA_TYPE_VIDEO）.toString（）。

VI、[setPreviewDisplay()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setPreviewDisplay(android.view.Surface))-
 为应用程序指定SurfaceView预览布局元素。 使用您为Connect Preview指定的相同对象。

注意：您必须按此顺序调用这些MediaRecorder配置方法，否则您的应用程序将遇到错误，并且录制将失败。

 3）、准备MediaRecorder - 通过调用MediaRecorder.prepare（）来准备提供的配置设置的MediaRecorder。

 4）、启动MediaRecorder - 通过调用MediaRecorder.start（）开始录制视频。




5、停止录制视频 - 调用以下方法顺序，成功完成录像：

 1）、停止MediaRecorder - 通过调用MediaRecorder.stop（）停止录制视频。

 2）、重新设置MediaRecorder - （可选）通过调用MediaRecorder.reset（）从记录器中删除配置设置。

 3）、释放MediaRecorder - 通过调用MediaRecorder.release（）释放MediaRecorder。

 4）、锁定相机 - 锁定相机，以便将来的MediaRecorder会话可以通过调用Camera.lock（）来使用它。 从Android 4.0（API级别14）开始，除非MediaRecorder.prepare（）调用失败，否则不需要此呼叫。




6、停止预览 - 使用相机完成活动后，请使用Camera.stopPreview（）停止预览。




7、释放相机 - 释放相机，以便其他应用程序可以通过调用Camera.release（）来使用它。




注意：可以先使用MediaRecorder创建相机预览，并跳过此过程的前几个步骤。 然而，由于用户通常喜欢在开始录制之前看到预览，所以这里不讨论该过程。





提示：如果您的应用程序通常用于录制视频，请在启动预览之前将setRecordingHint（boolean）设置为true。 此设置可以帮助减少开始录制所需的时间。





配置MediaRecorder


使用MediaRecorder类录制视频时，必须按照特定的顺序执行配置步骤，然后调用MediaRecorder.prepare（）方法来检查和实现配置。 以下示例代码演示如何正确配置和准备MediaRecorder类进行视频录制。




```java
private boolean prepareVideoRecorder(){

    mCamera = getCameraInstance();
    mMediaRecorder = new MediaRecorder();

    // Step 1: Unlock and set camera to MediaRecorder
    mCamera.unlock();
    mMediaRecorder.setCamera(mCamera);

    // Step 2: Set sources
    mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.CAMCORDER);
    mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);

    // Step 3: Set a CamcorderProfile (requires API Level 8 or higher)
    mMediaRecorder.setProfile(CamcorderProfile.get(CamcorderProfile.QUALITY_HIGH));

    // Step 4: Set output file
    mMediaRecorder.setOutputFile(getOutputMediaFile(MEDIA_TYPE_VIDEO).toString());

    // Step 5: Set the preview output
    mMediaRecorder.setPreviewDisplay(mPreview.getHolder().getSurface());

    // Step 6: Prepare configured MediaRecorder
    try {
        mMediaRecorder.prepare();
    } catch (IllegalStateException e) {
        Log.d(TAG, "IllegalStateException preparing MediaRecorder: " + e.getMessage());
        releaseMediaRecorder();
        return false;
    } catch (IOException e) {
        Log.d(TAG, "IOException preparing MediaRecorder: " + e.getMessage());
        releaseMediaRecorder();
        return false;
    }
    return true;
}
```


在Android 2.2（API 8级）之前，您必须直接设置输出格式和编码格式参数，而不是使用CamcorderProfile。 此方法在以下代码中进行了说明：





```java
// Step 3: Set output format and encoding (for versions prior to API Level 8)
    mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);
    mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.DEFAULT);
    mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.DEFAULT);
```


MediaRecorder的以下视频录制参数为默认设置，但您可能需要调整应用程序的这些设置：




- `[setVideoEncodingBitRate()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setVideoEncodingBitRate(int))`
- `[setVideoSize()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setVideoSize(int,%20int))`
- `[setVideoFrameRate()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setVideoFrameRate(int))`
- `[setAudioEncodingBitRate()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setAudioEncodingBitRate(int))`
- `[setAudioChannels()](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setAudioChannels(int))`
- `[setAudioSamplingRate()使用MediaRecorder.release（）释放媒体录音机](https://developer.android.google.cn/reference/android/media/MediaRecorder.html#setAudioSamplingRate(int))`




启动和停止MediaRecorder


当使用MediaRecorder类开始和停止视频录制时，您必须遵循以下列出的特定顺序。


1、用Camera.unlock（）解锁相机。




2、配置MediaRecorder，如上面的代码示例所示。




3、使用MediaRecorder.start（）开始录制。




4、录制视频。




5、停止录制使用MediaRecorder.stop（）。




6、使用MediaRecorder.release（）释放媒体录音机




7、使用Camera.lock（）锁定相机




以下示例代码演示如何使用相机和MediaRecorder类连接按钮以正确启动和停止视频录制。





注意：完成录像时，请勿松开相机，否则您的预览将被停止。




```java
private boolean isRecording = false;

// Add a listener to the Capture button
Button captureButton = (Button) findViewById(id.button_capture);
captureButton.setOnClickListener(
    new View.OnClickListener() {
        @Override
        public void onClick(View v) {
            if (isRecording) {
                // stop recording and release camera
                mMediaRecorder.stop();  // stop the recording
                releaseMediaRecorder(); // release the MediaRecorder object
                mCamera.lock();         // take camera access back from MediaRecorder

                // inform the user that recording has stopped
                setCaptureButtonText("Capture");
                isRecording = false;
            } else {
                // initialize video camera
                if (prepareVideoRecorder()) {
                    // Camera is available and unlocked, MediaRecorder is prepared,
                    // now you can start recording
                    mMediaRecorder.start();

                    // inform the user that recording has started
                    setCaptureButtonText("Stop");
                    isRecording = true;
                } else {
                    // prepare didn't work, release the camera
                    releaseMediaRecorder();
                    // inform user
                }
            }
        }
    }
);
```


注意：在上面的示例中，prepareVideoRecorder（）方法指的是配置MediaRecorder中显示的示例代码。 此方法负责锁定相机，配置和准备MediaRecorder实例。






八）、释放相机

相机是由设备上的应用程序共享的资源。 获取相机实例后，您的应用程序可以使用相机，当应用程序停止使用时，必须特别小心释放相机对象，并且一旦应用程序暂停（Activity.onPause（））。 如果您的应用程序没有正确地释放相机，则所有后续访问相机的尝试（包括您自己的应用程序）都将失败，并可能导致您或其他应用程序关闭。





要释放Camera对象的实例，请使用Camera.release（）方法，如下面的示例代码所示。




```java
public class CameraActivity extends Activity {
    private Camera mCamera;
    private SurfaceView mPreview;
    private MediaRecorder mMediaRecorder;

    ...

    @Override
    protected void onPause() {
        super.onPause();
        releaseMediaRecorder();       // if you are using MediaRecorder, release it first
        releaseCamera();              // release the camera immediately on pause event
    }

    private void releaseMediaRecorder(){
        if (mMediaRecorder != null) {
            mMediaRecorder.reset();   // clear recorder configuration
            mMediaRecorder.release(); // release the recorder object
            mMediaRecorder = null;
            mCamera.lock();           // lock camera for later use
        }
    }

    private void releaseCamera(){
        if (mCamera != null){
            mCamera.release();        // release the camera for other applications
            mCamera = null;
        }
    }
}
```


注意：如果您的应用程序没有正确地释放相机，则所有后续访问相机的尝试（包括您自己的应用程序）都将失败，并可能导致您或其他应用程序关闭。









六、保存媒体文件

用户创建的媒体文件（如图片和视频）应保存到设备的外部存储目录（SD卡）中，以节省系统空间，并允许用户在不使用设备的情况下访问这些文件。 有许多可能的目录位置可以在设备上保存媒体文件，作为开发人员，您只能考虑两个标准位置：


1、`[Environment.getExternalStoragePublicDirectory](https://developer.android.google.cn/reference/android/os/Environment.html#getExternalStoragePublicDirectory(java.lang.String))`(`[Environment.DIRECTORY_PICTURES](https://developer.android.google.cn/reference/android/os/Environment.html#DIRECTORY_PICTURES)`)
 - 此方法返回保存图片和视频的标准，共享和推荐位置。 此目录是共享的（public），因此其他应用程序可以轻松地发现，读取，更改和删除保存在此位置的文件。 如果您的应用程序被用户卸载，保存到此位置的媒体文件将不会被删除。 为了避免干扰用户现有的图片和视频，您应该为此目录中的应用程序的媒体文件创建一个子目录，如下面的代码示例所示。 此方法在Android 2.2（API 8级）中可用，对于早期API版本中的等效调用，请参阅保存共享文件。




2、`[Context.getExternalFilesDir](https://developer.android.google.cn/reference/android/content/Context.html#getExternalFilesDir(java.lang.String))`(`[Environment.DIRECTORY_PICTURES](https://developer.android.google.cn/reference/android/os/Environment.html#DIRECTORY_PICTURES)`)
 - 此方法返回用于保存与应用程序相关联的图片和视频的标准位置。 如果您的应用程序被卸载，则保存在此位置的所有文件都将被删除。 对该位置的文件不执行安全性，其他应用程序可能会读取，更改和删除它们。




以下示例代码演示如何为媒体文件创建文件或Uri位置，该文件可以在使用Intent或构建相机应用程序的一部分调用设备的相机时使用。




```java
public static final int MEDIA_TYPE_IMAGE = 1;
public static final int MEDIA_TYPE_VIDEO = 2;

/** Create a file Uri for saving an image or video */
private static Uri getOutputMediaFileUri(int type){
      return Uri.fromFile(getOutputMediaFile(type));
}

/** Create a File for saving an image or video */
private static File getOutputMediaFile(int type){
    // To be safe, you should check that the SDCard is mounted
    // using Environment.getExternalStorageState() before doing this.

    File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(
              Environment.DIRECTORY_PICTURES), "MyCameraApp");
    // This location works best if you want the created images to be shared
    // between applications and persist after your app has been uninstalled.

    // Create the storage directory if it does not exist
    if (! mediaStorageDir.exists()){
        if (! mediaStorageDir.mkdirs()){
            Log.d("MyCameraApp", "failed to create directory");
            return null;
        }
    }

    // Create a media file name
    String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
    File mediaFile;
    if (type == MEDIA_TYPE_IMAGE){
        mediaFile = new File(mediaStorageDir.getPath() + File.separator +
        "IMG_"+ timeStamp + ".jpg");
    } else if(type == MEDIA_TYPE_VIDEO) {
        mediaFile = new File(mediaStorageDir.getPath() + File.separator +
        "VID_"+ timeStamp + ".mp4");
    } else {
        return null;
    }

    return mediaFile;
}
```


注意：Environment.getExternalStoragePublicDirectory（）在Android 2.2（API级别8）或更高版本中可用。 如果您使用早期版本的Android定位设备，请改用Environment.getExternalStorageDirectory（）。 有关详细信息，请参阅[保存共享文件](https://developer.android.google.cn/guide/topics/data/data-storage.html#SavingSharedFiles)。






要使URI支持工作配置文件，首先将文件URI转换为内容URI。 然后，将内容URI添加到Intent的EXTRA_OUTPUT中。





有关在Android设备上保存文件的更多信息，请参阅[数据存储](https://developer.android.google.cn/guide/topics/data/data-storage.html)。








七、相机功能

Android支持您可以使用相机应用程序控制的各种相机功能，如图片格式，闪光模式，对焦设置等等。 本节列出了常见的相机功能，并简要讨论了如何使用它们。 大多数相机功能可以通过Camera.Parameters对象进行访问和设置。 但是，有几个重要的功能需要在Camera.Parameters中的简单设置。 以下部分将介绍这些功能：



- [Metering and focus areas](https://developer.android.google.cn/guide/topics/media/camera.html#metering-focus-areas)
- [Face detection](https://developer.android.google.cn/guide/topics/media/camera.html#face-detection)
- [Time lapse video](https://developer.android.google.cn/guide/topics/media/camera.html#time-lapse-video)

有关如何使用通过Camera.Parameters控制的功能的一般信息，请查看使用相机功能部分。 有关如何使用通过摄像机参数对象控制的功能的更多详细信息，请参见下面的功能列表中的链接到API参考文档。






表1.由引入它们的Android API级别排序的常见相机功能。



|Feature|API Level|Description|
|----|----|----|
|[Face Detection](https://developer.android.google.cn/guide/topics/media/camera.html#face-detection)|14|Identify human faces within a picture and use them for focus, metering and white balance|
|[Metering Areas](https://developer.android.google.cn/guide/topics/media/camera.html#metering-focus-areas)|14|Specify one or more areas within an image for calculating white balance|
|[Focus Areas](https://developer.android.google.cn/guide/topics/media/camera.html#metering-focus-areas)|14|Set one or more areas within an image to use for focus|
|```[White Balance Lock](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setAutoWhiteBalanceLock(boolean))```|14|Stop or start automatic white balance adjustments|
|`[Exposure Lock](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setAutoExposureLock(boolean))`|14|Stop or start automatic exposure adjustments|
|```[Video Snapshot](https://developer.android.google.cn/reference/android/hardware/Camera.html#takePicture(android.hardware.Camera.ShutterCallback,%20android.hardware.Camera.PictureCallback,%20android.hardware.Camera.PictureCallback))```|14|Take a picture while shooting video (frame grab)|
|[Time Lapse Video](https://developer.android.google.cn/guide/topics/media/camera.html#time-lapse-video)|11|Record frames with set delays to record a time lapse video|
|`[Multiple Cameras](https://developer.android.google.cn/reference/android/hardware/Camera.html#open(int))`|9|Support for more than one camera on a device, including front-facing and back-facing cameras|
|`[Focus Distance](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#getFocusDistances(float%5B%5D))`|9|Reports distances between the camera and objects that appear to be in focus|
|`[Zoom](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setZoom(int))`|8|Set image magnification|
|`[Exposure Compensation](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setExposureCompensation(int))`|8|Increase or decrease the light exposure level|
|`[GPS Data](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setGpsLatitude(double))`|5|Include or omit geographic location data with the image|
|`[White Balance](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setWhiteBalance(java.lang.String))`|5|Set the white balance mode, which affects color values in the captured image|
|`[Focus Mode](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setFocusMode(java.lang.String))`|5|Set how the camera focuses on a subject such as automatic, fixed, macro or infinity|
|`[Scene Mode](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setSceneMode(java.lang.String))`|5|Apply a preset mode for specific types of photography situations such as night, beach, snow or candlelight scenes|
|`[JPEG Quality](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setJpegQuality(int))`|5|Set the compression level for a JPEG image, which increases or decreases image output file quality and size|
|`[Flash Mode](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setFlashMode(java.lang.String))`|5|Turn flash on, off, or use automatic setting|
|`[Color Effects](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setColorEffect(java.lang.String))`|5|Apply a color effect to the captured image such as black and white, sepia tone or negative.|
|`[Anti-Banding](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setAntibanding(java.lang.String))`|5|Reduces the effect of banding in color gradients due to JPEG compression|
|`[Picture Format](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setPictureFormat(int))`|1|Specify the file format for the picture|
|`[Picture Size](https://developer.android.google.cn/reference/android/hardware/Camera.Parameters.html#setPictureSize(int,%20int))`|1|Specify the pixel dimensions of the saved picture|

注意：由于硬件差异和软件实现，并非所有设备都支持这些功能。 有关检查应用程序运行的设备上功能的可用性的信息，请参阅检查功能可用性。




一）、检查功能可用性

在Android设备上设置使用相机功能时，首先要了解的是，并非所有设备都支持所有相机功能。 此外，支持特定功能的设备可能会将其支持到不同的级别或不同的选项。 因此，在开发相机应用程序时，您的决策过程的一部分是决定要支持的相机功能以及什么级别。 做出决定后，您应该计划在相机应用程序中包含代码，以检查设备硬件是否支持这些功能，如果功能不可用，则会正常失败。





您可以通过获取摄像机参数对象的实例并检查相关方法来检查摄像机功能的可用性。 以下代码示例显示如何获取Camera.Parameters对象，并检查相机是否支持自动对焦功能：




```java
// get Camera parameters
Camera.Parameters params = mCamera.getParameters();

List<String> focusModes = params.getSupportedFocusModes();
if (focusModes.contains(Camera.Parameters.FOCUS_MODE_AUTO)) {
  // Autofocus mode is supported
}
```


您可以使用上面显示的技术来获得大多数相机功能。 Camera.Parameters对象提供了一个getSupported ...（），是... Supported（）或getMax ...（）方法来确定是否（以及在什么程度上）功能被支持。






如果您的应用程序需要某些相机功能才能正常运行，则可以通过添加应用程序清单来要求它们。 当您声明使用特定的相机功能（如闪光灯和自动对焦）时，Google Play将您的应用程序限制为不支持这些功能的设备上。 有关可以在应用程序清单中声明的摄像头功能的列表，请参阅[清单功能参考](https://developer.android.google.cn/guide/topics/manifest/uses-feature-element.html#hw-features)。





二）、使用相机功能

大多数相机功能都使用Camera.Parameters对象进行激活和控制。 您可以通过首先获取Camera对象的实例，调用getParameters（）方法，更改返回的参数对象，然后将其设置回相机对象来获取此对象，如以下示例代码所示：




```java
// get Camera parameters
Camera.Parameters params = mCamera.getParameters();
// set the focus mode
params.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
// set Camera parameters
mCamera.setParameters(params);
```


这种技术适用于几乎所有的相机功能，大多数参数可以在获取Camera对象的实例后随时更改。 在应用程序的相机预览中，用户对参数的更改通常是可见的。 在软件方面，参数更改可能需要几帧才能实际生效，因为相机硬件处理新指令，然后发送更新的图像数据。






重要提示：某些相机功能无法随意更改。 特别是，更改相机预览的大小或方向需要先停止预览，更改预览大小，然后重新启动预览。 从Android 4.0（API级别14）开始，预览方向可以更改，无需重新启动预览。





其他相机功能需要更多的代码才能实现，其中包括：



- 计量和焦点领域

- 面部检测

- 时间延迟视频

以下部分将介绍如何实现这些功能的快速概述。





三）、计量和焦点领域

在某些摄影场景中，自动对焦和测光可能无法产生所需的结果。 从Android 4.0（API 14级）开始，您的相机应用程序可以提供额外的控件，允许您的应用或用户指定图像中用于确定焦点或光线级别设置的区域，并将这些值传递到相机硬件以用于捕获
 图像或视频。





用于测光和对焦的区域与其他摄像机功能非常相似，您可以通过Camera.Parameters对象中的方法控制它们。 以下代码演示了为相机实例设置两个测光区域：



```java
// Create an instance of Camera
mCamera = getCameraInstance();

// set Camera parameters
Camera.Parameters params = mCamera.getParameters();

if (params.getMaxNumMeteringAreas() > 0){ // check that metering areas are supported
    List<Camera.Area> meteringAreas = new ArrayList<Camera.Area>();

    Rect areaRect1 = new Rect(-100, -100, 100, 100);    // specify an area in center of image
    meteringAreas.add(new Camera.Area(areaRect1, 600)); // set weight to 60%
    Rect areaRect2 = new Rect(800, -1000, 1000, -800);  // specify an area in upper right of image
    meteringAreas.add(new Camera.Area(areaRect2, 400)); // set weight to 40%
    params.setMeteringAreas(meteringAreas);
}

mCamera.setParameters(params);
```


Camera.Area对象包含两个数据参数：一个用于指定摄像机视场范围内的区域的Rect对象和一个权值，该值指示摄像机在测光或对焦计算时应该给出该区域的重要程度。





Camera.Area对象中的Rect字段描述了映射到2000 x 2000单位格网格的矩形。 坐标-1000，-1000表示摄像机图像的左上角，坐标1000,1000表示摄像机图像的右下角，如下图所示。


![](https://img-blog.csdn.net/20170506103248694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRXhwbG9yZXJfZGF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图1.红线表示在相机预览中指定Camera.Area的坐标系。 蓝色框显示具有Rect值333,333,667,667的相机区域的位置和形状。





该坐标系的边界总是对应于在相机预览中可见的图像的外边缘，并且不缩小或随缩放级别展开。 类似地，使用Camera.setDisplayOrientation（）的图像预览的旋转不会重新映射坐标系。






四）、面部检测


对于包含人物的图片，脸部通常是图片中最重要的部分，并且应该用于在拍摄图像时确定对焦和白平衡。 Android 4.0（API Level 14）框架提供用于识别人脸和使用脸部识别技术计算图像设置的API。


注意：当脸部检测功能正在运行时，setWhiteBalance（String），setFocusAreas（List <Camera.Area>）和setMeteringAreas（List <Camera.Area>）没有任何作用。





使用相机应用程序中的人脸检测功能需要几个一般步骤：


1、检查设备是否支持人脸检测。

2、创建一个人脸检测侦听器。

3、将面部检测侦听器添加到您的相机对象。

4、预览后开始脸部检测（每次预览重启后）。




并不是所有设备都支持人脸检测功能。 您可以通过调用getMaxNumDetectedFaces（）来检查此功能是否受支持。 此检查的示例显示在下面的startFaceDetection（）示例方法中。





为了得到通知并对脸部的检测做出响应，您的相机应用程序必须设置一个用于人脸检测事件的侦听器。 为此，您必须创建一个实现Camera.FaceDetectionListener接口的侦听器类，如下面的示例代码所示。




```java
class MyFaceDetectionListener implements Camera.FaceDetectionListener {

    @Override
    public void onFaceDetection(Face[] faces, Camera camera) {
        if (faces.length > 0){
            Log.d("FaceDetection", "face detected: "+ faces.length +
                    " Face 1 Location X: " + faces[0].rect.centerX() +
                    "Y: " + faces[0].rect.centerY() );
        }
    }
}
```


创建此类后，您将其设置为应用程序的Camera对象，如下面的示例代码所示：





```java
mCamera.setFaceDetectionListener(new MyFaceDetectionListener());
```


您的应用程序必须在每次启动（或重新启动）相机预览时启动人脸检测功能。 创建一种开始面部检测的方法，以便您可以根据需要调用它，如下面的示例代码所示。





```java
public void startFaceDetection(){
    // Try starting Face Detection
    Camera.Parameters params = mCamera.getParameters();

    // start face detection only *after* preview has started
    if (params.getMaxNumDetectedFaces() > 0){
        // camera supports face detection, so can start it:
        mCamera.startFaceDetection();
    }
}
```


每次启动（或重新启动）相机预览时，都必须开始面部检测。 如果您使用“创建预览”类中显示的预览类，请在预览类中将startFaceDetection（）方法添加到surfaceCreated（）和surfaceChanged（）方法，如下面的示例代码所示。





```java
public void surfaceCreated(SurfaceHolder holder) {
    try {
        mCamera.setPreviewDisplay(holder);
        mCamera.startPreview();

        startFaceDetection(); // start face detection feature

    } catch (IOException e) {
        Log.d(TAG, "Error setting camera preview: " + e.getMessage());
    }
}

public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

    if (mHolder.getSurface() == null){
        // preview surface does not exist
        Log.d(TAG, "mHolder.getSurface() == null");
        return;
    }

    try {
        mCamera.stopPreview();

    } catch (Exception e){
        // ignore: tried to stop a non-existent preview
        Log.d(TAG, "Error stopping camera preview: " + e.getMessage());
    }

    try {
        mCamera.setPreviewDisplay(mHolder);
        mCamera.startPreview();

        startFaceDetection(); // re-start face detection feature

    } catch (Exception e){
        // ignore: tried to stop a non-existent preview
        Log.d(TAG, "Error starting camera preview: " + e.getMessage());
    }
}
```


注意：在调用startPreview（）之后，请记住调用此方法。 不要尝试在相机应用程序主要活动的onCreate（）方法中启动人脸检测，因为在应用程序的执行过程中，预览不可用。






五）、时间延迟视频

时间延迟视频允许用户创建组合几秒或几分钟的照片组合的视频剪辑。 此功能使用MediaRecorder记录时间序列的图像。


要使用MediaRecorder录制时间延迟视频，您必须配置录像机对象，就像录制正常视频一样，将每秒拍摄的帧数设置为低数字，并使用其中一个时间流逝的质量设置，如代码示例下面所示。




```java
// Step 3: Set a CamcorderProfile (requires API Level 8 or higher)
mMediaRecorder.setProfile(CamcorderProfile.get(CamcorderProfile.QUALITY_TIME_LAPSE_HIGH));
...
// Step 5.5: Set the video capture rate to a low number
mMediaRecorder.setCaptureRate(0.1); // capture a frame every 10 seconds
```


这些设置必须作为MediaRecorder的较大配置过程的一部分来完成。 有关完整配置代码示例，请参阅配置MediaRecorder。 一旦配置完成，您就可以开始录像，就像录制正常视频剪辑一样。 有关配置和运行MediaRecorder的更多信息，请参阅[捕获视频](https://developer.android.google.cn/guide/topics/media/camera.html#capture-video)。








