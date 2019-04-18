# mass Framework placeholder插件 - weixin_33985507的博客 - CSDN博客
2012年02月17日 21:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
由于基础库已大体完成，从今天隔三差四就开发一个插件出来。今天给大家介绍的placeholder插件。由于jQuery已有几个这样的插件，下回来研究一下，很快就搞出自己的插件。
![o_20120217_placeholder.jpg](https://images.cnblogs.com/cnblogs_com/rubylouvre/199042/o_20120217_placeholder.jpg)
目前jQuery中最好的jQuery当属 danielstocks的[jQuery-Placeholder](https://github.com/danielstocks/jQuery-Placeholder)，但细看还是有许多改进。它的思路是这样，如果浏览器已经原生支持HTML5的placeholder就立即返回jQuery实例，不做任何修改，要用户自行设置placeholder。如果不支持，分两种情况。如果不是密码框，它就会在这input或textarea控件添加一个类名placeholder，目的是让输入字体变淡，当然这类名只会在输入框里面没有内容时才添加，并将用户设置的placeholder属性的值，赋到value中去。其他就是一系列行为模拟了。首先，当输入框获得焦点时，就检测里面的内容是否等于placeholder，如果不相等，说明用户正在输入，要清除placeholder类名，并将用户内容回填过去。如果失去焦点，又检测里面的内容，如果为空，则再添加placeholder类名，与将用户设置的placeholder属性的值，赋到value中去。为此danielstocks特地写了一个Placeholder类，来处理这些行为。此外，用户提交表单时或页面刷新时，还要做相应处理。大家听我说到这里，头都大了吧。嗯，是非常复杂。我还没有说密码框的情况呢。密码框还分两种，一种是可能修改其type属性，一种不能（因为IE678不充许修改）。我的整个思路就是建立密码框不能修改的情况下，免去这么多分支。下面是我的思路，danielstocks的实现自已到github中看源码。
我的想法，即，如果通过在输入框的value值来模拟placeholder，弊端太多，要注意表单提交与IE不能修改type属性什么的，于是决定**只**做一个层蒙在输入框上去。当用户输入时就让浮层隐去，如果离开时，发现里面没有内容就浮出来。同时我也不管focus, blur等不能冒泡的事件，我只绑定click事件（当用户点到浮层上时），input事件（用户输入时，向前消去内容会有可能重新让浮层出现），mouseout事件（用户鼠标离开输入框时）。首先是浮层，这个浮层我决定用一个不常用的HTML元素kbd来模拟，然后动态生成插入到输入框的后面，并设法让它定位到输入框的左上角。
    function fix(input, node, val) {//node = input[0]
        var placeholder = $._data(node,"placeholder");首先判定它是否已插入了浮层
        if(!placeholder){
            placeholder = $("").afterTo(input).css({
                position: "relative",
                left: -1 * (input.innerWidth() + parseFloat(input.css("marginRight"))) ,
                top:  -1 * parseFloat(input.css("paddingTop")),
                display: "inline-block",
                w: input.width() - 4 ,
                h: input.height(),
                border: 'none',
                cursor: 'text',
                bgc: input.css("bgc"),//背景
                c: "#808080",
                "font-weight": input.css("font-weight"),
                "font-size": input.css("font-size")      
            });
            $._data( node,"placeholder", placeholder );
        }
        return  placeholder.text(val);
    }
然后是绑定事件，click事件直接绑到刚才kbd元素上，mouseout,input则绑到输入框上。由于不知用户是否已在输入框绑定了事件，因此我们移除事件时必须传入回调函数的引用。不过这样也好，那么我们所有输入框的mouseout回调与input回调都共用一个了。
input.bind("mouseout,input",callback);
    var NATIVE_SUPPORT = !!("placeholder" in document.createElement( "input" ));
    $.fn.placeholder = function(val) {
        return this.each(function() {
            if( NATIVE_SUPPORT ){
                this.setAttribute("placeholder", val)
            }else{
                var input = $(this);
                var placeholder = fix(input, this, val );
                placeholder.css("display" , (input.val() ? "none" : "inline-block"))
                placeholder.click(function(){
                    placeholder.css("display","none" );
                });
                input.bind("mouseout,input",callback);
            }
        });
    }
共用回调函数：
function callback( e ){
    var placeholder = $._data( this,"placeholder");
    if( placeholder ){
        placeholder.css("display" , this.value ? "none" : "inline-block");
        if(!this.value ){
            this.focus();
        }
    }
}
它的用法很简单，比如
$("#password").placeholder("请输入密码")
如果对样式不满意，我们可以直接使用$._data( input,"placeholder")得到kbd的mass实例来设置样式，而标准浏览器下只有webkit系统可以修改样式，使用::-webkit-input-placeholder伪元素。
除此以后，它还有对应的解绑方法:**unplaceholder**
    $.fn.unplaceholder = function( ){
        return this.each(function() {
            if( NATIVE_SUPPORT ){
                this.setAttribute("placeholder", "");
            }else{
                var placeholder =  $._data( this,"placeholder")
                if( placeholder ){
                    var input = $(this);
                    input.unbind("mouseout,input",callback);
                    input.removeData("placeholder",true);
                    placeholder.unbind("click")
                    placeholder.remove();
                }
            }
        });
    }
下面就是完整例子：
IE下如果报错，请它刷新页面，再不行，下载回来看。
 
<!DOCTYPE html>
<html>
    <head>
        <meta charset="utf-8" />
        <title>placeholder by 司徒正美</title>
        <style type="text/css">
        </style>
        <script type="text/javascript" src="http://files.cnblogs.com/rubylouvre/mass.js">
        </script>
        <script type="text/javascript">
           window.onload = function(){
           $.require("ready,20120217_placeholder",function(){
                $("input").placeholder("请输入密码")
            })
           }
        </script>
    </head>
    <body>
        <input type="password">
    </body>
</html>
运行代码
我的placeholder的完整源码，请到[这里](https://github.com/RubyLouvre/mass-Framework/blob/master/client/more/placeholder.js)看。
