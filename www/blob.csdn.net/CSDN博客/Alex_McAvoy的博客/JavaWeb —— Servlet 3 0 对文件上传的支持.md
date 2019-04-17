# JavaWeb —— Servlet 3.0 对文件上传的支持 - Alex_McAvoy的博客 - CSDN博客





2018年12月18日 21:27:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：18








# 【概述】

在 Servlet 3.0 之前，文件上传是一件很麻烦的事，需要借助第三方工具来实现，而在 Servlet 3.0 中，可以借助 @MultipartConfig 注解在实现文件上传，其需要实现以下两项内容：
- 添加 @MultipartConfig 注解
- 从 Request 对象获取 Part 文件对象

# 【相关方法】

@MultipartConfig 注解需要标注在 @WebServlet 之上，其属性有：
- fileSizeThreshold：int 类型，当数据量大于其值时，内容将被写入文件
- location：String 类型，存放生成的文件地址
- maxFileSize：long 类型，允许上传的文件最大值，默认值为 -1，表示无限制
- maxRequestSize：long 类型，针对该 multipart/form-data 请求的最大数量，默认值为 -1，表示无限制

除了要配置 @MultipartConfig 注解外，还需要两个重要的方法：
- getPart(String name)：获取名为 name 的文件，返回类型为 Part 类型
- Collection<Part> getParts()：获取请求中的所有文件

其中，Part 对象处于 javax.servlet.http.Part 接口中，该接口提供了处理文件的简易方法，如：write()、delete() 等

# 【实现】

```java
protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    PrintWriter out=response.getWriter();
    response.setContentType("text/html;charset=UTF-8");//设置Response编码
    String path=this.getServletContext().getRealPath("/");//获取服务器地址
    Part part=request.getPart("file");//获取用户选择的上传文件
    if(part.getContentType().contains("image")) {//仅处理图片文件
        ApplicationPart ap=(ApplicationPart)part;
        String fname1=ap.getSubmittedFileName();//获取上传文件名
        int path_idx=fname1.lastIndexOf("\\")+1;//对文件名进行截取
        String fname2=fname1.substring(path_idx, fname1.length());
        part.write(path+"/upload/"+fname2);//写入Web项目根路径下的upload文件夹中
        out.write("文件写入成功");
    }
    else {
        out.write("请选择图片文件");
    }
}
```





