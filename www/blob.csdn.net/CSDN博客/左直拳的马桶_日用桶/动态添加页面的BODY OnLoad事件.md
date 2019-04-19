# 动态添加页面的BODY OnLoad事件 - 左直拳的马桶_日用桶 - CSDN博客
2010年04月21日 18:02:00[左直拳](https://me.csdn.net/leftfist)阅读数：1681
if (window.attachEvent) {
    window.attachEvent("onload", GetMyInfo);//IE
}
else {
    window.addEventListener("load", GetMyInfo, false);//FF
}
function GetMyInfo()
{
alert("Hello World!");
}
