# 如何通过jQuery实现select下拉框的联动效果 - Big Smile - CSDN博客
2017年12月10日 21:39:51[王啸tr1912](https://me.csdn.net/tr1912)阅读数：6731标签：[jquery																[select](https://so.csdn.net/so/search/s.do?q=select&t=blog)](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/tr1912/article/category/6400625)
        由于项目需要，小编需要实现三个HTML的select下拉框联动的效果，如果考虑到实际用户体验的话，直接用异步刷新的方式操作DOM是最好的选择，这里小编直接使用了jQuery作为DOM操作的工具，直接操作，完成了对于下拉框的操作，下面是实现方法：
# 一、页面效果
        如图，有这么两个下拉框，当选择上面的类型的时候，下面的动物种类就会跟着去变化：
![](https://img-blog.csdn.net/20171210210452223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)这里是牛的分类![](https://img-blog.csdn.net/20171210210458402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)这里是羊的分类
        然后，我们来看一下JSP页面的select代码
```
<div class="form-group">
      <label for="packType" class="col-sm-3 control-label">所属类型：</label>
      <div class="col-sm-4">                            
            <select id="packType" name="packType" class="form-control">
		<option value="0">请选择</option>
		<option value="1" <c:if test="${cmnPack.packType == 1}">selected="selected"</c:if>>宠物羊</option>
		<option value="2" <c:if test="${cmnPack.packType == 2}">selected="selected"</c:if>>宠物牛</option>
	    </select>
     </div>
</div>
```
这个是类型选择的
```
<div class="form-group">
   <label for="animalClassifyId" class="col-sm-3 control-label">动物分类:</label>
   <div class="col-sm-4">
       <select id="animalClassifyId" name="animalClassifyId" class="form-control input-150 input-sm" placeholder="动物分类">
               <option value="">选择动物分类</option>
               <c:if test="${classify != null}">
			<c:forEach items="${classify}" var="c"><option value="${c.animalClassifyId}" <c:if test="${cmnPack.packType == c.animalClassifyId}">selected="selected"</c:if>>${c.classifyName}</option></c:forEach>
		</c:if>
	</select>
   </div>
</div>
```
这个是动物分类的
# 二、JS代码实现
        由于我们的整个事件触发点是类型的选取，所以我在网上查了查关于select的改变选项的事件，于是发现了change这个事件，那么我们就以这个为起点来展示一下实现的过程：
      1、首先，我们看到了选择类型的select的ID是packType，所以我们需要在document的ready里面给这个select写入一个触发事件：
     先上全部的代码：
```java
$(document).ready(function() {
$("#packType").bind("change",function(){
        var typeId = $(this).val();        
        var url = BASE_URL + "/back/cmnPack/findClassifyByType";
        $.post(url,{"typeId":typeId},function(rd){
            if(rd==undefined){
                layer.msg("未查询到动物分类数据！");
            }else{
            	empty_animalClassify();
            	empty_manageFee();
            	$('.price').text('');
            	//input框清空
            	$("#appointDiscount").val("");
            	var dataObj=eval("("+rd+")");//转换为json对象 
            	for(var i=0;i<dataObj.length;i++){
            		//更新羊的分类
            		var animalClassifyId=dataObj[i].animalClassifyId;
            		var classifyName=dataObj[i].classifyName;
            		$("#animalClassifyId").append("<option value='"+animalClassifyId+"'>"+classifyName+"</option>");
            	}                
            }
        })        
});
}
```
     2、我们通过这个触发事件，先获得我们的分类ID：
```java
var typeId = $(this).val();
```
然后通过Ajax的方法，把这个ID传入到后台去取我们需要更新的数据，也就是这个jQuery带的Ajax代码：
```java
$.post(url,{"typeId":typeId},function(rd){
```
注意：URL里面的BASE_URL是我们在common.js中定义的静态变量。
     3、获得到数据之后，我们需要先进行动物分类的select的清空操作，empty_animalClassify();这个方法就是清空用的，具体代码如下：
```java
function empty_animalClassify(){
	$("#animalClassifyId").empty();
	$("#animalClassifyId").append("<option value=''>选择动物分类</option>");
}
```
     4、获得到数据之后，经过简单的判断，我们可以对于回传的json数据进行解析了方法就是用的eval对于回传的数据先对象化，然后进行对象的操作：
```java
var dataObj=eval("("+rd+")");//转换为json对象 
     for(var i=0;i<dataObj.length;i++){
        //更新羊的分类
        var animalClassifyId=dataObj[i].animalClassifyId;
        var classifyName=dataObj[i].classifyName;
        $("#animalClassifyId").append("<option value='"+animalClassifyId+"'>"+classifyName+"</option>");
}
```
       其中animalClassifyId为动物分类的select框的ID，我们在这里给他动态添加选项，也就是option，这样之后，我们的动态联动也就做好了。
**需要思考的：**
      其实经历了这次的探索，让我对于jQuery有了新的认识，因为基本所有的操作，包括添加属性，取值，改属性，都可以通过它来完成，并且经过在网上查阅，还看到了同样的绑定有不同的写法，这一点是很灵活的，所以下一篇博客我将重点分析一下jQuery对于一个对象的属性添加，赋值等操作的不同方法，以及各有什么好处。

