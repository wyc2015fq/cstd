# 技术实现(3)之备份文件异地容灾机制(FTP上传) - 零度的博客专栏 - CSDN博客
2016年11月29日 13:38:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：793
[APDPlat](https://github.com/ysc/APDPlat)在数据库备份成功之后，会调用一个**系统扩展点**，用户可以方便地编写自己的包含特定业务逻辑的**插件**，并可**配置**启用哪些插件。本文以将备份文件上传到FTP服务器以实现异地容灾为例子，来说明如何**编写**自己的插件并**配置**使其生效。
1、**如何编写？**
我们先看看系统扩展点，即BackupFileSender接口：
Java代码 ![复制代码](http://yangshangchuan.iteye.com/images/icon_copy.gif)![收藏代码](http://yangshangchuan.iteye.com/images/icon_star.png)![](http://yangshangchuan.iteye.com/images/spinner.gif)
- /**
-  * 备份文件发送器
-  * 将最新的备份文件发送到其他机器，防止服务器故障丢失数据
-  * @author 杨尚川
-  */
- publicinterface BackupFileSender {  
- publicvoid send(File file);  
- }  
```java
/**
 * 备份文件发送器
 * 将最新的备份文件发送到其他机器，防止服务器故障丢失数据
 * @author 杨尚川
 */
public interface BackupFileSender {
    public void send(File file);
}
```
然后新建一个类FtpBackupFileSender，实现系统扩展点：
Java代码 ![复制代码](http://yangshangchuan.iteye.com/images/icon_copy.gif)![收藏代码](http://yangshangchuan.iteye.com/images/icon_star.png)![](http://yangshangchuan.iteye.com/images/spinner.gif)
- /**
-  * 将备份文件发送到FTP服务器上面
-  * @author 杨尚川
-  */
- @Service
- publicclass FtpBackupFileSender implements BackupFileSender{  
- protectedfinal APDPlatLogger LOG = new APDPlatLogger(getClass());  
- 
- @Resource(name="ftpUtils")  
- private FtpUtils ftpUtils;  
- 
- @Resource(name="configurationEncryptor")  
- private StandardPBEStringEncryptor configurationEncryptor;  
- 
- @Override
- publicvoid send(File file) {  
- try{  
-             String host = PropertyHolder.getProperty("ftp.server.host");  
- int port = PropertyHolder.getIntProperty("ftp.server.port");  
-             String username = PropertyHolder.getProperty("ftp.server.username");  
-             String password = PropertyHolder.getProperty("ftp.server.password");  
- if(username!=null && username.contains("ENC(") && username.contains(")")){  
-                 username=username.substring(4,username.length()-1);  
-             }  
- if(password!=null && password.contains("ENC(") && password.contains(")")){  
-                 password=password.substring(4,password.length()-1);  
-             }          
-             username = configurationEncryptor.decrypt(username);  
-             password = configurationEncryptor.decrypt(password);  
-             String dist = PropertyHolder.getProperty("log.backup.file.ftp.dir");  
-             String database = PropertyHolder.getProperty("jpa.database");  
-             dist = dist.replace("${database}", database);  
-             LOG.info("本地备份文件："+file.getAbsolutePath());  
-             LOG.info("FTP服务器目标目录："+dist);  
- boolean connect = ftpUtils.connect(host, port, username, password);  
- if(connect){  
- boolean result = ftpUtils.uploadTo(file, dist);  
- if(result){  
-                     LOG.info("备份文件上传到FTP服务器成功");  
-                 }else{  
-                     LOG.error("备份文件上传到FTP服务器失败");  
-                 }  
-             }  
-         }catch(Exception e){  
-             LOG.error("备份文件上传到FTP服务器失败",e);  
-         }  
-     }  
- }  
```java
/**
 * 将备份文件发送到FTP服务器上面
 * @author 杨尚川
 */
@Service
public class FtpBackupFileSender implements BackupFileSender{
    protected final APDPlatLogger LOG = new APDPlatLogger(getClass());
    
    @Resource(name="ftpUtils")
    private FtpUtils ftpUtils;
    
    @Resource(name="configurationEncryptor")
    private StandardPBEStringEncryptor configurationEncryptor;
    
    @Override
    public void send(File file) {
        try{
            String host = PropertyHolder.getProperty("ftp.server.host");
            int port = PropertyHolder.getIntProperty("ftp.server.port");
            String username = PropertyHolder.getProperty("ftp.server.username");
            String password = PropertyHolder.getProperty("ftp.server.password");
            if(username!=null && username.contains("ENC(") && username.contains(")")){
                username=username.substring(4,username.length()-1);
            }
            if(password!=null && password.contains("ENC(") && password.contains(")")){
                password=password.substring(4,password.length()-1);
            }        
            username = configurationEncryptor.decrypt(username);
            password = configurationEncryptor.decrypt(password);
            String dist = PropertyHolder.getProperty("log.backup.file.ftp.dir");
            String database = PropertyHolder.getProperty("jpa.database");
            dist = dist.replace("${database}", database);
            LOG.info("本地备份文件："+file.getAbsolutePath());
            LOG.info("FTP服务器目标目录："+dist);
            boolean connect = ftpUtils.connect(host, port, username, password);
            if(connect){
                boolean result = ftpUtils.uploadTo(file, dist);
                if(result){
                    LOG.info("备份文件上传到FTP服务器成功");
                }else{
                    LOG.error("备份文件上传到FTP服务器失败");
                }
            }
        }catch(Exception e){
            LOG.error("备份文件上传到FTP服务器失败",e);
        }
    }
}
```
这里有**三个要点**：一是跟FTP服务器相关的信息（主机、端口、用户名、密码）从配置文件config.properties或config.local.properties中获取；二是使用FtpUtils的uploadTo方法实现实际的文件上传功能；三是使用StandardPBEStringEncryptor对加密的用户名和密码进行解密（配置文件中不存放明文用户名和密码）。
先看看FTP服务器的配置信息，默认存放在config.properties中，用户可以在config.local.properties中进行重新指定以覆盖默认配置：
Java代码 ![复制代码](http://yangshangchuan.iteye.com/images/icon_copy.gif)![收藏代码](http://yangshangchuan.iteye.com/images/icon_star.png)![](http://yangshangchuan.iteye.com/images/spinner.gif)
- ftp.server.host=192.168.0.100
- ftp.server.port=21
- ftp.server.username=ENC(rAjYIMF6ANd2q/cTgX6SpQ==)  
- ftp.server.password=ENC(GHVWGhan3XajaRZF8QzZKQ==)  
```java
ftp.server.host=192.168.0.100
ftp.server.port=21
ftp.server.username=ENC(rAjYIMF6ANd2q/cTgX6SpQ==)
ftp.server.password=ENC(GHVWGhan3XajaRZF8QzZKQ==)
```
接下来看看FtpUtils的实现：
Java代码 ![复制代码](http://yangshangchuan.iteye.com/images/icon_copy.gif)![收藏代码](http://yangshangchuan.iteye.com/images/icon_star.png)![](http://yangshangchuan.iteye.com/images/spinner.gif)
- /**
-  * FTP操作工具
-  * @author 杨尚川
-  */
- @Service
- publicclass FtpUtils {  
- protectedfinal APDPlatLogger LOG = new APDPlatLogger(getClass());  
- 
- private FTPClient ftpClient;  
- 
- /**
-      * 连接FTP服务器
-      * @param host FTP服务器地址
-      * @param port FTP服务器端口号
-      * @param username 用户名
-      * @param password 密码
-      * @return
-      */
- publicboolean connect(String host, int port, String username, String password) {  
- try{  
-             ftpClient = new FTPClient();  
-             ftpClient.connect(host, port);  
-             ftpClient.login(username, password);  
-             ftpClient.setFileType(FTPClient.BINARY_FILE_TYPE);  
- int reply = ftpClient.getReplyCode();  
- if (!FTPReply.isPositiveCompletion(reply)) {  
-                 ftpClient.disconnect();  
-                 LOG.error("连接FTP服务器失败，响应码："+reply);  
- returnfalse;  
-             }  
-         }catch(IOException e){  
-             LOG.error("连接FTP服务器失败",e);  
- returnfalse;  
-         }  
- returntrue;  
-     }  
- /**
-      * 上传文件到服务器上的特定路径
-      * @param file 上传的文件或文件夹
-      * @param path 上传到ftp服务器哪个路径下
-      * @return 
-      */
- publicboolean uploadTo(File file, String path){  
- //切换到服务器上面的合适目录
- //如果对应的目录不存在，则创建
-         LOG.info("上传文件 "+file.getAbsolutePath()+" 到服务器路径 "+path);  
- try{  
-             String[] segs = path.split("/");  
- for(String seg : segs){  
- if(!ftpClient.changeWorkingDirectory(seg)){  
-                     ftpClient.makeDirectory(seg);  
- if(!ftpClient.changeWorkingDirectory(seg)){  
-                         LOG.error("服务器目录切换错误:"+seg);  
- returnfalse;  
-                     }  
-                 }  
-             }  
-         }catch(IOException e){  
-             LOG.error("服务器目录切换错误",e);  
- returnfalse;  
-         }  
- return upload(file);  
-     }  
- /**
-      * 上传文件
-      * @param file 上传的文件或文件夹
-      * @return 是否上次成功
-      */
- privateboolean upload(File file) {  
- try{  
- if (file.isDirectory()) {  
-                 ftpClient.makeDirectory(file.getName());  
-                 ftpClient.changeWorkingDirectory(file.getName());  
-                 File[] subFiles = file.listFiles();  
- for (File subFile : subFiles) {  
- if (subFile.isDirectory()) {  
-                         upload(subFile);  
-                         ftpClient.changeToParentDirectory();  
-                     } else {  
- try (FileInputStream input = new FileInputStream(subFile)) {  
-                             ftpClient.storeFile(subFile.getName(), input);  
-                         }  
-                     }  
-                 }  
-             } else {  
- try (FileInputStream input = new FileInputStream(file)) {  
-                     ftpClient.storeFile(file.getName(), input);  
-                 }  
-             }  
-         }catch(IOException e){  
-             LOG.error("上传文件失败",e);  
- returnfalse;  
-         }  
- returntrue;  
-     }  
- }  
```java
/**
 * FTP操作工具
 * @author 杨尚川
 */
@Service
public class FtpUtils {
    protected final APDPlatLogger LOG = new APDPlatLogger(getClass());
    private FTPClient ftpClient;
    /**
     * 连接FTP服务器
     * @param host FTP服务器地址
     * @param port FTP服务器端口号
     * @param username 用户名
     * @param password 密码
     * @return
     */
    public boolean connect(String host, int port, String username, String password) {
        try{
            ftpClient = new FTPClient();
            ftpClient.connect(host, port);
            ftpClient.login(username, password);
            ftpClient.setFileType(FTPClient.BINARY_FILE_TYPE);
            int reply = ftpClient.getReplyCode();
            if (!FTPReply.isPositiveCompletion(reply)) {
                ftpClient.disconnect();
                LOG.error("连接FTP服务器失败，响应码："+reply);
                return false;
            }
        }catch(IOException e){
            LOG.error("连接FTP服务器失败",e);
            return false;
        }
        return true;
    }
    /**
     * 上传文件到服务器上的特定路径
     * @param file 上传的文件或文件夹
     * @param path 上传到ftp服务器哪个路径下
     * @return 
     */
    public boolean uploadTo(File file, String path){
        //切换到服务器上面的合适目录
        //如果对应的目录不存在，则创建
        LOG.info("上传文件 "+file.getAbsolutePath()+" 到服务器路径 "+path);
        try{
            String[] segs = path.split("/");
            for(String seg : segs){
                if(!ftpClient.changeWorkingDirectory(seg)){
                    ftpClient.makeDirectory(seg);
                    if(!ftpClient.changeWorkingDirectory(seg)){
                        LOG.error("服务器目录切换错误:"+seg);
                        return false;
                    }
                }
            }
        }catch(IOException e){
            LOG.error("服务器目录切换错误",e);
            return false;
        }
        return upload(file);
    }
    /**
     * 上传文件
     * @param file 上传的文件或文件夹
     * @return 是否上次成功
     */
    private boolean upload(File file) {
        try{
            if (file.isDirectory()) {
                ftpClient.makeDirectory(file.getName());
                ftpClient.changeWorkingDirectory(file.getName());
                File[] subFiles = file.listFiles();
                for (File subFile : subFiles) {
                    if (subFile.isDirectory()) {
                        upload(subFile);
                        ftpClient.changeToParentDirectory();
                    } else {
                        try (FileInputStream input = new FileInputStream(subFile)) {
                            ftpClient.storeFile(subFile.getName(), input);
                        }
                    }
                }
            } else {
                try (FileInputStream input = new FileInputStream(file)) {
                    ftpClient.storeFile(file.getName(), input);
                }
            }
        }catch(IOException e){
            LOG.error("上传文件失败",e);
            return false;
        }
        return true;
    }
}
```
最后看看解密，StandardPBEStringEncryptor是定义在Spring的配置文件中的，默认加密和解密的密码是config：
Xml代码 ![复制代码](http://yangshangchuan.iteye.com/images/icon_copy.gif)![收藏代码](http://yangshangchuan.iteye.com/images/icon_star.png)![](http://yangshangchuan.iteye.com/images/spinner.gif)
- <beanid="configurationEncryptor"class="org.jasypt.encryption.pbe.StandardPBEStringEncryptor">
- <propertyname="config"ref="environmentVariablesConfiguration"/>
- </bean>
- 
- <beanid="environmentVariablesConfiguration"class="org.jasypt.encryption.pbe.config.EnvironmentStringPBEConfig">
- <propertyname="algorithm"value="PBEWithMD5AndDES"/>
- <propertyname="password"value="config"/>
- </bean>
<bean id="configurationEncryptor" class="org.jasypt.encryption.pbe.StandardPBEStringEncryptor">
	<property name="config" ref="environmentVariablesConfiguration" />
</bean>
<bean id="environmentVariablesConfiguration" class="org.jasypt.encryption.pbe.config.EnvironmentStringPBEConfig">
	<property name="algorithm" value="PBEWithMD5AndDES" />
	<property name="password" value="config" />
</bean>
那么如何对明文的用户名和密码进行加密，然后放入到配置文件中呢？
可以使用APDPlat_Web子项目中的util.ConfigEncryptUtils类来实现：
Java代码 ![复制代码](http://yangshangchuan.iteye.com/images/icon_copy.gif)![收藏代码](http://yangshangchuan.iteye.com/images/icon_star.png)![](http://yangshangchuan.iteye.com/images/spinner.gif)
- /**
-  *把密文放到配置文件中的时候要注意：
-  * ENC(密文)
-  * @author 杨尚川
-  */
- publicclass ConfigEncryptUtils {  
- privatestaticfinal StandardPBEStringEncryptor ENCRYPTOR = new StandardPBEStringEncryptor();  
- static{  
-         EnvironmentStringPBEConfig config = new EnvironmentStringPBEConfig();  
-         config.setAlgorithm("PBEWithMD5AndDES");  
- //自己在用的时候更改此密码
-         config.setPassword("config");          
- 
-         ENCRYPTOR.setConfig(config);  
-     }  
- publicstaticvoid main(String[] args){  
-         String plaintext="test";  
-         String ciphertext=ENCRYPTOR.encrypt(plaintext);  
-         System.out.println(plaintext+" : "+ciphertext);  
-     }  
- }  
```java
/**
 *把密文放到配置文件中的时候要注意：
 * ENC(密文)
 * @author 杨尚川
 */
public class ConfigEncryptUtils {
    private static final StandardPBEStringEncryptor ENCRYPTOR = new StandardPBEStringEncryptor();
    static{
        EnvironmentStringPBEConfig config = new EnvironmentStringPBEConfig();
        config.setAlgorithm("PBEWithMD5AndDES");
        //自己在用的时候更改此密码
        config.setPassword("config");        
        
        ENCRYPTOR.setConfig(config);
    }
    public static void main(String[] args){
        String plaintext="test";
        String ciphertext=ENCRYPTOR.encrypt(plaintext);
        System.out.println(plaintext+" : "+ciphertext);
    }
}
```
在main方法中指定plaintext的值为待加密的字符串，然后运行ConfigEncryptUtils类，控制台输出如下：
Java代码 ![复制代码](http://yangshangchuan.iteye.com/images/icon_copy.gif)![收藏代码](http://yangshangchuan.iteye.com/images/icon_star.png)![](http://yangshangchuan.iteye.com/images/spinner.gif)
- test : pXnEaEXPoseN4lSNTuz3eQ==  
```java
test : pXnEaEXPoseN4lSNTuz3eQ==
```
把密文放到配置文件中的时候要注意，在密文前加入ENC(，在密文后加入)，形如ENC(密文)，如下所示：
Java代码 ![复制代码](http://yangshangchuan.iteye.com/images/icon_copy.gif)![收藏代码](http://yangshangchuan.iteye.com/images/icon_star.png)![](http://yangshangchuan.iteye.com/images/spinner.gif)
- ftp.server.username=ENC(rAjYIMF6ANd2q/cTgX6SpQ==)  
- ftp.server.password=ENC(pXnEaEXPoseN4lSNTuz3eQ==)  
```java
ftp.server.username=ENC(rAjYIMF6ANd2q/cTgX6SpQ==)
ftp.server.password=ENC(pXnEaEXPoseN4lSNTuz3eQ==)
```
2、如何配置？
插件编写完成之后，如何配置使其生效？
在config.local.properties中指定log.backup.file.sender的值为所编写的插件的Spring bean name，可指定多个，用;分割，如下所示：
Java代码 ![复制代码](http://yangshangchuan.iteye.com/images/icon_copy.gif)![收藏代码](http://yangshangchuan.iteye.com/images/icon_star.png)![](http://yangshangchuan.iteye.com/images/spinner.gif)
- log.backup.file.sender=ftpBackupFileSender;  
