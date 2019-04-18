# struts2标签 s:param 传递数组参数 - z69183787的专栏 - CSDN博客
2013年08月07日 15:30:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5140
struts2的s:param标签主要有两个属性name与value，
若想在value属性中输入直接量，则可以这样写：<s:param name="tableTitle" value="%{'user'}"/>，
也可以这样写：<s:param name="tableTitle">user</s:param>，这个表达式tableTitle变量的值为user字符串，而不是user变量；
若想取user对象的值，则可以这样写<s:param name="tableTitle" value="user"/>，
这些用法主要是在<s:component>标签中嵌套使用s:param的时候要注意
**若需要获得contextpath**
**<s:include value="%{#request.getContextPath()}/receive/process/simulate/officer.jsp"><s:param name="steplabel" value="params.processParam['steplabel']"></s:param><s:param name="pname" value="params.processParam['pname']"></s:param><s:param name="pincident" value="params.processParam['pincident']"></s:param></s:include>**
传递数组时：
<s:action name="secretaryRounds" namespace="/approve" executeResult="true">
<s:param name="pname" value="params.processParam['pname']"></s:param>
<s:param name="pincident" value="params.processParam['pincident']"></s:param>
<s:param name="stepname" 
value="new java.lang.String[]{
'秘书',
'批示秘书'
}">
</s:param>
</s:action>
            
