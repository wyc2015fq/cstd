# FFmpeg如何进行简单的音频转换 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月15日 15:54:48[boonya](https://me.csdn.net/boonya)阅读数：1088







英文原文：[https://www.howtoforge.com/tutorial/ffmpeg-audio-conversion/](https://www.howtoforge.com/tutorial/ffmpeg-audio-conversion/)


这里有一个简单的FFmpeg，它将如何涵盖框架的一部分功能。 我们将看到如何使用终端来执行各种音频文件类型（包括所有流行和广泛可用的格式）的简单转换。 在终端上使用FFmpeg，与下载和安装FFmpeg的GUI工具相比，性能更好，并且可以为高级用户提供更精确的设置。



从音频转换开始，我将使用来自Cut Chemist的现场表演的两个样本，它们是mp3和aif文件类型。 在开始之前，请确保您的系统中安装了Ffmpeg。 要做到这一点，打开一个终端，输入：


`ffmpeg –version`应该会出现类似下面屏幕截图所示的内容。
![](https://img-blog.csdn.net/20180115154440061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


如果FFmpeg缺失，您可以通过键入以下命令从相同的终端轻松安装它：


`sudo apt-get install ffmpeg`
如果您使用的是Ubuntu，或者：


`yaourt ffmpeg`如果你是一个Arch用户。 无论您选择何种分布，您都可以在您的软件包管理器中找到ffmpeg。 增加一个额外的仓库可能是必要的。



现在如果你输入：


`ffmpeg –help`在终端上，你会看到框架的力量。 您可以使用它来确定或更改音量，音频通道，采样率等等。 从基础开始，让我们看看如何将我们的文件转换成wav类型。
打开终端并进入包含输入文件的文件夹（即将要转换的文件）。 理想情况下，您可以使用文件管理器到达那里，然后右键单击空白处并在此处打开终端。 从那里你可以输入以下命令：


`ffmpeg -i filename.mp3 newfilename.wav`这是我的例子：
![](https://img-blog.csdn.net/20180115154843531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


在这种情况下，我重新命名了文件并将其转换为wav类型的音频文件。
好吧，这很容易，但我们如何确定什么是我们可以从/转换到支持的文件类型？ 这和打字一样简单


`ffmpeg -formats`
和


`ffmpeg -codecs`
并将显示所有支持的表单。

您可以使用一个输入文件来获得几个不同的输出文件，只需输入名称和前缀，如下所示：


`ffmpeg -i filename.mp3 newfilename.wav newfilename.ogg newfilename.mp4`

这将导致从一个mp3文件转换3个输出音频文件（wav，ogg，mp4）。 或者，您可以使用-c命令来设置所需的编解码器，如下所示：


`ffmpeg -i filename.mp4 c:a libopus newfilename.ogg`
现在让我们假设我们想把这里的其他文件转换成mp3，但是我们不想要任何类型的mp3。 假设我们要确定输出文件的比特率。 要做到这一点，你需要按照以下方式输入：


`ffmpeg -i filename.aif -b:a 320000 newfilename.mp3`请注意，比特率以比特/秒为单位，所以如果我想320kbit / s，我需要输入320000。
这是我的例子：
![](https://img-blog.csdn.net/20180115155343189?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这只是FFmpeg广泛的能力的介绍，我希望它可以作为您进一步发现高级框架的一种激励。
这些可能是您征服FFmpeg这个强大工具的第一步，或者是以更简单易懂的方式处理事情的另一种方式。 无论如何，你需要深入挖掘FFmpeg的神奇文档（https://www.ffmpeg.org/documentation.html），希望如果你有兴趣，你会发现很多很酷的东西。

链接：[ffmpeg](https://www.ffmpeg.org/)







