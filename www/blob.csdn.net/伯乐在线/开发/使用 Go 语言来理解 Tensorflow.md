# 使用 Go 语言来理解 Tensorflow - 文章 - 伯乐在线
原文出处： [P. Galeone](https://pgaleone.eu/tensorflow/go/2017/05/29/understanding-tensorflow-using-go/)   译文出处：[雁惊寒](http://www.iteye.com/news/32467)
*译者注：本文通过一个简单的Go绑定实例，让读者一步一步地学习到Tensorflow有关ID、作用域、类型等方面的知识。以下是译文。*
Tensorflow并不是机器学习方面专用的库，而是一个使用图来表示计算的通用计算库。它的核心是用C++实现的，并且还有不同语言的绑定。Go语言绑定是一个非常有用的工具，它与Python绑定不同，用户不仅可以通过Go语言使用Tensorflow，还可以了解Tensorflow的底层实现。
**绑定**
Tensorflow的开发者正式发布了：
- C++源代码：真正的Tensorflow核心，实现了具体的高级和低级操作。
- Python绑定和Python库：这个绑定是由C++实现自动生成的，这样我们可以使用Python来调用C++函数。此外，这个库将调用融合到了绑定中，以便定义更高级别的API。
- Java绑定。
- Go绑定。
作为一个Go开发者而不是一个Java爱好者，我开始关注Go绑定，以便了解他们创建了什么样的任务。
**Go绑定**
![](http://dl2.iteye.com/upload/attachment/0125/3210/5d4bd576-2c19-330c-aee5-8eb9da01eb3e.png)
地鼠与Tensorflow的徽标
首先要注意的是，Go API缺少对Variable的支持：该API旨在使用已经训练过的模型，而不是从头开始训练模型。安装Tensorflow for Go的时候已经明确说明了：
> 
TensorFlow提供了可用于Go程序的API。这些API特别适合于加载用Python创建并需要在Go程序中执行的模型。
如果我们对培训ML模型不感兴趣，万岁！相反，如果你对培训模型感兴趣，那就有一个建议：
> 
作为一个真正的Go开发者，保持简单！使用Python定义并训练模型；你可以随时使用Go来加载并使用训练过的模型！
简而言之，go绑定可用于**导入和定义**常量图；在这种情况下，常量指的是没有经过训练的过程，因此没有可训练的变量。
现在，开始用Go来深入学习Tensorflow吧：让我们来创建第一个应用程序。
在下文中，我假设读者已经准备好Go环境，并按照[README](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/go/README.md)中的说明编译并安装了Tensorflow绑定。
**理解Tensorflow结构**
让我们来重复一下什么是Tensorflow：
> 
TensorFlow™是一款使用数据流图进行数值计算的开源软件库。图中的节点表示数学运算，而图的边表示在节点之间传递的多维数据数组（张量）。
我们可以把Tensorflow视为一种描述性语言，这有点像SQL，你可以在其中描述你想要的内容，并让底层引擎（数据库）解析你的查询、检查句法和语义错误、将其转换为内部表示形式、进行优化并计算出结果：所有这一切都会给你正确的结果。
因此，当我们使用任何一个API时，我们真正做的是描述一个图：当我们把图放到Session中并显式地在Session中运行图时，图的计算就开始了。
知道了这一点之后，让我们试着来定义一个计算图并在一个Session中进行计算吧。[API文档](https://godoc.org/github.com/tensorflow/tensorflow/tensorflow/go)为我们提供了tensorflow（简写为 tf）和op包中所有方法的列表。
我们可以看到，这两个包包含了我们需要定义和计算图形的所有内容。
前者包含了构建一个基本的“空”结构（就像Graph本身）的功能，后者是包含由C++实现自动生成绑定的最重要的包。
然而，假设我们要计算A与x的矩阵乘法，其中
![](http://dl2.iteye.com/upload/attachment/0125/3212/b2aca1f7-5bfc-3c34-a4a3-6e1e640db759.png)
我假设读者已经熟悉了tensorflow图定义的基本思想，并且知道占位符是什么以及它们如何工作。下面的代码是对Tensorflow Python绑定的第一次尝试。我们来调用这个文件attempt1.go


```
package main  
  
import (  
    "fmt"  
    tf "github.com/tensorflow/tensorflow/tensorflow/go"  
    "github.com/tensorflow/tensorflow/tensorflow/go/op"  
)  
  
func main() {  
    // 这里，我们打算要: 创建图  
  
    // 我们要定义两个占位符用于在运行的时候传入  
    // 第一个占位符 A 将是一个 [2, 2] 的整数张量  
    // 第二个占位符 x 将是一个 [2, 1] 的整数张量  
  
    // 然后，我们要计算 Y = Ax  
  
    // 创建图的第一个节点： 一个空的节点，位于图的根  
    root := op.NewScope()  
  
    // 定义两个占位符  
    A := op.Placeholder(root, tf.Int64, op.PlaceholderShape(tf.MakeShape(2, 2)))  
    x := op.Placeholder(root, tf.Int64, op.PlaceholderShape(tf.MakeShape(2, 1)))  
  
    // 定义接收A和x作为输入参数的操作节点  
    product := op.MatMul(root, A, x)  
  
    // 每次我们把一个`Scope`穿给操作符的时候，把操作放在作用域的下面。  
    // 这样，我们有了一个空的作用域（由NewScope创建）：空的作用域是图的根，因此可以用“/”来表示。  
  
    // 现在，我们让tensorflow按照我们的定义来创建图。  
    // 把作用域和OP结合起来，创建具体的图。  
  
    graph, err := root.Finalize()  
    if err != nil {  
        // 这里没办法处理这个错误：  
        // 如果我们错误的定义了图，我们必须手工修改这个定义。  
  
        // 这就跟SQL查询一样：如果查询语句在语法上有问题，我们只能重新写  
        panic(err.Error())  
    }  
  
    // 如果在这里，图在语法上是正确的。  
    // 我们就可以把它放到一个Session里，并执行。  
  
    var sess *tf.Session  
    sess, err = tf.NewSession(graph, &tf.SessionOptions{})  
    if err != nil {  
        panic(err.Error())  
    }  
  
    // 要使用占位符，我们必须创建一个Tensors，这个Tensors包含要反馈到网络的数值  
    var matrix, column *tf.Tensor  
  
    // A = [ [1, 2], [-1, -2] ]  
    if matrix, err = tf.NewTensor([2][2]int64{ {1, 2}, {-1, -2} }); err != nil {  
        panic(err.Error())  
    }  
    // x = [ [10], [100] ]  
    if column, err = tf.NewTensor([2][1]int64{ {10}, {100} }); err != nil {  
        panic(err.Error())  
    }  
  
    var results []*tf.Tensor  
    if results, err = sess.Run(map[tf.Output]*tf.Tensor{  
        A: matrix,  
        x: column,  
    }, []tf.Output{product}, nil); err != nil {  
        panic(err.Error())  
    }  
    for _, result := range results {  
        fmt.Println(result.Value().([][]int64))  
    }  
}
```
代码注释的很详细，希望读者能阅读每一行注释。
现在，Tensorflow-Python用户期望该代码进行编译并正常工作。我们来看看它是否正确：


```
go run attempt1.go
```
这是他看到的结果：


```
panic: failed to add operation "Placeholder": Duplicate node name in graph: 'Placeholder'
```
等等，这里发生了什么？ 显然，存在两个名称都为“Placeholder”的操作。
**第一节课： 节点ID**
每当我们调用一个方法来定义一个操作时，Python API都会生成不同的节点，无论是否已经被调用过。下面的代码返回3。


```
import tensorflow as tf  
a = tf.placeholder(tf.int32, shape=())  
b = tf.placeholder(tf.int32, shape=())  
add = tf.add(a,b)  
sess = tf.InteractiveSession()  
print(sess.run(add, feed_dict={a: 1,b: 2}))
```
我们可以通过打印占位符的名称来验证此程序是否创建了两个不同的节点：print(a.name，b.name)生成Placeholder:0 Placeholder_1:0，因此，b占位符是Placeholder_1:0，而a占位符是Placeholder:0。
在Go中，相反，之前的程序会执行失败，因为A和x都命名为Placeholder。我们可以得出这样的结论：
**Go API不会在每次调用函数来定义操作的时候自动生成新的名字：操作的名字是固定的，我们无法修改。**
**提问时间：**
- 关于Tensorflow架构，我们学到了哪些东西？图中的每个节点都必须具有唯一的名称。每个节点都用名称来标识。
- 节点的名称与用名字来定义的操作相同吗？是的，但还有更好的答案，不完全是，节点的名称只是操作的一部分。
为了详细说明第二个答案，我们来解决节点名重复的问题。
**第二节课： 作用域**
正如我们刚刚看到的那样，每定义一个操作时，Python API都会自动创建一个新的名称。在底层，Python API调用类Scope的C++方法WithOpName。以下是方法的文档及其签名，保存在[scope.h](https://github.com/tensorflow/tensorflow/blob/a5b1fb8e56ceda0ee2794ee05f5a7642157875c5/tensorflow/cc/framework/scope.h)中：


```
/// Return a new scope. All ops created within the returned scope will have
/// names of the form <name>/<op_name>[_<suffix].
Scope WithOpName(const string& op_name) const;
```
我们注意到，这个用于命名节点的方法返回了一个Scope，因此，节点名实际上是一个Scope。Scope是从根 /（空的图）到op_name的**完整路径**。
当我们尝试添加一个具有与/到op_name相同路径的节点时，WithOpName方法会添加一个后缀_<suffix>（其中<suffix>是一个计数器），因此它将成为同一范围内的重复的节点。
知道了这一点之后，为了解决重复节点名的问题，我们期望在Scope类型中找到WithOpName方法。可悲的是，这种方法并不存在。
相反，查看[Scope类型的文档](https://godoc.org/github.com/tensorflow/tensorflow/tensorflow/go/op#Scope)，我们可以看到唯一的一个方法：SubScope，它返回一个新的Scope。
文档里是这么说的：
> 
SubScope返回一个新的Scope，这将导致添加到图中的所有操作都将以“namespace”为命名空间。如果命名空间与作用域内现有的命名空间冲突，则会添加一个后缀。
使用后缀的冲突管理与C++的WithOpName不同：WithOpName是在操作名之后添加后缀，但还是在同一作用域内（因此占位符变为了Placeholder_1），而Go的SubScope是在作用域名称后添加后缀。
这种差异会产生完全不同的图，但它们在计算上是等效的。
我们来改变占位符的定义，以此来定义两个不同的节点，此外，我们来打印一下作用域的名称。
让我们创建文件attempt2.go，把这几行从：


```
A := op.Placeholder(root, tf.Int64, op.PlaceholderShape(tf.MakeShape(2, 2)))  
x := op.Placeholder(root, tf.Int64, op.PlaceholderShape(tf.MakeShape(2, 1)))
```
改成：


```
// define 2 subscopes of the root subscopes, called "input". In this  
// way we expect to have a input/ and a input_1/ scope under the root scope  
A := op.Placeholder(root.SubScope("input"), tf.Int64, op.PlaceholderShape(tf.MakeShape(2, 2)))  
x := op.Placeholder(root.SubScope("input"), tf.Int64, op.PlaceholderShape(tf.MakeShape(2, 1)))  
fmt.Println(A.Op.Name(), x.Op.Name())
```
编译并运行：go run attempt2.go，输出结果：


```
input/Placeholder input_1/Placeholder
```
**提问时间：**
关于Tensorflow的架构，我们学到了什么？节点完全是由被定义的作用域来标识的。作用域是我们从图的根到达节点的路径。有两种定义节点的方法：在不同的作用域（Go语言）中定义操作或更改操作名称。
我们解决了重复节点名称的问题，但另一个问题显示在我们的终端上。


```
panic: failed to add operation "MatMul": Value for attr 'T' of int64 is not in the list of allowed values: half, float, double, int32, complex64, complex128
```
为什么MatMul节点会出现错误？我们只是想增加两个tf.int64矩阵！从这段错误提示来看，int64是MatMul唯一不接受的类型。
> 
int64类型的attr ‘T’的值不在允许的值列表中：half，float，double，int32，complex64，complex128
这个列表是什么？为什么我们可以做两个int32类型矩阵的乘法，而不是int64？
我们来解决这个问题，了解为什么会出现这种情况。
**第三节课：Tensorflow的类型系统**
我们来看一下[源代码](https://github.com/tensorflow/tensorflow/blob/r1.2/tensorflow/core/ops/math_ops.cc#L1048)，寻找MatMul操作的C++声明：


```
REGISTER_OP("MatMul")  
    .Input("a: T")  
    .Input("b: T")  
    .Output("product: T")  
    .Attr("transpose_a: bool = false")  
    .Attr("transpose_b: bool = false")  
    .Attr("T: {half, float, double, int32, complex64, complex128}")  
    .SetShapeFn(shape_inference::MatMulShape)  
    .Doc(R"doc(  
Multiply the matrix "a" by the matrix "b".  
The inputs must be two-dimensional matrices and the inner dimension of  
"a" (after being transposed if transpose_a is true) must match the  
outer dimension of "b" (after being transposed if transposed_b is  
true).  
*Note*: The default kernel implementation for MatMul on GPUs uses  
cublas.  
transpose_a: If true, "a" is transposed before multiplication.  
transpose_b: If true, "b" is transposed before multiplication.  
)doc");
```
该行定义了MatMul操作的接口：特别注意到代码里使用了REGISTER_OP宏来声明了op的：
- 名称：MatMul
- 参数：a，b
- 属性（可选参数）：transpose_a，transpose_b
- 模板T支持的类型：half，float，double，int32，complex64，complex128
- 输出形状：自动推断
- 说明文档
这个宏调用不包含任何C++代码，但它告诉我们，在定义一个操作时，尽管它使用了模板，但是我们必须为指定的类型T（或属性）指定一个类型列表中的类型。实际上，属性.Attr(“T: {half, float, double, int32, complex64, complex128}”)是将类型T约束为该列表的一个值。
我们可以从教程中阅读到，即使在使用模板T时，我们也必须对每个支持的重载显式地注册内核。内核是以CUDA方式对C/C++函数进行的引用，这些函数将会并行执行。
因此，MatMul的作者决定仅支持上面列出的类型，而不支持int64。有两个可能的原因：
- 疏忽了：这很有可能，因为Tensorflow的作者是人类！
- 对尚未完全支持int64操作的设备兼容，因此内核的这种具体实现不足以在每个支持的硬件上运行。
回到刚才的错误提示：修改方法是显而易见的。我们必须将参数以支持的类型传递给MatMul。
我们来创建attempt3.go，把所有引用int64的行改为int32。
有一点需要注意：Go绑定有自己的一组类型，与Go的类型的一一对应。当我们将值输入到图中时，我们必须关注映射关系。从图形中获取值时，必须做同样的事情。
执行go run attempt3.go。结果：


```
input/Placeholder input_1/Placeholder  
[[210] [-210]]
```
万岁！
**提问时间**
关于Tensorflow的架构，我们学到了什么？每个操作都与自己的一组内核相关联。被视为描述性语言的Tensorflow是一种强大的类型语言。它不仅要遵守C++类型规则，而且还要在op的注册阶段只实现某些指定类型的能力。
**结论**
使用Go来定义并执行一个图，使我们有机会更好地了解Tensorflow的底层结构。使用试错法，我们解决了这个简单的问题，我们一步一步地学到了有关图、节点和类型系统这些新东西。
