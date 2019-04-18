# Struts2标签 数据标签---action标签 - z69183787的专栏 - CSDN博客
2012年10月25日 10:03:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：736
使用action标签,可以允许在jsp页面中直接调用Action,在调用Action时候,可以指定需要被调用的Action的name和namespace.如果指定了executeResult参数的属性值为true,该标签会把Action的处理结果(视图资源)包含到本页面中.
       使用action标签指定属性有:
Java代码 
- id: 可选属性,作为该Action的引用ID  
- 
- name:必选属性,指定调用Action  
- 
- namespace:可选属性,指定该标签调用Action所属namespace  
- 
- executeResult:可选属性,指定是否将Action的处理结果包含到本页面中.默认值为**false**,不包含.  
- 
- ignoreContextParam:可选参数,指定该页面的请求参数是否需要传入调用的Action中,默认值是**false**,即传入参数.  
   使用样式:
   已配置Action,名称为textAction
     该action的执行结果传入本页面中. 
Java代码 " quality="high" allowscriptaccess="always" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer">
- <s:action name="textAction" executeResult="true"/>  
     该action的执行结果传入本页面中. ,但阻止本页面请求参数传入调用action   
Java代码 " quality="high" allowscriptaccess="always" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer">
- <s:action="testAction" executeResult="true" gnoreContextParam="true"/>  
   该action的执行结果禁止传入本页面中. 
Java代码 " quality="high" allowscriptaccess="always" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer">
- <s:action="testAction" />  
