# javaweb基础第二十六课：使用commons-fileupload进行文件上传 - PeterBishop - CSDN博客





2019年01月14日 21:04:50[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：27








  这节课我们就来学怎么真正的上传文件

  要想上传文件，我们需要引入新的第三方Jar包,两个:

- commons-fileupload

- commons-io

commons-fileupload依赖于commons-io



jar包一如既往的给大家准备好了，直接复制粘贴到/WEB-INF/lib下即可



OK，关键问题来了，怎么用。百度，准确的说是看官方文档，这节课就来教大家怎么去搜和看官方文档。



[http://commons.apache.org/proper/commons-fileupload/](http://commons.apache.org/proper/commons-fileupload/)

这个就是commons-fileupload的官网，可以看到也是属于Apache的，Apache我们不陌生了，我们用的Tomcat就是它家的



OK，继续看官网

![](https://img-blog.csdnimg.cn/20190114210422390.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



全英文，头疼。但是，我们的谷歌浏览器不是吃素的。

![](https://img-blog.csdnimg.cn/20190114210429299.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)



OK，变成中文了，当然，翻译的质量肯定很差，很多时候需要切回英文去看，再点一下就切回英文了。



OK，我们看什么呢？显然是用户指南。



最简单的使用情况，显然可以直接复制粘贴，OK，我们开始写代码
// Create a factory for disk-based file itemsDiskFileItemFactory factory = new DiskFileItemFactory();// Configure a repository (to ensure a secure temp location is used)ServletContext servletContext = this.getServletConfig().getServletContext();File repository = (File) servletContext.getAttribute("javax.servlet.context.tempdir");factory.setRepository(repository);// Create a new file upload handlerServletFileUpload upload = new ServletFileUpload(factory);// Parse the requestList<FileItem> items = upload.parseRequest(request);



package com.servlet;



import org.apache.commons.fileupload.FileItem;

import org.apache.commons.fileupload.FileUploadException;

import org.apache.commons.fileupload.disk.DiskFileItemFactory;

import org.apache.commons.fileupload.servlet.ServletFileUpload;



import javax.servlet.ServletContext;

import javax.servlet.ServletException;

import javax.servlet.ServletInputStream;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.*;

import java.util.Iterator;

import java.util.List;

import java.util.UUID;



//负责文件上传的Servlet

public class FileUploadServlet extends HttpServlet {



    //文件上传一定是POST方式，所以我们用doPost

    @Override

    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        try {

            //创建一个FileItemFactory,也就是FIleItem工厂

            DiskFileItemFactory factory = new DiskFileItemFactory();



            //设置临时存储区

            ServletContext servletContext = this.getServletConfig().getServletContext();

            File repository = (File) servletContext.getAttribute("javax.servlet.context.tempdir");

            factory.setRepository(repository);



            //new一个ServletFileUpload对象

            ServletFileUpload upload = new ServletFileUpload(factory);



            //解析request对象，获取FileItem集合

            List<FileItem> items = upload.parseRequest(req);



            //处理上传的项目

            //文档里代码的括号是中文的，真是讨厌

            //替换一下，解决

            Iterator<FileItem> iter = items.iterator();

            while(iter.hasNext()){

                FileItem item = iter.next();



                if(item.isFormField()){

                    processFormField(item);

                } else {

                    processUploadedFile(item);

                }

            }

            //OK，文件成功上传，下面开始解释



            */**             * **先来解释ServletContext.getRealPath()方法             * 这个方法就是根据相对路径获取真实路径，相对于谁的路径呢？相对于F:\code\java\JavaWebTeach01\out\artifacts\JavaWebTeach01_war_exploded的路径             * 当然，如果有ContextPath的话，那就是F:\code\java\JavaWebTeach01\out\artifacts\JavaWebTeach01_war_exploded\${ContextPath}             *             * 然后，为什么会是这个路径呢？             * 这个是在ProjectStructure里设置的，Eclipse又不一样，它会编译到Tomcat的/webapps里，也就是下面这条路径             * F:\tomcat8.5\apache-tomcat-8.5.35\webapps             *             * idea的就直接在项目下的out/artifacts/项目名_war_exploded下             *             * web下的东西会全部编译到out/artifacts/项目名_war_exploded下             * src里的源代码会被编译成.class文件存放在/WEB-INF/classes下             *             * 刚刚我新建了一个upload文件夹，然后放了个1.txt，注意，在idea里，web目录下的空文件夹不会被编译到out下，因此必须放个文件             * 还有，我的文件上传是上传到了out下的upload，web下的upload里只会有一个1.txt             * 这就是全部的文件上传             */        *} catch (FileUploadException e) {

            e.printStackTrace();

        }

    }



    //处理上传文件，这里的item保存着文件流

    private void processUploadedFile(FileItem item) throws IOException {

        //处理文件上传

        if(!item.isFormField()){

            //获取文件对应的属性名，就是input标签里的name属性值

            String fieldName = item.getFieldName();



            //获取文件名

            String fileName = item.getName();



            //获取文件类型

            String contentType = item.getContentType();



            //判断是否在内存中

            boolean isInMemory = item.isInMemory();



            //获取文件大小

            long sizeInBytes = item.getSize();



            //OK，之后自己写

            //接下来我们就是读文件内容，然后保存到自己设置的文件夹里

            InputStream inputStream = item.getInputStream();

            byte[] bytes = new byte[(int)sizeInBytes];

            inputStream.read(bytes);

            //这样要上传的文件的数据就都在bytes里了，接下来我们写到指定文件夹里就可以了

            //别忘了close掉inoutStream

            inputStream.close();



            //获取文件后缀名

            String fileSuffixName = fileName.substring(fileName.lastIndexOf("."));



            //然后设置我们想要的文件名即可

            //但有一点，文件名不可以重复，因此我们还需要一个随机文件名生成器

            //java自带这玩意，类名叫UUID

            String newFileName = UUID.*randomUUID*().toString() + fileSuffixName;

            System.*out*.println("新文件名: " + newFileName);



            String path = this.getServletContext().getRealPath("/upload");



            //File.separator就是文件分隔符，可以根据不同平台显示不同分隔符，windows就是\，Linux就是/

            newFileName = path + File.*separator *+ newFileName;

            System.*out*.println("新文件的完整路径: \n" + newFileName);

            File file = new File(newFileName);

            if (!file.exists()) {

                file.createNewFile();

                OutputStream outputStream = new FileOutputStream(file);

                outputStream.write(bytes);



                //别忘了close掉OutputStream

                outputStream.close();

            }



        }

    }



    //处理form表单数据，这类数据是字符串

    private void processFormField(FileItem item) {

        //处理常规表单字段

        if(item.isFormField()){

            String name = item.getFieldName();

            String value = item.getString();

            System.*out*.println("name: " + name);

            System.*out*.println("value: " + value);

        }

    }

}




