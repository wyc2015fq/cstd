# jquery全选/取消全选(反选)/单选操作以及获取值常见用法和错误-jquery prop()函数使用教程 - z69183787的专栏 - CSDN博客
2013年11月20日 14:08:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4506
用原生态的JavaScript操作页面上的一组checkbox全选/取消全选，逻辑很简单，实现代码也不难写。但使用jQuery实现则非常简单，代码很简洁-write less,do more！
jquery版本：1.9
先看看HTML代码，很简单的操作框
- 
<div class="checkbox_test">
- 
  <input type="checkbox" name="chk_list" id="chk_list_1" value="1" />1<br />
- 
  <input type="checkbox" name="chk_list" id="chk_list_2" value="2" />2<br />
- 
  <input type="checkbox" name="chk_list" id="chk_list_3" value="3" />3<br />
- 
  <input type="checkbox" name="chk_list" id="chk_list_4" value="4" />4<br />
- 
  <input type="checkbox" name="chk_all" id="chk_all" />全选/取消全选
- 
</div>
引入jquery库
- 
<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
对于checkbox的选择，实际上就是添加了一个属性：checked，不管后面有没有值，都是选中状态，在程序里面，如果checked值纯在，就是true，不存在就是false。于是就可以用下面的这个代码：
- 
//全选/反选
- 
$("#chk_all").on('click', function(){
- 
if($(".chk_all").attr("checked")){    //判断chk_all是否被选中
- 
$("input[name='chk_list']").attr("checked",false); //反选
- 
}else{
- 
$("input[name='chk_list']").attr("checked",true);//全选
- 
}
- 
})
可是，这段运行却是错误的，因为$(“.chk_all”).attr(“checked”)的值是undefined，因为在源代码里面没有这个属性。
查看jquery的api手册，发现了一个prop函数：
—————————-以下函数说明应用自jquery官方api———————————-
prop(name|properties|key,value|fn)
### 概述
获取在匹配的元素集中的第一个元素的属性值。
随着一些内置属性的DOM元素或window对象，如果试图将删除该属性，浏览器可能会产生错误。jQuery第一次分配undefined值的属性，而忽略了浏览器生成的任何错误
### 参数
#### **name**
属性名称
#### **properties**
作为属性的“名/值对”对象
#### **key,value**
属性名称，属性值
#### **key,function(index, attr)**
1:属性名称。
2:返回属性值的函数,第一个参数为当前元素的索引值，第二个参数为原先的属性值。
——————————————以上函数说明应用自jquery官方api————————————–
prop()获取匹配的元素的属性值。
这个方法是jquery1.6以后出来的，用来区别之前的.attr()方法.
区别最大的一点就是：布尔型的属性，1.6以后都是用.prop()方法就好了。
这个布尔型的属性，再解释一下，是属性值只有true|false的属性。
还有种情况就是只添加属性名，不需要写属性值的就可以生效的也同样使用.prop()方法。比如：checked、disable这样的，其实它们说到底还是属于布尔型的属性。
1.添加属性名称该属性就会生效应该使用prop();
2.是有true,false两个属性使用prop();
3.其他则使用attr();
### 那么
上面的代码可以这样写：
- 
//全选/反选
- 
$("#chk_all").on('click', function(){
- 
if($(".chk_all").prop("checked")){    //判断chk_all是否被选中
- 
$("input[name='chk_list']").prop("checked",false); //反选
- 
}else{
- 
$("input[name='chk_list']").prop("checked",true);//全选
- 
}
- 
})
这样修改以后，就不会报错，并且可以运行的很好！
