# Struts2文件上传的大小限制问题 - z69183787的专栏 - CSDN博客
2012年12月27日 13:17:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：892
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
问题：上传大文件报错……
解决：修改struts.xml文件中的参数如下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<constant name="struts.multipart.maxSize" value="55000000"/>
<action name="UploadFile" class="com.buptisc.srpms.action.UploadFileAction">
<result name="UploadFileResult">/pages/ShowOtherFiles.jsp</result>
<result name="JGBsuccess">/pages/JGBdetail.jsp</result>
<interceptor-ref name="fileUpload">
<param name="savePath">/data</param>  
<param name="maximumSize">52428800</param> 
</interceptor-ref>
<interceptor-ref name="defaultStack"></interceptor-ref>
</action>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
struts.xml文件中的大小与实际文件大小的关系：1048576（Bytes） = 1024*1024 = 1M实际文件大小.
struts.multipart.maxSize掌控整个项目所上传文件的最大的Size
struts.multipart.maxSize和fileUpload拦截器的maximumSize属性分工不同，总结如下：
1.struts.multipart.maxSize掌控整个项目所上传文件的最大的Size。超过了这个size，后台报错，程序处理不了如此大的文件。fielderror里面会有如下的提示：
the request was rejected because its size (16272982) exceeds the configured maximum (9000000）
2.fileUpload拦截器的maximumSize属性必须小于struts.multipart.maxSize的值。
struts.multipart.maxSize默认2M，当maximumSize大于2M时,必须设置struts.multipart.maxSize的值大于maximumSize。
3.当上传的文件大于struts.multipart.maxSize时，系统报错
当上传的文件在struts.multipart.maxSize和maximumSize之间时，系统提示：
File too large: file "MSF的概念.ppt" "upload__5133e516_129ce85285f__7ffa_00000005.tmp" 6007104 
当上传的文件小于maximumSize，上传成功。
综合来看 还是在 struts.properties中修改struts.multipart.maxSize 。
