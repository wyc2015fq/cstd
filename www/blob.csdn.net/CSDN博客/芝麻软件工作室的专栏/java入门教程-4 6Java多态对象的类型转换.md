
# java入门教程-4.6Java多态对象的类型转换 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月25日 08:55:42[seven-soft](https://me.csdn.net/softn)阅读数：129


这里所说的对象类型转换，是指存在继承关系的对象，不是任意类型的对象。当对不存在继承关系的对象进行强制类型转换时，java 运行时将抛出 java.lang.ClassCastException 异常。
在继承链中，我们将子类向父类转换称为“向上转型”，将父类向子类转换称为“向下转型”。
很多时候，我们会将变量定义为父类的类型，却引用子类的对象，这个过程就是向上转型。程序运行时通过动态绑定来实现对子类方法的调用，也就是多态性。
然而有些时候为了完成某些父类没有的功能，我们需要将向上转型后的子类对象再转成子类，调用子类的方法，这就是向下转型。
注意：不能直接将父类的对象强制转换为子类类型，只能将向上转型后的子类对象再次转换为子类类型。也就是说，子类对象必须向上转型后，才能再向下转型。请看下面的代码：publicclassDemo{
publicstaticvoidmain(Stringargs[]){
SuperClasssuperObj=newSuperClass();
SonClasssonObj=newSonClass();
// 下面的代码运行时会抛出异常，不能将父类对象直接转换为子类类型
// SonClass sonObj2 = (SonClass)superObj;
// 先向上转型，再向下转型
superObj=sonObj;
SonClasssonObj1=(SonClass)superObj;
}
}
classSuperClass{}
classSonClassextendsSuperClass{}

将第7行的注释去掉，运行时会抛出异常，但是编译可以通过。
因为向下转型存在风险，所以在接收到父类的一个引用时，请务必使用 instanceof 运算符来判断该对象是否是你所要的子类，请看下面的代码：publicclassDemo{
publicstaticvoidmain(Stringargs[]){
SuperClasssuperObj=newSuperClass();
SonClasssonObj=newSonClass();
// superObj 不是 SonClass 类的实例
if(superObjinstanceofSonClass){
SonClasssonObj1=(SonClass)superObj;
}else{
System.out.println("①不能转换");
}
superObj=sonObj;
// superObj 是 SonClass 类的实例
if(superObjinstanceofSonClass){
SonClasssonObj2=(SonClass)superObj;
}else{
System.out.println("②不能转换");
}
}
}
classSuperClass{}
classSonClassextendsSuperClass{}

运行结果：
①不能转换
总结：对象的类型转换在程序运行时检查，向上转型会自动进行，向下转型的对象必须是当前引用类型的子类。

