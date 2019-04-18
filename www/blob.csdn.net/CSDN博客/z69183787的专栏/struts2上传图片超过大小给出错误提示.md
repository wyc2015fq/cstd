# struts2上传图片超过大小给出错误提示 - z69183787的专栏 - CSDN博客
2014年06月26日 21:26:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1805
  今天碰到上传图片太大，上传不上去返回input视图的界面，回显的错误信息却是乱码，整了好久才整出来，在这里做个记录，方便自己以后查阅，也希望能给遇到这种情况的朋友点指示，呵呵！
    首先，在struts.xml里新增的配置，
    <!--  指定显示的错误信息的编码格式，注意这个编码应该跟你的回显的页面的格式是一致的  -->
    <constant name="struts.i18n.encoding" value="UTF-8" />
    <!-- 将错误消息转换成中文的文件，也称为国际化.我就直接放置在src目录下所以value不带包名 -->
    <constant name="struts.custom.i18n.resources" value="messages_zh_CN" />
   <!-- 扩大上传文件的大小 ,这里的配置是控制整个表单中上传的文件大小->
    <constant name="struts.multipart.maxSize" value="104557600" />
messages_zh_CN.properties文件的内容如下：
struts.messages.error.content.type.not.allowed=\不\支\持\上\传\该\类\型\的\文\件
struts.messages.error.file.too.large=\上\传\图\片\失\败\：\图\片\太\大
struts.messages.error.uploading=\上\传\文\件\时\发\生\错\误
对应的value和key是:
struts.messages.error.content.type.not.allowed=不支持上传该类型的文件
struts.messages.error.file.too.large=上传图片失败：图片太大
struts.messages.error.uploading=上传文件时发生错误
其次，struts.xml的action里应该配置对上传文件的类型、大小的拦截
  <action name="add"
            class="com.silon.web.struts.action.PicAction" method="add">
            <!-- 拦截图片上传类型 -->
            <interceptor-ref name="fileUpload">
                <param name="allowedTypes">
                    image/bmp,image/png,image/gif,image/jpeg,image/jpg
                    ,image/x-png, image/pjpeg
                </param>
<!--  这里是对单个上传文件大小的限制  -->
                <param name="maximumSize">102400</param>
            </interceptor-ref>
            <interceptor-ref name="defaultStack" />
            <result name="input">/pages/pic/picUpload/addUI.jsp</result>
 </action>
最后，回显页面addUI.jsp里应该有显示错误信息的语句，
    <strong>
               <FONT color="red">
                <s:fielderror />
                 </FONT>
    </strong>
错误均放置在filederror里，如果有的话就可以通过struts标签将其打印出来了
