# Java类型中ParameterizedType，GenericArrayType，TypeVariabl，WildcardType详解 - weixin_33985507的博客 - CSDN博客
2016年03月22日 17:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
(1). 和反射+泛型有关的接口类型
java.lang.reflect.**Type：**java语言中所有类型的**公共父接口**
java.lang.reflect.**ParameterizedType**
java.lang.reflect.**GenericArrayType**
java.lang.reflect.**WildcardType**
- **Type直接子接口**
**ParameterizedType，GenericArrayType，TypeVariable和WildcardType**四种类型的接口
**ParameterizedType**: 表示一种**参数化的类型**，比如Collection
**GenericArrayType**: 表示一种**元素类型**是**参数化类型**或者**类型变量**的**数组类型**
**TypeVariable**: 是**各种类型变量**的**公共父接口**
**WildcardType**: 代表**一种通配符类型表达式，**比如**?, ? extends Number, ? super Integer【wildcard是一个单词：就是“通配符”】**
- **Type直接实现子类 :Class类**
****3. java.lang.reflect.Type接口
**Type所有类型**指代的有：**原始类型 (**raw types**)【对应Class】**，**参数化类型 (**parameterizedtypes**)【对应ParameterizedType】， 数组类型 (**array types**)【对应GenericArrayType】，类型变量 (**type variables**)【对应TypeVariable】，基本数据类型(**primitivetypes**)【仍然对应Class】**
****4. java.lang.reflect.ParameterizedType接口
**ParameterizedType**接口类型的**含义**
表示**参数化类型。**比如：Map这种参数化类型
获取参数化类型<>**中的实际类型**
**源码声明**：Type[] getActualTypeArguments();
**【注意】无论<>中有几层<>嵌套**，这个方法**仅仅脱去最外层的****<>**之后**剩下的内容**就作为**这个方法的返回值**。
public static  E methodIV(
ArrayList> al1,
ArrayList al2,
ArrayList al3,
ArrayListextends Number> al4,
ArrayList al5){} 那么他的每一参数**总体上看**都是**参数化类型**的。
{1}. 对于ArrayList>，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是**ArrayList。**因此对这个参数的返回类型是**ParameterizedType**。
{2}. 对于ArrayList，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是**E。**因此对这个参数的返回类型是**TypeVariable**。
{3}. 对于ArrayList，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是**String。**因此对这个参数的返回类型是**Class**。
{4}. 对于ArrayListextends Number>，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是**? ExtendsNumber。**因此对这个参数的返回类型是**WildcardType**。
{5}. 对于ArrayList，通过getActualTypeArguments()返回之后，脱去最外层的<>之后，剩余的类型是**E[]。**因此对这个参数的返回类型是**GenericArrayType**。
所以，可能获得各种各样类型的实际参数，所以为了统一，采用直接父类数组Type[]进行接收。
- java.lang.reflect. GenericArrayType接口
GenericArrayType接口类型的含义
表示**泛型数组类型。**比如：void method(ArrayList[] al){…}****
**【注意】<>不能出现在数组的初始化**中，即**new数组之后不能出现<>**，否则javac无法通过。但是**作为引用变量**或者**方法的某个参数**是**完全可以的**。
获取泛型数组中元素的类型
**源码声明**：Type getGenericComponentType();
**【注意】无论从左向右有几个[]并列**，这个方法**仅仅脱去最右边的****[]**之后**剩下的内容**就作为**这个方法的返回值**。
为什么**返回值类型**是**Type**？
**public static**  E methodV(
String[] p1,
E[] p2,
ArrayList[] p3,
E[][] p4){}
{1}. 对于String[]，通过getComponentType()返回之后，脱去最右边的[]之后，剩余的类型是**String。**因此对这个参数的返回类型是**Class**
{2}. 对于E[]，通过getComponentType()返回之后，脱去最右边的[]之后，剩余的类型是**E。**因此对这个参数的返回类型是**TypeVariable**
{3}. 对于ArrayList[]，通过getComponentType()返回之后，脱去最右边的[]之后，剩余的类型是**ArrayList。**因此对这个参数的返回类型是**ParameterizedType**
{4}. 对于E[][]，通过getComponentType()返回之后，脱去最右边的[]之后，剩余的类型是**E[]。**因此对这个参数的返回类型是**GenericArrayType**
- java.lang.reflect. GenericArrayType接口
TypeVariable接口类型的含义
表示**类型参数**或者又叫做**类型变量。**比如：void method(E e){}中的E就是**类型变量**
获取类型变量的泛型限定的上边界的类型
**源码声明**：Type[] getActualTypeArguments();
**【注意】**这里面**仅仅是上边界**。原因就是**类型变量**在**定义的时候只能使用extends进行(多)边界限定**。**不能使用super，否则编译无法通过。**同时**extends给出的都是类型变量**的**上边界**。
为什么是返回类型是**数组**？因为**类型变量**可以**通过&**进行**多个上边界限定**，因此**上边界有多个**，因此返回值类型是**数组类型[ ]**。
例如下面的方法：
**public static** extends Map& Cloneable&Serializable> E methodVI(E e){…}
E的第一个上边界是Map,是ParameterizedType类型
E的第二个上边界是Cloneable，是Class类型
因此，为统一，返回值的数组的元素类型就是Type
- java.lang.reflect.WildcardType接口
WildcardType接口类型的含义
表示**通配符类型的表达式。**
比如** void printColl(ArrayListal); **中的 **? extends Number**【**注意**】根据上面API的注释提示：现阶段通配符表达式仅仅接受一个上边界或者下边界，这个和定义类型变量时候可以指定多个上边界是不一样。但是API说了，为了保持扩展性，这里返回值类型写成了数组形式。**实际上现在返回的数组的大小就是1**
获取通配符表达式对象的泛型限定的上边界的类型
**源码声明**：Type[] getUpperBounds();
【注意】上面说了，**现阶段返回的****Type[ ]**中的**数组大小**就是1个。写成Type[ ]是为了语言的升级而进行的扩展。
例如下面的方法：
{1}.** public static void**printColl(ArrayListextends ArrayList> al){}
**通配符表达式**是：? **extends**ArrayList，这样 extends后面是?的上边界，这个上边界是**ParameterizedType**类型。
{2}. **public static****void**printColl(ArrayListextends E> al){}
**通配符表达式**是：? **extends** E，这样 extends后面是?的上边界，这个上边界是**TypeVariable**类型
{3}.**public static****void**printColl(ArrayListextends E[]> al){}
**通配符表达式**是：? **extends** E[]，这样 extends后面是?的上边界，这个上边界是**GenericArrayType**类型
{4}.**public static****void**printColl(ArrayListextends Number> al){}
**通配符表达式**是：? **extends** Number，这样 extends后面是?的上边界，这个上边界是**Class**类型
最终统一成Type作为数组的元素类型。
- Type及其子接口的来历
一. 泛型出现之前的类型
**没有泛型**的时候，**只有**所谓的**原始类型**。此时，所有的**原始类型**都通过**字节码文件类Class类进行抽象**。**Class类的一个具体对象就代表一个指定的原始类型**。
二. 泛型出现之后的类型
**泛型出现之后**，扩充了数据类型。**从只有原始类型**扩充了**参数化类型**、**类型变量类型**、**泛型限定的的参数化类型 (含通配符+通配符限定表达式)**、**泛型数组类型**。
三. 与泛型有关的类型不能和原始类型统一到Class的原因
[1]. 【**产生泛型擦除的原因**】
本来**新产生的类型**+**原始类型都应该统一成各自的字节码文件类型对象**。但是**由于泛型不是最初Java中的成分。如果真的加入了泛型，涉及到JVM指令集的修改，这是非常致命的。**
[2]. 【**Java中如何引入泛型**】
为了**使用泛型的优势又不真正引入泛型**，Java采用**泛型擦除的机制**来引入泛型。Java中的泛型仅仅是给编译器javac使用的，**确保数据的安全性和免去强制类型转换的麻烦**。但是，一旦编译完成，所有的**和泛型有关的类型全部擦除**。
[3]. 【**Class不能表达与泛型有关的类型**】
因此，**与泛型有关**的**参数化类型**、**类型变量类型**、**泛型限定的的参数化类型 (含通配符+通配符限定表达式)**、**泛型数组类型这些类型全部被打回原形，在字节码文件中全部都是泛型被擦除后的原始类型，并不存在和自身类型一致的字节码文件。**所以**和泛型相关的新扩充进来的类型不能被统一到Class类**中。
(4). 与泛型有关的类型在Java中的表示
为了通过**反射操作这些类型**以迎合实际开发的需要，Java就**新增了ParameterizedType**，**GenericArrayType**，**TypeVariable** 和**WildcardType**几种类型来**代表不能被归一到Class类中的类型但是又和原始类型齐名的类型**。
(5). Type的引入：统一与泛型有关的类型和原始类型Class
【**引入Type的原因**】
为了程序的扩展性，最终引入了**Type接口**作为**Class**，**ParameterizedType**，**GenericArrayType**，**TypeVariable**和**WildcardType**这几种类型的**总的父接口**。这样实现了Type类型参数接受以上五种子类的实参或者返回值类型就是Type类型的参数。
【**Type接口中没有方法的原因**】
从上面看到，**Type的出现仅仅起到了通过多态**来达到**程序扩展性提高**的作用，**没有其他的作用**。因此Type接口的源码中没有任何方法。
