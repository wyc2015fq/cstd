# javaweb基础第十三课：servlet生命周期 - PeterBishop - CSDN博客





2018年12月27日 22:49:07[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：12
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









  这节课我们来讲讲servlet的生命周期，毕竟写了点业务代码，还是要来学点理论的嘛。



  Servlet生命周期指的是servlet对象从被创建到销毁的过程，与生命周期相关的方法有三个:

init() : 初始化方法，当servlet对象被创建后立即调用该方法初始化。

说明: servlet也可以使用构造器来初始化，但一般情况下我们还是通过重写init()方法来进行初始化

service(): 当请求到来时执行的方法(不管是get还是post)

destroy(): servlet对象被销毁前执行的方法



说明: 一般情况下，只有当servlet第一次被分发到请求时才会被创建



LifeCycleTeachServlet.java
package com.servlet;



import javax.servlet.ServletException;

import javax.servlet.http.HttpServlet;

import javax.servlet.http.HttpServletRequest;

import javax.servlet.http.HttpServletResponse;

import java.io.IOException;



//生命周期教学servlet

public class LifeCycleTeachServlet extends HttpServlet {



    @Override

    public void init() throws ServletException {

        System.*out*.println("我正在被初始化");

    }



    @Override

    protected void service(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        System.*out*.println("请求过来啦");

        StringBuffer url = req.getRequestURL();

        System.*out*.println("请求路径是: \"" + url + "\"");

    }



    @Override

    public void destroy() {

        System.*out*.println("我要被销毁啦");

    }

}






启动

浏览器地址栏: [http://localhost:8080/123](http://localhost:8080/123)

浏览器地址栏: [http://localhost:8080/123](http://localhost:8080/123)465

停止

结果:





结论:

初始化方法只会被执行一次

每当被分发到请求，service方法就会被调用一次

销毁方法只有在对象被销毁前调用



