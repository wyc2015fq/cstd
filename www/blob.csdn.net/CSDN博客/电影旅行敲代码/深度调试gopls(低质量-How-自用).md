# 深度调试gopls(低质量-How-自用) - 电影旅行敲代码 - CSDN博客
2019年02月23日 20:32:41[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：254
# 什么是LSP
*LSP*(Language Server Protocol)是微软在开发visual studio code中针对Language Server设计的一种协议，关于设计中的抉择，在文章《[Language Server Extension Guide](1)》有详细介绍。
下面我粘贴了这篇文章中的一些描述，采用Language Server的原因主要分为如下三点：
- 不同语言的language server在实现时，通常采用它们自身的语言，例如C++的language server Clangd就是C++实现的。而VSCode只自带了*Node.JS*的运行时。
- language server通常是资源密集型的，一言不合就要重新编译整个源码文件，所以就有将langauge server运行放在远端的需求
- 最后从 ***M*(编辑器) * *N*(language server实现)** 转化成了***M*(编辑器) + *N*(language server实现)**
*注：由于language server的是编译器的子集，简单的例如符号跳转，diagnostics等功能，编译前端就可以实现，但是另外的一些功能，例如analysis，则需要编译器偏后端的部分实现。由于编译器通常是native programming language实现的，所以language server的实现也会基于现有的编译器实现*
> 
First, Language Servers are usually implemented in their native programming languages, and that presents a challenge in integrating them with VS Code which has a Node.js runtime.
> 
Additionally, language features can be resource intensive. For example, to correctly validate a file, Language Server needs to parse a large amount of files, build up Abstract Syntax Trees for them and perform static program analysis. Those operations could incur significant CPU and memory usage and we need to ensure that VS Code’s performance remains unaffected.
> 
Finally, integrating multiple language toolings with multiple code editors could involve significant effort. From language toolings’ perspective, they need to adapt to code editors with different APIs. From code editors’ perspective, they cannot expect any uniform API from language toolings. This makes implementing language support for M languages in N code editors the work of M * N.
关于LSP囿于知识有限，这里不多做介绍。*LSP*的细节参见[Language Server Protocol Specification](2)。
# JSONRPC
由于*LSP*是基于*JSONRPC2*实现的，所以*JSONRPC*的概念是language server中非常重要的一环。由于自己不是后端工程师，对网络不是很熟，所以这里没有什么经验可谈。只是知道了**remote procedure call**这种抽象后，自己以前对什么是程序以及程序的执行理解的太肤浅了。
*注：[知乎用户用心阁的回答](3)和文章[Remote Procedure Calls](4)对我的帮助比较大。*
# 调试go official language server
我在文章《[调试gopls(tools/lsp)及部分源码剖析笔记（低质量，自用）](5)》中介绍了一种调试go official lsp的方法，就是**vim-lsp(client)** + **gopls(server shell)** + **delve attach(debugger)** 来调试go lsp的方法，这里对整个调试过程做一个简单的记录。
*注：vim-lsp的使用命令见[Supported commands][7]*
## The foundation of go language server
go language server有两个最基本的库，一个是*internal/jsonrpc2*，一个是编译器的库。当然不仅是go language server，任何language server都离不开这两个基石。
### jsonrpc2
*jsonrpc2*作为*client*和*server*之间信息传递的中间层，**因此在langugage server中处于最外层**。
### compiler libraries
go提供了非常方便的编译的库，例如*go.ast*，*go.token*，*go.parse*等，这些库对langugage server的开发人员来说是非常友善的，关于这些库的使用，可以参考[GothamGo 2016 - How to write your own Go tool?][6]。
*注：演讲者Fatih Arslan可以说是一个神童了。*
## The top-most function of language server
除了jsonrpc2之外，go langugage server的最外层是`Server` interface，*Server*的源码如下。基于LSP的规范来看，接口中的方法就是当前server所能支持的功能，未来可能的话会依次对这些函数进行介绍。
*注：go的interface基于duck typing实现*
```
type Server interface {
	Initialize(context.Context, *InitializeParams) (*InitializeResult, error)
	Initialized(context.Context, *InitializedParams) error
	Shutdown(context.Context) error
	Exit(context.Context) error
	DidChangeWorkspaceFolders(context.Context, *DidChangeWorkspaceFoldersParams) error
	DidChangeConfiguration(context.Context, *DidChangeConfigurationParams) error
	DidChangeWatchedFiles(context.Context, *DidChangeWatchedFilesParams) error
	Symbols(context.Context, *WorkspaceSymbolParams) ([]SymbolInformation, error)
	ExecuteCommand(context.Context, *ExecuteCommandParams) (interface{}, error)
	DidOpen(context.Context, *DidOpenTextDocumentParams) error
	DidChange(context.Context, *DidChangeTextDocumentParams) error
	WillSave(context.Context, *WillSaveTextDocumentParams) error
	WillSaveWaitUntil(context.Context, *WillSaveTextDocumentParams) ([]TextEdit, error)
	DidSave(context.Context, *DidSaveTextDocumentParams) error
	DidClose(context.Context, *DidCloseTextDocumentParams) error
	Completion(context.Context, *CompletionParams) (*CompletionList, error)
	CompletionResolve(context.Context, *CompletionItem) (*CompletionItem, error)
	Hover(context.Context, *TextDocumentPositionParams) (*Hover, error)
	SignatureHelp(context.Context, *TextDocumentPositionParams) (*SignatureHelp, error)
	Definition(context.Context, *TextDocumentPositionParams) ([]Location, error)
	TypeDefinition(context.Context, *TextDocumentPositionParams) ([]Location, error)
	Implementation(context.Context, *TextDocumentPositionParams) ([]Location, error)
	References(context.Context, *ReferenceParams) ([]Location, error)
	DocumentHighlight(context.Context, *TextDocumentPositionParams) ([]DocumentHighlight, error)
	DocumentSymbol(context.Context, *DocumentSymbolParams) ([]DocumentSymbol, error)
	CodeAction(context.Context, *CodeActionParams) ([]CodeAction, error)
	CodeLens(context.Context, *CodeLensParams) ([]CodeLens, error)
	CodeLensResolve(context.Context, *CodeLens) (*CodeLens, error)
	DocumentLink(context.Context, *DocumentLinkParams) ([]DocumentLink, error)
	DocumentLinkResolve(context.Context, *DocumentLink) (*DocumentLink, error)
	DocumentColor(context.Context, *DocumentColorParams) ([]ColorInformation, error)
	ColorPresentation(context.Context, *ColorPresentationParams) ([]ColorPresentation, error)
	Formatting(context.Context, *DocumentFormattingParams) ([]TextEdit, error)
	RangeFormatting(context.Context, *DocumentRangeFormattingParams) ([]TextEdit, error)
	OnTypeFormatting(context.Context, *DocumentOnTypeFormattingParams) ([]TextEdit, error)
	Rename(context.Context, *RenameParams) ([]WorkspaceEdit, error)
	FoldingRanges(context.Context, *FoldingRangeRequestParam) ([]FoldingRange, error)
}
```
而核心对*jsonrpc2*的request进行分派的函数是*serverHandler(server Server)*，源码精简后如下。*serverHandler()* 返回真正的handle请求的函数，这些请求最终都会被*Server*中的方法处理。
```
func serverHandler(server Server) jsonrpc2.Handler {
	return func(ctx context.Context, conn *jsonrpc2.Conn, r *jsonrpc2.Request) {
		switch r.Method {
		case "initialize":
			var params InitializeParams
			if err := json.Unmarshal(*r.Params, &params); err != nil {
				sendParseError(ctx, conn, r, err)
				return
			}
			resp, err := server.Initialize(ctx, &params)
			unhandledError(conn.Reply(ctx, r, resp, err))
		case "initialized":
			var params InitializedParams
			if err := json.Unmarshal(*r.Params, &params); err != nil {
				sendParseError(ctx, conn, r, err)
				return
			}
			unhandledError(server.Initialized(ctx, &params))
		// case ...
		}
	}
}
```
### “textDocument/didChange”
我以**textDocument/didChange**为例，描述一下整个过程。首先我们给出go-lsp运行的整个架构图，这里我们只关注server部分，在下图中我们可以看到server核心部分的外层是整个jsonrpc2，它是一个dead loop来不断接收vim-lsp传过来的request，然后通过一个大的switch来选择合适的handle进行处理。
![lgopls infra](https://img-blog.csdnimg.cn/20190224143903576.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1,size_16,color_FFFFFF,t_70)
*注：该图并不完善，未来会持续更新*
在文章《[调试gopls(tools/lsp)及部分源码剖析笔记（低质量，自用）](5)》中，我们介绍了query的部分，这里主要介绍*server*部分，server部分最顶层的函数是*server.go:Run()* 方法，最终都会调用下面的三个函数：
```
// RunServer starts an LSP server on the supplied stream, and waits until the 
// stream is closed.
func RunServer(ctx context.Context, stream jsonrpc2.Stream, opts ...interface{}) error {}
// RunServerOnOpts starts an LSP server on the given port and does not exit.
// This function exists for debugging purposes.
func RunServerOnPort(ctx context.Context, port int, opts ...interface{}) error {}
// RunServerOnPort starts an LSP server on the given port and does not exit.
// This function exists for debugging purposes.
func RunServerOnAddress(ctx context.Context, addr string, opts ...interface{}) error{}
```
这里我们列出RunServer这一条线上调用的函数，*server.go:Run()* -> *lsp.RunServer()* -> *protocol.RunServer()* -> *jsonrpc2.NewConn()* -> *jsonrpc.run()* ->
```
// lsp.RunServer
func RunServer(ctx context.Context, stream jsonrpc2.Stream, opts ...interface{}) error {
	s := $server{}
	conn, client := protocol.RunServer(ctx, stream, s, opts...)
	s.client = client
	return conn.Wait(ctx)
}
type clientDispatcher struct {
	*jsonrpc2.Conn
}
// protocol.RunServer
func RunServer(ctx context.Context, stream jsonrpc2.Stream, server Server, opts ...interface{}) (*jsonrpc2.Conn, Client) {
	opts = append([]interface{}{serverHandler(server), jsonrpc2.Canceler(canceller)}, opts...)
	conn := jsonrpc2.NewConn(ctx, stream, opts...)
	return conn, &clientDispatcher{Conn: conn}
}
// jsonrpc2 
// NewConn creates a new connection object that reads and writes messages from
// the supplied stream and dispatches incoming messages to the supplied handler.
func NewConn(ctx context.Context, s stream, options ...interface{}) *Conn {
	// (1) Create a conn.
	conn := &Conn{/**/}
	// (2) Handle the options.
	// (3) call `run`
	go func() {
		conn.err = conn.run(ctx)
		close(conn.done)
	}
}
// Run starts a read loop on the supplied reader.
// It must be called exactly once for each Conn.
// It returns only when the reader is closed or there is an error in the stream.
func (c *Conn) run(ctx context.Context) error {
	for {
		// get the data for a message
		data, err := c.stream.Read(ctx)
		if err != nil {
			// the stream failed, we cannot continue
			return err
		}
		// read a combined message
		msg := &combined{}
		if err := json.Unmarshal(data, msg); err != nil {
			// a badly formed message arrived, log it and continue
			// we trust the stream to have isolated the error to just this message
			// ...
			continue
		}
		// work out which kind of message we have
		switch {
		case msg.Method != "":
			// if method is set it must be a request
			request := &Request {
				Method: msg.Method,
				Params: msg.Params,
				ID: msg.ID,
			}
			if request.IsNotify() {
				c.log(Receive, request.ID, -1, requrst.Method, request.Params, nil)
				// we have a Notify, forward to the handler in a go routine
				c.handle(ctx, c, request)
			} else {
				// we have a Call, forward to the handler in another go routine
				reqCtx, cancelReq := context.WithCancel(ctx)
				c.handlingMu.Lock()
				c.handling[*request.ID] = handling{
					request: request,
					cancel:  cancelReq,
					start:   time.Now(),
				}
				c.handlingMu.Unlock()
				c.log(Receive, request.ID, -1, request.Method, request.Params, nil)
				c.handle(reqCtx, c, request)
			}
		case msg.ID != nil:
			// ...
		default:
			// ...
		}
	}
}
```
从上面列出的函数来看，进入*server*然后开始处理的核心是*jsonrpc2.run()*方法，它通过一个死循环，不断从connc中读取信息，然后*unmarshal*，最后通过调用*handler*中合适的方法进行处理。
处理的入口是*c.handle()*方法，其中这个*handle()*是在*protocol.RunServer()*中创建新connection时设置的，将*serverHandler()*，也就是前面提到的**Huge Switch**赋值给connection中的*handle()*方法，然后在Huge Switch中挑选合适的方法进行处理。
```
opts = append([]interface{}{serverHandler(server), jsonrpc2.Canceler(canceller)}, opts...)
conn := jsonrpc2.NewConn(ctx, stream, opts...)
```
### DidChange
在jsonrpc2对request进行unmarshal之后，对应的*case*如下，其中会调用*DidChange()*进行后续处理。
```
case "textDocument/didChange":
	var params DidChangeTextDocumentParams
	if err := json.Unmarshal(*r.Params, &params); err != nil {
		sendParseError(ctx, conn, e, err)
		return
	}
	unhandledError(server.DidChange(ctx, &params))
```
DidChange及其调用的源码如下：
```
func (s *server) DidChange(ctx context.Context, params *protocol.DidChangeTextDocumentParams) error {
	if len(params.ContentChanges) < 1 {
		return jsonrpc2.NewErrorf(jsonrpc2.CodeInternalError, "no content changes provided")
	}
	text, err := s.applyChanges(ctx, params)
	if err != nil {
		return err
	}
	s.cacheAndDiagnose(ctx, params.TextDocument.URI, text)
}
```
#### applyChanges()
在介绍*applyChanges()*之前，先给出*LSP*中用来表示*source change*的结构体*DidChangeTextDocumentParams*，包含两个数据成员，**文档的版本**和**文档的一组修改**（修改包括*Range*和*New Text*）。
```
// VersionedTextDocumentIdentifier is an identifier to denote a specific version of a text document.
type VersionedTextDocumentIdentifier struct {
	TextDocumentIdentifier
	/**
	 * The version number of this document. If a versioned text document identifier
	 * is sent from the server to the client and the file is not open in the editor
	 * (the server has not received an open notification before) the server can send
	 * `null` to indicate that the version is known and the content on disk is the
	 * truth (as speced with document content ownership)
	 */
	Version *uint64 `json:"version"`
}
/**
 * An event describing a change to a text document. If range and rangeLength are omitted
 * the new text is considered to be the full content of the document.
 */
type TextDocumentContentChangeEvent struct {
	/**
	 * The range of the document that changed.
	 */
	Range *Range `json:"range,omitempty"`
	/**
	 * The length of the range that got replaced.
	 */
	RangeLength float64 `json:"rangeLength,omitempty"`
	/**
	 * The new text of the range/document.
	 */
	Text string `json:"text"`
}
type DidChangeTextDocumentParams struct {
	/**
	 * The document that did change. The version number points
	 * to the version after all provided content changes have
	 * been applied.
	 */
	TextDocument VersionedTextDocumentIdentifier `json:"textDocument"`
	/**
	 * The actual content changes. The content changes describe single state changes
	 * to the document. So if there are two content changes c1 and c2 for a document
	 * in state S10 then c1 move the document to S11 and c2 to S12.
	 */
	ContentChanges []TextDocumentContentChangeEvent `json:"contentChanges"`
}
```
applyChanges主要包含两个部分：
- 检查参数的合理性
- 遍历修改，并应用修改
遍历修改，应用修改的部分代码如下所示，核心是读取文件content，然后将**content[:Range.start]** + **new text** + **content[Range.end:]** 拼接在一起，重新赋值给content，然后不断重复这个过程。最后将得到的content传递给*cacheAndDiagnose()*做处理。
*注：不知道这里先将修改做一遍去重，会不会对效率有提升。*
```
func (s *server) applyChanges(ctx context.Context, params *protocol.DidChangeTextDocumentParams) (string, error) {
	// ...
	sourceURI, err := fromProtocolURI(params.TextDocument.URI)
	file, err := s.view.GetFile(ctx, sourceURI)
	// ...
	content := file.GetContent()
	for _, change := range params.ContentChanges {
		start := bytesOffset(content, change.Range.Start)
		if start == -1 {
			return "", jsonrpc2.NewErrorf(jsonrpc2.CodeInternalError, "invalid range for content change")
		}
		end := bytesOffset(content, change.Range.End)
		if end == -1 {
			return "", jsonrpc2.NewErrorf(jsonrpc2.CodeInternalError, "invalid range for content change")
		}
		var buf bytes.Buffer
		buf.Write(content[:start])
		buf.WriteString(change.Text)
		buf.Write(content[end:])
		content = buf.Bytes()
	}
	return string(content), nil
}
```
#### cacheAndDiagnose()
当每次对文件进行了修改以后，需要更新*View*中关于该文件的信息，例如*File*对应的*ast*和*token*信息需要进行失效处理。由于文件进行了修改，需要对该文件进行重新*Diagnostics*，*cacheAndDiagnose()* 的源码如下：
```
func (s *server) cacheAndDiagnose(ctx context.Context, uri protocol.DocumentURI, content string) {
	sourceURI, err := fromProtocolURI(uri)
	if err != nil {
		return // handle error?
	}
	if err := s.setContent(ctx, sourceURI, []byte(content)); err != nil {
		return // handle error?
	}
	go func() {
		reports, err := source.Diagnostics(ctx, s.view, sourceURI)
		if err != nil {
			return // handle error?
		}
		for filename, diagnostics := range reports {
			s.client.PublishDiagnostics(ctx, &protocol.PublishDiagnosticsParams{
				URI:         protocol.DocumentURI(source.ToURI(filename)),
				Diagnostics: toProtocolDiagnostics(ctx, s.view, diagnostics),
			})
		}
	}()
}
```
处理完之后，最终又会回到*jsonrpc2*的等待新request的死循环中。
需要注意的是，存在一些其他的request，像"**textDocument/definition**"需要从*server*像*client*返回一些信息。*request*的*Reply*是通过*jsonrpc2*中的*Reply()* 方法实现的。
[1] [https://code.visualstudio.com/api/language-extensions/language-server-extension-guide](https://code.visualstudio.com/api/language-extensions/language-server-extension-guide)
[2] [https://microsoft.github.io/language-server-protocol/specification#initialize](https://microsoft.github.io/language-server-protocol/specification#initialize)
[3] [https://www.zhihu.com/question/25536695](https://www.zhihu.com/question/25536695)
[4] [https://www.cs.rutgers.edu/~pxk/417/notes/03-rpc.html](https://www.cs.rutgers.edu/~pxk/417/notes/03-rpc.html)
[5] [https://blog.csdn.net/dashuniuniu/article/details/87879061](https://blog.csdn.net/dashuniuniu/article/details/87879061)
[6] [https://www.youtube.com/watch?v=oxc8B2fjDvY&t=1s](https://www.youtube.com/watch?v=oxc8B2fjDvY&t=1s)
[7] [https://github.com/prabirshrestha/vim-lsp/blob/629a50140f2dd8435e79790b03c74254f4e112b0/README.md#supported-commands](https://github.com/prabirshrestha/vim-lsp/blob/629a50140f2dd8435e79790b03c74254f4e112b0/README.md#supported-commands)
