# VC6.0制作OCX并web调用 - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年03月22日 11:14:30[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6418








1.打开VC6.0，新建工程，选择：MFC ActiveX ControlWizard，后面步骤默认即可；

2.在ClassView上，可以看到主要有**App\**Ctrl\**ProgPage几个类，其中接口主要在**Ctrl内实现，可以通过MFC ClassWizard(ctrl+w快捷键）的Automation页建立接口函数；

3.接口函数是对外，因此在参数类型定义上要注意，字符我一般用LPCTSTR，如下面函数：

  void COnOcxCtrl::CallOcx(LPCTSTR devip) 

{

 // TODO: Add your dispatch handler code here

 AfxMessageBox(devip);

 ip=devip;

 Refresh();//重绘，调用ondraw()函数；

 return;

}

4.OCX控件界面主要在OnDraw()函数内实现，可以内嵌到IE的，如果是独立对话框和Console是无法内嵌到IE，如：

CONST char* ip="127.0.1.1";

void COnOcxCtrl::OnDraw(

   CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)

{

 // TODO: Replace the following code with your own drawing code.

 pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

 pdc->Ellipse(rcBounds);

 pdc->TextOut(0,0,ip);

}

5.本案例主要实现：接口方法CallOcx接收一个IP地址，然后更新控件界面的IP显示；下步应研究如何利用OnDraw()内CDC绘制界面，满足与用户的交互；

对于**ProgPage（属性）和**Events（触发对容器响应的事件）的理解可参考[http://www.cnblogs.com/jyz/archive/2008/04/11/1148476.html](http://www.cnblogs.com/jyz/archive/2008/04/11/1148476.html)地址；

6.Web调用OCX的代码：

---------------------------------------------------------------------------------------------------------------------------------

<HTML>

<H1> MFC ActiveX Test Page </H1><p>

You should see your MFC forms or controls embedded in the form below.

<HR><center><P>

<input id="input1" value="CallActiveX" type="button" onclick="CallOcx()">

<br>

<OBJECT id="devread" name="devread" classid="clsid:39F8ACC9-EF99-4B74-9C3C-EE50E5AD56C8" codebase="OnOcx.ocx" style="HEIGHT: 500px;WIDTH:500px"></OBJECT> 

</HTML>

<script   language="JavaScript"   type="text/JavaScript">   

function CallOcx()   

{   

   //--初始化、装载样式文件   

   var   iRes;

   iRes   =   devread.CallOcx("172.30.88.66");

   if(iRes==0)

   {

    alert("失败！");   

   }

}

</script>   

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

下面研究如何封装成cab来提供给web调用；

参考：[http://download.csdn.net/detail/fjssharpsword/4165589](http://download.csdn.net/detail/fjssharpsword/4165589)



