
# HTML5 标签audio添加网页背景音乐代码 - 阳光岛主 - CSDN博客

2015年09月21日 09:07:11[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：100440个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)



**html5**<audio> 是 HTML 5 的新标签，定义声音，比如音乐或其他音频流。
[HTML 5 <audio> 标签](http://www.w3school.com.cn/html5/html5_audio.asp)
[HTML5 audio让音乐东山再起](http://www.iefans.net/html5-yinyue/)
[HTML5 运用aduio标签打造音乐播放器](http://www.myexception.cn/HTML-CSS/620178.html)
可以在开始标签和结束标签之间放置文本内容，这样老的浏览器就可以显示出不支持该标签的信息。
|1
|2
|3
|4
|<|audio|autoplay|=|"autoplay"|controls|=|"controls"|loop|=|"loop"|preload|=|"auto"
|src|=|"http://demo.mimvp.com/html5/take_you_fly.mp3"|>
|你的浏览器不支持audio标签
|</|audio|>
|

|属性|值|描述|
|[autoplay](http://www.w3school.com.cn/html5/att_audio_autoplay.asp)|autoplay|如果出现该属性，则音频在就绪后|马上播放|。|
|[controls](http://www.w3school.com.cn/html5/att_audio_controls.asp)|controls|如果出现该属性，则向用户|显示控件|，比如播放按钮。|
|[loop](http://www.w3school.com.cn/html5/att_audio_loop.asp)|loop|如果出现该属性，则每当音频结束时重新|循环|开始播放。|
|[preload](http://www.w3school.com.cn/html5/att_audio_preload.asp)|preload|如果出现该属性，则音频在页面加载时进行加载，并预备播放。
|如果使用 “autoplay”，则忽略该属性。
|
|[src](http://www.w3school.com.cn/html5/att_audio_src.asp)|url|要播放的音频的 URL。也可以使用<source>标签来设置音频。|
HTML5 Audio标签能够支持**wav, mp3, ogg, acc, webm**等格式，但有个很重要的音乐文件格式**midi**(扩展名mid)却在各大浏览器中都没有内置的支持([详见此处](http://www.myexception.cn/HTML-CSS/581442.html))。不是所有的浏览器都支持MP3 OGG之类的，每个浏览器因为版权的问题支持的格式都是不一样的。
**浏览器和音频兼容性**
浏览器制造商并非都同意使用某种音频文件格式。对于图像，PNG、JPEG 或 GIF 格式的文件在*任何*浏览器上都能加载到您的网页里。遗憾的是，音频文件并非如此。表 1 展示了网页中可以使用的音频文件格式，但是并非所有格式都能用于所有浏览器。例如，Chrome、Internet Explorer 9 (IE9) 和 Safari 浏览器不支持 WAV 文件，这是一种使用非压缩格式且正在衰败的标准。
**HTML5浏览器和音频格式兼容性**
|音频格式|Chrome|Firefox|IE9|Opera|Safari|
|OGG|支持|支持|支持|不支持|不支持|
|MP3|支持|不支持|支持|不支持|支持|
|WAV|不支持|支持|不支持|支持|不支持|
没有一种通用的文件格式让每个浏览器都使用单个文件格式意味着至少有 2/5 的浏览器无法播放某些声音。这不是无法在单一音频标准中达成一致的浏览器制造商不妥协的问题，而是涉及专利权和特许权使用费的法律和财务问题。不受软件专利限制的 OGG 格式旨在一劳永逸地解决这个问题。然而，在撰写本文时，Opera 和 Safari 都不支持 OGG。与 OGG 格式的文件相比，可用的 WAV 和 MP3 文件数量要更多，因此毫无疑问，浏览器制造商考虑到了这一点。MP3 作为事实的标准是个很好的解决方案。
**解决方案：使用三种文件类型和<audio>标签**
鉴于目前的状况，您可能认为目前还不是在 HTML5 页面上使用音频的黄金时刻。在某些方面可能的确如此，但是 HTML5 提供了一个解决方案，使您喜欢的浏览器能够找到一种兼容的格式。
与 <audio> 标签结合使用时，<source> 标签可以嵌套在 <audio> 容器内。假设您是一个瓦格纳迷，想在 HTML5 网页上听他的歌剧*Ride of the Valkyries（《女武神》）*。首先，您需要获得三种文件类型的音乐，即 OGG、MP3 和 WAV。将这些音乐文件与 HTML5 文件放在同一个文件夹内。然后，将每个文件名放在单独的 <source> 标签里，并且音频容器中的所有源标签都由<audio></audio> 构成，如下所示。
<audio controls>
<source src=”http://demo.mimvp.com/html5/take_you_fly.ogg” />
<source src=”http://demo.mimvp.com/html5/take_you_fly.mp3″ />
<source src=”http://demo.mimvp.com/html5/take_you_fly.wav” />
</audio>
无论访问者使用什么浏览器，它都将自动选择所读取的第一个文件类型，并为您播放声音。
**浏览器音频控件：没有两个是完全相同的**
![html5-audio-add-background-music-01](http://cdn-blog.mimvp.com/wp-content/uploads/2015/09/html5-audio-add-background-music-01.jpg)
一旦您决定要在网站上提供音频，将面临一个有趣的设计选择。每个浏览器都有与众不同的外观，看起来像是有意识地故意使其与众不同。下面的图 1 展示了这些浏览器控件的外观。
**图1：不同浏览器上的音频控件**
除了 Chrome 浏览器外，所有浏览器都有开始/暂停控件、进度条、滑块、播放秒数、音量/静音控件，还显示声音文件的总秒数。使用HTML5 标准和浏览器支持，开发人员可以相信用户将拥有与 HTML5 音频类似的体验，因为浏览器控件是类似的。您还可以使用 Flash 和 Silverlight 等插件创建控件，但是对于不同的用户，体验可能会有所不同。
某些浏览器（如 IE9）甚至有自己的声音控制条，在浏览器本身之外运行。用户打开有声音的任何网站时，他们可以从 Windows 任务栏控制声音，并能够预览当前正在播放的声音。
**html代码**(隐藏播放控件)
|1
|2
|3
|<|audio|autoplay|=|"autoplay"|>
|<|source|src|=|"http://demo.mimvp.com/html5/take_you_fly.mp3"|type|=|"audio/mpeg"|>
|</|audio|>
|

**代码演示**(隐藏播放控件)
|1
|2
|3
|<|audio|autoplay|=|"autoplay"|controls|=|"controls"|>
|<|source|src|=|"http://demo.mimvp.com/html5/take_you_fly.mp3"|type|=|"audio/mpeg"|>
|</|audio|>
|

**演示实例：**
[http://demo.mimvp.com/html5/audio.html](http://demo.mimvp.com/html5/audio.html)


