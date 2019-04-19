# tornado模板引擎原理 - 宇宙浪子的专栏 - CSDN博客
2017年08月15日 17:51:57[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：154
个人分类：[Python																[tornado](https://blog.csdn.net/bluehawksky/article/category/7102850)](https://blog.csdn.net/bluehawksky/article/category/2542577)
转自：[http://blog.csdn.net/wyx819/article/details/45652713](http://blog.csdn.net/wyx819/article/details/45652713)
## 前言
老师问小明：已经a=1, 
求a+1的值。 
小明挠挠头，思考后回答：老师，a+1的结果是2。
以上是一个非常简单的例子，实际上就是一个模板编译过程。 
a=1,表示一个命名空间(namespace)中的一项,该命名空间中存在一个名称(name)为a,其值为1。在[Python](http://lib.csdn.net/base/python)里面，命名空间可以用[数据结构](http://lib.csdn.net/base/datastructure)字典来表示。
a+1，实际上是一个非常简单的模板。这个模板里面有且只有一条规则a+1。本博文中所说的模板，实际上更类似一份空白的数学考卷。这份数学考卷中，有很多类似上面老师问小明的题目。
学生解决这份考卷的过程，就是一个模板的编译过程。学生解答每一道题目获得答案，类似于模板每一个节点编译。一个模板是由很多节点组成。这样看来我们解决“模板编译”问题，可以转化成解决各个节点编译问题。反过来说，只要各个节点的编译问题解决了，整个模板的编译也就解决了。我们通常将一个大问题，转化成各个并行不相干的小问题(可以认为：一个大问题等于n个小问题相加)，然后各个击破。
不过有时候，会遇到比a+1更加难一点的问题：
老师对小明说：现在要求a+1的值，但现在a是一个未知数，只知道a=b+1, 
不过我们知道b=1。 
小明思考了一分钟，对老师说：a+1等于3，
我们知道要想解决a+1的值，a的值首先要获得，我们称解决a的值是解决a+1问题不可或缺的一个嵌套子问题。只有这个嵌套子问题获得解决，原来的问题才能获得解决的。所幸，老师提供a=b+1，而且知道了b=1，那么小明可以知道a=2,进而知道a+1等于3。这样思考过程，本质上对应于递归[算法](http://lib.csdn.net/base/datastructure)解决问题的思路。那么我们可以用计算机模拟这种问题的编译。
总结一下数学考试的过程，答一份数学考卷的过程。如果答一份数学考卷交给[机器人](http://lib.csdn.net/base/robot)来做，会用到怎样的算法实现？
伪代码如下：
```python
def solved_big_problem(big_problem):
    small_problem_list = split(big_problem)
    answer = ""
    foreach small_problem in small_problem_list:
        if is_problem_small_enough(small_problem):
            answer+=small_problem.done()
        else:
            answer+=solved_big_problem(big_problem)
    return answer
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
## tornado模板引擎如何划分小问题
tornado template引擎中，将整个静态模板划分成不同类型的节点。 
在我认为，template中主要有三种类型的节点(Node) 
1. 文本节点(TextNode) 
2. 注释节点(CommentNode) 
3. 表达式节点(ExpressionNode) 
4. 块节点(BlockNode)
### 文本节点
一个文本节点是一个不会被命名空间数据渲染的字符串，也就是模板中，该字符串是什么，最后编译结束后输出的就是什么。
### 注释节点
一个注释节点，以特殊标签”{#”开始，以“#}”结束，中间是一个对模板的注释字符串，注释节点在编译结束后，不会产生任何输出。
### 表达式节点
一个表达式节点，以特殊标签”((“开始，以“}}”结束，中间是一个字符串，这个字符串可能仅仅是一个名称的名字，编译后会被命名空间中该名称对应的值替换掉。也可能是一个字面量。更或者是一个复杂的表达式，在c++语言中，我们说一个表达式可以作为右值，用在等号的右边，赋值给一个变量。表达式的编译结果会被给表达式实际的值所替换。一个表达式中可能包含运算或者函数调用。
### 块节点
块节点是节点中最复杂的一种。通常以”{%”开始，以“ %}”结束。“{%…%}”中间的第一个单词，称之为指令(operator)。多个块节点将组合成更大的模板单位。我们将看到这些模板单位，会被映射成[python](http://lib.csdn.net/base/python)语言中的，控制块, 循环体, 异常处理块,语句等等。
|指令|用法|
|----|----|
|apply|{% apply function %}…{% end %}|
|autoescape|{% autoescape function %}|
|block|{% block name %}…{% end %}|
|comment|{% comment … %}|
|extends|{% extends filename %}|
|for|{% for var in expr %}…{% end %}|
|from|{% from x import y %}|
|if|{% if condition %}…{% elif condition %}…{% else %}…{% end %}|
|import|{% import module %}|
|include|{% include filename %}|
|module|{% module expr %}|
|raw|{% raw expr %}|
|set|{% set x = y %}|
|try|{% try %}…{% except %}…{% else %}…{% finally %}…{% end %}|
|while|{% while condition %}… {% end %}|
|break|{% break %}|
|continue|{% continue %}|
## 模板解析
模板解析的过程实际上就是找出静态模板字符串中各种节点。
### 节点类
```python
class _Node(object):
    def each_child(self):
        return ()
    def generate(self, writer):
        raise NotImplementedError()
    def find_named_blocks(self, loader, named_blocks):
        for child in self.each_child():
            child.find_named_blocks(loader, named_blocks)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
def each_child(self): 
一个节点的话，可能会包含很多子节点，提供这个接口，返回该节点所有子节点。
def generate(self, writer): 
将该节点映射成python语言代码
def find_named_blocks(self, loader, named_blocks): 
找到节点body中包含的命名块，此接口在处理模板继承时候作用重大。
|_Node子类|表示节点类型|
|----|----|
|_File|整个模板|
|_ChunkList|多个并列节点的集合，也可作为一个节点的body|
|_NamedBlock|命名块，对应block指令 ，在模板继承中起作用|
|_ExtendsBlock|对应extends指令|
|_IncludeBlock|对应include指令|
|_ApplyBlock|对应apply指令|
|_ControlBlock|对应if指令，控制块|
|_IntermediateControlBlock|对应else,elif,except,finally指令|
|_Statement|对应import,from,set，break，continue指令|
|_Expression|对应raw指令|
|_Module|对应module指令|
|_Text|对应文本节点|
### 解析算法
tornado模板节点按是否含有子节点来说，其实只分为两类。和”{% end %}”结合才起作用的节点，称之为宿主节点。不必和”{% end %}”结合就能起作用的节点，称之为”直接节点“。另外称”{% end %}”为结束节点。
宿主节点不是一个简单的节点，里面可以寄居若干子节点，形成树状层级结构。直接节点可能是一个表达式，或者语句，不含有子节点，这种情况是很容易处理的。
而“宿主块节点”该如何解析?其实类似于我们熟悉的问题-括号匹配。因为一个宿主块节点和“｛% end %｝”，结合才起作用，类似一对括号，只有左右括号都存在的时候，才是一个完整的括号，起到它应该有的意义。
一般上过数据结构课的人都知道，可以通过数据结构栈，解决括号匹配问题，其实也可以解决宿主节点解析问题。
如果宿主节点含有子节点，如下模板代码。
```
{% block content %}
    {% for line in lines %}
        <p>line</p>
    {% end %}
    {% if user=="admin" %}
        <p>welcome admin!</p>
    {% end %}
    <p>just test</p>
{% end %}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
我们换另外一种思路，递归思想来考虑这个问题，发现也非常直接。简单伪代码如下(流程不太严谨)。
```python
def parse(template_string):
   child_nodes = []
   while !template_string.end():
       node = get_next_node(template_string)
       if is_end_node(node):
           return child_nodes
       if is_has_child_nodes(node):
           template_string = move(template_string)
           node.child_nodes = parse(template_string)
       else:
           node.child_nodes = []
       child_nodes.append(node)
       template_string = move(template_string)
   return child_nodes
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
所有在模板中节点，按照父子关系组织起来，是一个森林的数据结构。解析整个模板的过程，就是构造森林的过程。这棵森林所有子树的叶子节点，可以是文本节点，注释节点，表达式节点和块节点中具有表达式意义或者语句意义的节点。而内部节点是宿主节点，但解析遇到一个结束节点，就知道一个宿主节点的所有子节点都解析完成，是时候return。
tornado中对模板的解析，是在template模块中_parse函数。
## 模板编译
在web开发，利用模板，目的是通过不同数据渲染相同模板，结果产生针对用户的不同内容页面。整个过程就是一个顶级函数，传入参数两个：一个是模板字符串，另外一个是数据(命名空间)，返回的是一个字符串(最终用户在浏览器中看到的内容页面)。
我们在模板字符串中挖的洞，最后需要被实际的数据填补起来。所以关键问题是该如何去填补这些洞。
tornado中的做法非常干脆利落，直接将模板字符串，翻译成一个python函数，然后编译这个python函数，如果web server接受到客户端请求。就直接调用这个编译好的函数，返回的结果就是一个内容页面。
### 强大工具
所幸，python语言为tornado模板引擎实现，提供了极大便利——内建函数compile和exec语句。
#### 内建函数compile
![这里写图片描述](https://img-blog.csdn.net/20150514222045560)
python中提供了一个内建函数compile，可以编译一个源码字符串成一个code对象，返回的[code对象](http://late.am/post/2012/03/26/exploring-python-code-objects.html)，能够被exec语句或者eval()函数执行。
#### exec语句
[exec语句](https://docs.python.org/2.0/ref/exec.html)支持python代码动态执行。 
python2.7中的语法如下：
```python
exec code[ in globals[,locals]]
```
- 1
- 1
code可以是一个字符串，一个打开的文件对象或者一个code对象。如果是字符串，会被解析成python语句执行。如果是文件对象，会一直解析到EOF然后执行。如果是code对象，已经不需要解析，可以直接执行。
exec语句也可以看作一个函数(实际上，在python3上，exec就是一个函数),globals(字典结构)和locals(字典结构)都是exec函数的可选参数。如果两者都没有提供，code在当前exec语句的作用域上下文下执行（code中用到全局作用域的对象和局部作用域的对象，并且可以对它们做修改）。
上面提到exec语句执行不提供任何的可选参数，是非常糟糕的，code执行会污染exec语句所在的作用域，并且引发很多难以调试的bug。
globals参数，可以避免这种情况。globals参数，让我们可以自定义code执行的作用域上下文，code执行只能获取globals中提供的变量、函数、类型等等，不过有一种例外—-python中的buildins。在code执行前，exec语句会在globals中，插入一项，key为”_builtins_“。这样code执行可以利用python中所有的buildins(tornado模板中可以利用所用python内建buildins)。
既然code在自定义的作用域上下文执行(相当于在一个沙盒子中)，如果我们想在exec语句执行完毕，或者code中定义的类型，变量，函数等等，那该怎么办?
locals参数正是为此而来。
```python
_code = '''
def my_output():
    print "hello world"
'''
def main():
    compiled = compile(_code, "<string>", "exec")
    global_dict = {}
    local_dict = {}
    exec compiled in global_dict, local_dict
    local_dict["my_output"]()
if __name__ == '__main__':
    main()
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
但是如果locals参数没有提供，只提供了globals参数。exec语句执行完毕，我们还是可以访问到code中定义的变量，类型，函数。exec语句把这一切都插入到了globals字典中。
我们可以看到，当提供了globals和locals参数，exec中的code执行，如果在一个沙盒子中，可以避免对exec语句所在作用域的污染。
### tornado模板引擎如何应用这些工具
#### 从模板节点生成代码
还记得在template模块中的_Node基类提供了一个没有实现的方法–generate。每种模板节点都有自己生成代码的方式,generate方法将在子类中实现。
```python
class _File(_Node):
    def __init__(self, template, body):
        self.template = template
        self.body = body
        self.line = 0
    def generate(self, writer):
        writer.write_line("def _tt_execute():", self.line)
        with writer.indent():
            writer.write_line("_tt_buffer = []", self.line)
            writer.write_line("_tt_append = _tt_buffer.append", self.line)
            self.body.generate(writer)
            writer.write_line("return _tt_utf8('').join(_tt_buffer)", self.line)
    def each_child(self):
        return (self.body,)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
我们先来看看_File类的generate方法。_File实际上对应的是整个模板文件。那么_File对象调用generate方法返回的结果应该是整个模板文件映射成python代码的结果。这个结果是一个名叫_tt_execute的函数代码字符串。
函数中定义了两个变量。_tt_buffer，一个列表,列表中的每一项将会是各种类型的模板节点映射成python的代码。_tt_append,函数，实际上是_tt_buffer.append函数。
那么_tt_execute函数体的代码如何生成？ 
由self.body对象调用generate方法。self.body实际上是一个_ChunkList对象。
```python
class _ChunkList(_Node):
    def __init__(self, chunks):
        self.chunks = chunks
    def generate(self, writer):
        for chunk in self.chunks:
            chunk.generate(writer)
    def each_child(self):
        return self.chunks
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
_ChunkList对象是父节点对象的子节点对象列表。
我们看到生成代码的过程，实际是节点森林自顶向下调用generate方法的过程，知道所用的叶子节点的generate方法调用完毕，才自底向上返回。整个过程也遍历了一遍节点森林。
整个过程，很多节点调用generate方法，都是直接拿”{% …%}”中含有的内容就作为生成的python代码。因此tornado的模板语言，具有极大的灵活性，它可以直接用到python语言中的很多语法。个别节点的generate方法生成代码，需要做特殊的处理。
#### 模板继承
模板继承语法，让生成代码多了一些曲折。在tornado模板语法中，模板继承语法，主要由以下两个指令共同其作用:
|指令|用法|对应_Node子类|
|----|----|----|
|block|{% block name %}…{% end %}|_NamedBlock|
|extends|{% extends filename %}|_ExtendsBlock|
子模板，继承了一个父模板，父模板的命名块(_NamedBlock)将会被子模板中的命名块(_NamedBlock)替换。
这个过程，如果要生成代码该如何实现？ 
实现的原理，也很容易可以想到。我们对子模板生成代码，其实不应该调用子模板对应的_File对象的generate方法，反而应该调用的是这个模板继承链最顶级的父模板_File对象的generate方法。
template模块中的Template类，提供了_get_ancestors方法。
```python
def _get_ancestors(self, loader):
        ancestors = [self.file]
        for chunk in self.file.body.chunks:
            if isinstance(chunk, _ExtendsBlock):
                if not loader:
                    raise ParseError("{% extends %} block found, but no "
                                     "template loader")
                template = loader.load(chunk.name, self.name)
                ancestors.extend(template._get_ancestors(loader))
        return ancestors
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
那么又有另外一个问题，父模板中的命名块节点(_NamedBlock)，应该被子模板同名的命名块节点替换，这个又该如何处理？
tornado对这个问题的处理代码如下，思路很简单，就留读者去思考了。
```python
def _generate_python(self, loader, compress_whitespace):
        buffer = StringIO()
        try:
            # named_blocks maps from names to _NamedBlock objects
            named_blocks = {}
            ancestors = self._get_ancestors(loader)
            ancestors.reverse()
            for ancestor in ancestors:
                ancestor.find_named_blocks(loader, named_blocks)
            writer = _CodeWriter(buffer, named_blocks, loader, ancestors[0].template,
                                 compress_whitespace)
            ancestors[0].generate(writer)
            return buffer.getvalue()
        finally:
            buffer.close()
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
#### 编译与执行
第一步首先利用python内建函数compile，将_generate_python返回的python代码字符串编译成code对象。
code对象的执行主要在Template类的中的generate方法。
```
def generate(self, **kwargs):
        namespace = {
            "escape": escape.xhtml_escape,
            "xhtml_escape": escape.xhtml_escape,
            "url_escape": escape.url_escape,
            "json_encode": escape.json_encode,
            "squeeze": escape.squeeze,
            "linkify": escape.linkify,
            "datetime": datetime,
            "_tt_utf8": escape.utf8,  # for internal use
            "_tt_string_types": (unicode_type, bytes),
            "__name__": self.name.replace('.', '_'),
            "__loader__": ObjectDict(get_source=lambda name: self.code),
        }
        namespace.update(self.namespace)
        namespace.update(kwargs)
        exec_in(self.compiled, namespace)
        execute = namespace["_tt_execute"]
        linecache.clearcache()
        return execute()
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
exec_in是tornado util模块中的一个函数，主要是为了兼容python2和python3(在python2中exec作为语句使用，在python3以后，exec作为函数使用)。 
从上面的代码，我们可以看到tornado自定义了一个执行上下文(namespace)，提供了一些tornado模板中能够使用的基本函数，如“escape”，“xhtml_escape”等等。当然也会把，generate的参数（用于模板渲染）插入到namespace字典中。_tt_execute函数就在这样的一个namespace环境中执行，最后获得向客户端输出的内容页面字符串。
实际在真正的tornado web开发中，并不需要直接和Template对象打交道，而是通过web模块RequestHandler类的render方法和render_string方法，函数签名如下：
```python
def render_string(self, template_name, **kwargs):
def render(self, template_name, **kwargs):
```
- 1
- 2
- 1
- 2
render_string与render有什么区别？render_string方法是根据一个模板的名字和字典参数，对模板进行渲染，返回的是一个内容页面字符串。而render方法会将渲染的结果，发送到客户端。
我们来看看render_string函数的实现。
```python
def render_string(self, template_name, **kwargs):
        template_path = self.get_template_path()
        if not template_path:
            frame = sys._getframe(0)
            web_file = frame.f_code.co_filename
            while frame.f_code.co_filename == web_file:
                frame = frame.f_back
            template_path = os.path.dirname(frame.f_code.co_filename)
        with RequestHandler._template_loader_lock:
            if template_path not in RequestHandler._template_loaders:
                loader = self.create_template_loader(template_path)
                RequestHandler._template_loaders[template_path] = loader
            else:
                loader = RequestHandler._template_loaders[template_path]
        t = loader.load(template_name)
        namespace = self.get_template_namespace()
        namespace.update(kwargs)
        return t.generate(**namespace)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
render_string函数的作用，根据模板路径和模板名字加载指定的模板，加载模板，创建Template对象，最后调用Template对象的genereate方法。注意到传入generate方法的参数。namespace实际上首先是self.get_template_namespace()函数的返回结果。而get_template_namespace函数的实现如下:
```python
def get_template_namespace(self):
        namespace = dict(
            handler=self,
            request=self.request,
            current_user=self.current_user,
            locale=self.locale,
            _=self.locale.translate,
            static_url=self.static_url,
            xsrf_form_html=self.xsrf_form_html,
            reverse_url=self.reverse_url
        )
        namespace.update(self.ui)
        return namespace
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
惊喜发现!原来在tornado的模板中还可以用到那么多tornado框架的对象和函数。
## 总结
本博文主要介绍tornado模板引擎实现原理，包括解析和编译。当然其中也简要介绍了模板语法，模板应用，涉及到数据结构与一些简单算法。tornado模板引擎实现非常简要，整个template模块代码不足千行，但也非常强大。
希望这篇文件对学习tornado模板的人有帮助。也愿意和读者交流学习，可以在csdn我的个人主页找到我的邮件联系方式。如果有转载，请注明来源[http://blog.csdn.net/wyx819/article/details/45652713](http://blog.csdn.net/wyx819/article/details/45652713)。

