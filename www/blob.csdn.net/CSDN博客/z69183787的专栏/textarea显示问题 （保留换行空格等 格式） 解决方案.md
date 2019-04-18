# textarea显示问题 （保留换行空格等 格式） 解决方案 - z69183787的专栏 - CSDN博客
2013年08月23日 16:22:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8229
## 1 使用 pre 标签（但IE7下有兼容问题）：
```
<pre style="width: 100%; white-space: pre-wrap !important; word-wrap: break-word;"><s:property escape="0" value="vo.remark"/></pre>
```
```
<style>
			pre {
				white-space: pre-wrap;       /* css-3 */
				white-space: -moz-pre-wrap;  /* Mozilla, since 1999 */
				white-space: -pre-wrap;      /* Opera 4-6 */
				white-space: -o-pre-wrap;    /* Opera 7 */
				word-wrap: break-word;       /* Internet Explorer 5.5+ */
				word-break:break-all;
				overflow:hidden;
			}
		</style>
```
## 2 textarea readonly border设为0 ，取消onfocus事件
```
TEXTAREA.reason{
	font-size: 13px;  font-family:'微软雅黑','黑体','宋体';font-weight: normal;
	border:0;width:650px; height:50px;margin-left:103px;background:transparent;overflow:hidden;
}
TEXTAREA.files {
	font-size: 13px;  font-family:'微软雅黑','黑体','宋体';font-weight: normal;line-height:1.5;text-decoration:underline; color:blue;cursor:pointer;
	border:0;width:300px; height:200px;margin-left:110px;background:transparent;overflow:hidden;
}
textarea.levels{
	font-size: 13px;  font-family:'微软雅黑','黑体','宋体';font-weight: normal;line-height:1.5; 
	border:0;width:150px; height:200px;margin-left:155px;background:transparent;overflow:hidden;
}
TEXTAREA.inputText{
	width:170px;  font-size: 13px; margin-left:10px; font-family:'微软雅黑','黑体','宋体';font-weight: normal; background:transparent;
	border:0;
	
}
<TEXTAREA class=reason id=applyReason onfocus=this.blur(); rows=3 readOnly>我 在 测试 
测试 
测试 </TEXTAREA>
```
```
<TEXTAREA class=reason id=applyReason onfocus=this.blur(); rows=3 readOnly>我 在 测试 
测试 
测试 </TEXTAREA>
```
