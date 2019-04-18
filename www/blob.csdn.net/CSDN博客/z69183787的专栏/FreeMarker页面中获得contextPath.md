# FreeMarker页面中获得contextPath - z69183787的专栏 - CSDN博客
2015年07月16日 15:05:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2911
[使用的环境为Spring MVC+FreeMarker，要在ftl页面中使用contextPath，需要在viewResolver中做如下配置(红色部分)：](http://item.taobao.com/item.htm?spm=a1z10.5.w4002-8141011887.38.TJi35O&id=42110237720)
[<bean id="viewResolver" class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
<property name="cache" value="true" />
<property name="prefix" value="" />
<property name="suffix" value=".ftl" />
<property name="exposeSpringMacroHelpers" value="true"/>
<property name="requestContextAttribute" value="rc"></property>
</bean>](#)
[这样，在页面中使用${rc.contextPath} 就可获得contextPath](http://item.taobao.com/item.htm?spm=a1z10.5.w4002-8141011887.38.TJi35O&id=42110237720)
