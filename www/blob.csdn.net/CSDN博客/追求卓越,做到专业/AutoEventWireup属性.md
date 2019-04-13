
# AutoEventWireup属性 - 追求卓越,做到专业 - CSDN博客


2012年09月13日 16:55:50[Waldenz](https://me.csdn.net/enter89)阅读数：377


使用ASP.NET时，新建的aspx页面第一行page指令中包含了一个 AutoEventWireup属性，这一属性决定了当前页是否自动关联某些特殊事件。如果启用了事件自动绑定，则为true；否则为false。默认值为true。
ASP.NET 页会引发诸如Init、Load、PreRender等生命周期事件。默认情况下，可以使用Page_事件名称的命名约定将页事件绑定到方法。例如，若要为页的Load事件创建处理程序，可以创建名为Page_Load的方法。编译时，ASP.NET
 将查找基于此命名约定的方法，并自动执行事件与方法之间的绑定。可以对Page类公开的任何事件使用Page_事件名称的约定。
AutoEventWireup属性的缺点是它要求页事件处理程序具有特定的、可预测的名称。这就限制了在命名事件处理程序方面的灵活性。
如果包括页事件的显式绑定，请确保将AutoEventWireup属性设置为false，以便方法不会意外地被调用两次。

页事件的显式绑定：AutoEventWireup属性设置为false时,将不会按命名约定去调用Page_Load方法，这时需要我们显示定)
```python
public _Default()
        {
            this.Load += new EventHandler(Page_Load);
        }
        protected void Page_Load(object sender, EventArgs e)
        {
        }
```




