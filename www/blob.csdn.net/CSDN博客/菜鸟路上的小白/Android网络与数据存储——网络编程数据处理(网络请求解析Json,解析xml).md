# Android网络与数据存储——网络编程数据处理(网络请求解析Json,解析xml) - 菜鸟路上的小白 - CSDN博客





2016年11月07日 14:32:04[somnus_小凯](https://me.csdn.net/u012486840)阅读数：658








# 一.网络请求

### 1.URL


在真正学习网络请求之前有必要了解一下什么是URL，URL的英文全拼是Uniform Resource Locator，翻译过来为统一资源定位器，大概意思就是URL是指向互联网资源的指针。


这里说的资源可以是文件或目录，也可以是对象的引用，例如对数据库或搜索引擎的查询。


URL的组成：协议名、主机、端口和资源。即满足如下格式：
- `protocol://host:port/resourceName`


例如 baidu的URL地址：
- `https://www.baidu.com`


URL类提供了多个构造器用于创建URL对象，一旦获得了URL对象之后，就可以访问该URL对象对应的资源了。

### 2.Http网络请求


Android中的Http网络请求有两种方式：
- HttpURLConnection
- HttpClient（已不推荐使用）


既然HttpClient已被弃用，那就用HttpURLConnection。


HttpURLConnection继承了URLConnection，可用于向指定网站发送GET请求、POST请求。提供了一下方法：
- HttpURLConnection openConnection()：返回一个HttpURLConnection对象，表示到URL所引用的远程对象的连接。
- int getResponseCode()：获取服务器的响应代码。例如：200表示服务器成功响应，404表示没响应。
- String getResponseMessage()：获取服务器的响应消息。
- String getRequestMethod()：获取发送请求的方法。
- void setRequestMethod()：设置发送请求的方法。
- void setConnectTimeout()：设置连接延迟时间。
- void setReadTimeout()：设置读取延迟时间。


下边写一个请求百度API获取全国城市的方法来使用上面提到的方法。代码如下：
- `public void requestCityData(final String urlString) {`
- `        new Thread(new Runnable() {`
- `            @Override`
- `            public void run() {`
- `                try {`
- `                    URL url = new URL(urlString);`
- `                    HttpURLConnection connection = (HttpURLConnection) url.openConnection();`
- `                    connection.setRequestMethod("GET");`
- `                    connection.setConnectTimeout(8000);`
- `                    connection.setReadTimeout(8000);`
- `                    connection.connect();`
- `                    if (connection.getResponseCode() == 200) {`
- `                        InputStream inputStream = connection.getInputStream();`
- `                        BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));`
- `                        String line;`
- `                        StringBuilder builder = new StringBuilder();`
- `                        while ((line = reader.readLine()) != null) {`
- `                            builder.append(line);`
- `                        }`
- `                        reader.close();`
- `                        String result = builder.toString();`
- `                        Message msg = new Message();`
- `                        msg.what = 0;`
- `                        msg.obj = result;`
- `                        handler.sendMessage(msg);`
- `                    }`
- `                } catch (MalformedURLException e) {`
- `                    e.printStackTrace();`
- `                } catch (IOException e) {`
- `                    e.printStackTrace();`
- `                }`
- `            }`
- `        }).start();`
- `}`


从以上代码可以看出请求网络数据的方法可以分为以下步骤：
- 创建URL对象url。
- 通过url.openConnection()获取HttpURLConnection对象connection。
- 设置请求方式、连接延迟、读取延迟、连接。
- 通过connection.getResponseCode()获取响应码来判断是否连接成功。
- 如果连接成功，接下来的读取文件和Java的读文件操作一样。
- 由于请求网络数据的操作属于耗时操作，所以应该把整个请求操作放在了子线程（处理异步数据有两种方式：handler和AsyncTask，这里用handler进行实现，下一篇中的利用多线程下载会用AsyncTask），最后把请求到的数据通过handler.sendMessage()方法发送到主线程，在handler中的handleMessage()方法中对请求到的数据进行处理。handler代码如下：

Handler handler = new Handler() { @Override public void handleMessage(Message msg) { super.handleMessage(msg); switch (msg.what) { case 0: String cityData = msg.obj.toString(); Log.d("CityActivity", cityData); break;
 } } };



这里的handler只是简单的打印了一下请求到的数据。


到目前为止，还不算请求完成，因为这里用到了网络，要想获取网络上的数据，该应用必须有请求网络的权限，在AndroidManifest.xml中加入网络权限即可。
- `<uses-permission android:name="android.permission.INTERNET" />`

# 二.解析xml


如果请求到的数据为xml类型的，就需要把xml类型的数据解析成我们想要的类型或从xml类型的数据中解析出我们想要的数据。


解析xml类型数据有两种方式：
- SAX：基于事件驱动的解析（解析器+事件处理器），较复杂。
- PULL
- DOM：基于文件流。


SAX、PULL是一个标签一个标签读，分段加载；DOM是整个文件读取出来，然后加载。

### 1.SAX


利用SAX解析如下xml中的id、url以及item text。
- `<?xml version="1.0" encoding="utf-8"?>`
- `<web>`
- `    <item id = "0" url = "http://www.baidu.com" >百度</item>`
- `    <item id = "1" url = "http://www.google.com" >谷歌</item>`
- `    <item id = "2" url = "http://www.sohu.com" >搜狐</item>`
- `</web>`


先新建一个SAXParseHandler类继承自DefaultHandler，并实现startDocument()、startElement()、characters()、endElement()、endDocument()这五个方法。
- `public class SAXParseHandler extends DefaultHandler {`
- `    private List<WebURL> mWebURLs;`
- `    private WebURL mWebURL;`
- `    private boolean state = false;`
- `    @Override`
- `    public void startDocument() throws SAXException {`
- `        mWebURLs = new ArrayList<>();`
- `    }`
- `    @Override`
- `    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {`
- `        mWebURL = new WebURL();`
- `        if (TextUtils.equals(localName, "item")){`
- `            for (int i = 0; i < attributes.getLength(); i++) {`
- `                if (TextUtils.equals(attributes.getLocalName(i), "id")){`
- `                    mWebURL.setmID(Integer.parseInt(attributes.getValue(i)));`
- `                }else if (TextUtils.equals(attributes.getLocalName(i), "url")){`
- `                    mWebURL.setmUrl(attributes.getValue(i));`
- `                }`
- `            }`
- `            state = true;`
- `        }`
- `    }`
- `    @Override`
- `    public void characters(char[] ch, int start, int length) throws SAXException {`
- `        String content = String.valueOf(ch, start, length);`
- `        if (state) {`
- `            mWebURL.setmContent(content);`
- `            state = false;`
- `        }`
- `    }`
- `    @Override`
- `    public void endElement(String uri, String localName, String qName) throws SAXException {`
- `        if (TextUtils.equals(localName, "item")){`
- `            mWebURLs.add(mWebURL);`
- `        }`
- `    }`
- `    @Override`
- `    public void endDocument() throws SAXException {`
- `    }`
- `}`


以上代码中startDocument()表示解析到最外层的标签，进行一些初始化操作，startElement()表示解析到子标签，通过attributes来获取该标签的属性id和url，characters()表示子标签开始标签和结束标签中间的内容，在这里通过转换ch可以获取到，endElement()表示结束子标签，在这个方法中将获取到的信息进行保存，endDocument()表示解析结束。


SAXParseHandler类将xml的每一个标签进行了遍历，接下来就是通过SAX解析器进行解析了。解析代码如下：
- `private void testSAXParse() throws ParserConfigurationException, SAXException, IOException {`
- `        //定义一个factory API，能够配置和获取一个SAX解析器去解析xml`
- `        //SAXParserFactory.newInstance();返回Android的接口SAXParserFactory，不像其他Java接口，该方法不能产生系统属性`
- `        SAXParserFactory saxParserFactory = SAXParserFactory.newInstance();//属性文件或服务API`
- `        //定义一个API包裹XMLReader实现类`
- `        SAXParser saxParser = saxParserFactory.newSAXParser();`
- `        //读一个xml文档的接口，用于回调`
- `        XMLReader xmlReader = saxParser.getXMLReader();`
- `        saxParseHandler = new SAXParseHandler();`
- `        //将saxParseHandler的实例设置到XMLReader中`
- `        xmlReader.setContentHandler(saxParseHandler);`
- `        InputStream inputStream = getResources().openRawResource(R.raw.test);`
- `        InputSource inputSource = new InputSource(inputStream);`
- `        //开始执行解析`
- `        xmlReader.parse(inputSource);`
- `    }`


以上代码中的注释写的很详细，就不在啰嗦了。

### 

# 三.解析Json


Android中解析Json的类有JSONObject和JSONArray。


格式化Json数据的网址：jsonlint.com


开源库：对开源库还未进行研究，待研究了回来补充...
- GSON
- fastJson


通过JSONObject和JSONArray来对下面一段Json数据进行解析。
- `{city_info: [ `
- `{city: "北京", cnty: "中国", id: "CN101010100", lat: "39.904000", lon: "116.391000", prov: "直辖市" }, ... `
- `] }`


写一个方法对以上Json数据进行解析，代码如下：
- `public List<CityInfo> parseCityData(String cityData) {`
- `        List<CityInfo> cityInfos = new ArrayList<>();`
- `        try {`
- `            JSONObject jsonObject = new JSONObject(cityData);`
- `            JSONArray jsonArray = jsonObject.getJSONArray("city_info");`
- `            for (int i = 0; i < jsonArray.length(); i++) {`
- `                JSONObject cityObject = (JSONObject) jsonArray.get(i);`
- `                String city = cityObject.getString("city");`
- `                String prov = cityObject.getString("prov");`
- `                CityInfo cityInfo = new CityInfo(city, prov);`
- `                cityInfos.add(cityInfo);`
- `            }`
- `        } catch (JSONException e) {`
- `            e.printStackTrace();`
- `        }`
- `        return cityInfos;`
- `}`


通过new JSONObject()获取整个JSONObject对象，然后利用getJSONArray()获取city_info对应的Json数组，然后再通过Json数组的get方法获取数组中的每一个JSONObject对象，通过关键字获取我们想要的数据，最后将数据存入列表。整个Json数据解析完毕。

# 四.网络状态处理


网络状态处理：可以判断是否连接网络，还可以区分移动网络流量还是WiFi网络流量
- ConnectivityManager
- NetworkInfo

# 五.扩展
- 下载电影、音乐的本质？

网络请求
- 断点下载

记录http的Head，下载时间，下载了哪些东西。
- 常用网络开源库
- android-async-http
- volley
- OKHttp
- Retrofit

- 封装请求及通用设置
- 封装能用Header
- 请求参数封装

- 封装结果处理
- 能用错误码处理
- 数据转换校验

- 拦截请求设置及代理
- Fiddler（Windows）
- Charles（Mac）
- wifi设置代理

- 技巧
- Postman查询API
- Restful API




