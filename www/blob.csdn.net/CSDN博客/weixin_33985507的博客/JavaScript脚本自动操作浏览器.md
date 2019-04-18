# JavaScript脚本自动操作浏览器 - weixin_33985507的博客 - CSDN博客
2018年05月28日 17:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
利用js模拟人为操作可以做很多机械化的操作,例如批量删除微博
```
'use strict';
var s = document.createElement('script');
s.setAttribute(
  'src',
  'https://lib.sinaapp.com/js/jquery/2.0.3/jquery-2.0.3.min.js'
);
s.onload = function() {
  setInterval(function() {
    if (!$('a[action-type="feed_list_delete"]')) {
      $('a.next').click();
    } else {
      $('a[action-type="feed_list_delete"]')[0].click();
      $('a[action-type="ok"]')[0].click();
    }
    // scroll bottom let auto load
    $('html, body').animate({ scrollTop: $(document).height() }, 'slow');
  }, 800);
};
document.head.appendChild(s);
```
有这方面需求的可以简单的学习js然后对就可以做很多很酷的事了...
