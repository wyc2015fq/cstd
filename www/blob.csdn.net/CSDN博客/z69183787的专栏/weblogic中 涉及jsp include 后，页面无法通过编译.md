# weblogic中 涉及jsp:include 后，页面无法通过编译 - z69183787的专栏 - CSDN博客
2013年02月20日 17:05:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2777
部署到weblogic后
```
<jsp:include page="../laterdue/laterdue.jsp">
						<jsp:param name="processCode" value="DocRe"/>	
						<jsp:param name="className" value="docReceive"/>	
						<jsp:param name="setMethodName" value="setPersonName"/>	
						<jsp:param name="processName" value='<%=doc.getModelId() %>'/>	
						<jsp:param name="incidents" value='<%=doc.getInstanceId() %>'/>	
						<jsp:param name="stepName" value='<%= request.getAttribute("stepName")%>'/>
						<jsp:param name="taskId" value='<%=taskId%>'/>
						<jsp:param name="taskUserName" value='<%=taskUserName%>'/>
						<jsp:param name="modelName" value='<%=modelName%>'/>
						<jsp:param name="incidentNo" value='<%=incidentNo %>'/>
						<jsp:param name="url" value='<%=request.getContextPath()+"/getDocReceive.action?id=" +doc.getId()%>'/>	
						<jsp:param name="bjSetMethodName" value="setFlag"/>	
						<jsp:param name="blfjSetMethodName" value="setAttach"/>					
				</jsp:include>
```
报错：
Failed to compile JSP /jobContact/newVersion/jobContactOperate.jsp
jobContactOperate.jsp:375:62: This attribute is not recognized.
修改为：
<jsp:include page="../section/section.jsp">
<jsp:param name="name" value='<%= modelName+":"+modelName %>'/>
<jsp:param name="incident" value='<%= incidentNo %>'/>
<jsp:param name="type" value="LD"/>
</jsp:include>
即 value 中的需要计算的值（比如 request.get等） 以单引号 ' 修饰，若为一般字符串，则可用双引号 " 修饰。
