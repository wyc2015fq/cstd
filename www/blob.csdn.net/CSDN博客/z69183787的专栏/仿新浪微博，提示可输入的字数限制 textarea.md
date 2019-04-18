# 仿新浪微博，提示可输入的字数限制 textarea - z69183787的专栏 - CSDN博客
2013年12月11日 21:28:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3677
js：
```java
var txtobj={
   divName:"area", //外层容器的class
   textareaName:"chackTextarea", //textarea的class
   numName:"num", //数字的class
   num:140 //数字的最大数目
  }
  
  var textareaFn=function(){
   //定义变量
   var $onthis;//指向当前
   var $divname=txtobj.divName; //外层容器的class
   var $textareaName=txtobj.textareaName; //textarea的class
   var $numName=txtobj.numName; //数字的class
   var $num=txtobj.num; //数字的最大数目
   
   function isChinese(str){  //判断是不是中文
    var reCh=/[u00-uff]/;
    return !reCh.test(str);
   }
   function numChange(){
    var strlen=0; //初始定义长度为0
    var txtval = $.trim($onthis.val());
    for(var i=0;i<txtval.length;i++){
     if(isChinese(txtval.charAt(i))==true){
      strlen=strlen+2;//中文为2个字符
     }else{
      strlen=strlen+1;//英文一个字符
     }
    }
    strlen=Math.ceil(strlen/2);//中英文相加除2取整数
    if($num-strlen<0){
     $par.html("超出 <b style='display:inline;color:red;font-weight:lighter' class="+$numName+">"+Math.abs($num-strlen)+"</b> 字"); //超出的样式
    }
    else{
     $par.html("还可以输入 <b style='display:inline;' class="+$numName+">"+($num-strlen)+"</b> 字"); //正常时候
    }
    $b.html($num-strlen);   
    $("#areaLength").val(strlen);
   }
   $("."+$textareaName).live("focus",function(){
    $b=$(this).parents("."+$divname).find("."+$numName); //获取当前的数字
    $par=$b.parent(); 
    $onthis=$(this); //获取当前的textarea
    var setNum=setInterval(numChange,500);    
   });
  }     
  textareaFn();
```
```
<div class="area">
				<div class="box01-num">
					<input type="hidden" name="areaLength" id="areaLength"/>
					<p>你还可以输入<b style="display:inline;" class="num">140</b>字</p>
				</div>
				<div class="inputarea">
					 <textarea class="chackTextarea" id="content" name="content" rows="15" ><s:property value='msgBo.content'/></textarea>
				</div>
			</div>
```
