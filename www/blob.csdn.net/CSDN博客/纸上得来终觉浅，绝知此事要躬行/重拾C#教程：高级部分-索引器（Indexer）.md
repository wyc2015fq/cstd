# 重拾C#教程：高级部分-索引器（Indexer） - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 13:46:10[boonya](https://me.csdn.net/boonya)阅读数：95








原文地址：[http://www.runoob.com/csharp/csharp-indexer.html](http://www.runoob.com/csharp/csharp-indexer.html)




**索引器（Indexer）** 允许一个对象可以像数组一样被索引。当您为类定义一个索引器时，该类的行为就会像一个 **虚拟数组（virtual array）** 一样。您可以使用数组访问运算符（[ ]）来访问该类的实例。


## 语法

一维索引器的语法如下：
element-type this[int index]{// get 访问器get{// 返回 index 指定的值}// set 访问器set{// 设置 index 指定的值 }}
## 索引器（Indexer）的用途

索引器的行为的声明在某种程度上类似于属性（property）。就像属性（property），您可使用 **get** 和 **set** 访问器来定义索引器。但是，属性返回或设置一个特定的数据成员，而索引器返回或设置对象实例的一个特定值。换句话说，它把实例数据分为更小的部分，并索引每个部分，获取或设置每个部分。

定义一个属性（property）包括提供属性名称。索引器定义的时候不带有名称，但带有 **this** 关键字，它指向对象实例。下面的实例演示了这个概念：
usingSystem;namespaceIndexerApplication{classIndexedNames{privatestring[] namelist =newstring[size];staticpublicint size =10;publicIndexedNames(){for(int i =0; i < size; i++)
         namelist[i]="N. A.";}publicstringthis[int index]{get{string tmp;if( index >=0&& index <= size-1){
               tmp = namelist[index];}else{
               tmp ="";}return( tmp );}set{if( index >=0&& index <= size-1){
               namelist[index]= value;}}}staticvoidMain(string[] args){IndexedNames names =newIndexedNames();
         names[0]="Zara";
         names[1]="Riz";
         names[2]="Nuha";
         names[3]="Asif";
         names[4]="Davinder";
         names[5]="Sunil";
         names[6]="Rubic";for(int i =0; i <IndexedNames.size; i++){Console.WriteLine(names[i]);}Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
ZaraRizNuhaAsifDavinderSunilRubic
N. A.
N. A.
N. A.
## 重载索引器（Indexer）

索引器（Indexer）可被重载。索引器声明的时候也可带有多个参数，且每个参数可以是不同的类型。没有必要让索引器必须是整型的。C# 允许索引器可以是其他类型，例如，字符串类型。

下面的实例演示了重载索引器：
usingSystem;namespaceIndexerApplication{classIndexedNames{privatestring[] namelist =newstring[size];staticpublicint size =10;publicIndexedNames(){for(int i =0; i < size; i++){
          namelist[i]="N. A.";}}publicstringthis[int index]{get{string tmp;if( index >=0&& index <= size-1){
               tmp = namelist[index];}else{
               tmp ="";}return( tmp );}set{if( index >=0&& index <= size-1){
               namelist[index]= value;}}}publicintthis[string name]{get{int index =0;while(index < size){if(namelist[index]== name){return index;}
               index++;}return index;}}staticvoidMain(string[] args){IndexedNames names =newIndexedNames();
         names[0]="Zara";
         names[1]="Riz";
         names[2]="Nuha";
         names[3]="Asif";
         names[4]="Davinder";
         names[5]="Sunil";
         names[6]="Rubic";// 使用带有 int 参数的第一个索引器for(int i =0; i <IndexedNames.size; i++){Console.WriteLine(names[i]);}// 使用带有 string 参数的第二个索引器Console.WriteLine(names["Nuha"]);Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
ZaraRizNuhaAsifDavinderSunilRubic
N. A.
N. A.
N. A.2



