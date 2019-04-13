
# 1.18 JQuery1：动态操作HTML和CSS - 尹成的技术博客 - CSDN博客

2018年11月11日 17:04:25[尹成](https://me.csdn.net/yincheng01)阅读数：41个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



# jquery选择器
### jquery用法思想一
选择某个网页元素，然后对它进行某种操作
### jquery选择器
jquery选择器可以快速地选择元素，选择规则和css样式相同，使用length属性判断是否选择成功。
```python
$(document) //选择整个文档对象
$('li') //选择所有的li元素
$('#myId') //选择id为myId的网页元素
$('.myClass') // 选择class为myClass的元素
$('input[name=first]') // 选择name属性等于first的input元素
$('#ul1 li span') //选择id为为ul1元素下的所有li下的span元素
```
### 对选择集进行修饰过滤(类似CSS伪类)
```python
$('#ul1 li:first') //选择id为ul1元素下的第一个li
$('#ul1 li:odd') //选择id为ul1元素下的li的奇数行
$('#ul1 li:eq(2)') //选择id为ul1元素下的第3个li
$('#ul1 li:gt(2)') // 选择id为ul1元素下的前三个之后的li
$('#myForm :input') // 选择表单中的input元素
$('div:visible') //选择可见的div元素
```
**对选择集进行函数过滤**
```python
$('div').has('p'); // 选择包含p元素的div元素
$('div').not('.myClass'); //选择class不等于myClass的div元素
$('div').filter('.myClass'); //选择class等于myClass的div元素
$('div').first(); //选择第1个div元素
$('div').eq(5); //选择第6个div元素
```
**选择集转移**
```python
$('div').prev('p'); //选择div元素前面的第一个p元素
$('div').next('p'); //选择div元素后面的第一个p元素
$('div').closest('form'); //选择离div最近的那个form父元素
$('div').parent(); //选择div的父元素
$('div').children(); //选择div的所有子元素
$('div').siblings(); //选择div的同级元素
$('div').find('.myClass'); //选择div内的class等于myClass的元素
```

# jquery属性操作
1、html() 取出或设置html内容
```python
// 取出html内容
var $htm = $('#div1').html();
// 设置html内容
$('#div1').html('<span>添加文字</span>');
```
2、text() 取出或设置text内容
```python
// 取出文本内容
var $htm = $('#div1').text();
// 设置文本内容
$('#div1').text('<span>添加文字</span>');
```
3、attr() 取出或设置某个属性的值
```python
// 取出图片的地址
var $src = $('#img1').attr('src');
// 设置图片的地址和alt属性
$('#img1').attr({ src: "test.jpg", alt: "Test Image" });
```

# jquery样式操作
### jquery用法思想二
同一个函数完成取值和赋值
### 操作行间样式
```python
// 获取div的样式
$("div").css("width");
$("div").css("color");
//设置div的样式
$("div").css("width","30px");
$("div").css("height","30px");
$("div").css({fontSize:"30px",color:"red"});
```

### 特别注意
选择器获取的多个元素，获取信息获取的是第一个，比如：$(“div”).css(“width”)，获取的是第一个div的width。
### 操作样式类名
```python
$("#div1").addClass("divClass2") //为id为div1的对象追加样式divClass2
$("#div1").removeClass("divClass")  //移除id为div1的对象的class名为divClass的样式
$("#div1").removeClass("divClass divClass2") //移除多个样式
$("#div1").toggleClass("anotherClass") //重复切换anotherClass样式
```
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/337650?tuin=63946d38)
扫码获取海量视频及源码   QQ群：721929980
![](https://img-blog.csdnimg.cn/20181108132958856.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

