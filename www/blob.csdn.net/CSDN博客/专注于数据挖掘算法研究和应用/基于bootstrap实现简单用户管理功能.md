# 基于bootstrap实现简单用户管理功能 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年04月14日 15:50:14[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3650








基于bootstrap实现简单用户管理功能，包括增删改以及列表分页展示功能。

1、web.xml配置servlet映射：



```
<servlet>
    <servlet-name>UserMgServlet</servlet-name>
    <servlet-class>com.userMgServlet</servlet-class>
  </servlet>
  <servlet-mapping>
    <servlet-name>UserMgServlet</servlet-name>
    <url-pattern>/ToUserMgServlet</url-pattern>
  </servlet-mapping>
```


2、userMg.jsp页面代码





```
<%@ page language="java" 
    import="java.util.*" 
    import="java.sql.*"
    import="java.io.*"
    contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<script type="text/javascript" src="loginjs/jquery-2.0.0.min.js"></script>
<script type="text/javascript" src="loginjs/jquery-ui"></script>
<link href="loginjs/bootstrap-combined.min.css" rel="stylesheet" media="screen">
<script type="text/javascript" src="loginjs/bootstrap.min.js"></script>
<title>用户管理</title>
</head>
<script language="JavaScript"> 
	function addClick(){ 
		var name = window.document.userMgForm.inputName.value;
		var pwd= window.document.userMgForm.inputPassword.value;
		if(name.length == 0 || pwd.length==0) { 
			alert("新建用户，账户名或密码不能为空！"); 
			return false; 
		} 
		window.document.userMgForm.action="ToUserMgServlet"; 
		window.document.userMgForm.submit(); 
	} 
	function updClick(){ 
		var name = window.document.userMgForm.inputName.value;
		if(name.length == 0) { 
			alert("修改用户，账户名不能为空！"); 
			return false; 
		} 
		window.document.userMgForm.action="ToUserMgServlet"; 
		window.document.userMgForm.submit(); 
	} 
	function delClick(){ 
		var name = window.document.userMgForm.inputName.value;
		if(name.length == 0) { 
			alert("删除用户，账户名不能为空！"); 
			return false; 
		} 
		window.document.userMgForm.action="ToUserMgServlet"; 
		window.document.userMgForm.submit(); 
	}
</script> 
<%
String userMgRet = (String)request.getSession().getAttribute("userMgRet");
request.getSession().removeAttribute("userMgRet"); 
%>
<body>
<div class="container-fluid">
	<div class="row-fluid">
		<div class="span12">
			<form class="form-horizontal" name="userMgForm" method="post">
			
				<div class="control-group">
					<label class="control-label" for="inputName">账户</label>
					<div class="controls">
						<input name="inputName" type="text" />
					</div>
				</div>
				
				<div class="control-group">
					<label class="control-label" for="inputPassword">密码</label>
					<div class="controls">
						<input name="inputPassword" type="password" />
					</div>
				</div>
				
				<div class="control-group">
				    <label class="control-label" for="inputType">账户类型</label>
				    <div class="controls">
					<div class="radio">					
						<input type="radio" name="optType"  value="1" checked>管理员
					</div>
					</div>
					<div class="controls">
					<div class="radio">							
						<input type="radio" name="optType" value="0">普通用户
					</div>		
					</div>			
				</div>
				
				<div class="control-group">
				    <label class="control-label" for="inputStatus">账户类型</label>
				    <div class="controls">
					<div class="form-group">
					    <select class="form-control" name="selectStatus"> 
					    	<option value="1">有效</option> 
					    	<option value="0">无效</option> 
					    </select>
					</div>
					</div>			
				</div>			
								
				<div class="control-group">
					<div class="controls">
						 <button type="submit" class="btn" name="btnPara" onclick="addClick()" value="add">新增</button>
						 <button type="submit" class="btn" name="btnPara" onclick="updClick()" value="upd">修改</button>
						 <button type="submit" class="btn" name="btnPara" onclick="delClick()" value="del">删除</button>
					</div>
				</div>
				
				<%if(userMgRet != null) {%>
					 <div class="control-group">
					 	<label><%=userMgRet%></label>
					 </div>
					 <%} %>
			</form> 
			
			<table class="table table-bordered table-hover table-condensed">
				<thead>
					<tr>
						<th>
							账户
						</th>
						<th>
							类型
						</th>
						<th>
							状态
						</th>
					</tr>
				</thead>
				<%!    
    				public static final int PAGESIZE = 5;  //每页5条件记录
   					int iPageCount=0;  
    				int iCurPage = 1;  
				%> 
			    <%
				try{
					Class.forName("com.mysql.jdbc.Driver");   
			     	Connection con = DriverManager.getConnection("jdbc:mysql://localhost:3306/dbname","root","123");
			     	if(con!=null){
			     		 String strSql="select * from tb_sm_user";
			    		 PreparedStatement stmt = con.prepareStatement(strSql,ResultSet.TYPE_FORWARD_ONLY,ResultSet.CONCUR_READ_ONLY);			    	 	 
			    	 	 ResultSet rs = stmt.executeQuery(); 
			    	 	 rs.last();  
			             int size = rs.getRow();  
			             iPageCount = (size%PAGESIZE==0)?(size/PAGESIZE):(size/PAGESIZE+1);
			             String strCurPage = request.getParameter("curPage");  
			             if(strCurPage==null){  
			            	 strCurPage="1";  
			             }  
			             iCurPage = Integer.parseInt(strCurPage);  
			             if(iCurPage>=iPageCount) iCurPage = iPageCount;  
			             boolean flag = rs.absolute((iCurPage-1)*PAGESIZE+1);  
			             int count = 0;  
			             
			    	 	 int iColor=0;
			    		 while (rs.next()) {
			    			 if(count>=PAGESIZE)break; 
			    			 String strName=rs.getString("name");
			    			 int iType=rs.getInt("type");
			    			 String strType="管理员";
			    			 if(iType==0) strType="普通用户";
			    		 	 int iStatus=rs.getInt("status");
			    		 	 String strStatus="有效";
			    		 	 if(iType==0) strStatus="无效";
			    		 	 count++;
			    		 	 if(iColor==0){
			    		 	 %>
			    		 	 <tbody>
								<tr class="success">
								<td><%=strName%></td>
						        <td><%=strType%></td>
						        <td><%=strStatus%></td>
						        </tr>
						     </tbody>
						     <%iColor=iColor+1;} 
			    		 	 else if(iColor==1){
				    		 	 %>
				    		 	 <tbody>
									<tr class="error">
									<td><%=strName%></td>
							        <td><%=strType%></td>
							        <td><%=strStatus%></td>
							        </tr>
							     </tbody>
							     <%iColor=iColor+1;}
			    		 	 else if(iColor==2){
				    		 	 %>
				    		 	 <tbody>
									<tr class="warning">
									<td><%=strName%></td>
							        <td><%=strType%></td>
							        <td><%=strStatus%></td>
							        </tr>
							     </tbody>
							     <%iColor=iColor+1;}
			    		 	 else if(iColor==3){
				    		 	 %>
				    		 	 <tbody>
									<tr class="info">
									<td><%=strName%></td>
							        <td><%=strType%></td>
							        <td><%=strStatus%></td>
							        </tr>
							     </tbody>
							     <%iColor=0;} %>					 
			    		 	 <%
			    	 	}
			     	}
				} catch (Exception e) { e.printStackTrace();}
			    %>				
			</table>
			
			<div class="pagination pagination-centered">
				<ul>
					<li>
						<a href="userMg.jsp?curPage=<%=iCurPage-1%>">上一页</a>
					</li>
					<li>
						<a href="userMg.jsp?curPage=1">1</a>
					</li>
					<li>
						<a href="userMg.jsp?curPage=2">2</a>
					</li>
					<li>
						<a href="userMg.jsp?curPage=3">3</a>
					</li>
					<li>
						<a href="userMg.jsp?curPage=4">4</a>
					</li>
					<li>
						<a href="userMg.jsp?curPage=5">5</a>
					</li>
					<li>
						<a href="userMg.jsp?curPage=<%=iCurPage+1%>">下一页</a>
					</li>
					<li>
						<a href="userMg.jsp?curPage=<%=iPageCount%>">尾页</a>
					</li>
					<li>
						第<%=iCurPage%>页/共<%=iPageCount%>页  
					</li>
				</ul>
			</div>
		</div>
	</div>
</div>
</body>
</html>
```





3、userMgServlet.java代码



```java
package com;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class userMgServlet
 */
@WebServlet("/userMgServlet")
public class userMgServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
       
    /**
     * @see HttpServlet#HttpServlet()
     */
    public userMgServlet() {
        super();
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub	
		String strBtn = request.getParameter("btnPara");
		String strName = request.getParameter("inputName");	
		String strPwd=request.getParameter("inputPassword");
		String strType=request.getParameter("optType");//1管理员，0普通用户
		String strStatus=request.getParameter("selectStatus");//1有效，0无效
		if(strBtn!=null){
			try {
				Class.forName("com.mysql.jdbc.Driver");   
			    Connection con = DriverManager.getConnection("jdbc:mysql://localhost:3306/dbname","root","123");    
			    if(con!=null){
			    	Statement stmt = con.createStatement(); 
			    	String strSql=null;
			    	if(strBtn.equals("add")){ //新增
			    		strSql="insert into tb_sm_user(name,password,type,status) values('"+strName+"','"+strPwd+"',"+Integer.parseInt(strType)+","+Integer.parseInt(strStatus)+")";			    		
			    	}
			    	else if(strBtn.equals("del")){//删除
			    		strSql="delete from tb_sm_user where name='"+strName+"'";
			    	}
			    	else if(strBtn.equals("upd")){//修改
			    		strSql="update tb_sm_user set type="+Integer.parseInt(strType)+",status="+Integer.parseInt(strStatus)+" where name='"+strName+"'";
			    	}
			    	int iRet=stmt.executeUpdate(strSql);
			    	if(iRet==1) {
			    		request.getSession().setAttribute("userMgRet", "操作成功");
			    		request.getRequestDispatcher("userMg.jsp").forward(request, response);
			    		return;
			    	}
			    }
			} catch (Exception e) { e.printStackTrace();}	
		}		
		request.getSession().setAttribute("userMgRet", "操作失败");
		request.getRequestDispatcher("userMg.jsp").forward(request, response);
		return;
		//response.getWriter().append("Served at: ").append(request.getContextPath());
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		doGet(request, response);
	}

}
```


4、mysql数据库tb_sm_user表定义





```
CREATE TABLE `tb_sm_user` (
`id`bigint not null AUTO_INCREMENT,
`name` varchar(40)not null,
`password` varchar(40) not null,
`type` tinyint DEFAULT1,/*0普通用户，1管理员*/
`status`tinyint DEFAULT 1,/*1有效，0无效*/
PRIMARY KEY (`id`)
) ENGINE=InnoDB
ROW_FORMAT=COMPRESSED
DEFAULT CHARSET=utf8;
```





