# js中document.write（转） - z69183787的专栏 - CSDN博客
2013年12月18日 19:30:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：796
    document.write()方法可以用在两个方面：**1、页面载入过程中用实时脚本创建页面内容**，**2、以及用延时脚本创建本窗口或新窗口的内容**。该方法需要一个字符串参数，它是写到窗口或框架中的HTML内容。这些字符串参数可以是变量或值为字符串的表达式，写入的内容常常包括HTML标记语言。
记住，在载入页面后，浏览器输出流自动关闭。在此之后，任何一个对当前页面进行操作的document.write()方法将打开—个新的输出流，它将清除当前页面内容（包括源文档的任何变量或值）。因此，假如希望用脚本生成的HTML替换当前页面，就必须把HTML内容连接起来赋给一个变量，使用一个document.write()方法完成写操作。不必清除文档并打开一个新数据流，一个document.write()调用就可完成所有的操作。
　　关于document.write()方法还有一点要说明的是它的相关方法document.close()。脚本向窗口（不管是本窗口或其他窗口）写完内容后，必须关闭输出流。在延时脚本的最后一个document.write()方法后面，必须确保含有document.close()方法，不这样做就不能显示图像和表单。并且，任何后面调用的document.write()方法只会把内容追加到页面后，而不会清除现有内容来写入新值。为了演示document.write()方法，我们提供了同一个应用程序的两个版本。一个向包含脚本的文档中写内容，另—个向—个单独的窗口写内容。请在文本编辑器中键人每个文档，以.html文件扩展名保存，并在浏览器中打开文档。
　　示例1创建一个按钮，它为文档组合新的HTML内容，包括新文档标题的HTML标记和标记的颜色属性。示例中有一个读者所不熟悉的操作符+＝，它把其右侧的字符串加到其左侧的变量中，这个变量用来存放字符串，这个操作符能很方便地把几个单独的语句组合成—个长字符串。使用组合在newContent变量中的内容，document.write()语句可以把所有新内容写到文档中，完全清除示例1中的所有原html内容。然后需要调用document.close()语句关闭输出流。当载入该文档并单击按钮时，可以注意到浏览器标题栏中的文档标题因此而改变。当回到原始文档并再次单击该按钮时，可以看到动态写入的第二个页面的载入速度甚至比重载原始文档还要快。
示例1 在当前窗口使用document.write()。
> <html>
> <head>
<title>Writing to Same Doc</title>
<script language="JavaScript">
  function reWrite(){
    // assemble content for new window
    var newContent = "<html><head><title>A New Doc</title></head>"
    newContent += "<body bgcolor='aqua'><h1>This document is brand new.</h1>"
    newContent += "Click the Back button to see original document."
    newContent += "</body></html>"
    // write HTML to new window document
    document.write(newContent)
    document.close() // close layout stream
  }
</script>
</head>
<body>
  <form>
    <input type="button" value="Replace Content" onClick="reWrite()">
  </form>
</body>
</html>
　　示例2中，情况有点复杂，因为脚本创建了一个子窗口，整个脚本生成的文档都将写入该窗口中。为了使新窗口的引用在两个函数中保持激活状态，我们将newWindow变量声明为全局变量。页面载入时，onLoad事件处理调用makeNewWindow()函数，该函数生成一个空的子窗口。另外，我们在window.open()方法的第三个参数中加入一个属性，使子窗口的状态栏可见。
　　页面上的按钮调用subWrite()方法，它执行的第一个任务是检查子窗口的closed属性。假如关闭了引用窗口，该属性（只在较新的浏览器版本中存在）返回true。如果是这种情况（假如用户手动关闭窗口），该函数再次调用makeNewWindow()函数来重新打开那个窗口。
　　窗口打开后，新的内容作为字符串变量组合在一起。与示例1一样，一次性写入内容（虽然对单独的窗口没有必要），接下来调用close()方法。但是注意一个重要的区别：write() 和 close()方法都明显地指定了子窗口。
示例2 在另一个窗口中使用document.write()
> <html>
<head>
<title>Writing to Subwindow</title>
<script language="JavaScript">
  var newWindow
  function makeNewWindow(){
    newWindow = window.open("","","status,height=200,width=300")
  }
  function subWrite(){
    // make new window if someone has closed it
    if(newWindow.closed){
      makeNewWindow()
    }
    // bring subwindow to front
    newWindow.focus()
    // assemble content for new window
    var newContent = "<html><head><title>A New Doc</title></head>"
    newContent += "<body bgcolor='coral'><h1>This document is brand new.</h1>"
    newContent += "</body></html>"
    // write HTML to new window document
    newWindow.document.write(newContent)
    newWindow.document.close()    // close layout stream
  }
</script>
</head>
<body onLoad="makeNewWindow()">
  <form>
    <input type="button" value="Write to Subwindow" onClick="subWrite()">
  </form>
</body>
</html>
