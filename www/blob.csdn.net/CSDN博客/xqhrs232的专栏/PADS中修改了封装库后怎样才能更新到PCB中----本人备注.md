# PADS中修改了封装库后怎样才能更新到PCB中----本人备注 - xqhrs232的专栏 - CSDN博客
2014年07月26日 22:06:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2112
原文地址::[http://zhidao.baidu.com/link?url=EPfrRwpOYi6YW1k0NLQGgRuW4zF6rWFLg2Y8BkvgzH0DSGKqnjgAFComwSsjMYYyJZm1roClkLdeL6Gtg_seyK](http://zhidao.baidu.com/link?url=EPfrRwpOYi6YW1k0NLQGgRuW4zF6rWFLg2Y8BkvgzH0DSGKqnjgAFComwSsjMYYyJZm1roClkLdeL6Gtg_seyK)
相关文章
1、pads中修改了封装库后怎样才能更新到PCB中----[http://qianbwh.blog.163.com/blog/static/1860924072012112510442594/](http://qianbwh.blog.163.com/blog/static/1860924072012112510442594/)
2种方法：
1：在PCB中进入ECO模式-选择要更新的元件-点击change component图标-右键点击元件-在弹出的菜单中选择libraby browse-勾选update part type from library 
2. 从logic[原理图](http://zhidao.baidu.com/search?word=%E5%8E%9F%E7%90%86%E5%9B%BE&fr=qb_search_exp&ie=utf8)来更新PCB
打开[原理图](http://zhidao.baidu.com/search?word=%E5%8E%9F%E7%90%86%E5%9B%BE&fr=qb_search_exp&ie=utf8)，选中要变更的元件，分配好要变更的PCB封装
tools-[PADS Layout](http://zhidao.baidu.com/search?word=PADS%20Layout&fr=qb_search_exp&ie=utf8)...点击preference,勾选compare PCB Decal Assignments-design-ECO to PCB

//==========================================================================================================================
备注::
1》针对已经画好的PCB最好使用第1种方法，因为画好的PCB板加了很多过地孔，如果使用第2种方法，那PCB中放好的过地孔的重新放置一遍，这样太麻烦了！！！
2》要更新一个Part Type的封装类型，最好重新建一个Part Type，然后绑定新的PCB封装，再采用第1种方法去更新
3》在PCB中可以直接增加元器件与连接，可以不需要去先从SCH中增加,然后再ECO to PCB
     //1111----进入PCB的ECO模式(ECO Toolbar), Add Connection/Add Route/Add Component,然后去布线 
//2222----Rename Component/Change Component 
     //3333----Delete Connection/Delete Net/Delete Component 
     //4444----增加的元器件如果与GND网络接到了一起，删除这个元器件会把放好的过地孔全部删除， 断开这个元器件的对地连接/把改元器件移到板的范围外再将该元器删除,就不会对过地孔造成影响 
    //5555----ECO工程更改可以从SCH到PCB，或者从PCB到SCH----这样就可以避免PCB中增加好的过地孔被删除 《昨天试了一下从SCH更新到PCB，PCB的过地孔又没有被删除，奇怪！》
