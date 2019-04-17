# BUG系列：调用startActivityForResult()&onActivityResult()无响应 - DEVELOPER - CSDN博客





2014年10月14日 13:53:07[学术袁](https://me.csdn.net/u012827205)阅读数：2313








```java
前天遇到了一个麻烦，还真是麻烦啊！
```

我使用startActivityForResult()&onActivityResult()，由Activity-A 跳转到Activity-B 页面，然后在Activity-B页面编辑完个人资料之后携带个人数据交回到Activity-A页面；其实很简单的问题，但是 让我没有预料到的是竟然出现了岔子！下面展示我的效果图及正确的代码：

![](https://img-blog.csdn.net/20141014132450380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141014132312328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)这些代码是作为编辑昵称：<Activity-B页面>

```java
public class EditToNickNameAty extends Activity implements OnClickListener{

	private EditText edtNickName;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.aty_thedata_edit_nickname);
		
		findViewById(R.id.btn_back).setOnClickListener(this);
		edtNickName=(EditText) findViewById(R.id.edt_nickName);
		findViewById(R.id.img_nickNameCancell).setOnClickListener(this);
		findViewById(R.id.btnOk).setOnClickListener(this);
		
		
		
		
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		switch (v.getId()) {
		case R.id.img_nickNameCancell://昵称清空设置键
			edtNickName.setText("");
			break;
		case R.id.btnOk:
			nickNameHandleViews();
			break;
		case R.id.btn_back:
			finish();
			overridePendingTransition(R.anim.loadout_from_enter,R.anim.loadout_from_exit);
			break;
		default:
			break;
		}
	}
	
	private void nickNameHandleViews() {
		// TODO Auto-generated method stub
		String result = edtNickName.getText().toString();
		Intent resultFor=new Intent(this, PersonalDataEditAty.class);
		if(TextUtils.isEmpty(result)){
			result = "";
		}
		Bundle data = new Bundle();
		data.putString("result", result);
		resultFor.putExtras(data);
		//设置结果并进行传输
		this.setResult(PersonalDataEditAty.ATY_CODE_NICKENAME, resultFor);
		this.finish();
	}
}
```

Activity-B页面：

跳转按钮作跳转监听：

```java
case R.id.lay_item_nikeName:
			Intent nickName = new Intent(this, EditToNickNameAty.class);
			startActivityForResult(nickName, 10);//ATY_CODE_NICKENAME
			overridePendingTransition(R.anim.loadin_from_enter,R.anim.loadin_from_exit);
			break;
```



接收从Activity-A携带过来的数据：

```java
@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
if(resultCode==ATY_CODE_NICKENAME){//昵称
			Log.d("json", "//昵称"+data.getExtras().getString("result"));
			if(OperationTool.isNetWorkConnected(this)){
				resetNickName = data.getExtras().getString("result");
				get(mHandler, UrlFinalcons.url+"member_edit?"+"token="+token+"&nick="+resetNickName, NormalFinalcons.NICK_NAME);
			}
			
			
		}
```



其中一些细节问题这里提一下：

在onActivityResult()；方法参数中参数--int resultCode  --int requestCode  --Intent data

其中，

resultCode：是Activity-A页面 startActivityForResult(intent,resultCode); 中与该跳转方法的第二个参数匹对且相等；

requestCode：是Activity-B页面 setResult(requestCode, intent);中与该跳转方法的第一个参数匹配且相等；

data：是Activity-B页面 setResult(requestCode, intent);中与该跳转方法的第二个参数匹配且相等；

注意：startActivityForResult(intent,resultCode);中的resultCode最好不要与setResult(requestCode, intent);中的requeCode相等；resultCode>0

*最后解决实际问题：为什么我的所有的配置都做了，就是不能由B------->A的正常跳转呢？*

```java
原因一：与 Activity 的加载模式（launchMode）有关；Activity的加载模式，在这种的跳转的方式下要使用标准模式(我就是使用了 android:launchMode="singleTop"模式，结果不能正常跳转)

原因二：调用startActivityForResult的参数问题，即原先调用时这样：startActivityForResult(intent, 0);是第二个参数的问题，该参数必须大于0才能在返回值，并激活onActivityResult方法。

原因三：在TabHost的子Activity中startActivityForResult调用其他Activity时候遭遇到onActivityResult方法不响应的问题；
```




解决方案：

```java
1.通过父Activity启动其他Activity;

2.实现父Activity的onActivityResult方法,将该处接收到的请求转发给当前活动的子Activity;

3.自定义一个转发接口用来实现第2步中的转发功能;

4.子Activity中实现第3步接口中的方法用来接收返回信息;
```



代码如下:

1.通过父Activity启动其他Activity

 getParent().startActivityForResult(new Intent(...), REQUEST_CODE); 


2.实现父Activity的onActivityResult方法

```java
@Override
     protected void onActivityResult(int requestCode, int resultCode, Intent data) {
         // 获取当前活动的Activity实例
         Activity subActivity = getLocalActivityManager().getCurrentActivity();
         //判断是否实现返回值接口
         if (subActivity instanceof OnTabActivityResultListe<wbr>ner) {
             //获取返回值接口实例
             OnTabActivityResultListe<wbr>ner listener = (OnTabActivityResultListe<wbr>ner) subActivity;
             //转发请求到子Activity
             listener.onTabActivityResult(requestCode, resultCode, data);
         }
         super.onActivityResult(requestCode, resultCode, data);
     }
```

3.自定义接口

```java
public interface OnTabActivityResultListener {

    public void onTabActivityResult(int requestCode, int resultCode, Intent data);

}
```

4.子Activity需要实现onTabActivityResult方法

```java
@Override

    public void onTabActivityResult(int requestCode, int resultCode, Intent data) {

        if (requestCode == REQUEST_CODE && resultCode == RESULT_OK) {

            //实现该处逻辑

　　　　　　　　...

        }

    }
```

```java
/** <span>凡是Activity本身含有上次父类的情况都不能直接使用startActivityForResult，而要通过getParent()，通过父类调用此方法。</span>     */
```





