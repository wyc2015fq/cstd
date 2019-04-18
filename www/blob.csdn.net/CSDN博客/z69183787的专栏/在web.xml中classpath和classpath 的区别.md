# 在web.xml中classpath和classpath*的区别 - z69183787的专栏 - CSDN博客
2015年10月06日 19:30:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6355
写spring的代码到现在，一直都很习惯性的拷贝web.xml中的内容，没怎么在意里面的内容，最近认真研究了下，很多东西都不是很理解，特别是classpath和classpath*的区别，研究了许久才搞明白，记录下备忘。
**classpath 和 classpath* 区别**：
**classpath：只会到你指定的class路径中查找找文件;**
**classpath*：不仅包含class路径，还包括jar文件中(class路径)进行查找.**
举个简单的例子，在我的web.xml中是这么定义的：classpath*:META-INF/spring/application-context.xml
那么在META-INF/spring这个文件夹底下的所有application-context.xml都会被加载到上下文中，这些包括META-INF/spring文件夹底下的 application-context.xml，META-INF/spring的子文件夹的application-context.xml以及jar中的application-context.xml。
如果我在web.xml中定义的是：classpath:META-INF/spring/application-context.xml
那么只有META-INF/spring底下的application-context.xml会被加载到上下文中。
Spring可以通过指定classpath*:与classpath:前缀加路径的方式从classpath加载文件,如bean的定义文件.classpath*:的出现是为了从多个jar文件中加载相同的文件.classpath:只能加载找到的第一个文件.
比如 resource1.jar中的package 'com.test.rs' 有一个 'jarAppcontext.xml' 文件,内容如下:
<bean name="ProcessorImplA" class="com.test.spring.di.ProcessorImplA" />
resource2.jar中的package 'com.test.rs' 也有一个 'jarAppcontext.xml' 文件,内容如下:
<bean id="ProcessorImplB" class="com.test.spring.di.ProcessorImplB" />
通过使用下面的代码则可以将两个jar包中的文件都加载进来
ApplicationContext ctx = new ClassPathXmlApplicationContext( "classpath*:com/test/rs/jarAppcontext.xml");
而如果写成下面的代码,就只能找到其中的一个xml文件(顺序取决于jar包的加载顺序)
ApplicationContext ctx = new ClassPathXmlApplicationContext( "classpath:com/test/rs/jarAppcontext.xml");
classpath*:的使用是为了多个component(最终发布成不同的jar包)并行开发,各自的bean定义文件按照一定的规则:package+filename,而使用这些component的调用者可以把这些文件都加载进来.
classpath*:的加载使用了classloader的 `getResources()` 方法,如果是在不同的J2EE服务器上运行,由于应用服务器提供自己的classloader实现，它们在处理jar文件时的行为也许会有所不同。 要测试`classpath*:` 是否有效，可以用classloader从classpath中的jar文件里加载文件来进行测试：`getClass().getClassLoader().getResources("<someFileInsideTheJar>")`。(上面的例子是在sun的jre中运行的状态)
 从Spring的源码,在PathMatchingResourcePatternResolver类中,我们可以更清楚的了解其对的处理:如果是以classpath*开头,它会遍历classpath.
**[java]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/5507799#)[copy](http://blog.csdn.net/kkdelta/article/details/5507799#)
- protected Resource[] findAllClassPathResources(String location) throws IOException {  
-     String path = location;  
- if (path.startsWith("/")) {  
-         path = path.substring(1);  
-     }  
-     Enumeration resourceUrls = getClassLoader().getResources(path);  
-     Set<Resource> result = new LinkedHashSet<Resource>(16);  
- while (resourceUrls.hasMoreElements()) {  
-         URL url = (URL) resourceUrls.nextElement();  
-         result.add(convertClassLoaderURL(url));  
-     }  
- return result.toArray(new Resource[result.size()]);  
- }  
http://blog.csdn.net/kkdelta/article/details/5560210,简介了在JAVA里遍历classpath中读取找到的所有符合名称的文件.
另外在加载resource的时候,其他前缀的意义如下表所示:注意classpath*只能用与指定配置文件的路径,不能用在用于getResource的参数.如appContext.getResource("classpath*:conf/bfactoryCtx.xml")会异常的.
|前缀|例子|说明|
|----|----|----|
|classpath:|`classpath:com/myapp/config.xml`|从classpath中加载。|
|file:|`file:/data/config.xml`|作为 `URL` 从文件系统中加载。[](http://blog.csdn.net/kkdelta/article/details/5507799#ftn.id460088)|
|http:|`http://myserver/logo.png`|作为 `URL` 加载。|
|(none)|`/data/config.xml`|根据 `ApplicationContext` 进行判断。|
|[](http://blog.csdn.net/kkdelta/article/details/5507799#id460088)| | |
从Spring的源码可以看出原因:如果是classpath:开头,从classpath加载,否则尝试URL,如果失败,调用 getResourceByPath
**[java]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/5507799#)[copy](http://blog.csdn.net/kkdelta/article/details/5507799#)
- public Resource getResource(String location) {  
-         Assert.notNull(location, "Location must not be null");  
- if (location.startsWith(CLASSPATH_URL_PREFIX)) {  
- returnnew ClassPathResource(location.substring(CLASSPATH_URL_PREFIX.length()), getClassLoader());  
-         }  
- else {  
- try {  
- // Try to parse the location as a URL...
-                 URL url = new URL(location);  
- returnnew UrlResource(url);  
-             }  
- catch (MalformedURLException ex) {  
- // No URL -> resolve as resource path.
- return getResourceByPath(location);  
-             }  
-         }  
-     }  
getResourceByPath会被不同`ApplicationContext` 实现覆盖.
如 GenericWebApplicationContext覆盖为如下:
**[java]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/5507799#)[copy](http://blog.csdn.net/kkdelta/article/details/5507799#)
- protected Resource getResourceByPath(String path) {  
- returnnew ServletContextResource(this.servletContext, path);  
-     }  
- 
- 如 FileSystemXmlApplicationContext覆盖为如下:  
- 
- protected Resource getResourceByPath(String path) {  
- if (path != null && path.startsWith("/")) {  
-             path = path.substring(1);  
-         }  
- returnnew FileSystemResource(path);  
-     }  
最终从文件加载的时候仍然是JAVA中常见的读取文件的方法:
如ClassPathResource得到inputstream的方法是利用class loader.
**[java]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/5507799#)[copy](http://blog.csdn.net/kkdelta/article/details/5507799#)
- public InputStream getInputStream() throws IOException {  
-     InputStream is;  
- if (this.clazz != null) {  
-         is = this.clazz.getResourceAsStream(this.path);  
-     }  
如FileSystemResource得到inputstream的方法是利用FileInputStream.
    public InputStream getInputStream() throws IOException {
        return new FileInputStream(this.file);
    }
如ServletContextResource得到inputstream的方法是利用servletContext.getResourceAsStream.
**[java]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/5507799#)[copy](http://blog.csdn.net/kkdelta/article/details/5507799#)
- public InputStream getInputStream() throws IOException {  
-     InputStream is = this.servletContext.getResourceAsStream(this.path);  
- if (is == null) {  
- thrownew FileNotFoundException("Could not open " + getDescription());  
-     }  
- return is;  
- }  
- 
