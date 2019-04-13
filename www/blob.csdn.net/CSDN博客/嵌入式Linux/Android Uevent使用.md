
# Android Uevent使用 - 嵌入式Linux - CSDN博客

2015年06月18日 14:27:11[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1191


kernel:

```python
diff --git a/android-lte/boot.img b/android-lte/boot.img
index 8a1bdf2..b688dcf 100644
Binary files a/android-lte/boot.img and b/android-lte/boot.img differ
diff --git a/android-lte/kernel/drivers/usb/core/hub.c b/android-lte/kernel/drivers/usb/core/hub.c
index c0f6f45..c432f95 100755
--- a/android-lte/kernel/drivers/usb/core/hub.c
+++ b/android-lte/kernel/drivers/usb/core/hub.c
@@ -57,6 +57,10 @@ EXPORT_SYMBOL(HostComplianceTest);
 int HostTest;
 EXPORT_SYMBOL(HostTest);
 #endif
+static struct class *zte_usbcamera_class = NULL;//weiqifa modify add
+static dev_t usbcamera_device_no;//weiqifa modify add
+struct device *usbcamera_class_dev;//weiqifa modify add
+
 
 extern int extern_control_lte_suspend(int value);
 /* if we are in debug mode, always announce new devices */
@@ -1764,9 +1768,9 @@ void usb_disconnect(struct usb_device **pdev)
 	if(le16_to_cpu(udev->descriptor.idVendor) == 0x0c45){
 		char *envp[2];
 		int ret;
-		envp[0]= "USBCAMERA_PULLOUT";
+		envp[0]= "USBCAMERA=pullout";
 		envp[1]=NULL;
-		ret = kobject_uevent_env(&udev->dev.kobj, KOBJ_CHANGE,envp);
+		ret = kobject_uevent_env(&usbcamera_class_dev->kobj, KOBJ_CHANGE,envp);
 		if(ret<0){
 			printk("%s kobject error\n",__func__);	
 		}else{
@@ -1823,15 +1827,15 @@ static void announce_device(struct usb_device *udev)
 	if(le16_to_cpu(udev->descriptor.idVendor) == 0x0c45){
 		char *envp[2];
 		int ret;
-		envp[0]= "USBCAMERA_INSERT";
+		envp[0]= "USBCAMERA=pushin";
 		envp[1]=NULL;
-		ret = kobject_uevent_env(&udev->dev.kobj, KOBJ_CHANGE,envp);
+		ret = kobject_uevent_env(&usbcamera_class_dev->kobj, KOBJ_CHANGE,envp);
 		if(ret<0){
 			printk("%s kobject error\n",__func__);	
 		}else{
 			printk("%s new usb camera found seccess\n",__func__);
 		}
-		printk("%s\n",kobject_get_path(&udev->dev.kobj,GFP_KERNEL));
+		printk("%s\n",kobject_get_path(&usbcamera_class_dev->kobj,GFP_KERNEL));
 	}else{
 		//zte_modem_enable = 0;
 		printk("%s the new USB devices is not usb camera\n",__func__);
@@ -4378,6 +4382,21 @@ int usb_hub_init(void)
 		class_destroy(zte_modem_class);
 	}
 #endif	
+//weiqifa modify for usbcamera 现在是自己生成一个目录，就是一个sys文件，也可以用原来的kobject的对像
+	zte_usbcamera_class = class_create(THIS_MODULE,"zte_usbcamera_class");
+	if (IS_ERR(zte_usbcamera_class)) {
+			   pr_err("%s: class_create() failed zte_usbcamera_class\n", __func__);
+			class_destroy(zte_usbcamera_class);
+	}
+	//add device to class 
+	usbcamera_class_dev = device_create(zte_usbcamera_class, NULL,
+				  usbcamera_device_no, NULL, "usbcamera");
+	if (!usbcamera_class_dev) {
+		pr_err("%s: class_device_create failed usbcamera \n", __func__);
+		device_destroy(zte_usbcamera_class, usbcamera_device_no);
+	}
+
+//weiqifa modify for usbcamera end
 	if (usb_register(&hub_driver) < 0) {
 		printk(KERN_ERR "%s: can't register hub driver\n",
 			usbcore_name);
diff --git a/android-lte/kernel/lib/kobject_uevent.c b/android-lte/kernel/lib/kobject_uevent.c
index 50f08ef..a9d89cd 100755
--- a/android-lte/kernel/lib/kobject_uevent.c
+++ b/android-lte/kernel/lib/kobject_uevent.c
@@ -201,7 +201,7 @@ int kobject_uevent_env(struct kobject *kobj, enum kobject_action action,
 		retval = -ENOENT;
 		goto exit;
 	}
-	printk("%s %s\n",__func__,devpath);
+	//printk("%s %s\n",__func__,devpath);
 	/* default keys */
 	retval = add_uevent_var(env, "ACTION=%s", action_string);
 	if (retval)
@@ -271,12 +271,13 @@ int kobject_uevent_env(struct kobject *kobj, enum kobject_action action,
 			/* add header */
 			scratch = skb_put(skb, len);
 			sprintf(scratch, "%s@%s", action_string, devpath);
-//这里可以看得出打印的目录和值，这些值就是上层在framework里面可以监听的东西
+			printk("weiqifa %s\n",scratch);
 			/* copy keys to our continuous event payload buffer */
 			for (i = 0; i < env->envp_idx; i++) {
 				len = strlen(env->envp[i]) + 1;
 				scratch = skb_put(skb, len);
 				strcpy(scratch, env->envp[i]);
+				printk("weiqifa2 %s\n",scratch);
 			}
 
 			NETLINK_CB(skb).dst_group = 1;
```
上层接收uevent发送广播
```python
diff --git a/android-lte/frameworks/base/services/java/com/android/server/usb/UsbDeviceManager.java b/android-lte/frameworks/base/services/java/com/android/server/usb/UsbDeviceManager.java
index 8e794e5..3e33792 100755
--- a/android-lte/frameworks/base/services/java/com/android/server/usb/UsbDeviceManager.java
+++ b/android-lte/frameworks/base/services/java/com/android/server/usb/UsbDeviceManager.java
@@ -74,7 +74,7 @@ import com.qrd.plugin.feature_query.FeatureQuery;
 public class UsbDeviceManager {
 
     private static final String TAG = UsbDeviceManager.class.getSimpleName();
-    private static final boolean DEBUG = true;
+    private static final boolean DEBUG = false;
 
     private static final String USB_HUB_MATCH =
             "DEVPATH=/devices/virtual/goso_hub/hub0";
@@ -96,6 +96,11 @@ public class UsbDeviceManager {
             "/sys/class/android_usb/android0/f_rndis/ethaddr";
     private static final String AUDIO_SOURCE_PCM_PATH =
             "/sys/class/android_usb/android0/f_audio_source/pcm";
+			
+	private static final String USB_CAMERA_MATCH = "DEVPATH=/devices/virtual/zte_usbcamera_class/usbcamera";
+		//"DEVPATH=/devices/platform/sm_hsusb_host.0/usb1/1-1/1-1.2";
+	
+	private static final String USB_CAMERA_PATH = "/devices/platform/sm_hsusb_host.0/usb1/1-1/1-1.2";
 
     private static final String USB_PLUG_PATH = 
             "/sys/class/power_supply/usb/online";
@@ -113,6 +118,7 @@ public class UsbDeviceManager {
     private static final int MSG_USB_PORT_SWITCH = 6;
     private static final int MSG_UPDATE_HUB_MODE_STATE = 7;
     private static final int MSG_UPDATE_HUB_PLUG_STATE = 8;
+	private static final int MSG_USB_CAMERA = 9;//usb camera msg
 
     // Delay for debouncing USB disconnects.
     // We often get rapid connect/disconnect events when enabling USB functions,
@@ -165,9 +171,13 @@ public class UsbDeviceManager {
             String hub_plug = event.get("HUB_PLUG");
             String state = event.get("USB_STATE");
             String accessory = event.get("ACCESSORY");
+			String usb_camera = event.get("USBCAMERA");
             if (hub_mode != null) {
                 mHandler.updateModeState(hub_mode);
             }
+			if (usb_camera != null) {
+			        mHandler.updateCameraState(usb_camera);
+			}
             if (hub_plug != null) {
                 mHandler.updatePlugState(hub_plug);
             }
@@ -402,6 +412,7 @@ public class UsbDeviceManager {
                 mUEventObserver.startObserving(USB_STATE_MATCH);
                 mUEventObserver.startObserving(ACCESSORY_START_MATCH);
                 mUEventObserver.startObserving(USB_HUB_MATCH);
+				mUEventObserver.startObserving(USB_CAMERA_MATCH);
 
                 mContext.registerReceiver(mBootCompletedReceiver,
                         new IntentFilter(Intent.ACTION_BOOT_COMPLETED));
@@ -452,6 +463,22 @@ public class UsbDeviceManager {
 
             sendMessage(MSG_UPDATE_HUB_MODE_STATE, mode == 1);
         }
+		
+		//usb camera pluged event
+        public void updateCameraState(String usb_camera) {
+            boolean pluge = false;
+            if (DEBUG) Slog.e(TAG, "updateCameraState:" + usb_camera);
+            if ("PLUGIN".equals(usb_camera)) {
+                pluge = true;
+            } else if ("PLUGOUT".equals(usb_camera)) {
+                pluge = false;
+            } else {
+                Slog.e(TAG, "unknown usb camera state " + usb_camera);
+                return;
+            }
+
+            sendMessage(MSG_USB_CAMERA, pluge);
+        }
 
         public void updatePlugState(String state) {
             int plug;
@@ -682,6 +709,16 @@ public class UsbDeviceManager {
 			intent.putExtra("usb_plug",isUSBConnection);
             mContext.sendStickyBroadcast(intent);
         }
+		
+		//camera pluged 
+	    private void sendUsbCameraBroadcast(boolean pluged) {
+            // send a sticky broadcast containing current USB state
+            Intent intent = new Intent("com.ztegota.action.usb_camera");
+            intent.addFlags(Intent.FLAG_RECEIVER_REPLACE_PENDING);
+            intent.putExtra("usb_camera", pluged);
+			if (DEBUG) Log.v(TAG, "sendUsbCameraBroadcast: " + intent.toString());
+            mContext.sendStickyBroadcast(intent);
+        }
 
         private void updateUsbState() {
             // send a sticky broadcast containing current USB state
@@ -835,6 +872,10 @@ public class UsbDeviceManager {
                         SystemProperties.set("persist.sys.usbfactory",  "0");
                     }
                     break;
+			    case MSG_USB_CAMERA:
+				    boolean pluged = (msg.arg1 == 1);
+					Log.v(TAG, "MSG_USB_CAMERA, pluged = " + pluged);
+					sendUsbCameraBroadcast(pluged);
             }
         }
```



