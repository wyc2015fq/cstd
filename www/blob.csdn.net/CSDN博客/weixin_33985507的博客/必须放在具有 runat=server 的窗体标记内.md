# 必须放在具有 runat=server 的窗体标记内 - weixin_33985507的博客 - CSDN博客
2012年12月14日 15:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
1,我在form之外添加了一个button,结果提示错误：“Button”的控件“PreButton”必须放在具有 runat=server 的窗体标记内。
解决办法:
   在页面中重写Page基类的VerifyRenderingInServerForm方法 
   public override void VerifyRenderingInServerForm(Control control) 
   { 
   // Confirms that an HtmlForm control is rendered for 
   }
来源：http://www.cnblogs.com/littleangel7/articles/1502694.html
