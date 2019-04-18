# 安卓Activity生命周期（转） - weixin_33985507的博客 - CSDN博客
2014年06月15日 22:27:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
http://www.oschina.net/question/54100_27841
![](https://images0.cnblogs.com/i/300932/201406/152222047645856.gif)
启动：onCreate()->onStart()->onResume
当从Activity界面直接按Back键，onPause()->onStop()->onDestory()
当从Activity界面按home键。onPause()->onStop()，
接着再进入Activity时onRestart()->onStart()->onResume()
```
public class Activity extends ApplicationContext {
        protected void onCreate(Bundle savedInstanceState);        
        protected void onStart();           
        protected void onRestart();        
        protected void onResume();       
        protected void onPause();        
        protected void onStop();        
        protected void onDestroy();
}
```
