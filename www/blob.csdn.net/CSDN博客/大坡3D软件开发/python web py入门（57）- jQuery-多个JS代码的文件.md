
# python web py入门（57）- jQuery - 多个JS代码的文件 - 大坡3D软件开发 - CSDN博客

2018年04月15日 19:19:56[caimouse](https://me.csdn.net/caimouse)阅读数：462


在前面学习了HTML与JS代码分离的功能，但是会随着项目功能的增加，或者随着时间的增加，维护了代码，新增加的代码不想影响旧的代码，必然会增加更多的JS代码文件。这样多个JS文件是怎么样调用的顺序呢？能不能随便地放置位置呢？依赖的关系是否很复杂？当很多模块代码时，加载页面速度是否会变慢？问题还是很多的，后面再来一一破解。先来看看怎么样分成多个JS文件，以及文件加载的顺序。为此，创建一个add.js文件，它的代码如下：
```python
alert("add.js法函数功能");
function add(s){
  alert(s);
}
```
创建一个sum.js文件，代码如下：
```python
alert("sum.js累加函数功能");
function sum(s){
  alert(s);
  //这里调用add.js文件的函数
  add("add:1+2");
}
```

修改checkemail.js文件，在这里调用sum.js里的代码，如下：
```python
$(document).ready(function(){
  var bd = {
        bind :  function(input_element, tip_element, validate_func, null_tip, invalid_tip) {
          var result = new Object();
          result.valid = false;
          // 为input_element绑定blur事件
          $(input_element).blur(function() {
              var input_val = input_element.val();
              if (input_val == "") {
                  result.valid = false;
                  $(tip_element).html(null_tip);
              } else if (!validate_func(input_val)) {
                  result.valid = false;
                  $(tip_element).html(invalid_tip);
              } else {
                  result.valid = true;
                  $(tip_element).html("");
              }
          });
          return result;
        }
  };
  //测试一下对象运行
  var ret = bd.bind($("form#register #email"),
    $("form#register #email").next(),
    function(val) {return val.match(/^([.a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+((\.[a-zA-Z0-9_-]{2,3}){1,2})$/);},
    "< 请输入邮箱",
    "< 邮箱格式不对"
  );
  //这里调用sum.js文件
  sum("sum:3");
  console.log(ret);
});
```

通过运行，可以发现会按这个顺序先加载：
<script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
<script type="text/JavaScript" src= "app/add.js"></script>
<script type="text/JavaScript" src= "app/sum.js"></script>
<script type="text/JavaScript" src= "app/checkemail.js"></script>
也就是说，满足下面条件：
假设条件是:JS(A)要调用JS(B)的函数.那么要满足以下条件:
1.要保证你所调用的JS必须在同一个页面里. 也就是JS(A)和JS(B)都要在页面X里.
2.要保证你所调用的JS先于调用者本身被解释.也就是JS(B)要先于JS(A)被解释.反映在页面上,就是JS(B)要写到JS(A)的前面.
3.要保证所调用的JS必须是同一个字符集下.这里有两层意思,一个是引用的JS文件的编码字符集要一样,还一个是嵌入HTML的JS标签属性charset也要一样.

运行的结果如下：
![](https://img-blog.csdn.net/20180415191842753)
![](https://img-blog.csdn.net/20180415191858183)
![](https://img-blog.csdn.net/20180415191915284)
![](https://img-blog.csdn.net/20180415191927625)

### 在VC2015里使用protobuf协议
### [http://edu.csdn.net/course/detail/2582](http://edu.csdn.net/course/detail/2582)
### 在VC2015里学会使用MySQL数据库
### [http://edu.csdn.net/course/detail/2672](http://edu.csdn.net/course/detail/2672)



