# struts2的标签调用 - z69183787的专栏 - CSDN博客
2013年08月03日 10:21:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1833
原文地址：http://124.207.104.55:8924/wiki/index.php?doc-view-2
http://hi.baidu.com/mlbb5220/item/a20c0731e1d3989ab90c03d7
在struts2的2.0.11及以上版本的changelog里，官方已经说了不再支持EL表达式，官方说的是因为安全问题，这个在 JE上已经有所讨论了。
至于struts2的标签还不支持<%%>这种JSP的标准java标签不知道是什么时候开始的,反正例如下面的
<s:url id="selectData1" value="<%=request.getContextPath()%>/test/country.jsp"/>会报错
  而<s:url id="selectData1" value="${pageContext.request.contextPath}/test/country.jsp"/>
  也会报错(EL不支持)
  那么如果我想得到应用的contextPath该怎么办呢，去google搜索，你会发现有人给出的方法
<s:property value="#request.get('javax.servlet.forward.context_path')" />
  或是先用
        <%request.setAttribute('root',request.getContextPath())%>
  再用
      <s:property value="%{#request.contextPath}" />
  怎么感觉这两种都很别扭，后来我试验发现可以直接这么写：
  <s:url id="selectData1" value="%{#request.getContextPath()}/test/country.jsp"/>
  也是正确的.
  ognl不仅支持lambda方式的取值，还支持这种普通的方法式的调用.
## struts2标签获取requestURI、contextPath的值(转)
获取contextPath       <s:property value="#request.get('javax.servlet.forward.context_path')" />         
获取requestURI       <s:property value="#request.get('javax.servlet.forward.request_uri')" />  
