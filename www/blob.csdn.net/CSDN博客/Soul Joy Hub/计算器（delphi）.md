# 计算器（delphi） - Soul Joy Hub - CSDN博客

2016年06月13日 09:56:24[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：573


  1 unit Unit1;
  2  3interface
  4  5uses
  6  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  7  Dialogs, StdCtrls, bsSkinData, BusinessSkinForm, Mask, bsSkinBoxCtrls, bscalc,
  8  bsSkinCtrls;
  9 10type 11   TForm1 = class(TForm)
 12    Button2: TButton;
 13    Button3: TButton;
 14    Button4: TButton;
 15    Button5: TButton;
 16    Button6: TButton;
 17    Button7: TButton;
 18    Button8: TButton;
 19    Button9: TButton;
 20    Button10: TButton;
 21    Button11: TButton;
 22    Button12: TButton;
 23    Button13: TButton;
 24    Button14: TButton;
 25    Button15: TButton;
 26    Button16: TButton;
 27    Button17: TButton;
 28    Button18: TButton;
 29    Button19: TButton;
 30    Button20: TButton;
 31    Button21: TButton;
 32    Button22: TButton;
 33    bsBusinessSkinForm1: TbsBusinessSkinForm;
 34    bsSkinData1: TbsSkinData;
 35    Edit1: TbsSkinEdit;
 36    bsCompressedStoredSkin1: TbsCompressedStoredSkin;
 37    Edit2: TbsSkinEdit;
 38    Edit3: TbsSkinEdit;
 39    Edit4: TbsSkinEdit;
 40    Label1: TLabel;
 41    Button1: TButton;
 42    Button23: TButton;
 43procedure Button1Click(Sender: TObject);
 44procedure FormCreate(Sender: TObject);
 45procedure Button2Click(Sender: TObject);
 46procedure Button3Click(Sender: TObject);
 47procedure Button4Click(Sender: TObject);
 48procedure Button5Click(Sender: TObject);
 49procedure Button6Click(Sender: TObject);
 50procedure Button7Click(Sender: TObject);
 51procedure Button8Click(Sender: TObject);
 52procedure Button9Click(Sender: TObject);
 53procedure Button10Click(Sender: TObject);
 54procedure Button11Click(Sender: TObject);
 55procedure Button12Click(Sender: TObject);
 56procedure Button13Click(Sender: TObject);
 57procedure Button14Click(Sender: TObject);
 58procedure Button15Click(Sender: TObject);
 59procedure Button16Click(Sender: TObject);
 60procedure Edit1Change(Sender: TObject);
 61procedure Edit3Change(Sender: TObject);
 62procedure Edit1Enter(Sender: TObject);
 63procedure Edit3Enter(Sender: TObject);
 64procedure Button17Click(Sender: TObject);
 65procedure Button18Click(Sender: TObject);
 66procedure Button19Click(Sender: TObject);
 67procedure Button20Click(Sender: TObject);
 68procedure Button21Click(Sender: TObject);
 69procedure Button22Click(Sender: TObject);
 70procedure bsSkinButton1Click(Sender: TObject);
 71procedure Button23Click(Sender: TObject);
 72  private
 73{ Private declarations } 74  public
 75{ Public declarations } 76 77end;
 78 79var 80  Form1: TForm1;
 81  str1:String;
 82   str2:String;
 83    int1:real;
 84     int2:real;
 85     bo:real;
 86    result:real;
 87implementation
 88 89{$R *.dfm} 90 91procedure TForm1.Button1Click(Sender: TObject);
 92begin 93 94if bo=0then 95begin 96   str1:=str1+'1';
 97   Edit1.Text:=str1;
 98end 99Else
100begin101   str2:=str2+'1';
102   Edit3.Text:=str2;
103end104105106end;
107108procedure TForm1.FormCreate(Sender: TObject);
109begin110111 int1:=0;
112  int2:=0;
113  bo:=0;
114  SetWindowLong(Edit1.Handle,GWL_STYLE,GetWindowLong(Edit1.Handle,GWL_STYLE) or Es_right);
115  SetWindowLong(Edit2.Handle,GWL_STYLE,GetWindowLong(Edit2.Handle,GWL_STYLE) or Es_right);
116  SetWindowLong(Edit3.Handle,GWL_STYLE,GetWindowLong(Edit3.Handle,GWL_STYLE) or Es_right);
117  SetWindowLong(Edit4.Handle,GWL_STYLE,GetWindowLong(Edit4.Handle,GWL_STYLE) or Es_right);
118end;
119120procedure TForm1.Button20Click(Sender: TObject);
121begin122 edit2.Text:='';
123 edit3.Text:='';
124 int2:=0;
125 edit4.Text:= FormatFloat('0.00000',1/int1) ;
126end;
127128procedure TForm1.Button21Click(Sender: TObject);
129begin130   edit2.Text:='';
131 edit3.Text:='';
132 int2:=0;
133 edit4.Text:= FormatFloat('0.00000',Sqrt(int1)) ;
134end;
135136procedure TForm1.Button22Click(Sender: TObject);
137begin138 result:=int1*int2/100;
139  Edit4.Text:=FormatFloat('0.00000',result);
140end;
141142procedure TForm1.Button23Click(Sender: TObject);
143begin144if bo=0then145     edit1.Text:=''146else147     edit3.Text:='';
148149end;
150151procedure TForm1.Button2Click(Sender: TObject);
152153154begin155if bo=0then156begin157   str1:=str1+'2';
158   Edit1.Text:=str1;
159end160Else
161begin162   str2:=str2+'2';
163   Edit3.Text:=str2;
164end165end;
166167procedure TForm1.Button3Click(Sender: TObject);
168begin169if bo=0then170begin171   str1:=str1+'3';
172   Edit1.Text:=str1;
173end174Else
175begin176   str2:=str2+'3';
177   Edit3.Text:=str2;
178end179end;
180181procedure TForm1.Button4Click(Sender: TObject);
182begin183if bo=0then184begin185   str1:=str1+'4';
186   Edit1.Text:=str1;
187end188Else
189begin190   str2:=str2+'4';
191   Edit3.Text:=str2;
192end193end;
194195procedure TForm1.Button5Click(Sender: TObject);
196begin197if bo=0then198begin199   str1:=str1+'5';
200   Edit1.Text:=str1;
201end202Else
203begin204   str2:=str2+'5';
205   Edit3.Text:=str2;
206end207end;
208209procedure TForm1.Button6Click(Sender: TObject);
210begin211if bo=0then212begin213   str1:=str1+'6';
214   Edit1.Text:=str1;
215end216Else
217begin218   str2:=str2+'6';
219   Edit3.Text:=str2;
220end221end;
222223procedure TForm1.Button7Click(Sender: TObject);
224begin225if bo=0then226begin227   str1:=str1+'7';
228   Edit1.Text:=str1;
229end230Else
231begin232   str2:=str2+'7';
233   Edit3.Text:=str2;
234end235end;
236237procedure TForm1.Button8Click(Sender: TObject);
238begin239if bo=0then240begin241   str1:=str1+'8';
242   Edit1.Text:=str1;
243end244Else
245begin246   str2:=str2+'8';
247   Edit3.Text:=str2;
248end249end;
250251procedure TForm1.Button9Click(Sender: TObject);
252begin253if bo=0then254begin255   str1:=str1+'9';
256   Edit1.Text:=str1;
257end258Else
259begin260   str2:=str2+'9';
261   Edit3.Text:=str2;
262end263end;
264265procedure TForm1.Edit1Change(Sender: TObject);
266begin267     str1:=edit1.Text;
268if edit1.Text<>''then269    int1:=strtofloat(edit1.Text)
270else271    int1:=0;
272273end;
274275procedure TForm1.Edit1Enter(Sender: TObject);
276begin277     bo:=0;
278end;
279280procedure TForm1.Edit3Change(Sender: TObject);
281begin282    str2:=edit3.Text;
283if edit3.Text<>''then284  int2:=strtofloat(edit3.Text)
285else286    int2:=0;
287288end;
289290procedure TForm1.Edit3Enter(Sender: TObject);
291begin292   bo:=1;
293end;
294295procedure TForm1.bsSkinButton1Click(Sender: TObject);
296begin297if bo=0then298begin299   str1:=str1+'1';
300   Edit1.Text:=str1;
301end302Else
303begin304   str2:=str2+'1';
305   Edit3.Text:=str2;
306end307end;
308309procedure TForm1.Button10Click(Sender: TObject);
310begin311if bo=0then312begin313   str1:=str1+'0';
314   Edit1.Text:=str1;
315end316Else
317begin318   str2:=str2+'0';
319   Edit3.Text:=str2;
320end321end;
322323procedure TForm1.Button11Click(Sender: TObject);
324begin325  bo:=1;
326 Edit2.Text:='+';
327end;
328329procedure TForm1.Button12Click(Sender: TObject);
330begin331 bo:=1;
332 Edit2.Text:='-';
333end;
334335procedure TForm1.Button13Click(Sender: TObject);
336begin337 bo:=1;
338 Edit2.Text:='*';
339end;
340341procedure TForm1.Button14Click(Sender: TObject);
342begin343 bo:=1;
344 Edit2.Text:='/';
345end;
346347procedure TForm1.Button15Click(Sender: TObject);
348begin349350351if  Edit2.Text='+'then352begin353      result:=int1+int2;
354      Edit4.Text:=floattostr(result);
355end ;
356357358if  Edit2.Text='-'then359begin360      result:=int1-int2;
361      Edit4.Text:=floattostr(result);
362end ;
363if  Edit2.Text='*'then364begin365      result:=int1*int2;
366      Edit4.Text:=floattostr(result);
367end ;
368if ( Edit2.Text='/') and (int2<>0 ) then369begin370      result:=int1/int2;
371{保留5位小数}372     Edit4.Text:=FormatFloat('0.00000',result);
373end  ;
374375if ( Edit2.Text='/') and (int2=0 ) then376begin377        Application.MessageBox('除数不能为0!','错误提示',MB_OKCANCEL);
378end  ;
379380381end;
382383384procedure TForm1.Button16Click(Sender: TObject);
385begin386387388    bo:=0;
389    Edit1.Text:='';
390    Edit2.Text:='';
391    Edit3.Text:='';
392    Edit4.Text:='';
393    int1:=0;
394    int2:=0;
395    result:=0;
396end;
397398procedure TForm1.Button17Click(Sender: TObject);
399begin400if bo=0then401       edit1.Text:= Copy( edit1.Text, 1, length( Edit1.text )-1 ) {截取}402else403        edit3.Text:= Copy( edit3.Text, 1, length( Edit3.text )-1 );
404405406end;
407408procedure TForm1.Button18Click(Sender: TObject);
409begin410if bo=0then411begin412   str1:=str1+'.';
413   Edit1.Text:=str1;
414end415Else
416begin417   str2:=str2+'.';
418   Edit3.Text:=str2;
419end420end;
421422procedure TForm1.Button19Click(Sender: TObject);
423begin424if bo=0then425begin426if int1>0then427     edit1.Text:='-'+edit1.Text
428else429begin430       int1:=0-strtofloat(edit1.Text);
431       edit1.Text:=floattostr(int1) ;
432end;
433434end435436else437begin438if int2>0then439     edit3.Text:='-'+edit3.Text
440else441begin442       int2:=0-strtofloat(edit3.Text);
443       edit3.Text:=floattostr(int2) ;
444end;
445end;
446end;
447448end.

