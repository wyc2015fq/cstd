
# [Android] AlertDialog获取网上天气并显示各城市天气 - 杨秀璋的专栏 - CSDN博客

2014年12月18日 21:29:07[Eastmount](https://me.csdn.net/Eastmount)阅读数：6178所属专栏：[Android实例开发与学习](https://blog.csdn.net/column/details/eastmount-android.html)



**最近帮同学做android百度地图,其中涉及到定位城市天气功能.才知道自己技术非常的一般,还有很多东西需要学习,同时需要回归到我研究生的方向——数据挖掘.同时又见识到了一位叫柳峰的大神,推荐大家去看看他的文章,好像他还是贵州的老乡.博文地址:****http://blog.csdn.net/lyq8479/**
**言归正传,我主要通过两种方法实现：**
**1.第一种方法是通过调用中国天气网信息实现的,它的思想是通过查询静态数据库中个城市对应的WeatherCode,在通过访问中国天气网获取JSON格式数据显示,天气对应的图片也从网上获取.**
**2.第二种方法是通过新浪天气API接口实现的,它只需要输入查询的具体城市或县城即可获取JSON格式天气,而且天气对应图片采用res中静态的对应.**
**参考资料:**
**中国天气网网上资料较多,主要参考郭神《Android第一行代码》**
**[
](http://smart.weather.com.cn/wzfw/smart/weatherapi.shtml)****http://smart.weather.com.cn/wzfw/smart/weatherapi.shtml**
**[http://blog.csdn.net/eyu8874521/article/details/11574485](http://blog.csdn.net/eyu8874521/article/details/11574485)**
**http://blog.csdn.net/lzqwebsoft/article/details/7054045**
**http://blog.csdn.net/pi9nc/article/details/9297085**

## 一. 中国天气网获取天气信息介绍
**中国天气网提供了获取天气及各个城市的代号信息.通过下面两种接口方法获取北京天气,而获取详细信息的接口可能已更改.**
**1.简单信息****http://www.weather.com.cn/data/cityinfo/101010100.html**
```python
{"weatherinfo":{"city":"北京","cityid":"101010100","temp1":"-4℃","temp2":"5℃","weather":"阴转晴","img1":"n2.gif","img2":"d0.gif","ptime":"18:00"}}
```
**2.简单信息****http://www.weather.com.cn/data/sk/101010100.html**
```python
{"weatherinfo":{"city":"北京","cityid":"101010100","temp":"1","WD":"东南风","WS":"2级","SD":"58%","WSE":"2","time":"18:15","isRadar":"1","Radar":"JC_RADAR_AZ9010_JB","njd":"暂无实况","qy":"1024"}}
```
**程序主要通过该URL获取城市的天气,其中101010100表示的就是北京的天气代号,它是如何获取的呢?同样中国天气网提供了网址获取不同城市/县城的天气代号.**
**http://www.weather.com.cn/data/list3/city.xml**
**服务器会返回省份名称+省级代号(一级列表,包括省/直辖市/自治区).如下:**
```python
01|北京,02|上海,03|天津,04|重庆,05|黑龙江,06|吉林,07|辽宁,08|内蒙古,09|河北,10|山西,11|陕西,12|山东,13|新疆,14|西藏,15|青海,16|甘肃,17|宁夏,18|河南,19|江苏,20|湖北,21|浙江,22|安徽,23|福建,24|江西,25|湖南,26|贵州,27|四川,28|广东,29|云南,30|广西,31|海南,32|香港,33|澳门,34|台湾
```
**在输入省级代号获取二级码,如北京city01,贵州city26**
**http://www.weather.com.cn/data/list3/city26.xml**
```python
2601|贵阳,2602|遵义,2603|安顺,2604|黔南,2605|黔东南,2606|铜仁,2607|毕节,2608|六盘水,2609|黔西南
```
**此时你如果你对地理熟悉,你可以发现它们分别是每个省的地区,同样道理可以获取每个地区的县份或区的编码.如输入city2605**
**http://www.weather.com.cn/data/list3/city2605.xml**
```python
260501|凯里,260502|岑巩,260503|施秉,260504|镇远,260505|黄平,260506|从江,260507|麻江,260508|丹寨,260509|三穗,260510|台江,260511|剑河,260512|雷山,260513|黎平,260514|天柱,260515|锦屏,260516|榕江
```
**通过这种方法就可以把全国所有省、市、县地区的天气都实时获取.如下是凯里市对应的天气代号**
**http://www.weather.com.cn/data/list3/city260501.xml**
```python
260501|101260501
```
**到了此处,想必你也知道如果获取凯里市的天气了吧.就是输入网址**
**http://www.weather.com.cn/data/cityinfo/101260501.html**
![](https://img-blog.csdn.net/20141218190927333)
**由于在使用google浏览器获取城市代号时总是报错"This page contains the following errors",你只需要使用IE查看源码就可获取成功.但是我没有通过网上动态获取城市代号,方法知道即可.**
**其中天气图标可通过网站查看,而获取具体图标网址忘记记录了.**
**http://www.weather.com.cn/static/html/legend.shtml**

## 二. 源码显示城市天气
**这里就看着郭神的书实现吧,那个界面挺好看的.同时我自定义的AlertDialog对话框也挺好看的.**
**1.布局文件 activity_main.xml**
```python
<?xml version="1.0" encoding="utf-8"?>
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical" >
    <RelativeLayout
        android:layout_width="match_parent"
        android:layout_height="50dp"
        android:background="#484E61" >
        <Button
            android:id="@+id/close_weather"
            android:layout_width="30dp"
            android:layout_height="30dp"
            android:layout_centerVertical="true"
            android:layout_marginLeft="10dp"
            android:background="@drawable/home" />
        <TextView
            android:id="@+id/city_name"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_centerInParent="true"
            android:textColor="#fff"
            android:textSize="24sp" />
        <Button
            android:id="@+id/refresh_weather"
            android:layout_width="30dp"
            android:layout_height="30dp"
            android:layout_alignParentRight="true"
            android:layout_centerVertical="true"
            android:layout_marginRight="10dp"
            android:background="@drawable/refresh" />
    </RelativeLayout>
    <RelativeLayout
        android:layout_width="match_parent"
        android:layout_height="0dp"
        android:layout_weight="1"
        android:background="#27A5F9" >
        <TextView
            android:id="@+id/publish_text"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_alignParentRight="true"
            android:layout_marginRight="10dp"
            android:layout_marginTop="10dp"
            android:textColor="#FFF"
            android:textSize="18sp" />
        <LinearLayout
            android:id="@+id/weather_info_layout"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_centerInParent="true"
            android:orientation="vertical" >
            <TextView
                android:id="@+id/current_date"
                android:layout_width="wrap_content"
                android:layout_height="40dp"
                android:gravity="center"
                android:textColor="#FFF"
                android:textSize="18sp" />
            <TextView
                android:id="@+id/weather_desp"
                android:layout_width="wrap_content"
                android:layout_height="60dp"
                android:layout_gravity="center_horizontal"
                android:gravity="center"
                android:textColor="#FFF"
                android:textSize="40sp" />
            <LinearLayout
                android:layout_width="wrap_content"
                android:layout_height="60dp"
                android:layout_gravity="center_horizontal"
                android:orientation="horizontal" >
                <TextView
                    android:id="@+id/temp1"
                    android:layout_width="wrap_content"
                    android:layout_height="wrap_content"
                    android:layout_gravity="center_vertical"
                    android:textColor="#FFF"
                    android:textSize="40sp" />
                <TextView
                    android:layout_width="wrap_content"
                    android:layout_height="wrap_content"
                    android:layout_gravity="center_vertical"
                    android:layout_marginLeft="10dp"
                    android:layout_marginRight="10dp"
                    android:text="~"
                    android:textColor="#FFF"
                    android:textSize="40sp" />
                <TextView
                    android:id="@+id/temp2"
                    android:layout_width="wrap_content"
                    android:layout_height="wrap_content"
                    android:layout_gravity="center_vertical"
                    android:textColor="#FFF"
                    android:textSize="40sp" />
            </LinearLayout>
        </LinearLayout>
    </RelativeLayout>
</LinearLayout>
```
**2.在res/drawable-hdpi文件夹中添加两张按钮图片home.png和refresh.png**
**3.MainActivity.java**
```python
public class MainActivity extends Activity implements OnClickListener {
	//自定义变量
	private TextView cityNameText;       //用于显示城市名
	private TextView publishText;        //用于显示发布时间
	private TextView weatherDespText;    //用于显示天气描述信息
	private TextView temp1Text;          //用于显示最低气温
	private TextView temp2Text;          //用于显示最高气温
	private TextView currentDateText;    //用于显示当前日期
	private Button closeWeather;         //退出程序
	private Button refreshWeather;       //更新天气按钮
	private String weatherCode;          //天气代码
	private String weatherJson;          //获取JSON格式
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
        
        //获取对象
        cityNameText = (TextView) findViewById(R.id.city_name);
		publishText = (TextView) findViewById(R.id.publish_text);
		weatherDespText = (TextView) findViewById(R.id.weather_desp);
		temp1Text = (TextView) findViewById(R.id.temp1);
		temp2Text = (TextView) findViewById(R.id.temp2);
		currentDateText = (TextView) findViewById(R.id.current_date);
		closeWeather = (Button) findViewById(R.id.close_weather);
		refreshWeather = (Button) findViewById(R.id.refresh_weather);
		
        //主活动 implements OnClickListener
		closeWeather.setOnClickListener(this);
		refreshWeather.setOnClickListener(this);
    }
    
    @Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.close_weather:
			finish();
			break;
		case R.id.refresh_weather:
			DialogChooseCity();
			break;
		default:
			break;
		}
	}
    
    /**
     * 自定义对话框 获取城市
     * 中国天气网 
     * http://www.weather.com.cn/data/cityinfo/101010100.html
     */
  	private void DialogChooseCity() {
  		//创建对话框
  		AlertDialog.Builder  builder = new AlertDialog.Builder(MainActivity.this);
  		builder.setTitle("请选择一个城市");
  		builder.setIcon(android.R.drawable.ic_dialog_info);
  		//指定下拉列表的显示数据
  		final String[] cities = {"北京", "上海", "天津", "广州", "贵阳", "台北", "香港"};
  		final String[] codes = {"101010100", "101020100", "101030100","101280101",
  				"101260101", "101340101", "101320101"};
  		builder.setItems(cities, new DialogInterface.OnClickListener()
        {
            @Override
            public void onClick(DialogInterface dialog, int which)
            {
            	weatherCode = codes[which];
            	//执行线程访问http
            	//否则 NetworkOnMainThreadException
            	new Thread(new Runnable() {
        			@Override
        			public void run() {
		            	
		            	//访问中国天气网
		            	String weatherUrl = "http://www.weather.com.cn/data/cityinfo/"
								+ weatherCode + ".html";
						weatherJson = queryStringForGet(weatherUrl);
						//JSON格式解析
						try {
							JSONObject jsonObject = new JSONObject(weatherJson);
							JSONObject weatherObject = jsonObject
									.getJSONObject("weatherinfo");
							Message message = new Message();
							message.obj = weatherObject;
							handler.sendMessage(message);
						} catch (JSONException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
        			}
            	}).start();
            }
        });
  		builder.show();
  	}
	
	/**
	 * 解析Json格式数据并显示
	 */
	@SuppressLint("HandlerLeak")
	Handler handler = new Handler() 
	{
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			JSONObject object = (JSONObject) msg.obj;
			try {
				cityNameText.setText(object.getString("city"));
				temp1Text.setText(object.getString("temp1"));
				temp2Text.setText(object.getString("temp2"));
				weatherDespText.setText(object.getString("weather"));
				publishText.setText("今天"+object.getString("ptime")+"发布");
				//获取当前日期
				SimpleDateFormat sdf = new SimpleDateFormat("yyyy年M月d日", Locale.CHINA);
				currentDateText.setText(sdf.format(new Date()));
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	};
	
	/**
	 * 网络查询
	 */
	private String queryStringForGet(String url) {
		HttpGet request = new HttpGet(url);
		String result = null;
		try {
			HttpResponse response = new DefaultHttpClient().execute(request);
			if (response.getStatusLine().getStatusCode() == 200) {
				result = EntityUtils.toString(response.getEntity(), HTTP.UTF_8);
				return result;
			}
		} catch (ClientProtocolException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (ParseException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return result;
	}
}
```
**4.最后在AndroidManifest.xml中声明网络权限**
```python
<!-- 申请权限 -->
<uses-permission android:name="android.permission.INTERNET" />
```
**5.运行效果如下图所示**
![](https://img-blog.csdn.net/20141218203219015)![](https://img-blog.csdn.net/20141218203247968)![](https://img-blog.csdn.net/20141218203317671)
6.需要注意几点
(1)在访问http或获取上传网络数据时,你可能会遇到错误**导致程序崩溃,即错误**NetworkOnMainThreadException.你需要自定义线程实现,通过:
```python
new Thread(new Runnable() {
        @Override
        public void run() {
            //具体操作
            ....
        }
    }).start();
```
**(2)当从网上获取JSON格式数据后,你需要学会JSONObject将数据解析出来.同时如果可能与数据库进行操作,存入db数据库中.**
**(3)如果可能通过自定义DBHelper(继承自SQLiteOpenHelper)和DBManager获取静态数据库中数据,查看对应城市或县份的天气代码.但是重新移植数据库总是报错"no such table:city_table".但是明明存在,可能是数据库路径"/data/data/包名/databases/mydata.db"定义或引用错误,仍未解决.**
**通过SQLiteSpy软件打开db文件,很明显存在city_table表和数据.如下图所示:**
![](https://img-blog.csdn.net/20141218204521437)
**(4)学会自定义AlertDialog并调用它,同时获取网络图片那个URL当时查看到没记录,可惜了.**
**PS:最后推荐看看郭神《Android第一行代码》第十四章关于天气的讲解,但基本原理和方法该文章我都已经讲述了.**


## 三. 新浪天气API实现
**新浪天气API方法同上,但唯一不同点是输入城市名称.这样就不需要再次获取WeatherCode,访问网址如下:**
**http://php.weather.sina.com.cn/iframe/index/w_cl.php?code=js&day=2&city=北京&dfc=3**
**如获取北京的新浪天气,返回的JSON格式如下：**
```python
(function(){var w=[];w['北京']=[{s1:'霾',s2:'阴',f1:'mai',f2:'yin',t1:'5',t2:'-4',p1:'≤3',p2:'3-4',d1:'无持续风向',d2:'北风'},{s1:'晴',s2:'晴',f1:'qing',f2:'qing',t1:'5',t2:'-5',p1:'4-5',p2:'4-5',d1:'北风',d2:'北风'},{s1:'晴',s2:'晴',f1:'qing',f2:'qing',t1:'2',t2:'-5',p1:'3-4',p2:'3-4',d1:'北风',d2:'北风'}];var add={now:'2014-12-18 20:55:04',time:'1418907304',update:'北京时间12月18日17:05更新',error:'0',total:'1'};window.SWther={w:w,add:add};})();//0
```
**程序中显示效果如下图所示,显示的时候是参考了****huanghsh****的模板,但是不知道它源地址.所以见谅!否则推荐大家去下载看看~**
![](https://img-blog.csdn.net/20141218205905474)![](https://img-blog.csdn.net/20141218205924148)
**其中原理与上面相同,通过调用HttpService.getWeather(cityName)函数获取天气.其中自定义类HttpService代码中getWeather如下,而DefaultHttpClient是自定义设置代理.**
```python
public static String getWeather(String city){   
	String result=null;
	String url="http://php.weather.sina.com.cn/iframe/index/w_cl.php?code=js&day=2&city="+city+"&dfc=3";
    try{   
    	DefaultHttpClient client = getDefaultHttpClient2();   
    	HttpGet mothod = new HttpGet(url);   
    	HttpResponse httpResponse = client.execute(mothod);
		if (httpResponse.getStatusLine().getStatusCode() == 200)  
         {  
              result = EntityUtils.toString(httpResponse.getEntity(),"gb2312");  
         }  
    }catch(Exception ex){   
        ex.printStackTrace();   
    }   
    return result;   
}
```
**而****天气图片是通过res/drawable文件夹下静态图片显示,该方法也介绍下!**
```python
private int imageResoId(String weather){
	int resoid=R.drawable.s_2;
	if(weather.indexOf("多云")!=-1||weather.indexOf("晴")!=-1){//多云转晴，以下类同 indexOf:包含字串
        resoid=R.drawable.s_1;}
    else if(weather.indexOf("多云")!=-1&&weather.indexOf("阴")!=-1){
        resoid=R.drawable.s_2;}
    else if(weather.indexOf("阴")!=-1&&weather.indexOf("雨")!=-1){
        resoid=R.drawable.s_3;}
    else if(weather.indexOf("晴")!=-1&&weather.indexOf("雨")!=-1){
        resoid=R.drawable.s_12;}
    else if(weather.indexOf("晴")!=-1&&weather.indexOf("雾")!=-1){
        resoid=R.drawable.s_12;}
    else if(weather.indexOf("晴")!=-1){resoid=R.drawable.s_13;}
    else if(weather.indexOf("多云")!=-1){resoid=R.drawable.s_2;}
    else if(weather.indexOf("阵雨")!=-1){resoid=R.drawable.s_3;}
    else if(weather.indexOf("小雨")!=-1){resoid=R.drawable.s_3;}
    else if(weather.indexOf("中雨")!=-1){resoid=R.drawable.s_4;}
    else if(weather.indexOf("大雨")!=-1){resoid=R.drawable.s_5;}
    else if(weather.indexOf("暴雨")!=-1){resoid=R.drawable.s_5;}
    else if(weather.indexOf("冰雹")!=-1){resoid=R.drawable.s_6;}
    else if(weather.indexOf("雷阵雨")!=-1){resoid=R.drawable.s_7;}
    else if(weather.indexOf("小雪")!=-1){resoid=R.drawable.s_8;}
    else if(weather.indexOf("中雪")!=-1){resoid=R.drawable.s_9;}
    else if(weather.indexOf("大雪")!=-1){resoid=R.drawable.s_10;}
    else if(weather.indexOf("暴雪")!=-1){resoid=R.drawable.s_10;}
    else if(weather.indexOf("扬沙")!=-1){resoid=R.drawable.s_11;}
    else if(weather.indexOf("沙尘")!=-1){resoid=R.drawable.s_11;}
    else if(weather.indexOf("雾")!=-1){resoid=R.drawable.s_12;}
	return resoid;
}
```
**其中自定义对话框输入AlertDialog,载入weather_other_city.xml布局仅有一个输入框ExitText控件.通过Intent传参至显示天气的界面.具体代码如下.**
```python
//输入城市名
private void showOtherCity() {
	LayoutInflater inflater = getLayoutInflater();
	View layout = inflater.inflate(R.layout.weather_other_city,(ViewGroup) findViewById(R.id.ws_dialog));
	dialogCity = (EditText)layout.findViewById(R.id.ws_city_name);
	//创建对话框
    new AlertDialog.Builder(this).setTitle("请输入城市名称").setView(layout)
    	.setPositiveButton("确定",new DialogInterface.OnClickListener() {
		     public void onClick(DialogInterface dialog, int id) {
		    	 cityName = dialogCity.getText().toString();
		    	 Intent intent = new Intent(MainActivity.this, WeatherScreen.class);
		    	 intent.putExtra("extra_data", cityName);
		    	 startActivity(intent);
		       }
    	})
	 .setNegativeButton("取消", new DialogInterface.OnClickListener() {
	     public void onClick(DialogInterface dialog, int id) {
	    	   dialog.cancel();
	       }
	   }).show();
}
```
**由于新浪方法类似,所以就简述了些其他如显示静态图片和AlertDialog的知识.**
**最后简单总结下吧!第一种方法主要通过中国天气网显示信息,其核心是获取天气代码WeatherCode,然后访问URL获取JSON格式数据并显示.第二种方法仅仅是传入城市名称(不要传错),然后获取JSON数据显示天气即可.最后希望文章对大家有所帮助,如果有错误或不足之处,还请海涵~**
**(By:Eastmount 2014-12-18 晚9点****[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)****)**


