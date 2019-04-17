# 在 Windows 下讓 GVim 完全使用 Unicode - V__KING__的专栏 - CSDN博客





2014年04月30日 13:05:41[v__king__](https://me.csdn.net/V__KING__)阅读数：698








在中文版 Windows 底下，GVim 預設會使用 cp950 作為編碼方式，而相信許多人都對 cp950 感到厭惡，我也是其中之一。因此在編輯 
_vimrc 的時候第一行就是 set encoding=utf-8。然而這樣做有個副作用：導致 GVim 的選單全部變成亂碼。解決的方法其實非常簡單，卻不是看看文件就可以馬上找出這個解決方法，因此在這做個記錄。

選單變成亂碼的原因在於 GVim 會先跟據系統的語系去讀取安裝目錄底下的選單設定檔，而在中文版的 Windows 下，預設會讀取以 cp950 儲存的選單設定檔，而建立完選單後，才會讀取使用者的
_vimrc。此時若把編碼改為 utf-8，那麼之前以 cp950 讀入的選單文字當然就變成亂碼了。解決的方式是把選單刪除並且重建一次：


```
let $LANG="zh_TW.UTF-8"
set langmenu=zh_tw.utf-8
set encoding=utf8

"reload menu with UTF-8 encoding
source $VIMRUNTIME/delmenu.vim
source $VIMRUNTIME/menu.vim
```


這麼一來，GVim 就會全面使用 utf-8，選單也是正常的。



