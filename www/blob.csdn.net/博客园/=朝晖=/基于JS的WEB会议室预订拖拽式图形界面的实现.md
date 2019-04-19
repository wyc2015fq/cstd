# 基于JS的WEB会议室预订拖拽式图形界面的实现 - =朝晖= - 博客园
# [基于JS的WEB会议室预订拖拽式图形界面的实现](https://www.cnblogs.com/dhcn/p/7116165.html)
06年的一篇blog，转到这个博客上：      
       很早之前写的，后来由于这个功能模块取消，最终没有上线，所以与Server交互的那部分还没有写，不过那部分方案我也已经出来了，而且现在客户端这一部分已经通过了比较完备的[测试](http://lib.csdn.net/base/softwaretest)。大家看看这一部分的代码，应该一看就懂。 那时候也不知道那么多[js](http://lib.csdn.net/base/javascript)的UI库，所以全部都是自己绘制的，界面有点简陋，希望凑或者看吧，再就是我们这个项目是局域网项目，所以大家用的都是IE，没有考虑跨浏览器的因素。发出来希望对做同样应用的同志少走点弯路，我写的时候，网上没有这方面的代码，现在有没有就不知道了。
网页代码：
```
<%@page contentType="text/html; charset=gb2312" pageEncoding="GB2312" %>  
<%@ page language="java"%>  
<%@ page import="java.util.List" %>  
<style>  
th.timem      {font-size: 8pt; font-family: Times New Roman;}  
td.reserved   {background-color: #FF0066}  
td.free       {background-color: #99CCFF}  
td.selected   {background-color: #ccCCFF}  
</style>  
<script language="JavaScript" type="text/javascript" src="timetable.js"></script>  
<script>  
<!--  
    var dragging=false;//是否在拖拽  
    var OrignCell="";//开始格位  
    var CurrentCell="";//当前格位  
    var pope;  
    window.onload=function()  
    {  
        try{  
            pope=document.getElementById("pop");  
            var tp=document.getElementById('TablePosition');  
            var tt=CreateTable();  
            tp.appendChild(tt);  
            SetReserves();  
            document.getElementById('2-5').className="reserved";  
                document.getElementById('5-10').className="reserved";//for test  
        }catch(e)  
        {  
            alert(e.name + ": " + e.message);  
        }  
    }  
       //设置已预订单元  
    function SetReserves()  
    {  
        //Insert the Reserved Cells Intialation by SetReserve()  
          
    }  
-->  
</script>  
<div id="pop" style="position:absolute;width:50; display: none; z-index:99; font-size: 13px; background:#FFCCFF;filter:alpha(opacity=40); border-style: solid; border-width: thin;border-color:#FF66CC; " >  
<div onclick="Reserve()" style="{cursor: hand;} ">预订</div>  
<hr size=0.1 color=#FF66CC;>  
<div onclick="Cancel()"style="{cursor: hand;} ">取消</div>  
</div>  
<div id="TablePosition"  >  
</div>
```
```
//创建会议室预订情况时间表  
    function CreateTable()  
    {  
        var tt=document.createElement("table");  
        var tbody=document.createElement("tbody");  
        tt.onmouseout=OutDragT;  
        tbody.onmouseout=CancleBubble;  
        tt.appendChild(tbody);  
        tt.border='1';  
        tt.id='TimeTable';  
        tt.cellSpacing ="1";//???????????  
        tt.borderColor="#6699ff";  
        tt.align='center';  
        tt.onselectstart =False;   
        for(var i=0;i<6;i++)  
        {  
            var tr=document.createElement("tr");  
            for(var j=0;j<18;j++)  
            {  
                var hd=document.createElement("td");  
                hd.id=i+'-'+j;  
                hd.innerHTML=" ";  
                hd.onmousedown = BeginDrag;  
                        hd.onmousemove = OnDrag;  
                        hd.onmouseup = EndDrag;  
                hd.className="free";///for test  
                hd.onmouseover=onshow;  
                if(i==0)  
                {  
                    hd=document.createElement("th");  
                    hd.className='timem';  
                    hd.width='30';  
                    if(j==0)  
                    {  
                        hd.innerHTML=" "  
                    }else{  
                        var hour=8+Math.floor(j/2);  
                        var minutes=30*(j%2);  
                        hd.appendChild(document.createTextNode(hour+':'+minutes));  
                    }  
                }  
                if(j==0&&i!=0)  
                {  
                    hd=document.createElement("th");  
                    hd.className='timem';  
                    hd.appendChild(document.createTextNode('?'+i));  
                }  
                tr.appendChild(hd);  
            }  
            tbody.appendChild(tr);  
        }  
  
  
        return tt;      
    }  
  
    function SetReserve(id)  
    {  
        document.getElementById(id)="reserved";  
    }  
    //取消事件冒泡  
    function CancleBubble()  
    {  
        event.cancelBubble=true;  
    }  
    //拖拽选择开始函数  
    function BeginDrag()  
    {  
        if (event.button != 1)  
        {  
            return;  
        }  
        OrignCell=this.id;  
        CurrentCell=this.id;  
        if(ModifyClass())  
        {          
               dragging=true;  
        }else{  
            alert("????????!");  
            OrignCell="";  
            CurrentCell="";  
        }  
    }  
   //拖拽过程处理函数  
    function OnDrag()  
    {  
        if(dragging==false||this.id==CurrentCell)  
        {  
            return;  
              
        }  
        CurrentCell=this.id;  
        if(!ModifyClass())  
        {  
            Reverse()  
            dragging=false;  
            OrignCell="";  
                    CurrentCell="";  
            pope.style.display = 'none';  
            alert("?????????????!");  
              
        }          
    }  
    //结束拖拽  
    function EndDrag()  
    {  
        if(dragging==false)  
        {  
            return;  
        }  
        dragging==false;  
        pope.style.top=event.clientY  
        pope.style.left=event.clientX  
        pope.style.display = "";  
    }  
    //本为处理鼠标过界操作后作他用  
    function OutDrag()  
    {      
        if(dragging==false)  
        {  
            return;  
        }  
        //???  
        Reverse();  
        dragging=false;  
      
    }  
    //Handler for mousemoveout of timetable  
    function OutDragT()  
    {  
        var Tt=document.getElementById("TimeTable");  
            if(event.clientX<=Tt.offsetLeft||event.clientX>=(Tt.offsetLeft+Tt.offsetWidth)||event.clientY<=Tt.offsetTop||event.clientY>=(Tt.offsetTop+Tt.offsetHeight))  
        {  
            OutDrag();  
            window.status="";  
        }  
    }  
    function False()  
    {  
         return false;  
    }  
    //修正各单元格的状态并返回结果  
    function ModifyClass()  
    {  
          
        Reverse();  
      
        for(var i=X1();i<=X2();i++)  
        {  
            for(var j=Y1();j<=Y2();j++)  
            {  
                var CName =document.getElementById(i+"-"+j).className;  
                if(CName.indexOf("reserved")!=-1)  
                {  
                    return false;  
                }  
                if(CName.indexOf("selected")!=-1)  
                {  
                    continue;  
                }  
                if(CName.indexOf("free")!=-1)  
                {  
                    document.getElementById(i+"-"+j).className="selected";  
                }                  
            }  
        }  
        return true;  
    }  
    //返回初始状态  
    function Reverse()  
    {  
        for(var i=1;i<6;i++)  
        {  
            for(var j=1;j<18;j++)  
            {  
                var CName =document.getElementById(i+"-"+j).className;  
                if(CName.indexOf("selected")!=-1)  
                {  
                    document.getElementById(i+"-"+j).className="free";      
                }  
            }              
        }  
    }  
    //取消选择  
    function Cancel()  
    {  
        pope.style.display = 'none';  
        OutDrag();  
        dragging=false;  
    }  
    //预订操作  
    function Reserve()  
    {  
        pope.style.display = 'none';  
        alert("????!")  
        dragging=false;  
    }  
    //选择区域的横方向小坐标  
    function X1()  
    {  
        var Bx,Ex;  
        var TArray=OrignCell.split('-');  
        Bx=parseInt(TArray[0]);  
        TArray=CurrentCell.split('-');  
        Ex=parseInt(TArray[0]);  
        return Bx<=Ex?Bx:Ex;  
          
    }  
    //选择区域的横方向大坐标  
    function X2()  
    {  
        var Bx,Ex;  
        var TArray=OrignCell.split('-');  
        Bx=parseInt(TArray[0]);  
        TArray=CurrentCell.split('-');  
        Ex=parseInt(TArray[0]);  
        return Bx>=Ex?Bx:Ex;  
    }  
    //选择区域的纵方向小坐标  
    function Y1()  
    {  
        var Bx,Ex;  
        var TArray=OrignCell.split('-');  
        Bx=parseInt(TArray[1]);  
        TArray=CurrentCell.split('-');  
        Ex=parseInt(TArray[1]);  
        return Bx<=Ex?Bx:Ex;  
    }  
    //选择区域的纵方向大坐标  
    function Y2()  
    {  
        var Bx,Ex;  
        var TArray=OrignCell.split('-');  
        Bx=parseInt(TArray[1]);  
        TArray=CurrentCell.split('-');  
        Ex=parseInt(TArray[1]);  
        return Bx>=Ex?Bx:Ex;  
    }  
    //显示选择情况！  
    function onshow()  
    {  
        window.status="Cell:"+OrignCell+"::"+CurrentCell+dragging;  
    }
```
[](http://blog.csdn.net/dipolar/article/details/44960121)

