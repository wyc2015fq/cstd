
# jQuery Tools Scrollable - 追求卓越,做到专业 - CSDN博客


2016年04月27日 19:36:14[Waldenz](https://me.csdn.net/enter89)阅读数：931标签：[scrollable																](https://so.csdn.net/so/search/s.do?q=scrollable&t=blog)个人分类：[JS/jQuery																](https://blog.csdn.net/enter89/article/category/1257949)


参考链接[点击打开链接](http://www.jb51.net/article/19904.htm)

```python
var scrollable=$("div.scrollable").scrollable(); 
    //alert(scrollable.getConf().prev);//获取配置对象中的prev属性 
    scrollable.getConf().speed=200;//设置配置对象的speed属性 
    //alert(scrollable.getIndex());//获取当前滚动项的索引 
    //alert(scrollable.getItems().length);//获取当前滚动项的数量 
    //alert(scrollable.getItemWrap().html());//获取包含滚动项的节点(class=scrollable)，并将所有滚动项显示出来 
    //alert(scrollable.getPageAmount());//获取当前滚动栏分页数 
    //alert(scrollable.getPageIndex());//获取当前所在分页 
    //alert(scrollable.getRoot().html());//获取滚动项的上一级节点（id=thumbs） 
    //alert(scrollable.getSize()); 
    //alert(scrollable.getVisibleItems().length);//获取当前可见滚动项数量 
    scrollable.next();//如果有下一个滚动项，则跳转到下一个滚动项 
    scrollable.prev(3000,function(){return true});//跳转到前一滚动项 
    //var seekTo= scrollable.click(0).seekTo(2,1000,function(){ 
        //alert(this.getIndex()); 
    //}); 
    //scrollable.move(2); 
    //scrollable.prevPage();//跳转到前一页 
    //scrollable.nextPage();//跳转到下一页 
    //scrollable.setPage(1);//跳转到下一页 
    //scrollable.begin();//跳转到第一个滚动项 
    //scrollable.end();//跳转到最后一个滚动项 
    scrollable.click(3);//使第四个滚动项处于选中状态 
    scrollable.onBeforeSeek(function(){ 
            alert("you click the "+this.getIndex()+"st scrollable item!"); 
    }); 
    $("#remove").click(function(){ 
        scrollable.getItems().filter(":last").remove();//删除最后一个滚动项 
        scrollable.reload().prev();//自动更新相关配置信息，并跳转到被删除滚动项的前一项 
    });
```
|方法名称
|返回值|说明|
|getConf()|Object|返回|scrollable|的配置对象，并且可通过设置该对象的相关属性值来修改该配置对象的属性。|
|getIndex()|number|获取当前滚动项的索引号，0代表第一个元素，1代表第二个元素，以此类推。此外，需注意的是，如果获取到多个滚动项，那么将会只返回第一个滚动项的索引号。|
|getItems()|jQuery|返回所有的滚动项，结果以jquery对象的方式返回。|
|getItemWrap()|jQuery|获取滚动项的父节点，结果以jquery对象的方式返回。|
|getPageAmount()|number|获取当前滚动栏的分页数。|
|getPageIndex()|number|返回当前分页索引号。比如说，如果分页设置为5个滚动项/页，并且当前滚动项位置为7的话，那么将会返回1（第二页）|
|getRoot()|jQuery|获取滚动项的上一级节点。|
|getSize()|number|返回滚动项的数量。该方法等同于getConf().size|
|getVisibleItems()|jQuery|获取一个由当前可见滚动项组成列表，该列表为一个jquery对象，可见滚动项的数量由配置对象的size属性定义。|
|
|reload()|API|scrollable支持动态添加和删除滚动项的功能。在动态添加或删除滚动项以后，调用此方法来自动更新分页导航以及滚动项移动的相关信息。|
|prev()|API|跳转到该滚动项的前一项（如果该滚动项不是第一个滚动项）|
|next()|API|跳转到该滚动项的下一项（如果该滚动项不是最后一个滚动项）|
|seekTo(index)|API|跳转到指定索引处的滚动项。|
|move(offset)|API|将处于当前状态（激活）的滚动项位置由当前滚动项向前/后移动offset。Offset为正，则滚动项向右/下移动，否则，向左/上移动。比如:move(2),则处于当前状态的滚动项的索引由i滚动项转移至i+2滚动项。|
|prevPage()|API|跳转到前一页（如果该页不是第一页）。|
|nextPage()|API|跳转到后一页（如果该页不是最后一页）。|
|setPage(index)|API|跳转到第index页。比如，index=2,那么会从当前页跳转到第3页。|
|movePage(offset)|API|用于将显示页的位置由当前页切换到该页/后offset页，该方法其他解释类似于(offset)。|
|begin()|API|跳转到第一个滚动项，相当于seekTo(0)。|
|end()|API|跳转到最后一个滚动项。|
|click(index)|API|使第|index|个滚动项处于选中（激活）状态。|
|
|onBeforeSeek(fn)|API|参见配置对象的|onBeforeSeek相关说明|
|onSeek(fn)|API|参见配置对象的onSeek相关说明|
注意：上面方法表中prev()方法以下的方法除了表中携带的参数外，还包含两个隐含参数：speed和callback。其中speed参数是用于控制滚动项的动画效果持续时间的，而callback为其回调方法。具体实现可参见scrollable的prev()方法使用示例。
完整示例代码

```python
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" 
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<script src="http://cdn.jquerytools.org/1.0.2/jquery.tools.min.js"></script> 
<script src="http://static.flowplayer.org/js/jquery.mousewheel.js" ></script> 
<link rel="stylesheet" type="text/css" href="http://static.flowplayer.org/tools/css/scrollable-navig.css" /> 
<style><!-- 
div.scrollable { 
    position:relative; 
    overflow:hidden; 
    width: 646px;     
    height:300px;     
} 
#thumbs {     
    position:absolute; 
    width:20000em;     
    clear:both; 
    border:1px solid #222; 
} 
a.prev, a.next { 
    margin-top:118px;     
} 
#thumbs div { 
    float:left; 
    width:214px; 
    height:300px; 
    background:#333 url(/upload/20090906120115441.png) repeat-x 0 146px; 
    color:#fff; 
    border-left:1px solid #333; 
    cursor:pointer; 
} 
#thumbs div.hover { 
    background-color:#444;     
} 
#thumbs div.active { 
    background-color:#066; 
    cursor:default; 
} 
#thumbs h3, #thumbs p, #thumbs span { 
    margin:13px;         
    font-family:"bitstream vera sans"; 
    font-size:13px; 
    color:#fff;     
} 
#thumbs h3 em { 
    font-style:normal; 
    color:yellow; 
} 
--></style><style>div.scrollable { 
    position:relative; 
    overflow:hidden; 
    width: 646px;     
    height:300px;     
} 
#thumbs {     
    position:absolute; 
    width:20000em;     
    clear:both; 
    border:1px solid #222; 
} 
a.prev, a.next { 
    margin-top:118px;     
} 
#thumbs div { 
    float:left; 
    width:214px; 
    height:300px; 
    background:#333 url(/upload/20090906120115441.png) repeat-x 0 146px; 
    color:#fff; 
    border-left:1px solid #333; 
    cursor:pointer; 
} 
#thumbs div.hover { 
    background-color:#444;     
} 
#thumbs div.active { 
    background-color:#066; 
    cursor:default; 
} 
#thumbs h3, #thumbs p, #thumbs span { 
    margin:13px;         
    font-family:"bitstream vera sans"; 
    font-size:13px; 
    color:#fff;     
} 
#thumbs h3 em { 
    font-style:normal; 
    color:yellow; 
}</style> 


<!-- navigator --> 
<div class="navi"></div> 
<!-- prev link --> 
<a class="prev"></a> 
<!-- root element for scrollable --> 
<div class="scrollable"> 
    <div id="thumbs"> 
         
            <div> 
                <img src="http://files.jb51.net/upload/20090906120116994.jpg" /> 
                <h3><em>1. </em>An example title</h3> 
                <p> 
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin et felis eget 
                    tellus pharetra porttitor. Praesent dui arcu, egestas quis, adipiscing a. 
                </p> 
                <span class="blue">60 sec</span> 
            </div> 
            <div> 
                <img src="http://files.jb51.net/upload/20090906120117587.jpg" /> 
                <h3><em>2. </em>An example title</h3> 
                <p> 
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin et felis eget 
                    tellus pharetra porttitor. Praesent dui arcu, egestas quis, adipiscing a. 
                </p> 
                <span class="blue">80 sec</span> 
            </div> 
            <div> 
                <img src="http://files.jb51.net/upload/20090906120117884.jpg" /> 
                <h3><em>3. </em>An example title</h3> 
                <p> 
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin et felis eget 
                    tellus pharetra porttitor. Praesent dui arcu, egestas quis, adipiscing a. 
                </p> 
                <span class="blue">100 sec</span> 
            </div> 
            <div> 
                <img src="http://files.jb51.net/upload/20090906120117490.jpg" /> 
                <h3><em>4. </em>An example title</h3> 
                <p> 
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin et felis eget 
                    tellus pharetra porttitor. Praesent dui arcu, egestas quis, adipiscing a. 
                </p> 
                <span class="blue">120 sec</span> 
            </div> 
            <div> 
                <img src="http://files.jb51.net/upload/20090906120118968.jpg" /> 
                <h3><em>5. </em>An example title</h3> 
                <p> 
                    Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin et felis eget 
                    tellus pharetra porttitor. Praesent dui arcu, egestas quis, adipiscing a. 
                </p> 
                <span class="blue">140 sec</span> 
            </div> 
    </div> 
</div> 
<!-- next link --> 
<a class="next"></a> 
<!-- let rest of the page float normally --> 
<br clear="all" /> 
<div> 
    <input type="button" value="remove" id="remove"/> 
</div> 
<script type="text/javascript"><!-- 
$(function() {         
    $("div.scrollable").scrollable({ 
        size: 3, 
        vertical:false, 
        //clickable:false, 
        loop:true,//设置是否自动跳转（根据间隔时间） 
        //interval: 1000,//设置间歇时间间隔 
        //speed:2000, 
        items: '#thumbs', 
        //prev:'.prev',//跳转到上一项 
        //next:'.next'//跳转到下一项 
        prevPage:'.prev',//跳转到上一页 
        nextPage:'.next',//跳转到下一页 
        hoverClass: 'hover', 
        easing:'linear' 
    });     
    var scrollable=$("div.scrollable").scrollable(); 
    //alert(scrollable.getConf().prev);//获取配置对象中的prev属性 
    scrollable.getConf().speed=200;//设置配置对象的speed属性 
    //alert(scrollable.getIndex());//获取当前滚动项的索引 
    //alert(scrollable.getItems().length);//获取当前滚动项的数量 
    //alert(scrollable.getItemWrap().html());//获取包含滚动项的节点(class=scrollable)，并将所有滚动项显示出来 
    //alert(scrollable.getPageAmount());//获取当前滚动栏分页数 
    //alert(scrollable.getPageIndex());//获取当前所在分页 
    //alert(scrollable.getRoot().html());//获取滚动项的上一级节点（id=thumbs） 
    //alert(scrollable.getSize()); 
    //alert(scrollable.getVisibleItems().length);//获取当前可见滚动项数量 
    scrollable.next();//如果有下一个滚动项，则跳转到下一个滚动项 
    scrollable.prev(3000,function(){return true});//跳转到前一滚动项 
    //var seekTo= scrollable.click(0).seekTo(2,1000,function(){ 
        //alert(this.getIndex()); 
    //}); 
    //scrollable.move(2); 
    //scrollable.prevPage();//跳转到前一页 
    //scrollable.nextPage();//跳转到下一页 
    //scrollable.setPage(1);//跳转到下一页 
    //scrollable.begin();//跳转到第一个滚动项 
    //scrollable.end();//跳转到最后一个滚动项 
    scrollable.click(3);//使第四个滚动项处于选中状态 
    scrollable.onBeforeSeek(function(){ 
            alert("you click the "+this.getIndex()+"st scrollable item!"); 
    }); 
    $("#remove").click(function(){ 
        scrollable.getItems().filter(":last").remove();//删除最后一个滚动项 
        scrollable.reload().prev();//自动更新相关配置信息，并跳转到被删除滚动项的前一项 
    }); 
}); 
// --></script>
```


