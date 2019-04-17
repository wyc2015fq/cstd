# 利用Velocity自动生成自定义代码_java版_源码下载 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月13日 00:21:49[boonya](https://me.csdn.net/boonya)阅读数：2001








原文地址：[http://www.cnblogs.com/hongten/archive/2013/03/10/hongten_velocity_automatic_code_generation.html](http://www.cnblogs.com/hongten/archive/2013/03/10/hongten_velocity_automatic_code_generation.html)


Velocity很多企业都要求要会，其实也不难，重要的是练过啊，实践出真知。对于有多年开发经验的人而言其实是比较简单的，但是却有可能在实际开发中从来没用过，我就是其中之一。此篇博客借鉴程序员的Velocity经验来加以学习。


**项目结构：**

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_autocodegeneration.png)

运行效果：**你可能会看到项目中有报错，不用着急，这个不会影响到你的项目运行。**

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_autocodegeneration_result1.png)

把其中的User类拿出来，给大家分享一下：

**自动生成UserDao.java**效果：

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_autocode_userdao.png)

****自动生成**UserDaoImpl.java**效果：

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_autocode_userdaoimpl.png)

****自动生成**UserService.java**效果：

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_autocode_userservice.png)

****自动生成**UserServiceImpl.java**效果：

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_autocode_userserviceimpl.png)

我想大家看到这里，是不是有亲自动手试试的兴奋感觉....

=======================================================

代码部分:这个是对“**[自己写的一个代码自动生成工具_java版_源码下载](http://www.cnblogs.com/hongten/archive/2013/02/24/hongten_code_create.html)**”的改进

=======================================================

User.java



```java
/**
 * 
 */
package com.b510.base.bean;

/**
 * @author hongten<br>
 * @date 2013-3-10
 */
public class User {

}
```




Annotation.java




```java
/**
 * 
 */
package com.b510.base.bean.install;

/**
 * 注释
 * @author hongten<br>
 * @date 2013-3-10
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
Bean.java





```java
/**
 * 
 */
package com.b510.base.bean.install;

/**
 * bean类
 * 
 * @author hongten<br>
 * @date 2013-3-10
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




BeanUtils.java




```java
/**
 * 
 */
package com.b510.base.bean.install;

import java.io.File;
import java.io.FileWriter;
import java.io.StringWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;


/**
 * @author hongten<br>
 * @date 2013-3-10
 */
public class BeanUtils {
    
    public static final String BEAN_DAO_TEMPLATE_VM_PATH = "src/vms/beanDao.vm";
    public static final String BEAN_DAO_IMPL_TEMPLATE_VM_PATH = "src/vms/beanDaoImpl.vm";
    public static final String BEAN_SERVICE_TEMPLATE_VM_PATH = "src/vms/beanService.vm";
    public static final String BEAN_SERVICE_IMPL_TEMPLATE_VM_PATH = "src/vms/beanServiceImpl.vm";

     //文件 地址
    //private static final String BEAN_PATH = "com/b510/base/bean";
    private static final String DAO_PATH = "com/b510/base/dao";
    private static final String DAO_IMPL_PATH = "com/b510/base/dao/impl";
    private static final String SERVICE_PATH = "com/b510/base/service";
    private static final String SERVICE_IMPL_PATH = "com/b510/base/service/impl";

    
    
    //包名
    private static final String BEAN_URL = "com.b510.base.bean";
    private static final String DAO_URL = "com.b510.base.dao";
    private static final String DAO_IMPL_URL = "com.b510.base.dao.impl";
    private static final String SERVICE_URL = "com.b510.base.service";
    private static final String SERVICE_IMPL_URL = "com.b510.base.service.impl";

    
    private static Bean bean = new Bean();
    private static Annotation annotation = new Annotation();
    
    /**
     * 初始化bean和注解
     * @param c
     */
    public void init(Class c){
        if(c != null){
            String cName = c.getName();
            bean.setName(getLastChar(cName));
            bean.setBeanUrl(cName);
            bean.setLowerName(getLowercaseChar(getLastChar(cName)));

            annotation.setAuthorName("hongten");
            annotation.setAuthorMail("hongtenzone@foxmail.com");
            SimpleDateFormat simpleDateFormat = new SimpleDateFormat(
                    "yyyy-MM-dd");
            annotation.setDate(simpleDateFormat.format(new Date()));
            annotation.setVersion("1.0");
        }
    }
    
    /**
     * 创建bean的Dao<br>
     * 
     * @param c
     * @throws Exception
     */
    public void createBeanDao(Class c) throws Exception {
        String cName = c.getName();
        String path = System.getProperty("user.dir") + "/src/" + DAO_PATH
                + "/";
        File filePath = new File(path);
        createFilePath(filePath);
        
        String fileName = path + getLastChar(cName) + "Dao.java";
        File file = new File(fileName);
        FileWriter fw = new FileWriter(file);
        
        bean.setBeanDaoUrl(DAO_URL);
        
        fw.write(createCode(BEAN_DAO_TEMPLATE_VM_PATH,bean,annotation));
        fw.flush();
        fw.close();
        showInfo(fileName);
    }
    
    /**
     * 创建bean的Dao的实现<br>
     * 
     * @param c
     * @throws Exception
     */
    public void createBeanDaoImpl(Class c) throws Exception {
        String cName = c.getName();
        String path = System.getProperty("user.dir") + "/src/" + DAO_IMPL_PATH
                + "/";
        File filePath = new File(path);
        createFilePath(filePath);
        
        String fileName = path + getLastChar(cName) + "DaoImpl.java";
        File file = new File(fileName);
        FileWriter fw = new FileWriter(file);
        
        bean.setBeanDaoUrl(DAO_URL);
        bean.setBeanDaoImplUrl(DAO_IMPL_URL);
        
        fw.write(createCode(BEAN_DAO_IMPL_TEMPLATE_VM_PATH,bean,annotation));
        fw.flush();
        fw.close();
        showInfo(fileName);
    }
    
    
    /**
     * 创建bean的Service<br>
     * 
     * @param c
     * @throws Exception
     */
    public void createBeanService(Class c) throws Exception {
        String cName = c.getName();
        String path = System.getProperty("user.dir") + "/src/" + SERVICE_PATH
                + "/";
        File filePath = new File(path);
        createFilePath(filePath);
        
        String fileName = path + getLastChar(cName) + "Service.java";
        File file = new File(fileName);
        FileWriter fw = new FileWriter(file);
        
        bean.setBeanServiceUrl(SERVICE_URL);
        
        fw.write(createCode(BEAN_SERVICE_TEMPLATE_VM_PATH,bean,annotation));
        fw.flush();
        fw.close();
        showInfo(fileName);
    }
    
    /**
     * 创建bean的Service的实现<br>
     * 
     * @param c
     * @throws Exception
     */
    public void createBeanServiceImpl(Class c) throws Exception {
        String cName = c.getName();
        String path = System.getProperty("user.dir") + "/src/" + SERVICE_IMPL_PATH
                + "/";
        File filePath = new File(path);
        createFilePath(filePath);
        
        String fileName = path + getLastChar(cName) + "ServiceImpl.java";
        File file = new File(fileName);
        FileWriter fw = new FileWriter(file);
        
        bean.setBeanDaoUrl(DAO_URL);
        bean.setBeanServiceUrl(SERVICE_URL);
        bean.setBeanServiceImplUrl(SERVICE_IMPL_URL);
        
        fw.write(createCode(BEAN_SERVICE_IMPL_TEMPLATE_VM_PATH,bean,annotation));
        fw.flush();
        fw.close();
        showInfo(fileName);
    }
    
    
    
    
    
    
    
    
    
    
    
    /**
     * 根据模板生成代码
     * @param fileVMPath 模板路径 
     * @param bean 目标bean
     * @param annotation 注释
     * @return
     * @throws Exception
     */
    public String createCode(String fileVMPath,Bean bean,Annotation annotation) throws Exception{
        VelocityEngine velocityEngine = new VelocityEngine();
        velocityEngine.setProperty("input.encoding", "UTF-8");
        velocityEngine.setProperty("output.encoding", "UTF-8");
        velocityEngine.init();
        Template template = velocityEngine.getTemplate(fileVMPath);
        VelocityContext velocityContext = new VelocityContext();
        velocityContext.put("bean", bean);
        velocityContext.put("annotation", annotation);
        StringWriter stringWriter = new StringWriter();
        template.merge(velocityContext, stringWriter);
        return stringWriter.toString();
    }
    
     /**
     * 创建文件
     * @param file
     */
    public void createFilePath(File file){
        if(!file.exists()){
            System.out.println("创建["+file.getAbsolutePath()+"]情况："+file.mkdirs());
        }else{
            System.out.println("存在目录："+file.getAbsolutePath());
        }
    }
    
 
    
    
    /**
     * 获取路径的最后面字符串<br>
     * 如：<br>
     *     <code>str = "com.b510.base.bean.User"</code><br>
     *     <code> return "User";<code>
     * @param str
     * @return
     */
    public String getLastChar(String str) {
        if ((str != null) && (str.length() > 0)) {
            int dot = str.lastIndexOf('.');
            if ((dot > -1) && (dot < (str.length() - 1))) {
                return str.substring(dot + 1);
            }
        }
        return str;
    }
    
    /**
     * 把第一个字母变为小写<br>
     * 如：<br>
     *     <code>str = "UserDao";</code><br>
     *     <code>return "userDao";</code>
     * @param str
     * @return
     */
    public String getLowercaseChar(String str){
        return str.substring(0,1).toLowerCase()+str.substring(1);
    }

    /**
     * 显示信息
     * @param info
     */
    public void showInfo(String info){
        System.out.println("创建文件："+ info+ "成功！");
    }
    
    /**
     * 获取系统时间
     * @return
     */
    public static String getDate(){
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd");
        return simpleDateFormat.format(new Date());
    }
}
```
BeanUtilsTest.java





```java
/**
 * 
 */
package com.b510.base.bean.install;

import com.b510.base.bean.Admin;
import com.b510.base.bean.People;
import com.b510.base.bean.User;


/**
 * @author hongten<br>
 * @date 2013-3-10
 */
public class BeanUtilsTest{
    public static void main(String[] args) throws Exception{
        BeanUtilsTest beanUtilTest = new BeanUtilsTest();
        BeanUtils beanUtils = new BeanUtils();
        beanUtilTest.beanTool(beanUtils, User.class);
        beanUtilTest.beanTool(beanUtils, People.class);
        beanUtilTest.beanTool(beanUtils, Admin.class);
    }
    
    /**
     * 根据bean生成相应的文件
     * @param beanUtils
     * @param c
     * @throws Exception
     */
    @SuppressWarnings("unchecked")
    public void beanTool(BeanUtils beanUtils ,Class c)throws Exception{
        beanUtils.init(c);
        beanUtils.createBeanDao(c);
        beanUtils.createBeanDaoImpl(c);
        beanUtils.createBeanService(c);
        beanUtils.createBeanServiceImpl(c);
    }
}
```
beanDao.vm





```
##create bean dao,this is a interface
package $!{bean.beanDaoUrl};

import com.blackants.core.dao.BaseDao;
import $!{bean.beanUrl};

/**
 * @author <a href="mailto:$!{annotation.authorMail}">$!{annotation.authorName}</a>
 * @date $!{annotation.date}
 *
 * @version $!{annotation.version}
 */
public interface $!{bean.name}Dao extends BaseDao<$!{bean.name}> {

}
```



beanDaoImpl.vm




```java
##create bean daoImpl,this is a class
package $!{bean.beanDaoImplUrl};

import com.blackants.core.dao.impl.AbstractBaseDaoImpl;
import $!{bean.beanDaoUrl}.$!{bean.name}Dao;
import $!{bean.beanUrl};

/**
 * @author <a href="mailto:$!{annotation.authorMail}">$!{annotation.authorName}</a>
 * @date $!{annotation.date}
 *
 * @version $!{annotation.version}
 */
public class $!{bean.name}DaoImpl extends AbstractBaseDaoImpl<$!{bean.name}> implements $!{bean.name}Dao{

}
```
beanService.vm





```java
##create bean service,this is a interface
package $!{bean.beanServiceUrl};

import com.blackants.core.service.BaseService;
import $!{bean.beanUrl};

/**
 * @author <a href="mailto:$!{annotation.authorMail}">$!{annotation.authorName}</a>
 * @date $!{annotation.date}
 *
 * @version $!{annotation.version}
 */
public interface $!{bean.name}Service extends BaseService<$!{bean.name}> {

}
```

beanServiceImpl.vm





```java
##create bean serviceImpl,this is a class
package $!{bean.beanServiceImplUrl};

import com.blackants.core.service.impl.AbstractBaseServiceImpl;
import com.blackants.core.dao.BaseDao;
import $!{bean.beanDaoUrl}.$!{bean.name}Dao;
import $!{bean.beanServiceUrl}.$!{bean.name}Service;
import $!{bean.beanUrl};

/**
 * @author <a href="mailto:$!{annotation.authorMail}">$!{annotation.authorName}</a>
 * @date $!{annotation.date}
 *
 * @version $!{annotation.version}
 */
public class $!{bean.name}ServiceImpl extends AbstractBaseServiceImpl<$!{bean.name}> implements $!{bean.name}Service{

    /**
     * $!{bean.lowerName} Dao
     */
    private $!{bean.name}Dao $!{bean.lowerName}Dao;
    
    @Override
    public BaseDao<$!{bean.name}> getBaseDao() {
        return $!{bean.lowerName}Dao;
    }

    // =================setter================= //
    
    public void setActivationCodeDao($!{bean.name}Dao $!{bean.lowerName}Dao) {
        this.$!{bean.lowerName}Dao = $!{bean.lowerName}Dao;
    }
    
}
```



**源码下载**：[http://files.cnblogs.com/hongten/AutomaticCodeGeneration.rar](http://files.cnblogs.com/hongten/AutomaticCodeGeneration.rar)

**下载说明：**

如果你下载了此压缩包，并且在你的本地导入eclipse，运行程序你会发现系统找不到：
com.blackants.core.dao.BaseDao;(interface)
com.blackants.core.dao.impl.AbstractBaseDaoImpl;(class)com.blackants.core.service.BaseService;(interface)com.blackants.core.service.impl.AbstractBaseServiceImpl;(class)
上面所列出的文件，这个不用着急，你可以在项目中新建这些package,然后加入就可以啦







