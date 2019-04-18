# Android系统广播监听网络断开 - weixin_33985507的博客 - CSDN博客
2017年05月14日 22:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
> 
我们的应用有时候需要检测网络断开，并且给用户友好的提示。根据需求，我们觉得利用Android系统广播，当我们收到断开网络的时候，我们弹出一个Dialog。这个时候我们需要新建一个BaseActivity。
```
package com.wj.networkcheckdemo.base;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import com.wj.networkcheckdemo.R;
import com.wj.networkcheckdemo.bean.ActivityController;
public class BaseActivity extends AppCompatActivity {
    private NetWorkChangeBroadcastReceiver myReceiver;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_base);
        ActivityController.setCurrentActivity(this);
        myReceiver = new NetWorkChangeBroadcastReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction("android.net.conn.CONNECTIVITY_CHANGE");
        registerReceiver(myReceiver, filter);
    }
    class NetWorkChangeBroadcastReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            ConnectivityManager connectivityManager =
                    (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
            if (connectivityManager != null) {
                NetworkInfo[] networkInfos = connectivityManager.getAllNetworkInfo();
                for (int i = 0; i < networkInfos.length; i++) {
                    NetworkInfo.State state = networkInfos[i].getState();
                    if (NetworkInfo.State.CONNECTED == state) {
                        //有网络,直接跳出该方法
                        return;
                    }
                }
            }
            final Activity activity = ActivityController.getCurrentActivity();//Dialog依托于Activity。所以该方法用户存储唯一可视的Activity
            if (activity != null) {
                showDialog(activity);
            }
        }
    }
    public void showDialog(Context context) {
        AlertDialog.Builder dialog = new AlertDialog.Builder(context);
        dialog.setTitle("提示")
                .setMessage("您已经断网了,是否重新连接")
                .setCancelable(false)
                .setPositiveButton("确定", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                    }
                }).setNegativeButton("取消", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
            }
        });
        dialog.create().show();
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(myReceiver);
    }
}
```
> 
我们知道Dialog是依托在Activity之上的。所以我们保存了一个Activity对象。
最后别忘记添加两个重要的权限。
```
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
```
