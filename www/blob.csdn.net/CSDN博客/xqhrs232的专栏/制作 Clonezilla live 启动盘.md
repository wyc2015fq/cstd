# 制作 Clonezilla live 启动盘 - xqhrs232的专栏 - CSDN博客
2018年03月20日 10:54:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：374
原文地址::[https://www.cnblogs.com/assassin/p/3518101.html](https://www.cnblogs.com/assassin/p/3518101.html)
相关文章
1、Clonezilla live 下載----[http://clonezilla.nchc.org.tw/clonezilla-live/download/](http://clonezilla.nchc.org.tw/clonezilla-live/download/)
2、Clonezilla稳定版版本库----[https://sourceforge.net/projects/clonezilla/files/clonezilla_live_stable/](https://sourceforge.net/projects/clonezilla/files/clonezilla_live_stable/)
3、UNetbootin----[http://unetbootin.github.io/](http://unetbootin.github.io/)
# [Clonezi](https://sourceforge.net/projects/clonezilla/)
### GNU/Linux Method A: Tuxboot
下載 GNU/Linux 版本使用的 [Tuxboot](http://tuxboot.nchc.org.tw/) 在您的環境 在 GNU/Linux 下,
請依 [指示](http://tuxboot.nchc.org.tw/installation-screenshots.php) 來執行 Tuxboot 並安裝再生龍在您的 USB 裝置.
# 安裝與使用
如 果使用MS Windows環境，下載tuxboot程式，點選執行後，選擇所要的套件，tuxboot就會下載，或者也可以選擇已經預先下載過的iso或zip檔 案，然後再選目的隨身碟或者隨身硬碟，做完之後的隨身碟或者隨身硬碟就可以開機了。 如果是在GNU/Linux環境，下載完tuxboot程式後，設定該程式變成可執行(可以使用指令"chmod +x ./tuxboot-linux*"，或者使用滑鼠點選該檔案，然後在"屬性"->"權限"裡，勾選"允許檔案作為程式執行")，然後啟動程式。也 可以直接在命令列中執行"sudo
 ./tuxboot-linux*" (將tuxboot-linux*取代成你下載的檔名，如"sudo ./tuxboot-linux-9")，你將會被提示要輸入密碼來變成擁有管理者權限才能繼續執行。之後就會顯示對話視窗，你可以選擇所要的套 件，tuxboot就會下載，或者也可以選擇已經預先下載過的iso或zip檔案，然後再選目的隨身碟或者隨身硬碟，做完之後的隨身碟或者隨身硬碟就可以 開機了。 在要使用的電腦插上USB隨身碟/隨身硬碟後，開機，然後在開機過程中按適當的按鍵(通常是F1, F2, F12, ESC,
 或者 backspace)就可以選用USB來當作開機的媒介。如果沒有這樣的選單選的話，可以進BIOS的設定選單，改變開機順序，選取USB為第一優先。
上述的描述乃是修改自[UNetbootin website](http://unetbootin.sourceforge.net/)。感謝UNetbootin的開發者。
# 螢幕截圖
|![](http://tuxboot.nchc.org.tw/images/Tuxboot.resized.png)|![](http://tuxboot.nchc.org.tw/images/Tuxboot-sel-dist.resized.png)|![](http://tuxboot.nchc.org.tw/images/Tuxboot-sel-version.resized.png)|
|----|----|----|
|*Tuxboot主畫面|*選擇所要的套件|*選擇版本|
|![](http://tuxboot.nchc.org.tw/images/Tuxboot-support-zip.resized.png)|![](http://tuxboot.nchc.org.tw/images/Tuxboot-saveiso.resized.png)|![](http://tuxboot.nchc.org.tw/images/Tuxboot-saveisoname.resized.png)|
|*選擇ISO或zip檔|*儲存下載的iso檔|*選擇路徑來存iso檔|
|![](http://tuxboot.nchc.org.tw/images/Tuxboot-progress.resized.png)|![](http://tuxboot.nchc.org.tw/images/Tuxboot-done.resized.png)| |
|*執行過程|*完成| |
参考网址：http://clonezilla.nchc.org.tw/clonezilla-live/liveusb.php#windows-method-a
http://clonezilla.org/downloads.php
http://tuxboot.nchc.org.tw/installation-screenshots.php

