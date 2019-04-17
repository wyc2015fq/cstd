# FreeMarker_模板引擎_代码自动生成器_源码下载 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月13日 00:25:46[boonya](https://me.csdn.net/boonya)阅读数：699








原文地址：[http://www.cnblogs.com/hongten/archive/2013/04/05/hongten_freemarker.html](http://www.cnblogs.com/hongten/archive/2013/04/05/hongten_freemarker.html)


项目结构：

![](http://images.cnblogs.com/cnblogs_com/hongten/467317/o_hongten_freemarker_project.png)

运行代码：

```
1 public static void main(String[] args) throws Exception {
2     helloWorld(FTLS_PATH, HONGTEN_HELLO_WORLD_FTL);
3  }
```

运行效果：

![](http://images.cnblogs.com/cnblogs_com/hongten/467317/o_hongten_hellworld_freemarker.png)

运行代码：

```
1 public static void main(String[] args) throws Exception {
2         myJavaFile(FTLS_PATH,BEAN_URL,HONGTEN_MY_JAVA_FILE_FTL);
3     }
```

运行效果：

![](http://images.cnblogs.com/cnblogs_com/hongten/467317/o_hongten_freemarker_result.png)

生成的User.java文件：

![](http://images.cnblogs.com/cnblogs_com/hongten/467317/o_hongten_freemarker_user.png)

==================================================================

代码部分：   你可以了解一下velocity(也是一个模板引擎)：[利用Velocity自动生成自定义代码_java版_源码下载](http://www.cnblogs.com/hongten/archive/2013/03/10/hongten_velocity_automatic_code_generation.html)

==================================================================

/freemarker/src/com/b510/freemarker/Bean.java

```
```java
package com.b510.freemarker;

/**
 * bean类
 * 
 * @author hongten(hongtenzone@foxmail.com)<br>
 * @date 2013-4-5
 */
public class Bean {

    /** bean 名称 */
    private String name;
    /** bean 首字母小写名称 */
    private String lowerName;
    /** bean 路径 */
    private String beanUrl;
    /** dao 路径 */
    private String beanDaoUrl;
    /** dao 实现路径 */
    private String beanDaoImplUrl;
    /** service 路径 */
    private String beanServiceUrl;
    /** service 实现路径 */
    private String beanServiceImplUrl;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getLowerName() {
        return lowerName;
    }

    public void setLowerName(String lowerName) {
        this.lowerName = lowerName;
    }

    public String getBeanUrl() {
        return beanUrl;
    }

    public void setBeanUrl(String beanUrl) {
        this.beanUrl = beanUrl;
    }

    public String getBeanDaoUrl() {
        return beanDaoUrl;
    }

    public void setBeanDaoUrl(String beanDaoUrl) {
        this.beanDaoUrl = beanDaoUrl;
    }

    public String getBeanDaoImplUrl() {
        return beanDaoImplUrl;
    }

    public void setBeanDaoImplUrl(String beanDaoImplUrl) {
        this.beanDaoImplUrl = beanDaoImplUrl;
    }

    public String getBeanServiceUrl() {
        return beanServiceUrl;
    }

    public void setBeanServiceUrl(String beanServiceUrl) {
        this.beanServiceUrl = beanServiceUrl;
    }

    public String getBeanServiceImplUrl() {
        return beanServiceImplUrl;
    }

    public void setBeanServiceImplUrl(String beanServiceImplUrl) {
        this.beanServiceImplUrl = beanServiceImplUrl;
    }

}
```
```

/freemarker/src/com/b510/freemarker/Annotation.java

```
```java
package com.b510.freemarker;

/**
 * 注释
 * 
 * @author hongten(hongtenzone@foxmail.com)<br>
 * @date 2013-4-5
 */
public class Annotation {

    /**
     * 作者名称
     */
    private String authorName;
    /**
     * 作者邮箱
     */
    private String authorMail;
    /**
     * 日期
     */
    private String date;
    /**
     * 版本
     */
    private String version;

    public String getAuthorName() {
        return authorName;
    }

    public void setAuthorName(String authorName) {
        this.authorName = authorName;
    }

    public String getAuthorMail() {
        return authorMail;
    }

    public void setAuthorMail(String authorMail) {
        this.authorMail = authorMail;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }

}
```
```

/freemarker/src/com/b510/freemarker/MyFreeMarker.java

```
```java
/**
 * 
 */
package com.b510.freemarker;

import java.io.File;
import java.io.FileWriter;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import freemarker.template.Configuration;
import freemarker.template.Template;

/**
 * freemarker测试
 * 
 * @author hongten(hongtenzone@foxmail.com)<br>
 * @date 2013-4-5
 */
public class MyFreeMarker {

    private static Configuration configuration;
    private static Template template;
    private static Writer writer;
    /**
     * 模板文件的存放路径，这里是存放在项目根目录下的ftls文件夹中
     */
    public static final String FTLS_PATH = "ftls";

    public static final String MESSAGE = "message";
    public static final String HELLO_WORLD = "Hello World!";
    public static final String HONGTEN_HELLO_WORLD_FTL = "hongten-helloworld.ftl";
    public static final String HONGTEN_MY_JAVA_FILE_FTL = "hongten-myJavaFile.ftl";

    // bean
    public static final String BEAN = "bean";
    public static final String BEAN_URL = "com.b510.bean";

    // annotation
    public static final String ANNOTATION = "annotation";
    public static final String ANNOTATION_AUTHOR_NAME = "hongten";
    public static final String ANNOTATION_AUTHOR_MAIL = "hongtenzone@foxmail.com";
    public static final String ANNOTATION_VERSION = "1.0";

    // date formate
    public static final String DATE_FROMATE = "yyyy-MM-dd";

    public static void main(String[] args) throws Exception {
        // helloWorld(FTLS_PATH, HONGTEN_HELLO_WORLD_FTL);
        myJavaFile(FTLS_PATH, BEAN_URL, HONGTEN_MY_JAVA_FILE_FTL);
    }

    /**
     * 利用模板在控制台打印helloworld信息
     * 
     * @param path
     *            模板存放的路径
     * @param ftlFile
     *            模板文件
     * @throws Exception
     */
    public static void helloWorld(String path, String ftlFile) throws Exception {
        // 创建Freemarker配置实例
        configuration = new Configuration();
        configuration.setDirectoryForTemplateLoading(new File(path));

        // 创建数据模型
        Map<String, String> root = new HashMap<String, String>();
        root.put(MESSAGE, HELLO_WORLD);

        // 加载模板文件
        template = configuration.getTemplate(ftlFile);

        // 显示生成的数据，这里打印在控制台
        writer = new OutputStreamWriter(System.out);
        template.process(root, writer);
        writer.flush();
        writer.close();
    }

    /**
     * 利用freemarker生成自定义的javaBean
     * 
     * @param path
     *            模板路径
     * @param packageUrl
     *            javaBean的url，即package名称
     * @param ftlFile
     *            使用的模板文件
     * @throws Exception
     */
    public static void myJavaFile(String path, String packageUrl, String ftlFile) throws Exception {
        // 创建Freemarker配置实例
        configuration = new Configuration();
        configuration.setDirectoryForTemplateLoading(new File(path));

        // 创建数据模型
        Map<String, Object> root = new HashMap<String, Object>();
        Bean bean = new Bean();
        bean.setName("User");
        bean.setLowerName("user");
        bean.setBeanUrl(packageUrl);
        root.put(BEAN, bean);

        Annotation annotation = new Annotation();
        annotation.setAuthorMail(ANNOTATION_AUTHOR_MAIL);
        annotation.setAuthorName(ANNOTATION_AUTHOR_NAME);
        annotation.setVersion(ANNOTATION_VERSION);
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat(DATE_FROMATE);
        annotation.setDate(simpleDateFormat.format(new Date()));
        root.put(ANNOTATION, annotation);

        // 加载模板文件
        template = configuration.getTemplate(ftlFile);

        String beanPath = System.getProperty("user.dir") + "/src/" + packageUrl.replace(".", "/") + "/";
        File filePath = new File(beanPath);
        if (!filePath.exists()) {
            filePath.mkdirs();
        }

        String filePathOfBean = beanPath + "/User.java";
        File file = new File(filePathOfBean);
        if (!file.exists()) {
            file.createNewFile();
        }

        // 显示生成的数据
        writer = new FileWriter(file);
        template.process(root, writer);
        writer.flush();
        writer.close();
    }
}
```
```

/freemarker/ftls/hongten-helloworld.ftl


`${message}`



/freemarker/ftls/hongten-myJavaFile.ftl

```
```java
package ${bean.beanUrl};

import java.util.Date;

/**
 * @author ${annotation.authorName}(${annotation.authorMail})<br>
 * @date ${annotation.date}
 * 
 * @version ${annotation.version}
 */
public class ${bean.name} {

    /**
     * id号
     */
    private Integer id;
    /**
     * 姓名
     */
    private String name;
    /**
     * 性别
     */
    private String sex;
    /**
     * 生日
     */
    private Date birthday;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSex() {
        return sex;
    }

    public void setSex(String sex) {
        this.sex = sex;
    }

    public Date getBirthday() {
        return birthday;
    }

    public void setBirthday(Date birthday) {
        this.birthday = birthday;
    }

}
```
```

**源码下载：**[http://files.cnblogs.com/hongten/freemarker_file.zip](http://files.cnblogs.com/hongten/freemarker_file.zip)



