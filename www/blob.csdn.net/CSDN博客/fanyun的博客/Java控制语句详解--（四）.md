# Java控制语句详解--（四） - fanyun的博客 - CSDN博客
2016年03月26日 09:22:11[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：888
所属专栏：[Java框架精要](https://blog.csdn.net/column/details/java-01.html)
一、True和False
       Java中不允许讲一个非布尔(boolean)类型的的变量当做布尔值使用，虽然C和C++中可以(True=非零，False=0)，若想在布尔测试中使用一个非布尔值，比如if(a)中，那么必须要加上一个条件表达式将其转化成布尔值，例如if(!=0)
二、If-else
      if-else是控制流程的最基本形式
      if(Boolean-expression)
             statement
若if后面扩后内boolean值为True则执行statement,statement是指用分号分开的的单句，或符合语句，复杂句应被扩在大括号中
      If(Boolean-expression){
                    ……..;
                    ……..;
       }
使用if-else判断两数字是大于小于或等于
public class IfElse{
       staticint result = 0;
       staticvoid test(int  testval,int  target){
             if(testvlal>target)
                    result = +1;
             else if(testval<target)
                    result = -1;
             else
                    result = 0; //相等
}
public static void main(String [] args){
      test(10,5);
      System.out.println(result);
      test(5,10);
      System.out.println(result);
      test(5,5);
      System.out.println(result);
}
}
输出
1
-1
0
上例中的else if的意思是在else后紧跟一个if语句。
迭代语句
Java流程控制中while,do-while,for用来控制循环,有时将他们划分为迭代语句(iterationstatement)。用大括号扩住需要重复执行的语句，被扩住的语句会一直重复执行，直到起控制作用的布尔表达式（Boolean-expression）得到False的结果为止。
三、while
while的循环格式如下
while(Boolean-expression)
             statement;
       循环开始时，会判断一遍布尔表达式的值，如果为True执行语句，迭代后会从新判断布尔值，直到为false，循环结束。
下面的例子是判断生成的随机数是否符合条件
public class WhileTest{
static Boolean condition(){
      Boolean result=Math.random() < 0.99;
      System.out.print(result+”.”);
       returnresult;
}
public static void main(String [] args){
      while(condition())
             System.out.print(“循环中 ‘while”);
      System.out.print(“退出 ‘’while”);
}
}
}
condition()方法里面用到了Math库里面的static(静态)方法random(),该方法的作用是产生0和1之间（包括0,但不包括1）的一个double值。result的值是通过比较操作符<而得到的，这个操作符将产生boolean类型的结果
四、do-while
do-while的格式如下:
       do
             statement
      while(Boolean-expression);
       区别于while循环，do-while循环至少会循环一次，比如循环开始时布尔值就为false,while循环会直接停止循环，而do-while会循环一次。
for
for循环是三种循环中最常用到的迭代形式,这种循环在第一次迭代之前要进行初始化。随后，它会进行条件测试，而且在每一次迭代结束时，进行某种形式的”进步”。for循环的格式如下
for(initialization;Boolean-expression;step)
      statement
初始化(initialization)表达式,布尔表达式(Boolean-expression)，或者进步(step)运算都可以为空。但是表达式后面的”；”不可以不写。每次迭代前会测试布尔表达式。若获得的结果是true,就会执行for语句后面的代码行。每次循环结束，会执行一次进步
for循环常用于执行”计数”任务:
public class ListCharacters{
       publicstatic void main(String [] args){
             for(char c=0;c<128;c++)
                    if(Character.isLowerCase(c))
                           System.out.println(“value: ”+(int)c+” character: ” + c);
}
}
输出:
value: 97 character : a
value: 98 character : b
switch
switch可被归纳为一种选择语句。根据正式表达式的值（也可以被<int型的变量代替），根据表达式的值，swtich语句可以从一系列代码选出一段去执行。
switch的语法如下
switch(integral-selector){
       caseintegrak-value1 : statement;break;
       caseintegrak-value2 : statement;break;
       caseintegrak-value3 : statement;break;
       caseintegrak-value4 : statement;break;
       //….
      default:statement;
}
      integral-selector选择因子是一个能够产生整数值的表达式,switch能将这个表达式的结果与每个integral-value相比较，若发现相同的，就会执行该case后的语句，若没有相符的则会执行default后面的语句。
       其中每一个case后面均有一个break结尾，这会让语句跳转至switch的末尾，这里break是可选的,如果不写break语句会继续执行接下来的case,直到遇到break位置。最后的default后面没有加break因为这里也是switch的末尾，也可以跟default后面加break，不过没有任何意义。
public class VowelsAndConsonants{
       publicstatic void main(String [] args){
             Random rand =new Random(47);
             for(int i=0; i < 100;i++){
                    int c=rand.nextInt(26)+ ‘a’;
                    System.out.print((char)c+ “,” + c + “:”);
                    switch(c){
                          case ‘a’:
                           case ‘e’:
                           case ‘i’:
                           case ‘o’:
                           case ‘u’:System.out.print(“vowel”);break;
                           case ‘y’:
                           case ‘w’:System.out.print(“Sometimes a vowel”);break;
                           default: System.out.print(“consonant”);
                    }
}
}
}
输出
y,121: Sometimes a vowel
这里a为偏移量，因为随机数会产生0-26任意数字，加上a后就会偏移值小写字母中
五、foreach
foreach语法可以不必创建int变量去对由访问项构成的序列进行计数，foreach将自动遍历获得每一项
foreach语法如下
for(变量类型 x : 该变量的序列){
      statement
}
用foreach遍历float数组
public class ForEachFloat{
       publicstatic void main(String [] args){
             Random rand = new Random(47);
             float f[] = new float[10];
             for(int i=0;i < 10;i++){
                    f[i]=rand.nextFloat();
}
for(float x : f)
      System.out.println(x);
}
}
输出
0.32454357.
. //共10行
如上例所示
for(float x : f){
       这条语句定义了一个名为x的float型变量,而后将每一个值遍历到x  
}
任何数组或返回值是数组的方法都可以应用于foreach。
public class ForEachString{
      publicn static void main(String [] args){
             for(char c : “Hello Word”.toCharArray()){
                    System.out.print(c+” ”);
}
}
}
输出
H e l l o  W o r d
但foreach不能直接对不存在的一组逻辑数字进行遍历，但for却可以
例如 for(int i=0;i < 100;i++)
如果想直接对数字进行遍历需要创建int型的数组，但可以做个静态方法自动根据传入数字来建立数组
例如for(int i : range(10)) 
这种方法虽然可以使得foreach更通用,但是这种方法会降低计算机效率。
六、return
Java中有多个关键词表示无条件分支，他们只是表示这个分支无需任何测试即可发生。这些关键词包括return,break,continue
       return关键词有两个作用,一是指定一个方法返回什么值(void方法除外),二是它会导致当前的方法退出，并返回值。可使用return关键词改写if-else中的例子
       publicclass Return{
       staticint test(int  testval,int  target){
             if(testvlal>target)
                    return 1;
             else if(testval<target)
                    return -1;
             else
                    return 0; //相等
}
public static void main(String [] args){
      System.out.println(test(10,5););
      System.out.println(test(5,10));
      System.out.println(test(5,5));
}
}
输出
1
-1
0
也可以省略掉else因为return后方法不再执行
       如果在返回void的方法中没有return语句，那么在该方法的结尾处会有一个隐藏的return,因此在方法中并非总是必须有一个return语句。但是，如果一个方法声明它将返回void之外的其他东西，那么必须确保每一条代码路径都将返回一个值。
break和continue
任何迭代语句的主体部分，都可以用break和continue来控制循环的流程.其中，break可以使程序强行退出，不执行循环中剩余的语句。而continue则会停止指定当前的循环，然后退回到循环的起始，开始下一次循环。
public class BreakContnue{
       publicstatic void main(String [] args){
             for(int i = 0;i < 100;i++){
                    if(i == 74) break;
                    if(i % 9 !=0) continue;
                    System.out.print(i+” “);
}
}
System.out.println();
int i = 0;
while(true){
       i++;
       int j= i * 27;
       if(j== 1269) break;
       if(i%10 !=0) continue;
      System.out.print(i+” ”);
}
}
输出
0 9 18 27 36 45 54 63 72
10 20 30 40
在上例的for循环中i的值达到74后就会触发break;结束循环因此i的值永远没法达到100,而每当i不能被9整除时便会触发continue;因为会跳过之后的代码，即输出部分，直接进入下一次循环(i的值增加)，只有能被9整除且小于74的数才会被输出。
后面的便是无限while循环因为括号内条件永远为true，但是循环中有一个break；可以控制循环结束。
还有一种无限循环的形式是for(;;)。编译器将while(true)和for(;;)看做是同一回事。所以用哪个取决于自己的习惯。
标签
在java中标签作用的地方时在迭代语句之前。在标签和迭代之前置入任何语句都是不好的。而在迭代之前设置标签的唯一理由是：我们希望潜逃另一个迭代或者一个开关。这是由于break和continue关键词通常只中断当前循环，但若随标签一起使用，他们就会中断循环，知道标签所在的地方:
标签的语法
label1:
label1:
outer-iteration{
      inner-iteration{
       //….
      break;//(1)
       //…..
      continue;//(2)
       //…..
       breaklabel1;//(3)
       //…..
      continue label1;//(4)
       在(1)中break会中断内层循环回到外部循环，在(2)中continue回结束内层循环的本次循环回到内部循环的起始点。在(3)中，break lable1中因为break后面加了label1所以会直接中断所有循环回到标签处，但不重新进入循环，也就是说直接终止了两个循环，而(4)中continue label1则会直接中断内部与外部循环回到标签处,随后继续循环，但是从外部循环开始。
public class labeledFor{
       publicstatic void main(String [] args){
             int i = 0;
             outer:
             for(;;){
                    inner:
                    for(;i<10;i++){
             System.out.print(“i=”+i);
             if(i == 2){
                    System.out.print(“continue”);
                    continue;
             }
             if(i == 3){
                    System.out.print(“break”);
i++;
                    break;
             }
             if(i == 7){
                    System.out.print(“continue outer”);
                    i++;
continue outer;
             }
             if(i == 8){
                    System.out.print(“breaj outer”);
                    break outer;
             }
             for(int k=0;k < 5;k++){
                    if(k==3){
                           System.out.print(“continue inner”);
                           continue inner;
}
} 
}
}
}
}
输出
i=0
continue inner
i=1
continue inner
i=2
continue
i=3
break
i=4
continue inner
i=5
continue inner
i=6
continue inner
i=7
continue outer
i=8
七、break outer
break会中断for循环，而且在抵达for循环末尾时，递增表达式不会执行，由于break跳过了递增表达，所以在i==3的情况下直接对i执行递增运算。在i==7的情况下,continue outer语句也会跳到循环顶部，而且也跳过递增。所以这里也对i进行了直接递增
如果没有break outer,就无法直接同时终止2个循环，如果想在终止循环的同时退出，可以使用return关键字。
