# EXT核心API详解(一)-Ext - weixin_33985507的博客 - CSDN博客
2008年05月23日 21:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Ext类
addBehaviors( Object obj ) : void
对选择的元素 应用事件侦听器,事件名以@附加在结尾,例如
addBehaviors({
   // id=foo下所有的a标签加入click事件
   '#foo a@click' : function(e, t){
       // do something
   },
   // 用,分隔多个选择器
   '#foo a, #bar span.some-class@mouseover' : function(){
       // do something
   }
});
apply( Object obj, Object config, Object defaults ) : Object
从config拷贝所有的属性到obj,如果有defaults参数,也将拷贝其属性到obj
applyIf( Object obj, Object config ) : Object
从config拷贝所有属性至obj(如果obj未定义相应属性)
decode(Object obj) : String
编码一个对象/数组或其它值为一个json格式字符串(支持对象,数组,日期,字符串)
destroy( Mixed arg1, Mixed (optional), Mixed (optional) ) : void
尝试去移除每个传入的对象,包括DOM,事件侦听者,并呼叫他们的destroy方法(如果存在)
each( Array/NodeList/Mixed array, Function fn, Object scope ) : void
利用array中的每个元素迭代调用fn.直到fn返回假,调用格式fn.call(scope || array[i], array[i], i, array);
encode(String json) : Object
将一个json格式字符串反序列化为对象
escapeRe( String str ) : String
为字符串正则编码将.在*+?^${}()|[]/\字符前加\
extend( Object subclass, Object superclass, [Object overrides] ) : void
从superclass类继承subclass,overrides参数是要重载的方法列表,详见override
fly( String/HTMLElement el, [String named] ) : Element
得到全局共享的一个浮动元素el,如果有多个浮动元素可以使用命名以避免潜在的冲突
get( Mixed el ) : Element
得到一个Element对象,参数可以是id或DOM节点或已存在的Element对象
getBody() : Element
得到当前文档的body对象
getCmp( String id ) : Component
通过id得到一个Component对象
getDoc() : Element
得到当前文档
getDom( Mixed el ) : HTMLElement
通过id或节点或Element对象返回一个DOM节点
id( [Mixed el], [String prefix] ) : String
为对象el生成一个惟一id,如果对象本身已有id,将不会被改变(可以使用前导符prefix)
isEmpty( Mixed value, [Boolean allowBlank] ) : Boolean
判断值是否为空,null, undefined或空字符串将返回真,如果allowBlank定义为真,空字符串将不被视为空
namespace( String namespace1, String namespace2, String etc ) : void
创建一个命名空间,例
Ext.namespace('Company', 'Company.data');
Company.Widget = function() { ... }
Company.data.CustomStore = function(config) { ... }
num( Mixed value, Number defaultValue ) : Number
将value转为数值,如果value不是合法的数值类型,将返回defaultValue,其实defaultValue并不一定要求是数值类型,只要你愿意
onReady( Function fn, Object scope, boolean override ) : void
当文档准备好了之后引发fn,发生在page的onload事件之前,并且图片此时都还没有载入,scope定义执有者,override定义scope是否有默认的选择
override( Object origclass, Object overrides ) : void
利用overrides重写origclass的方法,例
Ext.override(MyClass, {
    newMethod1: function(){
        // etc.
    },
    newMethod2: function(foo){
        // etc.
    }
});
query( String path, [Node root] ) : Array
通过path,在root中选择节点数组,path可以是以下四种选择器之一
[元素选择器] 例:
* 任意节点
E 一个E标签元素
E F 祖先节点为E的F节点
E > F 或 E/F 父节点为E的F节点
E + F 前一个兄弟节点为E的F节点
E ~ F 前面的兄弟节点中有E的F节点
[属性选择器] 例:
E[foo] 有foo属性的E节点
E[foo=bar] 有foo属性值为bar的E节点
E[foo^=bar] foo属性以bar开始的E节点
E[foo$=bar] foo属性以bar结尾的E节点
E[foo*=bar] foo属性中有bar字符串的E节点
E[foo%=2]  foo属性能被2整除的E节点
E[foo!=bar] foo属性值不为bar的E节点
[伪类选择器] 例:
E:first-child E节点是父节点中第一个子节点
E:last-child E节点是父节点中最后一个子节点
E:nth-child(n) E是父节点中每n个节点
E:nth-child(odd) E是父节点中的奇数节点
E:nth-child(even) E是父节点中的偶数节点
E:only-child E是父节点中惟一的子节点
E:checked checked属性为真的节点
E:first 子孙节点中的第一个E节点
E:last  子孙节点中的最后一个E节点
E:nth(n) 子孙节点中的第n个E节点
E:odd E:nth-child(odd)的简写
E:even E:nth-child(even)的简写
E:contains(foo)  innerHTML属性中含有foo的E节点
E:nodeValue(foo) E节点中包含一个值为foo的文本节点
E:not(S) 不匹配简单选择器S的E节点
E:has(S) 有能匹配简单选择器S的子节点的E节点
E:next(S) 下一个兄弟节匹配简单选择器S的E节点
E:prev(S) 前一个兄弟节匹配简单选择器S的E节点
type( Mixed object ) : String
判断对象类型,如果不是下列值之一将返回false
[样式选择器] 例:
E{display=none} display属性值为none的E节点
E{display^=none} display属性值以none开始的E节点
E{display$=none} display属性值以none结束的E节点
E{display*=none} display属性值含有none子字串的E节点
E{display%=2} display属性值能被2整除的E节点
E{display!=none} display属性值不等于none的E节点
select( String/Array selector, [Boolean unique], [HTMLElement/String root] ) :CompositeElementLite/CompositeElement
在root内通过样式选择器或元素数组selector选择元素或元素列表,unique指示是否只选择惟一值
urlDecode( String string, [Boolean overwrite] ) : Object
将url格式字符串string解码为json对象,overwrite参数指示是否不重写已存在的对象,例
Ext.urlDecode("foo=1&bar=2"); //返回 {foo: 1, bar: 2} 
Ext.urlDecode("foo=1&bar=2&bar=3&bar=4"); //返回 {foo: 1, bar: 4}.
Ext.urlDecode("foo=1&bar=2&bar=3&bar=4", true); //返回 {foo: 1, bar: [2, 3, 4]}.
urlEncode( Object o ) : String
将json对象编码为url格式字符串,参见urlDecode
type( Mixed object ) : String
得到object类型,如果不为以下列举值之一则返回false
string/number/boolean/function/object/array/regexp/element/nodelist/textnode/whitespace
