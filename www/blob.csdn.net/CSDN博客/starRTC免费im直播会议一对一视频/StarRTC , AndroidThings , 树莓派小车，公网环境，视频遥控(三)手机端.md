# StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(三)手机端 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月23日 20:07:37[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：271
原文地址： http://blog.starrtc.com/?p=111
这篇来介绍一下整个项目的手机端部分。在上一篇里我们已经将sdk导入到项目中了，下边直接用即可。
1 登录StarRTC的服务
跟小车端一样，第一步需要初始化StarRTCsdk并登录到StarRTC的服务。
跟小车端不同的是，手机端初始化SDK时，不需要设置一些特殊的默认值，小车端是为了适配树莓派才进行了特殊配置。
还有手机端要加动态权限申请。
MLOC.userId = "driver"+ new Random().nextInt(100)+ new Random().nextInt(100);
XHClient.getInstance().initSDK(this, new XHSDKConfig(MLOC.agentId),MLOC.userId);
XHClient.getInstance().getChatManager().addListener(new XHChatManagerListener());
XHClient.getInstance().getLoginManager().addListener(new XHLoginManagerListener());
在确认申请的权限已经得到授权后，可以开始执行登录流程。
首先获取登录授权码AuthKey，然后登录到StarRTC的服务。
    private int times = 0;
    private final int REQUEST_PHONE_PERMISSIONS = 0;
    private void checkPermission(){
        times++;
        final List permissionsList = new ArrayList<>();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M){
            if ((checkSelfPermission(Manifest.permission.READ_PHONE_STATE)!= PackageManager.PERMISSION_GRANTED)) permissionsList.add(Manifest.permission.READ_PHONE_STATE);
            if ((checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE)!= PackageManager.PERMISSION_GRANTED)) permissionsList.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
            if ((checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE)!= PackageManager.PERMISSION_GRANTED)) permissionsList.add(Manifest.permission.READ_EXTERNAL_STORAGE);
            if ((checkSelfPermission(Manifest.permission.CAMERA)!= PackageManager.PERMISSION_GRANTED)) permissionsList.add(Manifest.permission.CAMERA);
            if ((checkSelfPermission(Manifest.permission.BLUETOOTH)!= PackageManager.PERMISSION_GRANTED)) permissionsList.add(Manifest.permission.BLUETOOTH);
            if ((checkSelfPermission(Manifest.permission.RECORD_AUDIO)!= PackageManager.PERMISSION_GRANTED)) permissionsList.add(Manifest.permission.RECORD_AUDIO);
            if (permissionsList.size() != 0){
                if(times==1){
                    requestPermissions(permissionsList.toArray(new String[permissionsList.size()]),
                            REQUEST_PHONE_PERMISSIONS);
                }else{
                    new android.support.v7.app.AlertDialog.Builder(this)
                            .setCancelable(true)
                            .setTitle("提示")
                            .setMessage("获取不到授权，APP将无法正常使用，请允许APP获取权限！")
                            .setPositiveButton("确定", new DialogInterface.OnClickListener() {
                                @Override
                                public void onClick(DialogInterface arg0, int arg1) {
                                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                                        requestPermissions(permissionsList.toArray(new String[permissionsList.size()]),
                                                REQUEST_PHONE_PERMISSIONS);
                                    }
                                }
                            }).setNegativeButton("取消", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface arg0, int arg1) {
                            finish();
                        }
                    }).show();
                }
            }else{
                startAnimation();
                InterfaceUrls.demoLogin(MLOC.userId);//获取登录授权码
            }
        }else{
            startAnimation();
            InterfaceUrls.demoLogin(MLOC.userId);//获取登录授权码
        }
    }
    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull final String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        checkPermission();
    }
@Override
    public void dispatchEvent(String aEventID, boolean success, Object eventObj) {
        switch (aEventID){
            case AEvent.AEVENT_LOGIN:
                //拿到登录授权码，开始登录StarRTC服务
                if(success){
                    MLOC.d("", (String) eventObj);
                    XHClient.getInstance().getLoginManager().login(MLOC.authKey, new IXHCallback() {
                        @Override
                        public void success(Object data) {
                            isLogin = true;
                        }
                        @Override
                        public void failed(final String errMsg) {
                            MLOC.d("",errMsg);
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    MLOC.showMsg(SplashActivity.this,errMsg);
                                }
                            });
                        }
                    });
                }else{
                    MLOC.d("", (String) eventObj);
                }
                break;
            case AEvent.AEVENT_C2C_REV_MSG:
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        MLOC.showMsg(SplashActivity.this,"小车已经启动");
                    }
                });
                XHIMMessage message = (XHIMMessage) eventObj;
                //收到消息，并确认是小车发来的，消息内容为小车创建的直播间ID
                if(message.fromId.equals(waitCarId)){
                    Intent intent = new Intent(SplashActivity.this, VideoLiveActivity.class);
                    intent.putExtra(VideoLiveActivity.LIVE_ID,message.contentData);
                    intent.putExtra(VideoLiveActivity.LIVE_NAME,waitCarId);
                    intent.putExtra(VideoLiveActivity.CREATER_ID,waitCarId);
                    startActivity(intent);
                    removeListener();
                    finish();
                }
                break;
        }
    }
登录成功后的界面是这样的
![](http://blog.starrtc.com/wp-content/uploads/2018/06/476789724849771700.png)
2 与小车建立连接
登录成功后，手机端就可以给小车发启动指令了。这里指定的启动指令为“IotCarStart”,启动指令发送成功后，手机端等待小车端的回复，正常情况下，小车收到指令后会创建互动直播间，并将创建好的直播间ID通过IM消息发送给手机端，手机端再通过互动直播ID进入小车创建的房间，进入房间后再申请连麦，小车会自动通过手机端的申请，至此小车和手机端建连成功。
private void joinLive(){
        //加入直播
        liveManager.watchLive(liveId, new IXHCallback() {
            @Override
            public void success(Object data) {
                //加入直播成功后，自动申请连麦
                liveManager.applyToBroadcaster(createrId);
            }
            @Override
            public void failed(final String errMsg) {
                d("XHLiveManager","watchLive failed "+errMsg);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        showMsg(VideoLiveActivity.this,errMsg);
                        removeListener();
                        finish();
                    }
                });
            }
        });
    }
@Override
    public void dispatchEvent(String aEventID, boolean success, final Object eventObj) {
        d("XHLiveManager","dispatchEvent  "+aEventID + eventObj);
        switch (aEventID){
            case AEvent.AEVENT_LIVE_ADD_UPLOADER:
                try {
                    JSONObject data = (JSONObject) eventObj;
                    final String addId = data.getString("actorID");
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                           if(addId.equals(createrId)){
                               liveManager.attachPlayerView(addId,player1,true);
                           }
                        }
                    });
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                break;
            case AEvent.AEVENT_LIVE_REMOVE_UPLOADER:
               stop();
                break;
            case AEvent.AEVENT_LIVE_APPLY_LINK_RESULT:
                //申请连麦后，收到小车的回复
                if(eventObj== XHConstants.XHLiveJoinResult.XHLiveJoinResult_accept){
                    //切换角色，开始连麦
                    liveManager.changeToBroadcaster();
                }
                break;
            case AEvent.AEVENT_LIVE_ERROR:
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        String errStr = (String) eventObj;
                        showMsg(getApplicationContext(),errStr);
                        removeListener();
                        startActivity(new Intent(VideoLiveActivity.this, SplashActivity.class));
                        finish();
                    }
                });
                break;
        }
    }
建立链接成功后，将能看到小车采集到的实时画面。
![](http://blog.starrtc.com/wp-content/uploads/2018/06/179650760913383699.jpg)
3 给小车发送指令
从操控界面可以看到，左边上下左右有四个按钮，这四个按钮是控制小车运动方向的。
右边有个360度的方向舵，这个是控制小车上云台转动方向的。
左上有个红色的关闭按钮，点击可以停止对小车的操控。
右上有个绿色复位按钮，点击可以使摄像头云台恢复到默认状态，朝向车身正前方。
小车的运动指令有6个：up(前)、down（后）、left（左）、right（右）、start（电机启动）、stop（电机停止）。
每次按下一个方向的按钮时（以up为例），连续发送两个指令 start + up，小车启动并向前，手指从按钮抬起时发送一个指令 stop，小车将停止运动。
云台的运动指令有10个：camera++、camera+-、camera+=、camera-+、camera–、camera-=、camera=+、camera=-、camera==、cameraReset。
其中“+”代表舵机度数增加，“-”代表舵机度数减小，“=”代表舵机度数不变，cameraReset是命令云台复位。
手指触碰方向舵时，会根据触碰的位置不通发送不通的指令，云台的两个舵机会根据指令进行角度调整，手指从方向舵拿开时，会发送camera==，云台停止调整并保持当前的角度，直至收到下一个操控云台的指令。
多说几句，可能是不同舵机的具体情况不一样，我这里写的可能只适用于我的这个小车程序，即便是我自己的程序，舵机的表现也是前后不一的，刚完成的时候，舵机的运动很流畅，但是玩了这么就之后，舵机操作开始有卡顿和颤抖的情况，可能老化了，毕竟是最便宜的小玩应儿，就不强求了。
好了，远程视频遥控小车的相关介绍就到这里了，希望能对大家有所帮助。谢谢观看。
同行的认可是远行最大的动力，欢迎转载本博客文章，转载请注明出处，十分感谢。
[StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(一)准备工作](http://blog.starrtc.com/?p=48)
[StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(二)小车端](http://blog.starrtc.com/?p=94)
[StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(三)手机端](http://blog.starrtc.com/?p=111)
[源码下载地址](https://docs.starrtc.com/download/star_sdk_iot_car.zip)
