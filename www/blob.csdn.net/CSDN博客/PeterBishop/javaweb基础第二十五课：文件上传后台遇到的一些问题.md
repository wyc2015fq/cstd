# javaweb基础第二十五课：文件上传后台遇到的一些问题 - PeterBishop - CSDN博客





2019年01月13日 21:39:18[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：48
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









这课我们讲讲文件上传后端遇到的问题


package com.servlet;



import javax.servlet.ServletException;

import javax.servlet.ServletInputStream;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.BufferedReader;

import java.io.IOException;

import java.io.InputStream;

import java.io.InputStreamReader;



//负责文件上传的Servlet

public class FileUploadServlet extends HttpServlet {



    //文件上传一定是POST方式，所以我们用doPost

    @Override

    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        //       <input type="file" name="file">

        //OK，我们的名字没有写错

        //ContextPath被我改回了/

        //又忘了配web.xml了

        String file = req.getParameter("file");

        System.*out*.println(file);   //null

        //这说明我们没有办法通过request.getParameter()来获取

        //转念一想，我们传的是文件，或者说是一个流，那我们可不可以通过流来获取呢？

        InputStream inputStream = req.getInputStream();

        BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));



        String str;

        while ((str = reader.readLine()) != null)

        {

            System.*out*.println(str);

        }

        /*

        ------WebKitFormBoundaryJIxMvvlIiZee3v2D

        Content-Disposition: form-data; name="file"; filename="file.txt"

        Content-Type: text/plain



        这个文件将要被上传

        ------WebKitFormBoundaryJIxMvvlIiZee3v2D--



         */

        //成功读到了文件内容，但还附带其他信息，显然也不是我们想要的

        //以上就是我们在文件上传后端部分遇到的问题，下节课讲解决办法

    }

}




