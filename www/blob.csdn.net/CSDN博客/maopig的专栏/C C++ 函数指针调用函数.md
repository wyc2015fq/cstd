# C/C++ 函数指针调用函数 - maopig的专栏 - CSDN博客
2011年09月06日 19:02:52[maopig](https://me.csdn.net/maopig)阅读数：594
|`01`|`//C/C++ 函数指针调用函数`|
|`02`|`#include<iostream>`|
|`03`|`using````namespace````std;`|
|`04`||
|`05`|`void``site1()`|
|`06`|`{`|
|`07`|`    ``cout<<``"www.ok2002.com"``<<endl;`|
|`08`|`}`|
|`09`||
|`10`|`void``site2()`|
|`11`|`{`|
|`12`|`    ``cout<<``"www.ok1700.com"``<<endl;`|
|`13`|`}`|
|`14`||
|`15`|`void``site3()`|
|`16`|`{`|
|`17`|`    ``cout<<``"www.ok1500.com"``<<endl;`|
|`18`|`}`|
|`19`||
|`20`|`void``main()`|
|`21`|`{`|
|`22`|`    ``typedef``void``(*p)();`|
|`23`|`    ``p v[]={site1,site2,site3};`|
|`24`|`    ``int`` size_v=``sizeof``(v)/``sizeof``(v[0]);`|
|`25`|`    ``cout<<``"size_v="``<<size_v<<endl;`|
|`26`|`    ``int``i=0;`|
|`27`|`    ``for``(;i<size_v;++i)`|
|`28`|`        ``v[i]();`|
|`29`|`}`|
|`30`||
|`31`|`/*--------------------------`|
|`32`|`size_v=3`|
|`33`|`www.ok2002.com`|
|`34`|`www.ok1700.com`|
|`35`|`www.ok1500.com`|
|`36`|`Press any key to continue`|
|`37`|`--------------------------*/`|
