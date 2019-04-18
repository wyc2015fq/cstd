# 【jQuery插件】insertContent-在文本框光标位置插入内容并选中 - z69183787的专栏 - CSDN博客
2013年10月12日 10:34:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5193
在文本框光标位置插入内容在实际的项目应用中经常用到，比如在文本框插入表情，首先要获取光标在文本框中的位置，当然这个有浏览器兼容性问题。
IE下可以通过document.selection.createRange();获取光标位置，代码也很简单：
- if(document.selection){
-     pos = document.selection.createRange();
-     pos.text ="要插入的字符串";
- }
Firefox浏览器则稍微负责一点，需要首先获取光标位置，然后对value进行字符串截取处理:- if(obj.selectionStart || obj.selectionStart =='0'){//obj是文本框对象
- var startPos = obj.selectionStart;
- var endPos = obj.selectionEnd;
-     obj.value = obj.value.substring(0, startPos)+"要插入的字符串"+ obj.value.substring(endPos, obj.value.length);
-     obj.selectionStart = startPos + myValue.length;
-     obj.selectionEnd = startPos + myValue.length;
- }else{
-     obj.value +="要插入的字符串";
- }
现在微博很火，我们可以尝试一下插入话题，你点击话题，在光标位置就会插入：#请在这里输入自定义话题#，注意，“请在这里输入自定义话题”这几文字是被选中，你可以直接敲打键盘编辑文本。在火狐下先对比较简单，我们只要在上面的代码中插入一句就可以了：- obj.setSelectionRange(startPos-t,obj.selectionEnd+t);
t是根据数值选中插入文本内容两边的边界数值，如：0是表示插入文字全部选择，-1表示插入文字两边各少选中一个字符。
但是在ie下比较麻烦，要设置光标的位置，然后选中文字。
完整代码如下：- (function($){http://www.css88.com/archives/3627
-     $.fn.insertContent =function(myValue, t){
- var $t = $(this)[0];
- if(document.selection){//ie
- this.focus();
- var sel = document.selection.createRange();
- 			sel.text = myValue;
- this.focus();
- 			sel.moveStart('character',-l);
- var wee = sel.text.length;
- if(arguments.length ==2){
- var l = $t.value.length;
- 				sel.moveEnd("character", wee + t);
- 				t <=0? sel.moveStart("character", wee -2* t - myValue.length): sel.moveStart("character", wee - t - myValue.length);
- 
- 				sel.select();
- }
- }elseif($t.selectionStart || $t.selectionStart =='0'){
- var startPos = $t.selectionStart;
- var endPos = $t.selectionEnd;
- var scrollTop = $t.scrollTop;
- 			$t.value = $t.value.substring(0, startPos)+ myValue + $t.value.substring(endPos, $t.value.length);
- this.focus();
- 			$t.selectionStart = startPos + myValue.length;
- 			$t.selectionEnd = startPos + myValue.length;
- 			$t.scrollTop = scrollTop;
- if(arguments.length ==2){
- 				$t.setSelectionRange(startPos - t, $t.selectionEnd + t);
- this.focus();
- }
- }
- else{
- this.value += myValue;
- this.focus();
- }
- };
- })(jQuery);
调用方式：- $(文本域选择器).insertContent("插入的内容");
- //根据数值选中插入文本内容两边的边界
- $(文本域选择器).insertContent("插入的内容"，数值);
效果图：
![](http://www.oseye.net/upload/attached/image/20121220/20121220094548_26275.jpg)
