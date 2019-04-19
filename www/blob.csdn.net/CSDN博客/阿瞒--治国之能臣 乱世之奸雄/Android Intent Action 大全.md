# Android Intent Action 大全 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2017年03月10日 11:44:59[baofenny](https://me.csdn.net/jiaminbao)阅读数：723标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[学习实例开发](https://blog.csdn.net/jiaminbao/article/category/1158383)
**1.Intent的用法：**
（1）**Action跳转**
1、 使用Action跳转，当程序AndroidManifest.xml中某一个 Activity的IntentFilter定义了包含Action，如果恰好与目标Action匹配，且其IntentFilter中没有定义其它的Type或Category过滤条件，那么就正好匹配了。如果手机中有两个以上的Action程序匹配，那么就会弹出一个对话可框来提示说明。例如打开一个网址，弹出可选对话框：
![](https://img-my.csdn.net/uploads/201211/30/1354250463_8508.png)
Action 的值在[Android](http://lib.csdn.net/base/android)中有很多预定义，如果想直接转到自己定义的Intent接收者，可以在接收者的IntentFilter 中加入一个自定义的Action值（同时要设定
 Category值为"android.intent.category.DEFAULT"），在你的Intent中设定该值为Intent的 Action就直接能跳转到你自己的Intent接收者中，因为这个Action在系统中是唯一的。
2、 data/type，可以用Uri 来做为data，比如Uri uri = Uri.parse(“http://blog.csdn.net/sunboy_2050”[);](/;) Intent
 i = new Intent(Intent.ACTION_VIEW, uri); 手机的Intent分发过程中，会根据“http://blog.csdn.net/sunboy_2050” 的scheme判断出数据类型type。手机中安装的所有Brower都能匹配它，在Brower的Manifest.xml中的IntenFilter中首先有ACTION_VIEW 的Action能处理http的type
3、 Category分类，一般不要去在Intent中设置它，如果你写Intent的接收者就在Manifest.xml的Activity的 IntentFilter中包含android.category.DEFAULT，这样所有不设置 Category（Intent.addCategory(String c);）的Intent都会与这个Category匹配。
4,extras 附加信息，是其它所有附加信息的集合。使用extras可以为组件提供扩展信息，比如，如果要执行“发送电子邮件”这个动作，可以将电子邮件的标题、正文等保存在extras里，传给电子邮件发送组件。
（2）**用类名跳转**
    Intent负责对应用中一次操作的动作、动作涉及的数据、附加数据进行描述，Android则根据此Intent的描述， 负责找到对应的组件，将 Intent传递给调用的组件，并完成组件的调用。Intent在这里起着实现调用者与被调用者之间的解耦作用。Intent传递过程中，要找 到目标消费者（另一个Activity，IntentReceiver，Service），也就是Intent的响应者。
```
Intent intent = new Intent();
intent.setClass(context, targetActivy.class);  // 或者直接用 Intent intent = new Intent(context, targetActivity.class);
startActivity(intent);
```
不过注意用类名跳转，需要在AndroidManifest.xml中申明activity  
<activity android:name="targetActivity"></activity>
**2. 几种Intent的用法**
android 中intent是经常要用到的。不管是页面牵转，还是传递数据，或是调用外部程序，系统功能都要用到intent，下面是一些常用intent示例：
1.从google搜索内容 
Intent intent = new Intent(); 
intent.setAction(Intent.ACTION_WEB_SEARCH); 
intent.putExtra(SearchManager.QUERY,"searchString") 
startActivity(intent); 
2.浏览网页 
Uri uri = Uri.parse("http://www.google.com"); 
Intent it  = new Intent(Intent.ACTION_VIEW,uri); 
startActivity(it); 
3.显示地图 
Uri uri = Uri.parse("geo:38.899533,-77.036476"); 
Intent it = new Intent(Intent.Action_VIEW,uri); 
startActivity(it); 
4.路径规划 
Uri uri = Uri.parse("http://maps.google.com/maps?f=dsaddr=startLat%20startLng&daddr=endLat%20endLng&hl=en"); 
Intent it = new Intent(Intent.ACTION_VIEW,URI); 
startActivity(it); 
5.拨打电话 
Uri uri = Uri.parse("tel:xxxxxx"); 
Intent it = new Intent(Intent.ACTION_DIAL, uri);   
startActivity(it); 
6.调用发短信的程序 
Intent it = new Intent(Intent.ACTION_VIEW);    
it.putExtra("sms_body", "The SMS text");    
it.setType("vnd.android-dir/mms-sms");    
startActivity(it); 
7.发送短信 
Uri uri = Uri.parse("smsto:0800000123");    
Intent it = new Intent(Intent.ACTION_SENDTO, uri);    
it.putExtra("sms_body", "The SMS text");    
startActivity(it); 
String body="this is sms demo"; 
Intent mmsintent = new Intent(Intent.ACTION_SENDTO, Uri.fromParts("smsto", number, null)); 
mmsintent.putExtra(Messaging.KEY_ACTION_SENDTO_MESSAGE_BODY, body); 
mmsintent.putExtra(Messaging.KEY_ACTION_SENDTO_COMPOSE_MODE, true); 
mmsintent.putExtra(Messaging.KEY_ACTION_SENDTO_EXIT_ON_SENT, true); 
startActivity(mmsintent); 
8.发送彩信 
Uri uri = Uri.parse("content://media/external/images/media/23");    
Intent it = new Intent(Intent.ACTION_SEND);    
it.putExtra("sms_body", "some text");    
it.putExtra(Intent.EXTRA_STREAM, uri);    
it.setType("image/png");    
startActivity(it); 
StringBuilder sb = new StringBuilder(); 
sb.append("file://"); 
sb.append(fd.getAbsoluteFile()); 
Intent intent = new Intent(Intent.ACTION_SENDTO, Uri.fromParts("mmsto", number, null)); 
// Below extra datas are all optional. 
intent.putExtra(Messaging.KEY_ACTION_SENDTO_MESSAGE_SUBJECT, subject); 
intent.putExtra(Messaging.KEY_ACTION_SENDTO_MESSAGE_BODY, body); 
intent.putExtra(Messaging.KEY_ACTION_SENDTO_CONTENT_URI, sb.toString()); 
intent.putExtra(Messaging.KEY_ACTION_SENDTO_COMPOSE_MODE, composeMode); 
intent.putExtra(Messaging.KEY_ACTION_SENDTO_EXIT_ON_SENT, exitOnSent); 
startActivity(intent); 
9.发送Email 
Uri uri = Uri.parse("mailto:xxx@abc.com"); 
Intent it = new Intent(Intent.ACTION_SENDTO, uri); 
startActivity(it); 
Intent it = new Intent(Intent.ACTION_SEND);    
it.putExtra(Intent.EXTRA_EMAIL, "me@abc.com");    
it.putExtra(Intent.EXTRA_TEXT, "The email body text");    
it.setType("text/plain");    
startActivity(Intent.createChooser(it, "Choose Email Client")); 
Intent it=new Intent(Intent.ACTION_SEND);      
String[] tos={"me@abc.com"};      
String[] ccs={"you@abc.com"};      
it.putExtra(Intent.EXTRA_EMAIL, tos);      
it.putExtra(Intent.EXTRA_CC, ccs);      
it.putExtra(Intent.EXTRA_TEXT, "The email body text");      
it.putExtra(Intent.EXTRA_SUBJECT, "The email subject text");      
it.setType("message/rfc822");      
startActivity(Intent.createChooser(it, "Choose Email Client"));    
Intent it = new Intent(Intent.ACTION_SEND);    
it.putExtra(Intent.EXTRA_SUBJECT, "The email subject text");    
it.putExtra(Intent.EXTRA_STREAM, "file:///sdcard/mysong.mp3");    
sendIntent.setType("audio/mp3");    
startActivity(Intent.createChooser(it, "Choose Email Client")); 
10.播放多媒体   
Intent it = new Intent(Intent.ACTION_VIEW); 
Uri uri = Uri.parse("file:///sdcard/song.mp3"); 
it.setDataAndType(uri, "audio/mp3"); 
startActivity(it); 
Uri uri = Uri.withAppendedPath(MediaStore.Audio.Media.INTERNAL_CONTENT_URI, "1");    
Intent it = new Intent(Intent.ACTION_VIEW, uri);    
startActivity(it); 
11.uninstall apk 
Uri uri = Uri.fromParts("package", strPackageName, null);    
Intent it = new Intent(Intent.ACTION_DELETE, uri);    
startActivity(it); 
12.install apk 
Uri installUri = Uri.fromParts("package", "xxx", null); 
returnIt = new Intent(Intent.ACTION_PACKAGE_ADDED, installUri); 
13. 打开照相机
    <1>Intent i = new Intent(Intent.ACTION_CAMERA_BUTTON, null); 
           this.sendBroadcast(i);
     <2>long dateTaken = System.currentTimeMillis(); 
            String name = createName(dateTaken) + ".jpg"; 
            fileName = folder + name; 
            ContentValues values = new ContentValues(); 
            values.put(Images.Media.TITLE, fileName); 
            values.put("_data", fileName); 
            values.put(Images.Media.PICASA_ID, fileName); 
            values.put(Images.Media.DISPLAY_NAME, fileName); 
            values.put(Images.Media.DESCRIPTION, fileName); 
            values.put(Images.ImageColumns.BUCKET_DISPLAY_NAME, fileName); 
            Uri photoUri = getContentResolver().insert( 
                    MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values); 
            Intent inttPhoto = new Intent(MediaStore.ACTION_IMAGE_CAPTURE); 
            inttPhoto.putExtra(MediaStore.EXTRA_OUTPUT, photoUri); 
            startActivityForResult(inttPhoto, 10); 
14.从gallery选取图片 
  Intent i = new Intent(); 
            i.setType("image/*"); 
            i.setAction(Intent.ACTION_GET_CONTENT); 
            startActivityForResult(i, 11); 
15. 打开录音机 
   Intent mi = new Intent(Media.RECORD_SOUND_ACTION); 
            startActivity(mi); 
16.显示应用详细列表       
Uri uri = Uri.parse("market://details?id=app_id");         
Intent it = new Intent(Intent.ACTION_VIEW, uri);         
startActivity(it);         
//where app_id is the application ID, find the ID          
//by clicking on your application on Market home          
//page, and notice the ID from the address bar      
刚才找app id未果，结果发现用package name也可以 
Uri uri = Uri.parse("market://details?id=<packagename>"); 
这个简单多了 
17寻找应用       
Uri uri = Uri.parse("market://search?q=pname:pkg_name");         
Intent it = new Intent(Intent.ACTION_VIEW, uri);         
startActivity(it); 
//where pkg_name is the full package path for an application       
18打开联系人列表 
            <1>            
           Intent i = new Intent(); 
           i.setAction(Intent.ACTION_GET_CONTENT); 
           i.setType("vnd.android.cursor.item/phone"); 
           startActivityForResult(i, REQUEST_TEXT); 
            <2> 
            Uri uri = Uri.parse("content://contacts/people"); 
            Intent it = new Intent(Intent.ACTION_PICK, uri); 
            startActivityForResult(it, REQUEST_TEXT); 
19 打开另一程序 
Intent i = new Intent(); 
            ComponentName cn = new ComponentName("com.yellowbook.android2", 
                    "com.yellowbook.android2.AndroidSearch"); 
            i.setComponent(cn); 
            i.setAction("android.intent.action.MAIN"); 
            startActivityForResult(i, RESULT_OK); 
20.调用系统编辑添加联系人（高版本SDK有效）：
Intent it = newIntent(Intent.ACTION_INSERT_OR_EDIT);
               it.setType("vnd.android.cursor.item/contact");
                //it.setType(Contacts.CONTENT_ITEM_TYPE);
                it.putExtra("name","myName");
               it.putExtra(android.provider.Contacts.Intents.Insert.COMPANY,  "organization");
               it.putExtra(android.provider.Contacts.Intents.Insert.EMAIL,"email");
                it.putExtra(android.provider.Contacts.Intents.Insert.PHONE,"homePhone");
                it.putExtra(android.provider.Contacts.Intents.Insert.SECONDARY_PHONE,
                               "mobilePhone");
                it.putExtra(  android.provider.Contacts.Intents.Insert.TERTIARY_PHONE,
                               "workPhone");
               it.putExtra(android.provider.Contacts.Intents.Insert.JOB_TITLE,"title");
                startActivity(it);
21.调用系统编辑添加联系人（全有效）：
Intent intent = newIntent(Intent.ACTION_INSERT_OR_EDIT);
           intent.setType(People.CONTENT_ITEM_TYPE);
           intent.putExtra(Contacts.Intents.Insert.NAME, "My Name");
           intent.putExtra(Contacts.Intents.Insert.PHONE, "+1234567890");
           intent.putExtra(Contacts.Intents.Insert.PHONE_TYPE,Contacts.PhonesColumns.TYPE_MOBILE);
           intent.putExtra(Contacts.Intents.Insert.EMAIL, "com@com.com");
           intent.putExtra(Contacts.Intents.Insert.EMAIL_TYPE,                    Contacts.ContactMethodsColumns.TYPE_WORK);
           startActivity(intent);
intent action大全： 
- android.intent.action.ALL_APPS
- android.intent.action.ANSWER
- android.intent.action.ATTACH_DATA
- android.intent.action.BUG_REPORT
- android.intent.action.CALL
- android.intent.action.CALL_BUTTON
- android.intent.action.CHOOSER
- android.intent.action.CREATE_LIVE_FOLDER
- android.intent.action.CREATE_SHORTCUT
- android.intent.action.DELETE
- android.intent.action.DIAL
- android.intent.action.EDIT
- android.intent.action.GET_CONTENT
- android.intent.action.INSERT
- android.intent.action.INSERT_OR_EDIT
- android.intent.action.MAIN
- android.intent.action.MEDIA_SEARCH
- android.intent.action.PICK
- android.intent.action.PICK_ACTIVITY
- android.intent.action.RINGTONE_PICKER
- android.intent.action.RUN
- android.intent.action.SEARCH
- android.intent.action.SEARCH_LONG_PRESS
- android.intent.action.SEND
- android.intent.action.SENDTO
- android.intent.action.SET_WALLPAPER
- android.intent.action.SYNC
- android.intent.action.SYSTEM_TUTORIAL
- android.intent.action.VIEW
- android.intent.action.VOICE_COMMAND
- android.intent.action.WEB_SEARCH
- android.net.wifi.PICK_WIFI_NETWORK
- android.settings.AIRPLANE_MODE_SETTINGS
- android.settings.APN_SETTINGS
- android.settings.APPLICATION_DEVELOPMENT_SETTINGS
- android.settings.APPLICATION_SETTINGS
- android.settings.BLUETOOTH_SETTINGS
- android.settings.DATA_ROAMING_SETTINGS
- android.settings.DATE_SETTINGS
- android.settings.DISPLAY_SETTINGS
- android.settings.INPUT_METHOD_SETTINGS
- android.settings.INTERNAL_STORAGE_SETTINGS
- android.settings.LOCALE_SETTINGS
- android.settings.LOCATION_SOURCE_SETTINGS
- android.settings.MANAGE_APPLICATIONS_SETTINGS
- android.settings.MEMORY_CARD_SETTINGS
- android.settings.NETWORK_OPERATOR_SETTINGS
- android.settings.QUICK_LAUNCH_SETTINGS
- android.settings.SECURITY_SETTINGS
- android.settings.SETTINGS
- android.settings.SOUND_SETTINGS
- android.settings.SYNC_SETTINGS
- android.settings.USER_DICTIONARY_SETTINGS
- android.settings.WIFI_IP_SETTINGS
- android.settings.WIFI_SETTINGS
- android.settings.WIRELESS_SETTINGS
**附录：**
|String|"android.intent.action.ADD_SHORTCUT"|动作：在系统中添加一个快捷方式。.|
|----|----|----|
|String|"android.intent.action.ALL_APPS"|动作：列举所有可用的应用。|
|String|"android.intent.action.ANSWER"|动作：处理拨入的电话。|
|String|"android.intent.action.BUG_REPORT"|动作：显示 activity 报告错误。|
|String|"android.intent.action.CALL"|动作：拨打电话，被呼叫的联系人在数据中指定。|
|String|"android.intent.action.CLEAR_CREDENTIALS"|动作：清除登陆凭证 (credential)。|
|String|"android.intent.action.VIEW"|动作：和 VIEW_ACTION 相同，是在数据上执行的标准动作。|
|String|"android.intent.action.DELETE"|动作：从容器中删除给定的数据。|
|String|"android.intent.action.DIAL"|动作：拨打数据中指定的电话号码。|
|String|"android.intent.action.EDIT"|动作：为制定的数据显示可编辑界面。|
|String|"android.intent.action.EMERGENCY_DIAL"|动作：拨打紧急电话号码。|
|String|"android.intent.action.LOGIN"|动作：获取登录凭证。|
|String|"android.intent.action.MAIN"|动作：作为主入口点启动，不需要数据。|
|String|"android.intent.action.PICK"|动作：从数据中选择一个项目item，将被选中的项目返回。|
|String|"android.intent.action.PICK_ACTIVITY"|动作：选择一个activity，返回被选择的activity的类名|
|String|"android.intent.action.RUN"|动作：运行数据（指定的应用），无论它（应用）是什么。|
|String|"android.intent.action.SENDTO"|动作：向 data 指定的接收者发送一个消息。|
|String|"android.intent.action.GET_CONTENT"|动作：让用户选择数据并返回。|
|String|"android.intent.action.INSERT"|动作：在容器中插入一个空项 (item)。|
|String|"android.intent.action.SETTINGS"|动作：显示系统设置。输入：无。|
|String|"android.intent.action.VIEW"|动作：向用户显示数据。|
|String|"android.intent.action.WALLPAPER_SETTINGS"|动作：显示选择墙纸的设置界面。输入：无。|
|String|"android.intent.action.WEB_SEARCH"|动作：执行 web 搜索。|
|String|"android.intent.action.SYNC"|动作：执行数据同步。|
|String|"android.intent.action.SERVICE_STATE"|广播：电话服务的状态已经改变。|
|String|"android.intent.action.TIMEZONE_CHANGED"|广播：时区已经改变。|
|String|"android.intent.action.TIME_SET"|广播：时间已经改变（重新设置）。|
|String|"android.intent.action.TIME_TICK"|广播：当前时间已经变化（正常的时间流逝）。|
|String|"android.intent.action.UMS_CONNECTED"|广播：设备进入 USB 大容量存储模式。|
|String|"android.intent.action.UMS_DISCONNECTED"|广播：设备从 USB 大容量存储模式退出。|
|String|"android.intent.action.WALLPAPER_CHANGED"|广播：系统的墙纸已经改变。|
|String|"android.intent.action.XMPP_CONNECTED"|广播：XMPP 连接已经被建立。|
|String|"android.intent.action.XMPP_DI|广播：XMPP 连接已经被断开。|
|String|"android.intent.action.SIG_STR"|广播：电话的信号强度已经改变。|
|String|"android.intent.action.BATTERY_CHANGED"|广播：充电状态，或者电池的电量发生变化。|
|String|"android.intent.action.BOOT_COMPLETED"|广播：在系统启动后，这个动作被广播一次（只有一次）|
|String|"android.intent.action.DATA_ACTIVITY"|广播：电话的数据活动(data activity)状态已经改变|
|String|"android.intent.action.DATA_STATE"|广播：电话的数据连接状态已经改变。|
|String|"android.intent.action.DATE_CHANGED"|广播：日期被改变。|
|String|"android.server.checkin.FOTA_CANCEL"|广播：取消所有被挂起的 (pending) 更新下载。|
|String|"android.server.checkin.FOTA_INSTALL"|广播：更新已经被确认，马上就要开始安装。|
|String|"android.server.checkin.FOTA_READY"|广播：更新已经被下载，可以开始安装。|
|String|"android.server.checkin.FOTA_RESTART"|广播：恢复已经停止的更新下载。|
|String|"android.server.checkin.FOTA_UPDATE"|广播：通过 OTA 下载并安装[操作系统](http://lib.csdn.net/base/operatingsystem)更新。|
|String|"android.intent.action.MEDIABUTTON"|广播：用户按下了“Media Button”。|
|String|"android.intent.action.MEDIA_BAD_REMOVAL"|广播：扩展卡从SD卡插槽拔出，但是挂载点还没unmount。|
|String|"android.intent.action.MEDIA_EJECT"|广播：用户想要移除扩展介质（拔掉扩展卡）。|
|String|"android.intent.action.MEDIA_MOUNTED"|广播：扩展介质被插入，而且已经被挂载。|
|String|"android.intent.action.MEDIA_REMOVED"|广播：扩展介质被移除。|
|String|"android.intent.action.MEDIA_SCANNER_FINISHED"|广播：已经扫描完介质的一个目录。|
|String|"android.intent.action.MEDIA_SCANNER_STARTED"|广播：开始扫描介质的一个目录。|
|String|"android.intent.action.MEDIA_SHARED"|广播：扩展介质的挂载被解除 (unmount)|
|String|"android.intent.action.MEDIA_UNMOUNTED"|广播：扩展介质存在，但是还没有被挂载 (mount)。|
|String|"android.intent.action.MWI"|广播：电话的消息等待（语音邮件）状态已经改变。|
|String|"android.intent.action.PACKAGE_ADDED"|广播：设备上新安装了一个应用程序包。|
|String|"android.intent.action.PACKAGE_REMOVED"|广播：设备上删除了一个应用程序包。|
|String|"android.intent.action.PHONE_STATE"|广播：电话状态已经改变。|
|String|"android.intent.action.PROVIDER_CHANGED"|广播：更新将要（真正）被安装。|
|String|"android.intent.action.PROVISIONING_CHECK"|广播：要求provisioning service下载最新的设置|
|String|"android.intent.action.SCREEN_OFF"|广播：屏幕被关闭。|
|String|"android.intent.action.SCREEN_ON"|广播：屏幕已经被打开。|
|String|"android.intent.action.NETWORK_TICKLE_RECEIVED"|广播：设备收到了新的网络 "tickle" 通知。|
|String|"android.intent.action.STATISTICS_REPORT"|广播：要求 receivers 报告自己的统计信息。|
|String|"android.intent.action.STATISTICS_STATE_CHANGED"|广播：统计信息服务的状态已经改变。|
|String|"android.intent.action.CFF"|广播：语音电话的呼叫转移状态已经改变。|
|String|"android.intent.action.CONFIGURATION_CHANGED"|广播：设备的配置信息已经改变，参见 Resources.Configuration|
|String|"android.intent.category.ALTERNATIVE"|类别：说明activity是用户正在浏的数据的一个可选操作。|
|String|"android.intent.category.WALLPAPER"|类别：这个 activity 能过为设备设置墙纸。|
|String|"android.intent.category.UNIT_TEST"|类别：应该被用作单元[测试](http://lib.csdn.net/base/softwaretest)（通过 test harness 运行）。|
|String|"android.intent.category.TEST"|类别：作为测试目的使用，不是正常的用户体验的一部分。|
|String|"android.intent.category.TAB"|类别：activity应该在TabActivity中作为一个tab使用|
|String|"android.intent.category.SAMPLE_CODE"|类别：To be used as an sample code example (not part of the normal user experience).|
|String|"android.intent.category.PREFERENCE"|类别：activity是一个设置面板 (preference panel)。|
|String|"android.intent.category.HOME"|类别：主屏幕 (activity)，设备启动后显示的第一个 activity。|
|String|"android.intent.category.BROWSABLE"|类别：能够被浏览器安全使用的 activities 必须支持这个类别。|
|String|"android.intent.category.DEFAULT"|类别：如果 activity 是对数据执行确省动作（点击, center press）的一个选项，需要设置这个类别。|
|String|"android.intent.category.DEVELOPMENT_PREFERENCE"|类别：说明 activity 是一个设置面板 (development preference panel).|
|String|"android.intent.category.EMBED"|类别：能够在上级（父）activity 中运行。|
|String|"android.intent.category.FRAMEWORK_INSTRUMENTATION_TEST"|类别：To be used as code under test for framework instrumentation tests.|
|String|"android.intent.category.GADGET"|类别：这个 activity 可以被嵌入宿主 activity (activity that is hosting gadgets)。|
|String|"android.intent.category.LAUNCHER"|类别：Activity 应该被显示在顶级的 launcher 中。|
|String|"android.intent.category.SELECTED_ALTERNATIVE"|类别：对于被用户选中的数据，activity 是它的一个可选操作。|
|int|8 0x00000008|启动标记：和 NEW_TASK_LAUNCH 联合使用，禁止将已有的任务改变为前景任务 (foreground)。|
|int|4 0x00000004|启动标记：设置以后，activity 将成为历史堆栈中的第一个新任务（栈顶）。|
|int|1 0x00000001|启动标记：设置以后，新的 activity 不会被保存在历史堆栈中。|
|int|2 0x00000002|启动标记：设置以后，如果 activity 已经启动，而且位于历史堆栈的顶端，将不再启动（不重新启动） activity。|
|int|16 0x00000010|启动标记：如果这个标记被设置，而且被一个已经存在的 activity 用来启动新的 activity，已有 activity 的回复目标 (reply target) 会被转移给新的 activity。|
|String|"android.intent.extra.INTENT"|附加数据：和 PICK_ACTIVITY_ACTION 一起使用时，说明用户选择的用来显示的 activity；和 ADD_SHORTCUT_ACTION 一起使用的时候，描述要添加的快捷方式。|
|String|"android.intent.extra.LABEL"|附加数据：大写字母开头的字符标签，和 ADD_SHORTCUT_ACTION 一起使用。|
|String|"android.intent.extra.TEMPLATE"|附加数据：新记录的初始化模板。|
|Creator|无|无|
