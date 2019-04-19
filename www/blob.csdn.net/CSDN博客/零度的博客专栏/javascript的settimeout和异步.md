# javascript的settimeout和异步 - 零度的博客专栏 - CSDN博客
2016年06月30日 10:45:51[零度anngle](https://me.csdn.net/zmx729618)阅读数：346
       今晚看到QLeelulu的[一道JavaScript面试题（setTimeout）](http://www.cnblogs.com/QLeelulu/archive/2011/06/13/2079965.html)，稍微想了一下，好不容易连猜带蒙，凑巧说对了答案。但是原因到底是什么呢？自己一时也说不太清楚，反正感觉就是一个死循环造成的。然后看了一下文章下面的评论，发现5楼和6楼的回答很有道理,主要意思就是说javascript引擎是单线程执行的，while循环那里执行的时候，settimeout里面的函数根本没有执行的机会，这样while那里永远为真，造成死循环。但是单纯看还是不怎么踏实，最后发挥实践精神，自己动手做了两个实验。
       1、简单的settimeout
```
setTimeout(function () { while (true) { } }, 1000);
        setTimeout(function () { alert('end 2'); }, 2000);
        setTimeout(function () { alert('end 1'); }, 100);
        alert('end');
```
       执行的结果是弹出‘end’‘end 1’，然后浏览器假死，就是不弹出‘end 2’。也就是说第一个settimeout里执行的时候是一个死循环，这个直接导致了理论上比它晚一秒执行的第二个settimeout里的函数被阻塞，这个和我们平时所理解的异步函数多线程互不干扰是不符的。
       2、ajax请求回调
       接着我们来测试一下通过xmlhttprequest实现ajax异步请求调用，主要代码如下：
```
var xmlReq = createXMLHTTP();//创建一个xmlhttprequest对象
        function testAsynRequest() {
            var url = "/AsyncHandler.ashx?action=ajax";
            xmlReq.open("post", url, true);
            xmlReq.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            xmlReq.onreadystatechange = function () {
                if (xmlReq.readyState == 4) {
                    if (xmlReq.status == 200) {
                        var jsonData = eval('(' + xmlReq.responseText + ')');
                        alert(jsonData.message);
                    }
                    else if (xmlReq.status == 404) {
                        alert("Requested URL is not found.");
                    } else if (xmlReq.status == 403) {
                        alert("Access denied.");
                    } else {
                        alert("status is " + xmlReq.status);
                    }
                }
            };
            xmlReq.send(null);
        }
        testAsynRequest();//1秒后调用回调函数
        
        while (true) {
        }
```
        在服务端实现简单的输出：
```
private void ProcessAjaxRequest(HttpContext context)
        {
            string action = context.Request["ajax"];
            Thread.Sleep(1000);//等1秒
            string jsonObject = "{\"message\":\"" + action + "\"}";
            context.Response.Write(jsonObject);
        }
```
`     理论上，如果ajax异步请求，它的异步回调函数是在单独一个线程中，那么回调函数必然不被其他线程”阻挠“而顺利执行，也就是1秒后，它回调执行弹出‘ajax’，可是实际情况并非如此，回调函数无法执行，因为浏览器再次因为死循环假死。</span>`
        结论：根据实践结果，可以得出，javascript引擎确实是**单线程**处理它的任务队列（能理解成就是普通函数和回调函数构成的队列吗？）的。在javascript里实现异步编程很大程度上就是一种障眼法，单线程的引擎实现多线程的编程，如果要实现一些资源同步互斥之类的操作（一如C#、Java等语言的多线程），我感觉真正实现起来根本无法轻易得到保证。
       补充：如何实现javascript的sleep呢？在stackoverflow上找到一篇[javascript
 sleep](http://stackoverflow.com/questions/951021/javascript-sleep)，试了一下，效果是有了，但是执行的时候cpu很高，真还不如直接settimeout呢。
