# SpringMVC强大的数据绑定—注解式控制器详解 - z69183787的专栏 - CSDN博客
2016年12月16日 18:24:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：641
#### 6.6.2、@RequestParam绑定单个请求参数值
@RequestParam用于将请求参数区数据映射到功能处理方法的参数上。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String requestparam1(@RequestParam String username)  
请求中包含username参数（如/requestparam1?username=zhang），则自动传入。
此处要特别注意：右击项目，选择“属性”，打开“属性对话框”，选择“Java Compiler”然后再打开的选项卡将“Add variable attributes to generated class files”取消勾选，意思是不将局部变量信息添加到类文件中，如图6-12所示：
![](http://dl.iteye.com/upload/attachment/0075/5174/c39483d4-fa79-318c-af16-c890cde6381c.jpg)
 图6-12
当你在浏览器输入URL，如“requestparam1?username=123”时会报如下错误
Name for argument type [java.lang.String] not available, and parameter name information not found in class file either，表示得不到功能处理方法的参数名，此时我们需要如下方法进行入参：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String requestparam2(@RequestParam("username") String username)  
 即通过@RequestParam("username")明确告诉Spring Web MVC使用username进行入参。
接下来我们看一下@RequestParam注解主要有哪些参数：
value：参数名字，即入参的请求参数名字，如username表示请求的参数区中的名字为username的参数的值将传入；
required：是否必须，默认是true，表示请求中一定要有相应的参数，否则将报404错误码；
defaultValue：默认值，表示如果请求中没有同名参数时的默认值，默认值可以是SpEL表达式，如“#{systemProperties['java.vm.version']}”。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String requestparam4(@RequestParam(value="username",required=false) String username)   
 表示请求中可以没有名字为username的参数，如果没有默认为null，此处需要注意如下几点：
原子类型：必须有值，否则抛出异常，如果允许空值请使用包装类代替。
Boolean包装类型类型：默认Boolean.FALSE，其他引用类型默认为null。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String requestparam5(  
- @RequestParam(value="username", required=true, defaultValue="zhang") String username)   
- 
表示如果请求中没有名字为username的参数，默认值为“zhang”。
如果请求中有多个同名的应该如何接收呢？如给用户授权时，可能授予多个权限，首先看下如下代码：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String requestparam7(@RequestParam(value="role") String roleList)  
如果请求参数类似于url?role=admin&rule=user，则实际roleList参数入参的数据为“admin,user”，即多个数据之间使用“，”分割；我们应该使用如下方式来接收多个请求参数：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String requestparam7(@RequestParam(value="role") String[] roleList)     
或
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String requestparam8(@RequestParam(value="list") List<String> list)      
 到此@RequestParam我们就介绍完了，以上测试代码在cn.javass.chapter6.web.controller. paramtype.RequestParamTypeController中。
#### 6.6.3、@PathVariable绑定URI模板变量值
@PathVariable用于将请求URL中的模板变量映射到功能处理方法的参数上。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @RequestMapping(value="/users/{userId}/topics/{topicId}")  
- public String test(  
- @PathVariable(value="userId") int userId,   
- @PathVariable(value="topicId") int topicId)        
 如请求的URL为“控制器URL/users/123/topics/456”，则自动将URL中模板变量{userId}和{topicId}绑定到通过@PathVariable注解的同名参数上，即入参后userId=123、topicId=456。代码在PathVariableTypeController中。
#### 6.6.4、@CookieValue绑定Cookie数据值
@CookieValue用于将请求的Cookie数据映射到功能处理方法的参数上。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String test(@CookieValue(value="JSESSIONID", defaultValue="") String sessionId)   
如上配置将自动将JSESSIONID值入参到sessionId参数上，defaultValue表示Cookie中没有JSESSIONID时默认为空。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String test2(@CookieValue(value="JSESSIONID", defaultValue="") Cookie sessionId)         
传入参数类型也可以是javax.servlet.http.Cookie类型。
测试代码在CookieValueTypeController中。@CookieValue也拥有和@RequestParam相同的三个参数，含义一样。
#### 6.6.5、@RequestHeader绑定请求头数据
@RequestHeader用于将请求的头信息区数据映射到功能处理方法的参数上。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @RequestMapping(value="/header")  
- public String test(  
- @RequestHeader("User-Agent") String userAgent,  
- @RequestHeader(value="Accept") String[] accepts)  
- 
如上配置将自动将请求头“User-Agent”值入参到userAgent参数上，并将“Accept”请求头值入参到accepts参数上。测试代码在HeaderValueTypeController中。
@RequestHeader也拥有和@RequestParam相同的三个参数，含义一样。
#### 6.6.6、@ModelAttribute绑定请求参数到命令对象
@ModelAttribute一个具有如下三个作用：
①绑定请求参数到命令对象：放在功能处理方法的入参上时，用于将多个请求参数绑定到一个命令对象，从而简化绑定流程，而且自动暴露为模型数据用于视图页面展示时使用；
②暴露表单引用对象为模型数据：放在处理器的一般方法（非功能处理方法）上时，是为表单准备要展示的表单引用对象，如注册时需要选择的所在城市等，而且在执行功能处理方法（@RequestMapping注解的方法）之前，自动添加到模型对象中，用于视图页面展示时使用；
③暴露@RequestMapping方法返回值为模型数据：放在功能处理方法的返回值上时，是暴露功能处理方法的返回值为模型数据，用于视图页面展示时使用。
**一、绑定请求参数到命令对象**
如用户登录，我们需要捕获用户登录的请求参数（用户名、密码）并封装为用户对象，此时我们可以使用@ModelAttribute绑定多个请求参数到我们的命令对象。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String test1(@ModelAttribute("user") UserModel user)  
和6.6.1一节中的五、命令/表单对象功能一样。只是此处多了一个注解@ModelAttribute("user")，它的作用是将该绑定的命令对象以“user”为名称添加到模型对象中供视图页面展示使用。我们此时可以在视图页面使用${user.username}来获取绑定的命令对象的属性。
绑定请求参数到命令对象支持对象图导航式的绑定，如请求参数包含“?username=zhang&password=123&workInfo.city=bj”自动绑定到user中的workInfo属性的city属性中。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @RequestMapping(value="/model2/{username}")  
- public String test2(@ModelAttribute("model") DataBinderTestModel model) {   
DataBinderTestModel相关模型请从第三章拷贝过来，请求参数到命令对象的绑定规则详见【4.16.1、数据绑定】一节，URI模板变量也能自动绑定到命令对象中，当你请求的URL中包含“bool=yes&schooInfo.specialty=computer&hobbyList[0]=program&hobbyList[1]=music&map[key1]=value1&map[key2]=value2&state=blocked”会自动绑定到命令对象上。
当URI模板变量和请求参数同名时，URI模板变量具有高优先权。
**二、暴露表单引用对象为模型数据**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @ModelAttribute("cityList")  
- public List<String> cityList() {  
- return Arrays.asList("北京", "山东");  
- }   
如上代码会在执行功能处理方法之前执行，并将其自动添加到模型对象中，在功能处理方法中调用Model 入参的containsAttribute("cityList")将会返回true。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @ModelAttribute("user")  //①
- public UserModel getUser(@RequestParam(value="username", defaultValue="") String username) {  
- //TODO 去数据库根据用户名查找用户对象
- UserModel user = new UserModel();  
- user.setRealname("zhang");  
- return user;  
- }   
如你要修改用户资料时一般需要根据用户的编号/用户名查找用户来进行编辑，此时可以通过如上代码查找要编辑的用户。
也可以进行一些默认值的处理。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- @RequestMapping(value="/model1") //②
- public String test1(@ModelAttribute("user") UserModel user, Model model)   
此处我们看到①和②有同名的命令对象，那Spring Web MVC内部如何处理的呢：
(1、首先执行@ModelAttribute注解的方法，准备视图展示时所需要的模型数据；@ModelAttribute注解方法形式参数规则和@RequestMapping规则一样，如可以有@RequestParam等；
（2、执行@RequestMapping注解方法，进行模型绑定时首先查找模型数据中是否含有同名对象，如果有直接使用，如果没有通过反射创建一个，因此②处的user将使用①处返回的命令对象。即②处的user等于①处的user。
**三、暴露@RequestMapping方法返回值为模型数据**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public@ModelAttribute("user2") UserModel test3(@ModelAttribute("user2") UserModel user)  
大家可以看到返回值类型是命令对象类型，而且通过@ModelAttribute("user2")注解，此时会暴露返回值到模型数据（名字为user2）中供视图展示使用。那哪个视图应该展示呢？此时Spring Web MVC会根据RequestToViewNameTranslator进行逻辑视图名的翻译，详见【4.15.5、RequestToViewNameTranslator】一节。
此时又有问题了，@RequestMapping注解方法的入参user暴露到模型数据中的名字也是user2，其实我们能猜到：
（3、@ModelAttribute注解的返回值会覆盖@RequestMapping注解方法中的@ModelAttribute注解的同名命令对象。
**四、匿名绑定命令参数**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String test4(@ModelAttribute UserModel user, Model model)  
- 或  
- public String test5(UserModel user, Model model)   
此时我们没有为命令对象提供暴露到模型数据中的名字，此时的名字是什么呢？Spring Web MVC自动将简单类名（首字母小写）作为名字暴露，如“cn.javass.chapter6.model.UserModel”暴露的名字为“userModel”。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public@ModelAttribute List<String> test6()  
- 或  
- public@ModelAttribute List<UserModel> test7()   
对于集合类型（Collection接口的实现者们，包括数组），生成的模型对象属性名为“简单类名（首字母小写）”+“List”，如List<String>生成的模型对象属性名为“stringList”，List<UserModel>生成的模型对象属性名为“userModelList”。
其他情况一律都是使用简单类名（首字母小写）作为模型对象属性名，如Map<String, UserModel>类型的模型对象属性名为“map”。
#### 6.6.7、@SessionAttributes绑定命令对象到session
有时候我们需要在多次请求之间保持数据，一般情况需要我们明确的调用HttpSession的API来存取会话数据，如多步骤提交的表单。Spring Web MVC提供了@SessionAttributes进行请求间透明的存取会话数据。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- //1、在控制器类头上添加@SessionAttributes注解
- @SessionAttributes(value = {"user"})    //①
- publicclass SessionAttributeController   
- 
- //2、@ModelAttribute注解的方法进行表单引用对象的创建
- @ModelAttribute("user")    //②
- public UserModel initUser()   
- 
- //3、@RequestMapping注解方法的@ModelAttribute注解的参数进行命令对象的绑定
- @RequestMapping("/session1")   //③
- public String session1(@ModelAttribute("user") UserModel user)  
- 
- //4、通过SessionStatus的setComplete()方法清除@SessionAttributes指定的会话数据
- @RequestMapping("/session2")   //③
- public String session(@ModelAttribute("user") UserModel user, SessionStatus status) {  
- if(true) { //④
-         status.setComplete();  
-     }  
- return"success";  
- }   
@SessionAttributes(value = {"user"})含义：
@SessionAttributes(value = {"user"}) 标识将模型数据中的名字为“user” 的对象存储到会话中（默认HttpSession），此处value指定将模型数据中的哪些数据（名字进行匹配）存储到会话中，此外还有一个types属性表示模型数据中的哪些类型的对象存储到会话范围内，如果同时指定value和types属性则那些名字和类型都匹配的对象才能存储到会话范围内。
包含@SessionAttributes的执行流程如下所示：
① 首先根据@SessionAttributes注解信息查找会话内的对象放入到模型数据中；
② 执行@ModelAttribute注解的方法：如果模型数据中包含同名的数据，则不执行@ModelAttribute注解方法进行准备表单引用数据，而是使用①步骤中的会话数据；如果模型数据中不包含同名的数据，执行@ModelAttribute注解的方法并将返回值添加到模型数据中；
③ 执行@RequestMapping方法，绑定@ModelAttribute注解的参数：查找模型数据中是否有@ModelAttribute注解的同名对象，如果有直接使用，否则通过反射创建一个；并将请求参数绑定到该命令对象；
此处需要注意：如果使用@SessionAttributes注解控制器类之后，③步骤一定是从模型对象中取得同名的命令对象，如果模型数据中不存在将抛出HttpSessionRequiredException Expected session attribute ‘user’(Spring3.1)
或HttpSessionRequiredException Session attribute ‘user’ required - not found in session(Spring3.0)异常。
④ 如果会话可以销毁了，如多步骤提交表单的最后一步，此时可以调用SessionStatus对象的setComplete()标识当前会话的@SessionAttributes指定的数据可以清理了，此时当@RequestMapping功能处理方法执行完毕会进行清理会话数据。
我们通过Spring Web MVC的源代码验证一下吧，此处我们分析的是Spring3.1的RequestMappingHandlerAdapter，读者可以自行验证Spring3.0的AnnotationMethodHandlerAdapter，流程一样：
（1、RequestMappingHandlerAdapter.invokeHandlerMethod
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- //1、RequestMappingHandlerAdapter首先调用ModelFactory的initModel方法准备模型数据：
- modelFactory.initModel(webRequest, mavContainer, requestMappingMethod);  
- //2、调用@RequestMapping注解的功能处理方法
- requestMappingMethod.invokeAndHandle(webRequest, mavContainer);  
- //3、更新/合并模型数据
- modelFactory.updateModel(webRequest, mavContainer);   
（2、ModelFactory.initModel
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- Map<String, ?> attributesInSession = this.sessionAttributesHandler.retrieveAttributes(request);  
- //1.1、将与@SessionAttributes注解相关的会话对象放入模型数据中
- mavContainer.mergeAttributes(attributesInSession);  
- //1.2、调用@ModelAttribute方法添加表单引用对象
- invokeModelAttributeMethods(request, mavContainer);  
- //1.3、验证模型数据中是否包含@SessionAttributes注解相关的会话对象，不包含抛出异常
- for (String name : findSessionAttributeArguments(handlerMethod)) {  
- if (!mavContainer.containsAttribute(name)) {  
- //1.4、此处防止在@ModelAttribute注解方法又添加了会话对象
- //如在@ModelAttribute注解方法调用session.setAttribute("user", new UserModel());
-         Object value = this.sessionAttributesHandler.retrieveAttribute(request, name);  
- if (value == null) {  
- thrownew HttpSessionRequiredException("Expected session attribute '" + name + "'");  
-         }  
-         mavContainer.addAttribute(name, value);  
- }   
（3、ModelFactory.invokeModelAttributeMethods
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- for (InvocableHandlerMethod attrMethod : this.attributeMethods) {  
-     String modelName = attrMethod.getMethodAnnotation(ModelAttribute.class).value();   
- //1.2.1、如果模型数据中包含同名数据则不再添加
- if (mavContainer.containsAttribute(modelName)) {  
- continue;  
-     }  
- //1.2.2、调用@ModelAttribute注解方法并将返回值添加到模型数据中，此处省略实现代码
- }   
（4、requestMappingMethod.invokeAndHandle 调用功能处理方法，此处省略
（5、ModelFactory.updateMode 更新模型数据
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- //3.1、如果会话被标识为完成，此时从会话中清除@SessionAttributes注解相关的会话对象
- if (mavContainer.getSessionStatus().isComplete()){   
- this.sessionAttributesHandler.cleanupAttributes(request);  
- }  
- //3.2、如果会话没有完成，将模型数据中的@SessionAttributes注解相关的对象添加到会话中
- else {  
- this.sessionAttributesHandler.storeAttributes(request, mavContainer.getModel());  
- }  
- //省略部分代码 
到此@SessionAtrribute介绍完毕，测试代码在cn.javass.chapter6.web.controller.paramtype.SessionAttributeController中。
另外cn.javass.chapter6.web.controller.paramtype.WizardFormController是一个类似于【4.11、AbstractWizardFormController】中介绍的多步骤表单实现，此处不再贴代码，多步骤提交表单需要考虑会话超时问题，这种方式可能对用户不太友好，我们可以采取隐藏表单（即当前步骤将其他步骤的表单隐藏）或表单数据存数据库（每步骤更新下数据库数据）等方案解决。
#### 6.6.8、@Value绑定SpEL表示式
@Value用于将一个SpEL表达式结果映射到到功能处理方法的参数上。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- public String test(@Value("#{systemProperties['java.vm.version']}") String jvmVersion)  
到此数据绑定我们就介绍完了，对于没有介绍的方法参数和注解（包括自定义注解）在后续章节进行介绍。接下来我们学习下数据类型转换吧。
转载请注明出处【[http://jinnianshilongnian.iteye.com/blog/1703694](http://jinnianshilongnian.iteye.com/blog/1703694)】 
