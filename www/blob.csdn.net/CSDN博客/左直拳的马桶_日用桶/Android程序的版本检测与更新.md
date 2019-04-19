# Android程序的版本检测与更新 - 左直拳的马桶_日用桶 - CSDN博客
2013年01月06日 11:19:59[左直拳](https://me.csdn.net/leftfist)阅读数：5202
做个网站的安卓客户端，用户安装到自己手机上，如果我出了新版本怎么办呢？要有版本更新功能。
本来版本检测最好可以自动进行。但如果每次开启程序，都要先检测一轮，是一种浪费，毕竟版本更新是小概率的事情。或许可以程序开启的时候，判断一下时间，单日就检测，双日就不检测，或者随机什么的，降低一下检测的频率？
我采取的做法是将检测功能做到了菜单上，用户有需要，就手动打开自己检测一下。反正我们这个是网站客户端，有版本更新，在网站上发个通告就行了。
版本检测与更新有以下几个关键步骤：
1、检测有无新版本
2、下载新版本
3、安装替换新版本
我处理的方案是
1、在assets文件夹新增一个文件：ver.cfg，记录版本信息，纯文本格式，内容只有一句话：
`Version=1.0`![](https://img-my.csdn.net/uploads/201301/06/1357441939_7727.png)
这个会随安装包装到用户的手机上
然后在网站里面，设置一XML文件ver_apk.xml，内容也只有这么一点：
```
<?xml version="1.0" encoding="utf-8" ?>
<string>1.0</string>
```
检测的时候，就先访问网站的这个XML，得到最新版本号，然后与手机上的ver.cfg文件里记录的进行比对，不同的话就可以认为存在新版本，提示进行更新。
2、下载的话就是直接下载的，我还不知道怎么弄断点续传
3、安装替换，关键在于签名。就是每个版本的签名要保持一致。否则新的无法替换旧的，提示安装未完成。
------------------- 天气太冷，咯咯咯 ------------------------------------
这个功能做在菜单上，触发代码如下：
```java
//==========================================================================
// 菜单
//==========================================================================    
private static final String urlApk = "http://3g.***.com/tool/***.apk";
private static final String urlVer = "http://3g.***.com/tool/ver_apk.xml";    
@Override
public boolean onCreateOptionsMenu(Menu menu) {
	menu.add(Menu.NONE, Menu.FIRST + 1, 5, "检测更新").setIcon(
	android.R.drawable.ic_menu_upload);
	menu.add(Menu.NONE,Menu.FIRST+2,4,"退出").setIcon(android.R.drawable.ic_lock_power_off);
	return true;
}
@Override
public boolean onOptionsItemSelected(MenuItem item) {
	switch (item.getItemId()) {
        case Menu.FIRST + 1:
            Toast.makeText(this, "正在检测版本", Toast.LENGTH_LONG).show();
        	UpdateVer uv = new UpdateVer(urlApk,urlVer,MainActivity.this);
        	uv.checkVer();
            break;
        case Menu.FIRST + 2:
            confirmExit();
            break;
    }
    return false;
}
```
检测更新因为代码比较多，写成一个类进行封装
UpdateVer.java
```java
package android.***;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.util.Log;
import android.webkit.URLUtil;
import android.widget.Toast;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.Properties;
import org.xml.sax.InputSource;
import java.text.SimpleDateFormat;
import java.util.Date;
public class UpdateVer extends Activity{
    private static final String TAG = "DOWNLOADAPK";
    private String PastVersion;
    private String NowVersion;
	public 	ProgressDialog pBar; 
	private String currentFilePath = ""; 
	private String fileEx="";
	private String fileNa="";
	private String strURL="";
	private String VersionUri ="";
	private Context mContext;
	private final String fileVer = "ver.cfg";
	public UpdateVer(String urlapk,String urlver,final Context context){
		SimpleDateFormat df = new SimpleDateFormat("yyyyMMddHHmmss");
		String ver = "?ver=" + df.format(new Date());//主要是避开手机的缓存
		strURL = urlapk + ver;
		VersionUri = urlver + ver;
		mContext = context;
	}
	public void checkVer() {
		// 解析Version网页，获取版本号
		getVersionxml(VersionUri);
	}
	private void compareVer()	{
		load();
		
		//当有最新版本的时候
		if(PastVersion != null && !PastVersion.equals(NowVersion)){	
			Dialog dialog = new AlertDialog.Builder(mContext).setTitle("系统更新")       
	        .setMessage(String.format("发现新版本%s，目前版本为%s，请更新！",NowVersion,PastVersion))// 设置内容
	        // 设置确定按钮
	        .setPositiveButton("确定"
        		,new DialogInterface.OnClickListener() {       
                	@Override
					public void onClick(DialogInterface dialog,       
                                 int which) {       
		                            pBar = new ProgressDialog(mContext);
		                            pBar.setTitle("正在下载");       
		                            pBar.setMessage("请稍候...");
		                            pBar.setProgressStyle(ProgressDialog.STYLE_SPINNER);            
		                            fileEx = strURL.substring(strURL.lastIndexOf(".") + 1,strURL.length()).toLowerCase();
		                            fileEx = fileEx.substring(0,fileEx.lastIndexOf("?"));
		                            fileNa = strURL.substring(strURL.lastIndexOf("/") + 1,strURL.lastIndexOf("."));
		                            getFile(strURL);     
                         		}
                    }).setNegativeButton("取消",         
                    		new DialogInterface.OnClickListener() {
                    			@Override
								public void onClick(DialogInterface dialog,       
                    					int whichButton) {
                    						// 点击"取消"按钮之后退出程序         
                    					}
                    		}).create();// 创建
			 // 显示对话框
			 dialog.show();
		}
		else{
			Toast.makeText(mContext, String.format("当前为最新版本%s",PastVersion), Toast.LENGTH_LONG).show();
		}
	}
	private void getFile(final String strPath) 
	{ 
		pBar.show();
	    try{ 
	    	if (strPath.equals(currentFilePath) ){ 
	    		getDataSource(strPath);  
	    	}
	    	currentFilePath = strPath;      
	    	Runnable r = new Runnable(){   
	    		@Override
				public void run() 
	    		{   
	    			try{ 
	    				getDataSource(strPath); 
	    			} 
	    			catch (Exception e){
	    				Log.e(TAG, e.getMessage(), e); 
	    			} 
	    		} 
	    	};   
	    	new Thread(r).start(); 
	    } 
	    catch(Exception e){ 
	    	e.printStackTrace(); 
	    }
	}
	/*取得远程文件*/ 
	private void getDataSource(String strPath) throws Exception {
		if (!URLUtil.isNetworkUrl(strPath))	{
			Log.d("Tag","error");
		} 
		else { 
		    /*取得URL*/
			URL myURL = new URL(strPath); 
			/*建立联机*/
			URLConnection conn = myURL.openConnection();   
			conn.connect();   
			/*InputStream 下载文件*/
			InputStream is = conn.getInputStream();   
			if (is == null) {  
				Log.d("tag","error");
				throw new RuntimeException("没有读取到文件内容"); 
			} 
			/*建立临时文件*/ 
			File myTempFile = File.createTempFile(fileNa, "." + fileEx); 
			myTempFile.getAbsolutePath(); 
			/*将文件写入临时盘*/ 
			FileOutputStream fos = new FileOutputStream(myTempFile); 
			byte buf[] = new byte[128];   
			do{   
				int numread = is.read(buf);   
				if (numread <= 0) { 
					break; 
				} 
				fos.write(buf, 0, numread);   
			}while (true);  
			
			/*打开文件进行安装*/
			openFile(myTempFile);
			try	{ 
				is.close(); 
			}
			catch (Exception ex){ 
				Log.d("Tag","error");
				Log.e(TAG, "error: " + ex.getMessage(), ex); 
			}
		}
	} 
	/* 在手机上打开文件 */
	private void openFile(File f) {
		pBar.cancel();
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(android.content.Intent.ACTION_VIEW);
    
		/* 调用getMIMEType()来取得MimeType */
		String type = getMIMEType(f);
		/* 设定intent的file与MimeType */
		intent.setDataAndType(Uri.fromFile(f),type);
		mContext.startActivity(intent); 
	}
	/* 判断文件MimeType的method */
	private String getMIMEType(File f) { 
	    String type = "";
	    String fName = f.getName();
	    /* 取得扩展名 */
	    String end = fName.substring(fName.lastIndexOf(".")+1,fName.length()).toLowerCase(); 
	
	    /* 按扩展名的类型决定MimeType */
	    if(end.equals("m4a")
	    		|| end.equals("mp3")
	    		|| end.equals("mid")
	    		|| end.equals("xmf")
	    		|| end.equals("ogg")
	    		|| end.equals("wav")){
	    	type = "audio"; 
	    }
	    else if(end.equals("3gp") || end.equals("mp4")){
	    	type = "video";
	    }
	    else if(end.equals("jpg")
	    		|| end.equals("gif")
	    		|| end.equals("png")
	    		|| end.equals("jpeg")
	    		|| end.equals("bmp")){
	    	type = "image";
	    }
	    else if(end.equals("apk")){ 
	    	/* android.permission.INSTALL_PACKAGES */ 
	    	type = "application/vnd.android.package-archive"; 
	    } 
	    else{
	    	type = "*";
	    }
	    /*如果无法直接打开，就跳出软件清单给使用者选择 */
	    if(!end.equals("apk")){ 
	    	type += "/*";  
	    } 
	    return type;  
	}
	private void getVersionxml(String resourceUrl){
		GetVer gv = new GetVer();
		gv.execute(resourceUrl);
 	}
	private boolean load(){
		Properties properties = new Properties();
		try{
			InputStream stream = mContext.getAssets().open(fileVer);
			//FileInputStream stream = mContext.openFileInput(fileVer);
			//读取文件内容
			properties.load(stream);	
		}
		catch (FileNotFoundException e){
			return false;
		}
		catch(IOException e){
			return false;
		}
		catch(Exception e){
			return false;
		}
		PastVersion = String.valueOf(properties.get("Version").toString());	
		return true;
	}
	
	//==========================================================================
	// GetVer
	//==========================================================================	
	class GetVer extends AsyncTask<String, Integer, String> {
		@Override  
	    protected String doInBackground(String... urlVer) {  
			String db = null;
		    URL url = null;
		    	
			try {
				url = new URL(urlVer[0]);
			} 
			catch (MalformedURLException e) {
				e.printStackTrace();
			}  
			InputSource is = null; 
			try { 
				is = new InputSource(url.openStream()); 
				is.setEncoding("UTF-8");   
				db = SAXGetVersionService.readRssXml(is);
			} 
			catch (Exception e) {
				e.printStackTrace();
			} 
			return db;		
	    }  
	    @Override  
	    protected void onCancelled() {  
	        super.onCancelled();  
	    }  
	    @Override  
	    protected void onPostExecute(String result) { 
			NowVersion = result;
	    	compareVer();
	    }  
	}	
}
```
AndroidManifest.xml要加上几句
```
<uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.INSTALL_PACKAGES"/>
	<uses-permission android:name="android.permission.MOUNT_UNMOUNT_FILESYSTEMS"/>
	<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
    <uses-permission android:name="android.permission.RESTART_PACKAGES" />
    <uses-permission android:name="android.permission.READ_PHONE_STATE" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
```
参考文章：
http://download.csdn.net/detail/xtlvice/3510681

