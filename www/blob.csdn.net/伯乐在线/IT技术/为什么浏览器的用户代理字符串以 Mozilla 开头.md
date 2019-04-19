# 为什么浏览器的用户代理字符串以 Mozilla 开头 - 文章 - 伯乐在线
原文出处： [Solidot](https://www.solidot.org/story?sid=55694)
最早流行的浏览器是 NCSA Mosaic，它称自己为 NCSA_Mosaic/2.0 (Windows 3.1)；后来一个新浏览器出现了，它的名字叫 Mozilla，是 Mosaic Killer 的缩写，Mosaic 并不觉得这好笑，因此该浏览器改名为 Netscape，它称自己为 Mozilla/1.0 (Win3.1)。
Netscape 支持框架（frame），而 Mosaic 不支持，因此用户代理嗅探（User Agent sniffing）出现了，网站在检测到 Mozilla 后就发送框架，如果不是就不发送。后来微软开发了 IE，希望它成为 Netscape Killer，IE 也支持框架，但由于它不是 Mozilla，网站没有向它发送框架。
微软没有耐心等待网站修改而是声称它兼容于 Mozilla，因此冒充 Netscape 称自己为 Mozilla/1.22 (compatible; MSIE 2.0; Windows 95)。第一次浏览器战争以 Netscape 的失利结束，但 Netscape 以 Mozilla 的名字获得了新生。
Mozilla 构建了 Gecko，称自己为 Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.1) Gecko/20020826。Gecko 是一个渲染引擎，Mozilla 在此基础上开发了 Firefox，它称自己为 Mozilla/5.0 (Windows; U; Windows NT 5.1; sv-SE; rv:1.7.5) Gecko/20041108 Firefox/1.0。因为 Gecko 好于 IE，因此用户代理嗅探又出现了，网站在检测到 Gecko 后会提供更好的页面代码。
![YL}%@8@6764CYM (9J]%W[LU.png](http://jbcdn2.b0.upaiyun.com/2018/03/47a58f731472797655f14eeebe80e7d7.png)
Linux 上的浏览器 Konqueror 使用的渲染引擎是 KHTM，它只能冒充 Gecko 称自己为 Mozilla/5.0 (compatible; Konqueror/3.2; FreeBSD) (KHTML, like Gecko)。Opera 则提供了选项，让用户想冒充哪个浏览器就冒充哪个浏览器。苹果后来创建了 KHTML 的一个分支 WebKit，称自己为 Mozilla/5.0 (Macintosh; U; PPC Mac OS X; de-de) AppleWebKit/85.7 (KHTML, like Gecko) Safari/85.5。这就是为什么浏览器的用户代理字符串以 Mozilla 开头。
