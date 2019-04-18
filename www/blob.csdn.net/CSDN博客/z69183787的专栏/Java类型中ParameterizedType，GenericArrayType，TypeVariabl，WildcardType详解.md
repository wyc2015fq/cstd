# Java类型中ParameterizedType，GenericArrayType，TypeVariabl，WildcardType详解 - z69183787的专栏 - CSDN博客
2017年01月10日 15:13:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：893
摘要：Java类型中ParameterizedType，GenericArrayType，TypeVariabl，WildcardType详解
(1). 和反射+泛型有关的接口类型
- java.lang.reflect.Type：java语言中所有类型的公共父接口
- java.lang.reflect.ParameterizedType
- java.lang.reflect.GenericArrayType
- java.lang.reflect.WildcardType
1. Type直接子接口
ParameterizedType，GenericArrayType，TypeVariable和WildcardType四种类型的接口
- ParameterizedType: 表示一种参数化的类型，比如Collection
- GenericArrayType: 表示一种元素类型是参数化类型或者类型变量的数组类型
- TypeVariable: 是各种类型变量的公共父接口
- WildcardType: 代表一种通配符类型表达式，比如?,
 ? extends Number, ? super Integer【wildcard是一个单词：就是“通配符”】
2. Type直接实现子类 :Class类
## 3. java.lang.reflect.Type接口
Type所有类型指代的有：原始类型 (raw types)【对应Class】，参数化类型
 (parameterizedtypes)【对应ParameterizedType】， 数组类型 (array types)【对应GenericArrayType】，类型变量
 (type variables)【对应TypeVariable】，基本数据类型(primitivetypes)【仍然对应Class】
## 4. java.lang.reflect.ParameterizedType接口
ParameterizedType接口类型的含义
表示参数化类型。比如：Map这种参数化类型
获取参数化类型<>中的实际类型
源码声明：Type[] getActualTypeArguments();
【注意】无论<>中有几层<>嵌套，这个方法仅仅脱去最外层的<>之后剩下的内容就作为这个方法的返回值。
publicstatic  E methodIV(  
ArrayList<arraylist> al1,  
ArrayList al2,  
ArrayList<String> al3,  
ArrayList<? extends Number> al4,  
ArrayList[] al5 
那么他的每一参数总体上看都是参数化类型的。
{1}. 对于ArrayList<arraylist>，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是ArrayList。因此对这个参数的返回类型是ParameterizedType。
{2}. 对于ArrayList，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是E。因此对这个参数的返回类型是TypeVariable。
{3}. 对于ArrayList，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是String。因此对这个参数的返回类型是Class。
{4}. 对于ArrayList<? extends Number>，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是? extends Number。因此对这个参数的返回类型是WildcardType。
{5}. 对于ArrayList，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是E[]。因此对这个参数的返回类型是GenericArrayType。
所以，可能获得各种各样类型的实际参数，所以为了统一，采用直接父类数组Type[]进行接收。
## 4. java.lang.reflect. GenericArrayType接口
GenericArrayType接口类型的含义
表示泛型数组类型。比如：void method(ArrayList[] al){…}
【注意】<>不能出现在数组的初始化中，即new数组之后不能出现<>，否则javac无法通过。但是作为引用变量或者方法的某个参数是完全可以的。
获取泛型数组中元素的类型
源码声明：Type getGenericComponentType();
【注意】无论从左向右有几个[]并列，这个方法仅仅脱去最右边的[]之后剩下的内容就作为这个方法的返回值。
为什么返回值类型是Type？
publicstatic  E methodV(
String[] p1,
E[] p2,
ArrayList[] p3,
E[][] p4){}
{1}. 对于String[]，通过getComponentType()返回之后，脱去最右边的[]之后，剩余的类型是String。因此对这个参数的返回类型是Class
{2}. 对于E[]，通过getComponentType()返回之后，脱去最右边的[]之后，剩余的类型是E。因此对这个参数的返回类型是TypeVariable
{3}. 对于ArrayList[]，通过getComponentType()返回之后，脱去最右边的[]之后，剩余的类型是ArrayList。因此对这个参数的返回类型是ParameterizedType
{4}. 对于E[][]，通过getComponentType()返回之后，脱去最右边的[]之后，剩余的类型是E[]。因此对这个参数的返回类型是GenericArrayType
## 5. java.lang.reflect. GenericArrayType接口
TypeVariable接口类型的含义
表示类型参数或者又叫做类型变量。比如：void method(E e){}中的E就是类型变量
获取类型变量的泛型限定的上边界的类型
源码声明：Type[] getActualTypeArguments();
【注意】这里面仅仅是上边界。原因就是类型变量在定义的时候只能使用extends进行(多)边界限定。不能使用super，否则编译无法通过。同时extends给出的都是类型变量的上边界。
为什么是返回类型是数组？因为类型变量可以通过&进行多个上边界限定，因此上边界有多个，因此返回值类型是数组类型[
 ]。
例如下面的方法：
publicstatic <e extends Map&
 Cloneable&Serializable> E methodVI(E e){…}</e 
E的第一个上边界是Map,是ParameterizedType类型
E的第二个上边界是Cloneable，是Class类型
因此，为统一，返回值的数组的元素类型就是Type
## 6. java.lang.reflect.WildcardType接口
WildcardType接口类型的含义
表示通配符类型的表达式。
比如 void printColl(ArrayListal); 中的 ? extends Number
【注意】根据上面API的注释提示：现阶段通配符表达式仅仅接受一个上边界或者下边界，这个和定义类型变量时候可以指定多个上边界是不一样。但是API说了，为了保持扩展性，这里返回值类型写成了数组形式。实际上现在返回的数组的大小就是1
获取通配符表达式对象的泛型限定的上边界的类型
源码声明：Type[] getUpperBounds();
【注意】上面说了，现阶段返回的Type[ ]中的数组大小就是1个。写成Type[
 ]是为了语言的升级而进行的扩展。
例如下面的方法：
{1}. publicstaticvoid printColl(ArrayList<
 ? extends ArrayList> al){}
通配符表达式是：? extends ArrayList，这样 extends后面是?的上边界，这个上边界是ParameterizedType类型。
{2}. publicstaticvoid printColl(ArrayList<
 ? extends E> al){}
通配符表达式是：? extends E，这样 extends后面是?的上边界，这个上边界是TypeVariable类型
{3}.publicstaticvoidprintColl(ArrayList<
 ? extends E[]> al){}
通配符表达式是：? extends E[]，这样 extends后面是?的上边界，这个上边界是GenericArrayType类型
{4}.publicstaticvoid printColl(ArrayList<? extends Number>
 al){}
通配符表达式是：? extends Number，这样 extends后面是?的上边界，这个上边界是Class类型
最终统一成Type作为数组的元素类型。
## 7. Type及其子接口的来历
一. 泛型出现之前的类型
没有泛型的时候，只有所谓的原始类型。此时，所有的原始类型都通过字节码文件类Class类进行抽象。Class类的一个具体对象就代表一个指定的原始类型。
二. 泛型出现之后的类型
泛型出现之后，扩充了数据类型。从只有原始类型扩充了参数化类型、类型变量类型、泛型限定的的参数化类型 (含通配符+通配符限定表达式)、泛型数组类型。
三. 与泛型有关的类型不能和原始类型统一到Class的原因
[1]. 【产生泛型擦除的原因】
本来新产生的类型+原始类型都应该统一成各自的字节码文件类型对象。但是由于泛型不是最初Java中的成分。如果真的加入了泛型，涉及到JVM指令集的修改，这是非常致命的。
[2]. 【Java中如何引入泛型】
为了使用泛型的优势又不真正引入泛型，Java采用泛型擦除的机制来引入泛型。Java中的泛型仅仅是给编译器javac使用的，确保数据的安全性和免去强制类型转换的麻烦。但是，一旦编译完成，所有的和泛型有关的类型全部擦除。
[3]. 【Class不能表达与泛型有关的类型】
因此，与泛型有关的参数化类型、类型变量类型、泛型限定的的参数化类型 (含通配符+通配符限定表达式)、泛型数组类型这些类型全部被打回原形，在字节码文件中全部都是泛型被擦除后的原始类型，并不存在和自身类型一致的字节码文件。所以和泛型相关的新扩充进来的类型不能被统一到Class类中。
(4). 与泛型有关的类型在Java中的表示
为了通过反射操作这些类型以迎合实际开发的需要，Java就新增了ParameterizedType，GenericArrayType，TypeVariable 和WildcardType几种类型来代表不能被归一到Class类中的类型但是又和原始类型齐名的类型。
(5). Type的引入：统一与泛型有关的类型和原始类型Class
【引入Type的原因】
为了程序的扩展性，最终引入了Type接口作为Class，ParameterizedType，GenericArrayType，TypeVariable和WildcardType这几种类型的总的父接口。这样实现了Type类型参数接受以上五种子类的实参或者返回值类型就是Type类型的参数。
【Type接口中没有方法的原因】
从上面看到，Type的出现仅仅起到了通过多态来达到程序扩展性提高的作用，没有其他的作用。因此Type接口的源码中没有任何方法。
