# fast-json.jar的使用方法 - DEVELOPER - CSDN博客





2014年10月02日 09:41:02[学术袁](https://me.csdn.net/u012827205)阅读数：3150








fast-json.jar 解析json数据：一种json数据解析方式是这样的，*[点击这里下载jsonfast.jar+fastjsonAPI文档](http://download.csdn.net/detail/u012827205/7997131)*

```java
[
    {
        "id": 6378,
        "title": "test",
        "img": "http://image.jxvdy.com/2014/0929/5428d91c9e6dc8f78fd99_0.png",
        "score": 0,
        "description": "test",
        "time": 1411963174
    },
    {
        "id": 6142,
        "title": "微电影多角度拍摄技巧（三）",
        "img": "http://image.jxvdy.com/old/201409/24/11-54-15-17-1531.jpg",
        "score": 0,
        "description": "",
        "time": 1411530850
    },
    {
        "id": 6141,
        "title": "微电影多角度拍摄技巧（一）",
        "img": "http://image.jxvdy.com/old/201409/24/11-54-04-89-1531.jpg",
        "score": 0,
        "description": "",
        "time": 1411530835
    },
    {
        "id": 6140,
        "title": "微电影多角度拍摄技巧（二）",
        "img": "http://image.jxvdy.com/old/201409/24/11-49-54-18-1531.jpg",
        "score": 0,
        "description": "",
        "time": 1411530552
    },
    {
        "id": 4355,
        "title": "施比受，更有福",
        "img": "http://image.jxvdy.com/old/201409/24/11-46-06-65-3.jpg",
        "score": 0,
        "description": "一位老人用自己的一半时间去帮助他人，赠予帮助，收获快乐",
        "time": 1411530082
    },
    {
        "id": 4354,
        "title": "父子时光之旅",
        "img": "http://image.jxvdy.com/old/201409/24/11-35-13-81-3.jpg",
        "score": 0,
        "description": "当父亲老去，忙于生活的男人没有时间照顾体弱的父亲，于是，带上父亲上路吧，带他重走当年他走过无数遍的那段旅程",
        "time": 1411529699
    }
]
```

对于这一种json数据，使用fastjson进行解析的时候，调用方法之前应该先写出其对应的bean.java（我想你已经做过了）；上面的json数据对应的bean是这样的，

```java
public class NewMoviesBean {

	private int id;
	private String title;
	private String img;
	private String score;
	private String description;
	private int time;
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getImg() {
		return img;
	}
	public void setImg(String img) {
		this.img = img;
	}
	public String getScore() {
		return score;
	}
	public void setScore(String score) {
		this.score = score;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public int getTime() {
		return time;
	}
	public void setTime(int time) {
		this.time = time;
	}
	public NewMoviesBean(int id, String title, String img, String score,
			String description, int time) {
		super();
		this.id = id;
		this.title = title;
		this.img = img;
		this.score = score;
		this.description = description;
		this.time = time;
	}
	public NewMoviesBean() {
		super();
	}
	@Override
	public String toString() {
		return "NewMoviesBean [id=" + id + ", title=" + title + ", img=" + img
				+ ", score=" + score + ", description=" + description
				+ ", time=" + time + "]";
	}
	
	
}
```



那么对应的解析方法是这样的：

```java
JSON.parseArray(json, NewMoviesBean.class);
```

```java
为甚么回事这种解析方式呢？因为，分析整个json数据的格式我们能发现，最外层是中括号"[ ]",内侧是大括号"{ }";中括号说明整个json数据为一个数组类型，其中的大括号说明是数组中的元素；说明整个就是一个JSONArray,JSONArray中元素又是一个个的JSONObject。
```

另一种的解析方式：json数据是这样的，

```java
{"type": [
        "恐怖",
        "剧情"
    ]},
```

分析这种形式，大括号里面是小括号。也即是数组整体是通过键值对的形式呈现的。那么最外层就是一个JSONObject，KEY对应的就是JSONArray。应该这样：

```java
JSONArray jsonArrayType = JSONObject.getJSONArray("type");
			String[] type = new String[jsonArrayType.size()];
			for (int j = 0; j < jsonArrayType.size(); j++) {
				type[j] = (String)jsonArrayType.get(j);
			}
```

这样就能够解析出想要的数据。

与上面类似的另一种解析：json数据是这样的：

```java
{
        "playurl": {
            "360P": "http://v.jxvdy.com/sendfile/V7bzjsH5sIZlBzVG7t7qbL1u-y1_k6E0DCtzyZ8iv-pRF3GmewWOj-HQ_grNppGnnx_rRHb-bztNWAvzGQ",
            "480P": "http://v.jxvdy.com/sendfile/V7bzjsH5sIZlBzVG7t7qbL1u-y1_k6E0DCtzyZ8iv-pRF3GmewWOj-HQ_grNppGnnx_rRHb-bztNWAvzGT",
            "720P": "http://v.jxvdy.com/sendfile/V7bzjsH5sIZlBzVG7t7qbL1u-y1_k6E0DCtzyZ8iv-pRF3GmewWOj-HQ_grNppGnnx_rRHb-bztNWAvzGZ"
        }
    }
```



这种形式，外层大括号里面是一个键KEY对应了另一个大括号元素，那么其最外层是一个JSONObject；内层KEY对应的也是一个JSONObject。

当然也可以先创建开一个bean：

```java
public class MoviedefinitionBean {

	private String normalP;
	private String hightP;
	private String superP;
	public String getNormalP() {
		return normalP;
	}
	public void setNormalP(String normalP) {
		this.normalP = normalP;
	}
	public String getHightP() {
		return hightP;
	}
	public void setHightP(String hightP) {
		this.hightP = hightP;
	}
	public String getSuperP() {
		return superP;
	}
	public void setSuperP(String superP) {
		this.superP = superP;
	}
	public MoviedefinitionBean(String normalP, String hightP, String superP) {
		super();
		this.normalP = normalP;
		this.hightP = hightP;
		this.superP = superP;
	}
	public MoviedefinitionBean() {
		super();
	}
	@Override
	public String toString() {
		return "MoviedefinitionBean [normalP=" + normalP + ", hightP=" + hightP
				+ ", superP=" + superP + "]";
	}
	
}
```



然后对此做出解析：

```java
JSONObject jsonObjectDefination = jsonObject.getJSONObject("playurl");
				String normalP = jsonObjectDefination.getString("360P");
				String hightP = jsonObjectDefination.getString("480P");
				String superP = jsonObjectDefination.getString("720P");
				playurl = new MoviedefinitionBean(normalP, hightP, superP);
```

今天先写到这里|10-02-2014.

今天接着写我遇到的另一种解析类型：先给出json数据：

```java
{
    "1": "爱情",
    "2": "青春",
    "3": "温情",
    "4": "喜剧",
    "5": "悬疑",
    "6": "励志",
    "7": "职场",
    "8": "社会",
    "9": "刑侦",
    "10": "战争",
    "11": "古装",
    "12": "科幻",
    "13": "动作",
    "14": "穿越",
    "15": "广告",
    "16": "公益",
    "17": "恐怖",
    "18": "文艺",
    "19": "纪录",
    "20": "动画",
    "21": "剧情",
    "22": "其他"
}
```



然后对此做出解析；可以看出上面的json数据是一个JSONObject，并且他也是一种以键值对形式存在的类似Map的数据类型。那么也就是说可以使用遍历Map的方式来解析这个JOSNObject。

```java
public static void jsonParseFilterFields(String json){
		ids = new LinkedList<Integer>();
		items = new LinkedList<String>();
		JSONObject jsonObject = JSONObject.parseObject(json);
		Iterator<String> it = jsonObject.keySet().iterator();
		
		while(it.hasNext()){
			String key = it.next();
			ids.add(Integer.valueOf(key));
			items.add(jsonObject.getString(key));
		}
```

待续；2014-10-4

今天有接触到一种快速的解析方法：json数据是这样的 

```java
{
    introduce=小猪的成长历程,
    face=http://www.bejson.com/imgsss,
    sex=男,
    nick=猪猪,
    email=zhuzhu@163.com
}
```

然后我写了一个用户的个人信息的UserBean

```java
public class UserBean {

	private String name;
	private String nick;
	private String face;
	private String info;
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getNick() {
		return nick;
	}
	public void setNick(String nick) {
		this.nick = nick;
	}
	public String getFace() {
		return face;
	}
	public void setFace(String face) {
		this.face = face;
	}
	public String getInfo() {
		return info;
	}
	public void setInfo(String info) {
		this.info = info;
	}
	public UserBean(String name, String nick, String face, String info) {
		super();
		this.name = name;
		this.nick = nick;
		this.face = face;
		this.info = info;
	}
	public UserBean() {
		super();
	}
	@Override
	public String toString() {
		return "UserBean [name=" + name + ", nick=" + nick + ", face=" + face
				+ ", info=" + info + "]";
	}
	
}
```



然后通过fast-json.jar解析：

```java
/**
	 * 获取用户个人的信息
	 * @param json
	 * @return
	 */
	public static UserBean jsonParseUserBean(String json){
		return JSON.parseObject(json, UserBean.class);
	}
```



待续；2014/10/14         


建一个群，方便大家交流：蹦蹦哒Android <群号：423923313>






