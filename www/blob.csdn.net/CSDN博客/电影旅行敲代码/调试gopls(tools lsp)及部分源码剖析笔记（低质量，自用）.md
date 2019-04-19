# 调试gopls(tools/lsp)及部分源码剖析笔记（低质量，自用） - 电影旅行敲代码 - CSDN博客
2019年02月22日 17:30:44[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：208标签：[language server](https://so.csdn.net/so/search/s.do?q=language server&t=blog)
个人分类：[golang](https://blog.csdn.net/dashuniuniu/article/category/8691518)
# gopls
[gopls](https://github.com/golang/tools/tree/master/cmd/gopls)是go官方给出的go-langserver的一个外部接口，核心是`x/tools/internal/lsp`，所以调试gopls可以说就是调试go lsp。
# 调试query
使用delve进行调试，调试命令如下。前半段是调试go-pls的调试命令，后面跟的是gopls执行时的参数，下面的命令就是要查询`internal/lsp/cmd/definition.go`偏移1277的位置定义的是东西。
```
$GOPATH/bin/dlv debug cmd/gopls/main.go -- query definition internal/lsp/cmd/definition.go:#1277
```
## gopls
对于gopls的核心代码就一行，剩余的是通过`x/tools/internal/tool`作为中间层实现的，可以看到gopls调用的是工具*tool.Main()*，tool.Main方法是一个通用的方法，但是第二个实参`&cmd.Application{}`将gopls和lsp联系起来。
```
func main() {
	tool.Main(context.Background(), &cmd.Application{}, os.Args[1:])
}
```
下面是lsp中cmd.Application的定义，
```
// Application is the main application as passed to tool.Main
// It handles the main command line parsing and dispatch to the sub commands.
type Application struct {
	// Core application flags
	// Embed the basic profiling flags supported by the tool package
	tool.Profile
	// We include the server configuration directly for now, so the flags work
	// even without the verb.
	// TODO: Remove this when we stop allowing the serve verb by default.
	Serve Serve
	// An initial, common go/packages configuration
	Config packages.Config
	// Support for remote lsp server
	Remote string `flag:"remote" help:"*EXPERIMENTAL* - forward all commands to a remote lsp"`
}
```
而命令`query definition internal/lsp/cmd/definition.go:#1277`对应的入口函数是`lsp/cmd/definition.go:Run()`，核心代码如下所示。下面有针对性介绍涉及到的一些结构和函数。
```
// Run performs the definition query as specified by args and prints the
// results to stdout.
func (d *definition) Run(ctx context.Context, args ...string) error {
	if len(args) != 1 {
		return tool.CommandLineErrorf("definition expects 1 argument")
	}
	view := cache.NewView(&d.query.app.Config)
	from, err := parseLocation(args[0])
	if err != nil {
		return err
	}
	f, err := view.GetFile(ctx, source.ToURI(from.Filename))
	if err != nil {
		return err
	}
	tok := f.GetToken()
	pos := tok.Pos(from.Start.Offset)
	if !pos.IsValid() {
		return fmt.Errorf("invalid position %v", from.Start.Offset)
	}
	ident, err := source.Identifier(ctx, view, f, pos)
	if err != nil {
		return err
	}
	if ident == nil {
		return fmt.Errorf("not an identifier")
	}
	var result interface{}
	switch d.query.Emulate {
	case "":
		result, err = buildDefinition(view, ident)
	case emulateGuru:
		result, err = buildGuruDefinition(view, ident)
	default:
		return fmt.Errorf("unknown emulation for definition: %s", d.query.Emulate)
	}
	if err != nil {
		return err
	}
	if d.query.JSON {
		enc := json.NewEncoder(os.Stdout)
		enc.SetIndent("", "\t")
		return enc.Encode(result)
	}
	switch d := result.(type) {
	case *Definition:
		fmt.Printf("%v: defined here as %s", d.Location, d.Description)
	case *guru.Definition:
		fmt.Printf("%s: defined here as %s", d.ObjPos, d.Desc)
	default:
		return fmt.Errorf("no printer for type %T", result)
	}
	return nil
}
```
整个过程如下：
- 新建*View*对象，然后将*file*添加到该对象中。其中会使用regex engine，对query的字符串"internal/lsp/cmd/definition.go:#1277"进行分析，获取`FileName`和`Offset`，并将FileName转化为URI。
- 然后对该文件进行parse得到tokens和AST
- 得到指定Offset对应的ast Node path，根据ast node path得出identifier信息
- 最后调用`buildDefinition()`和`buildGuruDefinition()`得到一个结果
### 什么是View
`View`是在[internal/lsp: support range formatting](https://go-review.googlesource.com/c/tools/+/138275/)中添加的，这个patch的主要目的是为了支持`rang formatting`，`View`现在的定义是处理cache文件夹中。`View`的定义如下，从fields我们很难推断出`View`的用途。
```
type View struct {
	mu sync.Mutex // protects all mutable state of the view
	Config packages.Config
    // 注意这里的File是cache.File
    // cache.File用于记录文件打开后的[]byte, parse之后的ast.File和token.File等信息
	files map[source.URI]*File
	analysisCache *source.AnalysisCache
}
```
但是我们从`View`的下面四个method（暂且称之为method）中可以大致推断出`View`的用途。`View`维护了一组文件，这些文件是当前正在处理的文件，例如对某文件进行`parse`后的结果，对某文件进行`range formatting`（类似于clang中的`Rewriter`）后的结果。由于中间可能会存在频繁地对这些文件进行parse的操作，所以需要将这些file的ast，token或者analysis的中间结果cache下来。
```
// GetFile returns a File for the given URI. It will always succeed because it
// adds the file to the managed set if needed.
func (v *View) GetFile(ctx context.Context, uri source.URI) (source.File, error)
// getFile is the unlocked internal implementation of GetFile.
func (v *View) getFile(uri source.URI) *File
func (v *View) parse(uri source.URI) error
// SetContent sets the overlay contents for a file. A nil content value will
// remove the file from the active set and revert it to its on-disk contents.
func (v *View) SetContent(ctx context.Context, uri source.URI, content []byte) (source.View, error)
```
#### View.parse
### cache.File
```
// File holds all the information we know about a file.
type File struct {
	URI     source.URI
	view    *View
	active  bool
	content []byte
	ast     *ast.File
	token   *token.File
	pkg     *packages.Package
}
```
`cache.File`主要包含`GetContent()`，`GetFileSet()`，`GetToken()`，`GetAST()`和`GetPackage()`，这些函数都是对文件进行处理需要用到的很重要的函数。
```
// GetContent returns the contents of the file, reading it from file system if needed.
func (f *File) GetContent() []byte {
	f.view.mu.Lock()
	defer f.view.mu.Unlock()
	f.read()
	return f.content
}
func (f *File) GetFileSet() *token.FileSet {
	return f.view.Config.Fset
}
func (f *File) GetToken() *token.File {
	f.view.mu.Lock()
	defer f.view.mu.Unlock()
	if f.token == nil {
		if err := f.view.parse(f.URI); err != nil {
			return nil
		}
	}
	return f.token
}
func (f *File) GetAST() *ast.File {
	f.view.mu.Lock()
	defer f.view.mu.Unlock()
	if f.ast == nil {
		if err := f.view.parse(f.URI); err != nil {
			return nil
		}
	}
	return f.ast
}
func (f *File) GetPackage() *packages.Package {
	f.view.mu.Lock()
	defer f.view.mu.Unlock()
	if f.pkg == nil {
		if err := f.view.parse(f.URI); err != nil {
			return nil
		}
	}
	return f.pkg
}
// read is the internal part of Read that presumes the lock is already held
func (f *File) read() {
	if f.content != nil {
		return
	}
	// we don't know the content yet, so read it
	filename, err := f.URI.Filename()
	if err != nil {
		return
	}
	content, err := ioutil.ReadFile(filename)
	if err != nil {
		return
	}
	f.content = content
}
```
与此类似的还有*token.File*和*ast.File*两个类型，
### cache.File.GetToken
### source.Identifier()
*Identifier()* 处在pakcage source中，源码如下，该方法主要是通过*token.Pos*返回指定位置的*IdentifierInfo*对象。该方法的核心是同一文件夹下的*identifier()*方法。
```
// Identifier returns identifier information for a position
// in a file, accounting for a potentially incomplete selector.
func Identifier(ctx context.Context, v View, f File, pos token.Pos) (*IdentifierInfo, error) {
	if result, err := identifier(ctx, v, f, pos); err != nil || result != nil {
		return result, err
	}
	// If the position is not an identifier but immediately follows
	// an identifier or selector period (as is common when
	// requesting a completion), use the path to the preceding node.
	result, err := identifier(ctx, v, f, pos-1)
	if result == nil && err == nil {
		err = fmt.Errorf("no identifier found")
	}
	return result, err
}
```
#### IdentifierInfo
IdentifierInfo的定义如下，它可以看作为*ast.Ident*的强化版本，添加了*Name*，*Range*，*Type*，*Declaration*等数据成员。
```
// IdentifierInfo holds information about an identifier in Go source.
type IdentifierInfo struct {
	Name  string
	Range Range
	File  File
	Type  struct {
		Range  Range
		Object types.Object
	}
	Declaration struct {
		Range  Range
		Object types.Object
	}
	ident            *ast.Ident
	wasEmbeddedField bool
}
```
#### identifier()
*identifier()*主要是为了检查指定位置是否为*identifier*，所以首先就需要将其转化为AST，然后获取到指定位置的AST node，这部分源码如下。核心是通过*View.GetAST()*获取到ast，然后调用*astutil.PathEnclosingInterval()*方法获取到**AST leaf Node到root Node的路径**。
```
// identifier checks a single position for a potential identifier.
func identifier(ctx context.Context, v View, f File, pos token.Pos) (*IdentifierInfo, error) {
	fAST := f.GetAST()
	pkg := f.GetPackage()
	path, _ := astutil.PathEnclosingInterval(fAST, pos, pos)
	result := &IdentifierInfo{
		File: f,
	}
	if path == nil {
		return nil, fmt.Errorf("can't find node enclosing position")
	}
	// ... 
}
```
*astutil.PathEnclosingInterval()*的实现很长，这里只贴出该函数的非常漂亮的注释。
```
// PathEnclosingInterval returns the node that encloses the source
// interval [start, end), and all its ancestors up to the AST root.
//
// The definition of "enclosing" used by this function considers
// additional whitespace abutting a node to be enclosed by it.
// In this example:
//
//              z := x + y // add them
//                   <-A->
//                  <----B----->
//
// the ast.BinaryExpr(+) node is considered to enclose interval B
// even though its [Pos()..End()) is actually only interval A.
// This behaviour makes user interfaces more tolerant of imperfect
// input.
//
// This function treats tokens as nodes, though they are not included
// in the result. e.g. PathEnclosingInterval("+") returns the
// enclosing ast.BinaryExpr("x + y").
//
// If start==end, the 1-char interval following start is used instead.
//
// The 'exact' result is true if the interval contains only path[0]
// and perhaps some adjacent whitespace.  It is false if the interval
// overlaps multiple children of path[0], or if it contains only
// interior whitespace of path[0].
// In this example:
//
//              z := x + y // add them
//                <--C-->     <---E-->
//                  ^
//                  D
//
// intervals C, D and E are inexact.  C is contained by the
// z-assignment statement, because it spans three of its children (:=,
// x, +).  So too is the 1-char interval D, because it contains only
// interior whitespace of the assignment.  E is considered interior
// whitespace of the BlockStmt containing the assignment.
//
// Precondition: [start, end) both lie within the same file as root.
// TODO(adonovan): return (nil, false) in this case and remove precond.
// Requires FileSet; see loader.tokenFileContainsPos.
//
// Postcondition: path is never nil; it always contains at least 'root'.
//
func PathEnclosingInterval(root *ast.File, start, end token.Pos) (path []ast.Node, exact bool)
```
而余下的工作就是通过AST node信息进行综合分析，然后填充得到IdentifierInfo。这是一个很固定的套路，就是parse源文件，通过*PathEnclosingInterval()*得到AST Node path，**然后回溯node path收集信息**，最后得出判断，例如我们要判断一个identifier的SymbolKind。
*note: 这一块儿，我还没有搞的特别清楚，所以暂时留白*
### buildDefinition
*buildDefinition()*主要是*IdentifierInfo*的*Hover()*方法得到hover信息，然后再根据Location得到Definition对象。关于IdentifierInfo的Hover方法，我暂时还不是很清楚。
关于Definition的定义如下：
```
// A definition is the result of a 'definition' query.
type Definition struct {
	Location Location `json:"location"` // location of the definition
	Description string `json:"description"` // description of the denoted object
}
```
### buildGuruDefinition
*注：由于对Guru暂时不清楚，留坑待填*
# 调试server
原本我曾尝试使用`gopls`自带的vscode插件作为client，但是出现下图中的错误，所以最后give up。最终我选择了[vim-lsp](https://github.com/prabirshrestha/vim-lsp)作为client，最终实现了go-langserver的调试。
![vscode client error](https://img-blog.csdnimg.cn/20190223141430105.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1,size_16,color_FFFFFF,t_70)
关于vim-lsp的go-langserver的配置，参照官方配置就可以，下面是我的配置。由于我对vim-script不是很熟悉，所以配置中的一些命令不是很清楚。但是可以参照文档[Vim documentation: eval](1)进行简单的理解，例如`executable`就是用来检查某个执行文件上是否存在的函数。
```
80 augroup LspGo
 81   if executable('gopls')
 82       au User lsp_setup call lsp#register_server({
 83           \ 'name': 'gopls',
 84           \ 'cmd': {server_info->['gopls', '-mode', 'stdio', '-logfile', '/Users/henrywong/vimgopls.log']},
 85           \ 'whitelist': ['go'],
 86           \ })
 87   endif
 88 augroup END
```
上面配置的意思就是如果发现了`gopls`（前提是你需要将`$GOPATH/bin`添加到`$PATH`中），则调用register_server来进行server的注册，也就是启动一个gopls的进程，然后启动时的参数为
```
['gopls', '-mode', 'stdio', '-logfile', '/Users/henrywong/vimgopls.log']
```
然后设置gopls只对`go`源码有效。`gopls`启动后如下图所示，然后就可以使用`delve` attach到进程7003上调试了。
![gopls](https://img-blog.csdnimg.cn/20190223142905960.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1,size_16,color_FFFFFF,t_70)
*注：关于go-lsp的源码剖析待更新。*
另外一种可选的调试golsp的方式是通过*vscode-go*，细节见https://github.com/golang/go/wiki/gopls。这些client的实现大都是设置language server的name，然后通过Env环境变量中找到language server tool的path，然后启动。启动后，就可以通过*delve* attach上去了。
但是这种调试方式都有个问题，就是无法调试最初始的状态，等到你attach上去的时候，initialize的过程就已经完成了。
[1] [http://vimdoc.sourceforge.net/htmldoc/eval.html#executable()](http://vimdoc.sourceforge.net/htmldoc/eval.html#executable())
