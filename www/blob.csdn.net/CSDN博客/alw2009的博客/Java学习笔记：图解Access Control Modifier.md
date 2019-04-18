# Java学习笔记：图解Access Control Modifier - alw2009的博客 - CSDN博客





2016年09月19日 12:37:16[traveler_zero](https://me.csdn.net/alw2009)阅读数：308








Access Levels



|Modifier|Class|Package|Subclass|World|
|----|----|----|----|----|
|`public`|Y|Y|Y|Y|
|`protected`|Y|Y|Y|N|
|no modifier|Y|Y|N|N|
|`private`|Y|N|N|N|
![](https://docs.oracle.com/javase/tutorial/figures/java/classes-access.gif)

Visability


|Modifier|Alpha|Beta|Alphasub|Gamma|
|----|----|----|----|----|
|`public`|Y|Y|Y|Y|
|`protected`|Y|Y|Y|N|
|no modifier|Y|Y|N|N|
|`private`|Y|N|N|N|




