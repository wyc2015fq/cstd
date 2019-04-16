# jQuery 分页插件 jPages 使用 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月12日 13:58:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35
个人分类：[HTML和Javascript](https://blog.csdn.net/leixiaohua1020/article/category/1678189)










jQuery 分页插件 jPage实现了javascript分页。操作很傻瓜，在此记录一下。

使用方式如下：

1.引入头文件（注意一定要把jQuery放在前面）：

此处使用了动画效果



```
<!-- 使用插件的时候一定要引入jQuery！！！-->
<script type="text/javascript" src="js/jquery-1.8.3.min.js"></script>
<!-- jPage -->
  <link rel="stylesheet" href="css/jpage/jPages.css"/>
  <link rel="stylesheet" href="css/jpage/animate.css"/>
  <script src="js/jpage/jPages.min.js"></script>
   <script>
  $(function(){
    $("div.holder").jPages({
      containerID : "itemContainer",
      previous : "←",
      next : "→",
      perPage : 20,
      delay : 100
    });
  });
</script>
```


2.需要分页的<table>的<tbody>上（注意：不一定非得是<table>，也可以是<ul>等）的id标记为“itemContainer”，表明这里面放的是一行一行的内容。



class属性为“holder”的<div>用于放置“1”“2”“3”或者“Next”这类的导航链接。

注：本代码直接从SSH项目中拷贝出来，因此有EL等标签，可以忽略。



```
<div class="post_section">
  <h3>管理员信息</h3>
	<div class="holder"></div>
	<table id="alltable" cellspacing="0" class="tablesorter">
	<thead>
		<tr>
			<th>ID</th>
			<th>登录名</th>
			<th>姓名</th>
			<th>级别</th>
			<th>操作</th>
		</tr>
		</thead>
		<tbody id="itemContainer">
		<c:forEach items="${alladmin}" var="admin">
		<tr>    
			<td>${admin.id}</td>
			<td>${admin.loginname}</td>
			<td><a href="AdminReadByID_Admin2Action.action?adminid=${admin.id}">${admin.name}</a></td>
			<td>${admin.level}</td>
			<td><a href="AdminReadByID_Admin2Action.action?adminid=${admin.id}">查看</a>|
				<a href="AdminEditRead_Admin2Action.action?adminid=${admin.id}">编辑</a>|
				<a class="confirm" href="AdminDelete_Admin2Action.action?adminid=${admin.id}">删除</a>
			</td>
		</tr>
		</c:forEach>
		</tbody>
	</table> 
</div>
```


 jPages项目主页：[http://luis-almeida.github.io/jPages/index.html](http://luis-almeida.github.io/jPages/index.html)



jPages下载：[http://download.csdn.net/detail/leixiaohua1020/6376509](http://download.csdn.net/detail/leixiaohua1020/6376509)






