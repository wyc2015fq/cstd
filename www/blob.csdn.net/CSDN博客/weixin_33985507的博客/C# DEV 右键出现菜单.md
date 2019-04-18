# C# DEV 右键出现菜单 - weixin_33985507的博客 - CSDN博客
2012年12月24日 15:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：20
1 向工作区拖入一个DevExpress.XtraBas.Barmanager控件，拖入一个DevExpress.XtraBas.PopupMenu控件。
2 PopupMenu控件设定Manager属性是刚才拖入的Barmanager名称，在PopupMenu控件右击设定“Customise”,设定具体菜单
![](https://images0.cnblogs.com/blog/430354/201212/24153835-bc3b90a17bea4f6297879719ff8e4ea3.jpg)
3 树右击菜单的代码
        /// <summary>
        /// 右击节点出现的菜单
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void treeDistrictInfo_MouseUp(object sender, MouseEventArgs e)
        {
            TreeList tree = sender as TreeList;
            if ((e.Button == MouseButtons.Right) && (ModifierKeys == Keys.None)
                 && (treeDistrictInfo.State == TreeListState.Regular))
            {
                Point p = new Point(Cursor.Position.X, Cursor.Position.Y);
                TreeListHitInfo hitInfo = tree.CalcHitInfo(e.Location);
                if (hitInfo.HitInfoType == HitInfoType.Cell)
                {
                    tree.SetFocusedNode(hitInfo.Node);
                    popupMenu1.ShowPopup(p);
                }
            }
         } 
