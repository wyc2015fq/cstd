# [项目回顾]基于Redis的在线用户列表解决方案 - z69183787的专栏 - CSDN博客
2016年08月30日 18:21:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1521
# 前言：
　　由于项目需求，需要在集群环境下实现在线用户列表的功能，并依靠在线列表实现用户单一登陆（同一账户只能一处登陆）功能：
　　在单机环境下，在线列表的实现方案可以采用SessionListener来完成，当有Session创建和销毁的时候做相应的操作即可完成功能及将相应的Session的引用存放于内存中，由于持有了所有的Session的引用，故可以方便的实现用户单一登陆的功能（比如在第二次登陆的时候使之前登陆的账户所在的Session失效）。
　　而在集群环境下，由于用户的请求可能分布在不同的Web服务器上，继续将在线用户列表储存在单机内存中已经不能满足需要，不同的Web服务器将会产生不同的在线列表，并且不能有效的实现单一用户登陆的功能，因为某一用户可能并不在接受到退出请求的Web服务器的在线用户列表中（在集群中的某台服务器上完成的登陆操作，而在其他服务器上完成退出操作）。
　　现有解决方案：
　　1.将用户的在线情况记录进入数据库中，依靠数据库完成对登陆状况的检测
　　2.将在线列表放在一个公共的缓存服务器上
　　由于缓存服务器可以为缓存内容设置指定有效期，可以方便实现Session过期的效果，以及避免让数据库的读写性能成为系统瓶颈等原因，我们采用了Redis来作为缓存服务器用于实现该功能。
# 单机环境下的解决方案：
　　基于HttpSessionListener：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 import java.util.Date;
 2 import java.util.Hashtable;
 3 import java.util.Iterator;
 4 
 5 import javax.servlet.http.HttpSession;
 6 import javax.servlet.http.HttpSessionEvent;
 7 import javax.servlet.http.HttpSessionListener;
 8 
 9 import com.xxx.common.util.StringUtil;
10 
11 /**
12  * 
13  * @ClassName: SessionListener
14  * @Description: 记录所有登陆的Session信息，为在线列表做基础
15  * @author BuilderQiu
16  * @date 2013-9-18 09:35:13
17  *
18  */
19 public class SessionListener implements HttpSessionListener {
20     
21     //在线列表<uid,session>
22     private static Hashtable<String,HttpSession> sessionList = new Hashtable<String, HttpSession>();
23     
24 
25     public void sessionCreated(HttpSessionEvent event) {
26         //不做处理，只处理登陆用户的列表
27         
28     }
29 
30     public void sessionDestroyed(HttpSessionEvent event) {
31         removeSession(event.getSession());
32     }
33     
34     public static void removeSession(HttpSession session){
35         if(session == null){
36             return ;
37         }
38 
39         String uid=(String)session.getAttribute("clientUserId");//已登陆状态会将用户的UserId保存在session中
40         if(!StringUtil.isBlank(uid)){//判断是否登陆状态
41             removeSession(uid);
42         }
43     }
44     
45     public static void removeSession(String uid){
46         HttpSession session = sessionList.get(uid);
47         try{
48             sessionList.remove(uid);//先执行，防止session.invalidate()报错而不执行
49             if(session != null){
50                 session.invalidate();
51             }
52         }catch (Exception e) {
53             System.out.println("Session invalidate error!");
54         }
55     }
56     
57     public static void addSession(String uid,HttpSession session){
58         sessionList.put(uid, session);
59     }
60     
61     public static int getSessionCount(){
62         return sessionList.size();
63     }
64     
65     public static Iterator<HttpSession> getSessionSet(){
66         return sessionList.values().iterator();
67     }
68     
69     public static HttpSession getSession(String id){
70         return sessionList.get(id);
71     }
72     
73     public static boolean contains(String uid){
74         return sessionList.containsKey(uid);
75     }
76     
77     /**
78      * 
79      * @Title: isLoginOnThisSession
80      * @Description: 检测是否已经登陆
81      * @param @param uid 用户UserId
82      * @param @param sid 发起请求的用户的SessionId
83      * @return boolean true 校验通过 
84      */
85     public static boolean isLoginOnThisSession(String uid,String sid){
86         if(uid==null||sid==null){
87             return false;
88         }
89         if(contains(uid)){
90             HttpSession session = sessionList.get(uid);
91             
92             if(session!=null&&session.getId().equals(sid)){
93                 return true;
94             }
95         }
96         return false;
97     }
98     
99 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　用户的在线状态全部维护记录在sessionList中，并且可以通过sessionList获取到任意用户的session对象，可以用来完成使指定用户离线的功能(调用该用户的`session.invalidate()`方法)。
　　用户登录的时候调用`addSession(uid,session)`方法将用户与其登录的Session信息记录至sessionList中，再退出的时候调用`removeSession(session) or removeSession(uid)`方法，在强制下线的时候调用`removeSession(uid)`方法，以及一些其他的操作即可实现相应的功能。
# 基于Redis的解决方案：
　　该`解决方案的实质是将在线列表的所在的内存共享出来，让集群环境下所有的服务器都能够访问到这部分数据，并且将用户的在线状态在这块内存中进行维护。`
　　Redis连接池工具类：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 import java.util.ResourceBundle;
 2 
 3 import redis.clients.jedis.Jedis;
 4 import redis.clients.jedis.JedisPool;
 5 import redis.clients.jedis.JedisPoolConfig;
 6 
 7 public class RedisPoolUtils {
 8     
 9     private static final JedisPool pool;
10     
11     static{
12         ResourceBundle bundle = ResourceBundle.getBundle("redis");
13         JedisPoolConfig config = new JedisPoolConfig();
14         if (bundle == null) {    
15             throw new IllegalArgumentException("[redis.properties] is not found!");    
16         }
17         //设置池配置项值  
18         config.setMaxActive(Integer.valueOf(bundle.getString("jedis.pool.maxActive")));    
19         config.setMaxIdle(Integer.valueOf(bundle.getString("jedis.pool.maxIdle")));    
20         config.setMaxWait(Long.valueOf(bundle.getString("jedis.pool.maxWait")));    
21         config.setTestOnBorrow(Boolean.valueOf(bundle.getString("jedis.pool.testOnBorrow")));    
22         config.setTestOnReturn(Boolean.valueOf(bundle.getString("jedis.pool.testOnReturn")));
23         
24         pool = new JedisPool(config, bundle.getString("redis.ip"),Integer.valueOf(bundle.getString("redis.port")) );
25     }
26     
27     /**
28      * 
29      * @Title: release
30      * @Description: 释放连接
31      * @param @param jedis
32      * @return void
33      * @throws
34      */
35     public static void release(Jedis jedis){
36         pool.returnResource(jedis);
37     }
38     
39     public static Jedis getJedis(){
40         return pool.getResource();
41     }
42 
43 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　Redis在线列表工具类：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
  1 import java.util.ArrayList;
  2 import java.util.Collections;
  3 import java.util.Comparator;
  4 import java.util.Date;
  5 import java.util.List;
  6 import java.util.Set;
  7 
  8 import net.sf.json.JSONObject;
  9 import net.sf.json.JsonConfig;
 10 import net.sf.json.processors.JsonValueProcessor;
 11 
 12 import cn.sccl.common.util.StringUtil;
 13 
 14 import com.xxx.common.util.JsonDateValueProcessor;
 15 import com.xxx.user.model.ClientUser;
 16 
 17 import redis.clients.jedis.Jedis;
 18 import redis.clients.jedis.Pipeline;
 19 import tools.Constants;
 20 
 21 /**
 22  * 
 23  * Redis缓存中存放两组key：
 24  * 1.SID_PREFIX开头，存放登陆用户的SessionId与ClientUser的Json数据
 25  * 2.UID_PREFIX开头，存放登录用户的UID与SessionId对于的数据
 26  *
 27  * 3.VID_PREFIX开头，存放位于指定页面用户的数据（与Ajax一起使用，用于实现指定页面同时浏览人数的限制功能）
 28  * 
 29  * @ClassName: OnlineUtils
 30  * @Description: 在线列表操作工具类
 31  * @author BuilderQiu
 32  * @date 2014-1-9 上午09:25:43
 33  *
 34  */
 35 public class OnlineUtils {
 36     
 37     //KEY值根据SessionID生成    
 38     private static final String SID_PREFIX = "online:sid:";
 39     private static final String UID_PREFIX = "online:uid:";
 40     private static final String VID_PREFIX = "online:vid:";
 41     private static final int OVERDATETIME = 30 * 60;
 42     private static final int BROADCAST_OVERDATETIME = 70;//Ajax每60秒发起一次，超过BROADCAST_OVERDATETIME时间长度未发起表示已经离开该页面
 43 
 44     public static void login(String sid,ClientUser user){
 45         
 46         Jedis jedis = RedisPoolUtils.getJedis();
 47 
 48         jedis.setex(SID_PREFIX+sid, OVERDATETIME, userToString(user));
 49         jedis.setex(UID_PREFIX+user.getId(), OVERDATETIME, sid);
 50         
 51         RedisPoolUtils.release(jedis);
 52     }
 53     
 54     public static void broadcast(String uid,String identify){
 55         
 56         if(uid==null||"".equals(uid)) //异常数据，正常情况下登陆用户才会发起该请求
 57             return ;
 58         
 59         Jedis jedis = RedisPoolUtils.getJedis();
 60         
 61         jedis.setex(VID_PREFIX+identify+":"+uid, BROADCAST_OVERDATETIME, uid);
 62         
 63         RedisPoolUtils.release(jedis);
 64     }
 65     
 66     
 67     private static String userToString(ClientUser user){
 68         JsonConfig  config = new JsonConfig();
 69         JsonValueProcessor processor = new JsonDateValueProcessor("yyyy-MM-dd HH:mm:ss");
 70         config.registerJsonValueProcessor(Date.class, processor);
 71         JSONObject obj = JSONObject.fromObject(user, config);
 72 
 73         return obj.toString();
 74     }
 75     
 76     /**
 77      * 
 78      * @Title: logout
 79      * @Description: 退出
 80      * @param @param sessionId
 81      * @return void
 82      * @throws
 83      */
 84     public static void logout(String sid,String uid){
 85         
 86         Jedis jedis = RedisPoolUtils.getJedis();
 87         
 88         jedis.del(SID_PREFIX+sid);
 89         jedis.del(UID_PREFIX+uid);
 90         
 91         RedisPoolUtils.release(jedis);
 92     }
 93     
 94     /**
 95      * 
 96      * @Title: logout
 97      * @Description: 退出
 98      * @param @param UserId  使指定用户下线
 99      * @return void
100      * @throws
101      */
102     public static void logout(String uid){
103         Jedis jedis = RedisPoolUtils.getJedis();
104         
105         //删除sid
106         jedis.del(SID_PREFIX+jedis.get(UID_PREFIX+uid));
107         //删除uid
108         jedis.del(UID_PREFIX+uid);
109         
110         RedisPoolUtils.release(jedis);
111     }
112     
113     public static String getClientUserBySessionId(String sid){
114         
115         Jedis jedis = RedisPoolUtils.getJedis();
116         
117         String user = jedis.get(SID_PREFIX+sid);
118         
119         RedisPoolUtils.release(jedis);
120         
121         return user;
122     }
123     
124     public static String getClientUserByUid(String uid){
125         Jedis jedis = RedisPoolUtils.getJedis();
126         
127         String user = jedis.get(SID_PREFIX+jedis.get(UID_PREFIX+uid));
128         
129         RedisPoolUtils.release(jedis);
130         
131         return user;
132     }
133     
134     /**
135      * 
136      * @Title: online
137      * @Description: 所有的key
138      * @return List  
139      * @throws
140      */
141     public static List online(){
142 
143         Jedis jedis = RedisPoolUtils.getJedis();
144         
145         Set online = jedis.keys(SID_PREFIX+"*");
146         
147         RedisPoolUtils.release(jedis);
148         return new ArrayList(online);
149     }
150     
151     /**
152      * 
153      * @Title: online
154      * @Description: 分页显示在线列表
155      * @return List  
156      * @throws
157      */
158     public static List onlineByPage(int page,int pageSize) throws Exception{
159         
160         Jedis jedis = RedisPoolUtils.getJedis();
161         
162         Set onlineSet = jedis.keys(SID_PREFIX+"*");
163         
164         
165         List onlines =new ArrayList(onlineSet);
166         
167         if(onlines.size() == 0){
168             return null;
169         }
170         
171         Pipeline pip = jedis.pipelined();
172         for(Object key:onlines){
173             pip.get(getKey(key));
174         }
175         List result = pip.syncAndReturnAll();
176         RedisPoolUtils.release(jedis);
177         
178         List<ClientUser> listUser=new ArrayList<ClientUser>();
179         for(int i=0;i<result.size();i++){
180             listUser.add(Constants.json2ClientUser((String)result.get(i)));
181         }
182         Collections.sort(listUser,new Comparator<ClientUser>(){
183             public int compare(ClientUser o1, ClientUser o2) {
184                 return o2.getLastLoginTime().compareTo(o1.getLastLoginTime());
185             }
186         });
187         onlines=listUser;
188         int start = (page - 1) * pageSize;
189         int toIndex=(start+pageSize)>onlines.size()?onlines.size():start+pageSize;
190         List list = onlines.subList(start, toIndex);
191     
192         return list;
193     }
194     
195     private static String getKey(Object obj){
196         
197         String temp = String.valueOf(obj);
198         String key[] = temp.split(":");
199 
200         return SID_PREFIX+key[key.length-1];
201     }
202     
203     /**
204      * 
205      * @Title: onlineCount
206      * @Description: 总在线人数
207      * @param @return
208      * @return int
209      * @throws
210      */
211     public static int onlineCount(){
212         
213         Jedis jedis = RedisPoolUtils.getJedis();
214         
215         Set online = jedis.keys(SID_PREFIX+"*");
216         
217         RedisPoolUtils.release(jedis);
218         
219         return online.size();
220         
221     }
222     
223     /**
224      * 获取指定页面在线人数总数
225      */
226     public static int broadcastCount(String identify) {
227         Jedis jedis = RedisPoolUtils.getJedis();
228         
229         Set online = jedis.keys(VID_PREFIX+identify+":*");
230         
231         
232 
233         RedisPoolUtils.release(jedis);
234         
235         return online.size();
236     }
237     
238     /**
239      * 自己是否在线
240      */
241     public static boolean broadcastIsOnline(String identify,String uid) {
242         
243         Jedis jedis = RedisPoolUtils.getJedis();
244         
245         String online = jedis.get(VID_PREFIX+identify+":"+uid);
246         
247         RedisPoolUtils.release(jedis);
248         
249         return !StringUtil.isBlank(online);//不为空就代表已经找到数据了，也就是上线了
250     }
251     
252     /**
253      * 获取指定页面在线人数总数
254      */
255     public static int broadcastCount() {
256         Jedis jedis = RedisPoolUtils.getJedis();
257         
258         Set online = jedis.keys(VID_PREFIX+"*");
259         
260         RedisPoolUtils.release(jedis);
261         
262         return online.size();
263     }
264     
265     
266     /**
267      * 
268      * @Title: isOnline
269      * @Description: 指定账号是否登陆
270      * @param @param sessionId
271      * @param @return
272      * @return boolean 
273      * @throws
274      */
275     public static boolean isOnline(String uid){
276         
277         Jedis jedis = RedisPoolUtils.getJedis();
278         
279         boolean isLogin = jedis.exists(UID_PREFIX+uid);
280         
281         RedisPoolUtils.release(jedis);
282         
283         return isLogin;
284     }
285     
286     public static boolean isOnline(String uid,String sid){
287         
288         Jedis jedis = RedisPoolUtils.getJedis();
289         
290         String loginSid = jedis.get(UID_PREFIX+uid);
291         
292         RedisPoolUtils.release(jedis);
293         
294         return sid.equals(loginSid);
295     }
296 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 　  由于在线状态是记录在Redis中的，并不单纯依靠Session的过期机制来实现，所以需要通过拦截器在每次发送请求的时候去更新Redis中相应的缓存过期时间来更新用户的在线状态。
　　登陆、退出操作与单机版相似，强制下线需要配合拦截器实现，当用户下次访问的时候，自己来校验自己的状态是否为已经下线，不再由服务器控制。
　　配合拦截器实现在线状态维持与强制登陆(使其他地方登陆了该账户的用户下线)功能：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 ...
 2 if(uid != null){//已登录
 3     if(!OnlineUtils.isOnline(uid, session.getId())){
 4         session.invalidate();
 5 
 6         return ai.invoke();
 7     }else{
 8         OnlineUtils.login(session.getId(), (ClientUser)session.getAttribute("clientUser"));
 9         //刷新缓存
10     }
11 }
12 ...
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
注：Redis在线列表工具类中的部分代码是后来需要实现限制同时访问指定页面浏览人数功能而添加的，同样基于Redis实现，前端由Ajax轮询来更新用户停留页面的状态。
## 附录：
　　Redis连接池配置文件：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 ###redis##config########
 2 #redis服务器ip # 
 3 #redis.ip=localhost
 4 #redis服务器端口号#
 5 redis.port=6379
 6 
 7 ###jedis##pool##config###
 8 #jedis的最大分配对象#
 9 jedis.pool.maxActive=1024
10 #jedis最大保存idel状态对象数 #
11 jedis.pool.maxIdle=200
12 #jedis池没有对象返回时，最大等待时间 #
13 jedis.pool.maxWait=1000
14 #jedis调用borrowObject方法时，是否进行有效检查#
15 jedis.pool.testOnBorrow=true
16 #jedis调用returnObject方法时，是否进行有效检查 #
17 jedis.pool.testOnReturn=true
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
