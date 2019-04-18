# spring mvc 接受多对象的处理 - z69183787的专栏 - CSDN博客
2016年07月11日 18:08:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1411
spring mvc感觉很好用，尤其是对接收对象参数的自动绑定非常简便，但对于同时传多个对象时有些困扰。同时项目并没有直接使用spring的formtag。从网上学来的多对象传值，自己优化了下，原文找不到出处了这里记录下。
首先声明一个注解类，用于对传值对象的声明
Java代码  ![收藏代码](http://lee1177.iteye.com/images/icon_star.png)
- /**
-  * 处理spring mvc 对象绑定注解
-  * @author lee
-  *
-  */
- @Target(ElementType.PARAMETER)  
- @Retention(RetentionPolicy.RUNTIME)  
- @Documented
- public@interface RequestBean {  
-     String value() default"_def_param_name";  
- }  
 然后是一个对WebArgumentResolver的实现类，对象参数绑定主要是这个类来处理
Java代码  ![收藏代码](http://lee1177.iteye.com/images/icon_star.png)
- /**
-  * 对象传值的参数绑定处理
-  * @author lee
-  *
-  */
- publicclass BeanArgumentResolver implements WebArgumentResolver {  
- 
- @SuppressWarnings("rawtypes")  
- public Object resolveArgument(MethodParameter param, NativeWebRequest request){  
-         RequestBean requestBean = param.getParameterAnnotation(RequestBean.class);  
- try{  
- if (requestBean != null) {  
-                 String _param = requestBean.value();  
- if (_param.equals("_def_param_name")) {  
-                     _param = param.getParameterName();  
-                 }  
-                 Class clazz = param.getParameterType();  
-                 Object object = clazz.newInstance();  
-                 HashMap<String, String[]> paramsMap = new HashMap<String, String[]>();  
-                 Iterator<String> itor = request.getParameterNames();  
- while (itor.hasNext()) {  
-                     String webParam = (String) itor.next();  
-                     String[] webValue = request.getParameterValues(webParam);  
-                     List<String> webValueList = new ArrayList<String>();  
- for(int i = 0;i<webValue.length;i++){  
- if(webValue[i]!=null&&!"".equals(webValue[i])){  
-                             webValueList.add(webValue[i]);  
-                         }  
-                     }  
- if (webParam.startsWith(_param)&&!webValueList.isEmpty()) {  
-                         paramsMap.put(webParam, webValueList.toArray(new String[webValueList.size()]));  
-                     }  
-                 }  
-                 BeanWrapper obj = new BeanWrapperImpl(object);  
-                 obj.registerCustomEditor(Date.class, null, new CustomDateEditor(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss"), true));  
-                 System.out.println(obj.findCustomEditor(Date.class, null).getAsText());  
- 
- for (String propName : paramsMap.keySet()) {  
-                     Object propVals = paramsMap.get(propName);  
-                     String[] props = propName.split("\\.");  
- if (props.length == 2) {  
-                         obj.setPropertyValue(props[1], propVals);  
-                     } elseif (props.length == 3) {  
-                         Object tmpObj = obj.getPropertyValue(props[1]);  
- if (tmpObj == null)  
-                             obj.setPropertyValue(props[1], obj.getPropertyType(props[1]).newInstance());  
-                         obj.setPropertyValue(props[1] + "." + props[2], propVals);  
-                     }  
- 
-                 }  
- return object;  
-             }  
-         }catch(Exception e){  
-             e.printStackTrace();  
-         }  
- return WebArgumentResolver.UNRESOLVED;  
-     }  
- }  
 两个类写好后对mvc配置文件进行配置
Xml代码  ![收藏代码](http://lee1177.iteye.com/images/icon_star.png)
- <mvc:annotation-driven>
- <mvc:argument-resolvers>
- <beanclass="xx.xx.xx.xx.BeanArgumentResolver"/>
- </mvc:argument-resolvers>
- <mvc:message-convertersregister-defaults="true">
- <!-- 将StringHttpMessageConverter的默认编码设为UTF-8 -->
- <beanclass="org.springframework.http.converter.StringHttpMessageConverter">
- <constructor-argvalue="UTF-8"/>
- </bean>
- <!-- 将Jackson2HttpMessageConverter的默认格式化输出设为true -->
- <beanclass="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
- <propertyname="prettyPrint"value="true"/>
- </bean>
- </mvc:message-converters>
- </mvc:annotation-driven>
 接下来就是使用了mvc的controller方法如下
Java代码  ![收藏代码](http://lee1177.iteye.com/images/icon_star.png)
- @RequestMapping(value="/saveEvent")  
- @ResponseBody
- public AjaxResult saveEvent(@RequestBean Event event){  
-     event =eventService.saveTemporaryEvent(event);  
- return AjaxResult.objectResult(event);  
- }  
 页面form表单代码
Html代码  ![收藏代码](http://lee1177.iteye.com/images/icon_star.png)
- <tdalign="right">客户代码：</td>
- <td><inputtype="hidden"name="event.submitUser.userId"value="${event.submitUser.userId }"/></td>
- <tdalign="right">客户电话：</td>
- <td><inputclass="inpname_"type="text"id="submitTel"name="event.submitTel"value="${event.submitTel }"/></td>
-------------------------------------------------------------分割线----------------------------------------------------------------
经过一段时间的使用，发现此方法对于传值有很多限制，复杂情况处理很不理想。特意补充官方推荐用法。
controller端直接使用注解声明
Java代码  ![收藏代码](http://lee1177.iteye.com/images/icon_star.png)
- @Controller
- @RequestMapping("/demo/formbean")  
- publicclass FormBeanController {  
- @Autowired
- private FormBeanService formBeanService;  
- 
- @InitBinder("formBean1")  
- publicvoid initBinderFormBean1(WebDataBinder binder) {    
-         binder.setFieldDefaultPrefix("formBean1.");    
-     }  
- @InitBinder("formBean2")  
- publicvoid initBinderFormBean2(WebDataBinder binder) {    
-         binder.setFieldDefaultPrefix("formBean2.");    
-     }  
- @RequestMapping("/save12")  
- @ResponseBody
- public ModelAndView save12(FormBean1 formBean1, FormBean2 formBean2){  
-         formBean2 = formBeanService.saveFromBean12(formBean1, formBean2);  
-         ModelAndView mav = new ModelAndView();  
-         mav.setViewName("redirect:/demo/formbean/edit12/"+formBean2.getId());  
- return mav;  
-     }  
- 
- }  
 html方式相差不大
Html代码  ![收藏代码](http://lee1177.iteye.com/images/icon_star.png)
- <formclass="form-horizontal"id="formBean1Form"method="post">
- <divclass="control-group">
- <labelclass="control-label"for="code">FB1编号</label>
- <divclass="controls">
- <inputtype="hidden"id="formBean1.id"name="formBean1.id"value="${formBean2.formBean1.id }"/>
- <inputtype="text"id="formBean1.code"name="formBean1.code"value="${formBean2.formBean1.code }"/>
- </div>
- </div>
- <divclass="control-group">
- <labelclass="control-label"for="name">FB1名称</label>
- <divclass="controls">
- <inputtype="text"id="formBean1.name"name="formBean1.name"value="${formBean2.formBean1.name }">
- </div>
- </div>
- <divclass="control-group">
- <labelclass="control-label"for="name">FB2编号</label>
- <divclass="controls">
- <inputtype="hidden"id="formBean2.id"name="formBean2.id"value="${formBean2.id }"/>
- <inputtype="text"id="formBean2.code"name="formBean2.code"value="${formBean2.code }">
- </div>
- </div>
- <divclass="control-group">
- <labelclass="control-label"for="name">FB2名称</label>
- <divclass="controls">
- <inputtype="text"id="formBean2.name"name="formBean2.name"value="${formBean2.name }">
- </div>
- </div>
- <divclass="control-group">
- <divclass="controls">
- <buttontype="submit"class="btn"onclick="this.form.action='${ctx}/demo/formbean/save12'">提交</button>
- </div>
- </div>
- </form>
 官方方法是用性更强，对于复杂情况如集合多级嵌套对象等处理完美，计较推荐。
