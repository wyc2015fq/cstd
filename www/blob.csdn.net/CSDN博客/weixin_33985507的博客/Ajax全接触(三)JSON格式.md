# Ajax全接触(三)JSON格式 - weixin_33985507的博客 - CSDN博客
2016年12月14日 00:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
JSON是存储和交换文本信息的语法，类似与XML。它采用键值的方式来组织，易于人们阅读和编写，同时也易于解析和生成
#### json基本概念
- 
JSON:JavaScript对象表示法，(JavaScript Object Notation)
- 
JSON是存储和交换文本信息的语法，类似与XML。它采用键值的方式来组织，易于人们阅读和编写，同时也易于解析和生成
- 
JSON是独立于语言的，也就是说不管什么语言，都可以解析JSON，只需要按照JSON的规则来就行
#### json与XML比较
- 
json的长度和xml格式比起来很短小
- 
json读写的速度更快
- 
json可以使用javascript内建方法直接进行解析，转成javascript对象，非常方便。
#### JSON语法规则
- 
**JSON 数据的书写格式是：名称/值对。**
名称/值对组合中的名称写在前面(在双引号中)，值对写在后面(同样在双引号中)，中间用冒号隔开：比如`"name":"郭靖"`
- 
**json的值可以是下面这些类型：**
数字(整数或浮点数)，比如123，1.23
字符串(在双引号中)
逻辑值(true或false)
数组(在方括号中)
对象(在花括号中)
null
```
{
    "staff":[
        {"name":"洪七","age":"70"},
        {"name":"郭靖","age":"35"},
        {"name":"黄蓉","age":"30"},
    ]
}
```
这是一个典型的json。他首先用花括号括起来，表示这是一个json对象，对象里面有一个值对，这个值对名称叫staff，他的值是一个数组，这个数组是由很多晓得json对象组成，对象里面又有两个属性。
#### json解析、格式化和校验工具
**json解析javascript**
> 
在js中解析json有两种方式eval和JSON.parse
*eval方法*
```
var jsonobj = '{"staff":[{"name":"洪七","age":"70"},{"name":"郭靖","age":"35"},{"name":"黄蓉","age":"30"},]}'
var jsonop = eval('(' + jsonobj + ')');
alert(jsonop.staff[0].name);
```
![3877962-483a285fec5215a1.png](https://upload-images.jianshu.io/upload_images/3877962-483a285fec5215a1.png)
Paste_Image.png
*JSON.parse方法*
```
var jsonobj = '{"staff":[{"name":"洪七","age":"70"},{"name":"郭靖","age":"35"},{"name":"黄蓉","age":"30"}]}'
var jsonop = JSON.parse(jsonobj);
alert(jsonop.staff[0].name);
```
![3877962-ed7a6331fe80e4a5.png](https://upload-images.jianshu.io/upload_images/3877962-ed7a6331fe80e4a5.png)
Paste_Image.png
注意：用eval方法执行他不会去看你的json字符串是否合法，而且json中一些js方法会直接的执行，这是非常的危险。 所以尽量使用JSON.parse方法，来去解析json字符串，能返回一些语法错误，又安全又方便。
json在书写的时候非常容易出错，所以把json字符串进行校验是必不可少的。
推荐json校验在线工具[jsonlint.com](https://link.jianshu.com?t=http://jsonlint.com/)
#### AJAX使用json方式
```
<?php
//设置页面内容是html编码格式是utf-8
header("Content-Type: text/plain;charset=utf-8"); 
//header("Content-Type: application/json;charset=utf-8"); 
//header("Content-Type: text/xml;charset=utf-8"); 
//header("Content-Type: text/html;charset=utf-8"); 
//header("Content-Type: application/javascript;charset=utf-8");
```
服务器端返回json对象
```
//通过员工编号搜索员工
function search(){
    //检查是否有员工编号的参数
    //isset检测变量是否设置；empty判断值为否为空
    //超全局变量 $_GET 和 $_POST 用于收集表单数据
    if (!isset($_GET["number"]) || empty($_GET["number"])) {
        echo '{"success":false,"msg":"参数错误"}';
        return;
    }
    //函数之外声明的变量拥有 Global 作用域，只能在函数以外进行访问。
    //global 关键词用于访问函数内的全局变量
    global $staff;
    //获取number参数
    $number = $_GET["number"];
    $result = '{"success":false,"msg":"没有找到员工。"}';
    
    //遍历$staff多维数组，查找key值为number的员工是否存在，如果存在，则修改返回结果
    foreach ($staff as $value) {
        if ($value["number"] == $number) {
            $result = '{"success":true,"msg":"找到员工：员工编号：' . $value["number"] . 
                            '，员工姓名：' . $value["name"] . 
                            '，员工性别：' . $value["sex"] . 
                            '，员工职位：' . $value["job"] . '"}';
            break;
        }
    }
    echo $result;
}
```
返回json格式信息
查询错误是返回`{"success":false,"msg":"参数错误"}`json格式对象
没有找到员工时返回`{"success":false,"msg":"没有找到员工。"}`json格式对象
```
//创建员工
function create(){
    //判断信息是否填写完全
    if (!isset($_POST["name"]) || empty($_POST["name"])
        || !isset($_POST["number"]) || empty($_POST["number"])
        || !isset($_POST["sex"]) || empty($_POST["sex"])
        || !isset($_POST["job"]) || empty($_POST["job"])) {
        echo '{"success":false,"msg":"参数错误，员工信息填写不全"}';
        return;
    }
    //TODO: 获取POST表单数据并保存到数据库
    
    //提示保存成功
    echo '{"success":true,"msg":"员工：' . $_POST["name"] . ' 信息保存成功！"}';
}
```
新建员工时错误时也返回json对象
客户端修改
客户端要把服务端传来的json格式对象进行解析
查询修改
```
document.getElementById("search").onclick = function() { 
    var request = new XMLHttpRequest();
    request.open("GET", "serverjson.php?number=" + document.getElementById("keyword").value);
    request.send();
    request.onreadystatechange = function() {
        if (request.readyState===4) {
            if (request.status===200) { 
                var data = JSON.parse(request.responseText);
                if (data.success) { 
                    document.getElementById("searchResult").innerHTML = data.msg;
                } else {
                    document.getElementById("searchResult").innerHTML = "出现错误：" + data.msg;
                }
            } else {
                alert("发生错误：" + request.status);
            }
        } 
    }
}
```
`var data = JSON.parse(request.responseText);`定义变量data使用JSON.parse解析获取的json数据。再进行判断显示在页面
```
document.getElementById("save").onclick = function() { 
    var request = new XMLHttpRequest();
    request.open("POST", "serverjson.php");
    var data = "name=" + document.getElementById("staffName").value 
                      + "&number=" + document.getElementById("staffNumber").value 
                      + "&sex=" + document.getElementById("staffSex").value 
                      + "&job=" + document.getElementById("staffJob").value;
    request.setRequestHeader("Content-type","application/x-www-form-urlencoded");
    request.send(data);
    request.onreadystatechange = function() {
        if (request.readyState===4) {
            if (request.status===200) { 
                var data = JSON.parse(request.responseText);
                if (data.success) { 
                    document.getElementById("createResult").innerHTML = data.msg;
                } else {
                    document.getElementById("createResult").innerHTML = "出现错误：" + data.msg;
                }
            } else {
                alert("发生错误：" + request.status);
            }
        } 
    }
}
```
> 
wei
