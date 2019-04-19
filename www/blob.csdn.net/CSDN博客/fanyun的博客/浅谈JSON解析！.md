# 浅谈JSON解析！ - fanyun的博客 - CSDN博客
2016年05月26日 09:23:00[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：7297
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
** 什么是JSON？**
         JSON([JavaScript](http://baike.baidu.com/view/16168.htm) Object Notation) 是一种轻量级的数据交换格式。它基于[ECMAScript](http://baike.baidu.com/view/810176.htm)的一个子集。
 JSON采用完全独立于语言的文本格式，但是也使用了类似于C语言家族的习惯（包括[C](http://baike.baidu.com/subview/10075/6770152.htm)、C++、[C#](http://baike.baidu.com/view/6590.htm)、[Java](http://baike.baidu.com/subview/29/12654100.htm)、JavaScript、[Perl](http://baike.baidu.com/view/46614.htm)、[Python](http://baike.baidu.com/view/21087.htm)等）。这些特性使JSON成为理想的数据交换语言。
 易于人阅读和编写，同时也易于机器解析和生成(一般用于提升网络传输速率)。 
** JSON  VS   XML：**
    1.JSON和XML的数据可读性基本相同
    2.JSON和XML同样拥有丰富的解析手段
    3.JSON相对于XML来讲，数据的体积小
    4.JSON与JavaScript的交互更加方便
    5.JSON对数据的描述性比XML较差
    6.JSON的速度要远远快于XML
    android2.3提供的json解析类 ：android的json解析部分都在包org.json下，主要有以下几个类： 
    JsonObject：可以看作是一个JSON对象,这是系统中有关JSON定义的基本单元，其包含一对儿(Key/Value)数值。它对外部(External：   应用toString()方法输出的数值)调用的响应体现为一个标准的字符串（例如：{"JSON": "Hello, World"}，最外被大括号包裹，其中的Key和Value被冒号":"分隔）。其对于内部(Internal)行为的操作格式略微，例如：初始化一个JSONObject实例，引用内部的put()方法添加数值：new JSONObject().put("JSON","Hello,
 World!")，在Key和Value之间是以逗号","分隔。Value的类型包括：Boolean、JSONArray、JSONObject、Number、String或者默认值JSONObject.NULL object 。
    JSONStringer：JSON文本构建类 ，根据官方的解释，这个类可以帮助快速和便捷的创建JSON text。其最大的优点在于可以减少由于 格式的错误导致程序异常，引用这个类可以自动严格按照JSON语法规则（syntax rules）创建JSON text。每个JSONStringer实体只能对应创建一个JSONtext。。其最大的优点在于可以减少由于格式的错误导致程序异常，引用这个类可以自动严格按照JSON语法规则（syntax rules）创建JSON text。每个JSONStringer实体只能对应创建一个JSONtext。
    JSONArray：它代表一组有序的数值。将其转换为String输出(toString)所表现的形式是用方括号包裹，数值以逗号”,”分隔（例如：     [value1,value2,value3]，大家可以亲自利用简短的代码更加直观的了解其格式）。这个类的内部同样具有查询行为，     get()和opt()两种方法都可以通过index索引返回指定的数值，put()方法用来添加或者替换数值。同样这个类的value类型可以包括：Boolean、JSONArray、JSONObject、Number、String或者默认值JSONObject.NULL
 object。
    JSONTokener：json解析类 
    JSONException：json中用到的异常 
   JSONObject, JSONArray来构建JSON文本 
代码  
1. // 假设现在要创建这样一个JSON文本  
   2. //  {  
   3. //      "phone" :["12345678", "87654321"], // 数组  
   4. //      "name" :"yuanzhifei89", // 字符串  
   5. //      "age" : 100, // 数值  
   6. //      "address" : {"country" : "china", "province" :"jiangsu" }, // 对象  
   7. //      "married" : false // 布尔值  
   8. //  }  
   9.   
  10. try {  
  11.     // 首先最外层是{}，是创建一个对象  
  12.     JSONObject person = new JSONObject();  
  13.     // 第一个键phone的值是数组，所以需要创建数组对象  
  14.     JSONArray phone = new JSONArray();  
  15.    phone.put("12345678").put("87654321");  
  16.     person.put("phone", phone);  
  17.   
  18.     person.put("name","yuanzhifei89");  
19.     person.put("age", 100);  
  20.     // 键address的值是对象，所以又要创建一个对象  
  21.     JSONObject address = new JSONObject();  
  22.     address.put("country", "china"); 
  23.     address.put("province","jiangsu");  
  24.     person.put("address", address);   
  25.     person.put("married", false);  
  26. } catch (JSONException ex) {  
  27.     // 键为null或使用json不支持的数字格式(NaN, infinities)  
  28.     throw new RuntimeException(ex);  
  29. }  
getType和optTypeapi的使用 :
    getType可以将要获取的键的值转换为指定的类型，如果无法转换或没有值则抛出JSONException 
    optType也是将要获取的键的值转换为指定的类型，无法转换或没有值时返回用户提供或这默认提供的值 
代码 
 1. try {  
   2.     // 所有使用的对象都是用上面创建的对象  
   3.     // 将第一个电话号码转换为数值和将名字转换为数值  
   4.     phone.getLong(0);  
   5.     person.getLong("name"); // 会抛异常，因为名字无法转换为long        
   6.     phone.optLong(0); // 代码内置的默认值  
   7.     phone.optLong(0, 1000); // 用户提供的默认值  
   8.     person.optLong("name");  
   9.     person.optLong("name", 1000); // 不像上面那样抛异常，而是返回1000  
  10. } catch (JSONException ex) {  
  11.     // 异常处理代码  
  12. }  
除了上面的两个类，还可以使用JSONStringer来构建json文本 
Java代码 ：
   1. try {  
   2.     JSONStringer jsonText = new JSONStringer(); 
   3.     // 首先是{，对象开始。object和endObject必须配对使用  
   4.     jsonText.object();  
   5.       
   6.     jsonText.key("phone");  
   7.     // 键phone的值是数组。array和endArray必须配对使用  
   8.     jsonText.array();  
   9.    jsonText.value("12345678").value("87654321");  
  10.     jsonText.endArray();  
  11.       
  12.     jsonText.key("name");  
  13.     jsonText.value("yuanzhifei89");  
  14.     jsonText.key("age");  
  15.     jsonText.value(100);  
  16.       
  17.     jsonText.key("address");  
  18.     // 键address的值是对象  
  19.     jsonText.object();  
  20.     jsonText.key("country");  
  21.     jsonText.value("china");  
  22.     jsonText.key("province");  
  23.     jsonText.value("jiangsu");  
  24.     jsonText.endObject();  
  25.       
  26.     jsonText.key("married");  
  27.     jsonText.value(false);  
  28.       
  29.     // }，对象结束 
  30.     jsonText.endObject();  
  31. } catch (JSONException ex) {  
  32.     throw new RuntimeException(ex);  
  33. }  
json文本解析类JSONTokener 
按照RFC4627规范将json文本解析为相应的对象。 
对于将json文本解析为对象，只需要用到该类的两个api： 
构造函数 
public Object nextValue(); 
代码  :
1. //  {  
   2. //      "phone" :["12345678", "87654321"], // 数组  
   3. //      "name" :"yuanzhifei89", // 字符串  
   4. //      "age" : 100, // 数值  
   5. //      "address" : { "country": "china", "province" : "jiangsu" }, // 对象  
   6. //      "married" : false // 布尔值  
   7. //  }  
   8.   
   9. private static final String JSON =   
  10. "{" +  
  11.     "   \"phone\" :[\"12345678\", \"87654321\"]," +  
  12.     "   \"name\" :\"yuanzhifei89\"," +  
  13.     "   \"age\" : 100," +  
  14.     "   \"address\" : {\"country\" : \"china\", \"province\" :\"jiangsu\" }," +  
  15.     "   \"married\" : false," + 
  16. "}";  
17.   
  18. try {  
  19.     JSONTokener jsonParser = new JSONTokener(JSON);  
  20.     // 此时还未读取任何json文本，直接读取就是一个JSONObject对象。  
  21.     // 如果此时的读取位置在"name" : 了，那么nextValue就是"yuanzhifei89"（String）  
  22.     JSONObject person = (JSONObject)jsonParser.nextValue();  
  23.     // 接下来的就是JSON对象的操作了  
  24.     person.getJSONArray("phone");  
  25.     person.getString("name");  
  26.     person.getInt("age");  
  27.     person.getJSONObject("address");  
  28.     person.getBoolean("married");  
  29. } catch (JSONException ex) {  
  30.     // 异常处理代码  
  31. }  
其它的api基本就是用来查看json文本中的文本的 代码  ：
1. try {  
   2.     JSONTokener jsonParser = new JSONTokener(JSON); 
   3.     // 继续向下读8个json文本中的字符。此时刚开始，即在{处  
   4.     jsonParser.next(8); //{    "phone。tab算一个字符  
   5.       
   6.     // 继续向下读1个json文本中的字符  
   7.     jsonParser.next(); //"  
   8.       
   9.     // 继续向下读取一个json文本中的字符。该字符不是空白、同时也不是注视中的字符  
  10.     jsonParser.nextClean(); //:  
  11.       
  12.     // 返回当前的读取位置到第一次遇到'a'之间的字符串（不包括a）。  
  13.     jsonParser.nextString('a'); //  ["12345678","87654321"],    "n（前面有两个空格）  
  14.       
  15.     // 返回当前读取位置到第一次遇到字符串中(如"0089")任意字符之间的字符串，同时该字符是trimmed的。（此处就是第一次遇到了89）  
  16.     jsonParser.nextTo("0089"); //me" :"yuanzhifei  
  17.       
  18.     // 读取位置撤销一个  
  19.     jsonParser.back();  
  20.     jsonParser.next(); //i  
  21.       
  22.     // 读取位置前进到指定字符串处（包括字符串）  
  23.     jsonParser.skipPast("address");  
  24.     jsonParser.next(8); //" : { "c  
  25.       
  26.     // 读取位置前进到执行字符处（不包括字符）  
  27.     jsonParser.skipTo('m');  
  28.     jsonParser.next(8); //married"  
  29. } catch (JSONException ex) {  
  30.     // 异常处理代码  
  31. }  
      以下是一个标准的JSON请求实现过程：
01 HttpPost request = new HttpPost(url); 
02 // 先封装一个 JSON 对象 
03 JSONObject param = new JSONObject(); 
04 param.put("name", "rarnu"); 
05 param.put("password", "123456"); 
06 // 绑定到请求 Entry 
07 StringEntity se = new StringEntity(param.toString());  
08 request.setEntity(se); 
09 // 发送请求 
10 HttpResponse httpResponse = new DefaultHttpClient().execute(request); 
11 // 得到应答的字符串，这也是一个 JSON 格式保存的数据 
12 String retSrc = EntityUtils.toString(httpResponse.getEntity()); 
13 // 生成Json对象 
14 JSONObject result = new JSONObject( retSrc); 
15 String token = result.get("token");
        下面这个是自己修改别人的小例子，主要是加一些注释和讲解，这个例子主要是使用android进行json解析。
1 单数据{'singer':{'id':01,'name':'tom','gender':'男'}}
2 多个数据{"singers":[
3        {'id':02,'name':'tom','gender':'男'},
4         {'id':03,'name':'jerry,'gender':'男'},
5 {'id':04,'name':'jim,'gender':'男'},
6 {'id':05,'name':'lily,'gender':'女'}]}
        下面的类主要是解析单个数据parseJson（）和多个数据的方法parseJsonMulti（）:
01 public class JsonActivity extends Activity { 
02    /** Called when the activity is first created. */ 
03    private TextView tvJson; 
04    private Button btnJson; 
05    private Button btnJsonMulti; 
06    @Override 
07    public void onCreate(Bundle savedInstanceState) { 
08        super.onCreate(savedInstanceState); 
09        setContentView(R.layout.main); 
10        tvJson = (TextView) this.findViewById(R.id.tvJson); 
11        btnJson = (Button)this.findViewById(R.id.btnJson); 
12        btnJsonMulti = (Button)this.findViewById(R.id.btnJsonMulti); 
13        btnJson.setOnClickListener(newView.OnClickListener() { 
14            @Override 
15            public void onClick(View v) { 
16                // url 
17                // String strUrl ="http://10.158.166.110:8080/AndroidServer/JsonServlet"; 
18                String strUrl =ServerPageUtil.getStrUrl(UrlsOfServer.JSON_SINGER); 
19                //获得返回的Json字符串 
20                String strResult =connServerForResult(strUrl); 
21                //解析Json字符串 
22               parseJson(strResult); 
23            } 
24        }); 
25        btnJsonMulti.setOnClickListener(newView.OnClickListener() { 
26            @Override 
27            public void onClick(View v) { 
28                String strUrl =ServerPageUtil.getStrUrl(UrlsOfServer.JSON_SINGERS); 
29                String strResult =connServerForResult(strUrl); 
30                //获得多个Singer 
31               parseJsonMulti(strResult); 
32            } 
33        }); 
34    } 
35    private String connServerForResult(String strUrl) { 
36        // HttpGet对象 
37        HttpGet httpRequest = new HttpGet(strUrl); 
38        String strResult = ""; 
39        try { 
40            // HttpClient对象 
41            HttpClient httpClient = newDefaultHttpClient(); 
42            // 获得HttpResponse对象 
43            HttpResponse httpResponse =httpClient.execute(httpRequest); 
44            if (httpResponse.getStatusLine().getStatusCode()== HttpStatus.SC_OK) { 
45                // 取得返回的数据 
46                strResult =EntityUtils.toString(httpResponse.getEntity()); 
47            } 
48        } catch (ClientProtocolException e) { 
49            tvJson.setText("protocolerror"); 
50            e.printStackTrace(); 
51        } catch (IOException e) { 
52            tvJson.setText("IOerror"); 
53            e.printStackTrace(); 
54        } 
55        return strResult; 
56    } 
57    // 普通Json数据解析 
58    private void parseJson(String strResult) { 
59        try { 
60            JSONObject jsonObj = newJSONObject(strResult).getJSONObject("singer"); 
61            int id =jsonObj.getInt("id"); 
62            String name =jsonObj.getString("name"); 
63            String gender =jsonObj.getString("gender"); 
64            tvJson.setText("ID号"+id + ", 姓名：" + name + ",性别：" + gender); 
65        } catch (JSONException e) { 
66            System.out.println("Json parseerror"); 
67            e.printStackTrace(); 
68        } 
69    } 
70    //解析多个数据的Json
71    private void parseJsonMulti(String strResult) { 
72        try { 
73            JSONArray jsonObjs = newJSONObject(strResult).getJSONArray("singers"); 
74            String s = ""; 
75            for(int i = 0; i <jsonObjs.length() ; i++){ 
76                JSONObject jsonObj =((JSONObject)jsonObjs.opt(i)) 
77               .getJSONObject("singer"); 
78                int id =jsonObj.getInt("id"); 
79                String name =jsonObj.getString("name"); 
80                String gender =jsonObj.getString("gender"); 
81                s +=  "ID号"+id + ", 姓名：" + name + ",性别：" + gender+ "\n" ; 
82            } 
83            tvJson.setText(s); 
84        } catch (JSONException e) { 
85            System.out.println("Jsonsparse error !"); 
86            e.printStackTrace(); 
87        } 
88    } 
89 } 
**使用总结：**
**第一步，首先根据网址获取Json格式的字符串，方法如下：**
      以下方法的返回值就是JSON格式的字符串：
  String strurl_1 = "http://api.map.baidu.com/telematics/v3/weather?location=北京&output=json&ak=5slgyqGDENN7Sy7pw29IUvrZ";//北京的天气接口。
   public String connServerForResult(String strUrl) {
// HttpGet对象
HttpGet httpRequest = new HttpGet(strUrl);
String strResult = "";
try {
// HttpClient对象
HttpClient httpClient = new DefaultHttpClient();
HttpParams params = httpClient.getParams();
// 请求超时5秒 接受超时5秒
HttpConnectionParams.setConnectionTimeout(params, 5000);
HttpConnectionParams.setSoTimeout(params, 5000);
// 获得HttpResponse对象
HttpResponse httpResponse = httpClient.execute(httpRequest);
if (httpResponse.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {
// 取得返回的数据
strResult = EntityUtils.toString(httpResponse.getEntity());
} else {
return strResult;
}
} catch (ClientProtocolException e) {
System.out.println("protocol error");
e.printStackTrace();
} catch (IOException e) {
System.out.println("IO error");
e.printStackTrace();
}
return strResult;
   }
**第二步，通过上面的方法已经获得了JSON格式的字符串，假设StringstrResult=connServerForResult(strurl_1);        下面虚线中的的字符串就是根据实际运行获得的JSON格式的字符串strResult：**
  "error":0,
    "status":"success",
    "date":"2014-04-01",
    "results":[
        {
           "currentCity":"\u5317\u4eac",
            "weather_data":[
                {
                   "date":"\u5468\u4e8c(\u4eca\u5929,\u5b9e\u65f6\uff1a22\u2103)",
                   "dayPictureUrl":"http:\/\/www.baidu.com\/aladdin\/img\/new_weath\/bigicon\/7.png",
                   "nightPictureUrl":"http:\/\/api.map.baidu.com\/images\/weather\/night\/yin.png",
                   "weather":"\u973e\u8f6c\u9634",
                   "wind":"\u5fae\u98ce",
                   "temperature":"23 ~ 11\u2103",
                   "dayPictureurl":"http:\/\/api.map.baidu.com\/images\/weather\/day\/mai.png"
                },
                {
                   "date":"\u5468\u4e09",
                   "dayPictureUrl":"http:\/\/api.map.baidu.com\/images\/weather\/day\/yin.png",
                   "nightPictureUrl":"http:\/\/api.map.baidu.com\/images\/weather\/night\/yin.png",
                   "weather":"\u9634",
                   "wind":"\u5fae\u98ce",
                   "temperature":"21 ~ 11\u2103"
                },
                {
                   "date":"\u5468\u56db",
                   "dayPictureUrl":"http:\/\/api.map.baidu.com\/images\/weather\/day\/qing.png",
                   "nightPictureUrl":"http:\/\/api.map.baidu.com\/images\/weather\/night\/qing.png",
                   "weather":"\u6674",
                   "wind":"\u5317\u98ce3-4\u7ea7",
                   "temperature":"21 ~ 10\u2103"
                },
                {
                   "date":"\u5468\u4e94",
                   "dayPictureUrl":"http:\/\/api.map.baidu.com\/images\/weather\/day\/qing.png",
                   "nightPictureUrl":"http:\/\/api.map.baidu.com\/images\/weather\/night\/qing.png",
                   "weather":"\u6674",
                   "wind":"\u5fae\u98ce",
                   "temperature":"24 ~ 9\u2103"
                }
            ]
        }
    ]
}
**第三步：现在已经获取到了JSON格式的字符串，字符串的信息是北京市4天的天气信息，下面就是对它的解析方法。**
       为方便起见，我们假设要获取第一天的气温、天气、日期的信息，即"23 ~ 11\u2103"、"\u973e\u8f6c\u9634"、"\u5468        \u4e8c(\u4eca\u5929,\u5b9e\u65f6\uff1a22\u2103)",那么我们要这么做：
   try {
JSONObject jsonObj1 = new JSONObject(strResult);
JSONArray array1=jsonObj1.getJSONArray("results");
JSONObject array1_2=array1.getJSONObject(0);
JSONArray array1_3=array1_2.getJSONArray("weather_data");
////////////////////////////////////////////////////////////////////////////////////////////////////////////
String weather_0=array1_3.getJSONObject(0).getString("weather");//今天天气
String date_0=array1_3.getJSONObject(0).getString("date");//今天星期
String temp_0 =array1_3.getJSONObject(0).getString("temperature");//今天气温
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////
   } catch(JSONException e) {
System.out.println("Json parse error");
e.printStackTrace();
 }
**第四步:通过第三步已经能够获取第一天的天气信息了，如果要获取第二天或者第三天的信息，只需要把上面斜杠（/）之间的代码中的0改为1或者2就可以了，如果要获取第四天的信息，就改为3，这是因为数组下标是从0开始的。**
        现在你或许会奇怪，为什么上面的信息里面没有汉字，都是些带有反斜杠的字符呢？不要担心，也不要作任何修改，那是国际统一标识码，每一个码对应一个汉字或者其他国家的什么语言，计算机会自动读取出其对应的汉字的。例如System.out.println("\u5fae");显示的一定是一个汉字，无需深究，也没必要知道。
**第五步:下面来详细说明一下第三步是如何解析JSON的，其原理是什么？如果不是数组又该如何解析？ **
        1、首先，请看，上面的JSON格式的字符串整个都是在一对大括号{}里面的，那么这整个大括号里面的全部内容就是一个
        JSONObject对象，那么就通过JSONObject jsonObj1 = new JSONObject(strResult)来获取这个JSONObject对象，其中
        strResult等于那整个大括号包括大括号里面的全部内容的字符串。
        2、想必你已经看到最外层大括号里面有"error"，"status"，"date"，"results"等数据。如果要读取"error"中的值，可以        这么做，String error=jsonObj1.getString("error");至于"status"和"date"中的值的读取方法跟它一样，类比一下就行        了。我们发现"results"的值是一个数组，这是因为JSON中中括号[]就是数组的标志。如果要读取这个数组，你可以这么做
        ，JSONArrayarray1=jsonObj1.getJSONArray("results") 。而你发现数组"results"里面有一对大括号{}，把所有数据都       包含在里面了，嗯，那么这个大括号本身就是这个数组的第0个元素，而这个数组元素是一个JSONObject对象，通过            JSONObject array1_2=array1.getJSONObject(0)来获取。而你又发现array1_2这个JSONObject对象里面有 "currentCity"和
      "weather_data"两个元素，那么通过String currentCity=array1_2.getString("currentCity")来读取"currentCity"的值，
      通过JSONArrayarray1_3=array1_2.getJSONArray("weather_data")来读取这个"weather_data"数组。
       3、此时，你看到，"weather_data"数组里面有4对大括号，那么这4对大括号就代表这个数组中的4个元素，每个元素又是一      个JSONObject对象（这是因为JSONObject对象是以大括号为标志的，大括号里面是键—值对），可以通过JSONObject              ob1=array1_3.getJSONObject(0)来获取，而其中的"weather"的值，则通过String weather_0=ob1.getString("weather")来获     取
  ，我在第三步中没有分开写，而是写在一起了，即Stringweather_0=array1_3.getJSONObject(0).getString("weather");
     但本质上都是一样的。
     4、其实不管是对JSON数组的读取还是对普通对象的读取，其本质上没有大的区别，你只需要记住一点，一对大括号{}就是一个JSONObject对象！一对中括号[]就是一个JSONArray数组！一对大括号即一个JSONObject对象中可以包含多个普通键值对、JSONObject对象和JSONArray数组！一对中括号即一个JSONArray数组中也可以包含多个普通键值对、JSONObject对象和JSONArray数组。
