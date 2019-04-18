# jquery获得select option的值 和对select option的操作 - z69183787的专栏 - CSDN博客
2012年10月23日 22:32:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2198
个人分类：[Javascript-Jquery](https://blog.csdn.net/z69183787/article/category/2175111)
jQuery获取Select元素，并选择的Text和Value: 
1. $("#select_id").change(function(){//code...});   //为Select添加事件，当选择其中一项时触发 
2. var checkText=$("#select_id").find("option:selected").text();  //获取Select选择的Text 
3. var checkValue=$("#select_id").val();  //获取Select选择的Value 
4. var checkIndex=$("#select_id ").get(0).selectedIndex;  //获取Select选择的索引值 
5. var maxIndex=$("#select_id option:last").attr("index");  //获取Select最大的索引值 
jQuery获取Select元素，并设置的 Text和Value: 
实例分析： 
1. $("#select_id ").get(0).selectedIndex=1;  //设置Select索引值为1的项选中 
2. $("#select_id ").val(4);   // 设置Select的Value值为4的项选中 
3. $("#select_id option[text='jQuery']").attr("selected", true);   //设置Select的Text值为jQuery的项选中 
jQuery添加/删除Select元素的Option项： 
实例分析： 
1. $("#select_id").append("<option value='Value'>Text</option>");  //为Select追加一个Option(下拉项) 
2. $("#select_id").prepend("<option value='0'>请选择</option>");  //为Select插入一个Option(第一个位置) 
3. $("#select_id option:last").remove();  //删除Select中索引值最大Option(最后一个) 
4. $("#select_id option[index='0']").remove();  //删除Select中索引值为0的Option(第一个) 
5. $("#select_id option[value='3']").remove();  //删除Select中Value='3'的Option 
6. $("#select_id option[text='4']").remove();  //删除Select中Text='4'的Option 
三级分类 <select name="thirdLevel" id="thirdLevel" 
onchange="getFourthLevel()"> 
<option value="0" id="thirdOption"> 
请选择三级分类 
</option> 
</select> 
</div> 
四级分类: 
<select name="fourthLevelId" id="fourthLevelId"> 
<option value="0" id="fourthOption"> 
请选择四级分类 
</option> 
</select> 
</div> 
.if($("#thirdLevel").val()!=0){ 
$("#thirdLevel option[value!=0]").remove(); 
} 
if($("#fourthLevelId").val()!=0){ 
$("#fourthLevelId option[value!=0]").remove(); 
}//这个表示：假如我们希望当选择选择第三类时：如果第四类中有数据则删除，如果没有数据第四类的商品中的为默认值。在后面学习了AJAX技术后经常会使用到！
获取Select ：
 获取select 选中的 text :
   $("#ddlRegType").find("option:selected").text();
 获取select选中的
 value:
   $("#ddlRegType ").val();
 获取select选中的索引:
     $("#ddlRegType ").get(0).selectedIndex;
设置select:
 设置select
 选中的索引：
     $("#ddlRegType ").get(0).selectedIndex=index;//index为索引值
 设置select
 选中的value：
    $("#ddlRegType ").attr("value","Normal“);
    $("#ddlRegType ").val("Normal");
    $("#ddlRegType ").get(0).value = value;
 设置select
 选中的text:
var count=$("#ddlRegType option").length;
  for(var i=0;i<count;i++)  
     {           if($("#ddlRegType ").get(0).options[i].text == text)  
        {  
            $("#ddlRegType ").get(0).options[i].selected = true;  
            break;  
        }  
    }
$("#select_id option[text='jQuery']").attr("selected", true);
设置select option项:
 $("#select_id").append("<option
 value='Value'>Text</option>");  //添加一项option
 $("#select_id").prepend("<option
 value='0'>请选择</option>"); //在前面插入一项option
 $("#select_id
 option:last").remove(); //删除索引值最大的Option
 $("#select_id
 option[index='0']").remove();//删除索引值为0的Option
 $("#select_id
 option[value='3']").remove(); //删除值为3的Option
 $("#select_id
 option[text='4']").remove(); //删除TEXT值为4的Option
清空 Select:
$("#ddlRegType ").empty();
jquery获得值：
.val()
.text()
设置值 
.val('在这里设置值')
$("document").ready(function(){ 
$("#btn1").click(function(){ 
$("[name='checkbox']").attr("checked",'true');//全选 
}) 
$("#btn2").click(function(){ 
$("[name='checkbox']").removeAttr("checked");//取消全选 
}) 
$("#btn3").click(function(){ 
$("[name='checkbox']:even").attr("checked",'true');//选中所有奇数 
}) 
$("#btn4").click(function(){ 
$("[name='checkbox']").each(function(){//反选 
if($(this).attr("checked")){ 
$(this).removeAttr("checked"); 
} 
else{ 
$(this).attr("checked",'true'); 
} 
}) 
}) 
$("#btn5").click(function(){//输出选中的值 
var str=""; 
$("[name='checkbox'][checked]").each(function(){ 
str+=$(this).val()+"\r\n"; 
//alert($(this).val()); 
}) 
alert(str); 
}) 
}) 
