# Android弹框使用:Dialog&Toast&Snackbar - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月26日 13:21:41[boonya](https://me.csdn.net/boonya)阅读数：639标签：[java																[android																[控件																[消息提示](https://so.csdn.net/so/search/s.do?q=消息提示&t=blog)
个人分类：[Android](https://blog.csdn.net/boonya/article/category/2496125)





文章来源：[http://blog.csdn.net/guolin_blog/article/details/51336415](http://blog.csdn.net/guolin_blog/article/details/51336415)


      Dialog和Toast所有人肯定都不会陌生的，这个我们平时用的实在是太多了。而Snackbar是Design Support库中提供的新控件，有些朋友可能已经用过了，有些朋友可能还没去了解。但是你真的知道什么时候应该使用Dialog，什么时候应该使用Toast，什么时候应该使用Snackbar吗？本篇文章中我们就来学习一下这三者使用的时机，另外还会介绍一些额外的技巧。

# 1. Dialog

首先来介绍一下Dialog的用法吧，其实很简单，相信大多数人都是经常使用的：



```java
AlertDialog.Builder builder = new AlertDialog.Builder(this);
builder.setTitle("Title")
       .setMessage("Dialog content.")
       .setPositiveButton("OK", new DialogInterface.OnClickListener() {
               @Override
               public void onClick(DialogInterface dialog, 
               int which) {
               }
       })
       .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
               @Override
               public void onClick(DialogInterface dialog,
                int which) {
               }
        })
       .show();
```
这段代码就可以弹出一个非常精美的Dialog了，如下图所示： 
![](https://img-blog.csdn.net/20160717190208018)
现在这个Dialog是Material Design风格的，因为我是在6.0系统上运行的，因此会自动赋予这样的风格。但是如果在老版本系统上运行，比如说2.3系统，会是什么样的效果呢？运行一下就知道了，如下图所示：

![](https://img-blog.csdn.net/20160717190240159)
额。。这个效果就比较丑了，不过没办法，这就是2.3系统当时的风格呀。

人的审美总是在进步的，我们有没有什么办法在老版本的系统中也使用Material Design风格的Dialog呢？当然有，Google已经充分考虑到了这一点，在appcompat-v7库中也提供了一个AlertDialog类，完整路径是：

```java
android.support.v7.app.AlertDialog
```
我们使用这个包中的AlertDialog，就能让对话框在所有的系统版本中都保持一致的风格了。现在在2.3系统中重新运行一下，效果如下所示：

![](https://img-blog.csdn.net/20160717190351520)
可以看到，现在的效果就比较不错了，这也算是一个小技巧吧。

Dialog的作用是给用户一个提示信息，并让用户根据提示做出判断。而Dialog的特征就是，它会阻止你原本正在进行的操作，必须停止下来对Dialog进行处理。但是，大多数的人可能并不喜欢这样被打断，也许用户正在处理一项重要的操作，突然弹出一个Dialog遮挡住了他原本的操作，这个时候用户会变得很恼火。

因此，使用Dialog的时候还是谨慎一点比较好，尽量不要给用户带来糟糕的体验感。

# 2. Toast

说到不会阻挡用户原本正在进行的操作，这就延伸到我们今天的第二个主题，Toast。Toast只会弹出一段信息，告诉用户某某事情已经发生了，过一段时间后就会自动消失。它完全不会阻挡用户的任何操作，甚至用户也可以完全不用理会Toast。

那么我们还是先来看一下Toast的基本用法吧，如下所示：



```java
Toast.makeText(context, "things happened", Toast.LENGTH_SHORT).show();
```
最后一个参数用于指定Toast显示的时长，Toast.LENGTH_SHORT表示显示时间较短，Toast.LENGTH_LONG表示显示时间较长。

不过也不是说Toast的用法就一点深度都没有了，比如说上述的写法就会存在如下图所示的问题：
![](https://img-blog.csdn.net/20160717190636909)
可以看到，这里我快速连续点击了五次按钮，Toast就触发了五次。这样的体验其实是不好的，因为也许用户是手抖了一下多点了几次，导致Toast就长时间关闭不掉了。又或者我们其实已在进行其他操作了，应该弹出新的Toast提示，而上一个Toast却还没显示结束。

因此，最佳的做法是将Toast的调用封装成一个接口，写在一个公共的类当中，如下所示：



```java
public class Util {

    private static Toast toast;

    public static void showToast(Context context, 
        String content) {
        if (toast == null) {
            toast = Toast.makeText(context,
                         content, 
                         Toast.LENGTH_SHORT);
        } else {
            toast.setText(content);
        }
        toast.show();
    }

}
```
可以看到，这里和我们平时使用Toast的方式并不一样，这里会先判断Toast对象是否为空，如果是空的情况下才会调用makeText()方法来去生成一个Toast对象，否则就直接调用setText()方法来设置显示的内容，最后再调用show()方法将Toast显示出来。由于不会每次调用的时候都生成新的Toast对象，因此刚才我们遇到的问题在这里就不会出现了。

调用的时候也很简单，只需要把Context对象和Toast要显示的内容传进来就可以了：

```java
Util.showToast(context, "things happened");
```
现在我们再重新运行一遍程序，效果如下图所示： 
![](https://img-blog.csdn.net/20160717190747269)
可以看到，现在不管我们触发多少次Toast调用，都只会持续一次Toast显示的时长，这也算是一个小技巧吧。

Toast的作用是告诉用户现在发生了什么事情，不会阻挡用户的操作，但同时用户只能被动接收这个事情，因为没有什么办法来让用户是选择同意还是拒绝。

虽说Toast在用户体验方面要比Dialog好一些，但是也要慎用，尤其是涉及到一些敏感操作的时候。比如说删除数据，只给用户一个提示：“你的数据已被删除”，而不给用户选择是否要删除的机会，这个时候用户可能就要暴走了。

# 3. Snackbar

如果说Dialog和Toast是两个极端的话，那么Snackbar就是处于中间的位置了。Snackbar和Toast比较相似，但是用途更加广泛，并且它是可以和用户进行交互的。Snackbar使用一个动画效果从屏幕的底部弹出来，过一段时间后也会自动消失。

在使用Snackbar之前，首先需要在app/build.gradle中添加相应的依赖：



```java
dependencies {
    compile 'com.android.support:design:23.4.0'
}
```
然后就可以使用Snackbar了，它的用法和Toast是比较相似的：



```java
Snackbar.make(view, "data deleted",Snackbar.LENGTH_LONG)
        .setAction("Undo", new View.OnClickListener(){
                @Override
                public void onClick(View v) {
                }
            })
        .show();
```
这里调用Snackbar的make()方法来创建一个Snackbar对象，make()方法的第一个参数需要传入一个view，只要是当前界面布局的任意一个view都可以，Snackbar会使用这个view来自动查找最外层的布局，用于展示Snackbar。第二个参数就是Snackbar中显示的内容，第三个参数是Snackbar显示的时长。这些和Toast都是类似的。

接着这里又调用了一个setAction()方法来设置一个动作，从而让Snackbar不仅仅是一个提示，而是可以和用户进行交互的。最后调用show()方法让Snackbar显示出来。

现在重新运行一下程序，效果如下图所示： 

![](https://img-blog.csdn.net/20160717191403222)

可以看到，Snackbar的效果有点类似于Toast，不过它是从屏幕底部弹出来的。另外Snackbar上面可以加入和用户交互的按钮，比如删除数据的时候给用户一个Undo的选项，从这些小的细节方面都可以提升很多的用户体验。

# 4. 总结

现在你有三种方式可以给用户提示信息，Dialog、Toast和Snackbar，下面我们对这三种方式的使用时机做个总结吧。
- Dialog：当提示信息是至关重要的，并且必须要由用户做出决定才能继续的时候，使用Dialog。
- Toast：当提示信息只是告知用户某个事情发生了，用户不需要对这个事情做出响应的时候，使用Toast。
- Snackbar：以上两者之外的任何其他场景，Snackbar可能会是你最好的选择。](https://so.csdn.net/so/search/s.do?q=控件&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




