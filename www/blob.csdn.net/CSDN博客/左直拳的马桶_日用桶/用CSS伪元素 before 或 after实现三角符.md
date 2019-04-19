# 用CSS伪元素:before 或 :after实现三角符 - 左直拳的马桶_日用桶 - CSDN博客
2019年03月15日 18:30:08[左直拳](https://me.csdn.net/leftfist)阅读数：52
很low的文章。但我也要记一下。事实上，目前我们所学所掌握的，可能大部分都不是知识，只是一些……使用经验？随着技术更新换代，这些鸡毛蒜皮也将随着我们的年华一同流逝，被无情抛弃，总被雨打风吹去。剩下来的，只有苍苍白发，满脸皱纹，风湿骨痛。
嗯，杨辉三角符是这样的：
```
<style>
        .arrow-down{
            cursor:pointer;
        }
        .arrow-down:after {
             content: '\25bc';
             padding-left: 0.5em;
            color:#999;
            font-size:12px!important;
        }
</style>
```
```
<div>
<span class="arrow-down">我后面有一个杨辉三角</span>
</div>
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190315182944622.png)
