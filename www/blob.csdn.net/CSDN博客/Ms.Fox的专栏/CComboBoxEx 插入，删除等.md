# CComboBoxEx 插入，删除等 - Ms.Fox的专栏 - CSDN博客
2014年05月24日 10:22:55[lulu-lu](https://me.csdn.net/smbluesky)阅读数：398
个人分类：[MFC](https://blog.csdn.net/smbluesky/article/category/2211173)
    1、首先需要简单的拖动一个CComboBoxEx控件到对话框上，声明一个变量名为：CComboBoxEx m_materialExSelect;
       在资源中添加两个图标IDI_ICON1，IDI_ICON2，并声明：
       CImageList m_image; 
HICON m_icon[2];
     2、  在OnInitDialog()函数中：
        给m_icon赋值：
m_icon[0]=AfxGetApp()->LoadIcon(IDI_ICON1);
m_icon[1]=AfxGetApp()->LoadIcon(IDI_ICON2);
给m_image赋值
        m_image.Create(16,16,ILC_COLOR,2,2);
m_image.Add(m_icon[0]);
m_image.Add(m_icon[1]);
       将m_image赋给CComboBoxEx 的对象m_materialExSelect
       m_materialExSelect.SetImageList(&m_image);
3、设置方法
        COMBOBOXEXITEM ComboListItem;
       ComboListItem.iItem=i;（这个是当前的插入的位置）
       ComboListItem.iImage=i;（i可以为0，或1，因为上面我只设置了两个ICON）
       ComboListItem.iSelectedImage=i;（i可以为0，或1，因为上面我只设置了两个ICON）
       ComboListItem.iOverlay=i;（i可以为0，或1，因为上面我只设置了两个ICON）
       ComboListItem.mask=CBEIF_IMAGE|CBEIF_OVERLAY|CBEIF_SELECTEDIMAGE|CBEIF_TEXT; （模式）
CString str=namelist[i].c_str();
ComboListItem.pszText=(LPTSTR)(LPCTSTR)str;
        m_materialExSelect.InsertItem(&ComboListItem);
      插入后，就可以成功的创建有图标的CComboBoxEx 。
4、m_materialExSelect.SetCurSel(1);   设置当前的选择项为1
5、CEdit *pedit=m_materialExSelect.GetEditCtrl();
pedit->GetWindowText(str);     可获取当前的选择项的文字
        int i=m_materialExSelect.GetCurSel();     可获取当前的选择项的文字
m_materialExSelect.GetLBText(i,str);
![](http://img.bbs.csdn.net/upload/201404/21/1398083219_134879.png)
