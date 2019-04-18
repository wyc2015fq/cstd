# ReactiveX 学习笔记（21）使用 Rx.NET + ReactiveUI 进行 GUI 编程 - zwvista - 博客园

## [ReactiveX 学习笔记（21）使用 Rx.NET + ReactiveUI 进行 GUI 编程](https://www.cnblogs.com/zwvista/p/9560373.html)

### 课题
- 程序界面由3个文本编辑框和1个文本标签组成。
- 要求文本标签实时显示3个文本编辑框所输入的数字之和。
- 文本编辑框输入的不是合法数字时，将其值视为0。
- 3个文本编辑框的初值分别为1，2，3。

### 创建工程

打开 Visual Studio，File / New / Project...

新建一个名为 RxExample 的 WPF 应用程序。

### ReactiveUI

打开 NuGet 包管理器，搜索并安装 ReactiveUI 这个包。

### AppViewModel

在工程中添加 AppViewModel 类，内容如下

```
using ReactiveUI;
using System;

namespace RxExample
{
    public class AppViewModel : ReactiveObject
    {
        private string _number1;
        public string Number1
        {
            get { return _number1; }
            set { this.RaiseAndSetIfChanged(ref _number1, value); }
        }

        private string _number2;
        public string Number2
        {
            get { return _number2; }
            set { this.RaiseAndSetIfChanged(ref _number2, value); }
        }

        private string _number3;
        public string Number3
        {
            get { return _number3; }
            set { this.RaiseAndSetIfChanged(ref _number3, value); }
        }

        private readonly ObservableAsPropertyHelper<string> _result;
        public string Result => _result.Value;

        public AppViewModel()
        {
            _number1 = "1"; _number2 = "2"; _number3 = "3";

            int f(string s) =>
                int.TryParse(s, out var o) ? o : 0;

            _result = this.WhenAnyValue(x => x.Number1, x => x.Number2, x => x.Number3,
                (s1, s2, s3) => (f(s1) + f(s2) + f(s3)).ToString())
                .ToProperty(this, x => x.Result, "");
        }

    }

}
```

### MainWindow

将程序界面 MainWindow.xaml 的内容改为

```
<Window x:Class="RxExample.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        xmlns:d="http://schemas.microsoft.com/expression/blend/2008"
        xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
        xmlns:local="clr-namespace:RxExample"
        mc:Ignorable="d"
        Title="MainWindow" Height="200" Width="300">
    <Grid>
        <Grid.RowDefinitions>
            <RowDefinition Height="Auto"/>
            <RowDefinition Height="Auto"/>
            <RowDefinition Height="Auto"/>
            <RowDefinition Height="Auto"/>
        </Grid.RowDefinitions>
        <Grid.ColumnDefinitions>
            <ColumnDefinition Width="Auto"/>
            <ColumnDefinition Width="Auto"/>
        </Grid.ColumnDefinitions>
        <TextBox Grid.Column="1" Margin="10" Text="{Binding Number1, Mode=TwoWay, UpdateSourceTrigger=PropertyChanged}" Width="120" TextAlignment="Right"/>
        <TextBox Grid.Column="1" Margin="10" Grid.Row="1" Text="{Binding Number2, Mode=TwoWay, UpdateSourceTrigger=PropertyChanged}" Width="120" TextAlignment="Right"/>
        <TextBlock Margin="10" Grid.Row="2" Text="+" />
        <TextBox Grid.Column="1" Margin="10" Grid.Row="2" Text="{Binding Number3, Mode=TwoWay, UpdateSourceTrigger=PropertyChanged}" Width="120" TextAlignment="Right"/>
        <TextBlock Grid.Column="1" Margin="10" Grid.Row="3" Text="{Binding Result}" TextAlignment="Right"/>
    </Grid>
</Window>
```

打开 MainWindow.xaml.cs 文件，在 MainWindow的构造方法中添加以下代码
`DataContext = new AppViewModel();`


