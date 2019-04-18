# dui UIDesigner代码问题修改与使用方法 - 深之JohnChen的专栏 - CSDN博客

2017年06月02日 10:08:56[byxdaz](https://me.csdn.net/byxdaz)阅读数：1716


一、UIDesigner代码问题

1、保存skin文件（*.xml）失败，CLayoutManager::SaveSkinFile函数中打开文件失败。

HANDLE hFile = ::CreateFile(pstrPathName, GENERIC_ALL, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

修改成

```cpp
HANDLE hFile = ::CreateFile(pstrPathName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
```

2、DUILIB UIDesigner.exe 多个控件布局的时候 总是会crash。

在使用UIDesigner.exe多个控件布局总是出错。发现代码crash在

void CMultiUITracker::ExcludeChildren(CArray<CControlUI*,CControlUI*>&?arrSelected)

函数中，修改后正常。

```cpp
void CMultiUITracker::ExcludeChildren(CArray<CControlUI*,CControlUI*>& arrSelected)
{
int size = arrSelected.GetSize();
int* pDepth = new int[size];
for(int i=0; i<size; i++)
{
ExtendedAttributes* pExtended = (ExtendedAttributes*)arrSelected[i]->GetTag();
pDepth[i] = pExtended->nDepth;
}
for(int i=0; i<arrSelected.GetSize()-1; i++)
{
CControlUI* pControl1 = arrSelected[i];
for(int j=i+1; j<arrSelected.GetSize(); j++)
{
if(pDepth[i] != pDepth[j])
{
CControlUI* pControl2 = arrSelected[j];
if (pControl2)
{
if (pDepth[i] < pDepth[j])
{
int depth = pDepth[j] - pDepth[i];
while (depth--)
{
CControlUI* tmp = pControl2->GetParent();
if (tmp)
{
pControl2 = tmp;
}
else
{
break;
}
}
if (pControl1 == pControl2)
{
arrSelected.RemoveAt(j--);
}
}
else
{
int depth = pDepth[i] - pDepth[j];
while (depth--)
{
CControlUI* tmp = pControl1->GetParent();
if (tmp)
{
pControl1 = tmp;
}
else
{
break;
}
}
if (pControl1 == pControl2)
{
arrSelected.RemoveAt(i--);
}
}
}
}
}
}
delete[] pDepth;
}
```

代码下载：[uidesigner](http://download.csdn.net/detail/byxdaz/9858256)

二、界面设计器DuiDesigner使用方法

参考这篇文章：[http://www.cnblogs.com/Alberl/p/3343838.html](http://www.cnblogs.com/Alberl/p/3343838.html)

