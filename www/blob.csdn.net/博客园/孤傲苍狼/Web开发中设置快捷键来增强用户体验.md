# Web开发中设置快捷键来增强用户体验 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Web开发中设置快捷键来增强用户体验](https://www.cnblogs.com/xdp-gacl/p/3475487.html)



　　从事对日外包一年多以来，发现日本的无论是WinForm项目还是Web项目都注重快捷键的使用，日本人操作的时候都喜欢用键盘而不是用鼠标去点，用他们的话来说"**键盘永远比鼠标来的快**"，所以他们很注重键盘的操作，常见的操作就是按下Tab键来切换鼠标的焦点，按下回车键时能够提交表单等，难得今天周末，有时间好好研究一下在web开发中设置快捷键，这样可以方便用户操作，并且增强用户体验。

　　在web开发中主要是使用HTML的表单元素自带的tabindex属性和使用js设置组合快捷键来实现快捷键，

# 一、为HTML的表单元素设置tabindex来实现快捷键

有如下的一段HTML代码：

```
1 <div class="login-tr-title">用户登录</div>
 2          <table class=" table-refer">
 3             <tr>
 4                 <td class="login-td-title">
 5                     用户ID
 6                 </td>
 7                 <td>
 8                     <input id="txtUserName" name="txtUserName" type="text" 
 9 **                        tabindex="1"** maxlength="30" title="用户ID"/>
10                 </td>
11             </tr>
12             <tr>
13                 <td class="login-td-title">
14                     密码
15                 </td>
16                 <td>
17                     <input id="txtPassword" name="txtPassword" type="password"
18 **                        tabindex**="2" maxlength="30" title="密码" />
19                 </td>
20             </tr>
21         </table>
22         <ul class="login-button">
23             <li class="login-main-button">
24                 <asp:Button ID="btnLogin" runat="server" Text="登录"** TabIndex**="3" CssClass="s-button"/>
25             </li>
26         </ul>
27 </div>
```

　　这段HTML代码中每一个表单控件都设置了一个**tabindex**属性，属性值从小到大，**控件设置上tabindex属性后，按下键盘上的Tab键切换光标时，光标就会按照控件设置的tabindex从小到大(1→2→3)顺序来切换了，这样就可以做到使用键盘上的Tab键来控制鼠标焦点落到按tabindex设置的顺序落到相应的控件上**，按照w3c 的规定tabindex的取值范围是：0~32767。
　　在web项目按下回车键时是做不到按照tabindex的设置顺序来切换鼠标焦点的，为了使按下回车键时也能按照tabindex的设置顺序来切换鼠标焦点，可以用JavaScript去控制回车键的响应动作，实现代码如下：

```
1 $(document).ready(function () {
 2     $("form").keydown(function (e) {
 3         var actE = document.activeElement;
 4         var nxtE = $("[tabindex='" + actE.tabIndex + "']")[0];
 5         if (nxtE != null) {
 6             var oType = nxtE.type;
 7             if (oType == "textarea") return;
 8         }
 9         var tabIndex = actE.tabIndex + 1;
10         var keyCode = e.keyCode ? e.keyCode : e.which ? e.which : e.charCode;
11         if (keyCode == 13) {
12             if (nxtE != null) {
13                 oType = nxtE.type;
14                 switch (oType) {
15                     case "button": { if (nxtE.click) nxtE.click(); return false; }
16                     case "select-one": { break; }
17                     case "submit": { return true; }
18                     default: break;
19                 }
20             }
21 
22             nxtE = $("[tabindex='" + tabIndex + "']")[0];
23             if (!nxtE) nxtE = $("[tabindex='0']")[0];
24             if (nxtE != null) {
25                 nxtE.focus();
26 //                                var oType = nxtE.type;
27 //                                switch (oType) {
28 //                                    case "button": { if (nxtE.click) nxtE.click(); break; }
29 //                                    case "select-one": { break; }
30 //                                    case "submit": { return true; }
31 //                                    default: break;
32 //                                }
33             }
34             else {
35                 return true;
36             }
37             return false;
38         }
39     });
40 });
```

# 二、使用js 设置组合快捷键

**　　使用js设置快捷键的本质就是获得你要设置的这个键的keyCode 的值，如果要加ctrl，alt，shift，那么就添加一个ctrlkey，altKey，shiftKey来判断下，所以关键在于获取keycode的值。**

　　(1) 设置 ctrl +enter 提交　　

```
1 if (e.ctrlKey && e.keyCode == 13){ 　
2 　　　return submit(); 　
3 }
```

　　(2) 设置  Alt+ 方向键 ←提交

```
1 if (e.altKey&& e.keyCode == 37){  
2 　　　　return submit(); 
3 }
```

　　(3) 设置  shift+F10 提交

```
1 if (e.shiftKey&& e.keyCode == 37){  　
2 　　　return submit();
3 
4 }
```

　　(4) 设置enter 提交

```
1 if (e.keyCode == 13){
2 　return submit();  　
3 }
```

　　一些常见的快捷键的KeyCode：

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


 1 keycode    8 = BackSpace BackSpace
 2 　　keycode    9 = Tab Tab
 3 　　keycode   12 = Clear
 4 　　keycode   13 = Enter
 5 　　keycode   16 = Shift_L
 6 　　keycode   17 = Control_L
 7 　　keycode   18 = Alt_L
 8 　　keycode   19 = Pause
 9 　　keycode   20 = Caps_Lock
10 　　keycode   27 = Escape Escape
11 　　keycode   32 = space space
12 　　keycode   33 = Prior
13 　　keycode   34 = Next
14 　　keycode   35 = End
15 　　keycode   36 = Home
16 　　keycode   37 = Left
17 　　keycode   38 = Up
18 　　keycode   39 = Right
19 　　keycode   40 = Down
20 　　keycode   41 = Select
21 　　keycode   42 = Print
22 　　keycode   43 = Execute
23 　　keycode   45 = Insert
24 　　keycode   46 = Delete
25 　　keycode   47 = Help　　


View Code
```









