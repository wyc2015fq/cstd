# 练习PopupWindow弹出框之实现界面加载的时候显示弹出框到指定的view下面--两种延迟方法... - weixin_33985507的博客 - CSDN博客
2015年09月12日 11:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
今天在练习PopupWindow弹出框的时候，打算在界面加载的时候将弹出框展现出来并显示在指定的view下面。
初步方法是直接在OnResume方法里面直接执行showPopupWindows方法。
但是报“Unable to add window -- token null is not valid; is your activity running?”
原因参考：[http://cb269267.iteye.com/blog/178777](http://cb269267.iteye.com/blog/1787779)[9](http://cb269267.iteye.com/blog/1787779)
总结下原因如下：popupwindow必须要指定一个view，如果这个view还没有创建显示出来，那么就会报这种错误。
解决思路是：延迟一段时间再执行showPopupWindows方法。上面的链接里面就是这个思路。
**方法一：直接使用Handle延迟**
```
public void onResume() {
    super.onResume();        
    //方式一：直接使用Handle延迟
    Message msg = popHandler.obtainMessage();
    msg.what = 0;//message的what值
    msg.arg1 = 0;//选项卡的下标值，用于匹配popupwindow的背景图片数组中的下标值
    msg.obj = toptab_Items.get(0);//第一个选项卡的view实例
    popHandler.sendMessageDelayed(msg, 500);
}
```
```
private Handler popHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            
            switch (msg.what) {
            case 0:
                
                View view = (View) msg.obj;//选项卡的实例对象
                int index = msg.arg1;//选项卡的下标值，用于匹配popupwindow的背景图片数组中的下标值
                showPopupWindows(view,index);
                break;
            default:
                break;
            }
        }
    };
```
**方法二：通过子线程延迟**
```
public void onResume() {
        super.onResume();
        
        //方式二：子线程延迟
        popHandler.post(runnable);
    }
```
```
private Handler popHandler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            
            switch (msg.what) {
            case 0:
                
                View view = (View) msg.obj;//选项卡的实例对象
                int index = msg.arg1;//选项卡的下标值，用于匹配popupwindow的背景图片数组中的下标值
                showPopupWindows(view,index);
                
                if(timer != null){
                    timer.cancel();
                }
                popHandler.removeCallbacks(runnable);
                
                break;
            default:
                break;
            }
        }
    };
```
```
private Timer timer;
    
    //创建一个子线程
    private Runnable runnable = new Runnable() {
        
        public void run() {
            try {
                timer = new Timer();
                
                TimerTask task = new TimerTask() {
                    
                    public void run() {
                        Message msg = popHandler.obtainMessage();
                        msg.what = 0;//message的what值
                        msg.arg1 = 0;//选项卡的下标值，用于匹配popupwindow的背景图片数组中的下标值
                        msg.obj = toptab_Items.get(0);//第一个选项卡的view实例
                        popHandler.sendMessage(msg);
                    }
                };
                
                timer.schedule(task, 500);
                
            } catch (Exception e) {
                // TODO: handle exception
            }
            
        }
    };
```
![](https://images2015.cnblogs.com/blog/93830/201509/93830-20150930115509246-476402883.png)
备注：
/**显示popupWindow弹出框在指定的view下面*/
 private void showPopupWindows(View view, int index){。。。。}
```
//显示popupWindow弹出框
    private void showPopupWindows(View view, int index){
        
                
        if(popupWindow != null){
            if(popupWindow.isShowing()){
                popupWindow.dismiss();
            }
        }
        
        //一个自定义的布局，作为显示的内容
        View popupWindowView = LayoutInflater.from(getActivity()).inflate(R.layout.fragment_popupwindow, null);
        
        //布局文件中的根节点的背景图片
        LinearLayout popupLayout = (LinearLayout) popupWindowView.findViewById(R.id.popupLayout);
        popupLayout.setBackgroundDrawable(getResources().getDrawable(popupBackGrounds[index]));
        
        //修改文本内容
        TextView message_Text = (TextView) popupWindowView.findViewById(R.id.message_Text);
        message_Text.setText(messages[index]);
        
        //按钮的弹出信息
        Button cancle_btn = (Button) popupWindowView.findViewById(R.id.cancle_btn);
        
        cancle_btn.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                Toast.makeText(getActivity(), "这是一个弹出框", Toast.LENGTH_SHORT).show();
            }
        });
        
        
        //实例化PopupWindow
        popupWindow = new PopupWindow(popupWindowView, LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        
        popupWindow.setTouchable(true);//设置可以触摸
        
        popupWindow.showAsDropDown(view);
        
    }
```
