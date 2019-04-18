# springmvc上传临时文件目录配置 - z69183787的专栏 - CSDN博客
2016年08月20日 14:13:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7007

```
<bean id="multipartResolver"
        class="org.springframework.web.multipart.commons.CommonsMultipartResolver">
        <property name="uploadTempDir" value = "/upload"></property>
        <!-- <property name="maxUploadSize" value="10000000" /> -->
    </bean>
```
默认是 javax.servlet.context.tempdir=C:\Users\Administrator\.IntelliJIdea13\system\tomcat\Unnamed_emidas-third-party\work\Catalina\localhost\_
uploadTempDir属性。 
但是这个属性只能帮助我们将上传目录指向tomcat。 
而如果想指向外面，可以利用windows建立文件夹映射，linux一样。
    defaultEncoding：表示用来解析request请求的默认编码格式，当没有指定的时候根据Servlet规范会使用默认值ISO-8859-1。当request自己指明了它的编码格式的时候就会忽略这里指定的defaultEncoding。
    uploadTempDir：设置上传文件时的临时目录，默认是Servlet容器的临时目录。
    maxUploadSize：设置允许上传的最大文件大小，以字节为单位计算。当设为-1时表示无限制，默认是-1。
    maxInMemorySize：设置在文件上传时允许写到内存中的最大值，以字节为单位计算，默认是10240。
