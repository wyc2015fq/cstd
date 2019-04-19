# Android之NFC - xqhrs232的专栏 - CSDN博客
2016年04月09日 11:36:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：582
原文地址::[http://blog.csdn.net/bear_huangzhen/article/details/46333421](http://blog.csdn.net/bear_huangzhen/article/details/46333421)
相关文章
1、
# [NFC](http://blog.csdn.net/wangkaiblog/article/details/43085451)----[http://blog.csdn.net/wangkaiblog/article/details/43085451](http://blog.csdn.net/wangkaiblog/article/details/43085451)
# 2、
基于Android的NFC实现与应用----[http://wenku.baidu.com/link?url=OZh--hw4ZDrAfWP0msp6VJrzkMHX5OKEdnZWAQTflFVgU9jz34MUeQWmiBrzBUsvpN5bvFknPkjYNa8mr-YQ-mdqePVjEcoRXo3-GBRvbo_](http://wenku.baidu.com/link?url=OZh--hw4ZDrAfWP0msp6VJrzkMHX5OKEdnZWAQTflFVgU9jz34MUeQWmiBrzBUsvpN5bvFknPkjYNa8mr-YQ-mdqePVjEcoRXo3-GBRvbo_)
版权声明：本文为博主原创文章，未经博主允许不得转载。
**NFC简介：**
Near Field Communication 近场通信，是一种数据传输技术。
与wifi、蓝牙、红外线等数据传输技术的一个主要差异就是有效距离一般不能超过4cm。
**NFC支持3种工作模式：**
1.读卡器模式；
2.仿真卡模式；
3.点对点模式；
1.读卡器模式：
通过NFC设备（支持NFC的Android手机）从带有NFC芯片的标签、贴纸、报纸、明信片等媒介读取信息，或将数据写到这些媒介中。
2.仿真卡模式：
是将支持NFC的手机或其他电子设备当成借记卡、信用卡、公交卡、门禁卡等IC卡使用；基本原理是将相应的IC卡中的信息（支付凭证）封装成数据包存储在支持NFC的手机中，在使用时还需要一个NFC射频器（相当于刷传统IC卡时使用的刷卡器），将手机靠近NFC射频器，手机就会收到NFC射频器发过来的信号，在通过一系列复杂的验证后，将IC卡的相应信息传入NFC射频器，最后这些IC卡数据会传入NFC射频器连接的计算机，并进行相应的处理（如电子转账、开门等操作）。
3.点对点模式：
与蓝牙、红外差不多，可以用于不同的NFC设备之间进行数据交换，只是NFC的点对点模式有效距离更短，不能超过4cm；但是如果两个设备使用的都是Android4.2及以上版本，NFC会直接利用蓝牙传输，这种技术被称为Android Beam，所以Android Beam传输数据的两部设备不局限于4cm之内。
**基础知识：**
1.Android SDK API主要支持NFC论坛标准（Forum Standard），这种标准被称为NDEF（NFC Data Exchange Format，NFC数据交换格式）；
2.Android SDK API支持如下三种NDEF数据的操作：
a.从NFC标签读取NDEF格式的数据；
b.向NFC标签写入NDEF格式的数据；
c.通过Android Beam技术将NDEF数据发送到另一部NFC设备；
3.在一个NFC设备读取NFC标签或另一个NFC设备中的数据之前会在0.1秒的时间之内建立NFC连接，然后数据会自动从被读取一端流向读取数据的一端；数据接收端会根据具体的数据格式和标签类型调用相应的Activity（这种行为也称为Tag Dispatch），这些Activity都需要定义Intent Filter，这些Intent Filter中就会指定不同的过滤机制，分为三个级别，也称为NFC的三重过滤机制。
4.NDEF_DISCOVERED：
只过滤固定格式的NDEF数据。例如：纯文本、指定协议（http、ftp、smb等）的URI等；
  TECH_DISCOVERED：
当ACTION_NDEF_DISCOVERED指定的过滤机制无法匹配Tag时，就会使用这种过滤机制进行匹配，这种过滤机制并不是通过Tag中的数据格式进行匹配的，而是根据Tag支持的数据存储格式进行匹配，因此这种过滤机制的范围更广；
  TAG_DISCOVERED：
如果将NFC过滤机制看成if...else if...else语句的话，那么这种过滤机制就相当于else部分，当前面两种过滤机制都匹配失败后，系统就会利用这种过滤机制来处理，这种过滤机制用来处理未识别的Tag（数据格式不对，而且Tag支持的格式也不匹配）。
5.Android系统会依次匹配NDEF_DISCOVERED、TECH_DISCOVERED和TAG_DISCOVERED；如果通过三重过滤机制仍然无法匹配Tag，则什么都不做；通常在成功匹配Tag后，Android设备会发出比较清脆的声音，而未成功匹配Tag，就会发出比较沉闷的声音。
此过程的处理流程如下图所示：
![](https://img-blog.csdn.net/20150602233643563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmVhcl9odWFuZ3poZW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
6.在manifest文件中需要设置的部分有：
设置权限：
<uses-permission android:name="android.permission.NFC" />
限制Android版本：
android:minSdkVersion="14"
限制安装的设备：
<uses-feature  android:name="android.hardware.nfc"  android:required="true" />
设置Activity的Intent Filter，比如设置为三种过滤机制的一种：
<intent-filter>
<action android:name="android.nfc.action.TECH_DISCOVERED" />
</intent-filter>
接下来，我们来第一个例子，这个例子是属于**读卡器模式**，从NFC芯片中读取和写入数据。
它的manifest文件内容如下：
**[html]**[view
 plain](http://blog.csdn.net/bear_huangzhen/article/details/46333421#)[copy](http://blog.csdn.net/bear_huangzhen/article/details/46333421#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <manifestxmlns:android="http://schemas.android.com/apk/res/android"
- package="com.r8c.nfc_demo"
- android:versionCode="110"
- android:versionName="1.1.0">
- 
- <uses-sdk
- android:minSdkVersion="15"
- android:targetSdkVersion="17"/>
- <!-- NFC权限声明 -->
- <uses-permissionandroid:name="android.permission.NFC"/>
- 
- <uses-feature
- android:name="android.hardware.nfc"
- android:required="true"/>
- 
- <application
- android:allowBackup="true"
- android:icon="@drawable/ic_launcher"
- android:label="@string/app_name"
- android:theme="@style/AppTheme">
- 
- <activity
- android:name="com.r8c.nfc_demo.NfcDemoActivity"
- android:configChanges="orientation|keyboardHidden|screenSize"
- android:label="@string/app_name"
- android:launchMode="singleTask">
- <intent-filter>
- <actionandroid:name="android.intent.action.MAIN"/>
- 
- <categoryandroid:name="android.intent.category.LAUNCHER"/>
- </intent-filter>
- <!-- TECH_DISCOVERED类型的nfc -->
- <intent-filter>
- <actionandroid:name="android.nfc.action.TECH_DISCOVERED"/>
- </intent-filter>
- <!-- 后设资源  调用自己建立的文件夹xml中的文件 -->
- <meta-data
- android:name="android.nfc.action.TECH_DISCOVERED"
- android:resource="@xml/nfc_tech_filter"/>
- </activity>
- </application>
- 
- </manifest>
它的Activity的内容如下，包括读取、写入、删除三大功能：（其中删除功能是通过写入空值来实现的）
**[java]**[view
 plain](http://blog.csdn.net/bear_huangzhen/article/details/46333421#)[copy](http://blog.csdn.net/bear_huangzhen/article/details/46333421#)
- import java.io.IOException;  
- import java.io.UnsupportedEncodingException;  
- import java.nio.charset.Charset;  
- 
- import android.media.AudioManager;  
- import android.media.MediaPlayer;  
- import android.media.RingtoneManager;  
- import android.net.Uri;  
- import android.nfc.FormatException;  
- import android.nfc.NdefMessage;  
- import android.nfc.NdefRecord;  
- import android.nfc.NfcAdapter;  
- import android.nfc.Tag;  
- import android.nfc.tech.MifareUltralight;  
- import android.nfc.tech.Ndef;  
- import android.nfc.tech.NfcA;  
- import android.os.Bundle;  
- import android.app.Activity;  
- import android.app.PendingIntent;  
- import android.content.Context;  
- import android.content.Intent;  
- import android.content.IntentFilter;  
- import android.graphics.Color;  
- import android.util.Log;  
- import android.view.Menu;  
- import android.view.View;  
- import android.view.View.OnClickListener;  
- import android.widget.Button;  
- import android.widget.TextView;  
- import android.widget.Toast;  
- 
- publicclass NfcDemoActivity extends Activity implements OnClickListener {  
- 
- // NFC适配器
- private NfcAdapter nfcAdapter = null;  
- // 传达意图
- private PendingIntent pi = null;  
- // 滤掉组件无法响应和处理的Intent
- private IntentFilter tagDetected = null;  
- // 文本控件
- private TextView promt = null;  
- // 是否支持NFC功能的标签
- privateboolean isNFC_support = false;  
- // 读、写、删按钮控件
- private Button readBtn, writeBtn, deleteBtn;  
- 
- @Override
- protectedvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.activity_nfc_demo);  
-         setupViews();  
-         initNFCData();  
-     }  
- 
- @Override
- protectedvoid onResume() {  
- super.onResume();  
- if (isNFC_support == false) {  
- // 如果设备不支持NFC或者NFC功能没开启，就return掉
- return;  
-         }  
- // 开始监听NFC设备是否连接
-         startNFC_Listener();  
- 
- if (NfcAdapter.ACTION_TECH_DISCOVERED.equals(this.getIntent()  
-                 .getAction())) {  
- // 注意这个if中的代码几乎不会进来，因为刚刚在上一行代码开启了监听NFC连接，下一行代码马上就收到了NFC连接的intent，这种几率很小
- // 处理该intent
-             processIntent(this.getIntent());  
-         }  
-     }  
- 
- @Override
- protectedvoid onPause() {  
- super.onPause();  
- if (isNFC_support == true) {  
- // 当前Activity如果不在手机的最前端，就停止NFC设备连接的监听
-             stopNFC_Listener();  
-         }  
-     }  
- 
- @Override
- protectedvoid onNewIntent(Intent intent) {  
- super.onNewIntent(intent);  
- // 当前app正在前端界面运行，这个时候有intent发送过来，那么系统就会调用onNewIntent回调方法，将intent传送过来
- // 我们只需要在这里检验这个intent是否是NFC相关的intent，如果是，就调用处理方法
- if (NfcAdapter.ACTION_TECH_DISCOVERED.equals(intent.getAction())) {  
-             processIntent(intent);  
-         }  
-     }  
- 
- @Override
- publicvoid onClick(View v) {  
- 
- // 点击读按钮后
- if (v.getId() == R.id.read_btn) {  
- try {  
-                 String content = read(tagFromIntent);  
- if (content != null && !content.equals("")) {  
-                     promt.setText(promt.getText() + "nfc标签内容：\n" + content  
-                             + "\n");  
-                 } else {  
-                     promt.setText(promt.getText() + "nfc标签内容：\n" + "内容为空\n");  
-                 }  
-             } catch (IOException e) {  
-                 promt.setText(promt.getText() + "错误:" + e.getMessage() + "\n");  
-                 Log.e("myonclick", "读取nfc异常", e);  
-             } catch (FormatException e) {  
-                 promt.setText(promt.getText() + "错误:" + e.getMessage() + "\n");  
-                 Log.e("myonclick", "读取nfc异常", e);  
-             }  
- // 点击写后写入
-         } elseif (v.getId() == R.id.write_btn) {  
- try {  
-                 write(tagFromIntent);  
-             } catch (IOException e) {  
-                 promt.setText(promt.getText() + "错误:" + e.getMessage() + "\n");  
-                 Log.e("myonclick", "写nfc异常", e);  
-             } catch (FormatException e) {  
-                 promt.setText(promt.getText() + "错误:" + e.getMessage() + "\n");  
-                 Log.e("myonclick", "写nfc异常", e);  
-             }  
-         } elseif (v.getId() == R.id.delete_btn) {  
- try {  
-                 delete(tagFromIntent);  
-             } catch (IOException e) {  
-                 promt.setText(promt.getText() + "错误:" + e.getMessage() + "\n");  
-                 Log.e("myonclick", "删除nfc异常", e);  
-             } catch (FormatException e) {  
-                 promt.setText(promt.getText() + "错误:" + e.getMessage() + "\n");  
-                 Log.e("myonclick", "删除nfc异常", e);  
-             }  
-         }  
-     }  
- 
- privatevoid setupViews() {  
- // 控件的绑定
-         promt = (TextView) findViewById(R.id.promt);  
-         readBtn = (Button) findViewById(R.id.read_btn);  
-         writeBtn = (Button) findViewById(R.id.write_btn);  
-         deleteBtn = (Button) findViewById(R.id.delete_btn);  
- // 给文本控件赋值初始文本
-         promt.setText("等待RFID标签");  
- // 监听读、写、删按钮控件
-         readBtn.setOnClickListener(this);  
-         writeBtn.setOnClickListener(this);  
-         deleteBtn.setOnClickListener(this);  
-     }  
- 
- privatevoid initNFCData() {  
- // 初始化设备支持NFC功能
-         isNFC_support = true;  
- // 得到默认nfc适配器
-         nfcAdapter = NfcAdapter.getDefaultAdapter(getApplicationContext());  
- // 提示信息定义
-         String metaInfo = "";  
- // 判定设备是否支持NFC或启动NFC
- if (nfcAdapter == null) {  
-             metaInfo = "设备不支持NFC！";  
-             Toast.makeText(this, metaInfo, Toast.LENGTH_SHORT).show();  
-             isNFC_support = false;  
-         }  
- if (!nfcAdapter.isEnabled()) {  
-             metaInfo = "请在系统设置中先启用NFC功能！";  
-             Toast.makeText(this, metaInfo, Toast.LENGTH_SHORT).show();  
-             isNFC_support = false;  
-         }  
- 
- if (isNFC_support == true) {  
-             init_NFC();  
-         } else {  
-             promt.setTextColor(Color.RED);  
-             promt.setText(metaInfo);  
-         }  
-     }  
- 
- @Override
- publicboolean onCreateOptionsMenu(Menu menu) {  
- // Inflate the menu; this adds items to the action bar if it is present.
-         getMenuInflater().inflate(R.menu.nfc_demo, menu);  
- returntrue;  
-     }  
- 
- // 字符序列转换为16进制字符串
- private String bytesToHexString(byte[] src) {  
- return bytesToHexString(src, true);  
-     }  
- 
- private String bytesToHexString(byte[] src, boolean isPrefix) {  
-         StringBuilder stringBuilder = new StringBuilder();  
- if (isPrefix == true) {  
-             stringBuilder.append("0x");  
-         }  
- if (src == null || src.length <= 0) {  
- returnnull;  
-         }  
- char[] buffer = newchar[2];  
- for (int i = 0; i < src.length; i++) {  
-             buffer[0] = Character.toUpperCase(Character.forDigit(  
-                     (src[i] >>> 4) & 0x0F, 16));  
-             buffer[1] = Character.toUpperCase(Character.forDigit(src[i] & 0x0F,  
- 16));  
-             System.out.println(buffer);  
-             stringBuilder.append(buffer);  
-         }  
- return stringBuilder.toString();  
-     }  
- 
- private Tag tagFromIntent;  
- 
- /**
-      * Parses the NDEF Message from the intent and prints to the TextView
-      */
- publicvoid processIntent(Intent intent) {  
- if (isNFC_support == false)  
- return;  
- 
- // 取出封装在intent中的TAG
-         tagFromIntent = intent.getParcelableExtra(NfcAdapter.EXTRA_TAG);  
- 
-         promt.setTextColor(Color.BLUE);  
-         String metaInfo = "";  
-         metaInfo += "卡片ID：" + bytesToHexString(tagFromIntent.getId()) + "\n";  
-         Toast.makeText(this, "找到卡片", Toast.LENGTH_SHORT).show();  
- 
- // Tech List
-         String prefix = "android.nfc.tech.";  
-         String[] techList = tagFromIntent.getTechList();  
- 
- //分析NFC卡的类型： Mifare Classic/UltraLight Info
-         String CardType = "";  
- for (int i = 0; i < techList.length; i++) {  
- if (techList[i].equals(NfcA.class.getName())) {  
- // 读取TAG
-                 NfcA mfc = NfcA.get(tagFromIntent);  
- try {  
- if ("".equals(CardType))  
-                         CardType = "MifareClassic卡片类型 \n 不支持NDEF消息 \n";  
-                 } catch (Exception e) {  
-                     e.printStackTrace();  
-                 }  
-             } elseif (techList[i].equals(MifareUltralight.class.getName())) {  
-                 MifareUltralight mifareUlTag = MifareUltralight  
-                         .get(tagFromIntent);  
-                 String lightType = "";  
- // Type Info
- switch (mifareUlTag.getType()) {  
- case MifareUltralight.TYPE_ULTRALIGHT:  
-                     lightType = "Ultralight";  
- break;  
- case MifareUltralight.TYPE_ULTRALIGHT_C:  
-                     lightType = "Ultralight C";  
- break;  
-                 }  
-                 CardType = lightType + "卡片类型\n";  
- 
-                 Ndef ndef = Ndef.get(tagFromIntent);  
-                 CardType += "最大数据尺寸:" + ndef.getMaxSize() + "\n";  
- 
-             }  
-         }  
-         metaInfo += CardType;  
-         promt.setText(metaInfo);  
-     }  
- 
- // 读取方法
- private String read(Tag tag) throws IOException, FormatException {  
- if (tag != null) {  
- //解析Tag获取到NDEF实例
-             Ndef ndef = Ndef.get(tag);  
- //打开连接
-             ndef.connect();  
- //获取NDEF消息
-             NdefMessage message = ndef.getNdefMessage();  
- //将消息转换成字节数组
- byte[] data = message.toByteArray();  
- //将字节数组转换成字符串
-             String str = new String(data, Charset.forName("UTF-8"));  
- //关闭连接
-             ndef.close();  
- return str;  
-         } else {  
-             Toast.makeText(NfcDemoActivity.this, "设备与nfc卡连接断开，请重新连接...",  
-                     Toast.LENGTH_SHORT).show();  
-         }  
- returnnull;  
-     }  
- 
- // 写入方法
- privatevoid write(Tag tag) throws IOException, FormatException {  
- if (tag != null) {  
- //新建NdefRecord数组，本例中数组只有一个元素
-             NdefRecord[] records = { createRecord() };  
- //新建一个NdefMessage实例
-             NdefMessage message = new NdefMessage(records);  
- // 解析TAG获取到NDEF实例
-             Ndef ndef = Ndef.get(tag);  
- // 打开连接
-             ndef.connect();  
- // 写入NDEF信息
-             ndef.writeNdefMessage(message);  
- // 关闭连接
-             ndef.close();  
-             promt.setText(promt.getText() + "写入数据成功！" + "\n");  
-         } else {  
-             Toast.makeText(NfcDemoActivity.this, "设备与nfc卡连接断开，请重新连接...",  
-                     Toast.LENGTH_SHORT).show();  
-         }  
-     }  
- 
- // 删除方法
- privatevoid delete(Tag tag) throws IOException, FormatException {  
- if (tag != null) {  
- //新建一个里面无任何信息的NdefRecord实例
-             NdefRecord nullNdefRecord = new NdefRecord(NdefRecord.TNF_MIME_MEDIA,  
- newbyte[] {}, newbyte[] {}, newbyte[] {});  
-             NdefRecord[] records = { nullNdefRecord };  
-             NdefMessage message = new NdefMessage(records);  
- // 解析TAG获取到NDEF实例
-             Ndef ndef = Ndef.get(tag);  
- // 打开连接
-             ndef.connect();  
- // 写入信息
-             ndef.writeNdefMessage(message);  
- // 关闭连接
-             ndef.close();  
-             promt.setText(promt.getText() + "删除数据成功！" + "\n");  
-         } else {  
-             Toast.makeText(NfcDemoActivity.this, "设备与nfc卡连接断开，请重新连接...",  
-                     Toast.LENGTH_SHORT).show();  
-         }  
-     }  
- 
- //返回一个NdefRecord实例
- private NdefRecord createRecord() throws UnsupportedEncodingException {  
- //组装字符串，准备好你要写入的信息
-         String msg = "BEGIN:VCARD\n" + "VERSION:2.1\n" + "中国湖北省武汉市\n"
-                 + "武汉大学计算机学院\n" + "END:VCARD";  
- //将字符串转换成字节数组
- byte[] textBytes = msg.getBytes();  
- //将字节数组封装到一个NdefRecord实例中去
-         NdefRecord textRecord = new NdefRecord(NdefRecord.TNF_MIME_MEDIA,  
- "text/x-vCard".getBytes(), newbyte[] {}, textBytes);  
- return textRecord;  
-     }  
- 
- private MediaPlayer ring() throws Exception, IOException {  
- // TODO Auto-generated method stub
-         Uri alert = RingtoneManager  
-                 .getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);  
-         MediaPlayer player = new MediaPlayer();  
-         player.setDataSource(this, alert);  
- final AudioManager audioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);  
- if (audioManager.getStreamVolume(AudioManager.STREAM_NOTIFICATION) != 0) {  
-             player.setAudioStreamType(AudioManager.STREAM_NOTIFICATION);  
-             player.setLooping(false);  
-             player.prepare();  
-             player.start();  
-         }  
- return player;  
-     }  
- 
- privatevoid startNFC_Listener() {  
- // 开始监听NFC设备是否连接，如果连接就发pi意图
-         nfcAdapter.enableForegroundDispatch(this, pi,  
- new IntentFilter[] { tagDetected }, null);  
-     }  
- 
- privatevoid stopNFC_Listener() {  
- // 停止监听NFC设备是否连接
-         nfcAdapter.disableForegroundDispatch(this);  
-     }  
- 
- privatevoid init_NFC() {  
- // 初始化PendingIntent，当有NFC设备连接上的时候，就交给当前Activity处理
-         pi = PendingIntent.getActivity(this, 0, new Intent(this, getClass())  
-                 .addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP), 0);  
- // 新建IntentFilter，使用的是第二种的过滤机制
-         tagDetected = new IntentFilter(NfcAdapter.ACTION_TECH_DISCOVERED);  
-         tagDetected.addCategory(Intent.CATEGORY_DEFAULT);  
-     }  
- 
- }  
下面是该示例的完整源码下载链接：
[Android NFC Demo1](http://download.csdn.net/detail/bear_huangzhen/8766235)
