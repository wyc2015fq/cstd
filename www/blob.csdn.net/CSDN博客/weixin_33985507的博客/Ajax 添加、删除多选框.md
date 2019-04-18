# Ajax 添加、删除多选框 - weixin_33985507的博客 - CSDN博客
2008年12月17日 10:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
<TITLE> JSP+Ajax 添加删除Checkbox实例 example test</TITLE>
<META NAME="Generator" CONTENT="EditPlus">
<META NAME="Author" CONTENT="">
<META NAME="Keywords" CONTENT="">
<META NAME="Description" CONTENT="">
<script type="text/javascript">
function $() {
  var elements = new Array();
  for (var i = 0; i < arguments.length; i++) {
    var element = arguments[i];
    if (typeof element == 'string')
      element = document.getElementById(element);
    if (arguments.length == 1) 
      return element;
    elements.push(element);
  }
  return elements;
}
//cody by jarry;
var emptyCol="",overCol="#f1f1f1",checkedCol="#F4FFF4";
function getAbsoluteLeft( ob ){
 if(!ob){return null;}
   var obj = ob;
   var objLeft = obj .offsetLeft;
   while( obj != null && obj .offsetParent != null && obj .offsetParent.tagName != "BODY" ){
     objLeft += obj .offsetParent.offsetLeft;
     obj = obj .offsetParent;
   }
 return objLeft ;
}
// get absolute TOP position
function getAbsoluteTop( ob ){
 if(!ob){return null;}
 var obj = ob;
 var objTop = obj .offsetTop;
 while( obj != null && obj .offsetParent != null && obj .offsetParent.tagName != "BODY" ){
   objTop += obj .offsetParent.offsetTop;
   obj = obj .offsetParent;
 }
 return objTop ;
}
String.prototype.trim = function()
{ return this.replace(/(^\s*)|(\s*$)/g, "");}
function isExist(child,obj){ 
 //obj has element the child;
   for(var i=0;i<obj.elements.length;i++){
    if(obj.elements[i].tagName.toLowerCase()=="input" && obj.elements[i].type=="checkbox"){
       if(obj.elements[i].value==child)return true;
    }
   }
   return false;
}
function validateURL(url){
 //validateURL
 if(url.search(/^[A-Za-z0-9 -]+\.[A-Za-z0-9]+[\/=\?%\-&_~`@[\]\':+!]*([^<>\"\"])*$/)!=0){
 return false;
 //}else if(url.substring(0,4)=="www."){
 //return false;
 }else{
 return true;
 }
}
function countLength(obj){
 if(obj==null)return;
 var dLen = 0,sLen = 0;
 for(i=0; i< obj.length; i++ ){
   if(obj.charCodeAt(i) > "0" && obj.charCodeAt(i) < "128")
     sLen+=1;
 else
      dLen+=2;
 }
  totalLen = sLen+dLen;
  return totalLen;
}
var strAll=unescape("%u5168%u9009");
var canAll=unescape("%u53D6%u6D88");
function clickchk(obj){
var allcheck=true;
   for(var i=0;i<obj.elements.length;i++){
    var eli = obj.elements[i];
    if(eli.tagName.toLowerCase()=="input" && eli.type=="checkbox"){
   eli.parentNode.style.backgroundColor = eli.checked ? checkedCol :"";
      if(eli.checked==false){allcheck=false;}
     }
  }
  obj.elements["check"].value=allcheck?canAll:strAll;  
}
function addSite(obj){
  var url=obj.url.value.trim();
  if(url.match(/^\s*$/g) || !validateURL(url)){alert("请输入正确网址：如mysite.com");obj.url.focus();return;}
  if(isExist(url,obj)){alert("你添加的网址已经存在列表中");return;}
  var list=document.getElementById("list");
  var chkbox=document.createElement("input");
  chkbox.type="checkbox";
  chkbox.onclick=function(){clickchk(obj);}//全部选择后check按钮显示取消;
  chkbox.value=url;
  chkbox.name="url_chkbox";
  var hr=document.createElement("hr");
  hr.size="0";hr.style.borderTop="1px solid gray";
  var txt=document.createTextNode(url);  
  var div=document.createElement("div");
  div.appendChild(chkbox);
  div.appendChild(txt);
  div.style.borderBottom="1px dashed gray";
  div.style.padding="5px";
  //div.appendChild(hr);
  //div.style.backgroundColor = "olive";
  div.onmouseover=function(){
   showDelete(this);
  }
  div.onmouseout=function(){
   hideDelete(this);
  }
  list.appendChild(div);
//end add;
//如果已经添加了多选框则去掉禁用;
  if(obj.check.disabled==true){
    obj.check.disabled=false;
    obj.recheck.disabled = false;
    obj.remove.disabled=false;
    }
return false;
}
function checkall(obj){//全选or取消全选;
 var chk=obj.elements["check"];
   for(var i=0;i<obj.elements.length;i++){
    if(obj.elements[i].tagName.toLowerCase()=="input" && obj.elements[i].type=="checkbox"){
      if(chk.value==strAll){//check all checkbox input;
      obj.elements[i].checked=true;
   obj.elements[i].parentNode.style.backgroundColor = checkedCol;
      }else{
       obj.elements[i].checked=false;
    obj.elements[i].parentNode.style.backgroundColor = emptyCol;
      }
    }
  } 
  if($('list').childNodes.length<=1)return;
  chk.value=chk.value==canAll?strAll:canAll;
  //更改全选/取消的显示;
}
function reverseCheck(obj){
   var chk=obj.elements["check"];
   for(var i=0;i<obj.elements.length;i++){
    if(obj.elements[i].tagName.toLowerCase()=="input" && obj.elements[i].type=="checkbox"){
      obj.elements[i].checked = obj.elements[i].checked ? false : true;
    }
    clickchk(obj);
  } 
}
function del(list,obj){
//删除list的内容
  var candel=false;
  var i=list.childNodes.length;
  var end = -1;
   while(  i > end ){  //如果存在child
     var  s=list.childNodes[i];
  if(typeof(s)!="undefined"  && s.tagName){
         try{ //如果child的类型不为空;
          var tgname = s.tagName.toUpperCase();
    if(tgname != "DIV" || s.childNodes.length <=0 )return;
    //子集是div且还有子集
                var s1 = s.childNodes[0];
    if( s1.type.toLowerCase() =="checkbox" && s1.checked==true ){
    //如果该子集里的checkbox选中,那么删除改子集(div);
     candel = true;  
     list.removeChild(s);
      }   
          }catch(ex){ 
           //alert(ex.toString());         
         }
       }
      i--;
   }
   if(candel==false){alert("请选择你要删除的选项")}
isDisabledButton();
}
function isDisabledButton(){
  var objform=document.sitelist;
  var list=$("list");
 var hasDivChild = false;
 if(list.childNodes.length>0 ){
  for(var i=0;i<list.childNodes.length;i++){
     var s=list.childNodes[i];
  if( typeof(s)!="undefined"  && s.tagName) {
   if(s.tagName.toUpperCase()=="DIV")
   hasDivChild = true;
  }
  }
 }
  objform.check.disabled= hasDivChild?false:true;
  objform.recheck.disabled= hasDivChild?false:true;
  objform.check.value=strAll;
  objform.remove.disabled= hasDivChild?false:true; 
}
function delSingle(objparent,objdel,objchild){
    objparent.removeChild(objchild);
       objdel.style.display="none";
//
   isDisabledButton();
}
function editSingle(objparent,objedit,objchild){
  function makeLink(text , tag){
   var temobj = document.createElement(tag);
   temobj.style.marginLeft="5px"; temobj.style.marginRight="5px";
   temobj.style.textDecoration="underline";temobj.style.cursor="pointer";
   temobj.appendChild( document.createTextNode(text) );
    return temobj;  
  }
  try{
  var secondChild = objchild.childNodes[1];
  var url = secondChild;
  var urlStr =  url.nodeValue ;//? url.nodeValue : url.toString() ;
     if(urlStr==null || urlStr.trim=="")return;
  var editorPanel = document.createElement("span");
  objchild.removeChild(url);
  var editor = document.createElement("input");
  editor.type="text";
  //var maxLen = parseInt(objparent.style.width) - 250;
  var maxLen = 50;
     var thisLen = parseInt( countLength(urlStr) );
  editor.size =  maxLen > thisLen ? thisLen : maxLen;
  editor.value = urlStr;
     editor.select();
  editor.onmouseover=function(){this.select();}
  var update =  makeLink("update","font");update.title="更新";
  update.onclick=function(){
     //updateContens();
        var newvalue = editor.value;
  if(newvalue.trim() == ""){editor.focus();return;}
     objchild.removeChild(objchild.childNodes[1]);
     objchild.appendChild(document.createTextNode(newvalue));
  }
  var cancle =  makeLink("cancle","font");cancle.title="取消";
  cancle.onclick=function(){
     objchild.removeChild(objchild.childNodes[1]);
     objchild.appendChild(url);
  }
  //add to editorPanel;
  editorPanel.style.fontSize="11px";
  editorPanel.style.color="blue";
     editorPanel.appendChild(editor);
     editorPanel.appendChild(update);
     editorPanel.appendChild(document.createTextNode("|"));
     editorPanel.appendChild(cancle);
  //add to objchild;
  objchild.appendChild(editorPanel);
   }catch(ex){
      alert(ex.toString());
   }finally{
      //
   }
}
var currentDiv;
function showDelete(self){
  self.style.backgroundColor = overCol;
 currentDiv = self;
 var theid = "del_icon";
 if(!$(theid)){
  var delDiv=document.createElement("div");
  delDiv.id=theid;
  var d=delDiv;
  d.style.textAlign="center";d.style.fontSize="11px";
  var edit = document.createElement("span");
  edit.title="编辑此条";edit.style.textDecoration="underline";
     edit.onclick=function(){
         editSingle(currentDiv.parentNode,this.parentNode,currentDiv);
  }
  var del =  document.createElement("span");
  del.title="删除此条";del.style.textDecoration="underline";
     del.onclick=function(){
         delSingle(currentDiv.parentNode,this.parentNode,currentDiv);
  }
  edit.appendChild(document.createTextNode("edit"));
     d.appendChild(edit);
     d.appendChild(document.createTextNode(" | "));
  del.appendChild(document.createTextNode("delete"));
     d.appendChild(del);
  d.style.width="80px";
  d.style.lineHeight="20px";
  d.style.color="blue";
 // d.style.zIndex="-10";
//  d.style.backgroundColor="green";
  document.body.appendChild(d);
  $(theid).onmouseover=function(){
      currentDiv.style.backgroundColor = overCol;
   this.style.display="";
   //mouseOndeleteDiv=true;
  }
  //
  $(theid).style.cursor="pointer";
 }
 if($(theid)){
   $(theid).style.display="";
   var l= getAbsoluteLeft(self) + self.offsetWidth - 80;
   var t= getAbsoluteTop(self) + 5;
   var scrollT = self.parentNode.scrollTop;//减去出现滚动条的高度;
   var t= getAbsoluteTop(self) + 5 - scrollT;
   $(theid).style.position="absolute";
   $(theid).style.left = l+"px";
   $(theid).style.top = t+"px";
 }
}
function hideDelete(self){
 var theid = "del_icon";
  if( $(theid) ){
   self.style.backgroundColor = self.childNodes[0].checked?checkedCol:emptyCol;
   $(theid).style.display="none";
  }
}
</script>
</HEAD>
<BODY>
<style>
.drag{position:absolute;width:600px; background-color:#f1f1f1;
filter:alpha(opacity=100);
    -moz-opacity:1.0;
    opacity:1.0;
 }
.da{width:100%;background-color:gray;cursor:move;font-size:14px;vertical-align:middle;line-height:30px}
.max_min_btn{float:right;color:red;cursor:pointer;font-size:9pt;width:50px;line-height:30px;text-align:center;}
#list{border-top:2px ridge #9A9898;border-left:2px ridge #9A9898;border-bottom:1px solid #D4D0C8;border-right:1px solid #D4D0C8;background:white;width:578px;height:350px;margin:10px;overflow:auto}
.shadowBottom{margin-left:6px;height:5px;background:gray;font-size:0px;line-height:0px;filter:alpha(opacity=55);-moz-opacity:0.55;opacity:0.55;}
</style>
<pre>
本例是采用JSP + Ajax + 文本来演示多选、全选，添加、删除、更新checkbox多选框的效果。
本例充分展现了利用JavaScript控制HTML DOM以及利用XML http 传递数据，利用JSP操作文本的效果。
1，JavaScript 用来控制页面内容的现实与增删效果;
2，JSP作为后台，对文本进行读取、写入、更新等操作;
3，文本格式是用来存储数据的，数据编码UTF-8;<br>4, 本例子在win32,Red hat Linux 9下 jdk1.5,Tomcat 5.0,Apache 2.5下调试通过，同时兼容Firefox 1.0,Opera 8.0,IE6。
<br>在线说明文档 <a href="http://jiarry.blogchina.com/5599587.html">http://jiarry.blogchina.com/5599587.html</a><br>静态页面演示地址：<A href="http://jarryli.googlepages.com/checkbox_test.html">http://jarryli.googlepages.com/checkbox_test.html</A> (详细的说明，随后补充)<BR>源文件下载地址：<A href="http://jarryli.googlepages.com/checkbox.rar">http://jarryli.googlepages.com/checkbox.rar</A>
<br>
另外之前做的<br>
PHP+Ajax 发帖程序实例:<a href="http://jiarry.blogchina.com/5555296.html">http://jiarry.blogchina.com/5555296.html</a>  源码下载:<a href="http://jarryli.googlepages.com/post_ajax.rar">http://jarryli.googlepages.com/post_ajax.rar</a><br>
JSP登录证码无刷新提示:<a href="http://jiarry.blogchina.com/5287654.html">http://jiarry.blogchina.com/5287654.html</a> 源码下载:<a href="http://jarryli.googlepages.com/imgValidate.rar">http://jarryli.googlepages.com/imgValidate.rar</a><br>
<br>
<div class="drag" rel="drag" id="siteMainDarg" style="left:10px;top:10px;" onMouseDown="dargit(this,event);">
<div class="da" ><span class="max_min_btn" id="max_min" style="background-color:#333"><font style="font-family:webdings">2</font>CLOSE</span>  拖拽区</div>
<div id="sitelistDiv" >
<form name="sitelist" onSubmit="return addSite(this);" style="border:1px groove blue;margin:0;padding:0;border-top:0;"><br clear="all">
  http:// <INPUT TYPE="text" size="30" NAME="url" onmouseover="this.focus()"> <INPUT TYPE="button" NAME="add" value="添加网址至列表" onClick="addSite(this.form)">
<div id="list">
<div onMouseOver="showDelete(this);"  onmouseout="hideDelete(this);" style="padding:5px;border-bottom:1px dashed gray"><input type=checkbox value="example.com" name="url_chkbox" onClick="clickchk(this.form)">example1.com </div>
<div onMouseOver="showDelete(this);" onMouseOut="hideDelete(this);"  style="padding:5px;border-bottom:1px dashed gray"><input type=checkbox  value="example.com" name="url_chkbox" onClick="clickchk(this.form)">example2.com </div>
</div>
  <INPUT TYPE="button" NAME="check" value="全选" disabled onClick="checkall(this.form)">  <input type="button" value="反选" name="recheck" disabled  onclick="reverseCheck(this.form)">  <INPUT TYPE="button" NAME="remove" disabled value="删除" onClick="del($('list'),this.form)"><br><br>
</form>
</div>
<div class="shadowBottom"> </div>
</div><br><br><br>
Cody by JarryLi@gmail.com;
<br>
CopyRight?Jarry,All right reserved!<br>
本例仅供个人参考，引用请保留版权信息。谢谢！
<br>
homepage:<a href="http://jiarry.126.com">jiarry.126.com</a><br>
Blog:<a href="http://jiarry.blogchina.com">http://jiarry.blogchina.com</a>
<script>
//页面加载时初始化
function init(l,f){
  var list=l;
  f.check.disabled=list.childNodes.length>1?false:true;
  f.recheck.disabled=list.childNodes.length>1?false:true;
  f.check.value=strAll;
  f.remove.disabled=list.childNodes.length>1?false:true; 
}
window.onload=function(){
 init($("list"),document.sitelist);
}
activeCookie = true;
if(document.cookie==null){
 //alert("cookie没有开启！");
  activeCookie = false;
}
function getCookie(name) {
    if( !activeCookie  ) return;
 var nameEQ = name + "=";
 var ca = document.cookie.split(';');
 for(var i=0;i < ca.length;i++) {
  var c = ca[i];
  while (c.charAt(0)==' ') c = c.substring(1,c.length);
  if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
 }
 return null;
}
function setCookie(name , value)
{
    if( !activeCookie  ) return;
 var expdate = new Date();
 expdate.setTime(expdate.getTime() + (365*24*120));
 document.cookie=""+ name + "=" + value +";expires="+ expdate.toGMTString();
}
var theLeft = getCookie("siteMainDargLeft")!=null ? getCookie("siteMainDargLeft") : 10;
var theTop = getCookie("siteMainDargTop")!=null ? getCookie("siteMainDargTop") : 10;
var listDivDisplay =  getCookie("sitelistDivDisplay")!=null ? getCookie("sitelistDivDisplay") : "";
$("siteMainDarg").style.top = theTop;
$("siteMainDarg").style.left = theLeft;
$("sitelistDiv").style.display = listDivDisplay;
//cookie end;
////////////////////////
var d_width = 550;
var d_height = 30;
var ey=0,ex=0,lx=0,ly=0,canDrg=false,thiso=null;//
var x, y,rw,rh;
 var divs=document.getElementsByTagName("div");
 for (var i=0;i<divs.length;i++){ 
   if(divs[i].getAttribute("rel")=="drag"){
     divs[i].onmousemove=function(){
     thismove(this);//实时得到当前对象与鼠标的值以判断拖动及关闭区域;
  }
   }
 }
function thismove(o){
    rw = parseInt(x)-parseInt(getAbsoluteLeft(o));
    rh = parseInt(y)-parseInt(getAbsoluteTop(o));
}
function dargit(o,e){
thiso = o;
canDrg = true;
 if(!document.all){
      lx = e.clientX; ly = e.clientY;
      }else{
        lx = event.x; ly = event.y;
   }
 try{//设置渐变色;
  if(rh<=d_height && rw<d_width &&rh>0){//设定在拖动区域
   if(document.all)thiso.filters.Alpha.opacity=80;
   else thiso.style.MozOpacity=80/100;
  }
}catch(e){
}
 st(o); 
}
document.onmousemove = function(e){
if(!document.all){ x = e.clientX; y = e.clientY; }else{ x = event.x; y = event.y; }
if(canDrg){
   if(rh<=d_height && rw<d_width &&rh>0){//如果要设定拖动区域可以作判断
  var ofsx = x - lx;
  thiso.style.left = (parseInt(thiso.style.left) + ofsx) +"px";
  lx = x;
  var ofsy = y - ly;
  thiso.style.top = (parseInt(thiso.style.top) + ofsy) +"px";
  ly = y;
   }else{
    canDrg=false;
   }
 }
}
document.onmouseup=function(){
     canDrg=false;//拖拽变量设为false
 try{
  if(document.all)thiso.filters.Alpha.opacity=100;
  else thiso.style.MozOpacity=100;
}catch(e){}
     if(document.all && thiso != null){
         thiso.releaseCapture();
        thiso = null;
   }
}
function st(o){
  if(rh <= d_height && rw >= d_width){
   canDrg=false;
   showHide($("sitelistDiv"));
   }
}
function showHide(obj,self){
   obj.style.display = obj.style.display=="none"?"":"none";
   //$("del_icon").display="none";
   var icon = "1";
   var text = "OPEN";
   if(obj.style.display==""){
     icon = "2";
     text = "CLOSE";
   }
    $("max_min").childNodes[0].innerHTML = icon;
 $("max_min").childNodes[1].nodeValue = text;
}
window.onunload = function(){
   setCookie("siteMainDargLeft",getAbsoluteLeft( $("siteMainDarg") ) );
   setCookie("siteMainDargTop",getAbsoluteTop( $("siteMainDarg") ) );
   setCookie("sitelistDivDisplay",$("sitelistDiv").style.display );
}
</script>
</BODY>
</HTML>
原文网址：http://xxrblog.cn/article.asp?id=419
