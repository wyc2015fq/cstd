# 下载文件出错Chrome:ERR_RESPONSE_HEADERS_MULTIPLE_CONTENT_DISPOSITION - z69183787的专栏 - CSDN博客
2018年11月09日 16:34:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：145
使用谷歌浏览器下载文件名中带有英文半角逗号的文件时，请求已发送，但响应时发生浏览器崩溃现象，页面提示 ERR_RESPONSE_HEADERS_MULTIPLE_CONTENT_DISPOSITION ，其他浏览器无此现象，查到是谷歌浏览器特有现象，应该说是一个bug，
服务器端函数响应中若设置了 content-disposition http头，需要确保 filename 参数使用双引号包裹。
有时 filename* 参数也需要使用双引号括起。
参考讨论：http://codeverge.com/embarcadero.delphi.intraweb/chrome-err_response_headers_multi/1061174
