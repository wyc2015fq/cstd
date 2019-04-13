
# python web py入门（66）- jQuery - 隐藏显示实现界面大小可变布局 - 大坡3D软件开发 - CSDN博客

2018年05月03日 10:37:42[caimouse](https://me.csdn.net/caimouse)阅读数：493标签：[js](https://so.csdn.net/so/search/s.do?q=js&t=blog)[webpy](https://so.csdn.net/so/search/s.do?q=webpy&t=blog)[](https://so.csdn.net/so/search/s.do?q=js&t=blog)个人分类：[webpy](https://blog.csdn.net/caimouse/article/category/7445116)



在前面学习鼠标位置来显示不同的页面，现在来学习怎么样实现页面内容动态地改变。比如有一些不常用的功能，如果一直放在页面上，显得很占用位置，让用户一眼看上去，找不到聚焦点，这样用户感觉乱乱的，太复杂的感觉。为了保持界面清晰，简单，其实大家都喜欢简单，这样办事情效率才高。因此，设计页面交互时，常常在同一个页面会针对不同的用户，进行不同的内容显示，这样也叫个性设计，也可以叫人功智能自适应。怎么样实现隐藏呢？下面带着这个问题来学习。要实现隐藏，首先要学习HTML的隐藏的用法，如下面例子：
```python
<!DOCTYPE html>
<html>
  <body>
    <p hidden="hidden">这是一段隐藏的段落。</p>
    <p>这是一段可见的段落。</p>
  </body>
</html>
```
![](https://img-blog.csdn.net/20180503103444952)

在这个例子里，可以看到hidden="hidden"这个属性，会把这个元素的内容都不可见了，实现了凭空消失的本领。如果用什么方法可以动态在添加或删除这个属性，那么隐藏和显示，不就可以实现了吗?可见原理就是这么简单，这么明了。好吧，接着下面使用jQuery实现这个隐藏和显示，就更方便快捷了，因为它操作一类这样的元素是批量的，是像SQL语句一样可以满足条件的就查询出来。代码如下：
```python
<html>
  <head>
    <meta charset="utf-8">
    <title>文章显示系统</title>
     <link rel="stylesheet" href="page/page.css" type="text/css" />
     <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
     <script>
       $(document).ready(function(){
         //鼠标位置在切换窗口上显示不同的颜色
         $('#switcher').hover(
           function(){ $(this).addClass('hover');    },
           function(){ $(this).removeClass('hover'); }
         );
         // 隐藏按钮，缩小显示位置
         var toggleSwitcher = function(event) {
           if (!$(event.target).is('button')) {
             $('#switcher button').toggleClass('hidden');
           }
         };
         $('#switcher').on('click', toggleSwitcher);
         // 模拟一次点击，让它在收缩状态
         $('#switcher').click();
       });
     </script>
  </head>
  <body>
    <div id="container">
      <div id="switcher" class="switcher">
        <h3>页面布局选择</h3>
        <button id="switcher-default">
          默认
        </button>
        <button id="switcher-narrow">
          一般显示
        </button>
        <button id="switcher-large">
          大号显示
        </button>
      </div>
    </div>
    <span></span>
  </body>
</html>
```

在上一个例子里添加了下面的代码：
// 隐藏按钮，缩小显示位置
var toggleSwitcher = function(event) {
if (!$(event.target).is('button')) {
$('\#switcher button').toggleClass('hidden');
}
};
$('\#switcher').on('click', toggleSwitcher);

// 模拟一次点击，让它在收缩状态
$('\#switcher').click();
这一段代码，先定义了一个函数toggleSwitcher，它实现针对按钮进行隐藏。
$(event.target).is('button')
这一句用来判断是否是按钮类型的事件点击，如果是的话，就不要进行隐藏功能响应，在前面加了一个非运算符，就变成了非按钮点击都进行响应。
$('\#switcher button').toggleClass('hidden')
这一行代码使用ID选择器，然后再根据类button选择器来找到三个按钮，然后在三个按钮里添加hidden="hidden"，也就实现了隐藏的功能。不过toggleClass（）函数有点像一个触发器，点击一下展开，点击一下隐藏。
运行的效果如下：
![](https://img-blog.csdn.net/20180503103658432)
![](https://img-blog.csdn.net/20180503103712215)

### TensorFlow入门基本教程
### [http://edu.csdn.net/course/detail/4369](http://edu.csdn.net/course/detail/4369)
### C++标准模板库从入门到精通
### [http://edu.csdn.net/course/detail/3324](http://edu.csdn.net/course/detail/3324)
### 跟老菜鸟学C++
### [http://edu.csdn.net/course/detail/2901](http://edu.csdn.net/course/detail/2901)
### 跟老菜鸟学python
### [http://edu.csdn.net/course/detail/2592](http://edu.csdn.net/course/detail/2592)



