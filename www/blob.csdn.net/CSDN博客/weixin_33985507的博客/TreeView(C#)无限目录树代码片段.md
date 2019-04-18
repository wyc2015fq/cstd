# TreeView(C#)无限目录树代码片段 - weixin_33985507的博客 - CSDN博客
2013年07月25日 19:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
```
#region 绑定客户树
    protected void bindTreeView()
    {
        TreeView1.Nodes.Clear();
        string userid = Session["UserID"].ToString();
        string sqlwr = new SY_ADMINUSER().GetUserIDListByLoginUser(userid, "CUSTOMERSERVICE", false);
        DataTable dt = Maticsoft.DBUtility.DbHelperOra.Query("Select PARENTID,CUSTOMERID,CUSTOMERSHORT,CUSTOMERSERVICE from CU_CUSTOMER where ISDELETE='0' " + sqlwr + " order by CUSTOMERSHORT ").Tables[0];
        //定义临时树节点  
        TreeNode tmpNd;
        //特殊处理
        foreach (DataRow dr in dt.Rows)
        {
            DataRow[] drs = dt.Select("CUSTOMERID='" + dr["PARENTID"] + "'");
            if (drs.Length == 0) //无上级节点
            {
                dr["PARENTID"] = "0";
            }
        }
        dt.AcceptChanges();
        //从DataTable中得到所有父节点为null的DataRow形成的数组  
        DataRow[] rows = dt.Select("PARENTID='0'");
        //遍历根节点数组  
        foreach (DataRow row in rows)
        {
            tmpNd = new TreeNode();
            //给根节点赋值  
            tmpNd.Value = row["CUSTOMERID"].ToString();
            tmpNd.Text = row["CUSTOMERSHORT"].ToString();
            tmpNd.Expanded = true;
            //将节点加入到树中  
            TreeView1.Nodes.Add(tmpNd);
            AddTree(dt, TreeView1, tmpNd, row["CUSTOMERID"].ToString());
        }
    }
    public void AddTree(DataTable dt, TreeView tv, TreeNode pNode, string CUSTOMERID)
    {
        DataRow[] rows = dt.Select("PARENTID='" + CUSTOMERID + "'");
        TreeNode tmpNd;
        foreach (DataRow row in rows)
        {
            tmpNd = new TreeNode();
            //给根节点赋值  
            tmpNd.Value = row["CUSTOMERID"].ToString();
            tmpNd.Text = row["CUSTOMERSHORT"].ToString();
            tmpNd.Expanded = true;
            pNode.ChildNodes.Add(tmpNd);
            //将节点加入到树中  
            //TreeView1.Nodes.Add(pNode);
            AddTree(dt, TreeView1, tmpNd, row["CUSTOMERID"].ToString());
        }
    }
    #endregion
```
