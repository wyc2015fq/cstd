# WPF学习笔记：获取ListBox的选中项 - 左直拳的马桶_日用桶 - CSDN博客
2014年06月19日 22:59:14[左直拳](https://me.csdn.net/leftfist)阅读数：7443
有代码有J8：
UI
```
<UserControl x:Class="UnitViews.UserListUV"
             xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
             xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
             xmlns:i="clr-namespace:System.Windows.Interactivity;assembly=System.Windows.Interactivity"
             >
    <StackPanel Orientation="Vertical">
        <ListBox x:Name="Lst" ItemsSource="{Binding Path=UserList}" BorderBrush="Transparent">
            <i:Interaction.Triggers>
                <i:EventTrigger EventName="SelectionChanged">
                    <i:InvokeCommandAction Command="{Binding SelectionChangedCmd}"
                             CommandParameter="{Binding ElementName=Lst}" />
                </i:EventTrigger>
            </i:Interaction.Triggers>
            <ListBox.ItemTemplate>
                <DataTemplate>
                    <Grid Height="30">
                        <Grid.ColumnDefinitions>
                            <ColumnDefinition Width="25"/>
                            <ColumnDefinition Width="220"/>
                            <ColumnDefinition Width="*"/>
                        </Grid.ColumnDefinitions>
                        <Image Source="{Binding Path=StrUrlPath}" Style="{StaticResource UserStatusIcon}" Grid.Column="0" />
                        <TextBlock Text="{Binding Path=StrReMarkName}" Margin="5 0" FontSize="18" Grid.Column="1"></TextBlock>
                        <Image Source="{Binding Path=CmdIcon}" Style="{StaticResource UserCmdIcon}" Grid.Column="2" />
                    </Grid>
                </DataTemplate>
            </ListBox.ItemTemplate>
        </ListBox>
    </StackPanel>
</UserControl>
```
VM:
```
DelegateCommand<ListBox> _SelectionChangedCmd = null;
public DelegateCommand<ListBox> SelectionChangedCmd
{
    get
    {
        if (this._SelectionChangedCmd == null)
        {
            this._SelectionChangedCmd = new DelegateCommand<ListBox>(SelectionChanged);
        }
        return this._SelectionChangedCmd;
    }
}
void SelectionChanged(ListBox lst)
{
    SelectedUser = lst.SelectedItem as User;
}
//这个就是选中项啦。User是自定义类
public User SelectedUser { get; set; }
```
