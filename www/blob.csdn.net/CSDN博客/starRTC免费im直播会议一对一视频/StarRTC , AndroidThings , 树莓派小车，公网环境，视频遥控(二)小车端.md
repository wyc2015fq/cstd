# StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(二)小车端 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年06月23日 20:08:27[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：117
原文地址：http://blog.starrtc.com/?p=94
1 创建工程
IDE：Android Studio 3.1；
File>New>New Project>输入项目名>next>Target Android Devices 复选框勾选 Phone and Tablet 和 Android Things>next… finish；
创建成功后项目会包括mobile和things两个module，mobile是手机端程序，things是小车上的程序。
things部分编译出来的是apk只能运行在Android Things系统上，后面我们会在树莓派上运行这module。
2 导入StarRTC SDK
SDK下载：[https://docs.starrtc.com/download/](https://docs.starrtc.com/download/)
开发文档：[https://docs.starrtc.com/docs/android-1.html](https://docs.starrtc.com/docs/android-1.html)
按照开发文档所述，分别在mobile和things两个module下导入StarRTC的sdk。
3 开始码代码，小车端程序（things module）
这个项目的代码大部分都是从StarRTC官网demo源码中拷贝过来的，并做了些简单的修改。
3.1 小车开机后原地待命
小车要先登录StarRTC的服务，后边才能接收到手机端的启动指令。
//设置小车的ID，后边要通过这个ID来唤醒小车
MLOC.userId = "car0001";
//设置适合树莓派的默认参数值
XHClient.getInstance().setDefaultConfig(true,true,0,0,1,false,false,false,false,XHConstants.XHCropTypeEnum.STAR_VIDEO_CROP_CONFIG_BIG_NOCROP_SMALL_NONE);
XHClient.getInstance().setCustomEncoderConfig(640,480,640,480,15,500,45);
//初始化SDK，添加登录和消息状态监听
XHClient.getInstance().initSDK(this, new XHSDKConfig(MLOC.agentId),MLOC.userId);
XHClient.getInstance().getChatManager().addListener(new XHChatManagerListener());
XHClient.getInstance().getLoginManager().addListener(new XHLoginManagerListener());
checkNetAvailable();
这里有个问题要注意一下，本来这段初始化代码最后，就要进行登录操作了，但树莓派开机时，会第一时间自动运行小车的程序，这样就可能遇到程序在进行初始化时，网络未连接或设备时间未同步的问题。这两个问题都会导致登录失败。所以登录之前先检查一下网络是否已经可用，时间是否已经同步。这里只是判断了时间中的year部分是否包含”201″。
    private Timer checkNetTimer = new Timer();
    private void checkNetAvailable(){
        checkNetTimer.schedule(new TimerTask() {
            @Override
            public void run() {
                Runtime runtime = Runtime.getRuntime();
                Process pingProcess = null;
                try {
                    String nowDate = new SimpleDateFormat("yyyy-MM-dd HH:mm").format(new java.util.Date());
                    //时间是否已经同步
                    if(nowDate.contains("201")){
                        checkNetTimer.cancel();
                        InterfaceUrls.demoLogin(MLOC.userId);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        },3000,3000);
    }
上边代码中InterfaceUrls.demoLogin(MLOC.userId)是向服务器获取登录证明AuthKey的，拿到证明之后就是真正的登录了。这里我通过自定义的事件来专递各种消息和参数。
登录SDK成功后，小车将处于待命状态，等待操控者的启动指令。这里使用的启动指令是“IotCarStart”这个字符串。
@Override
    public void dispatchEvent(String aEventID, boolean success, Object eventObj) {
        switch (aEventID){
            case AEvent.AEVENT_LOGIN:
                if(success){
                    MLOC.d("", (String) eventObj);
                    //登录SDK
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
                XHIMMessage message = (XHIMMessage) eventObj;
                String command = message.contentData;
                switch (command){
                    case "IotCarStart":
                        startCar(message.fromId);
                        break;
                }
                break;
        }
    }
    private void startCar(String fromId){
        removeListener();
        Intent intent = new Intent(SplashActivity.this,SampleLiveActivity.class);
        intent.putExtra("driverId",fromId);
        startActivity(intent);
    }
3.2 小车收到开机指令
当手机端通过IM，向小车发送一条内容为“IotCarStart”的一对一消息时，小车就会启动，并且记录操控者的ID，直到本次遥控结束，小车将忽略其他人发来的一切消息。
小车启动时，要创建一个互动直播的房间，并开始直播。开始直播后，通过一对一消息将直播间的id发给手机端。手机端通过直播间id进入直播间，并申请连麦。小车收到连麦申请后判断是否是操控者发来的，是操控者将允许连麦，否则拒绝连麦。
     private void createLive(){
        //创建直播
        final XHLiveItem liveItem = new XHLiveItem();
        liveItem.setLiveType(XHConstants.XHLiveType.XHLiveTypeGlobalPublic);
        liveItem.setLiveName(MLOC.userId);
        //创建直播间
        liveManager.createLive(liveItem, new IXHCallback() {
            @Override
            public void success(Object data) {
                //创建成功
                MLOC.d("XHLiveManager","createLive success "+data);
                liveId = (String) data;
                MLOC.saveSharedData(SampleLiveActivity.this,MLOC.userId+"_iotCarId", liveId);
                InterfaceUrls.demoReportLive(liveId,liveItem.getLiveName(),MLOC.userId);
                starLive();
            }
            @Override
            public void failed(final String errMsg) {
                //创建失败
                MLOC.d("XHLiveManager","createLive failed "+errMsg);
                removeListener();
                finish();
            }
        });
    }
    private void starLive(){
        //开始直播
        liveManager.startLive(liveId, new IXHCallback() {
            @Override
            public void success(Object data) {
                //成功
                MLOC.d("XHLiveManager","startLive success "+data);
                //给操控者发送直播间ID
                XHClient.getInstance().getChatManager().sendOnlineMessage(liveId, driverId, null);
            }
            @Override
            public void failed(final String errMsg) {
                //失败
                MLOC.d("XHLiveManager","startLive failed "+errMsg);
                MLOC.saveSharedData(SampleLiveActivity.this,MLOC.userId+"_iotCarId", "");
                stop();
            }
        });
    }
 @Override
    public void dispatchEvent(String aEventID, boolean success, final Object eventObj) {
        MLOC.d("XHLiveManager","dispatchEvent  "+aEventID + eventObj);
        switch (aEventID){
            case AEvent.AEVENT_C2C_REV_MSG:
                XHIMMessage message = (XHIMMessage) eventObj;
                if(message.fromId.equals(driverId)){
                    String command = message.contentData;
                    if(command.equals("IotCarStart")){
                        XHClient.getInstance().getChatManager().sendOnlineMessage(liveId, driverId, null);
                    }
                }
                break;
            case AEvent.AEVENT_LIVE_ADD_UPLOADER:
                //连麦者加入，因为小车不需要播放，所以设置为不接收视频
                StarRtcCore.getInstance().setNullVideo();
                break;
            case AEvent.AEVENT_LIVE_REMOVE_UPLOADER:
                //操控者退出，本次操控结束
                driverId = "";
                stop();
                break;
            case AEvent.AEVENT_LIVE_APPLY_LINK:
                //收到连麦申请
                if(driverId.equals((String) eventObj)){
                    // 操控者申请，自动同意上麦
                    liveManager.agreeApplyToBroadcaster(driverId);
                }else{
                    // 拒绝其他人上麦
                    liveManager.refuseApplyToBroadcaster((String) eventObj);
                }
                break;
            case AEvent.AEVENT_LIVE_ERROR:
                removeListener();
                finish();
                MLOC.d("VideoLiveActivity","AEVENT_LIVE_ERROR  "+eventObj);
                break;
            case AEvent.AEVENT_LIVE_REV_REALTIME_DATA:
                // 收到实时流数据，操控车的指令
                if(success){
                    try {
                        JSONObject jsonObject = (JSONObject) eventObj;
                        final byte[] tData = (byte[]) jsonObject.get("data");
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                //给小车下达指令
                                GpioManager.getInstance().controlCar(tData);
                            }
                        });
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                }
                break;
        }
    }
4 通过树莓派GPIO和PWM控制小车运动
4.1 GPIO控制驱动电机
我购买的直流电机驱动板可以直接插在树莓派上，并将树莓的引脚再次暴露出来，用起来比较方便。
根据驱动板使用说明，将电机线接到驱动板上，然后通过GPIO设置相应引脚的高低电平就能控制电机的启动停止和正转反转。
使用GPIO要记得先申请相关权限
GPIO使用前记得重置，使用后记得销毁，GPIO口一旦被占用，后来者将无法使用。
    //初始化小车需要的GPIO口
    public void initCarGpio(){
        manager = PeripheralManager.getInstance();
        try {
            mGpioLeftRun = manager.openGpio(GpioNameLeftRun);
            resetGpio(mGpioLeftRun);
            mGpioLeftDirection = manager.openGpio(GpioNameLeftDirection);
            resetGpio(mGpioLeftDirection);
            mGpioRightRun = manager.openGpio(GpioNameRightRun);
            resetGpio(mGpioRightRun);
            mGpioRightDirection = manager.openGpio(GpioNameRightDirection);
            resetGpio(mGpioRightDirection);
        } catch (IOException e) {
            MLOC.d("IOTCAR","initCarGpio IOException"+e.getMessage());
            e.printStackTrace();
        }
        MLOC.d("IOTCAR","initCarGpio");
    }
    //关闭车
    public void stopCarGpio(){
        destoryGpio(mGpioLeftRun);
        destoryGpio(mGpioLeftDirection);
        destoryGpio(mGpioRightRun);
        destoryGpio(mGpioRightDirection);
        MLOC.d("IOTCAR","stopCarGpio");
    }
    //重置GPIO
    private void resetGpio(Gpio gpio){
        try {
            if(gpio!=null) {
                gpio.setDirection(Gpio.DIRECTION_OUT_INITIALLY_LOW);//设置为输出，默认低电平
                gpio.setActiveType(Gpio.ACTIVE_HIGH);//设置高电平为活跃的
                gpio.setValue(false);//设置成低电平
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    //销毁GPIO
    private void destoryGpio(Gpio gpio){
        try {
            if(gpio!=null){
                gpio.close();
                gpio = null;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
在这吐糟一下树莓派和电机驱动板，树莓派每次通电开机时，控制电机使能的两个GPIO口默认输出的都是高电平，导致小车每次开机时会不受控制的一直往前跑。直到开始运行程序，GPIO口被重置成低电平才会停下来…
4.2 PWM控制云台舵机旋转
摄像头云台的控制需要使用PWM，Android Things在树莓派上提供了两个可以生成PWM波的引脚，正好可以将云台的两个舵机接入到树莓派的相应PWM引脚上，一个控制左右转动，一个控制上上下转动。
这里需要说一下自己碰到的坑，刚开始不知道Android Things提供了PWM的API，所以自己写了一个SoftPWM。但是PWM的周期是20ms,控制舵机从0到180度转动所需的高电平宽度是0.5ms-1.5ms,然而java的计时器最小单位就是ms,所以根本无法满足舵机调节角度的精度需求。幸亏很快发现了系统提供的API。之后又遇到个坑，只要小车打开视频，云台就无法控制。后来定位到问题是，播放声音时speaker也是通过PWM控制发声的，也就是说PWM被系统拿去播放音频了，所以云台无法控制。解决这个问题只需要再加一个PWM发生器即可，我选择了另一条路，禁用了小车的音频输出。
继续说舵机和PWM，PWM是通过设置频率和占空比来产生不通方波的，理论上舵机的0~180度对应的高电平宽是0.5ms~1.5ms,也就是0度的占空比 = 0.5ms/20ms = 2.5%,可能因为理论值和实际值有偏差，经过我的测试，设置占空比为3.27%时，也就是高电平宽w = 3.27%*20ms = 0.654ms时，舵机的角度为0度。后边又测出了180度时的占空比值，最终计算出角度每增加1度，占空比增加约0.0463。有了0度的基础值和单步值，后边设置角度时就比较方便了。
比如 90度时PWM的占空比=3.27+90*0.0463。
PWM最好持续输出，时断时续的容易造成舵机无规则摆动，停止遥控时一定要记得停止你使用的PWM，不然舵机一直工作，影响舵机使用寿命和电池续航。
    public void initCarPwm(){
        MLOC.d("IOTCAR_PWM","initCarPwm");
        manager = PeripheralManager.getInstance();
        try {
            if(mPwmCameraH!=null) {
                mPwmCameraH.setEnabled(false);
                mPwmCameraH.close();
                mPwmCameraH = null;
            }
            running.set(true);
            mPwmCameraH = manager.openPwm(GpioNameHRotate);
            mPwmCameraH.setPwmDutyCycle(beginValue+90*stepLenght);//设置占空比
            mPwmCameraH.setPwmFrequencyHz(50);//设置频率 
            mPwmCameraH.setEnabled(true); //开始生成PWM
            if(mPwmCameraV!=null) {
                mPwmCameraV.setEnabled(false);
                mPwmCameraV.close();
                mPwmCameraV = null;
            }
            mPwmCameraV = manager.openPwm(GpioNameVRotate);
            mPwmCameraV.setPwmDutyCycle(beginValue+40*stepLenght);
            mPwmCameraV.setPwmFrequencyHz(50);
            mPwmCameraV.setEnabled(true);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void changePwm(){
        try {
            if(mPwmCameraV!=null&&lastCameraV!=camearV.get()){
                mPwmCameraV.setEnabled(false);//先停掉之前的PWM
                mPwmCameraV.setPwmDutyCycle(beginValue+camearV.get()*stepLenght);//重新设置占空比
                mPwmCameraV.setEnabled(true);//开始生成PWM
                lastCameraV = camearV.get();
            }
            if(mPwmCameraH!=null&&lastCameraH!=camearH.get()){
                mPwmCameraH.setEnabled(false);
                mPwmCameraH.setPwmDutyCycle(beginValue+camearH.get()*stepLenght);
                mPwmCameraH.setEnabled(true);
                lastCameraH = camearH.get();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void stopPwm(){
        running.set(false);
        if(mPwmCameraH!=null){
            try {
                mPwmCameraH.setEnabled(false);
                mPwmCameraH.close();
                mPwmCameraH = null;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        if(mPwmCameraV!=null){
            try {
                mPwmCameraV.setEnabled(false);
                mPwmCameraV.close();
                mPwmCameraV = null;
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        MLOC.d("IOTCAR_PWM","stopPwm");
    }
到这里小车端的程序介绍完了，后边再说说手机端。
同行的认可是远行最大的动力，欢迎转载本博客文章，转载请注明出处，十分感谢。
[StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(一)准备工作](http://blog.starrtc.com/?p=48)
[StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(二)小车端](http://blog.starrtc.com/?p=94)
[StarRTC , AndroidThings , 树莓派小车，公网环境，视频遥控(三)手机端](http://blog.starrtc.com/?p=111)
[源码下载地址](https://docs.starrtc.com/download/star_sdk_iot_car.zip)
[](https://docs.starrtc.com/download/star_sdk_iot_car.zip)
