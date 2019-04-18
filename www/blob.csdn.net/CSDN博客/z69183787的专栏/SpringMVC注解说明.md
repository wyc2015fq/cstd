# SpringMVC注解说明 - z69183787的专栏 - CSDN博客
2016年08月08日 15:31:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：565
个人分类：[SpringMvc](https://blog.csdn.net/z69183787/article/category/2176803)
**@controller**
通过@controller标注即可将class定义为一个controller类。
**@RequestMapping**
|**value**|表示需要匹配的url的格式。|
|----|----|
|**method**|表示所需处理请求的http 协议(如get,post,put,delete等)，可选值为RequestMethod这个enum的值。|
|**params**|格式为”paramname=paramvalue” 或 “paramname!=paramvalue”。 表示参数必须等于某值，或者不等于才进入此映射方法。不填写的时候表明不限制|
|**headers**|用来限定对应的reqeust请求的headers中必须包括的内容，例如headers={"Connection=keep-alive"}, 表示请求头中的connection的值必须为keep-alive。|
@RequestParam
|value|对应表单name空间的值|
|----|----|
|required|是否允许为空|
|defaultValue|默认值|
@PathVariable
获得地址栏中传的参数 例如：
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @RequestMapping(value="/{groupId}.do")  
- publicvoid detail(@PathVariablelong groupId){  
-     groupRepository.selectOne(groupId);  
- }  
@RequestBody
在参数之前加入@RequestBody注解。用来将指定的客户端发送过来的请求参数的数据格式转换成java实体
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @RequestMapping(value = "/xxxxx.do")  
- publicvoid create(@RequestBody() String host){  
-     System.out.println("-----------" + host);  
- }  
@RequestHeader
在参数之前加入@RequestHeader注解。用来将指定的请求头信息影射为方法的参数。
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @RequestMapping(value = "/xxxxx.do")  
- publicvoid create(@RequestHeader() MultiValueMap<String, String> host){  
-     System.out.println("-----------" + host);  
- }  
@ResponseBody
如果这个方法定义了@ResponseBody注解。那么会把返回值转换成这个数据格式，输出给客户端
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @RequestMapping(value = "/xxx.do")  
- @ResponseBody
- public MultiValueMap<String, String> create(@RequestHeader() MultiValueMap<String, String> hosts) throws Exception {  
- return hosts;  
- }  
@ResponseStatus
返回一个指定的http response状态码。
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @ResponseStatus(reason="no reason",value=HttpStatus.BAD_REQUEST)  
- @RequestMapping("/responsestatus")  
- publicvoid responseStatusTest(){  
- 
- }  
@SessionAttributes
写在类级别的注解，定义一个session attributes，属性名字为SessionAttributes指定。可以指定多个（数组），也同时可以指定类型。
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @Controller
- @SessionAttributes( { "user" })  
- @RequestMapping("/test")  
- publicclass ControllerTest {  
- @RequestMapping("/session")  
- @ResponseBody
- public String sessionIn(@ModelAttribute("user") User user) {  
- return"index";  
-   }   
- }  
@CookieValue
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @RequestMapping("/cookie")  
- @ResponseBody
- public String cookie(@CookieValue("JSESSIONID") String sessionId) {  
- return sessionId;  
- }  
**@InitBinder **
在controller中注册一个customer protperty editor以解析request中的参数并通过date bind机制与handler method中的参数做绑定。
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @InitBinder
- publicvoid initBinder(WebDataBinder binder) {  
-    SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");  
-    dateFormat.setLenient(false);  
-    binder.registerCustomEditor(Date.class, new CustomDateEditor(  
-           dateFormat, false));  
- }  
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @RequestMapping("/databind1")  
- public ModelAndView databind1(Date date) {  
-    …     
- }  
访问url http://localhost:8080/springmvc/databind1.action?date=2000-01-02
通过initbinder中注册的customeDateEditor类型，自动将2000-01-02转换为日期类型
**@ExceptionHandler**
**[java]**[view
 plain](http://blog.csdn.net/w40338544/article/details/6881464#)[copy](http://blog.csdn.net/w40338544/article/details/6881464#)
[print](http://blog.csdn.net/w40338544/article/details/6881464#)[?](http://blog.csdn.net/w40338544/article/details/6881464#)
- @RequestMapping("/exception")  
- publicvoid ExceptionTest() throws Exception{  
- thrownew Exception("i don't know");  
-  }    
- @ExceptionHandler
- public String handleException(Exception e,HttpServletRequest request){  
-     System.out.println(e.getMessage());  
- return"helloworld";  
-  }  
- 
