# javascript上传图片前预览图片兼容大多数浏览器 - z69183787的专栏 - CSDN博客
2014年12月12日 10:58:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：992
                
代码如下:
<div id="localImag"><img id="preview" width="-1" height="-1" style="display:none" /></div>
<asp:FileUpload ID="file_head" runat="server" onchange="javascript:setImagePreview();" />
复制代码 代码如下:
<script type="text/javascript"> 
function setImagePreview() { 
var docObj = document.getElementById("ctl00_ContentMain_file_head"); 
var fileName = docObj.value; 
if (!fileName.match(/.jpg|.gif|.png|.bmp/i)) { 
alert('您上传的图片格式不正确，请重新选择！'); 
return false; 
} 
var imgObjPreview = document.getElementById("preview"); 
if (docObj.files && docObj.files[0]) { 
//火狐下，直接设img属性 
imgObjPreview.style.display = 'block'; 
imgObjPreview.style.width = '63px'; 
imgObjPreview.style.height = '63px'; 
//imgObjPreview.src = docObj.files[0].getAsDataURL(); 
if (window.navigator.userAgent.indexOf("Chrome") >= 1 || window.navigator.userAgent.indexOf("Safari") >= 1) {
imgObjPreview.src = window.webkitURL.createObjectURL(docObj.files[0]); 
} 
else { 
imgObjPreview.src = window.URL.createObjectURL(docObj.files[0]); 
} 
} else { 
//IE下，使用滤镜 
docObj.select(); 
docObj.blur(); 
var imgSrc = document.selection.createRange().text; 
var localImagId = document.getElementById("localImag"); 
//必须设置初始大小 
localImagId.style.width = "63px"; 
localImagId.style.height = "63px"; 
//图片异常的捕捉，防止用户修改后缀来伪造图片 
try { 
localImagId.style.filter = "progid:DXImageTransform.Microsoft.AlphaImageLoader(sizingMethod=scale)";
localImagId.filters.item("DXImageTransform.Microsoft.AlphaImageLoader").src = imgSrc;
} catch (e) { 
alert("您上传的图片格式不正确，请重新选择！"); 
return false; 
} 
imgObjPreview.style.display = 'none'; 
document.selection.empty(); 
} 
return true; 
} 
</script> 
此js解决360浏览器显示图片问题。因为360浏览器6.2用的是Chrome的内核，而 
复制代码 代码如下:
window.URL.createObjectURL(docObj.files[0]); 
只有在火狐下才认得到，chrome中不认识window.URL.createObjectURL，所有用Safari的window.webkitURL.createObjectURL
