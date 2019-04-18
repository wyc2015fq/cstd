# 互联网软件的安装包界面设计-Inno setup - weixin_33985507的博客 - CSDN博客
2016年03月11日 11:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
```
“安装界面太丑了，不堪入目！”
    “这界面应该属于20年代初的旧产物……”
    “难看！”
   每每听到设计师和产品经理这样的吐槽时，心中难免会有一点失落。细看一下，咱们软件的安装包界面的确还是比较古老，仍然带有xp系统自带的界面边框。于是趁着这次新版软件发布之际，专门花了时间去研究界面设计。
   **目标要求：**
   1、   界面border去掉原本windows自带的对话框格式，采取扁平化设计；
   2、   简化安装流程，不要弹出那么多安装向导页，不要让用户一直点“下一步”，简洁人性化；
   3、   安装界面可加载漂亮的背景图片；
   4、   优化安装包安装时间。
   **原型设计：**
```
![1492845-9b7c8892988d6656.png](https://upload-images.jianshu.io/upload_images/1492845-9b7c8892988d6656.png)
界面1
![1492845-4ff208f37a20a5f4.png](https://upload-images.jianshu.io/upload_images/1492845-4ff208f37a20a5f4.png)
界面2
![1492845-e3489f9858af3f22.png](https://upload-images.jianshu.io/upload_images/1492845-e3489f9858af3f22.png)
界面3
```
**开发工具：**
   因为之前采用Inno setup设计，而且它拥有Pascal脚本引擎，功能算强大，故采用Inno setup。
   **主要流程：**
   ![流程图](http://upload-images.jianshu.io/upload_images/1492845-bc1c0c95ce0143a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
   由图中可以清晰地看出，所有向导页之间都是通过“下一步”或者“上一步”进行驱动更换的。
   **概要分析：**
   这里需要调用两个美化插件动态库：botva2.dll和InnoCallback.dll，用于界面的美化和功能完善。这里就不全部把代码贴出来了，只针对其中的要点进行分析概括。
   在开始之前，先了解一下inno setup基本的过程和函数。
```
```
//该过程在开始的时候改变向导或者向导页，不要指望使用InitializeSetup函数实现改变向导页的功能，因为InitializeSetup函数触发时向导窗口并不存在。
procedure InitializeWizard();
//该函数在安装程序初始化时调用，返回False 将中断安装，True则继续安装.
function InitializeSetup(): Boolean;
//该过程提供用户完成预安装和安装之后的任务，更多的是提供了安装过程中的状态。参数CurStep=ssInstall是在程序实际安装前，CurStep=ssPostInstall是实际安装完成后，而CurStep=ssDone是在一次成功的安装完成后、安装程序终止前（即点击finish按钮后执行）。 
procedure CurStepChanged(CurStep: TSetupStep);
//当用户单击下一步按钮时调用。如果返回True，向导将移到下一页；如果返回False，它仍保留在当前页。
function NextButtonClick(CurPageID: Integer): Boolean;
//向导调用这个事件函数确定是否在所有页或不在一个特殊页 (用PageID 指定) 显示。如果返回True，将跳过该页；如果你返回False，该页被显示。注意: 这个事件函数不被wpWelcome、wpPreparing 和wpInstalling 页调用，还有安装程序已经确定要跳过的页也不会调用 (例如，没有包含组件安装程序的wpSelectComponents)。
function ShouldSkipPage(PageID: Integer): Boolean;
//在新向导页 (由CurPageID 指定) 显示后调用。
procedure CurPageChanged(CurPageID: Integer);
```
```
1、  界面border去掉原本windows自带的对话框格式，采取扁平化设计
```
```
//设置欢迎向导页的尺寸大小： 
WizardForm.Center WizardForm.BorderStyle:=bsNone; //去掉对话框border WizardForm.ClientWidth:=601; 
WizardForm.ClientHeight:=341; 
WizardForm.InnerNotebook.Hide; 
WizardForm.OuterNotebook.Hide; 
wizardform.Color:=TransparentColor; 
WizardForm.Bevel.Hide; 
WizardForm.NextButton.Width:=0; 
WizardForm.BackButton.Width:=0; 
WizardForm.CancelButton.Width:=0; 
WizardForm.OnMouseDown:=@WizardFormMouseDown; //可拉动
```
```
2、   简化安装包安装流程，不要用户一直点“下一步”，简洁人性化
   这里只有两个向导页，一个是选择安装目录和许可协议的界面，一个是正在安装的界面。用户只需点击“开始安装”即可安装，并有进度条和百分比显示，不用经常性地点击“下一步”。
   3、   安装界面可加载漂亮的图片和动画效果
```
```
// 创建描述位图
Bmp:= TBitmapImage.Create(WizardForm);
Bmp.Top:=ScaleX(0);
Bmp.Left:=ScaleX(0);
Bmp.Width:=WizardForm.ClientWidth;
Bmp.Height:=ScaleY(371);
Bmp.BackColor:= WizardForm.Color;
Bmp.Parent:=WizardForm;
Bmp.Bitmap.LoadFromFile(ExpandConstant('{tmp}\bg.bmp'));
Bmp.OnMouseDown := @WizardFormMouseDown; //可拉动
//动画效果
AnimateWindow(StrToInt(ExpandConstant('{wizardhwnd}')), 400, AW_Center or AW_Activate);
SetWindowLong(StrToInt(ExpandConstant('{wizardhwnd}')),GWL_Exstyle, WS_Ex_Layered);
SetLayeredWindowAttributes(StrToInt(ExpandConstant('{wizardhwnd}')), 0, 240, LWA_Alpha);
```
```
4、   优化安装包安装时间。
   把不用的安装文件重新梳理了一遍，减少打包时的体积，不用多次调用读写注册表。
   **难点分析：**
   1、   如何跳过向导页？
```
```
//主要是程序调用“下一步”，然后在ShouldSkipPage函数中进行跳过。
function ShouldSkipPage(PageID: Integer): Boolean;
begin
if PageID=wpWelcome then 
result := true;
if PageID=wpSelectDir then 
result := true;
end;
```
```
2、   “下一步”如何调用？
```
```
WizardForm.NextButton.OnClick(WizardForm);
```
```
3、   如何用鼠标拉动对话框？
```
```
重载WizardFormMouseDown过程，然后调用SendMessage(WizardForm.Handle, WM_SYSCOMMAND, $F012, 0);
```
```
4、   如何自定义浏览文件夹按钮？
```
```
//记住，获得的路径要赋给WizardForm.DirEdit.Text，这是浏览文件夹向导页wpSelectDir的路径编辑框。
procedure DirOnClick(hBtn:HWND);
var
res: Boolean;
UserSelectDir,str: string;
begin 
UserSelectDir := Edit.Text; 
res := BrowseForFolder('浏览文件夹', UserSelectDir, True); 
if res then 
begin 
str:=Copy(UserSelectDir,Length(UserSelectDir),1);//取最后一个字符
 if '\'=str then
 Edit.Text := UserSelectDir 
else 
Edit.Text := UserSelectDir; 
end; 
//浏览文件夹向导页的文件夹路径 
WizardForm.DirEdit.Text := Edit.Text;
end;
```
```
5、   如何点击按钮显示“许可协议”？
```
```
ShellExec('', ExpandConstant('{tmp}\license.txt'),'', '', SW_SHOW, ewNoWait, ErrorCode)。
```
```
6、   如何安装程序完成后自动运行程序？
```
```
Exec(ExpandConstant('{app}\MySoftware.exe'), '', '', SW_SHOW, ewNoWait, RCode);
```
```
7、   如何删除文件夹？
```
```
DelTree(ExpandConstant('{app}\文件夹名称'), True, True, False);
```
```
8、   如何关闭向导页？
```
```
WizardForm.Close;
```
```
9、   如何自定义完成向导页的完成按钮？
```
```
通过程序自行调用WizardForm.NextButton.OnClick(WizardForm);达到“下一步”的功能；
然后通过函数ShouldSkipPage一直调到最后完成页wpFinished；
最后调用WizardForm.NextButton.OnClick(WizardForm);完成。
```
```
10、  如何在安装页面换背景图片？
```
```
procedure CurPageChanged(CurPageID: Integer);
begin 
if CurPageID = wpInstalling then 
begin 
Bmp:= TBitmapImage.Create(WizardForm); 
Bmp.Top:=ScaleX(0); 
Bmp.Left:=ScaleX(0); 
Bmp.Width:=WizardForm.ClientWidth;
Bmp.Height:=ScaleY(371); 
Bmp.BackColor:= WizardForm.Color;
Bmp.Parent:= WizardForm; 
Bmp.Bitmap.LoadFromFile(ExpandConstant('{tmp}\bg2.bmp')); 
Bmp.OnMouseDown := @WizardFormMouseDown; 
WizardForm.OnMouseDown:=@WizardFormMouseDown; 
end;
end;
```
```
11、 如何读写注册表？
```
```
//读取信息
RegQueryStringValue(HKLM, 'Software\***\***','Version',svValue)
//写入信息
RegWriteStringValue(HKLM, 'Software\***\***','Version', '4.0');
```
```
12、 安装前如何判断是否有进程正在进行？
```
```
//安装前判断是否有进程正在运行
function IsModuleLoaded(modulename: String ): Boolean;
external 'IsModuleLoaded@files:psvince.dll stdcall setuponly';
IsModuleLoaded('Mysoftware.exe');
```
```
13、 如何使用botva2.dll插件创建自定义按钮？
```
```
//调用动态库函数
function BtnCreate(hParent :HWND; Left, Top, Width, Height :integer; FileName :PAnsiChar; ShadowWidth :integer; IsCheckBtn :boolean) :HWND;
external 'BtnCreate@files:botva2.dll stdcall';
FinishBtnH := BtnCreate(WizardForm.Handle,ProgressBar.Left+ProgressBar.Width-83, ProgressBar.Top+ProgressBar.Height+ScaleX(10), ScaleX(83), ScaleY(28),ExpandConstant('{tmp}\Finish.png'),1,False);
BtnSetEvent(FinishBtnH,BtnClickEventID,WrapBtnCallback(@FinishButtonClick,1));
BtnSetVisibility(FinishBtnH,False);
```
```
14、 百分比如何确定？
```
```
//百分比
function PBProc(h:hWnd;Msg,wParam,lParam:Longint):Longint;
var 
pr,i1,i2 : Extended; 
w : integer; 
begin 
Result:=CallWindowProc(PBOldProc,h,Msg,wParam,lParam);
 if (Msg=$402) and (WizardForm.ProgressGauge.Position>WizardForm.ProgressGauge.Min) then 
begin 
i1:=WizardForm.ProgressGauge.Position-WizardForm.ProgressGauge.Min; 
i2:=WizardForm.ProgressGauge.Max-WizardForm.ProgressGauge.Min; 
pr:=i1*100/i2; 
PrLabel.Caption:=IntToStr(Round(pr))+'%'; 
w:=Round(110*pr/100); //改变数值，达到百分比和进度条的同步 
ProgressBar.Position := w; 
end;
end;
```
```
**错误解决：**
    对于某些电脑环境，安装时只要点击“退出”按钮或者安装完后总会遇到这个错误，弹出以下提示图片：
```
![1492845-77fb4fe94c618d23.jpg](https://upload-images.jianshu.io/upload_images/1492845-77fb4fe94c618d23.jpg)
错误问题
```
解决办法:
```
```
因为调用WizardForm.Close或者WizardForm.NextButton.OnClick(WizardForm)退出时没有进行WizardForm的资源释放，所以导致了该错误。只要在退出或者完成前加上WizardForm.Release即可解决。
```
