# WPF学习笔记——DataContext 与 ItemSource - 左直拳的马桶_日用桶 - CSDN博客
2014年05月19日 16:50:09[左直拳](https://me.csdn.net/leftfist)阅读数：8404
作为一个WPF新手，在ListBox控件里，我分不清 DataContext 与 ItemSource的区别。
在实践中，似乎：
`<ListBox x:Name="Lst" DataContext="{Binding model.UserList}" ItemsSource="{Binding}">`与
`<ListBox x:Name="Lst" ItemsSource="{Binding model.UserList}">`
似乎是等价的。
我先记录下来，看看以后是否会明白。
=======================================================================
对于一个用户控件，如果在外部赋了DataContext，那么会覆盖了控件本身的DataContext，如
`<uv:UserListUV DataContext="{Binding UserListDataContext}"/>`
会导致UserListUV控件内部的
this.DataContext = ... 无效。在这种情况下，要对控件内部的Grid或其他容器重新赋值。
this.grid.DataContext = ...
========================================================================
看来WPF这个数据源绑定真是个问题，因为没有起作用，所以今晚又调试了一晚。总结出两个要注意的地方：
1、在控件中赋数据上下文，形式为DataContext={Binding ***} 而不是 DataContext=***
2、要注意此控件的容器是否也有DataContext，是否有冲突或不兼容

