# js 对select 操作 - weixin_33985507的博客 - CSDN博客
2010年07月27日 12:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
简介：这是Javascript  select控件操作大全（新增、修改、删除、选中、清空、判断存在等）的详细页面，介绍了和表单,有关的知识，加入收藏请按键盘ctrl+D，谢谢大家 的观看！要查看更多有关信息，[请点击此处](http://www.dnbcw.com/biancheng/biaodan)
 1判断select选项中 是否存在Value="paraValue"的Item 
2向select选项中 加入一个Item 
3从 select选项中 删除一个Item 
4删除select中选中的项 
5修改select选项中  value="paraValue"的text为"paraText" 
6设置select中text="paraText"的第一个Item为 选中 
7设置select中value="paraValue"的Item为选中 
8得到select的当前选中项的value 
9 得到select的当前选中项的text 
10得到select的当前选中项的Index 
11清空select的项 
js  代码 
// 1.判断select选项中 是否存在Value="paraValue"的Item 
function  jsSelectIsExitItem(objSelect, objItemValue) { 
var isExit = false; 
for  (var i = 0; i < objSelect.options.length; i++) { 
if  (objSelect.options[i].value == objItemValue) { 
isExit = true; 
break;  
} 
} 
return isExit; 
} 
// 2.向select选项中 加入一个Item 
function  jsAddItemToSelect(objSelect, objItemText, objItemValue) { 
//判断是否存在 
if  (jsSelectIsExitItem(objSelect, objItemValue)) { 
alert("该Item的Value值 已经存在"); 
} else { 
var varItem = new Option(objItemText,  objItemValue); 
objSelect.options.add(varItem); 
alert("成功加入"); 
}  
} 
// 3.从select选项中 删除一个Item 
function  jsRemoveItemFromSelect(objSelect, objItemValue) { 
//判断是否存在 
if  (jsSelectIsExitItem(objSelect, objItemValue)) { 
for (var i = 0; i  < objSelect.options.length; i++) { 
if (objSelect.options[i].value  == objItemValue) { 
objSelect.options.remove(i); 
break; 
} 
}  
alert("成功删除"); 
} else { 
alert("该select中 不存在该项"); 
} 
}  
// 4.删除select中选中的项 
function  jsRemoveSelectedItemFromSelect(objSelect) { 
var length =  objSelect.options.length - 1; 
for(var i = length; i >= 0; i--){ 
if(objSelect[i].selected  == true){ 
objSelect.options[i] = null; 
} 
} 
} 
//  5.修改select选项中 value="paraValue"的text为"paraText" 
function  jsUpdateItemToSelect(objSelect, objItemText, objItemValue) { 
//判断是否存 在 
if (jsSelectIsExitItem(objSelect, objItemValue)) { 
for (var i =  0; i < objSelect.options.length; i++) { 
if  (objSelect.options[i].value == objItemValue) { 
objSelect.options[i].text  = objItemText; 
break; 
} 
} 
alert("成功修改"); 
} else { 
alert(" 该select中 不存在该项"); 
} 
} 
//  6.设置select中text="paraText"的第一个Item为选中 
function  jsSelectItemByValue(objSelect, objItemText) { 
//判断是否存在 
var  isExit = false; 
for (var i = 0; i < objSelect.options.length;  i++) { 
if (objSelect.options[i].text == objItemText) { 
objSelect.options[i].selected  = true; 
isExit = true; 
break; 
} 
} 
//Show出结果 
if  (isExit) { 
alert("成功选中"); 
} else { 
alert("该select中 不存在该项"); 
}  
} 
// 7.设置select中value="paraValue"的Item为选中 
document.all.objSelect.value  = objItemValue; 
// 8.得到select的当前选中项的value 
var currSelectValue =  document.all.objSelect.value; 
// 9.得到select的当前选中项的text 
var  currSelectText =  document.all.objSelect.options[document.all.objSelect.selectedIndex].text;  
// 10.得到select的当前选中项的Index 
var currSelectIndex =  document.all.objSelect.selectedIndex; 
// 11.清空select的项 
document.all.objSelect.options.length  = 0;
