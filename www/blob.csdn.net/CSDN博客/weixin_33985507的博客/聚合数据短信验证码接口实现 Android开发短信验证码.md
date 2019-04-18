# 聚合数据短信验证码接口实现 Android开发短信验证码 - weixin_33985507的博客 - CSDN博客
2016年10月09日 22:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
在说Android中的短信验证码这个知识点前，我们首先来了解下聚合数据
聚合数据介绍
聚合数据是一家国内最大的基础数据API提供商，专业从事互联网数据服务。免费提供从天气查询、空气质量、地图坐标到金融基金、电商比价、违章查询等各个领域的安全、稳定和高效的数据。开发者可以免费试用聚合数据API进行移动APP的快速开发,免除数据收集、维护等环节，大大降低开发周期及成本。因此我们可以采用聚合数据给我提供好的一套东西来进行短信验证码的功能的添加。
首先我们要做准备工作，使用聚合数据主要分为以下步骤：
第一步，注册聚合数据
进入聚合数据官网([https://www.juhe.cn/)](https://www.juhe.cn/))注册一个账号，完成注册后进入，"个人中心"——>“我的数据”点击申请数据如下图所示：
![909818-20160928095929266-1211494133.png](https://image-static.segmentfault.com/383/195/3831954597-57f9dbf9c7c26_articlex)
第二步，申请Appkey
申请后你会得到一个Appkey，此Appkey非常重要。
第三步，下载SDK
下载地址：[https://www.juhe.cn/juhesdk](https://www.juhe.cn/juhesdk)
第四步，创建工程，配置环境
1.创建一个工程把我们下载的SDK里libs下的“armeabi文件”和"smscaptcha_v_1_4.jar"复制到我们工程的libs目录下
2.在AndroidManifest中添加开发密钥、所需权限等信息
(1)在application中添加开发密钥
```
<meta-data      
      android:name="JUHE_KEY"      
      android:value="开发者 key" />
```
(2)添加所需权限
```
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />      
<uses-permission android:name="android.permission.INTERNET" />      
<uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />      
<uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION " />      
<uses-permission android:name="android.permission.READ_FINE_LOCATION" />    
<uses-permission android:name="android.permission.READ_PHONE_STATE" />    
<uses-permission android:name="android.permission.READ_CONTACTS" />
```
(3)在应用程序创建时初始化 SDK引用的Context全局变量
在这一步我们有两种方法
第一种在Activity中配置
```
public class MainActivity extends Activity {      
    @Override      
    protected void onCreate(Bundle savedInstanceState) {      
        super.onCreate(savedInstanceState);       
        //在使用SDK各组件之前初始化context信息，传入ApplicationContext      
        //注意该方法要再setContentView方法之前实现      
    /**   
    * 初始化方法  
    * @param context  
    * @needFriends 是否需要好友功能  
    */     
        CommonFun.initialize(getApplicationContext(),true);      
        setContentView(R.layout.activity_main);      
    }      
}
```
下面这个注意是聚合数据文档给我们的建议，它建议我们把这个初始化工作放在Application中，因为程序运行首先会运行Application中的方法
注意：在SDK各功能组件使用之前都需要调用CommonFun.initialize(getApplicationContext(),true);，因此我们建议该方法放在Application的初始化方法中
所以我们就按它建议的方法，而不采用第一种方法这样我们需要创建一个Applicaiton类MyApplication如下
```
package com.example.android.sms;    
    
import com.thinkland.sdk.util.CommonFun;    
    
import android.app.Application;    
    
public class MyApplication extends Application {    
    
    @Override    
    public void onCreate() {    
        super.onCreate();    
        CommonFun.initialize(getApplicationContext(), false);     
    }    
}
```
声明好Application后不要忘了在清单文件中配置
![Center](https://image-static.segmentfault.com/360/702/3607022610-57f9dc5478666_articlex)
好了环境已经配置好了，我们来验证一下看看我们是否能收到短信
MainActivity的代码如下
```
package com.example.android.sms;    
    
import com.thinkland.sdk.sms.SMSCaptcha;    
import com.thinkland.sdk.util.BaseData;    
import com.thinkland.sdk.util.CommonFun;    
    
import android.os.Bundle;    
import android.app.Activity;    
import android.util.Log;    
import android.view.Menu;    
import android.widget.Toast;    
    
public class MainActivity extends Activity {    
    
    protected static final String TAG ="MainActivity";    
    
    @Override    
    protected void onCreate(Bundle savedInstanceState) {    
        super.onCreate(savedInstanceState);    
        
        setContentView(R.layout.activity_main);    
        //初始化 SMSCaptcha对象在这个对象中有给我们发送短信验证码的方法    
        SMSCaptcha smsCaptcha=SMSCaptcha.getInstance();    
        //调用发送短信验证码的方法，在其中有一个回调    
            
        /**  
          phone 手机号码  
          callBack  返回结果回调方法.  
         */    
        smsCaptcha.sendCaptcha("此处输入你的手机号",new BaseData.ResultCallBack() {    
                
            @Override    
            public void onResult(int code, String reason, String result) {    
                    
                   /*  
                    code:返回码:  
                    服务器: 0 成功; 1 错误;  
                    本地: -2 本地网络异常; -3 服务器网络异常;-4 解析错误;-5初始化异常  
                    reason:返回信息 成功或错误原因.  
                    result:返回结果,JSON格式.错误或者无返回值时为空.*/    
                    
                if(code==0){    
                    Log.i(TAG,"code="+code);    
                    Log.i(TAG,"reason="+reason);    
                    Log.i(TAG,"result="+result);    
                }    
            }    
        });    
    }    
}
```
返回结果如下：
![Center](https://image-static.segmentfault.com/117/093/1170931299-57f9dc71a913b_articlex)
第五步：完整的功能的实现，以及界面UI的实现
首先我们来看下界面第一个界面
![909818-20160928102838172-1857952630.png](https://image-static.segmentfault.com/130/643/1306431503-57f9dc8570b6b_articlex)
我们在这个界面输入手机号，及短信的目的地，点击下一步后会弹出一个Dialog
![909818-20160928102846781-991540807.png](https://image-static.segmentfault.com/385/306/3853069199-57f9dc9a5ad27_articlex)
点击确定后会调用会调用聚合数据给我们提供的sendCaptcha方法
```
/*点击确定后调用sendCaptcha方法向服务器提交手机号码  
                  code(返回码):   
                  服务器：0 成功，1 错误；   
                  本  地：-2 本地网络异常，-3 服务器网络异常，-4 解析错误，-5 初始化异常。   
                  reason(返回信息):成功或错误的原因。   
                  result(返回结果):JSON格式，错误或者无返回值时为空。   
                */     
smsCaptcha.sendCaptcha(phone,new ResultCallBack() {    
                        
@Override    
public void onResult(int code, String reason, String result) {    
closeDialog();    
if(code==0){    
afterCapterRequested();    
}                           
  }    
    
);
```
当返回的code==0时说明短信验证码发送成功，我们需要进行界面的跳转，代码如下
```
//当code=0成功时，跳转到输入验证码的Activity    
    private void afterCapterRequested() {    
        String phone=etPhoneNumber.getText().toString().trim().replaceAll("\\ss*",     
                "");    
        String code=tvCountryNumber.getText().toString().trim();    
        String fomatedPhone=code+" "+splitPhoneNum(phone);    
            
        Toast.makeText(this,"成功",Toast.LENGTH_SHORT).show();    
        Intent intent=new Intent();    
        intent.setClass(RegistActivity.this,CaptchaActivity.class);    
        //把手机号码和格式化的手机号码传过去    
        intent.putExtra("formatedPhone",fomatedPhone);    
        intent.putExtra("phone",phone);    
        startActivity(intent);    
    }    
        
    //格式化手机号码的方法，格式化后的形式：+86 131 4118 2951    
    private String splitPhoneNum(String phone) {    
            
        StringBuilder builder=new StringBuilder(phone);    
        builder.reverse();    
        for(int i=4,len=builder.length();i<len;i+=5){    
            builder.insert(i,' ');    
        }    
        builder.reverse();    
        return builder.toString();    
    }
```
跳转的界面的布局如下
![909818-20160928102857547-1638254122.png](https://image-static.segmentfault.com/221/975/2219755740-57f9dce8e990e_articlex)
在这个界面我们需要输入短信验证码，输入短信验证码后点击下一步时会调用聚合数据的commitCaptcha这个方法，比对数据的方法，
```
smsCaptcha.commitCaptcha(phoneNumbber,verfiyCode,new ResultCallBack() {    
        /*   
                    code:返回码:   
                    服务器: 0 成功; 1 错误;   
                    本地: -2 本地网络异常; -3 服务器网络异常;-4 解析错误;-5初始化异常   
                    reason:返回信息 成功或错误原因.   
                    result:返回结果,JSON格式.错误或者无返回值时为空.*/     
        @Override    
        public void onResult(int code,String reason,String result) {    
        if(code==0){    
        Log.i(TAG,code+"");    
        Log.i(TAG,reason);    
        Log.i(TAG,result);    
        Toast.makeText(CaptchaActivity.this,"您输入的验证码正确",Toast.LENGTH_SHORT).show();    
        CaptchaActivity.this.finish();    
        }else{    
        Toast.makeText(CaptchaActivity.this,"您输入的验证码不正确",Toast.LENGTH_SHORT).show();    
                }    
            }    
        });
```
我们可以看到这个方法所需的参数是①手机号码②手机验证码，我们调用此方法后聚合数据的服务器会进行判断，此验证码是否是刚才发送给这个手机号的验证码，验证后给我返回结果，我们根据返回的结果就可以进行相应的操作。
在上面的界面中我们发现有一个倒计时的功能，我在这里说一下思路，当然这个倒计时方法有很多。在这个Demo中倒计时是放在一个tvCountDown的一个textView中，首先
```
//R.string.receiveMessgeCountDown的内容为：<Data>接收短信大约需要<font color=#209526>%s</font>秒</Data>    
//这句话就是用time把 html里的%号替换掉    
String unReceive=getResources().getString(R.string. receiveMessgeCountDown,time );    
//把用html格式化的字符串设置给倒计时的文本框    
tvCountDown.setText(Html.fromHtml(unReceive));
```
这样我们可以把html格式化的文本填充到tvCountDown中。接下来就是倒计时的方法了
```
private final int RETRY_INTERVAL =60;    
private int time=RETRY_INTERVAL;    
     //倒计时方法    
     private void countDown() {    
            new Thread( new Runnable() {    
                    
                 @Override    
                 public void run() {    
                 while( time-->0){    
               //减一后的time把<Data>接收短信大约需要<font color=#209526>%s</font>秒</Data>中的%号替换掉    
                String countDownTime=CaptchaActivity.this .getResources().getString(R.string. receiveMessgeCountDown,time );    
              //在主线程上更新tvCountDown    
              upDateTvCountDown(countDownTime);    
              try {    
                 Thread. sleep(1000);    
                  } catch (InterruptedException e) {    
                      e.printStackTrace();    
                     }    
                 }       
                String countDownTime=CaptchaActivity.this .getResources().getString(R.string. unreceiveMessage, time);    
               upDateTvCountDown(countDownTime);    
               time = RETRY_INTERVAL;    
           }    
     }).start();    
}    
//在主线程上更新tvCountDown    
private void upDateTvCountDown(final String countDownTime) {    
           runOnUiThread( new Runnable() {    
                 @Override    
                 public void run() {    
                  tvCountDown.setText(Html. fromHtml(countDownTime));    
                  tvCountDown.setEnabled( false);    
                }    
           });    
     }
```
总结：其实使用聚合数据的短信验证码功能，主要就两个方法，一个是获取验证码的方法sendCaptcha，一个是提交验证码进行验证的方法commitCaptcha。我们在此基础上，可以根据需求对界面进行更改。以上就是对短信验证码的主要逻辑进行的介绍。
