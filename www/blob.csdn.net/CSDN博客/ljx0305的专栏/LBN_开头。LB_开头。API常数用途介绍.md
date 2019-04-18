# LBN_开头。LB_开头。API常数用途介绍 - ljx0305的专栏 - CSDN博客
2010年06月30日 16:50:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：2714
HIWORD(wParam)中是具體消息。
有些消息由Windows處理，有些消息要由程序處理，這類消息包括：
LBN_DBLCLK //雙擊列表框中的某一項
LBN_ERRSPACE //列表框不能分配足夠的內存實現一個要求
LBN_KILLFOCUS //列表框失去鍵盤焦點
LBN_SELCANCEL //取消對某一項的選擇
LBN_SELCHANGE //列表框中被選中的某項要被改變
LBN_SETFOCUS //列表框得到鍵盤焦點
用SendMessage 或 SendDlgItemMessage向列表框發送的消息包括
LB_ADDFILE //用DlgDirList函數將一個文件加入目錄列表框中並返回該項索引
LB_ADDSTRING //添加字符串到列表框返回索引
LB_DELETESTRING //從列表框移除一個字符串返回列表框剩餘表項
LB_DIR //添加文件名列表，返回最後一個添加的文件名的索引
LB_FINDSTRING //返回第一個與指定字符串匹配的索引
LB_FINDSTRINGEXACT //返回具有某一指定前綴的索引
LB_GETANCHORINDEX //返回鼠標最後選中的項的索引
LB_GETCARETINDEX //返回具有矩形焦點的項的索引
LB_GETCOUNT //返回列表框中項的個數
LB_GETCURSEL //返回當前被選中項的索引
LB_GETHORIZONTALEXTENT //返回列表框的可滾動的寬度（單位：象素）
LB_GETITEMDATA //返回和給定項相關聯的32位值
LB_GETITEMHEIGHT //返回列表框中某一項的高度（單位：象素）
LB_GETITEMRECT //獲得列表項的客戶區的RECT
来源：([http://blog.sina.com.cn/s/blog_60f9d0400100dtbp.html](http://blog.sina.com.cn/s/blog_60f9d0400100dtbp.html)) - LBN_开头。LB_开头。API常数用途介绍_zagj22_新浪博客
LB_GETLOCALE //獲得區域信息，高位國家代碼，低位語言標識符
LB_GETSEL //返回列表項選中狀態
LB_GETSELCOUNT //返回被選中列表項的數目
LB_GETSELITEMS //創建一個被選中項的索引數組，返回被選中項個數
LB_GETTEXT //獲得和給定項相關聯的字符串和串長度
LB_GETTEXTLEN //返回和給定項相關聯的字符串長度（單位：字符）
LB_GETTOPINDEX //返回列表框中第一個可見項的索引
LB_INITSTORAGE //為指定項數和相關字符串分配內存
LB_INSERTSTRING //在列表框中指定位置插入字符串
LB_ITEMFROMPOINT //獲得與指定點最近的項的索引
LB_RESETCONTENT //從列表框中移除所有項
LB_SELECTSTRING //選擇與給定前綴相匹配的第一個字符串
LB_SELITEMRANGE //選擇給定範圍內的表項
LB_SETANCHORINDEX //設置鼠標最後選中的表項成指定表項
LB_SETCARETINDEX //設置矩形焦點到指定表項
LB_SETCOLUMNWIDTH //設置列的寬度（單位：象素）
LB_SETCOUNT //設置表項數目
LB_SETCURSEL //選擇被給定的列表項
LB_SETHORIZONTALEXTENT //設置列表框的滾動寬度（單位：象素）
LB_SETITEMHIEGHT //設置表項高度（單位：項素）
LB_SETLOCALE //設置區域，返回當前區域
LB_SETSEL //在多選列表框中選中一個表項
LB_SETTOPINDEX //滾動列表使指定的項能顯示出來
[http://msdn.microsoft.com/en-us/library/ms927938.aspx](http://msdn.microsoft.com/en-us/library/ms927938.aspx)
引用:http://blog.sina.com.cn/s/blog_60f9d0400100dtbp.html
