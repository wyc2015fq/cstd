
# java入门教程-5.5Java接口和抽象类的区别 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月25日 09:04:24[seven-soft](https://me.csdn.net/softn)阅读数：738


类是对象的模板，抽象类和接口可以看做是具体的类的模板。
由于从某种角度讲，接口是一种特殊的抽象类，它们的渊源颇深，有很大的相似之处，所以在选择使用谁的问题上很容易迷糊。我们首先分析它们具有的相同点。都代表类树形结构的抽象层。在使用引用变量时，尽量使用类结构的抽象层，使方法的定义和实现分离，这样做对于代码有松散耦合的好处。
都不能被实例化。
都能包含抽象方法。抽象方法用来描述系统提供哪些功能，而不必关心具体的实现。
下面说一下抽象类和接口的主要区别。
1) 抽象类可以为部分方法提供实现，避免了在子类中重复实现这些方法，提高了代码的可重用性，这是抽象类的优势；而接口中只能包含抽象方法，不能包含任何实现。publicabstractclassA{
publicabstractvoidmethod1();
publicvoidmethod2(){
//A method2
}
}
publicclassBextendsA{
publicvoidmethod1(){
//B method1
}
}
publicclassCextendsA{
publicvoidmethod1(){
//C method1
}
}

抽象类A有两个子类B、C，由于A中有方法method2的实现，子类B、C中不需要重写method2方法，我们就说A为子类提供了公共的功能，或A约束了子类的行为。method2就是代码可重用的例子。A 并没有定义 method1的实现，也就是说B、C
 可以根据自己的特点实现method1方法，这又体现了松散耦合的特性。
再换成接口看看：publicinterfaceA{
publicvoidmethod1();
publicvoidmethod2();
}
publicclassBimplementsA{
publicvoidmethod1(){
//B method1
}
publicvoidmethod2(){
//B method2
}
}
publicclassCimplementsA{
publicvoidmethod1(){
//C method1
}
publicvoidmethod2(){
//C method2
}
}

接口A无法为实现类B、C提供公共的功能，也就是说A无法约束B、C的行为。B、C可以自由地发挥自己的特点现实 method1和 method2方法，接口A毫无掌控能力。
2) 一个类只能继承一个直接的父类（可能是抽象类），但一个类可以实现多个接口，这个就是接口的优势。interfaceA{
publicvoidmethod2();
}
interfaceB{
publicvoidmethod1();
}
classCimplementsA,B{
publicvoidmethod1(){
//C method1
}
publicvoidmethod2(){
//C method2
}
}
//可以如此灵活的使用C，并且C还有机会进行扩展，实现其他接口
Aa=newC();
Bb=newC();
abstractclassA{
publicabstractvoidmethod1();
}
abstractclassBextendsA{
publicabstractvoidmethod2();
}
classCextendsB{
publicvoidmethod1(){
//C method1
}
publicvoidmethod2(){
//C method2
}
}

对于C类，将没有机会继承其他父类了。
综上所述，接口和抽象类各有优缺点，在接口和抽象类的选择上，必须遵守这样一个原则：行为模型应该总是通过接口而不是抽象类定义，所以通常是优先选用接口，尽量少用抽象类。
选择抽象类的时候通常是如下情况：需要定义子类的行为，又要为子类提供通用的功能。

