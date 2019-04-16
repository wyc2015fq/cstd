# JavaWeb学习总结(四十九)——简单模拟Sping MVC - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaWeb学习总结(四十九)——简单模拟Sping MVC](https://www.cnblogs.com/xdp-gacl/p/4101727.html)



　　在Spring MVC中，将一个普通的java类标注上Controller注解之后，再将类中的方法使用RequestMapping注解标注，那么这个普通的java类就够处理Web请求，示例代码如下：

```
1 /**
 2  * 使用Controller注解标注LoginUI类
 3  */
 4 @Controller
 5 public class LoginUI {
 6     
 7     //使用RequestMapping注解指明forward1方法的访问路径  
 8     @RequestMapping("LoginUI/Login2")
 9     public View forward1(){
10         //执行完forward1方法之后返回的视图
11         return new View("/login2.jsp");  
12     }
13     
14     //使用RequestMapping注解指明forward2方法的访问路径  
15     @RequestMapping("LoginUI/Login3")
16     public View forward2(){
17         //执行完forward2方法之后返回的视图
18         return new View("/login3.jsp");  
19     } 
20 }
```

　　spring通过java annotation就可以注释一个类为action ，在方法上添加上一个java annotation 就可以配置请求的路径了，那么这种机制是如何实现的呢，今天我们使用"自定义注解+Servlet"来简单模拟一下Spring MVC中的这种注解配置方式。

## 一、编写注解

### 1.1、Controller注解

　　开发Controller注解，这个注解只有一个value属性，默认值为空字符串，代码如下：

```
1 package me.gacl.annotation;
 2 
 3 import java.lang.annotation.ElementType;
 4 import java.lang.annotation.Retention;
 5 import java.lang.annotation.RetentionPolicy;
 6 import java.lang.annotation.Target;
 7 
 8 /**
 9 * @ClassName: Controller
10 * @Description: 自定义Controller注解
11 * @author: 孤傲苍狼
12 * @date: 2014-11-16 下午6:16:40
13 *
14 */ 
15 @Retention(RetentionPolicy.RUNTIME)
16 @Target(ElementType.TYPE)
17 public @interface Controller {
18 
19     public String value() default "";
20 }
```

### 1.2、RequestMapping注解

　　开发RequestMapping注解，用于定义请求路径，这个注解只有一个value属性，默认值为空字符串，代码如下：

```
1 package me.gacl.annotation;
 2 
 3 import java.lang.annotation.ElementType;
 4 import java.lang.annotation.Retention;
 5 import java.lang.annotation.RetentionPolicy;
 6 import java.lang.annotation.Target;
 7 
 8 /**
 9  * 定义请求路径的java annotation
10  */
11 @Target(ElementType.METHOD) 
12 @Retention(RetentionPolicy.RUNTIME)  
13 public @interface RequestMapping {
14 
15     public String value() default "";
16 }
```

　　以上就是我们自定义的两个注解，注解的开发工作就算是完成了，有了注解之后，那么就必须针对注解来编写处理器，否则我们开发的注解配置到类或者方法上面是不起作用的，这里我们使用Servlet来作为注解的处理器。

## 二、编写核心的注解处理器

### 2.1、开发AnnotationHandleServlet

　　这里使用一个Servlet来作为注解处理器，编写一个AnnotationHandleServlet，代码如下：

```
1 package me.gacl.web.controller;
  2 
  3 import java.io.IOException;
  4 import java.lang.reflect.InvocationTargetException;
  5 import java.lang.reflect.Method;
  6 import java.util.Set;
  7 import javax.servlet.ServletConfig;
  8 import javax.servlet.ServletException;
  9 import javax.servlet.http.HttpServlet;
 10 import javax.servlet.http.HttpServletRequest;
 11 import javax.servlet.http.HttpServletResponse;
 12 import me.gacl.annotation.Controller;
 13 import me.gacl.annotation.RequestMapping;
 14 import me.gacl.util.BeanUtils;
 15 import me.gacl.util.RequestMapingMap;
 16 import me.gacl.util.ScanClassUtil;
 17 import me.gacl.web.context.WebContext;
 18 import me.gacl.web.view.DispatchActionConstant;
 19 import me.gacl.web.view.View;
 20 
 21 /**
 22  * <p>ClassName: AnnotationHandleServlet<p>
 23  * <p>Description: AnnotationHandleServlet作为自定义注解的核心处理器以及负责调用目标业务方法处理用户请求<p>
 24  * @author xudp
 25  * @version 1.0 V
 26  */
 27 public class AnnotationHandleServlet extends HttpServlet {
 28     
 29     private String pareRequestURI(HttpServletRequest request){
 30         String path = request.getContextPath()+"/";
 31         String requestUri = request.getRequestURI();
 32         String midUrl = requestUri.replaceFirst(path, "");
 33         String lasturl = midUrl.substring(0, midUrl.lastIndexOf("."));
 34         return lasturl;
 35     }
 36     
 37     public void doGet(HttpServletRequest request, HttpServletResponse response)
 38             throws ServletException, IOException {
 39         this.excute(request, response);
 40     }
 41 
 42     public void doPost(HttpServletRequest request, HttpServletResponse response)
 43             throws ServletException, IOException {
 44         this.excute(request, response);
 45     }
 46     
 47     private void excute(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException{
 48         //将当前线程中HttpServletRequest对象存储到ThreadLocal中，以便在Controller类中使用
 49         WebContext.requestHodler.set(request);
 50         //将当前线程中HttpServletResponse对象存储到ThreadLocal中，以便在Controller类中使用
 51         WebContext.responseHodler.set(response);
 52         //解析url
 53         String lasturl = pareRequestURI(request);
 54         //获取要使用的类
 55         Class<?> clazz = RequestMapingMap.getRequesetMap().get(lasturl);
 56         //创建类的实例
 57         Object classInstance = BeanUtils.instanceClass(clazz);
 58         //获取类中定义的方法
 59         Method [] methods = BeanUtils.findDeclaredMethods(clazz);
 60         Method method = null;
 61         for(Method m:methods){//循环方法，找匹配的方法进行执行
 62             if(m.isAnnotationPresent(RequestMapping.class)){
 63                 String anoPath = m.getAnnotation(RequestMapping.class).value();
 64                 if(anoPath!=null && !"".equals(anoPath.trim()) && lasturl.equals(anoPath.trim())){
 65                     //找到要执行的目标方法
 66                     method = m;
 67                     break;
 68                 }
 69             }
 70         }
 71         try {
 72             if(method!=null){
 73                 //执行目标方法处理用户请求
 74                 Object retObject = method.invoke(classInstance);
 75                 //如果方法有返回值，那么就表示用户需要返回视图
 76                 if (retObject!=null) {
 77                     View view = (View)retObject;
 78                     //判断要使用的跳转方式
 79                     if(view.getDispathAction().equals(DispatchActionConstant.FORWARD)){
 80                         //使用服务器端跳转方式
 81                         request.getRequestDispatcher(view.getUrl()).forward(request, response);
 82                     }else if(view.getDispathAction().equals(DispatchActionConstant.REDIRECT)){
 83                         //使用客户端跳转方式
 84                         response.sendRedirect(request.getContextPath()+view.getUrl());
 85                     }else{
 86                         request.getRequestDispatcher(view.getUrl()).forward(request, response);
 87                     }
 88                 }
 89             }
 90         } catch (IllegalArgumentException e) {
 91             e.printStackTrace();
 92         } catch (IllegalAccessException e) {
 93             e.printStackTrace();
 94         } catch (InvocationTargetException e) {
 95             e.printStackTrace();
 96         }
 97     }
 98 
 99     @Override
100     public void init(ServletConfig config) throws ServletException {
101         /**
102          * 重写了Servlet的init方法后一定要记得调用父类的init方法，
103          * 否则在service/doGet/doPost方法中使用getServletContext()方法获取ServletContext对象时
104          * 就会出现java.lang.NullPointerException异常
105          */
106         super.init(config); 
107         System.out.println("---初始化开始---");
108         //获取web.xml中配置的要扫描的包
109         String basePackage = config.getInitParameter("basePackage");
110         //如果配置了多个包，例如：<param-value>me.gacl.web.controller,me.gacl.web.UI</param-value>
111         if (basePackage.indexOf(",")>0) {
112             //按逗号进行分隔
113             String[] packageNameArr = basePackage.split(",");
114             for (String packageName : packageNameArr) {
115                 initRequestMapingMap(packageName);
116             }
117         }else {
118             initRequestMapingMap(basePackage);
119         }
120         System.out.println("----初始化结束---");
121     }
122     
123     /**
124     * @Method: initRequestMapingMap
125     * @Description:添加使用了Controller注解的Class到RequestMapingMap中
126     * @Anthor:孤傲苍狼
127     * @param packageName
128     */ 
129     private void initRequestMapingMap(String packageName){
130         Set<Class<?>> setClasses =  ScanClassUtil.getClasses(packageName);
131         for (Class<?> clazz :setClasses) {
132             if (clazz.isAnnotationPresent(Controller.class)) {
133                 Method [] methods = BeanUtils.findDeclaredMethods(clazz);
134                 for(Method m:methods){//循环方法，找匹配的方法进行执行
135                     if(m.isAnnotationPresent(RequestMapping.class)){
136                         String anoPath = m.getAnnotation(RequestMapping.class).value();
137                         if(anoPath!=null && !"".equals(anoPath.trim())){
138                             if (RequestMapingMap.getRequesetMap().containsKey(anoPath)) {
139                                 throw new RuntimeException("RequestMapping映射的地址不允许重复！");
140                             }
141                             RequestMapingMap.put(anoPath, clazz);
142                         }
143                     }
144                 }
145             }
146         }
147     }
148 }
```

　　这里说一下AnnotationHandleServlet的实现思路

　　1、AnnotationHandleServlet初始化(init)时扫描指定的包下面使用了Controller注解的类，如下图所示：

![](data:image/png;base64,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)

　　2、遍历类中的方法，找到类中使用了RequestMapping注解标注的那些方法，获取RequestMapping注解的value属性值，value属性值指明了该方法的访问路径，以RequestMapping注解的value属性值作为key，Class类作为value将存储到一个静态Map集合中。如下图所示：

![](data:image/png;base64,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)

　　当用户请求时(无论是get还是post请求)，会调用封装好的execute方法 ，execute会先获取请求的url，然后解析该URL，根据解析好的URL从Map集合中取出要调用的目标类 ，再遍历目标类中定义的所有方法，找到类中使用了RequestMapping注解的那些方法，判断方法上面的RequestMapping注解的value属性值是否和解析出来的URL路径一致,如果一致，说明了这个就是要调用的目标方法，此时就可以利用java反射机制先实例化目标类对象，然后再通过实例化对象调用要执行的方法处理用户请求。服务器将以下图的方式与客户端进行交互  

![](data:image/png;base64,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)

　　另外，方法处理完成之后需要给客户端发送响应信息，比如告诉客户端要跳转到哪一个页面，采用的是服务器端跳转还是客户端方式跳转，或者发送一些数据到客户端显示，那么该如何发送响应信息给客户端呢，在此，我们可以设计一个View(视图)类，对这些操作属性进行封装，其中包括跳转的路径 、展现到页面的数据、跳转方式。这就是AnnotationHandleServlet的实现思路。

### 2.2、在Web.xml文件中注册AnnotationHandleServlet

　　在web.xml文件中配置AnnotationHandleServlet和需要扫描的包

```
1 <servlet>
 2     <servlet-name>AnnotationHandleServlet</servlet-name>
 3     <servlet-class>me.gacl.web.controller.AnnotationHandleServlet</servlet-class>
 4     <init-param>
 5          <description>配置要扫描包及其子包, 如果有多个包,以逗号分隔</description>
 6         <param-name>basePackage</param-name>
 7         <param-value>me.gacl.web.controller,me.gacl.web.UI</param-value>
 8         <!-- <param-value>me.gacl.web.controller</param-value> -->
 9     </init-param>
10     <load-on-startup>1</load-on-startup>
11   </servlet>
12 
13   <servlet-mapping>
14     <servlet-name>AnnotationHandleServlet</servlet-name>
15     <!-- 拦截所有以.do后缀结尾的请求 -->
16     <url-pattern>*.do</url-pattern>
17   </servlet-mapping>
```

## 三、相关代码讲解

### 3.1、BeanUtils

　　BeanUtils工具类主要是用来处理一些反射的操作

```
1 package me.gacl.util;
 2 
 3 import java.lang.reflect.Constructor;
 4 import java.lang.reflect.Field;
 5 import java.lang.reflect.InvocationTargetException;
 6 import java.lang.reflect.Method;
 7 import java.lang.reflect.Modifier;
 8 
 9 /**
10  * 对java反射中操作的一些封装
11  */
12 public class BeanUtils {
13     
14     /**
15      * 实例化一个class 
16      * @param <T>
17      * @param clazz Person.class
18      * @return
19      */
20     public static <T> T instanceClass(Class<T> clazz){
21         if(!clazz.isInterface()){
22             try {
23                 return clazz.newInstance();
24             } catch (InstantiationException e) {
25                 e.printStackTrace();
26             } catch (IllegalAccessException e) {
27                 e.printStackTrace();
28             }
29         }
30         return null;
31     }
32     
33     /**
34      * 通过构造函数实例化
35      * @param <T>
36      * @param ctor
37      * @param args
38      * @return
39      * @throws IllegalArgumentException
40      * @throws InstantiationException
41      * @throws IllegalAccessException
42      * @throws InvocationTargetException
43      */
44     public static <T> T instanceClass(Constructor<T> ctor, Object... args)
45             throws IllegalArgumentException, InstantiationException, 
46             IllegalAccessException, InvocationTargetException{
47         makeAccessible(ctor);
48         return ctor.newInstance(args);//调用构造方法实例化
49     }
50     
51     /**
52      * 查找某个class的方法
53      * @param clazz
54      * @param methodName
55      * @param paramTypes
56      * @return
57      * @throws SecurityException
58      * @throws NoSuchMethodException
59      */
60     public static  Method findMethod(Class<?> clazz, String methodName, Class<?>... paramTypes){
61         try {
62             return clazz.getMethod(methodName, paramTypes);
63         } catch (NoSuchMethodException e) {
64             return findDeclaredMethod(clazz, methodName, paramTypes);//返回共有的方法
65         }
66     }
67     
68     public static Method findDeclaredMethod(Class<?> clazz, String methodName, Class<?>[] paramTypes){
69         try {
70             return clazz.getDeclaredMethod(methodName, paramTypes);
71         }
72         catch (NoSuchMethodException ex) {
73             if (clazz.getSuperclass() != null) {
74                 return findDeclaredMethod(clazz.getSuperclass(), methodName, paramTypes);
75             }
76             return null;
77         }
78     }
79     
80     public static Method [] findDeclaredMethods(Class<?> clazz){
81             return clazz.getDeclaredMethods();
82     }
83     
84     public static void makeAccessible(Constructor<?> ctor) {
85         if ((!Modifier.isPublic(ctor.getModifiers()) 
86                 || !Modifier.isPublic(ctor.getDeclaringClass().getModifiers()))
87                 && !ctor.isAccessible()) {
88             ctor.setAccessible(true);//如果是私有的 设置为true 使其可以访问
89         }
90     }
91     
92     public static Field[] findDeclaredFields(Class<?> clazz){
93         return clazz.getDeclaredFields();
94     }
95 }
```

### 3.2、RequestMapingMap

　　该类是用于存储方法的访问路径，AnnotationHandleServlet初始化时会将类(使用Controller注解标注的那些类)中使用了RequestMapping注解标注的那些方法的访问路径存储到RequestMapingMap中。

```
1 package me.gacl.util;
 2 
 3 import java.util.HashMap;
 4 import java.util.Map;
 5 
 6 /**
 7 * @ClassName: RequestMapingMap
 8 * @Description: 存储方法的访问路径
 9 * @author: 孤傲苍狼
10 * @date: 2014-11-16 下午6:31:43
11 *
12 */ 
13 public class RequestMapingMap {
14 
15     /**
16     * @Field: requesetMap
17     *          用于存储方法的访问路径
18     */ 
19     private static Map<String, Class<?>> requesetMap = new HashMap<String, Class<?>>();
20     
21     public static Class<?> getClassName(String path) {
22         return requesetMap.get(path);
23     }
24 
25     public static void put(String path, Class<?> className) {
26         requesetMap.put(path, className);
27     }
28 
29     public static Map<String, Class<?>> getRequesetMap() {
30         return requesetMap;
31     }
32 }
```

### 3.3、ScanClassUtil

　　扫描某个包下面的类的工具类

```
1 package me.gacl.util;
  2 
  3 import java.io.File;
  4 import java.io.FileFilter;
  5 import java.io.IOException;
  6 import java.net.JarURLConnection;
  7 import java.net.URL;
  8 import java.net.URLDecoder;
  9 import java.util.Enumeration;
 10 import java.util.LinkedHashSet;
 11 import java.util.Set;
 12 import java.util.jar.JarEntry;
 13 import java.util.jar.JarFile;
 14 
 15 /**
 16 * @ClassName: ScanClassUtil
 17 * @Description: 扫描指定包或者jar包下面的class
 18 * @author: 孤傲苍狼
 19 * @date: 2014-11-16 下午6:34:10
 20 *
 21 */ 
 22 public class ScanClassUtil {
 23 
 24     /**
 25      * 从包package中获取所有的Class
 26      * 
 27      * @param pack
 28      * @return
 29      */
 30     public static Set<Class<?>> getClasses(String pack) {
 31 
 32         // 第一个class类的集合
 33         Set<Class<?>> classes = new LinkedHashSet<Class<?>>();
 34         // 是否循环迭代
 35         boolean recursive = true;
 36         // 获取包的名字 并进行替换
 37         String packageName = pack;
 38         String packageDirName = packageName.replace('.', '/');
 39         // 定义一个枚举的集合 并进行循环来处理这个目录下的things
 40         Enumeration<URL> dirs;
 41         try {
 42             dirs = Thread.currentThread().getContextClassLoader().getResources(
 43                     packageDirName);
 44             // 循环迭代下去
 45             while (dirs.hasMoreElements()) {
 46                 // 获取下一个元素
 47                 URL url = dirs.nextElement();
 48                 // 得到协议的名称
 49                 String protocol = url.getProtocol();
 50                 // 如果是以文件的形式保存在服务器上
 51                 if ("file".equals(protocol)) {
 52                     System.err.println("file类型的扫描");
 53                     // 获取包的物理路径
 54                     String filePath = URLDecoder.decode(url.getFile(), "UTF-8");
 55                     // 以文件的方式扫描整个包下的文件 并添加到集合中
 56                     findAndAddClassesInPackageByFile(packageName, filePath,
 57                             recursive, classes);
 58                 } else if ("jar".equals(protocol)) {
 59                     // 如果是jar包文件
 60                     // 定义一个JarFile
 61                     System.err.println("jar类型的扫描");
 62                     JarFile jar;
 63                     try {
 64                         // 获取jar
 65                         jar = ((JarURLConnection) url.openConnection())
 66                                 .getJarFile();
 67                         // 从此jar包 得到一个枚举类
 68                         Enumeration<JarEntry> entries = jar.entries();
 69                         // 同样的进行循环迭代
 70                         while (entries.hasMoreElements()) {
 71                             // 获取jar里的一个实体 可以是目录 和一些jar包里的其他文件 如META-INF等文件
 72                             JarEntry entry = entries.nextElement();
 73                             String name = entry.getName();
 74                             // 如果是以/开头的
 75                             if (name.charAt(0) == '/') {
 76                                 // 获取后面的字符串
 77                                 name = name.substring(1);
 78                             }
 79                             // 如果前半部分和定义的包名相同
 80                             if (name.startsWith(packageDirName)) {
 81                                 int idx = name.lastIndexOf('/');
 82                                 // 如果以"/"结尾 是一个包
 83                                 if (idx != -1) {
 84                                     // 获取包名 把"/"替换成"."
 85                                     packageName = name.substring(0, idx)
 86                                             .replace('/', '.');
 87                                 }
 88                                 // 如果可以迭代下去 并且是一个包
 89                                 if ((idx != -1) || recursive) {
 90                                     // 如果是一个.class文件 而且不是目录
 91                                     if (name.endsWith(".class")
 92                                             && !entry.isDirectory()) {
 93                                         // 去掉后面的".class" 获取真正的类名
 94                                         String className = name.substring(
 95                                                 packageName.length() + 1, name
 96                                                         .length() - 6);
 97                                         try {
 98                                             // 添加到classes
 99                                             classes.add(Class
100                                                     .forName(packageName + '.'
101                                                             + className));
102                                         } catch (ClassNotFoundException e) {
103                                             // log
104                                             // .error("添加用户自定义视图类错误 找不到此类的.class文件");
105                                             e.printStackTrace();
106                                         }
107                                     }
108                                 }
109                             }
110                         }
111                     } catch (IOException e) {
112                         // log.error("在扫描用户定义视图时从jar包获取文件出错");
113                         e.printStackTrace();
114                     }
115                 }
116             }
117         } catch (IOException e) {
118             e.printStackTrace();
119         }
120 
121         return classes;
122     }
123     
124     /**
125      * 以文件的形式来获取包下的所有Class
126      * 
127      * @param packageName
128      * @param packagePath
129      * @param recursive
130      * @param classes
131      */
132     public static void findAndAddClassesInPackageByFile(String packageName,
133             String packagePath, final boolean recursive, Set<Class<?>> classes) {
134         // 获取此包的目录 建立一个File
135         File dir = new File(packagePath);
136         // 如果不存在或者 也不是目录就直接返回
137         if (!dir.exists() || !dir.isDirectory()) {
138             // log.warn("用户定义包名 " + packageName + " 下没有任何文件");
139             return;
140         }
141         // 如果存在 就获取包下的所有文件 包括目录
142         File[] dirfiles = dir.listFiles(new FileFilter() {
143             // 自定义过滤规则 如果可以循环(包含子目录) 或则是以.class结尾的文件(编译好的java类文件)
144             public boolean accept(File file) {
145                 return (recursive && file.isDirectory())
146                         || (file.getName().endsWith(".class"));
147             }
148         });
149         // 循环所有文件
150         for (File file : dirfiles) {
151             // 如果是目录 则继续扫描
152             if (file.isDirectory()) {
153                 findAndAddClassesInPackageByFile(packageName + "."
154                         + file.getName(), file.getAbsolutePath(), recursive,
155                         classes);
156             } else {
157                 // 如果是java类文件 去掉后面的.class 只留下类名
158                 String className = file.getName().substring(0,
159                         file.getName().length() - 6);
160                 try {
161                     // 添加到集合中去
162                     //classes.add(Class.forName(packageName + '.' + className));
163                      //经过回复同学的提醒，这里用forName有一些不好，会触发static方法，没有使用classLoader的load干净
164                     classes.add(Thread.currentThread().getContextClassLoader().loadClass(packageName + '.' + className));  
165                     } catch (ClassNotFoundException e) {
166                     // log.error("添加用户自定义视图类错误 找不到此类的.class文件");
167                     e.printStackTrace();
168                 }
169             }
170         }
171     }
172 }
```

### 3.4、WebContext

　　WebContext主要是用来存储当前线程中的HttpServletRequest和HttpServletResponse，当别的地方需要使用HttpServletRequest和HttpServletResponse，就可以通过requestHodler和responseHodler获取，通过WebContext.java这个类 ，我们可以在作为Controller的普通java类中获取当前请求的request、response或者session相关请求类的实例变量，并且线程间互不干扰的，因为用到了ThreadLocal这个类。

```
1 package me.gacl.web.context;
 2 
 3 import javax.servlet.ServletContext;
 4 import javax.servlet.http.HttpServletRequest;
 5 import javax.servlet.http.HttpServletResponse;
 6 import javax.servlet.http.HttpSession;
 7 
 8 /**
 9  * WebContext主要是用来存储当前线程中的HttpServletRequest和HttpServletResponse
10  * 当别的地方需要使用HttpServletRequest和HttpServletResponse，就可以通过requestHodler和responseHodler获取
11  **/
12 public class WebContext {
13 
14     public static ThreadLocal<HttpServletRequest> requestHodler = new ThreadLocal<HttpServletRequest>();
15     public static ThreadLocal<HttpServletResponse> responseHodler = new ThreadLocal<HttpServletResponse>();
16    
17     public HttpServletRequest getRequest(){
18            return requestHodler.get();
19     }
20    
21     public HttpSession getSession(){
22            return requestHodler.get().getSession();
23     }
24    
25     public ServletContext getServletContext(){
26            return requestHodler.get().getSession().getServletContext();
27     }
28     
29     public HttpServletResponse getResponse(){
30         return responseHodler.get();
31     }
32 }
```

### 3.5、View

　　一个视图类，对一些客户端响应操作进行封装，其中包括跳转的路径 、展现到页面的数据、跳转方式

```
1 package me.gacl.web.view;
 2 
 3 /**
 4  * 视图模型
 5  **/
 6 public class View {
 7 
 8     private String url;//跳转路径
 9     
10     private String dispathAction = DispatchActionConstant.FORWARD;//跳转方式
11 
12     public View(String url) {
13         this.url = url;
14     }
15     
16     public View(String url,String name,Object value) {
17         this.url = url;
18         ViewData view = new ViewData();
19         view.put(name, value);
20     }
21     
22     
23     public View(String url,String name,String dispathAction ,Object value) {
24         this.dispathAction = dispathAction;
25         this.url = url;
26         ViewData view = new ViewData();//请看后面的代码
27         view.put(name, value);
28     }
29     
30     
31     public String getUrl() {
32         return url;
33     }
34     
35     
36     public void setUrl(String url) {
37         this.url = url;
38     }
39 
40     public String getDispathAction() {
41         return dispathAction;
42     }
43 
44     public void setDispathAction(String dispathAction) {
45         this.dispathAction = dispathAction;
46     }
47 }
```

### 3.6、ViewData

　　request范围的数据存储类，当需要发送数据到客户端显示时，就可以将要显示的数据存储到ViewData类中。使用ViewData.put(String name,Object value)方法往request对象中存数据。

```
1 package me.gacl.web.view;
 2 
 3 import javax.servlet.http.HttpServletRequest;
 4 
 5 import me.gacl.web.context.WebContext;
 6 
 7 /**
 8  * 需要发送到客户端显示的数据模型
 9  */
10 public class ViewData {
11     
12     private HttpServletRequest request;
13     
14     public ViewData() {
15         initRequest();
16     }
17 
18     private void initRequest(){
19         //从requestHodler中获取request对象
20         this.request = WebContext.requestHodler.get();
21     }
22     
23     public void put(String name,Object value){
24         this.request.setAttribute(name, value);
25     }
26 }
```

3.7、DispatchActionConstant

　　一个跳转方式的常量类

```
1 package me.gacl.web.view;
 2 
 3 /**
 4  * 跳转常量
 5  */
 6 public class DispatchActionConstant {
 7 
 8     public static String FORWARD = "forward";//服务器跳转
 9     
10     public static String REDIRECT = "redirect";//客户端跳转
11 }
```

## 四、Controller注解和RequestMapping注解测试

### 4.1、简单测试

　　编写一个LoginUI类，用于跳转到具体的jsp页面，代码如下：

```
1 package me.gacl.web.UI;
 2 
 3 import me.gacl.annotation.Controller;
 4 import me.gacl.annotation.RequestMapping;
 5 import me.gacl.web.view.View;
 6 /**
 7  * 使用Controller注解标注LoginUI类
 8  */
 9 @Controller
10 public class LoginUI {
11     
12     //使用RequestMapping注解指明forward1方法的访问路径  
13     @RequestMapping("LoginUI/Login2")
14     public View forward1(){
15         //执行完forward1方法之后返回的视图
16         return new View("/login2.jsp");  
17     }
18     
19     //使用RequestMapping注解指明forward2方法的访问路径  
20     @RequestMapping("LoginUI/Login3")
21     public View forward2(){
22         //执行完forward2方法之后返回的视图
23         return new View("/login3.jsp");  
24     } 
25 }
```

　　运行结果如下所示：

![](https://images0.cnblogs.com/blog/289233/201411/221203538127091.gif)

### 4.2、复杂测试

　　编写用于处理用户登录请求的Controller，代码如下：

```
1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 import javax.servlet.http.HttpServletRequest;
 5 import javax.servlet.http.HttpServletResponse;
 6 import me.gacl.annotation.Controller;
 7 import me.gacl.annotation.RequestMapping;
 8 import me.gacl.web.context.WebContext;
 9 import me.gacl.web.view.View;
10 import me.gacl.web.view.ViewData;
11 
12 /**
13 * 
14 * @ClassName: LoginServlet2
15 * @Description:处理用户登录的Servlet，
16 * LoginServlet现在就是一个普通的java类，不是一个真正的Servlet
17 * @author: 孤傲苍狼
18 * @date: 2014-10-8 上午12:07:58
19 *
20 */
21 @Controller //使用Controller注解标注LoginServlet2
22 public class LoginServlet2 {
23 
24     /**
25     * @Method: loginHandle
26     * @Description:处理以普通方式提交的请求
27     * @Anthor:孤傲苍狼
28     *
29     * @return View
30     */
31     //使用RequestMapping注解标注loginHandle方法，指明loginHandle方法的访问路径是login/handle
32     @RequestMapping("login/handle")
33     public View loginHandle(){
34         //创建一个ViewData对象，用于存储需要发送到客户端的响应数据
35         ViewData viewData = new ViewData();
36         //通过WebContext类获取当前线程中的HttpServletRequest对象
37         HttpServletRequest request = WebContext.requestHodler.get();
38         //接收提交上来的参数
39         String username =request.getParameter("usename");
40         String pwd = request.getParameter("pwd");
41         if (username.equals("gacl") && pwd.equals("xdp")) {
42             request.getSession().setAttribute("usename", username);
43             //将响应数据存储到ViewData对象中
44             viewData.put("msg", "欢迎您！"+username);
45             //返回一个View对象，指明要跳转的视图的路径
46             return new View("/index.jsp");
47         }else {
48             //将响应数据存储到ViewData对象中
49             viewData.put("msg", "登录失败，请检查用户名和密码是否正确！");
50             //返回一个View对象，指明要跳转的视图的路径
51             return new View("/login2.jsp");
52         }
53     }
54     
55     /**
56     * @Method: ajaxLoginHandle
57     * @Description: 处理以AJAX方式提交的请求
58     * @Anthor:孤傲苍狼
59     *
60     * @throws IOException
61     */ 
62     //使用RequestMapping注解标注ajaxLoginHandle方法，指明ajaxLoginHandle方法的访问路径是ajaxLogin/handle
63     @RequestMapping("ajaxLogin/handle")
64     public void ajaxLoginHandle() throws IOException{
65         //通过WebContext类获取当前线程中的HttpServletRequest对象
66         HttpServletRequest request = WebContext.requestHodler.get();
67         //接收提交上来的参数
68         String username =request.getParameter("usename");
69         String pwd = request.getParameter("pwd");
70         //通过WebContext类获取当前线程中的HttpServletResponse对象
71         HttpServletResponse response = WebContext.responseHodler.get();
72         if (username.equals("gacl") && pwd.equals("xdp")) {
73             request.getSession().setAttribute("usename", username);
74             response.getWriter().write("success");
75         }else {
76             response.getWriter().write("fail");
77         }
78     }
79 }
```

　　编写用于测试的jsp页面，代码如下所示：

　　Login2.jsp登录页面

```
1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>login2.jsp登录页面</title>
 6   </head>
 7   
 8   <body>
 9     <fieldset>
10         <legend>用户登录</legend>
11         <form action="${pageContext.request.contextPath}/**login/handle**.do" method="post">
12             用户名：<input type="text" value="${param.usename}" name="usename">
13             <br/>
14             密码：<input type="text" value="${param.pwd}" name="pwd">
15             <br/>
16             <input type="submit" value="登录"/>
17         </form>
18     </fieldset>
19     <hr/>
20     <label style="color: red;">${msg}</label>
21   </body>
22 </html>
```

　　login3.jsp登录页面

```
1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>login3登录页面</title>
 6     <script type="text/javascript" src="${pageContext.request.contextPath}/ajaxUtil.js"></script>
 7     <script type="text/javascript" src="${pageContext.request.contextPath}/js/Utils.js"></script>
 8     <script type="text/javascript">
 9     
10         function login(){
11              Ajax.request({
12                  url : "${pageContext.request.contextPath}/ajaxLogin/handle.do",
13                  data : {
14                      "usename" : document.getElementById("usename").value,
15                      "pwd" : document.getElementById("pwd").value
16                  },
17                  success : function(xhr) {
18                      onData(xhr.responseText);
19                  },
20                  error : function(xhr) {
21                      
22                  }
23              });
24         }
25         
26         function onData(responseText){
27             if(responseText=="success"){
28                 //window.location.href="index.jsp";//改变url地址
29                 /*
30                 window.location.replace("url")：将地址替换成新url，
31                 该方法通过指定URL替换当前缓存在历史里（客户端）的项目，因此当使用replace方法之后，
32                 你不能通过“前进”和“后 退”来访问已经被替换的URL，这个特点对于做一些过渡页面非常有用！
33                 */
34                 location.replace(g_basePath+"/index.jsp");
35             }else{
36                 alert("用户名和密码错误");
37             }
38         }
39     </script>
40   </head>
41   
42   <body>
43     <fieldset>
44         <legend>用户登录</legend>
45         <form>
46             用户名：<input type="text" name="usename" id="usename">
47             <br/>
48             密码：<input type="text" name="pwd" id="pwd">
49             <br/>
50             <input type="button" value="登录" onclick="login()"/>
51         </form>
52     </fieldset>
53   </body>
54 </html>
```

　　index.jsp页面代码如下：

```
1 <%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
 2 
 3 <!DOCTYPE HTML>
 4 <html>
 5   <head>
 6     <title>首页</title>
 7   </head>
 8   
 9   <body>
10         登录的用户名：${usename}
11     <br/>
12     ${msg}
13   </body>
14 </html>
```

　　jsp页面中使用到的Utils.js代码如下：

```
1 //立即执行的js
 2 (function() {
 3     //获取contextPath
 4     var contextPath = getContextPath();
 5     //获取basePath
 6     var basePath = getBasePath();
 7     //将获取到contextPath和basePath分别赋值给window对象的g_contextPath属性和g_basePath属性
 8     window.g_contextPath = contextPath;
 9     window.g_basePath = basePath;
10 })();
11 
12 /**
13  * @author 孤傲苍狼
14  * 获得项目根路径，等价于jsp页面中
15  *  <%
16         String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
17     %>
18  * 使用方法：getBasePath();
19  * @returns 项目的根路径
20  *  
21  */
22 function getBasePath() {
23     var curWwwPath = window.document.location.href;
24     var pathName = window.document.location.pathname;
25     var pos = curWwwPath.indexOf(pathName);
26     var localhostPath = curWwwPath.substring(0, pos);
27     var projectName = pathName.substring(0, pathName.substr(1).indexOf('/') + 1);
28     return (localhostPath + projectName);
29 }
30 
31 /**
32  * @author 孤傲苍狼
33  * 获取Web应用的contextPath，等价于jsp页面中
34  *  <%
35         String path = request.getContextPath();
36     %>
37  * 使用方法:getContextPath();
38  * @returns /项目名称(/EasyUIStudy_20141104)
39  */
40 function getContextPath() {
41     return window.document.location.pathname.substring(0, window.document.location.pathname.indexOf('\/', 1));
42 };
```

　　测试结果如下：

![](https://images0.cnblogs.com/blog/289233/201411/221226384373891.gif)

![](https://images0.cnblogs.com/blog/289233/201411/221230214069739.gif)

　　以上就是对Spring MVC的简单模拟。









