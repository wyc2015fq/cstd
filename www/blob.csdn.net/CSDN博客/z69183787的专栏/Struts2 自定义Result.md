# Struts2 自定义Result - z69183787的专栏 - CSDN博客
2015年05月21日 20:57:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2312

其实，开发自定义Result是非常简单的，只需要实现com.opensymphony.xwork2.Result接口就可以了，这个接口的定义也非常简单，示例如下：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/4056.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/4056.html#)[打印](http://sishuok.com/forum/blogPost/list/4056.html#)
- publicinterface Result extends Serializable {  
- publicvoid execute(ActionInvocation invocation) throws Exception;  
- }  
就是一个execute方法，在这个方法里面去写Result的真正处理，就是如何展示视图。当然，所有需要的数据都可以从ActionInvocation里面获取到。
       好了，下面来写个示例示范一下，如何开发自定义的Result。为了简单，就采用字符界面来展示视图，简单点说，就是把信息在后台输出一下。
1：写MyResult类
示例如下：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/4056.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/4056.html#)[打印](http://sishuok.com/forum/blogPost/list/4056.html#)
- package cn.javass.action.action;  
- 
- import com.opensymphony.xwork2.ActionInvocation;  
- import com.opensymphony.xwork2.Result;  
- 
- publicclass MyResult implements Result{  
- publicvoid execute(ActionInvocation invocation) throws Exception {  
-         System.out.println("要处理的Result字符串是="+invocation.getResultCode());  
-     }  
- }  
只是简单的输出了Action返回的要处理的Result的字符串的值，至于要想获取更多要展示的值，可以通过ActionInvocation去获取ActionContext，在ActionContext里面封装着所有需要的值。由于ActionContext在后面才会详细学到，这里就先不去示例了。
2：在struts.xml中配置使用MyResult
示例如下：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/4056.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/4056.html#)[打印](http://sishuok.com/forum/blogPost/list/4056.html#)
- <package name="helloworld"extends="struts-default">  
-         <result-types>  
-             <result-type name="MyResult"class="cn.javass.action.action.MyResult"default="false"/>  
-         </result-types>  
- 
-         <action name="helloworldAction"class="cn.javass.action.action.HelloWorldAction">  
-             <result name="toWelcome" type="MyResult">/s2impl/welcome.jsp</result>  
-         </action>  
- </package>  
这里面有两个地方要注意：
- 在<action>当前的<package>里面配置了自定义的<result-type>，当然它的配置也很简单，主要就是name和class属性，还有一个default的属性用来表示这个ResultType是否缺省使用的。
- 另外一个地方，在<action>里面配置<result>的时候，要配置type属性，type属性的值就是我们自定义的<result-type>的name属性的值
3：测试一下
其它的没有什么变化，照例从登录页面开始运行，去测试一下，看看效果。后台应该有如下输出信息：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/4056.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/4056.html#)[打印](http://sishuok.com/forum/blogPost/list/4056.html#)
- 用户输入的参数为===account=test,password=test,submitFlag=login  
- 要处理的Result字符串是=toWelcome  
其中加粗的这句话就是由我们自定义的Result输出的。
       注意：由于此时我们的Result并没有向页面输出任何东西，因此，此时的页面应该是一片空白。
       通过这个示例可以看到，自定义Result也不是一件很困难的事情，困难的是到底如何展示数据，也就是必须有一个相应的视图展示技术，Result就相当于是包装这个视图展示技术的。
注意：我只要是解决自定义返回Json 和异常处理问题
新建一个类 AjaxResult   继承 StrutsResultSupport 看看代码吧
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class AjaxResult extends StrutsResultSupport {
    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 1L;
    private static final String AJAX_SUCCESS = "{\"success\":true}";
    private static final String SUCCESS_PERFIX = "{\"success\":true,result:[";
    private static final String FAILURE_PERFIX = "{\"success\":false,result:[],";
    private static final String SUFFIX = "]}";
    private Writer writer;
    private String defaultEncoding = "UTF-8";
    @Inject("struts.i18n.encoding")
    public void setDefaultEncoding(String encoding) {
        this.defaultEncoding = encoding;
    }
    protected void doExecute(String finalLocation, ActionInvocation invocation)
            throws Exception {
        Object action = invocation.getAction();
        String responseData = "";
        if ((action instanceof BaseAction)) {
            BaseAction ajaxAction = (BaseAction) action;
            HttpServletResponse response = ServletActionContext.getResponse();
            String encoding = getEncoding(finalLocation);
            String contentType = getContentType(finalLocation);
            if (encoding != null) {
                contentType = contentType + ";charset=" + encoding;
            }
            response.setContentType(contentType);
            String successData = ajaxAction.getResponseData();
            if (successData != null) {
                if ("success".equals(successData)) {
                    responseData = "{\"success\":true}";
                } else {
                    responseData = successData;
                }
            }
            // if(true){
            // String errorResultLocation = ajaxAction.getErrorResultLocation();
            // String exceptionMessage =
            // invocation.getStack().findString("exception.message");
            // exceptionMessage = exceptionMessage.replaceAll("\r", " ");
            // exceptionMessage = exceptionMessage.replaceAll("\n", " ");
            // exceptionMessage = exceptionMessage.replaceAll("\t", " ");
            // responseData = getFailureData(null, exceptionMessage);
            // }
            getWriter().write(responseData);
        }
    }
    private String getFailureData(String errorResultLocation,
            String exceptionMessage) {
        String errors = "errors:[{msg:\"" + exceptionMessage + "\"}]";
        // if (StringUtils.isNotBlank(errorResultLocation)) {
        // String target = ",\"target\":\"" + errorResultLocation;
        // return "{\"success\":false,result:[]," + errors + target + "\"}";
        // }
        return "{\"success\":false,result:[]," + errors + "}";
    }
    public void setWriter(Writer writer) {
        this.writer = writer;
    }
    protected Writer getWriter() throws IOException {
        if (this.writer != null) {
            return this.writer;
        }
        return ServletActionContext.getResponse().getWriter();
    }
    protected String getContentType(String templateLocation) {
        return "application/json";
    }
    protected String getEncoding(String templateLocation) {
        String encoding = this.defaultEncoding;
        if (encoding == null) {
            encoding = System.getProperty("file.encoding");
        }
        if (encoding == null) {
            encoding = "UTF-8";
        }
        return encoding;
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
接下来，我们需要一个Struts 的配置文件
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<package name="ajax-default" abstract="true" extends="struts-default">
        <result-types>
            <result-type name="ajax"
                class="com.guy.core.common.util.AjaxResult" />
        </result-types>
        <global-results>
            <result name="ajax" type="ajax" />
        </global-results>
        
    </package>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
之后我们新建一个公用类  BaseAction
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class BaseAction extends ActionSupport implements ModelDriven,SessionAware, ParameterAware, ServletRequestAware, ServletResponseAware{
        /**
     * serialVersionUID
     */
    protected final Log logger = LogFactory.getLog(getClass());
    private static final long serialVersionUID = 1L;
    public String SUCCESS="SUCCESS";
    public static final String AJAX = "ajax";
    protected Map session;
    protected Map parameters;
    protected HttpServletRequest servletRequest;
    protected HttpServletResponse servletResponse;
    private String responseData;
    protected void createJSonData(String jsonData) {
        setResponseData(jsonData);
    }
    public String getResponseData() {
        return responseData;
    }
    public void setResponseData(String responseData) {
        this.responseData = responseData;
    }
    public Map getSession() {
        return session;
    }
    public void setSession(Map session) {
        this.session = session;
    }
    public Map getParameters() {
        return parameters;
    }
    public void setParameters(Map parameters) {
        this.parameters = parameters;
    }
    public HttpServletRequest getServletRequest() {
        return servletRequest;
    }
    public void setServletRequest(HttpServletRequest servletRequest) {
        this.servletRequest = servletRequest;
    }
    public HttpServletResponse getServletResponse() {
        return servletResponse;
    }
    public void setServletResponse(HttpServletResponse servletResponse) {
        this.servletResponse = servletResponse;
    }
    @Override
    public Object getModel() {
        return null;
    }
      
    
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
所有的action 都继承BaseAction   ModelDriven 我就不在解释了百度去
例如 
```
public class LoginAction extends BaseAction{
```
```
createJSonData("{\"success\":false,\"msg\":\"密码错误。\"}");
return AJAX;
```
```
```
这样我们的  BaseAction  就完事了，
对象ToString 转成 json 格式了，方便查看
@Override
     public String toString() {
          return ToStringBuilder.reflectionToString(this);
     }
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1  <interceptor-ref name="landingIct">  
 2                     <!-- 包括的方法，也就是拦截器拦截的方法<param name="includeMethods">方法1，方法2</param>      
 3                       
 4                     excludeMethods表示排除指定的方法，即不对标记为excludeMethods的方法进行拦截          
 5                     -->  
 6                     <param name="excludeMethods">landing</param>                  
 7                 </interceptor-ref>                  
 8                 <!-- 默认拦截器栈，如果不写则通过默认拦截器完成的功能将失效。如：国际化等等详细查看struts-default -->  
 9                 <!--  
10                     如果action中没有自定义的拦截器，struts2会为该action添加默认的拦截器，即defaultStack；如果action中用户自己添加了自定义拦截器，将覆盖掉系统的defaultStack，这时候需要我们显式调用该拦截器栈。 
11                  -->  
 
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
抛出异常  处理，在beasAction设置 IsAjaxError  AjaxErrorMessage
给get set 方法，  
新建 AjaxExceptionInterceptor
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 public String intercept(ActionInvocation invocation)
    throws Exception
  {
    String result;
    try
    {
      result = invocation.invoke();
    }
    catch (Exception e) {
      if (this.logEnabled) {
        handleLogging(e);
      }
      List exceptionMappings = invocation.getProxy().getConfig().getExceptionMappings();
      String mappedResult = findResultFromExceptions(exceptionMappings, e);
      if (mappedResult != null) {
        result = mappedResult;
        Object action = invocation.getAction();
        if (action instanceof AjaxProvider) {
          AjaxProvider ajaxAction = (AjaxProvider)action;
          Map results = invocation.getProxy().getConfig().getResults();
          ResultConfig resultConfig = (ResultConfig)results.get(result);
          String location = (String)resultConfig.getParams().get("location");
          ajaxAtion.setIsAjaxError ("true");
          ajaxAction.setAjaxErrorMessage(location);
          result = "ajaxError";
        }
        super.publishException(invocation, new ExceptionHolder(e));
      }
      else {
        throw e;
      }
    }
    return result;
  }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
baseAction  这里判断下是否有异常，有的花转成json输出到页面
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 // if(true){
            // String errorResultLocation = ajaxAction.getErrorResultLocation();
            // String exceptionMessage =
            // invocation.getStack().findString("exception.message");
            // exceptionMessage = exceptionMessage.replaceAll("\r", " ");
            // exceptionMessage = exceptionMessage.replaceAll("\n", " ");
            // exceptionMessage = exceptionMessage.replaceAll("\t", " ");
            // responseData = getFailureData(null, exceptionMessage);
            // }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
