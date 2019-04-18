# AJAX 利用 XHR2 Progress Event 實作下載進度列 - z69183787的专栏 - CSDN博客
2015年07月31日 20:30:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1517
#### AJAX 也有 Progress Bar
在 Web 開發環境下，我們常會利用 AJAX 技術提昇網頁反應速度與使用者體驗。這樣的作法在桌面環境的網路情況，通常不會有太大的問題，但是到了網路品質不佳的行動裝置環境，就容易造成反效果。以前的作法都是跳出「載入中...」等等之類的提示訊息，無法告訴使用者要等多久，但其實在 XHR2 (XMLHttpRequest
 Level 2) 的規範下就已經有定義了 Progress Event 事件，好讓我們在發送非同步請求時，也能即時得知資料傳送狀態。
#### 原生 JavaScript 實作
JavaScript 實作方式如下，下面的範例利用了 Progress Tag 來顯示進度列。
```
```
####  jQuery 實作 Download Progress Bar
我們最常用的 jQuery 也同樣可以實作，範例程式碼如下：
```
```
jQuery 用到了很冷門的 xhrFields 設定來替 XHR 註冊 Progress Event，我也是第一次發現有這招！瀏覽器執行結果如下：
![jquery-ajax-progress](http://blog.toright.com/wp-content/uploads/2014/03/jquery-ajax-progress.png)
#### 討論
在 Server 都沒有進行任何修改，XHR2 的 Progress Event 就可以正常工作。那麼 XHR2 是如何做到的呢？看了程式碼其實就可以發現，主要是透過 event.total 來計算，那麼資料還沒傳送完成前 event.total 怎麼來的？其實是透過 HTTP Response 中的 Content-length Header 來取得。一開始我在進行測試的時候，就是因為 WebService 沒有送出 Content-length 所以一直沒有成功，卡關的朋友們記得注意一下這個問題，沒有送出
 Content-length 的話 Progress Event 是不會被觸發的喔。
