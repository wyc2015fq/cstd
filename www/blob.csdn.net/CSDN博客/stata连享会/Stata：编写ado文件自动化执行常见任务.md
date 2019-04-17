# Stata：编写ado文件自动化执行常见任务 - stata连享会 - CSDN博客





2018年10月23日 10:34:06[arlionn](https://me.csdn.net/arlionn)阅读数：585








![](https://pic4.zhimg.com/v2-e284ac6eb94cb2f0152fb46069aded86_1200x500.jpg)

## ### 文章目录- [@[toc]](#toc_3)- [1. 使用脚本文件](#1__26)- [2. 使用 Do 文件自动化执行](#2__Do__56)- [3. 为什么我们想要 **if** 限定语句？](#3__if__121)- [4. 创建一个新命令：ado 文档](#4_ado__201)- [4.1 编写 ado 文件](#41__ado__208)- [4.2 ado 文件注意事项](#42_ado__249)- [5. 典型流程归纳](#5__281)- [6. 添加更多的选项](#6__300)- [关于我们](#_397)- [联系我们](#_403)

> 
谢作翰 [译] |  ([知乎](https://zhuanlan.zhihu.com/arlion) | [简书](http://www.jianshu.com/u/69a30474ef33) | [码云](https://gitee.com/arlionn))


> 
Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)


> 
**编者按：** 这是 Stata Blog 上的一篇推文，由浅入深地讲解了如何从一个简单的任务出发，逐步通过撰写 dofile，adofile 来实现任务的自动化可重复性执行。这篇推文帮助我们在不自觉中学会了编写标准的 Stata 程序 (ado 文件)。


> 
原文地址 ：[https://blog.stata.com/2018/10/09/how-to-automate-common-tasks/](https://blog.stata.com/2018/10/09/how-to-automate-common-tasks/)


高效进行数据分析的关键是将常见任务自动化。自动化能把你从大量重复性操作中拯救出来，不但节约时间，还能减少出错概率。

本篇让我们使用Stata自动执行一些任务。任务本身并不重要，重要的是熟悉自动化执行任务的流程。

我们选择的任务是将变量**标准化**——**减去变量的均值并除以其标准差**。

正如您所知道的，Stata社区中有现成的命令来执行此操作，而且比我们接下来自己写更为便捷（在Stata中搜索 **normalize variable**）；您也可以使用Stata的**egen**命令对单个变量标准化，但我们打算做的远不止如此。

我认为本文读者是Stata自动化任务的新手。所以，如果您已经是专家，可能对这篇文章不感兴趣，当然也可能会一些新的发现。

### 1. 使用脚本文件

首先，我们将直接在分析脚本中执行标准化操作。在Stata中，我们将分析脚本称为 **do-files**，因为它们可以执行某些操作。

让我们把原始变量命名为 **x**。因为我们不想改变现有变量的内容，所有新建一个变量 **xN**，其中 **N** 后缀表示标准化（如果您不喜欢 **N** 后缀，可以改变，比如 `_norm`，也可使用前缀）。Stata 的 **summarize** 命令将给出原始变量平均值和标准差。

```
*-----------begin------mydo.do-----
···
summarize x
generate xN = (x - r(mean)) / r(sd)
···
*-----------end--------mydo.do-----
```

如上，对一个变量标准化只需要两行代码即可。

**r(mean)** 和 **r(sd)** 是什么意思，我们又该如何了解它们？

它们分别表示变量均值和标准差的返回值。

在 Stata 中，几乎所有命令都会返回结果。已模型估计为主要目标的命令 (如 `regress`, `logit` 等) 的返回结果以 **e()** 表示，大多数其他命令以 **r()** 表示。输入 `help summarize` 并拉到帮助文件的底部就能看到 `summarize` 返回的所有结果及其描述。也可以在执行完 `summaryrize` 命令后输入 `return list` 命令来查看返回值列表。

**既然我们的任务只有两行，那为什么我们要自动化呢？**

因为当我们需要大量重复操作时，即使只有两行代码，也极易出错。比如，我们想复制这段代码 100 次进行 100 个变量标准化时，我们必须将代码中变量名 **x** 更改为 100 个新的变量名称，但是我们往往会忘记。你可能会忘记更改 **summarize** 中的 **x**，或是忘记更改 **xN**，并收到报错消息。或是忘记更改在表达式中的变量名。这三种错误我都犯过。

### 2. 使用 Do 文件自动化执行

我们将脚本放入自己的do-file中。

**[normalize.do](http://normalize.do)** (1)

```
version 15.1

summarize x
generate xN = (x - r(mean)) / r(sd)
```

**ps**：在文件顶部我添加了版本命令。请切记，一定要为你的do文件标明版本信息！我使用的是 Stata 15.1，一旦标记上，这个脚本将始终以 15.1 版 **stata** 的特性运行，即使将来用 Stata 42 版运行这个文件（可能 42 版的 **stata** 早已取消 `summarize` 命令或完全改变 `summarize` 的工作方式）但 Stata 会识别出版本号，并按 15.1 版本的语法规则正常运行。

我们通过输入以下命令执行所写的脚本

```
. do normalize
```

或者在直接在do文件里添加 **do normalize** 语句。

我们目前的 **[normalize.do](http://normalize.do)** 并不太有趣。我们需要它来处理除 **x** 之外的变量。

这是一个版本：

**[normalize.do](http://normalize.do)** (2)

```
version 15.1

summarize `1'
generate `1'N = (`1' - r(mean)) / r(sd)
```

然后输入

```
. do normalize y
```

从（1）到（2）的变化是什么？我们所做的只是用 **`1′**

替换每次出现的 **x** 。为什么是 **`1’** ？Stata 的 do-files 会将其参数依次放进编号为 1, 2, 3 等的局部暂元进行解析。第一个参数进入局部暂元``1'`， 第二个参数进入 ``2'`，依此类推。

什么是局部暂元？暂元是一个存储价值的容器。**1** 可以是暂元名称。为什么我们用``'`包裹 1？因为如果我们只输入 1，那指的是数字 1，而我们需要的是小盒子(暂元) **1** 中的值，所以我们需要对它解引用。因为我们输入了 **y** ——我们的第一个（也是唯一的）参数，所以暂元**`1’** 解引为（**deference**）**y**。如果你不喜欢“解引用”这个表达，也可以说 **“1”** 扩展为 **y** 。

若你在我们的第二版 **[normalize.do](http://normalize.do)** 中以 **y** 代替 **`1’** ，它就成了第一个版本。这也正是 Stata 所做的。

使用我们的新 **[normalize.do](http://normalize.do)**，我们可以方便的输入：

```
. do normalize myvariable
. do normalize myothervariable
. do normalize x1
. do normalize x2
...
. do normalize x100
```

使用以上代码我们犯错率大大减小了，只是代码非常冗赘。这个问题稍后我们回来解决。

现在我现在想探究的是，能否在我们的 do 文件中加入 Stata 的 **if** 限定符，答案是肯定的，而且很容易。

### 3. 为什么我们想要 **if** 限定语句？

```
. do normalize income if male == 0
```

我们可能想对样本中的女性对象标准化，这时候就可以使用 **if** 限定符 **if male== 0**。

以下是包含 **if** 和 **in** 限定符的 do 文件。

（如果你不知道什么是 **in** 限定词，点击[https://www.stata.com/help.cgi?in](https://www.stata.com/help.cgi?in)）

**[normalize.do](http://normalize.do)** (3)

```
version 15.1

syntax varlist(min=1 max=1) [if] [in]

summarize `varlist' `if' `in'
generate `varlist'N = (`varlist' - r(mean)) / r(sd)  `if' `in'
```

最后两行代码与之前版本相比有所改变，主要在两个方面：
- 第一， 局部暂元 **`1’** 更换为暂元 **``varlist’`**；
- 第二，两个命令结尾部分加入 `if`、`in` 限定符。我们的 do-file 现在直接支持 **if** 和 **in** 限定符，所以新的 **syntax** 命令似乎表现出很多魔力，事实上确实如此。

这里的 **syntax** 指令具有神奇的作用，具体解释如下：
- **syntax** 解析命令看起来像是标准的 Stata 命令——有一个变量列表（命令 `_varlist` 中_），可选的 **if** 限定词，可选的 **in** 限定词，除了缺少选项部分（`option`）其他都有了。
- **syntax** 命令的真正优点在于您只需输入完成任务最基本的命令，并且 **syntax** 解析命令行，使用相关的语法部分填充局部暂元。当输入的内容与您指定的语法不匹配时，它还会发出错误消息。这就是为什么我们不计麻烦地在 **syntax** 命令 **varlist** 参数上添加 **(min = 1 max = 1)** 。若不加入括号部分，**syntax** 允许指定多个变量。而现在我们只需要一个变量。
- **[if]** 和 **[in]** 限定符是可选参数。如果语句中 **if** 不带有方括号，此时 **if** 限定词变为语法必选的参数。

前两个 do 文件有个问题我们此前忽略了——我从未检查过 **`1’** 是一个未缩写的变量名。Stata 允许缩写变量名称。如果你有一个名为 **foreign** 的变量，并且没有其他变量名的缩写是 **for**，此时输入

```
. do normalize for
```

便会会创建一个名为 **forN** 的新变量，而不是 **foreignN**。无论如何，你必须要小心。一些方法可以解决这个问题，但我们不再过多介绍。

使用 **syntax** 语句则没有这个问题。即使在命令行中输入 **for**，局部暂元 **`varlist’** 也会扩展为未缩写的变量名 **foreign**。这也是 **syntax** 的过人之处。

现在，让我们来解决代码冗赘问题。

如果我们想要标准化的变量数量很多怎么办？这也很容易，但我们必须最终添加到我们的两行计算代码中。

这是一个 do-file，它接受变量列表并对每个变量进行规范化，同时支持 **if** 和 **in** 限定符。

**[normalize.do](http://normalize.do)** (4)

```
version 15.1

syntax varlist [if] [in]

foreach var in `varlist' {
  summarize `var' `if' `in'
  generate `var'N = (`var' - r(mean)) / r(sd)   `if' `in'
}
```

具体解释如下：
- 在顶部的 syntax 语句格式行，我们删除了 **(min = 1 max = 1)**，因为现在我们要接受含多个变量的列表 **varlist** 。
- **foreach** 命令很容易理解。对每个属于变量列表（**varlist**）中的变量 **var** 依次执行标准化命令。暂元 **``varlist'`** 解析为 do 所指定的变量列表。
- **var** 是用来保存单个变量名称的代号，因为我们一次只能循环一个变量。我们也可以使用其他代号如 **变量**，**v**，**z**。如果您对暂元的概念不熟悉，不妨将其理解为空一个容器或仓库，在程序解析时才被赋予为特定的值（这一过程又称解引用或拓展）。以上代码中 ``varlist'` 仓库中存放的是由环境中多个变量名组成的列表，而仓库 ``var'`存放的是某单个变量名，并且var这个仓库里存放的变量名会随着循环次数的变化改变，但每次都只能存放一位，一直到遍历仓库 ``varlist`` 中的所有变量名。

我们现在输入

```
. do normalize x1 x2 x3  if male==0
```

或者

```
. do normalize x*
```

**[normalize.do](http://normalize.do)** 将从 Stata `varlist` 中接受符合条件的变量。如果您还了解 `varlist`，请单击 [https://www.stata.com/help.cgi?varlist](https://www.stata.com/help.cgi?varlist) 查看所有含义。

### 4. 创建一个新命令：ado 文档

这小小的自动化过程给我们带来了许多方便，我们可能不满足仅仅把它保存为一个 do 文件，而想要将它变成一个新的 Stata 命令，这样我们可以在任何项目中使用它，甚至可以分享给我们的同事。

再说一次，这并不难。

#### 4.1 编写 ado 文件

我们将创建一个 **ado 文件**（自动化的 do 文件）。在 ado 文件中定义的程序就像Stata内置命令一样，会自动被发现并运行。

**normalize.ado** (a)

```
capture program drop normalize
program normalize
    version 15.1

    syntax varlist [if] [in]

    foreach var in `varlist' {
        summarize `var' `if' `in'
        generate `var'N = (`var' - r(mean)) / r(sd)   `if' `in'
    }
end
```

我们做了哪些调整呢？
- 我们对 version（4）代码进行了缩进，但这仅仅只是为了美观。
- 我们在文件顶部添加了 **program normalize**，底部添加了 **end**。这样以来 Stata 会将其视为命令(自动确定其身份)，因此我们 **不必** 在运行时先输入 **do**。

我们现在有一个程序，只要我们输入 **normalize** ，Stata 就会自动发现并运行它。

我们现在可以输入

```
. normalize x1 x2 x3  if male==0
```

或

```
. normalize x*
```

我们可以将文件 **normalize.ado** 提供给同事，他们可以像使用其它 Stata 命令那样使用它。

现在行动起来，自动化执行自己的任务吧！

#### 4.2 ado 文件注意事项
- 虽然 ado 文件是在 dofile 编辑器中编写的，但写好的 ado 文件必须保存为后缀为 **.ado** 的 ado 文件；
- 文件的名称必须与程序名保持一致 (包括大小写)。
- 保存路径问题：ado 文件必须存放在 Stata 能够找到的路径列表下方。输入 `adopath` 可以查看路径清单。你可以使用 `adopath + pathname` 命令添加新的路径至清单中。

**A. ado 文件的保存**

如果您将 **normalize.ado** 放在可以被 Stata 发现的路径中（如当前的工作目录）它的确足以应付日常的自动化需求。但您可能不希望将其放在每个工作目录中。而且万一我们对命令改动升级了怎么办？我们还得在好几个地方改变它。这时，在 Stata 中输入

```
. adopath
```

该路径上的一个目录将被标记为 **(PERSONAL)**。复制 **normalize.ado** 到此处。无论您在哪个目录中工作，现在都可以在所有项目中找到它。

如果您将 **normalize.ado** 提供给同事，请告诉他们将其复制到他们的 **(PERSONAL)** 目录。

当然，你也可以把你的 ado 文件放在别的地方，但此时需要使用 `adopath + pathname` 把路径名称告诉 Stata，它会把这个路径添加到其查找 ado 文档的路径清单中。详情参见 `help adopath`。

事实上，我并不总是采用自动化。我发现不同情况下止步于我们的 do 文件的版本 (1), (2), (3) 或 (4) 都是有用的。或者一直到新的命令。

另外，我们将程序名叫做 **normalize** 并将其放入名为 **normalize.ado** 文件中并非随意：**程序名称和文件名必须相同！**

**B. 程序的调试**

还有一个细节。每次键入 **do normalize** …时，都会从 **[normalize.do](http://normalize.do)** 文件重新加载您的文件。键入 **normalize** 后，您的ado文件程序将保留在 Stata 的内存中。下次键入 **normalize** 时，Stata 会从内存运行程序而不重新读取 **normalize.ado** 文件。那更快。但是…如果您正在调试程序并编辑文件，则不会重新加载您的更改。在键入 **normalize** 之前，您需要键入 **discard** … 这样，您的程序将从内存中删除，并将从您的文件重新加载。更为标准的做法是，在程序的首行写入 `capture program drop normalize`，以便在调试过程中自动清除内存中的旧版程序。当完成程序的所有调试工作后，可以去掉这一行，以便提高程序的运行效率。

在调试程序过程中，有些莫名的错误往往难以觉察，此时，可以使用 `set trace on` (官方命令) 或 `help tr` (外部命令) 来呈现程序的解析过程。Stata 会在出错的地方自动停下来，并用红色标记出错信息。

**C. 程序的分享和发布**

与整个 Stata 社区分享您的新命令也很简单。查看常见问题解答[如何与 Stata 用户共享新命令？](https://www.stata.com/support/faqs/resources/sharing-a-command/)

### 5. 典型流程归纳

这是一个典型的自动化过程：
- **Step 1**. 编写特定问题的解决方案。

​  - a.发现自己一遍又一遍地复制那些代码。

​  - b. 问自己从一个问题到另一个问题的变化。- **Step 2**. 编写一个 do 文件，将那些变化的内容作为参数。

​  - a. 提炼。

​  - b. 测试

​  - C. 重复 2a 和 2b，直到满意- **Step 3**. 可以把你的 do 文件变成一个 ado 文件。
- **Step 4**. 可以与同事分享您的 ado 文件。
- **Step 5**. 可以与整个 Stata 社区分享您的 ado 文件。

恭喜，您现在可以自动执行 Stata 中的常见任务。无论你是否愿意，你都在成为一名程序员。

如果你对我们迄今为止所做的事情感到满意，这将是退出阅读的好时机。

### 6. 添加更多的选项

您可能不希望自动将 **N** 附加到原始变量名称的末尾以指定标准化变量。也许你想使用不同的字母，或者可能是一组字符，比如 **_norm**。或者您可能更喜欢后缀的前缀。甚至也许你想要两个。

我们可以通过增加选项实现这一点。

**normalize.ado**（b）

```
program normalize
  version 15.1

  syntax varlist [if] [in] [ , prefix(name) suffix(name) ]

  foreach var in `varlist' {
    summarize `var' `if' `in'
    generate `prefix'`var'`suffix' = (`var' - r(mean)) / r(sd)   `if' `in'
  }
end
```

从版本（a）到版本（b），我们所做的只是把

```
syntax varlist [if] [in]
```

改为

```
syntax varlist [if] [in] [, prefix(name) suffix(name)]
```

并改变

```
generate `var'N = ...
```

至

```
generate `prefix'`var'`suffix' = ...
```

让我们先来理解 **syntax** 行的变化：

方括号 **[ ]** 表示所含部分为可选项 (并非强制设定)。

但是，如果用户决定使用这些选项，他们必须先输入逗号 `,` 。

然后，他们可以键入 **prefix()** (前缀)，或 **suffix()** (后缀)，或两者兼具。

如果他们键入 **prefix()** (前缀)，那么局部暂元 `prefix`将包含他们在括号中输入的内容。
我们编写 **syntax** 命令时必须小心。因为我们写了 **prefix(name)** 而不是 **prefix(string)**，所以用户必须在括号中键入符合 Stata 变量名规则的字符串，而不能随意填写。

简言之，``prefix'`var'`suffix'` 构成的新变量名必须符合 Stata 的命令规则 (详见 `help varname`)。

那么，以下代码是什么意思呢？

```
generate `prefix'`var'`suffix' = ...
```

暂元 **`prefix’** 和 **`suffix’** 将扩展为用户在 **prefix** 和 **suffix** 选项中输入的任何内容。我们的新变量名将带有用户输入入的前缀和后缀。

使用我们的新 ado 文件，我们现在可以输入类似的内容

```
. normalize x1 x2 x3 x4 , prefix(norm_of_)
. normalize x* , prefix(norm_of_)
```
- 第一行创建四个新变量：**norm_of_x1**，**norm_of_x2**，**norm_of_x3**，**norm_of_x4**。我不喜欢这些名字，但它们含义清晰。除非你在考虑矩阵。有时，这些称为标准化变量，因此您可能更喜欢 **prefix(std_)**。
- 在第二行中，**x*** 匹配以 **x** 开头的所有变量。它们中的每一个都将被标准化，并使用指定的前缀 **norm_of_** 创建一个/组新变量。
- 你可能已经注意到了一个潜伏的 bug。如果用户既不键入 **prefix()**，也不键入 **suffix()** 选项，那么 **`prefix’** 和 **`suffix’** 都将为空。

我们的 **generate** 命令将尝试创建一个与原始变量同名的变量。那…是语法错误。

避免该错误的一种方法是默认为新变量添加后缀 **“N”**，这可以通过在 **syntax** 行下面添加以下三行新的语句来实现：

```
if "`prefix'`suffix'" == "" {
    local suffix "N"
}
```

其含义是：如果前缀和后缀 (**`prefix’ 以及 `suffix’**) 都为空，则使用 **“N”** 作为后缀。

另一个不错的改进是为我们的新变量添加标签，例如：

```
label variable `prefix'`var'`suffix' "`var' normalized"
```

我们可以将上述语句添加在 **for** 循环中中的 **generate** 命令之后，以便实现对所有变量批量添加标签。

这就是程序变长的方式。您可以改进它们，并添加功能。坚持这一点，你很快就会编写一些复杂代码来难倒你的同事。

> 
#### 关于我们

- 【**Stata 连享会(公众号：StataChina)**】由中山大学连玉君老师团队创办，旨在定期与大家分享 Stata 应用的各种经验和技巧。
- 公众号推文同步发布于 [【简书-Stata连享会】](http://www.jianshu.com/u/69a30474ef33) 和 [【知乎-连玉君Stata专栏】](https://www.zhihu.com/people/arlionn)。可以在**简书**和**知乎**中搜索关键词`Stata`或`Stata连享会`后关注我们。
- Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)

> 
#### 联系我们

- **欢迎赐稿：** 欢迎将您的文章或笔记投稿至`Stata连享会(公众号: StataChina)`，我们会保留您的署名；录用稿件达`五篇`以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **意见和资料：** 欢迎您的宝贵意见，您也可以来信索取推文中提及的程序和数据。
- **招募英才：** 欢迎加入我们的团队，一起学习 Stata。合作编辑或撰写稿件五篇以上，即可**免费**获得 Stata 现场培训 (初级或高级选其一) 资格。
- **联系邮件：**[StataChina@163.com](mailto:StataChina@163.com)

> 
Stata连享会 [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md)


![欢迎加入Stata连享会(公众号: StataChina)](http://upload-images.jianshu.io/upload_images/7692714-589cc89048bf5905.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)






