# VB——编译错误 - Jun5203 - CSDN博客
2018年05月01日 09:38:55[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：99
### **1、编译错误：用户定义类型未定义**
原因：未引用控件或代码错误
以下三种解决方法：
1）工程——引用——选中"Microsoft ActiveX Data Objects 2.6 Library"——点击“确定”
拓展：ActiveX Data Objects，简称ADO，访问数据库的一个接口，就是用来跟数据库交互的。
2）检查定义的代码是否有问题，要保证上下文的定义代码一致
3）"Ctrl+T"打开部件，添加“Microsoft ADD Data Control 6.0(OLEDB)”,点击“应用”——“确定”
### **2、编译错误：在End Sub、EndFunction或End属性后面只能出现注释**
原因：End Sub、End Function后不能接句子
解决方法：若End Sub、EndFunction后边的句子有用，则移动位置；若没用，直接删除（简单粗暴）
### **3、编译错误：变量未定义**
解决方法：将出现问题的代码名称用Dim定义
例：UserName="'
Dim UserName As String
### **4、编译错误：无效或不合格的引用**
例：Set .mrc
解决方法：去掉“点”（画蛇添足）Set mrc
### **5、编译错误：子程序或函数中的属性无效**
解决方法：删除出问题的语句
### **6、编译错误：未找到方法或数据成员**
原因：未引用部件
解决方法：工程——部件——选中“Microsoft DataGrid Control 6.0(OLEDB)”——点击“确定”
### **7、编译错误：子程序或函数未定义**
例：ExecuteSQL_Exit
解决方法：在上述语句后加“：”（马大哈）
### **8、编译错误：ByRef参数类型不符**
![](https://img-blog.csdn.net/20180530213458333)
原因：对应参数的类型不符造成
造成这种情况一般有几种情况：
(1)传递参数未定义类型，如果参数在调用前未定义类型，系统自动将它定义为Variant类型，由于与过程或函数中的对就参数类型不相同，所以提示"ByRef参数类型不符"
(2)传递参数类型已经定义，但由于定义方法不对，所以提示"ByRef参数类型不符"。具体情况如下：
当VB中如此定义变量时，将会出现以上错误信息的出现
```
Private Sub Command1_Click()
           Dim i , j As Integer
           i = 3
           j = 4
          Call get1(i, j)
          Text1.Text = i
      End Sub
     Private Sub get1(ByRef para1 As Integer, para2 As Integer)
         para1 = para1 + para2
     End Sub
     如果将 Dim i , j As Integer 改为
       Dim i  As Integer  
       Dim i  As Integer
```
程序执行正常。也就是说，第一种方法的变量定义，只是对 J 做了定义，并未对前面的 I 进行定义。
一般在过程和函数中定义参数时采用ByRef，则要求参数声明的类型和实际参数的类型完全相同，否则VB就会报告错误。
解决方法：将出现问题的代码重新定义即可
### **9、编译错误：缺少子程序、函数或属性**
![](https://img-blog.csdn.net/20180530213416566)
——解决方法
很明显的错误，语句写的不对，这就要求大家在写代码的时候一定要认真，认真再认真，正确的如下：
![](https://img-blog.csdn.net/20180530213442897)
## 以上是目前发现的问题，未完待续，敬请期待...
