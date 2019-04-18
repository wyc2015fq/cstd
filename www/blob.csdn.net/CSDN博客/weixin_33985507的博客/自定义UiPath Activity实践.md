# 自定义UiPath Activity实践 - weixin_33985507的博客 - CSDN博客
2018年12月19日 05:25:58[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 为了对UiPath Activity的实现方式一探究竟，自己尝试实践编写了一个简单的Activity，取名叫SelectRandomItem。
## 1. 开发环境准备：
- [Microsoft Visual Studio](https://www.visualstudio.com/) with the .NET C# desktop development workload installed.
- [NuGet Package Explorer](https://github.com/NuGetPackageExplorer/NuGetPackageExplorer/releases).
- [UiPath](https://www.uipath.com/)
## 2. Activity概述
自定义Activity分两种，CodeActivity和NativeActivity。简单的区分就是CodeActivity只是执行一段代码，NativeActivity的效果就像内置Activities一样，它们实际上就是不同Activity的父类，实现的时候选择继承哪个类，你的Activity就是属于哪个分类。
我们这里是实现CodeActivity，NativeActivity请看[开源代码](https://github.com/UiPath/Community.Activities)的实现。
功能是把特定分隔符连接的字符串分割开，然后随机返回其中的某一个。应用在给选择框一个随机的值。因为主要是学习的目的，所以实际上并没有跟选择框有太大的关联，只是对字符做了处理而已。
自定义Activity分两步，首先通过C#语言来编写你的Activity逻辑，编译生成.dll文件，然后通过NuGet Package Explorer打包。
## 3.创建C#项目，编写实现代码
下面跟着提示一步一步创建C#项目：
- Launch Microsoft Visual Studio.
- Click 文件 > 创建 > 项目 (shortcut: Ctrl + Shift + N). The New Project window is displayed.
- Click Visual C#. The list of all dependencies using C# is displayed.
- 给你的Activity取个名字, 这里是 “SelectRandomItem”。
- 选择类库(.NET Framework) and click OK. 这样才能把项目导出为 .dll文件。
- Click 项目 > 添加引用….
- 分别搜索 `System.Activities` 和 `System.ComponentModel.Composition` 引用，并勾选。
- Click the OK button.这样就可以在代码中使用 `System.Activities` 和 `System.ComponentModel.Composition` 这两个基础组件了。
下面是已添加注释的实现代码：
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.Activities;
using System.ComponentModel;
    
namespace SelectRandomItem
{
    public class SelectRandomItem : CodeActivity
    {
        //参数类型，输入或者输出，或者两者都是
        [Category("Input")]
        //必须参数
        [RequiredArgument]
        public InArgument<String> FullText { get; set; }
        [Category("Input")]
        //参数默认值
        [DefaultValue("\r\n")]
        public InArgument<String> Separator { get; set; }
        [Category("Output")]
        public OutArgument<String> ChoiceResult { get; set; }
        /**
         * Execute是CodeActivity必须重载的方法
         * 处理逻辑根据Separator指定的分割符分割FullText
         * 然后随机返回其中一个
         * 
         **/
        protected override void Execute(CodeActivityContext context)
        {
            //所有的参数取值、赋值都是通过context
            var fullText = FullText.Get(context);
            var separator = Separator.Get(context);
            string[] items = Regex.Split(fullText, separator, RegexOptions.IgnoreCase);
            Random ran = new Random();
            var result = items[ran.Next(items.Length)];
            ChoiceResult.Set(context, result);
        }
    }
}
```
然后点击 生成 > 生成 SelectRandomItem。在输出栏找到SelectRandomItem.dll文件所在位置，准备下一步打包使用。
## 4. 使用NuGet Package Explorer打包
- 打开NuGet Package Explorer。点击Create a new package (Ctrl + N)，你会看到左右分割的两栏Package metadata和Package contents。
- 在右边Package contents栏的右键菜单单击 Add lib folder。
- 在lib文件夹上右键，点击 Add Existing File… 添加 SelectRandomItem.dll 文件。
- 点击顶部菜单栏 Edit > Edit Metadata，填写你的包信息。需要注意的是Id字段必须包含 “`Activities`”,不然UiPath会无法识别。
![打包信息](https://image-static.segmentfault.com/337/635/3376358425-5c1a360b52c18_articlex)
- 点击 File > Save ，保存你的包，文件名应该是类似这样：`ActivitiesSelectRandomItem.1.0.0.nupkg`。
至此你的Activity就创建完成了。
## 添加到UiPath和你的项目当中实际使用
- 首先确定你的UiPath本地包的目录，打开Manage Packages，点击左上方的Settings,在Default packages sources栏中查看Local标签对应的路径。把你创建的包放到这个文件夹内。
- 在Manage Packages左侧点击All Packages > Local ，你或者应该可以直接看到你的包了或者通过搜索来找到你的包。
- 点击你的包，在右侧信息栏点击install，然后Save，至此你的包已经安装到你的UiPath Studio的Activities面板中了。
![安装自定义包](https://image-static.segmentfault.com/734/307/734307825-5c1a3a327a896_articlex)
- 拖到你项目中体验一下吧：）
![实际使用图示](https://image-static.segmentfault.com/208/034/2080343521-5c1a3b0c58b15_articlex)
## 5. 代码文件下载
该Activity的源文件都发布在个人github仓库，有需要请[点击这里](https://github.com/ilinxiao/rpa_test)查看和下载。
同时该Activity在[记事本自动录入项目](https://github.com/ilinxiao/rpa_test/tree/master/NotepadControl)中使用到两次，分别是随机选择字体和随机字体大小。对比我通过Python模块实现同样的功能来看，自定义Activity的执行速度比调用Python模块要稳定要快很多。
最后，谢谢你能看完！有不完善的地方还希望与大家多交流。
