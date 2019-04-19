# 复制DropDownList - 左直拳的马桶_日用桶 - CSDN博客
2013年09月16日 11:57:13[左直拳](https://me.csdn.net/leftfist)阅读数：1649
```
DropDownList ddlA;
        ListItem[] ar = new ListItem[ddlB.Items.Count];
        ddlB.Items.CopyTo(ar,0);
        ddlA.DataSource = ar;
        ddlA.DataTextField = "text";
        ddlA.DataValueField = "value";
        ddlA.DataBind();
```
