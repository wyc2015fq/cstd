# Java/JSP/JS Debug笔记 - =朝晖= - 博客园
# [Java/JSP/JS Debug笔记](https://www.cnblogs.com/dhcn/p/7116166.html)
2006年的blog，当时好生涩啊： 
------------------------ 谨以此文献给我没有头绪或心劲去debug的日子和很多辛苦debug的同志们。
      应部门一个科的需求，给他们写一个夜维日志记录的功能模块，本来是个很小的需求，计划一周内上线的，可能是我这个人比较偷懒，用了一点点以前略有不同的实现方式，搞得它之后的复杂需求都完成了，它那个模块还由于bug重重没有调试出来。
       本来这种模块可以用4到5个页面实现，为了图方便，仅用了两个页面实现，导致内容更新页面代码过多，所以在这个页面，问题比较多。
      东西写完基本代码快一个月了，刚出来的那段时间，可能是比较忙或者身体、心情的原因，debug一直是一筹莫展，最近感觉心劲还不错，把那个模块拿出来慢慢debug,今天一天（有点其他的杂事）定位了2个bug，解决了一个，一个还没有完全解决。
      第一个bug：这个夜维日志字段比较多，还需要上传附件，本来附件上传这个功能是比较熟悉的，可是我在附件上传完以后其他参数的传递再显示（在同一个页面上传附件，附件上传的过程，其他编辑控件中的填写值不变，这儿暂时没有用ajax。）和修改时原日志的从java到填写控件的值的传递才用了JS方式，就是在onload函数里对input空间赋初始值，我不知道这种方法是否最优，只是觉得这种方式在代码的组织上非常好，可惜这种方式在这个模块中我是第一次大规模用，可以说比较失败：
              1、把window.onload编辑时编辑成了winodw.onload.
              2、在一个input空间赋值的时候遗漏了<%=java variable>两边的''号;也就是因为这两个原因导致了向页面的传参失败。也就是第一个让人郁闷的“小”地方。JS的debug的确很考查人，难怪很多人都不喜欢用这个东西，第一：跨浏览器需要兼容，第二debug非常麻烦，差错点很难发现，上一个模块中有一个js的bug上线后才发现，还有一个上线N久都没有发现，还是后来我在论坛上讨论的时候发现自己代码那个地方有问题。
      第二个bug：编码的问题，我们这个项目编码问题本来是一次性解决的，但后来不知道怎么回事，在运行server Weblogic上可以正确运行的Charset Filter在我的测试Server Tomcat上没有效用，所以一般我在写代码的时候都要对测试代码的参数处理自己要写一下编码处理。本来这个处理我以前用过，可是后来那种代码在上线后就删除了，这次用到这种代码的时候，一时居然找不到代码，看了Charset资料写出来的有问题，采用
            value=new String(value.getBytes(from_encoding),"to_encoding");
写出来的代码编码以后并不能正确显示，可能是今天马上下班了，有点气躁了（从这个问题发现自己真够菜的！----Java基本功还很薄），可能是此处采用common upload组件作附件上传处理，还不太清楚它的字段值是什么编码。问了一下同组的一位大拿，指点了一下，common upload的item.getstring()默认编码方式可能是ISO-8859-1,所以需要使用getstring()的重载方法getstring("GB2312").代码改了，还没有上线测试——杂事太多。
      这个页面还发现一个bug:删除的时候文件名传参还有问题，目前还没有定位到此bug的差错点。
      觉得Gmail的那种附件上传方式不错，决定尝试一下！已经有人将它的原理解剖了（看http://howsmile.cnblogs.com/archive/2006/07/11/447837.html），不用我去啃GWT了。
       现在对GWT生成的JS和HTML比较感兴趣，很想分析一下，可惜没有时间和相关的分析资料。GWT的doc才看了1/3左右，很想看一下它生成代码的结构和运行流程，有相关经验和资料的同志可以留言一下，当然可能需要自己实践一下，昨天在一个网站上发现一个GWT  Visual eidtor,就是一个基于eclipse的GWT可视化界面编辑器，不过刚刚放出Beta版本，且不提供下载，需要发邮件申请，如果申请成功，给大家放出来------(主要是忘了收藏，链接也找不到了。用Firefox，没存历史。)
      10.24日今天在测试文档管理模块新功能时发现一个问题,select元素在disabled=true以后,就意味着其只是一个显示体了，在向后段发出请求的时候，也没有了这个元素对应的参数，所幸今天这个元素的readOnly不甚重要，我则直接设置为普通元素，只是赋了个初值。给一个相关解决此问题的索引http://www.cnblogs.com/dotage/archive/2005/05/16/156463.html吧
      顺便，这几天从老外的blog上面找到一个不错的JS tab控件，比较好用，完全的客户端代码，可惜免费版左下角有一个小小的链接，把地址也给贴出来吧:http://codethat.com/tab/ ，这家公司挺有名的，不过国内好像大多用它的grid和table控件。
    10.27日在安装上面提到那种iframe方式实现这个模块的无刷新上传时，一个小间断：给动态生成的一个元素的事件赋值一个函数对象，需要给这个函数传的参数也是动态生成的，这时候想到了把这个参数赋给这个元素的id,然后通过this传参，找到了一篇文章《JavaScript中this关键字使用方法详解》,这是我看过《ajax in action》某个章节后一直想看的一个话题，另外这个blog也不错，文章比较多。
     10.30日，以前做的一个工作用的 java的swt的小程序，每次都打开eclipse来启动比较麻烦，今天打了一下包，搜索"java 打包 eclipse"装了Fat Jar的eclipse打包插件，打好包，运行没反应,又搜索“swt 打包”找到《Eclipse SWT应用程序项目的打包与部署 》,原来swt程序需要与包jar同时部署它的本地dll文件，fat jar+copy本地dll打包成功。
    11.2日，今日解决了某个*.js的文件中的变量无法引用的问题，原因在于通过servlet转向到jsp文件，jsp文件的目录在firefox中是servlet的目录，导致原有的jsp中引用js的相对目录失效所引起，所以js在服务器上引用最好用服务器绝对目录。
         第二个问题比较让人吐血，一个在firefox中通过html性质注册的事件函数在IE下无效,最后试来试去,原来是函数名的问题，函数名不能是on**之类的元素事件性质的名字，否则就会导致此处的时间性质无效。这个性质倒可以说非正常之大正常
   11.3日  今天发现了IE中iframe file upload无反应的原因，因为对top自定义的一个属性好象不能成立。
firefox的删除调试也趋近完成，发现一个令人意外的东西：在FireFox中一个Element的childNodes里面竟然把换行符当作是text元素算在内，所以要真对有意义的childNodes作遍历，最好前面用nodeType作一下过滤！
   11.7日 1、今天发现在FireFox中对NodeList遍历的第二个问题，如果你需要获得一个Node的非Node属性的其他如id之类的区别于nodeType的Element属性，不能用直接以属性名索引的方式如：(children=Element.childNodes;)children[i].id这样的方式，而必须用children[i].attributes['id'].value这样的方式，也就是说Node的类型不能实现动态判定，可惜我还不清楚js中怎样把Node的类型实现动态转换为具体的Element类型。 
2、发现IE和FireFox给common fileupload上传组件传值的一个差别:IE传给的文件名的是完整路径包括盘符和目录，而FireFox传给的就是文件名。
    11.9日解决event.target为空或不是对象的问题，下把那张贴子贴出来吧:
  事件通过以下方式注册:
    var delA=document.createElement('a');
    delA.id=attachfile;
    delA.className='delfile';
    delA.appendChild(document.createTextNode("X"));
    delA.onclick=top.deleteFile;//注册事件
    attLi.appendChild(delA);
    事件处理函数如下定义
function deleteFile(event){
    var url='.....';
    var params='attachid='+Event.element(event).id;
                           ~~~~~~~~~~~~~~~~~~~~Prototype的函数！
    .....
}
//IE6的报错“为'target' 为空或不是对象”出错的也就是这条语句：
也就是在prototype中的这条语句：
  element: function(event) {
    return event.target || event.srcElement;
  },
请问Event在传srcElement的合理用法应该是怎样，或者各位在这种方式的事件注册方式下有什么更好的给函数传参的方式，也欢迎提出来。
   ps:今天这个问题和那天那个iframe的问题在FireFox下都不会发生。
   而且IE中还发生了"不能执行已释放 Script 的代码"这个框架代码相互操作时的怪异问题，被迫将本来动态生成的poptip最后还是用DIV写在了网页里再进行操作
——————————————————————————————————————
解 决,通过采用Prototype的事件注册方式注册事件处理函数即可。我在事件处理函数中alert判定了一下，发现event本身就不是所要的 event,所以IE根本没有把事件传递进来，于是采用Prototype的注册方式试了一下，因为我发现scriptaculous的事件处理代码本身 并不作event是否为空的判定，却可以正确的处理event，也就是说在IE中如果要代码中注册事件并传递event参数，必须采用Event的事件处 理模型(Element.attachEvent)注册事件，才可以达到相应的目标，也就是意味着IE4以前的浏览器，Prototype这种代码就没法 用了。 
      上面这个bug解决了以后，虽然也出现了其它的bug，但总的今天比较顺利，全部解决，至此这个日志模块公开测试版开始交给配置管理员上线。
     总结一下：JS/JSP的调试非常琐碎，需要极大的耐心和细心。比较便利的方法就是走比较正常的代码路线，不要讨巧走邪路。
                                                                                       2006年10.20日第一次更新
                                                                                       2006年10.24日第二次更新
                                                                                       2006年10.27日第三次更新
                                                                                       2006年11.2日 第四次更新
                                                                                       2006年11.7日第五次更新
                                                                                       2006年11.8日第六次更新(最后一次)

