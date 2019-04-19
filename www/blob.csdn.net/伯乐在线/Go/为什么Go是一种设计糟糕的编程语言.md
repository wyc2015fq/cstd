# 为什么Go是一种设计糟糕的编程语言 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [王子健](http://www.jobbole.com/members/bearsiji) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Ian Byrd](https://medium.com/@tucnak/why-go-is-a-poorly-designed-language-1cc04e5daf2)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
好吧，我承认这个标题有点放肆。我多告诉你一点：我爱肆意妄言的标题，它能够吸引注意力。不管怎样，在这篇博文中我会试图证明 Go 是一个设计得很糟糕的语言（剧透：事实上它是）。我已经摆弄 Go 有几个月了，而且，我想我在六月某个时候运行了第一个 helloworld 程序。虽然我的数学不太好，但在那之后已经有四个月了，并且我的 Github 上已经有了几个 package。不必多说，我仍完全没有在生产中使用 Go 的经验，所以把我说的有关 “编码支持”、“部署”以及相关内容当作不可尽信的吧。
我喜欢 Go语言。自从试用了它以后我就爱上了。我花了几天来接受 Go 的语言习惯，来克服没有泛型的困难，了解奇怪的错误处理和 Go 的所有典型问题。我读了 [Effective Go](https://golang.org/doc/effective_go.html)，以及 Dave Cheney 的博客上的许多文章，而且注意与 Go 有关的一切动向等等。我可以说我是一个活跃的社区成员！我爱 Go 而且我无法自拔—Go 令人惊奇。然而依我拙见，与它所宣传的正好相反，Go 是一个设计糟糕、劣质的语言。
Go 被认为是一个简练的编程语言。根据 Rob Pike 所说，他们使出了浑身解数来使这个语言的规范简单明了。这门语言的这一方面是令人惊奇的：你可以在几小时内学会基础并且直接开始编写能运行的代码，大多数情况下 Go 会如所期待的那样工作。你会被激怒，但是希望它管用。现实并不一样，Go语言并不是一个简洁，它只是低劣。以下有一些论点来证明。
### 理由1. 切片（Slice）操作压根就不对！
切片很棒，我真的很喜欢这个概念和一些用法。但是让我们花一秒钟，想象一下我们真的想要去用切片写一些代码。显而易见，切片存在于这门语言的灵魂中，它让 Go 强大。但是，再一次，在“理论”讨论的间隙，让我们想象一下我们有时会写一些实实在在的代码。以下列出的代码展示了你在 Go 中如何做列表操作。

Go
```
// 请给我一些数字！
numbers := []int{1, 2, 3, 4, 5}
log(numbers)         // 1. [1 2 3 4 5]
log(numbers[2:])     // 2. [3 4 5]
log(numbers[1:3])    // 3. [2 3]
// 有趣的是，你不能使用负数索引
//
// 来自 Python 的 numbers[:-1] 并不能正确工作，相反的是，
// 你必须这样做：
//
log(numbers[:len(numbers)-1])    // 4. [1 2 3 4]
// 可读性真实“太好了”，Pike 先生！干的漂亮！
//
// 现在，让我们在尾部插入一个6：
//
numbers = append(numbers, 6)
log(numbers) // 5. [1 2 3 4 5 6]
// 把3从numbers中移除 :
//
numbers = append(numbers[:2], numbers[3:]...)
log(numbers)    // 6. [1 2 4 5 6]
// 想要插入一些数？别急，这里是一个Go语言*通用*最佳实践
//
// 我特别喜欢。。。哈哈哈。
//
numbers = append(numbers[:2], append([]int{3}, numbers[2:]...)...)
log(numbers)    // 7. [1 2 3 4 5 6]
// 为了拷贝一份切片，你需要这样做：
//
copiedNumbers := make([]int, len(numbers))
copy(copiedNumbers, numbers)
log(copiedNumbers)    // 8. [1 2 3 4 5 6]
//还有一些其他操作。。。
```
信不信由你，这是 Go 程序员每天如何转换切片的真实写照。而且我们没有任何泛型机制，所以，哥们，你不能创造一个漂亮的 insert() 函数来掩盖这个痛苦。我在 playgroud 贴了这个，所以你不应该相信我：自己双击一下去亲自看看。
### 理由2. Nil 接口并不总是 nil :)
他们告诉我们“在 Go 中错误不只是字符串”，并且你不该把它们当字符串对待。比如，来自 Docker 的 [spf13](http://spf13.com/) 在他精彩的“Go 中的7个失误以及如何避免”中如此讲过。
他们也说我应该总是返回 error 接口类型（为了一致性、可读性等等）。我在以下所列代码中就是这么做的。你会感到惊讶，但是这个程序真的会跟 Pike 先生 say hello，但是这是所期待的吗?

Go
```
package main
import "fmt"
type MagicError struct{}
func (MagicError) Error() string {
	return "[Magic]"
}
func Generate() *MagicError {
	return nil
}
func Test() error {
	return Generate()
}
func main() {
	if Test() != nil {
		fmt.Println("Hello, Mr. Pike!")
	}
}
```
是的，我知道为什么这会发生，因为我阅读了一堆复杂的关于接口和接口在 Go 中如何工作的资料。但是对于一个新手……拜托哥们，这是当头一棒！实际上，这是一个常见的陷阱。如你所见，没有这些让人心烦意乱的特性的 Go 是一个直接易学的语言，它偶尔说 nil 接口并不是nil ;)
### 理由3. 可笑的变量覆盖
为了以防万一你对这个术语不熟悉，让我引用一下 Wikipedia：”当在某个作用域（判定块、方法或者内部类）中声明的一个变量与作用域外的一个变量有相同的名字，变量覆盖就会发生。“看上去挺合理，一个相当普遍的做法是，多数的语言支持变量覆盖而且这没有问题。Go 并不是例外，但是却不太一样。下面是覆盖如何工作的：

Go
```
package main
import "fmt"
func Secret() (int, error) {
	return 42, nil
}
func main() {
	number := 0
	fmt.Println("before", number) // 0
	{
		// meet the shadowing
		number, err := Secret()
		if err != nil {
			panic(err)
		}
		fmt.Println("inside", number) // 42
	}
	fmt.Println("after", number) // 0
}
```
是的，我也认识到 := 操作符制造了一个新的变量并且赋了一个右值，所以根据语言规范这是一个完全合法的行为。但是这里有件有意思的事：试着去掉内部作用域——它会如期望的运行（”在42之后“）。否则，就跟变量覆盖问个好吧。
无需赘言，这不是什么我在午饭时想起来的一个好玩的例子，它是人们早晚会遇到的**真实**的东西。这周的早些时候我重构了一些 Go 代码，就遇到了整个问题两次。编译没问题，代码检查没问题，什么都没问题——代码就是不正常运行。
### 理由4. 你不能传递把 []struct 作为 []interface 传递
接口很棒，Pike&Co. 一直说它就是 Go 语言的一切：接口事关你如何处理泛型，如何做 mock 测试，它是多态的实现方法。让我告诉你吧，当我阅读“Effective Go”的时候我真心爱着接口，而且我一直爱着它。除了上面我提出的“nil 接口不是 nil”的问题外，这里有另一个令人讨厌的事让我认为接口在 Go 语言中没有得到头等支持。基本上，你不能传递一个结构的切片到一个接收接口类型切片的函数上：

Go
```
package main
import (
	"fmt"
	"strconv"
)
type FancyInt int
func (x FancyInt) String() string {
	return strconv.Itoa(int(x))
}
type FancyRune rune
func (x FancyRune) String() string {
	return string(x)
}
// 实际上，任何具有String()方法的对象
type Stringy interface {
	String() string
}
// String, made of string representations of items given.
func Join(items []Stringy) (joined string) {
	for _, item := range items {
		joined += item.String()
	}
	return
}
func main() {
	numbers := []FancyInt{1, 2, 3, 4, 5}
	runes := []FancyRune{'a', 'b', 'c'}
	// You can't do this!
	//
	// fmt.Println(Join(numbers))
	// fmt.Println(Join(runes))
	//
	// prog.go:40: cannot use numbers (type []FancyInt) as type []Stringy in argument to Join
	// prog.go:41: cannot use runes (type []FancyRune) as type []Stringy in argument to Join
	//
	// 相反，你应该这样做：
	//
	properNumbers := make([]Stringy, len(numbers))
	for i, number := range numbers {
		properNumbers[i] = number
	}
	properRunes := make([]Stringy, len(runes))
	for i, r := range runes {
		properRunes[i] = r
	}
	fmt.Println(Join(properNumbers))
	fmt.Println(Join(properRunes))
}
```
不出意外，这是个已知的根本没有被当作问题的问题。它只是 Go 的又一个可笑的事，对吧？我真的推荐你阅读一下相关的 wiki，你会发现为什么“传递结构切片作为借口切片”不可行。但是呀，好好想想！我们可以做到，这里没什么魔法，这只是编译器的问题。看，在 49-57行 我做了一个由 []struct 到 []interface的显式转换。为什么 Go 编译器不为我做这些？是的显示要比隐式好，但是WTF？
我只是无法忍受人们看着这种狗屁语言又一直说“好，挺好的”。并不是。这些让 Go 变成了一个糟糕的语言。
### 理由5. 不起眼的 range“按值”循环
这是我曾经遇到过的第一个语言问题。好吧，在 Go 中有一个 “for-range”循环，是用来遍历切片和监听 channel 的。它到处都用得到而且还不错。然而这里有一个小问题，大多数新手被坑在这上面：range 循环只是按值的，它只是值拷贝，你不能真的去做什么，它不是 C++ 中的 foreach。

Go
```
package main
import "fmt"
func main() {
	numbers := []int{0, 1, 2, 3, 4}
	for _, number := range numbers {
		number++
	}
	fmt.Println(numbers) // [0 1 2 3 4]
	for i, _ := range numbers {
		numbers[i]++
	}
	fmt.Println(numbers) // [1 2 3 4 5]
}
```
请注意，我没有抱怨 Go 里没有按引用的 range，我抱怨的是 range 太不起眼。动词“range”有点像是说“遍历项目“，而不是”遍历项目的拷贝“。让我们看一眼”Effective Go“中的 For，它听起来一点也不像”遍历切片中的拷贝值“，一点也不。我同意这是个小问题，我很快（几分钟）就克服了它，但是没有经验的 gopher 也许会花上一些时间调试代码，惊讶于为什么值没有改变。你们至少可以在”Effective Go“里面把这点讲述明白。
### 理由6. 可疑的编译器严谨性
就如我之前已经告诉你的，Go被认为是一个有着严谨的编译器的，简单明了并且可读性高的语言。比如，你不能编译一个带有未使用的 import 的程序。为什么？只是因为 Pike 先生认为这是对的。信不信由你，未使用的 import 不是世界末日，我完全可以与其共存。我完全同意它不对而且编译器不惜打印出相关的警告，但是为什么你为了这么一个小事中止编译？就为了未使用的 import，当真？
Go1.5 引入了一个有趣的语言变化：现在你可以列出 map 字面量，而不必显示列出被包含的类型名。这花了他们五年（甚至更多）来认识到显示类型列出被滥用了。
另一个我在 Go 语言里非常享受的事情：逗号。你看，在 Go 中你可以自由地定义多行 import、const 或者 var 代码块：

Go
```
import (
    "fmt"
    "math"
    "github.com/some_guy/fancy"
)
const (
    One int = iota
    Two
    Three
)
var (
    VarName int = 35
)
```
好吧，这挺好的。但是一旦它涉及到“可读性”，Rob Pike 认为加上逗号会很棒。某一刻，在加上逗号以后，他决定你应该也把结尾的逗号留着！所以你并不这样写：

Go
```
numbers := []Object{
    Object{"bla bla", 42}
    Object("hahauha", 69}
}
```
你必须这样写：

Go
```
numbers := []Object{
    Object{"bla bla", 42},
    Object("hahauha", 69},
}
```
我仍然怀疑为什么我们在 import/var/consts 代码块中可以忽略逗号，但是在列表和映射中不能。无论如何，Rob Pike 比我清楚！可读性万岁！
### 理由7. Go generate 太诡异了
首先，你要知道我没有反对代码生成。对于 Go 这样一个粗劣的语言，这也许是仅有的可用来避免拷贝-粘贴一些常见的东西的途径。然而，Go:generate——一个 Go 用户到处都用的代码生成工具，现在仅仅是垃圾而已。好吧，公平来说，这个工具本身还好，我喜欢它。而整个的方式是错的。我们看看吧，你要通过使用特别的魔法命令来生成一些代码。对，通过代码注释中的一些神奇的字节序列来做代码生成。
注释是用来解释代码，而不是生成代码。不过神奇的注释在当今的 Go 中是一种现象了。非常有意思的是，没人在乎，大家觉得这就挺好的。依我愚见，这绝对比吓人的未使用的 import 要糟糕。
### 后记
如你所见，我没有抱怨泛型、错误处理、语法糖和其他 Go 相关的典型问题。我同意泛型不至关重要，但如果你去掉泛型，请给我们一些正常的代码生成工具而不是随机的乱七八糟的狗屎神奇注释。如果你去掉异常，请给我们安全地把接口与 nil 比较的能力。如果你去掉语法糖，请给我们一些能够如预期工作的代码，而不是一些像变量遮蔽这样的“哎呦卧槽“的东西。
总而言之，我会继续使用 Go。理由如下：因为我爱它。我恨它因为它就是堆垃圾，但是我爱它的社区，我爱它的工具，我爱巧妙的设计决定（接口你好）和整个生态。
嘿伙计，想尝试尝试 Go 吗?
**（译者注：原文一些理由并不站得住脚，原文链接中的评论也值得一看。）**
