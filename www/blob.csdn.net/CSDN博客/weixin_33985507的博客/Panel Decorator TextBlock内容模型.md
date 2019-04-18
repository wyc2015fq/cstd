# Panel Decorator TextBlock内容模型 - weixin_33985507的博客 - CSDN博客
2012年12月04日 14:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
一、Panel内容模型
Panel内容模型指从System.Windows.Controls.Panel继承的控件，这些控件都是容器，可以在内部承载其他的控件和子容器。Panel内容模型包含的容器有： Canvas DockPanel Grid TabPanel ToolBarOverflowPanel UniformGrid StackPanel ToolBarPanel VirtualizingPanel VirtualizingStackPanel WrapPanel
对于Panel模型，其包含一个Children属性，表示其所有的子控件和子容器的集合，在XAML代码中可以省略<XXX.Children>标记 如
```
<StackPanel Height="184" HorizontalAlignment="Left" Margin="10,10,0,0" Name="stackPanel1" VerticalAlignment="Top" Width="256">
            <StackPanel.Children>
                <TextBlock Text="ddd" Name="dd"></TextBlock>
                <Button>Button A</Button>
            </StackPanel.Children>
        </StackPanel>
```
也可以通过代码，动态添加Children中的对象
```
private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            TextBlock tblock = new TextBlock();
            tblock.Text = "代码";
          
            TextBlock time = new TextBlock();
            time.Text = DateTime.Now.ToString("hh:mm:ss");
            StackPanel panel = new StackPanel();
            panel.Children.Add(time);
            panel.Children.Add(tblock);
            button4.Content = panel;
        }
```
二、Decorator内容模型
Decorator内容模型指的是从System.Windows.Controls.Decorator类继承的控件，主要是对其中的一个子元素的边缘进行修饰。Decorator模型的主要控件包含： AdornerDecorator Border InlineUIContainer BulletDecorator ButtonChrome ClassicBorderDecorator InkPresenter ListBoxChrome SystemDropShadowChrome Viewbox Decorator模型包含一个Child属性，表示其包含的一个子元素（注意，只能是一个子元素（控件或容器，在容器中可以再添加其他的控件）），Child属性的XAML标记可以省略。
例如，对于一个TextBox添加一个边框，使用XAML语言定义：
```
<StackPanel Height="100" HorizontalAlignment="Left" Margin="10,10,0,0" Name="stackPanel1" VerticalAlignment="Top" Width="200">
            <Border BorderThickness="5" BorderBrush="DarkBlue" Margin="5">
                <Border.Child>
                    <TextBox Text="TextBox Content"/>
                </Border.Child>
            </Border>
           
        </StackPanel>
```
同样可以通过代码添加
```
private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Border border = new Border();
            border.BorderThickness = new Thickness(5);
            border.BorderBrush = new SolidColorBrush(Colors.DarkRed);
            border.Margin = new Thickness(5);
            TextBox textBox = new TextBox();
            textBox.Text = "TextBox Content Text";
            border.Child = textBox;
            stackPanel1.Children.Add(border);
        }
```
三、TextBlock模型
TextBlock模型实际上指的就是System.Windows.Controls.TextBlock类，它是一个用于显示少量流内容的轻量控件。其中包含一个InLines属性，支持 Inline 流内容元素的承载和显示。 支持的元素包括 AnchoredBlock、Bold（粗体字符串）、Hyperlink（超链接，在浏览器支持的模式下有效）、InlineUIContainer（承载其他控件的容器）、Italic（斜体字符串）、LineBreak（换行符）、Run（普通字符串）、Span（可以设置字体、颜色等的Span） 和 Underline（下划线）。
```
<Grid>
        <StackPanel Height="206" HorizontalAlignment="Left" Margin="10,10,0,0" Name="stackPanel1" VerticalAlignment="Top" Width="248">
            <TextBlock Margin="5" TextWrapping="WrapWithOverflow">
            <TextBlock.Inlines>
                <Bold>
                   <Run>BlockText 控件XAML示例</Run>
                </Bold>
                <!--换行-->
                <LineBreak/>
                <Run>TextBlock支持以下的几种流显示样式：</Run>
                <LineBreak/>
                <Bold>粗体（Bold）</Bold>
                <LineBreak/>
                <Italic>斜体（Italic）</Italic>
                <LineBreak/>
                <Underline>下划线（Underline）</Underline> 
                <LineBreak/>
                <Hyperlink NavigateUri="http://www.microsoft.com">超链接</Hyperlink>
                <LineBreak/>
                <Span Foreground="Red" FontSize="18">Span设置字体、颜色等</Span>
                <LineBreak />
                <InlineUIContainer>
                    <StackPanel Background="AntiqueWhite" Margin="5">
                        <TextBlock>Inline UI 容器</TextBlock>
                        <Button Content="按钮" Width="83" Height="25" />
                    </StackPanel>
                    
                </InlineUIContainer>
            </TextBlock.Inlines>
            </TextBlock>
        </StackPanel>
    </Grid>
```
 同样也可以使用代码添加
```
private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            TextBlock myTextBlock = new TextBlock();
            myGrid.Children.Add(myTextBlock); 
            myTextBlock.Inlines.Add("TextBlock的使用:"); 
            Italic myItalic = new Italic(new Run("(如Width Hight等)")); 
            myItalic.FontSize = 24; myItalic.Foreground = Brushes.Purple; 
            myTextBlock.Inlines.Add(myItalic);
            myTextBlock.Inlines.Add("是很奇怪的，它不是普通的像素，这个单位被称为与设备无关的单位"); 
            myTextBlock.TextWrapping = TextWrapping.WrapWithOverflow;
            Bold myBold = new Bold(new Italic(new Run("（Device-independent unit）"))); 
            myTextBlock.Inlines.Add(myBold);
            myTextBlock.HorizontalAlignment = HorizontalAlignment.Stretch; 
            myTextBlock.Inlines.Add(new LineBreak());
            Bold myBold1 = new Bold(new Run("粗体"));
            myTextBlock.Inlines.Add(myBold1); 
            myTextBlock.Inlines.Add(new LineBreak());
            Underline myUnderline = new Underline(new Run("下划线")); 
            myTextBlock.Inlines.Add(myUnderline); 
            myTextBlock.Inlines.Add(new LineBreak()); 
            Hyperlink myHyperlink = new Hyperlink(new Run("百度"));
            myHyperlink.NavigateUri = new Uri("http://www.baidu.com");
            myTextBlock.Inlines.Add(myHyperlink); 
            myTextBlock.Inlines.Add(new LineBreak());
            Span mySpan = new Span(new Run("Span设置字体、颜色等")); 
            mySpan.Foreground = Brushes.Red;//或者改为：mySpan.Foreground=new SolidColorBrush(Colors.Red);
            myTextBlock.Inlines.Add(mySpan); 
            myTextBlock.Inlines.Add(new LineBreak()); 
            InlineUIContainer myInlineUIContainer = new InlineUIContainer(); 
            myTextBlock.Inlines.Add(myInlineUIContainer); 
            StackPanel myStackPanel=new StackPanel();
            myInlineUIContainer.Child = myStackPanel; 
            Button myButton = new Button(); myButton.Content = "lucky";
            myStackPanel.Children.Add(myButton);
        }
```
