# jquery实现记住用户名和密码 - _天枢 - 博客园
## [jquery实现记住用户名和密码](https://www.cnblogs.com/yhleng/p/8582912.html)
2018-03-16 16:02 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8582912)
**这里我们选择的方法是cookie的方式去记录**
**首先我们写将用户名和密码写到cookie的js代码**
```
//保存到cookie
        function save_cookies(){
            if($("#remember").prop("checked")){
                var username = $("#username").val();
                var password = $("#password").val();
                $.cookie("remember","true",{expires:7});
                $.cookie("username",username,{expires:7 });
                $.cookie("password",password,{expires:7 });
            }else{
                $.cookie("remember","false",{expires:-1});
                $.cookie("username","",{ expires:-1 });
                $.cookie("password","",{ expires:-1 });
            }
        };
```
**注意：$("#id").prop("checked")    通过这个来获取复选框 勾选状态   jquery1.6版本之后用prop方法，之前版本用attr方法。**
**千万注意prop不要写错，我就把prop写成了porp找了好久的问题，原来是自己把自己坑了。**
1.$.cookie("username") 读取cookie 名为username的值
2.$cookie("username","qcbin",{expires:7})  创建cookie 名为username值为qcbin,保存期限为7天。   如果将{expires:7}替换为365意思是，浏览器关闭即清除
**写加载cookie的代码，这部分js代码，我们放在head头中，这样我们加载登录页的时候就会先去读cookie。**
```
<script>
        $(document).ready(function(){
            var rem = $.cookie('remember');
            if(rem){
                $("#remember").prop("checked",true);
                $("#username").val($.cookie("username"));
                $("#password").val($.cookie("password"));
            }
        });
    </script>
```
**注意：$(document).ready() 这个方法，浏览器就绪，就开妈执行这个下边的代码。**
**登录的html示例代码：**
```
<!DOCTYPE html>
<html>
<head lang="en">
   <meta charset="UTF-8">
    <title>login-AM</title>
    <script src="/static/js/jquery-3.2.1.min.js"></script>
    <link href="/static/css/bootstrap3.3.7.min.css" rel="stylesheet">
    <link href="https://cdn.bootcss.com/font-awesome/4.7.0/css/font-awesome.min.css" rel="stylesheet">
    <script src="/static/js/bootstrap3.3.7.min.js"></script>
    <script src="/static/js/jquery.cookie.js"></script>
    <style>
        .form{background: rgba(255,255,255,0.2);width:400px;margin:120px auto;}
        /*阴影*/
        .fa{display: inline-block;top: 27px;left: 6px;position: relative;color: #ccc;}
        input[type="text"],input[type="password"]{padding-left:26px;}
        .checkbox{padding-left:21px;}
    </style>
    <script>
        $(document).ready(function(){
            var rem = $.cookie('remember');
            if(rem){
                $("#remember").prop("checked",true);
                $("#username").val($.cookie("username"));
                $("#password").val($.cookie("password"));
            }
        });
    </script>
</head>
<body>
    <div class="container">
        <div class="form row">
            <div class="form-horizontal col-md-offset-3" id="login_form">
                <h3 class="form-title">LOGIN</h3>
                <div class="col-md-9">
                    <div class="form-group">
                        <i class="fa fa-user fa-lg"></i>
                        <input class="form-control required" type="text" placeholder="Username" id="username" name="username" autofocus="autofocus" maxlength="20"/>
                    </div>
                    <div class="form-group">
                            <i class="fa fa-lock fa-lg"></i>
                            <input class="form-control required" type="password" placeholder="Password" id="password" name="password" maxlength="8"/>
                    </div>
                    <div class="form-group">
                        <label class="checkbox">
                            <input type="checkbox" id="remember" value="1" />记住我
                        </label>
                    </div>
                    <div class="form-group col-md-offset-9">
                        <a  type="button" class="btn btn-primary pull-right" id="register" href="/register/">注册</a>
                        <button  type="button" class="btn btn-success pull-right" id="submit">登录</button>
                    </div>
                    <div class="form-group">
                            <i class="fa fa-exclamation-triangle fa-lg" id="display_text"></i>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <script>
        $("#submit").click(function(){
            if($("#username").val()!="" && $("#password").val()!="")
            {
                save_cookies();
                $.ajax({
                    url:'/login_validation/',
                    type:'post',
                    data:{
                        'username':$("#username").val(),
                        'password':$("#password").val()
                    },
                    dataType:'json',
                    success:function(args){
                        if(args.res==1){
                            window.location="/index/";
                        }else{
                            $("#display_text").html("用户名或密码错误.");
                        }
                    },
                    error:function(data){
                        alert('ajax error');
                    }
                });
            }else{
                alert("用户名或密码不能为空。");
            }
        });
     //保存到cookie
        function save_cookies(){
            if($("#remember").prop("checked")){
                var username = $("#username").val();
                var password = $("#password").val();
                $.cookie("remember","true",{expires:7});
                $.cookie("username",username,{expires:7 });
                $.cookie("password",password,{expires:7 });
            }else{
                $.cookie("remember","false",{expires:-1});
                $.cookie("username","",{ expires:-1 });
                $.cookie("password","",{ expires:-1 });
            }
        };
    </script>
</body>
</html>
```
