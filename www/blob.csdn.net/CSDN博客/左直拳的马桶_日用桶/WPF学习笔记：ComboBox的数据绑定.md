# WPF学习笔记：ComboBox的数据绑定 - 左直拳的马桶_日用桶 - CSDN博客
2014年06月20日 22:28:28[左直拳](https://me.csdn.net/leftfist)阅读数：18942
UI
```
<UserControl x:Class="UnitViews.UserMeUV"
             xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
             xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
             xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006" 
             xmlns:d="http://schemas.microsoft.com/expression/blend/2008" 
             mc:Ignorable="d" 
             xmlns:i="clr-namespace:System.Windows.Interactivity;assembly=System.Windows.Interactivity"
             d:DesignHeight="300" d:DesignWidth="300">
    <Grid>
        <ComboBox x:Name="cbStatus" ItemsSource="{Binding StatusList}" 
                  SelectedValuePath="Key" DisplayMemberPath="Value" SelectedItem="{Binding SelectedStatus}"
                  Height="30">
            <i:Interaction.Triggers>
                <i:EventTrigger EventName="SelectionChanged">
                    <i:InvokeCommandAction Command="{Binding SelectionChangedCmd}"
                             CommandParameter="{Binding ElementName=cbStatus}" />
                </i:EventTrigger>
            </i:Interaction.Triggers>            
        </ComboBox>
    </Grid>
</UserControl>
```
VM
```
static Dictionary<User.EStatus, string> olist = new Dictionary<User.EStatus, string> 
        { 
            {1,"有钱" }
            ,{2,"有闲" }
            ,{3,"有料" }
            ,{4,"有鬼" }
        };
        public Dictionary<User.EStatus, string> StatusList
        {
            get
            {
                return olist;
            }
        }
        KeyValuePair<User.EStatus, string> _kvp;
        public KeyValuePair<User.EStatus, string> SelectedStatus
        {
            get
            {
                return this._kvp;
            }
            set
            {
                this._kvp = value;
                this.RaisePropertyChanged("SelectedStatus");
            }
        }
        DelegateCommand<ComboBox> _SelectionChangedCmd = null;
        public DelegateCommand<ComboBox> SelectionChangedCmd
        {
            get
            {
                if (this._SelectionChangedCmd == null)
                {
                    this._SelectionChangedCmd = new DelegateCommand<ComboBox>(SelectionChanged);
                }
                return this._SelectionChangedCmd;
            }
        }
        void SelectionChanged(ComboBox cb)
        {
            SelectedStatus = (KeyValuePair<User.EStatus, string>)cb.SelectedItem;
            Status = SelectedStatus.Key;
        }
        public User.EStatus Status
        {
            set
            {
                var query = olist.Where(n => n.Key == value);
                if (query != null && query.Count() > 0)
                {
                    this.SelectedStatus = query.First();
                }
            }
        }
```
注意里面用了Dictionary<>作为数据源的类型，因此有ComboBox控件里有
`SelectedValuePath="Key" DisplayMemberPath="Value"`
参考文章：
[http://www.luacloud.com/2011/wpf-combobox-binding-data.html](http://www.luacloud.com/2011/wpf-combobox-binding-data.html)
