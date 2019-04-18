# 面试-SizeOf一个对象会得到什么？ - weixin_33985507的博客 - CSDN博客
2017年02月28日 11:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**相关资料:**
1.https://zhidao.baidu.com/question/283124338.html
```
1 unit Unit1;
 2 
 3 interface
 4 
 5 uses
 6   Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
 7   Dialogs, Menus, StdCtrls;
 8 
 9 type
10   TForm1 = class(TForm)
11     Button1: TButton;
12     Edit1: TEdit;
13     procedure Button1Click(Sender: TObject);
14   private
15     { Private declarations }
16   public
17     FInteger :Integer;
18     FBool :Boolean;
19     FDouble: Double;
20     { Public declarations }
21   end;
22 
23 var
24   Form1: TForm1;
25 
26 implementation
27 
28 {$R *.dfm}
29 
30 procedure TForm1.Button1Click(Sender: TObject);
31 var
32   I: Integer;
33 begin
34   I := 0;
35   I := SizeOf(FInteger);
36   ShowMessage(IntToStr(I)); //4
37   I := SizeOf(FBool);
38   ShowMessage(IntToStr(I)); //1
39   I := SizeOf(FDouble);
40   ShowMessage(IntToStr(I)); //8
41   I := SizeOf(Button1);
42   ShowMessage(IntToStr(I)); //4
43   I := SizeOf(Edit1);
44   ShowMessage(IntToStr(I)); //4  
45   I := SizeOf(Form1);
46   ShowMessage(IntToStr(I)); //4
47   I := SizeOf(TForm1);
48   ShowMessage(IntToStr(I)); //4
49   I := Self.InstanceSize;
50   ShowMessage(IntToStr(I)); //888
51 end;
52 
53 end.
```
