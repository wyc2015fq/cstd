# Java泛型中通配符的几点理解 - z69183787的专栏 - CSDN博客
2014年06月13日 10:18:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：867
置换原则
结合Java本身的一些面向对象的特性，我们很容易理解这么一个置换原则：
一个指定类型的变量可以被赋值为该类型的任何子类；一个指定某种类型参数的方法可以通过传入该类型的子类来进行调用。
总的来说，就是说我们使用的任何类型变量都可以用该类型的子类型来替换。
泛型中一种错误的继承关系
在泛型的编程中，我们考虑到子类型关系的时候，容易把一种关系给弄混淆，并错误的采用置换原则。
比如说：
- List<Integer> ints = new ArrayList<Integer>();  
- ints.add(1);  
- ints.add(2);  
- List<Number> nums = ints; // compile error
在这段代码中，我们看到类型参数Integer是Number的子类型，就容易想当然的认为List<Integer>也是List<Number>的子类。实际上并不是。所以才会导致类型不匹配，产生编译时错误。
有点时候，我们觉得，这样的转换看似不能用到一个好处，就是利用对象之间继承的关系。要是我们能有一个列表，它既能处理某种类型的数据，还能处理该类型的所有子类型的数据，这样岂不是既能用到泛型的好处又可以用到对象关系的好处么？于是在这里就引出了通配符（wildcard）。
通配符（Wildcard）
在Java类库中Collection接口定义中有一个用到通配符的方法：
- interface Collection<E> {  
- ...  
- publicboolean addAll(Collection<? extends E> c);  
- ...  
- } 
在addAll方法的描述里，可以接受Collection类型的参数。其中Collection中的类型参数可以为任何继承E的子类型。
因此，我们可以在实际代码中这么使用：
- List<Number> nums = new ArrayList<Number>();  
- List<Integer> ints = Arrays.asList(1, 2);  
- List<Double> dbls = Arrays.asList(2.78, 3.14);  
- nums.addAll(ints);  
- nums.addAll(dbls); 
在代码中我们可以看到，List<Integer>和List<Double>都是Collection<? extends Number>类型的子类。所以上面的方法中可以将Integer和Double两种类型的List传入到方法中。
通配符使用限制1：
使用通配符的泛型数据类型比较有意思，既然前面我们可以将其作为方法声明的参数，那么是否可以将它作为一个变量类型来直接创建变量呢？
看如下代码：
- List<? extends Number> nums = new ArrayList<Integer>(); //compile error
实际上上面这段代码是编译通不过的。
通配符使用限制2：
既然不能用来直接创建变量对象，那么再看下面这段代码：
- List<Integer> ints = new ArrayList<Integer>();  
- ints.add(1);  
- ints.add(2);  
- List<? extends Number> nums = ints;  
- nums.add(3.14); // compile error
这段代码的第5行会导致编译错误。在第4行代码中，我们将ints赋值给nums，表面上nums声明为一个List<Integer>的父类型，所以第4行编译正常。为什么第5行代码会出错呢？表面上看来，既然nums类型可以接受继承自Number的所有参数，那加一个Double类型的数据应该是没问题的。实际上我们再考虑一下这样会带来的问题：
nums本来引用的是一个继承自该类型的List<Integer>，如果我们允许加入Double类型的数据的话，那么ints这个Integer的List里面就包含了Double的数据，当我们使用ints的时候，和我们所期望的只包含Integer类型的数据不符合。
因此，这段代码也说明了一个问题，就是在? extends E这种通配符引用的数据类型中，如果向其中增加数据操作的话会有问题。所以向其中增加数据是不允许的。但是我们可以从其中来读取数据。
总结：
1：通配符修饰的泛型不能用来直接创建变量对象。
2：通配符修饰相当于声明了一种变量，它可以作为参数在方法中传递。这么做带来的好处就是我们可以将应用于包含某些数据类型的列表的方法也应用到包含其子类型的列表中。相当于可以在列表中用到一些面向对象的特性。
