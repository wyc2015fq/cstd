# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之64---BREW 应用的交叉编译 - 我相信...... - CSDN博客





2010年07月18日 12:06:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1295








为了使应用在支持BREW的手机上运行，必须将应用的源代码针对ARM CPU体系结构重新编译。其中编译后可执行文件应为一个扩展名为.mod的文件，例如myapp.mod。

进行重编译的最好工具就是ARM开发工具组（ARM Developer Suite），可以是1.0.1以上的版本，为了更好的实现兼容性，选用ARM开发工具组的版本最好与手机平台的自身编译版本一致。此外，建议将ARM开发工具组保存在C:/ADS，即确保ARM开发工具组保存在中间没有空格的路径下，否则在ARM开发工具组在执行编译的时候可能会产生问题。该路径在makefiles(.mak文件)中的假设是SDK中所有示例程序保存的位置。

ARM开发工具组使用一个命令行编译器去构造.mod文件，开发者也可以通过BREW add-in在集成开发环境中执行。构造程序是有一个同应用源文件在同一目录下的makefile。可以自行创建makefile，并运行它构造一个.mod文件，也可以使用BREW Add-in集成到Visual C++的用户界面中的工具。

为了创建makefile，打开 Visual C++中的应用项目工作空间(.dsw file)。这里应该有Visual C++的工具栏，上面有三个图标，显示“生成ARM Makefile”、“执行BREW MIF编辑器”和“执行BREW MIF编辑器”。为了生成makefile, 点击显示“生成ARM Makefile”的图标。如果当鼠标接近工具栏图标时并没有出现工具提示，选择Tools/Customize，在选择Toolbars标签，然后选中 Show ToolTips（显示工具提示）。如果工具栏和三个工具提示没能出现，则选择Tools / Customize，再选择 Add-ins and Macro Files （插件和宏文件）的标签，选中ARMMakeBREWApp，关闭对话框。如果工具栏还是没能显示，选择Tools / Customize，再选择 Toolbars标签，其中在工具栏列表中应有尚未选中的一项，叫Toolbar1，选中该项，工具栏就会出现**。**

如果ARM开发工具组是1.1或以上版本的ARM开发系统，可能需要修改一下Makefile文件。以下两行是需要修改的语句：

ZA = -za1 # LDR may only access 32-bit aligned addresses

ZAS = -zas4 # Min byte alignment for structures

将上两行更改为：

ZA = -zo1 # LDR may only access 32-bit aligned addresses

ZAS = -zos4 # Min byte alignment for structures

### 交叉编译既可以从IDE中运行又可以通过命令行的方式运行。在基于Visual Studio 的集成开发环境中，在Visual C++中选择**Tools / BREW ARM Make**即可开始编译该应用。

命令行编译的方法是，打开一个命令窗口(“DOS 框”) 并键入类似以下的命令：

nmake /f myapp.mak all

**nmake是**Visual C++中的一个应用程序。执行该操作的路径应包含在PC机的系统路径变量中，以方便使用。在这个命令中，“/f ”切换确定使用那个.mak file进行输入。“all”告诉nmake来重构目标文件。当构造完成之后，一个新.mod文件就会出现在应用的目录中，该文件是应用程序在设备上使用的二进制版本。几个中间文件也会出现在目录中。如果你想清除中间文件和应用模块文件，请在Visual C++选择**Tools / BREW ARM Make Clean**，也可以在一个命令窗口通过一个如下的命令来实现：

nmake /f myapp.mak clean



