# 走出Java资源加载的迷宫 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年04月23日 18:01:53[gloomyfish](https://me.csdn.net/jia20003)阅读数：3614








**走出Java资源加载的迷宫**

Java开发中常常要加载各种各样的资源文件，如property文件，xml配置文件，图片文件等等。

Java的资源加载策略选择比较多，很多资源加载方法都是基于不同的项目配置，没有一种资

源加载方法是万能的。首先来看看Java中的几种常见的资源加载方法：



**一：通过ResourceBundle类来实现资源加载**

这种方式通常被用来加载多语言支持的property文件，Java语言通过ResourceBundle可以非

常好的支持国际化。假设在默认为英文的系统中有语言资源文件mybundle_en.properties，则

调用代码如下：

**ResourceBundle myBundle =ResourceBundle.*****getBundle*****(****"mybundle"****);**

资源文件mybundle_en.properties的位置应该在sourcefolder的根目录下，假如资源文件位于

包路径com.mybundle.test下则上述代码应该改写为：

**ResourceBundle myBundle =ResourceBundle.*****getBundle*****(****"****com.mybundle.test.****mybundle"****);**



二：通过Class类的getResourceAsStream()方法来实现资源加载

这种加载方式同时是最常用也是最容易让开发者搞错的方法，getResourceAsStream()方法使用

默认文件名加载资源时，要求资源必须与Class文件同时相同的包路径。加载代码如下：

client.getClass().getResourceAsStream("mybundle.properties");



三：通过ClassLoader的getResourceAsStream()来实现资源文件加载

通过ClassLoader来加载资源文件是一种非常有效和实用的方法，只要资源位于classpath的路

径下面，ClassLoader都可以通过适当的策略来找到你要加载的资源。但是应该注意的是如果你

的资源位于SourceFloder的根目录，如下代码就可以完成：

**this**.getClass().getClassLoader().getResource("mybundle.properties");

如果你的资源文件位于独立的config文件中，只需要把config配置到classpath的变量中，然后使

用如下代码即可：

**this**.getClass().getClassLoader().getResource("config/mybundle.properties");



**三种资源加载方式比较与总结：**
|**方法**|**参数**|**失败时行为**|**用法示例代码**|
|----|----|----|----|
|ClassLoader.getResourceAsStream()|绝对路径加载时候不需要反斜杠/|Silent (return null)|This.getCalss().getClassLoader().getResourceAsStream(“some/pkg/resource.prpperties”);|
|Class.getResourceAsStream()|有反斜杠/表现为绝对路径|Silent(return null)|This.getClass().getResourceAsStream(“resource.properties”);|
|ResourceBundle.getBundle()|不需要反斜杠/, .已经暗示绝对路径.properties为其默认的文件扩展名|Throws uncheckedjava.util.MissingResourceException|ResourceBundle.getBundle(“some.pkg.resource”)|

**资源加载策略说明：**

不同的开发者对资源文件的管理千差万别，有的喜欢把它们放在不同的类路径下面，有的喜

欢放在单一的config文件下，没有一种方法是错误的。但是为了能够正确的加载到你需要的

资源文件，永远应该记得把资源文件夹的路径配置做为classpath的一部分，如果资源文件没

有位于你的source folder之内。否则你不得不通过额外的手段来配置与加载你的资源文件。

基于上述的总结，完成了一个资源加载的UtilityClass, 让你从此不再对Java中资源加载怀有好奇

与恐惧之心。

**Resource Loading Utility Class源代码如下：**



```java
import java.io.InputStream;
import java.util.Locale;
import java.util.Properties;
import java.util.ResourceBundle;

public class CommonResourceLoader {

		/**
		 * please use this method if property files located at same package with CommonResourceLoader
		 * Java class
		 * 
		 * @param PropertiesName
		 * @return
		 */
        public static Properties loadPropertyFile(String PropertiesName) {

              Properties props = new Properties();

              try
              {
                  InputStream in = CommonResourceLoader.class.getResourceAsStream(PropertiesName);
                 if (in != null)
                 {
                    props.load(in);
                 } 
                 else
                 {
                    throw new IllegalArgumentException("No file exists for" + PropertiesName);
                 }
              }
              catch (Exception e)
              {
                 e.printStackTrace();
              }

              return props;
        }
        
        /**
         * if the properties file is located at obj's package, please use it
         * @param obj
         * @param PropertiesName
         * @return
         */
        public static Properties loadPropertyFile(Class obj,  String PropertiesName) {

              Properties props = new Properties();

              try
              {
                  InputStream in = obj.getResourceAsStream(PropertiesName);
                 if (in != null)
                 {
                    props.load(in);
                 } 
                 else
                 {
                    throw new IllegalArgumentException("No file exists for" + PropertiesName);
                 }
              }
              catch (Exception e)
              {
                 e.printStackTrace();
              }

              return props;
        }
        
        /**
         * base name is file name if file located at class path root directory
         * base name is directory + file full name if file at some folder inside class path list.
         * eg: myPro.properties
         * project structure
         * pro--src
         *  |    |---com.mytest.pkg
         *  |    |               |-----Hello.java
         *  |    |               |-----test.properties
         *  |---myPro.properties
         *  baseName= com/mytest/pkg/test.properties if load test.properties
         *  baseName= myPro.properties if load myPro.properties
         *
         * @param baseName
         * @return
         */
        public static Properties loadCurrentPropertyFile(String baseName) {
            Properties props = new Properties();

            try
            {
                InputStream in = CommonResourceLoader.class.getClassLoader().getResourceAsStream(baseName);
               if (in != null)
               {
                  props.load(in);
               } 
               else
               {
                  throw new IllegalArgumentException("No file exists for" + baseName);
               }
            }
            catch (Exception e)
            {
               e.printStackTrace();
            }

            return props;
        }
        
        public static Properties loadSystemPropertyFile(String name, ClassLoader loader) {
            Properties props = new Properties();

            try
            {
            	if(loader == null)
            		loader = ClassLoader.getSystemClassLoader();
                InputStream in = loader.getResourceAsStream(name);
               if (in != null)
               {
                  props.load(in);
               } 
               else
               {
                  throw new IllegalArgumentException("No file exists for" + name);
               }
            }
            catch (Exception e)
            {
               e.printStackTrace();
            }

            return props;
        }
        
        /**
         * resourceURL is file name if file located at class path root directory
         * resourceURL is directory + file full name if file at some folder inside class path list.
         * eg: myPro.properties
         * project structure
         * pro--src
         *  |    |---com.mytest.pkg
         *  |    |               |-----Hello.java
         *  |    |               |-----test.properties
         *  |---myPro.properties
         *  resourceURL= com.mytest.pkg.test.properties if load test.properties
         *  resourceURL= myPro.properties if load myPro.properties
         *
         * @param baseName
         * @return
         */
        public static ResourceBundle loadResourceBundle(String resourceURL) {

                Locale locale = Locale.ENGLISH;
                ResourceBundle bundle = null;

                try {
                        bundle = ResourceBundle.getBundle(resourceURL, locale);
                        if (bundle == null) {
                                throw new IllegalArgumentException("No file exists for" + resourceURL);
                        }
                } catch (Exception e) {
                        e.printStackTrace();
                }

                return bundle;
        }

}
```
**特别说明：**


这里的资源是指配置文件(xml,property等), 图片,Java程序运行需要的一切文本文件等等。






