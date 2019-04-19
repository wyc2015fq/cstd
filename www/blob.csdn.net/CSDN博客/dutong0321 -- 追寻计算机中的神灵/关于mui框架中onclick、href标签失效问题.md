# 关于mui框架中onclick、href标签失效问题 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年03月06日 17:28:16[dutong0321](https://me.csdn.net/dutong0321)阅读数：7574
2018-05-30更新： 
href尚未发现什么问题，但是最近发现onclick在不同浏览器中经常崩溃，所以我又重新想了一个办法来更新一下。
```java
mui('body').on('tap', 'a', function() {
            // 获取onclick
            var str = this.onclick;
            // 强制转换为字符串
            str = String(str);
            /*
            例如：onclick='alert("Hello")';
            转换成的字符串内容大概是：
            function click(event){
                alert("Hello")
            }
            所以，需要做一些截取。
             */
            var order = str.split('\n');
            console.log(order[1]);
            // 执行就好了
            eval(order[1]);
        });
```
原文： 
MUI框架我觉得是个不错的框架，至少在移动端用起来还是很不错的，减少很多的工作量。不过在使用的时候需要注意2点： 
1.滚动页面时需要采用MUI中的方法滚动，因为今天没有遇到，以后可能会总结的写一些。 
2.onclick/href方法失效。 
关于onclick、href方法失效，还并不是完全失效，有的地方可以用，同一个地方有的浏览器也可以用，甚至同一个地方同一个浏览器多点几次也有时候有效，很坑爹，很莫名奇妙，有时候大晚上的都会让你怀疑灵异事件。 
在我私底下开发自己网站的前端页面时，我遇到过，从晚上12点到凌晨2点才找到。这不是最坑，今天上午给老板演示又遇到这个问题，下午开始以为是动态加载的原因，后来调了将近1个小时才想起来。 
OK，不废话，先看看MUI官方的解答。 
[官网下的 demo MUI ， 为什么 a链接都是失效的](http://ask.dcloud.net.cn/question/6444)
大概意思就是，只要引入官方的JS文件，就会因为MUI的一些控件需要拦截onclick和href而选择屏蔽。 
解决方案有2种： 
1.使用官方提供的方法：“除了可以使用addEventListener()方法监听某个特定元素上的事件外， 也可以使用.on()方法实现批量元素的事件绑定。”[事件绑定](http://dev.dcloud.net.cn/mui/event/#on)
2.这是我按照MUI官方解答里面云雾回答的思路想的一种方法，既然官方可以屏蔽，咱们就可以再创建。 
云雾关于启动a链接中href生效的方法
```java
// 监听tap事件，解决 a标签 不能跳转页面问题
mui('body').on('tap','a',function(){document.location.href=this.href;});
```
我仿照他，写的关于启动onclick的方法：
```java
mui('body').on('tap','a',function(){
    // 获取onclick
    var str = this.onclick;
    // 强制转换为字符串
    str = String(str);
    /*
    例如：onclick='alert("Hello")';
    转换成的字符串内容大概是：
    function click(event){
        alert("Hello")
    }
    所以，需要做一些截取。
    */
    var functionString = str.substr(28,str.length - 29);
    // 执行就好了
    eval(functionString);
});
```
有BUG没？有的。 
没事别return，有事也别return。不过话说都屏蔽了，也不跳转form中的action了，return不return我真觉得没必要了。 
可以执行多条语句。 
就是这样！ 
另： 
自己的项目采用的是第一种方法，因为从0构建，好修改，其实这种思路是很好的。 
公司的项目采用的是第二种方法，因为我已经写了很多了，部分核心代码都是和PC一样的，重新构建不划算，再加上最近比较急，所以用到mui.js的页面直接复制了几行代码就搞定了。 
欢迎留言，看到就会回复！
