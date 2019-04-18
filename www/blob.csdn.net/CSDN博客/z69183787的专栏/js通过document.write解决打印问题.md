# js通过document.write解决打印问题 - z69183787的专栏 - CSDN博客
2013年10月25日 14:52:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3360
```java
<script>
	
	function startPrint(obj){
    var oWin=window.open("","_blank");
	var strPrint='<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"><html xmlns="http://www.w3.org/1999/xhtml"><head><meta http-equiv="Content-Type" content="text/html; charset=gb2312" /><title>打印预览</title></head><body>';
    strPrint=strPrint +"<h4 style='font-size:18px; text-align:center;'>打印预览</h4>\n";
    strPrint=strPrint + "<script type=\"text/javascript\">\n";
    strPrint=strPrint + "function printWin()\n";
    strPrint=strPrint + "{";
    strPrint=strPrint +    "var oWin=window.open(\"\",\"_blank\");\n";
    strPrint=strPrint + "oWin.document.write(document.getElementById(\"content\").innerHTML);\n";
    strPrint=strPrint + "oWin.focus();\n";
    strPrint=strPrint + "oWin.document.close();\n";
    strPrint=strPrint + "oWin.print()\n";
    strPrint=strPrint + "oWin.close()\n";
    strPrint=strPrint + "}\n";
    strPrint=strPrint + "<\/script>\n";
    strPrint=strPrint + "<hr size='1'/>\n";
    strPrint=strPrint + "<div id=\"content\">\n";
	strPrint=strPrint + '<link href="http://js.3conline.com/pcgames/global.css" type="text/css" rel="stylesheet" /><link href="http://www1.pcgames.com.cn/zt/ssxt/g20120531/css/style.css" type="text/css" rel="stylesheet" /><link href="http://www1.pcgames.com.cn/zt/ssxt/g20120531/css/table.css" type="text/css" rel="stylesheet" /><link href="http://www1.pcgames.com.cn/zt/ssxt/g20120531/css/tableshow.css" type="text/css" rel="stylesheet" /><style type="text/css">body{background:#fff}</style>';
    strPrint=strPrint + obj.innerHTML + "\n";
    strPrint=strPrint + "</div>\n";
    strPrint=strPrint + "<hr size='1'/>\n";
    strPrint=strPrint + "<div style='text-align:center'><button onclick='printWin()' style='padding-left:4px;padding-right:4px;'>打印</button><button onclick='window.opener=null;window.close();'  style='padding-left:4px;padding-right:4px;'>关闭</button></div>\n";
    strPrint=strPrint + "<body></body></html>";
	oWin.document.write(strPrint);
    oWin.focus();
    oWin.document.close();
}
function a(){
	startPrint(document.getElementById("tables2"));	
	}
</script>
```
