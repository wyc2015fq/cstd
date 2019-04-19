# jquery Ajax应用 - _天枢 - 博客园
## [jquery Ajax应用](https://www.cnblogs.com/yhleng/p/8571654.html)
2018-03-15 09:28 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8571654)
** 介绍：**
**　　 AJAX 是一种与服务器交换数据的技术，可以在不重新载入整个页面的情况下更新网页的一部分。**
**jquery ajax语法:**
```
$.ajax({
　　url:'/login/',     #向后台传送数据的地址
　　type:'post',    #数据传送的类型
　　data:{                                         #传送的数据
　　　　'username':'qcbin',
　　　　'password':'q123456'
　　　　},
　　dataType:'json',    #返回的数据类型
　　success:function(args){
　　　　alert(args.res); #返回的是一个json，args是一个json对象，res是字典的key值
　　},
　　error:function(err){
　　　　alert(err);
　　}
});
```
**示例代码：点击id是submit的按钮，时触发ajax事件。**
**先介绍几种jquery获取对象，赋值的方法。**
$("#submit").click();  #jquery的用法，点击id是submit时触发。
$("#username").val(); #取id为username的value
$("#username").val('qcbin');  #给id为username的对象value赋值为qcbin
$("#content").html(); #获取标签赋值相当于<span id="content">获取这里的值</span>
$("#content").html("abc123"); #给标签赋值相当于<span id="content">给这里赋值</span>
```
$("#submit").click(function(){
            if($("#username").val()!="" && $("#password").val()!="")
            {
                **$.ajax**({
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
```
