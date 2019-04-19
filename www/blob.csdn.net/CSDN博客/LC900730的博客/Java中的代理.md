# Java中的代理 - LC900730的博客 - CSDN博客
2017年08月01日 11:44:49[lc900730](https://me.csdn.net/LC900730)阅读数：120标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## 对Request对象代理
“`JAVA 
public void doFilter( 
    ServletRequest req,  
    ServletResponse res,  
    FilterChain chain) throws IOException, ServletException{ 
        // TODO Auto-generated method stub 
        // place your code here 
        // pass the request along the filter chain 
        //转型 
HttpServletRequest request=(HttpServletRequest) req; 
HttpServletResponse response=(HttpServletResponse) res; 
        //一、处理公用业务 
    request.setCharacterEncoding(“UTF-8”); 
    response.setContentType(“text/html;charset=UTF-8”);
```
HttpServletRequest proxy=(HttpServletRequest)Proxy.newProxyInstance(request.getClass().getClassLoader(),
        new Class[]{HttpServletRequest.class},
        new InvocationHandler() {
        @Override
        public Object invoke(Object proxy, Method method, Object[] args){
        // TODO Auto-generated method stub
        //定义方法返回值
        Object returnValue=null;
        //获取方法名字
        String methodName=method.getName();
        //判断：对getParameter方法进行GET提交中文处理         if("getParameter".equals(methodName)){
        //获取请求数据的值
        //<input type="text" name="userName">
```
String value=request.getParameter(args[0].toString()); 
            //获取提交方式 
            //直接调用目标对象的方法 
String methodSubmit=request.getMethod();
```
//判断如果是GET提交，则进行代理              if("GET".equals(methodSubmit)){
if(value!=null && !"".equals(value.trim())){
```
value=new String(value.getBytes(“ISO8859-1”),”UTF-8”); 
    } 
} 
        return value; 
    }else { 
        //如果指向httpSEervletRequest的其他方法       returnValue=method.invoke(request, args); 
                        } 
            return returnValue; 
                } 
                }); 
        //二、 
        chain.doFilter(proxy, response);
```
}
```
“`![这里写图片描述](https://img-blog.csdn.net/20170801112736929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果要获取上传文件袋数据，需要对上面数据进行解析 
通常对于文件文件上传，调用组件FileUpload组件!
### FileUpload组件
是apache开源项目 
使用： 
    1.下载组件，引入jar包； 
    2.配置prop/xml; 
    3.Api学习
