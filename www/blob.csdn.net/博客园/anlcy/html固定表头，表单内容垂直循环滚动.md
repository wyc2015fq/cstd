
# html固定表头，表单内容垂直循环滚动 - anlcy - 博客园






# [html固定表头，表单内容垂直循环滚动](https://www.cnblogs.com/camilla/p/7553919.html)
<!DOCTYPE html>
<html lang="zh-cn">
<head>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Bootstrap 101 Template</title>
<link rel="stylesheet" href="http://cdn.bootcss.com/bootstrap/3.3.0/css/bootstrap.min.css">
<style type="text/css">
.content {
width: 500px;
margin: 50px 50px;
}
.header {
line-height: 50px;
background-color: \#ECECEC;
}
.data {
height: 300px;
overflow: hidden;
}
</style>
</head>
<body>
<div class="content">
<div class="header row">
<div class="col-md-3">姓名</div>
<div class="col-md-3">性别</div>
<div class="col-md-3">年龄</div>
<div class="col-md-3">职业</div>
</div>
<div class="data">
<div class="data-content">
<table class="table table-hover">
</table>
</div>
<div class="data-footer"></div>
</div></div><script src="http://cdn.bootcss.com/jquery/1.11.1/jquery.min.js">
</script><script src="http://cdn.bootcss.com/bootstrap/3.3.0/js/bootstrap.min.js"></script>
<script type="text/javascript">
var html = '';
for(var i = 0; i < 15; i++) {
html += '<tr>';
html += '<td>张三' + i + '</td>';
if(i % 2 == 0) {
html += '<td>男</td>';
} else {
html += '<td>女</td>';
}
html += '<td>' + (10 + i) + '</td>';
html += '<td>程序员</td>';
html += '</tr>';
}
$('.table').html(html);
$('td').addClass('col-md-3');
var dataDOM = $('.data')[0];
var dataContentDOM = $('.data-content')[0];
var dataFooterDOM = $('.data-footer')[0];
var height = dataDOM.offsetTop + dataDOM.offsetHeight;
setInterval(function() {
if(dataFooterDOM.offsetTop - dataDOM.scrollTop - height <= 0) {
dataDOM.scrollTop -= dataContentDOM.offsetHeight;
} else {
dataDOM.scrollTop++;
}
},
20);
</script>
</body>
</html>





