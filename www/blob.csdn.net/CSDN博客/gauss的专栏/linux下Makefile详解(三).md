# linux下Makefile详解(三) - gauss的专栏 - CSDN博客
2014年11月24日 00:19:04[gauss](https://me.csdn.net/mathlmx)阅读数：214
书写命令
————
每条规则中的命令和操作系统Shell的命令行是一致的。make会一按顺序一条一条的执行命令，每条命令的开头必须以[Tab]键开头，除非，命令是紧跟在依赖规则后面的分号后的。在命令行之间中的空格或是空行会被忽略，但是如果该空格或空行是以Tab键开头的，那么make会认为其是一个空命令。
我们在UNIX下可能会使用不同的Shell，但是make的命令默认是被“/bin/sh”——UNIX的标准Shell解释执行的。除非你特别指定一个其它的Shell。Makefile中，“#”是注释符，很像C/C++中的“//”，其后的本行字符都被注释。
一、显示命令
通常，make会把其要执行的命令行在命令执行前输出到屏幕上。当我们用“@”字符在命令行前，那么，这个命令将不被make显示出来，最具代表性的例子是，我们用这个功能来像屏幕显示一些信息。如：
    @echo 正在编译XXX模块......
当make执行时，会输出“正在编译XXX模块......”字串，但不会输出命令，如果没有“@”，那么，make将输出：
    echo 正在编译XXX模块......
    正在编译XXX模块......
如果make执行时，带入make参数“-n”或“--just-print”，那么其只是显示命令，但不会执行命令，这个功能很有利于我们调试我们的Makefile，看看我们书写的命令是执行起来是什么样子的或是什么顺序的。
而make参数“-s”或“--slient”则是全面禁止命令的显示。
二、命令执行
当依赖目标新于目标时，也就是当规则的目标需要被更新时，make会一条一条的执行其后的命令。需要注意的是，如果你要让上一条命令的结果应用在下一条命令时，你应该使用分号分隔这两条命令。比如你的第一条命令是cd命令，你希望第二条命令得在cd之后的基础上运行，那么你就不能把这两条命令写在两行上，而应该把这两条命令写在一行上，用分号分隔。如：
    示例一：
        exec:
                cd /home/hchen
                pwd
    示例二：
        exec:
                cd /home/hchen; pwd
当我们执行“make exec”时，第一个例子中的cd没有作用，pwd会打印出当前的Makefile目录，而第二个例子中，cd就起作用了，pwd会打印出“/home/hchen”。
make一般是使用环境变量SHELL中所定义的系统Shell来执行命令，默认情况下使用UNIX的标准Shell——/bin/sh来执行命令。但在MS-DOS下有点特殊，因为MS-DOS下没有SHELL环境变量，当然你也可以指定。如果你指定了UNIX风格的目录形式，首先，make会在SHELL所指定的路径中找寻命令解释器，如果找不到，其会在当前盘符中的当前目录中寻找，如果再找不到，其会在PATH环境变量中所定义的所有路径中寻找。MS-DOS中，如果你定义的命令解释器没有找到，其会给你的命令解释器加上诸如“.exe”、“.com”、“.bat”、“.sh”等后缀。
三、命令出错
每当命令运行完后，make会检测每个命令的返回码，如果命令返回成功，那么make会执行下一条命令，当规则中所有的命令成功返回后，这个规则就算是成功完成了。如果一个规则中的某个命令出错了（命令退出码非零），那么make就会终止执行当前规则，这将有可能终止所有规则的执行。
有些时候，命令的出错并不表示就是错误的。例如mkdir命令，我们一定需要建立一个目录，如果目录不存在，那么mkdir就成功执行，万事大吉，如果目录存在，那么就出错了。我们之所以使用mkdir的意思就是一定要有这样的一个目录，于是我们就不希望mkdir出错而终止规则的运行。
为了做到这一点，忽略命令的出错，我们可以在Makefile的命令行前加一个减号“-”（在Tab键之后），标记为不管命令出不出错都认为是成功的。如：
   clean:
            -rm -f *.o
还有一个全局的办法是，给make加上“-i”或是“--ignore-errors”参数，那么，Makefile中所有命令都会忽略错误。而如果一个规则是以“.IGNORE”作为目标的，那么这个规则中的所有命令将会忽略错误。这些是不同级别的防止命令出错的方法，你可以根据你的不同喜欢设置。
还有一个要提一下的make的参数的是“-k”或是“--keep-going”，这个参数的意思是，如果某规则中的命令出错了，那么就终目该规则的执行，但继续执行其它规则。
四、嵌套执行make
在一些大的工程中，我们会把我们不同模块或是不同功能的源文件放在不同的目录中，我们可以在每个目录中都书写一个该目录的Makefile，这有利于让我们的Makefile变得更加地简洁，而不至于把所有的东西全部写在一个Makefile中，这样会很难维护我们的Makefile，这个技术对于我们模块编译和分段编译有着非常大的好处。
例如，我们有一个子目录叫subdir，这个目录下有个Makefile文件，来指明了这个目录下文件的编译规则。那么我们总控的Makefile可以这样书写：
    subsystem:
            cd subdir && $(MAKE)
其等价于：
    subsystem:
            $(MAKE) -C subdir
定义$(MAKE)宏变量的意思是，也许我们的make需要一些参数，所以定义成一个变量比较利于维护。这两个例子的意思都是先进入“subdir”目录，然后执行make命令。
我们把这个Makefile叫做“总控Makefile”，总控Makefile的变量可以传递到下级的Makefile中（如果你显示的声明），但是不会覆盖下层的Makefile中所定义的变量，除非指定了“-e”参数。
如果你要传递变量到下级Makefile中，那么你可以使用这样的声明：
    export <variable ...>;
如果你不想让某些变量传递到下级Makefile中，那么你可以这样声明： 
    unexport <variable ...>;
如：
    示例一：
        export variable = value
        其等价于：
        variable = value
        export variable
        其等价于：
        export variable := value
        其等价于：
        variable := value
        export variable
    示例二：
        export variable += value
        其等价于：
        variable += value
        export variable
如果你要传递所有的变量，那么，只要一个export就行了。后面什么也不用跟，表示传递所有的变量。
需要注意的是，有两个变量，一个是SHELL，一个是MAKEFLAGS，这两个变量不管你是否export，其总是要传递到下层Makefile中，特别是MAKEFILES变量，其中包含了make的参数信息，如果我们执行“总控Makefile”时有make参数或是在上层Makefile中定义了这个变量，那么MAKEFILES变量将会是这些参数，并会传递到下层Makefile中，这是一个系统级的环境变量。
但是make命令中的有几个参数并不往下传递，它们是“-C”,“-f”,“-h”“-o”和“-W”（有关Makefile参数的细节将在后面说明），如果你不想往下层传递参数，那么，你可以这样来：
    subsystem:
            cd subdir && $(MAKE) MAKEFLAGS=
如果你定义了环境变量MAKEFLAGS，那么你得确信其中的选项是大家都会用到的，如果其中有“-t”,“-n”,和“-q”参数，那么将会有让你意想不到的结果，或许会让你异常地恐慌。
还有一个在“嵌套执行”中比较有用的参数，“-w”或是“--print-directory”会在make的过程中输出一些信息，让你看到目前的工作目录。比如，如果我们的下级make目录是“/home/hchen/gnu/make”，如果我们使用“make -w”来执行，那么当进入该目录时，我们会看到：
    make: Entering directory `/home/hchen/gnu/make'.
而在完成下层make后离开目录时，我们会看到：
    make: Leaving directory `/home/hchen/gnu/make'
当你使用“-C”参数来指定make下层Makefile时，“-w”会被自动打开的。如果参数中有“-s”（“--slient”）或是“--no-print-directory”，那么，“-w”总是失效的。
五、定义命令包
如果Makefile中出现一些相同命令序列，那么我们可以为这些相同的命令序列定义一个变量。定义这种命令序列的语法以“define”开始，以“endef”结束，如：
    define run-yacc
    yacc $(firstword $^)
    mv y.tab.c $@
    endef
这里，“run-yacc”是这个命令包的名字，其不要和Makefile中的变量重名。在“define”和“endef”中的两行就是命令序列。这个命令包中的第一个命令是运行Yacc程序，因为Yacc程序总是生成“y.tab.c”的文件，所以第二行的命令就是把这个文件改改名字。还是把这个命令包放到一个示例中来看看吧。
    foo.c : foo.y
            $(run-yacc)
我们可以看见，要使用这个命令包，我们就好像使用变量一样。在这个命令包的使用中，命令包“run-yacc”中的“$^”就是“foo.y”，“$@”就是“foo.c”（有关这种以“$”开头的特殊变量，我们会在后面介绍），make在执行命令包时，命令包中的每个命令会被依次独立执行。
使用变量
————
在Makefile中的定义的变量，就像是C/C++语言中的宏一样，他代表了一个文本字串，在Makefile中执行的时候其会自动原模原样地展开在所使用的地方。其与C/C++所不同的是，你可以在Makefile中改变其值。在Makefile中，变量可以使用在“目标”，“依赖目标”，“命令”或是Makefile的其它部分中。
变量的命名字可以包含字符、数字，下划线（可以是数字开头），但不应该含有“:”、“#”、“=”或是空字符（空格、回车等）。变量是大小写敏感的，“foo”、“Foo”和“FOO”是三个不同的变量名。传统的Makefile的变量名是全大写的命名方式，但我推荐使用大小写搭配的变量名，如：MakeFlags。这样可以避免和系统的变量冲突，而发生意外的事情。
有一些变量是很奇怪字串，如“$<”、“$@”等，这些是自动化变量，我会在后面介绍。
一、变量的基础
变量在声明时需要给予初值，而在使用时，需要给在变量名前加上“$”符号，但最好用小括号“（）”或是大括号“{}”把变量给包括起来。如果你要使用真实的“$”字符，那么你需要用“$$”来表示。
变量可以使用在许多地方，如规则中的“目标”、“依赖”、“命令”以及新的变量中。先看一个例子：
    objects = program.o foo.o utils.o
    program : $(objects)
            cc -o program $(objects)
    $(objects) : defs.h
变量会在使用它的地方精确地展开，就像C/C++中的宏一样，例如：
    foo = c
    prog.o : prog.$(foo)
            $(foo)$(foo) -$(foo) prog.$(foo)
展开后得到：
    prog.o : prog.c
            cc -c prog.c
当然，千万不要在你的Makefile中这样干，这里只是举个例子来表明Makefile中的变量在使用处展开的真实样子。可见其就是一个“替代”的原理。
另外，给变量加上括号完全是为了更加安全地使用这个变量，在上面的例子中，如果你不想给变量加上括号，那也可以，但我还是强烈建议你给变量加上括号。
二、变量中的变量
在定义变量的值时，我们可以使用其它变量来构造变量的值，在Makefile中有两种方式来在用变量定义变量的值。
先看第一种方式，也就是简单的使用“=”号，在“=”左侧是变量，右侧是变量的值，右侧变量的值可以定义在文件的任何一处，也就是说，右侧中的变量不一定非要是已定义好的值，其也可以使用后面定义的值。如：
    foo = $(bar)
    bar = $(ugh)
    ugh = Huh?
    all:
            echo $(foo)
我们执行“make all”将会打出变量$(foo)的值是“Huh?”（ $(foo)的值是$(bar)，$(bar)的值是$(ugh)，$(ugh)的值是“Huh?”）可见，变量是可以使用后面的变量来定义的。
这个功能有好的地方，也有不好的地方，好的地方是，我们可以把变量的真实值推到后面来定义，如：
    CFLAGS = $(include_dirs) -O
    include_dirs = -Ifoo -Ibar
当“CFLAGS”在命令中被展开时，会是“-Ifoo -Ibar -O”。但这种形式也有不好的地方，那就是递归定义，如：
    CFLAGS = $(CFLAGS) -O
    或：
    A = $(B)
    B = $(A)
这会让make陷入无限的变量展开过程中去，当然，我们的make是有能力检测这样的定义，并会报错。还有就是如果在变量中使用函数，那么，这种方式会让我们的make运行时非常慢，更糟糕的是，他会使用得两个make的函数“wildcard”和“shell”发生不可预知的错误。因为你不会知道这两个函数会被调用多少次。
为了避免上面的这种方法，我们可以使用make中的另一种用变量来定义变量的方法。这种方法使用的是“:=”操作符，如：
    x := foo
    y := $(x) bar
    x := later
其等价于：
    y := foo bar
    x := later
值得一提的是，这种方法，前面的变量不能使用后面的变量，只能使用前面已定义好了的变量。如果是这样：
    y := $(x) bar
    x := foo
那么，y的值是“bar”，而不是“foo bar”。
上面都是一些比较简单的变量使用了，让我们来看一个复杂的例子，其中包括了make的函数、条件表达式和一个系统变量“MAKELEVEL”的使用：
    ifeq (0,${MAKELEVEL})
    cur-dir   := $(shell pwd)
    whoami    := $(shell whoami)
    host-type := $(shell arch)
    MAKE := ${MAKE} host-type=${host-type} whoami=${whoami}
    endif
关于条件表达式和函数，我们在后面再说，对于系统变量“MAKELEVEL”，其意思是，如果我们的make有一个嵌套执行的动作（参见前面的“嵌套使用make”），那么，这个变量会记录了我们的当前Makefile的调用层数。
下面再介绍两个定义变量时我们需要知道的，请先看一个例子，如果我们要定义一个变量，其值是一个空格，那么我们可以这样来：
    nullstring :=
    space := $(nullstring) # end of the line
nullstring是一个Empty变量，其中什么也没有，而我们的space的值是一个空格。因为在操作符的右边是很难描述一个空格的，这里采用的技术很管用，先用一个Empty变量来标明变量的值开始了，而后面采用“#”注释符来表示变量定义的终止，这样，我们可以定义出其值是一个空格的变量。请注意这里关于“#”的使用，注释符“#”的这种特性值得我们注意，如果我们这样定义一个变量：
    dir := /foo/bar    # directory to put the frobs in
dir这个变量的值是“/foo/bar”，后面还跟了4个空格，如果我们这样使用这样变量来指定别的目录——“$(dir)/file”那么就完蛋了。
还有一个比较有用的操作符是“?=”，先看示例：
    FOO ?= bar
其含义是，如果FOO没有被定义过，那么变量FOO的值就是“bar”，如果FOO先前被定义过，那么这条语将什么也不做，其等价于：
    ifeq ($(origin FOO), undefined)
      FOO = bar
    endif
三、变量高级用法
这里介绍两种变量的高级使用方法，第一种是变量值的替换。
我们可以替换变量中的共有的部分，其格式是“$(var:a=b)”或是“${var:a=b}”，其意思是，把变量“var”中所有以“a”字串“结尾”的“a”替换成“b”字串。这里的“结尾”意思是“空格”或是“结束符”。
还是看一个示例吧：
    foo := a.o b.o c.o
    bar := $(foo:.o=.c)
这个示例中，我们先定义了一个“$(foo)”变量，而第二行的意思是把“$(foo)”中所有以“.o”字串“结尾”全部替换成“.c”，所以我们的“$(bar)”的值就是“a.c b.c c.c”。
另外一种变量替换的技术是以“静态模式”（参见前面章节）定义的，如：
    foo := a.o b.o c.o
    bar := $(foo:%.o=%.c)
这依赖于被替换字串中的有相同的模式，模式中必须包含一个“%”字符，这个例子同样让$(bar)变量的值为“a.c b.c c.c”。 
第二种高级用法是——“把变量的值再当成变量”。先看一个例子：
    x = y
    y = z
    a := $($(x))
在这个例子中，$(x)的值是“y”，所以$($(x))就是$(y)，于是$(a)的值就是“z”。（注意，是“x=y”，而不是“x=$(y)”）
我们还可以使用更多的层次：
    x = y
    y = z
    z = u
    a := $($($(x)))
这里的$(a)的值是“u”，相关的推导留给读者自己去做吧。
让我们再复杂一点，使用上“在变量定义中使用变量”的第一个方式，来看一个例子：
    x = $(y)
    y = z
    z = Hello
    a := $($(x))
这里的$($(x))被替换成了$($(y))，因为$(y)值是“z”，所以，最终结果是：a:=$(z)，也就是“Hello”。
再复杂一点，我们再加上函数：
    x = variable1
    variable2 := Hello
    y = $(subst 1,2,$(x))
    z = y
    a := $($($(z)))
这个例子中，“$($($(z)))”扩展为“$($(y))”，而其再次被扩展为“$($(subst 1,2,$(x)))”。$(x)的值是“variable1”，subst函数把“variable1”中的所有“1”字串替换成“2”字串，于是，“variable1”变成“variable2”，再取其值，所以，最终，$(a)的值就是$(variable2)的值——“Hello”。（喔，好不容易）
在这种方式中，或要可以使用多个变量来组成一个变量的名字，然后再取其值：
    first_second = Hello
    a = first
    b = second
    all = $($a_$b)
这里的“$a_$b”组成了“first_second”，于是，$(all)的值就是“Hello”。
再来看看结合第一种技术的例子：
    a_objects := a.o b.o c.o
    1_objects := 1.o 2.o 3.o
    sources := $($(a1)_objects:.o=.c)
这个例子中，如果$(a1)的值是“a”的话，那么，$(sources)的值就是“a.c b.c c.c”；如果$(a1)的值是“1”，那么$(sources)的值是“1.c 2.c 3.c”。
再来看一个这种技术和“函数”与“条件语句”一同使用的例子：
    ifdef do_sort
    func := sort
    else
    func := strip
    endif
    bar := a d b g q c
    foo := $($(func) $(bar))
这个示例中，如果定义了“do_sort”，那么：foo := $(sort a d b g q c)，于是$(foo)的值就是“a b c d g q”，而如果没有定义“do_sort”，那么：foo := $(sort a d b g q c)，调用的就是strip函数。
当然，“把变量的值再当成变量”这种技术，同样可以用在操作符的左边：
    dir = foo
    $(dir)_sources := $(wildcard $(dir)/*.c)
    define $(dir)_print
    lpr $($(dir)_sources)
    endef
这个例子中定义了三个变量：“dir”，“foo_sources”和“foo_print”。
四、追加变量值
我们可以使用“+=”操作符给变量追加值，如：
    objects = main.o foo.o bar.o utils.o
    objects += another.o
于是，我们的$(objects)值变成：“main.o foo.o bar.o utils.o another.o”（another.o被追加进去了）
使用“+=”操作符，可以模拟为下面的这种例子：
    objects = main.o foo.o bar.o utils.o
    objects := $(objects) another.o
所不同的是，用“+=”更为简洁。
如果变量之前没有定义过，那么，“+=”会自动变成“=”，如果前面有变量定义，那么“+=”会继承于前次操作的赋值符。如果前一次的是“:=”，那么“+=”会以“:=”作为其赋值符，如：
    variable := value
    variable += more
等价于：
    variable := value
    variable := $(variable) more
但如果是这种情况： 
    variable = value
    variable += more
由于前次的赋值符是“=”，所以“+=”也会以“=”来做为赋值，那么岂不会发生变量的递补归定义，这是很不好的，所以make会自动为我们解决这个问题，我们不必担心这个问题。
五、override 指示符
如果有变量是通常make的命令行参数设置的，那么Makefile中对这个变量的赋值会被忽略。如果你想在Makefile中设置这类参数的值，那么，你可以使用“override”指示符。其语法是：
    override <variable>; = <value>;
    override <variable>; := <value>;
当然，你还可以追加：
    override <variable>; += <more text>;
对于多行的变量定义，我们用define指示符，在define指示符前，也同样可以使用ovveride指示符，如：
    override define foo
    bar
    endef
使用函数
————
在Makefile中可以使用函数来处理变量，从而让我们的命令或是规则更为的灵活和具有智能。make所支持的函数也不算很多，不过已经足够我们的操作了。函数调用后，函数的返回值可以当做变量来使用。
一、函数的调用语法
函数调用，很像变量的使用，也是以“$”来标识的，其语法如下：
    $(<function>; <arguments>;)
或是
    ${<function>; <arguments>;}
这里，<function>;就是函数名，make支持的函数不多。<arguments>;是函数的参数，参数间以逗号“,”分隔，而函数名和参数之间以“空格”分隔。函数调用以“$”开头，以圆括号或花括号把函数名和参数括起。感觉很像一个变量，是不是？函数中的参数可以使用变量，为了风格的统一，函数和变量的括号最好一样，如使用“$(subst a,b,$(x))”这样的形式，而不是“$(subst a,b,${x})”的形式。因为统一会更清楚，也会减少一些不必要的麻烦。
还是来看一个示例：
    comma:= ,
    empty:=
    space:= $(empty) $(empty)
    foo:= a b c
    bar:= $(subst $(space),$(comma),$(foo))
在这个示例中，$(comma)的值是一个逗号。$(space)使用了$(empty)定义了一个空格，$(foo)的值是“a b c”，$(bar)的定义用，调用了函数“subst”，这是一个替换函数，这个函数有三个参数，第一个参数是被替换字串，第二个参数是替换字串，第三个参数是替换操作作用的字串。这个函数也就是把$(foo)中的空格替换成逗号，所以$(bar)的值是“a,b,c”。
二、字符串处理函数
$(subst <from>;,<to>;,<text>;) 
    名称：字符串替换函数——subst。
    功能：把字串<text>;中的<from>;字符串替换成<to>;。
    返回：函数返回被替换过后的字符串。
    示例：
        $(subst ee,EE,feet on the street)，
        把“feet on the street”中的“ee”替换成“EE”，返回结果是“fEEt on the strEEt”。
$(patsubst <pattern>;,<replacement>;,<text>;) 
    名称：模式字符串替换函数——patsubst。
    功能：查找<text>;中的单词（单词以“空格”、“Tab”或“回车”“换行”分隔）是否符合模式<pattern>;，如果匹配的话，则以<replacement>;替换。这里，<pattern>;可以包括通配符“%”，表示任意长度的字串。如果<replacement>;中也包含“%”，那么，<replacement>;中的这个“%”将是<pattern>;中的那个“%”所代表的字串。（可以用“\”来转义，以“\%”来表示真实含义的“%”字符）
    返回：函数返回被替换过后的字符串。
    示例：
        $(patsubst %.c,%.o,x.c.c bar.c)
        把字串“x.c.c bar.c”符合模式[%.c]的单词替换成[%.o]，返回结果是“x.c.o bar.o”
    备注：
        这和我们前面“变量章节”说过的相关知识有点相似。如：
        “$(var:<pattern>;=<replacement>;)”
         相当于
        “$(patsubst <pattern>;,<replacement>;,$(var))”，
         而“$(var: <suffix>;=<replacement>;)”
         则相当于
         “$(patsubst %<suffix>;,%<replacement>;,$(var))”。
         例如有：objects = foo.o bar.o baz.o，
         那么，“$(objects:.o=.c)”和“$(patsubst %.o,%.c,$(objects))”是一样的。
$(strip <string>;)
    名称：去空格函数——strip。
    功能：去掉<string>;字串中开头和结尾的空字符。
    返回：返回被去掉空格的字符串值。
    示例：
        $(strip a b c )
        把字串“a b c ”去到开头和结尾的空格，结果是“a b c”。
$(findstring <find>;,<in>;)
    名称：查找字符串函数——findstring。
    功能：在字串<in>;中查找<find>;字串。
    返回：如果找到，那么返回<find>;，否则返回空字符串。
    示例：
        $(findstring a,a b c)
        $(findstring a,b c)
        第一个函数返回“a”字符串，第二个返回“”字符串（空字符串）
$(filter <pattern...>;,<text>;)
    名称：过滤函数——filter。
    功能：以<pattern>;模式过滤<text>;字符串中的单词，保留符合模式<pattern>;的单词。可以有多个模式。
    返回：返回符合模式<pattern>;的字串。
    示例：
        sources := foo.c bar.c baz.s ugh.h
        foo: $(sources)
                cc $(filter %.c %.s,$(sources)) -o foo
        $(filter %.c %.s,$(sources))返回的值是“foo.c bar.c baz.s”。
$(filter-out <pattern...>;,<text>;)
    名称：反过滤函数——filter-out。
    功能：以<pattern>;模式过滤<text>;字符串中的单词，去除符合模式<pattern>;的单词。可以有多个模式。
    返回：返回不符合模式<pattern>;的字串。
    示例：
        objects=main1.o foo.o main2.o bar.o
        mains=main1.o main2.o
        $(filter-out $(mains),$(objects)) 返回值是“foo.o bar.o”。
$(sort <list>;)
    名称：排序函数——sort。
    功能：给字符串<list>;中的单词排序（升序）。
    返回：返回排序后的字符串。
    示例：$(sort foo bar lose)返回“bar foo lose” 。
    备注：sort函数会去掉<list>;中相同的单词。
$(word <n>;,<text>;)
    名称：取单词函数——word。
    功能：取字符串<text>;中第<n>;个单词。（从一开始）
    返回：返回字符串<text>;中第<n>;个单词。如果<n>;比<text>;中的单词数要大，那么返回空字符串。
    示例：$(word 2, foo bar baz)返回值是“bar”。
$(wordlist <s>;,<e>;,<text>;)  
    名称：取单词串函数——wordlist。
    功能：从字符串<text>;中取从<s>;开始到<e>;的单词串。<s>;和<e>;是一个数字。
    返回：返回字符串<text>;中从<s>;到<e>;的单词字串。如果<s>;比<text>;中的单词数要大，那么返回空字符串。如果<e>;大于<text>;的单词数，那么返回从<s>;开始，到<text>;结束的单词串。
    示例： $(wordlist 2, 3, foo bar baz)返回值是“bar baz”。
$(words <text>;)
    名称：单词个数统计函数——words。
    功能：统计<text>;中字符串中的单词个数。
    返回：返回<text>;中的单词数。
    示例：$(words, foo bar baz)返回值是“3”。
    备注：如果我们要取<text>;中最后的一个单词，我们可以这样：$(word $(words <text>;),<text>;)。
$(firstword <text>;)
    名称：首单词函数——firstword。
    功能：取字符串<text>;中的第一个单词。
    返回：返回字符串<text>;的第一个单词。
    示例：$(firstword foo bar)返回值是“foo”。
    备注：这个函数可以用word函数来实现：$(word 1,<text>;)。
以上，是所有的字符串操作函数，如果搭配混合使用，可以完成比较复杂的功能。这里，举一个现实中应用的例子。我们知道，make使用“VPATH”变量来指定“依赖文件”的搜索路径。于是，我们可以利用这个搜索路径来指定编译器对头文件的搜索路径参数CFLAGS，如：
    override CFLAGS += $(patsubst %,-I%,$(subst :, ,$(VPATH)))
    如果我们的“$(VPATH)”值是“src:../headers”，那么“$(patsubst %,-I%,$(subst :, ,$(VPATH)))”将返回“-Isrc -I../headers”，这正是cc或gcc搜索头文件路径的参数。
三、文件名操作函数
下面我们要介绍的函数主要是处理文件名的。每个函数的参数字符串都会被当做一个或是一系列的文件名来对待。
$(dir <names...>;) 
    名称：取目录函数——dir。
    功能：从文件名序列<names>;中取出目录部分。目录部分是指最后一个反斜杠（“/”）之前的部分。如果没有反斜杠，那么返回“./”。
    返回：返回文件名序列<names>;的目录部分。
    示例： $(dir src/foo.c hacks)返回值是“src/ ./”。
$(notdir <names...>;) 
    名称：取文件函数——notdir。
    功能：从文件名序列<names>;中取出非目录部分。非目录部分是指最后一个反斜杠（“/”）之后的部分。
    返回：返回文件名序列<names>;的非目录部分。
    示例： $(notdir src/foo.c hacks)返回值是“foo.c hacks”。
$(suffix <names...>;) 
    名称：取后缀函数——suffix。
    功能：从文件名序列<names>;中取出各个文件名的后缀。
    返回：返回文件名序列<names>;的后缀序列，如果文件没有后缀，则返回空字串。
    示例：$(suffix src/foo.c src-1.0/bar.c hacks)返回值是“.c .c”。
$(basename <names...>;)
    名称：取前缀函数——basename。
    功能：从文件名序列<names>;中取出各个文件名的前缀部分。
    返回：返回文件名序列<names>;的前缀序列，如果文件没有前缀，则返回空字串。
    示例：$(basename src/foo.c src-1.0/bar.c hacks)返回值是“src/foo src-1.0/bar hacks”。
$(addsuffix <suffix>;,<names...>;) 
    名称：加后缀函数——addsuffix。
    功能：把后缀<suffix>;加到<names>;中的每个单词后面。
    返回：返回加过后缀的文件名序列。
    示例：$(addsuffix .c,foo bar)返回值是“foo.c bar.c”。
$(addprefix <prefix>;,<names...>;) 
    名称：加前缀函数——addprefix。
    功能：把前缀<prefix>;加到<names>;中的每个单词后面。
    返回：返回加过前缀的文件名序列。
    示例：$(addprefix src/,foo bar)返回值是“src/foo src/bar”。
$(join <list1>;,<list2>;)
    名称：连接函数——join。
    功能：把<list2>;中的单词对应地加到<list1>;的单词后面。如果<list1>;的单词个数要比<list2>;的多，那么，<list1>;中的多出来的单词将保持原样。如果<list2>;的单词个数要比<list1>;多，那么，<list2>;多出来的单词将被复制到<list2>;中。
    返回：返回连接过后的字符串。
    示例：$(join aaa bbb , 111 222 333)返回值是“aaa111 bbb222 333”。
四、foreach 函数
foreach函数和别的函数非常的不一样。因为这个函数是用来做循环用的，Makefile中的foreach函数几乎是仿照于Unix标准Shell（/bin/sh）中的for语句，或是C-Shell（/bin/csh）中的foreach语句而构建的。它的语法是：
    $(foreach <var>;,<list>;,<text>;)
这个函数的意思是，把参数<list>;中的单词逐一取出放到参数<var>;所指定的变量中，然后再执行<text>;所包含的表达式。每一次<text>;会返回一个字符串，循环过程中，<text>;的所返回的每个字符串会以空格分隔，最后当整个循环结束时，<text>;所返回的每个字符串所组成的整个字符串（以空格分隔）将会是foreach函数的返回值。
所以，<var>;最好是一个变量名，<list>;可以是一个表达式，而<text>;中一般会使用<var>;这个参数来依次枚举<list>;中的单词。举个例子：
    names := a b c d
    files := $(foreach n,$(names),$(n).o)
上面的例子中，$(name)中的单词会被挨个取出，并存到变量“n”中，“$(n).o”每次根据“$(n)”计算出一个值，这些值以空格分隔，最后作为foreach函数的返回，所以，$(files)的值是“a.o b.o c.o d.o”。
注意，foreach中的<var>;参数是一个临时的局部变量，foreach函数执行完后，参数<var>;的变量将不在作用，其作用域只在foreach函数当中。
五、if 函数
if函数很像GNU的make所支持的条件语句——ifeq（参见前面所述的章节），if函数的语法是：
    $(if <condition>;,<then-part>;) 
或是
    $(if <condition>;,<then-part>;,<else-part>;)
可见，if函数可以包含“else”部分，或是不含。即if函数的参数可以是两个，也可以是三个。<condition>;参数是if的表达式，如果其返回的为非空字符串，那么这个表达式就相当于返回真，于是，<then-part>;会被计算，否则<else-part>;会被计算。
而if函数的返回值是，如果<condition>;为真（非空字符串），那个<then-part>;会是整个函数的返回值，如果<condition>;为假（空字符串），那么<else-part>;会是整个函数的返回值，此时如果<else-part>;没有被定义，那么，整个函数返回空字串。
所以，<then-part>;和<else-part>;只会有一个被计算。
六、call函数
call函数是唯一一个可以用来创建新的参数化的函数。你可以写一个非常复杂的表达式，这个表达式中，你可以定义许多参数，然后你可以用call函数来向这个表达式传递参数。其语法是：
    $(call <expression>;,<parm1>;,<parm2>;,<parm3>;...)
当make执行这个函数时，<expression>;参数中的变量，如$(1)，$(2)，$(3)等，会被参数<parm1>;，<parm2>;，<parm3>;依次取代。而<expression>;的返回值就是call函数的返回值。例如：
    reverse =  $(1) $(2)
    foo = $(call reverse,a,b)
那么，foo的值就是“a b”。当然，参数的次序是可以自定义的，不一定是顺序的，如：
    reverse =  $(2) $(1)
    foo = $(call reverse,a,b)
此时的foo的值就是“b a”。
七、origin函数
origin函数不像其它的函数，他并不操作变量的值，他只是告诉你你的这个变量是哪里来的？其语法是：
    $(origin <variable>;)
注意，<variable>;是变量的名字，不应该是引用。所以你最好不要在<variable>;中使用“$”字符。Origin函数会以其返回值来告诉你这个变量的“出生情况”，下面，是origin函数的返回值:
“undefined”
      如果<variable>;从来没有定义过，origin函数返回这个值“undefined”。
“default”
      如果<variable>;是一个默认的定义，比如“CC”这个变量，这种变量我们将在后面讲述。
“environment”
      如果<variable>;是一个环境变量，并且当Makefile被执行时，“-e”参数没有被打开。
“file”
      如果<variable>;这个变量被定义在Makefile中。
“command line”
      如果<variable>;这个变量是被命令行定义的。
“override”
      如果<variable>;是被override指示符重新定义的。
“automatic”
      如果<variable>;是一个命令运行中的自动化变量。关于自动化变量将在后面讲述。
这些信息对于我们编写Makefile是非常有用的，例如，假设我们有一个Makefile其包了一个定义文件Make.def，在Make.def中定义了一个变量“bletch”，而我们的环境中也有一个环境变量“bletch”，此时，我们想判断一下，如果变量来源于环境，那么我们就把之重定义了，如果来源于Make.def或是命令行等非环境的，那么我们就不重新定义它。于是，在我们的Makefile中，我们可以这样写：
    ifdef bletch
    ifeq "$(origin bletch)" "environment"
    bletch = barf, gag, etc.
    endif
    endif
当然，你也许会说，使用override关键字不就可以重新定义环境中的变量了吗？为什么需要使用这样的步骤？是的，我们用override是可以达到这样的效果，可是override过于粗暴，它同时会把从命令行定义的变量也覆盖了，而我们只想重新定义环境传来的，而不想重新定义命令行传来的。
八、shell函数
shell函数也不像其它的函数。顾名思义，它的参数应该就是操作系统Shell的命令。它和反引号“`”是相同的功能。这就是说，shell函数把执行操作系统命令后的输出作为函数返回。于是，我们可以用操作系统命令以及字符串处理命令awk，sed等等命令来生成一个变量，如：
    contents := $(shell cat foo)
    files := $(shell echo *.c)
注意，这个函数会新生成一个Shell程序来执行命令，所以你要注意其运行性能，如果你的Makefile中有一些比较复杂的规则，并大量使用了这个函数，那么对于你的系统性能是有害的。特别是Makefile的隐晦的规则可能会让你的shell函数执行的次数比你想像的多得多。
九、控制make的函数
make提供了一些函数来控制make的运行。通常，你需要检测一些运行Makefile时的运行时信息，并且根据这些信息来决定，你是让make继续执行，还是停止。
$(error <text ...>;)
    产生一个致命的错误，<text ...>;是错误信息。注意，error函数不会在一被使用就会产生错误信息，所以如果你把其定义在某个变量中，并在后续的脚本中使用这个变量，那么也是可以的。例如：
    示例一：
    ifdef ERROR_001
    $(error error is $(ERROR_001))
    endif
    示例二：
    ERR = $(error found an error!)
    .PHONY: err
    err: ; $(ERR)
    示例一会在变量ERROR_001定义了后执行时产生error调用，而示例二则在目录err被执行时才发生error调用。
$(warning <text ...>;)
     这个函数很像error函数，只是它并不会让make退出，只是输出一段警告信息，而make继续执行。
