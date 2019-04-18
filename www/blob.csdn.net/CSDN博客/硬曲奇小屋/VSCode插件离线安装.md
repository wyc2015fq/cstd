# VSCode插件离线安装 - 硬曲奇小屋 - CSDN博客





2017年09月29日 11:32:16[HardCookies](https://me.csdn.net/james_616)阅读数：1702








现在有了新的任务要在VS 2013上写C++的程序，在Windows平台编辑器的话当然首先考虑亲VS的亲儿子VS Code。由于习惯了在Sublime Text上的快捷操作，我们可以在VS安装插件来实现。 

本来安装的话很简单，直接在插件中搜索就行，但是实验室的电脑没网啊！（-_-）所以就只能离线安装了。

1.首先在Marketplace中找到并打开所要的插件**Sublime Text Keymap**
[https://marketplace.visualstudio.com/items?itemName=ms-vscode.sublime-keybindings](https://marketplace.visualstudio.com/items?itemName=ms-vscode.sublime-keybindings)

2.下载方式
`http://${publisher}.gallery.vsassets.io/_apis/public/gallery/publisher/${publisher}/extension/${extension name}/${version}/assetbyname/Microsoft.VisualStudio.Services.VSIXPackage`
根据Sublime Text Keymap的信息，对上面的下载链接进行替换。

```
publisher       =   ms-vscode
extension name  =   sublime-keybindings
version         =   2.9.1
```

则最后下载链接为： 
[http://ms-vscode.gallery.vsassets.io/_apis/public/gallery/publisher/ms-vscode/extension/sublime-keybindings/2.9.1/assetbyname/Microsoft.VisualStudio.Services.VSIXPackage](http://ms-vscode.gallery.vsassets.io/_apis/public/gallery/publisher/ms-vscode/extension/sublime-keybindings/2.9.1/assetbyname/Microsoft.VisualStudio.Services.VSIXPackage)

3.下载后需要重命名，如sublime-keybindings.vsix（后缀一定要以.vsix结尾）。

4.安装插件 

打开VS Code，拓展，点击三个小点，“从VSIX安装”，成功后重启软件。

下载其他VS Code扩展插件的方法与上述方法相同。



