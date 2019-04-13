
# 打印时间（MS级别） - 嵌入式Linux - CSDN博客

2015年01月26日 17:06:41[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：509



```python
--- a/android-lte/packages/apps/Camera/src/com/android/camera/Camera.java
+++ b/android-lte/packages/apps/Camera/src/com/android/camera/Camera.java
@@ -64,6 +64,9 @@ import android.widget.ImageView;
 import android.widget.LinearLayout;
 import android.widget.TextView;
 import android.widget.Toast;
+import java.text.SimpleDateFormat;//owen.wei modify
+import java.util.Date;//owen.wei modify
+import android.text.format.DateFormat;//owen.wei modify
 import com.android.camera.ui.CameraPicker;
 import com.android.camera.ui.FaceView;
@@ -397,7 +400,13 @@ public class Camera extends ActivityBase implements FocusManager.Listener,
                 // We need to check whether the activity is paused before long
                 // operations to ensure that onPause() can be done ASAP.
                 if (mCancelled) return;
+                //owen.wei modify add start
+                                                               SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
+                                                               Date   curDate   =   new   Date(System.currentTimeMillis());
+
                 mCameraDevice = Util.openCamera(Camera.this, mCameraId);
+
+
                 mParameters = mCameraDevice.getParameters();
                 // Wait until all the initialization needed by startPreview are
                 // done.
@@ -410,6 +419,10 @@ public class Camera extends ActivityBase implements FocusManager.Listener,
                 mHandler.sendEmptyMessage(CAMERA_OPEN_DONE);
                 if (mCancelled) return;
                 startPreview();
+                Date   endDate   =   new   Date(System.currentTimeMillis());
+                                                               long diff = endDate.getTime() - curDate.getTime();
+                                                               Log.e(TAG, "curDate:"+String.valueOf(curDate)+"\nendDate:"+String.valueOf(endDate)+"\ndiff
+                //owen.wei modify add end
                 mHandler.sendEmptyMessage(START_PREVIEW_DONE);
                 mOnResumeTime = SystemClock.uptimeMillis();
                 mHandler.sendEmptyMessage(CHECK_DISPLAY_ROTATION);
```


