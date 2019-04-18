# springmvc + freemarker 开发环境 缓存设置 - z69183787的专栏 - CSDN博客
2017年01月20日 16:18:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2133
通过阅读源码搞定了
- 先设置springmvc的缓存控制。这是最高优先级的缓存控制。一旦springmvc发现没有缓存开启，就会进行cache，如果发现缓存没开启，则会每次生成一个view（通过调用freemarker的方法来进行）。
- 再设置freemakre的缓存控制。freemakre通过一个时间长度来控制缓存
- 在上面两步完成之后，一般来说就可以了。但是还有一个问题是，web访问的是target目录下的classpath和WEB-INFO目录，而我们开发环境修改的模板文件不是target目录下。这就要求IntelliJ要有热部署功能，也就是在修改文件后，能自动更新target目录下的文件数据。
### springmvc 设置解析器 缓存设置
```
<bean id="viewResolverFtl" class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
    <property name="suffix" value=".ftl"/>
    <property name="order" value="0"/>
    <property name="contentType" value="text/html;charset=UTF-8"/>
    <!-- 此变量值为pageContext.request, 页面使用方法：rc.contextPath -->
    <property name="requestContextAttribute" value="rc"/>
    <property name="cache" value="false"/>
</bean>
```
这里关键是让 cache设置为 false.
### freemarker 缓存设置
```
<!-- 配置FreeMarkerConfigurer -->
<bean id="freemarkerConfig" class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
    <property name="templateLoaderPath" value="/WEB-INF/freemarker/"/>
    <property name="freemarkerSettings">
        <props>
            <prop key="template_update_delay">0</prop>
        </props>
    </property>
</bean>
```
这里关键是设置 template_update_delay 为0 ，0标示不检测模板，每次都重新载入，因此每次在刷新 free marker页面时，均会进行修改时间 的校验，判断是否应该重新载入。
freemarker，每次保存时都会重新启动下面给出了相应的解决办法：
1.FreeMarker 通过将模版文件的内容进行缓存，来降低模版文件读取的频次，降低系统的负载。
2.当处理某个模版时，FreeMarker 直接从缓存中返回对应的 Template 对象，并有一个默认的机制来保证该模版对象是跟模版文件同步的
template_update_delay 用来指定更新模版文件的间隔时间，相当于多长时间检测一下是否有必要重新加载模版文件。
0 表示不检测模板，每次都重新载入，否则为多少毫秒钟检测一下模版是否更改。
3.所以修改freemarker.properties内容为：
template_update_delay=0
即修改ftl，不重启tomcat， 可以看到所修改的内容。
