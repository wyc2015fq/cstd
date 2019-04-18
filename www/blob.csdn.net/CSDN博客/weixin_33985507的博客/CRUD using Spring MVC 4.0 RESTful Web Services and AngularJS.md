# CRUD using Spring MVC 4.0 RESTful Web Services and AngularJS - weixin_33985507的博客 - CSDN博客
2014年11月14日 17:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
国内私募机构九鼎控股打造APP，来就送 20元现金领取地址：[http://jdb.jiudingcapital.com/phone.html](http://jdb.jiudingcapital.com/phone.html)
内部邀请码：**C8E245J ****（不写邀请码，没有现金送）**
**国内私募机构九鼎控股打造，九鼎投资是在全国股份转让系统挂牌的公众公司，股票代码为****430719****，为****“****中国****PE****第一股****”****，市值超****1000****亿元。**
------------------------------------------------------------------------------------------------------------------------------------------------------------------
原文: [http://www.tuicool.com/articles/MneeU3f](http://www.tuicool.com/articles/MneeU3f)
![](http://img0.tuicool.com/YNjQBr.png)
Based on the requests from many readers, I am now presenting an article on how to make CRUD operations using Spring MVC 4.0 RESTFul web services and AngularJS. I had already written few articles on Spring MVC 4.0 RESTFul Web Services in case you are new to this.
For the sake of best understanding, I came up with a small task manager AngularJS application powered by Spring MVC 4.0 RESTFul Web Services. With this application, you can list all existing tasks, create a new task, mark completion of an existing task and archive completed tasks. I had tried to keep it very simple as this is not for real time use but for the best understanding of the concept.
DEMO [DOWNLOAD](http://www.dropbox.com/sh/arhvjju8pflnv2n/AAC7apsxfxlduWhcP1kW7SN0a/TaskManagerApp?dl=1)
#### Prerequisites
-- MySql Server
-- Eclipse J2EE
-- Tomcat Server v7.0
1. Let us start by creating task table in MySql Server.
Execute the below commands in MySql Server. This will create a dedicated database for our application and will create a task_list table with dummy values to start with initially,
```
use taskmanager;
create table task_list(task_id int not null auto_increment, task_name varchar(100) not null, task_description text,task_priority varchar(20),task_status varchar(20),task_start_time datetime not null,task_end_time datetime not null,task_archived bool default false,primary key(task_id));
insert into task_list values(1,'Gathering Requirement','Requirement Gathering','MEDIUM','ACTIVE',curtime(),curtime() + INTERVAL 3 HOUR,0);
insert into task_list values(2,'Application Designing','Application Designing','MEDIUM','ACTIVE',curtime(),curtime() + INTERVAL 2 HOUR,0);
insert into task_list values(3,'Implementation','Implementation','MEDIUM','ACTIVE',curtime(),curtime() + INTERVAL 3 HOUR,0);
insert into task_list values(4,'Unit Testing','Unit Testing','LOW','ACTIVE',curtime(),curtime() + INTERVAL 4 HOUR,0);
insert into task_list values(5,'Maintanence','Maintanence','LOW','ACTIVE',curtime(),curtime() + INTERVAL 5 HOUR,0);
select * from task_list;
```
2.  Download
        -- Spring MVC 4.0 jar files from this maven repository  [here](http://maven.springframework.org/release/org/springframework/spring/4.0.0.RELEASE/)
.
        --  Download latest version of jackson json library from  [her](http://repo1.maven.org/maven2/com/fasterxml/jackson/core/)e
             (1) jackson-annotations-x.x.x.jar
             (2) jackson-core-x.x.x.jar
             (3) jackson-databind-x.x.x.jar
        -- Download mysql java connector library.
3. Create a dynamic web project in eclipse and add the above downloaded jar files to your application WEN-INF\lib folder.
4. Now edit web.xml file under WebContent folder to notify the application container about the spring configuration file. Add below code before </web-app>
<servlet>
 <servlet-name>rest</servlet-name>
 <servlet-class>
  org.springframework.web.servlet.DispatcherServlet
 </servlet-class>
 <load-on-startup>1</load-on-startup>
</servlet>
<servlet-mapping>
 <servlet-name>rest</servlet-name>
 <url-pattern>/*</url-pattern>
</servlet-mapping>
Note that in the above code,we have named Spring Dispatcher servlet class as "rest" and the url pattern is given as "/*" which means any uri with the root of this web application will call DispatcherServlet. So what's next? DispatcherServlet will look for configuration files following this naming convention -  [servlet-name]-servlet.xml . In this example, I have named dispatcher servlet class as "rest" and hence it will look for file named 'rest-servlet.xml'.
5. Create a file under WEB-INF folder and name it as "rest-servlet.xml". Add below spring configuration code to it,
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
 xmlns:context="http://www.springframework.org/schema/context"
 xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xmlns:p="http://www.springframework.org/schema/p"
 xsi:schemaLocation="
   http://www.springframework.org/schema/beans     
   http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
   http://www.springframework.org/schema/context
   http://www.springframework.org/schema/context/spring-context-4.0.xsd
   http://www.springframework.org/schema/mvc
   http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">
 <context:component-scan base-package="com.programmingfree.springservice.controller" />
 <mvc:annotation-driven />
 </beans>
I have already explained component-scan element and mvc:annotation-driven element in my previous article, so I am not going to repeat it here again.
6. Create a Java class and name it "Task.java". This is the model class and it represents the fields of a single task in the database.
```java
package com.programmingfree.springservice.domain;
public class Task {
 
 private int task_id;
  private String task_name;
  private String task_description; 
  private String task_priority;
  private String task_status;
  
  public int getTaskId() {
   return task_id;
  }
  public void setTaskId(int taskId) {
   this.task_id = taskId;
  }
  public String getTaskName() {
   return task_name;
  }
  public void setTaskName(String taskName) {
   this.task_name = taskName;
  }
  public String getTaskDescription() {
   return task_description;
  }
  public void setTaskDescription(String taskDescription) {
   this.task_description = taskDescription;
  }
  
  
  public String getTaskPriority() {
   return task_priority;
  }
  public void setTaskPriority(String taskPriority) {
   this.task_priority = taskPriority;
  }
  
  public String getTaskStatus() {
   return task_status;
  }
  
 public void setTaskStatus(String taskStatus) {
   this.task_status = taskStatus;
  }
  
  @Override
  public String toString() {
   return "Task [task_id=" + task_id + ", task_name=" + task_name
     + ", task_description=" + task_description + ", task_priority="
     + task_priority +",task_status="+task_status+ "]";
  }
   
}
```
7. Create a utility class to handle connections to database. The connection string properties are kept in a configuration file called "db.properties" in the src folder.
```java
package com.programmingfree.springservice.utility;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;
public class DBUtility {
 private static Connection connection = null;
  public static Connection getConnection() {
    if (connection != null)
      return connection;
    else {
      try {
       Properties prop = new Properties();
        InputStream inputStream = DBUtility.class.getClassLoader().getResourceAsStream("/config.properties");
        prop.load(inputStream);
        String driver = prop.getProperty("driver");
        String url = prop.getProperty("url");
        String user = prop.getProperty("user");
        String password = prop.getProperty("password");
        Class.forName(driver);
        connection = DriverManager.getConnection(url, user, password);
      } catch (ClassNotFoundException e) {
        e.printStackTrace();
      } catch (SQLException e) {
        e.printStackTrace();
      } catch (FileNotFoundException e) {
        e.printStackTrace();
      } catch (IOException e) {
        e.printStackTrace();
      }
      return connection;
    }
  }
}
```
Properties configuration file should have contents such as this,
driver=com.mysql.jdbc.Driver
url=jdbc:mysql://localhost:3306/databasename
user=username
password=xxxxxx
8. Create a service class that performs data access operations to get data from database,
```java
package com.programmingfree.dao;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import com.programmingfree.springservice.domain.*;
import com.programmingfree.springservice.utility.DBUtility;
public class TaskManagerService {
 
 private Connection connection;
 public TaskManagerService() {
  connection = DBUtility.getConnection();
 }
 public void addTask(Task task) {
  try {
   PreparedStatement preparedStatement = connection
     .prepareStatement("insert into task_list(task_name,task_description,task_priority,task_status,task_archived,task_start_time,task_end_time) values (?, ?, ?,?,?,?,?)");
   System.out.println("Task:"+task.getTaskName());
   preparedStatement.setString(1, task.getTaskName());
   preparedStatement.setString(2, task.getTaskDescription());   
   preparedStatement.setString(3, task.getTaskPriority());
   preparedStatement.setString(4, task.getTaskStatus());
   preparedStatement.setInt(5,0);
   Date dt = new Date();
   SimpleDateFormat sdf = 
        new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
   String currentTime = sdf.format(dt);
   preparedStatement.setString(6,currentTime);
   preparedStatement.setString(7,currentTime);
   preparedStatement.executeUpdate();
  } catch (SQLException e) {
   e.printStackTrace();
  }
 }
 
 public void archiveTask(int taskId) {
  try {
   PreparedStatement preparedStatement = connection
     .prepareStatement("update task_list set task_archived=true where task_id=?");
   // Parameters start with 1
   preparedStatement.setInt(1, taskId);
   preparedStatement.executeUpdate();
  } catch (SQLException e) {
   e.printStackTrace();
  }
 }
 
 public void updateTask(Task task) throws ParseException {
  try {
   PreparedStatement preparedStatement = connection
     .prepareStatement("update task_list set task_name=?, task_description=?, task_priority=?,task_status=?" +
       "where task_id=?");
   preparedStatement.setString(1, task.getTaskName());
   preparedStatement.setString(2, task.getTaskDescription());
   
   preparedStatement.setString(3, task.getTaskPriority());
   preparedStatement.setString(4, task.getTaskStatus());
   preparedStatement.setInt(4, task.getTaskId());
   preparedStatement.executeUpdate();
  } catch (SQLException e) {
   e.printStackTrace();
  }
 }
 
 public void changeTaskStatus(int taskId,String status) throws ParseException {
   try {
    PreparedStatement preparedStatement = connection
      .prepareStatement("update task_list set task_status=? where task_id=?");
    preparedStatement.setString(1,status);
    preparedStatement.setInt(2, taskId);
    preparedStatement.executeUpdate();
   } catch (SQLException e) {
    e.printStackTrace();
   }
  }
 public List<task> getAllTasks() {
  List<task> tasks = new ArrayList<task>();
  try {
   Statement statement = connection.createStatement();
   ResultSet rs = statement.executeQuery("select * from task_list where task_archived=0");
   while (rs.next()) {
    Task task = new Task();
    task.setTaskId(rs.getInt("task_id"));
    task.setTaskName(rs.getString("task_name"));
    task.setTaskDescription(rs.getString("task_description"));    
    task.setTaskPriority(rs.getString("task_priority"));
    task.setTaskStatus(rs.getString("task_status"));
    tasks.add(task);
   }
  } catch (SQLException e) {
   e.printStackTrace();
  }
  return tasks;
 }
 
 public Task getTaskById(int taskId) {
  Task task = new Task();
  try {
   PreparedStatement preparedStatement = connection.
     prepareStatement("select * from task_list where task_id=?");
   preparedStatement.setInt(1, taskId);
   ResultSet rs = preparedStatement.executeQuery();
   
   if (rs.next()) {
     task.setTaskId(rs.getInt("task_id"));
     task.setTaskName(rs.getString("task_name"));
     task.setTaskDescription(rs.getString("task_description"));    
     task.setTaskPriority(rs.getString("task_priority"));
     task.setTaskStatus(rs.getString("task_status"));
   }
  } catch (SQLException e) {
   e.printStackTrace();
  }
  return task;
 }
}
```
9. Create Spring Controller class that maps the incoming request to appropriate methods and returns response in json format. We are going to use @RestController annotation which has @Controller and @Responsebody annotated within itself.
```java
package com.programmingfree.springservice.controller;
import java.text.ParseException;
import java.util.List;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import com.programmingfree.dao.TaskManagerService;
import com.programmingfree.springservice.domain.Task;
@RestController
public class TaskManagerController {
 
 TaskManagerService taskmanagerservice=new TaskManagerService();
 
  @RequestMapping(value="/tasks",method = RequestMethod.GET,headers="Accept=application/json")
  public List<task> getAllTasks() {  
   List<task> tasks=taskmanagerservice.getAllTasks();
   return tasks;
 
  }
  
  
  @RequestMapping(value="/tasks/archive/{taskIds}",method = RequestMethod.POST,headers="Accept=application/json")
  public List<task> archiveAllTasks(@PathVariable int[] taskIds) { 
   for(int i=0;i<taskids .length="" ask="" i="" list="" taskids="" taskmanagerservice.archivetask=""> tasks=taskmanagerservice.getAllTasks();
   return tasks;
 
  }
  
  @RequestMapping(value="/tasks/{taskId}/{taskStatus}",method = RequestMethod.POST,headers="Accept=application/json")
  public List<task> changeTaskStatus(@PathVariable int taskId,@PathVariable String taskStatus) throws ParseException { 
   taskmanagerservice.changeTaskStatus(taskId,taskStatus);   
   return taskmanagerservice.getAllTasks();
   
  }
  
  @RequestMapping(value="/tasks/insert/{taskName}/{taskDesc}/{taskPriority}/{taskStatus}",method = RequestMethod.POST,headers="Accept=application/json")
  public List<task> addTask(@PathVariable String taskName,@PathVariable String taskDesc,@PathVariable String taskPriority,@PathVariable String taskStatus) throws ParseException { 
  Task task = new Task();
  task.setTaskName(taskName);
  task.setTaskDescription(taskDesc);
  task.setTaskPriority(taskPriority);
  task.setTaskStatus(taskStatus);
  taskmanagerservice.addTask(task);
  return taskmanagerservice.getAllTasks();
   
  }        
}
```
Let us now take a closer look into the methods we have in the Spring Controller class.
-- Initially we use getAllTasks () method to fetch all tasks from database. This will fetch all tasks that are not archived (task_archived = '0').
-- Then we give an option to the user via archiveTasks() method, to archive all completed tasks so that it won't show up on the dashboard. For this purpose, we have a field in task_list table called 'task_archived' with values 0 or 1 marking whether a task is archived or not.
-- An option to update the status of a task from 'ACTIVE' to 'COMPLETED' or vice-versa is provided in the changeStatus() method
-- AddTask() method enables one to add a new task to the database.
10. Let us now create the jsp file that sends requests to the Spring controller to fetch data and to update data in the server,
```java
<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html ng-app="taskManagerApp">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>AngularJS Task Manager</title>
<script data-require="angular.js@*" data-semver="1.2.13" src="http://code.angularjs.org/1.2.13/angular.js"></script>
<script type="text/javascript" src="./js/app.js"></script>
</head>
<body>
<div ng-controller="taskManagerController">
 <div id="task-panel" class="fadein fadeout showpanel panel"  ng-show="toggle"> 
  <div class="panel-heading">
   <i class="panel-title-icon fa fa-tasks"></i>
   <span class="panel-title">Recent Tasks</span>
   <div class="panel-heading-controls">
    <button ng-click="toggle = !toggle" >Add New Task</button>
    <button confirmed-click="archiveTasks()" ng-confirm-click="Would you like to archive completed tasks?">Clear completed tasks</button>
   </div>
  </div>
  <div>
   <div ng-repeat="task in tasks">
    <span>
     {{task.taskPriority}}
    </span>
    <div>
     <input id="{{task.taskId}}" type="checkbox" value="{{task.taskId}}" ng-checked="selection.indexOf(task.taskId) > -1" ng-click="toggleSelection(task.taskId)" />
       <label for="{{task.taskId}}"></label>  
    </div>
    <div ng-if="task.taskStatus=='COMPLETED'">    
     <a href="#" class="checkedClass">
      {{task.taskName}}
     <span class="action-status">{{task.taskStatus}}</span>
     </a>
    </div>
    <div ng-if="task.taskStatus=='ACTIVE'">    
     <a href="#" class="uncheckedClass">
      {{task.taskName}}
      <span class="action-status">{{task.taskStatus}}</span>
     </a>
    </div>
   </div>
  </div>
 </div>
 <div id="add-task-panel" ng-hide="toggle">
  <div>
   <span>Add Task</span>
   <div>
    <button ng-click="toggle = !toggle">Show All Tasks</button>
   </div>
  </div>
  <div>
   <div>
    <table>
     <tr>
      <td>Task Name:</td>
      <td><input type="text" ng-model="taskName"/></td>
     </tr>
     <tr>
      <td>Task Description:</td>
      <td><input type="text" ng-model="taskDesc"/></td>
     </tr>
     <tr>
      <td>Task Status:</td>
      <td>
       <
```
