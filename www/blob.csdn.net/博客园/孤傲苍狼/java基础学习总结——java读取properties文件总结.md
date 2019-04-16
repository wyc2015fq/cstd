# java基础学习总结——java读取properties文件总结 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [java基础学习总结——java读取properties文件总结](https://www.cnblogs.com/xdp-gacl/p/3640211.html)



## 一、java读取properties文件总结

　　在java项目中，操作properties文件是经常要做的，因为很多的配置信息都会写在properties文件中，这里主要是总结使用**getResourceAsStream**方法和**InputStream**流去读取properties文件，使用**getResourceAsStream**方法去读取properties文件时需要特别注意properties文件路径的写法，测试项目如下：

### 1.1.项目的目录结构

![](data:image/png;base64,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)

### **1.2. java读取properties文件代码测试**

```
/*    范例名称：java读取properties文件总结
 *     源文件名称：PropertiesFileReadTest.java
 *    要  点：
 *        1. 使用getResourceAsStream方法读取properties文件
 *        2. 使用InPutStream流读取properties文件
 *        3. 读取properties文件的路径写法问题
 *    时间：2014/4/2
 */
package propertiesFile.read.test;

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.text.MessageFormat;
import java.util.Properties;

public class PropertiesFileReadTest {

    /**
     * @param args
     */
    public static void main(String[] args) {
        try {
            readPropFileByGetResourceAsStream();
            System.out.println("");
            readPropFileByInPutStream();
        } catch (Exception e) {
            e.printStackTrace();// TODO: handle exception
        }
    }

    /**
     * 使用getResourceAsStream方法读取properties文件
     */
    static void readPropFileByGetResourceAsStream() {
        /**
         * 读取src下面config.properties包内的配置文件 
         * test1.properties位于config.properties包内
         */
        InputStream in1 = PropertiesFileReadTest.class.getClassLoader()
                .getResourceAsStream("config/properties/test1.properties");
        /**
         * 读取和PropertiesFileReadTest类位于同一个包里面的配置文件 
         * test2.properties和PropertiesFileReadTest类在同一个包里面
         */
        InputStream in2 = PropertiesFileReadTest.class
                .getResourceAsStream("test2.properties");
        /**
         * 读取src根目录下文件的配置文件 
         * jdbc.properties位于src目录
         */
        InputStream in3 = PropertiesFileReadTest.class.getClassLoader()
                .getResourceAsStream("jdbc.properties");
        /**
         * 读取位于另一个source文件夹里面的配置文件 
         * config是一个source文件夹，config.properties位于config source文件夹中
         */
        InputStream in4 = PropertiesFileReadTest.class.getClassLoader()
                .getResourceAsStream("config.properties");

        Properties p = new Properties();
        System.out.println("----使用getResourceAsStream方法读取properties文件----");
        try {
            System.out
                    .println("----------------------------------------------");
            p.load(in1);
            System.out.println("test1.properties:name=" + p.getProperty("name")
                    + ",age=" + p.getProperty("age"));
            System.out
                    .println("----------------------------------------------");

            p.load(in2);
            System.out.println("test2.properties:name=" + p.getProperty("name")
                    + ",age=" + p.getProperty("age"));
            System.out
                    .println("----------------------------------------------");

            p.load(in3);
            System.out.println("jdbc.properties:");
            System.out.println(String.format("jdbc.driver=%s",
                    p.getProperty("jdbc.driver")));// 这里的%s是java String占位符
            System.out.println(String.format("jdbc.url=%s",
                    p.getProperty("jdbc.url")));
            System.out.println(String.format("jdbc.usename=%s",
                    p.getProperty("jdbc.usename")));
            System.out.println(String.format("jdbc.password=%s",
                    p.getProperty("jdbc.password")));
            System.out
                    .println("----------------------------------------------");

            p.load(in4);
            System.out.println("config.properties:");
            System.out.println(MessageFormat.format("dbuser={0}",
                    p.getProperty("dbuser")));// {0}是一个java的字符串占位符
            System.out.println(MessageFormat.format("dbpassword={0}",
                    p.getProperty("dbpassword")));
            System.out.println(MessageFormat.format("database={0}",
                    p.getProperty("database")));
            System.out
                    .println("----------------------------------------------");
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } finally {
            if (in1 != null) {
                try {
                    in1.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (in2 != null) {
                try {
                    in2.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (in3 != null) {
                try {
                    in3.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (in4 != null) {
                try {
                    in4.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * 使用InPutStream流读取properties文件
     */
    static void readPropFileByInPutStream() {
        InputStream in1 = null;
        InputStream in2 = null;
        InputStream in3 = null;
        InputStream in4 = null;
        System.out.println("----使用InputStream流读取properties文件----");
        try {
            /**
             * 读取src根目录下文件的配置文件 
             * jdbc.properties位于src目录
             */
            in1 = new BufferedInputStream(new FileInputStream(
                    "src/jdbc.properties"));
            /**
             * 读取src下面config.properties包内的配置文件 
             * test1.properties位于config.properties包内
             */
            in2 = new BufferedInputStream(new FileInputStream(
                    "src/config/properties/test1.properties"));
            /**
             * 读取和PropertiesFileReadTest类位于同一个包里面的配置文件 
             * test2.properties和PropertiesFileReadTest类在同一个包里面
             */
            in3 = new BufferedInputStream(new FileInputStream(
                    "src/propertiesFile/read/test/test2.properties"));
            /**
             * 读取位于另一个source文件夹里面的配置文件 
             * config是一个source文件夹，config.properties位于config source文件夹中
             */
            in4 = new FileInputStream("config/config.properties");

            Properties p = new Properties();
            System.out
                    .println("----------------------------------------------");

            p.load(in1);
            System.out.println("jdbc.properties:");
            System.out.println(String.format("jdbc.driver=%s",
                    p.getProperty("jdbc.driver")));// 这里的%s是java String占位符
            System.out.println(String.format("jdbc.url=%s",
                    p.getProperty("jdbc.url")));
            System.out.println(String.format("jdbc.usename=%s",
                    p.getProperty("jdbc.usename")));
            System.out.println(String.format("jdbc.password=%s",
                    p.getProperty("jdbc.password")));
            System.out
                    .println("----------------------------------------------");

            p.load(in2);
            System.out.println("test1.properties:name=" + p.getProperty("name")
                    + ",age=" + p.getProperty("age"));
            System.out
                    .println("----------------------------------------------");
            p.load(in3);
            System.out.println("test2.properties:name=" + p.getProperty("name")
                    + ",age=" + p.getProperty("age"));
            System.out
                    .println("----------------------------------------------");

            p.load(in4);
            System.out.println("config.properties:");
            System.out.println(MessageFormat.format("dbuser={0}",
                    p.getProperty("dbuser")));// {0}是一个java的字符串占位符
            System.out.println(MessageFormat.format("dbpassword={0}",
                    p.getProperty("dbpassword")));
            System.out.println(MessageFormat.format("database={0}",
                    p.getProperty("database")));
            System.out
                    .println("----------------------------------------------");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (in1 != null) {
                try {
                    in1.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (in2 != null) {
                try {
                    in2.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (in3 != null) {
                try {
                    in3.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if (in4 != null) {
                try {
                    in4.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

}
```

**运行结果：**

![](data:image/png;base64,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)

![](data:image/png;base64,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)

项目[demo下载](http://files.cnblogs.com/xdp-gacl/JavaReadPropertiesFileTest.zip)









