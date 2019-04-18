# 写代码时，让 Vim 播放打字机的音效 - 知乎
# 



编辑器就是程序员的大玩具，闲着摆弄一下是件很有意思的事情，比如今天我们让 Vim 发出打字机的声音：
![](https://pic1.zhimg.com/v2-dd8e909fa5da3f03f76cffc4c67089a8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='683' height='512'></svg>)
可以配合：[typewriter](https://link.zhihu.com/?target=https%3A//github.com/logico-dev/typewriter) 的配色方案一起使用，让你真的觉得自己正在用打字机输入。总共五种不同主题的音效可以使用，增加你编程时候的浸入感和专注感，让你在充满节奏的音效中，获得内心的平静与力量。

演示视频：
![](https://pic1.zhimg.com/80/v2-c94e029fcc1a335b93b9c6b0052ebc44_b.jpg)https://www.zhihu.com/video/999404872545947648
视频里录制问题，音效会有少许延迟，实际使用不会延迟。




项目地址：[skywind3000/vim-keysound](https://link.zhihu.com/?target=https%3A//github.com/skywind3000/vim-keysound)




**安装和配置**


```
Plug 'skywind3000/vim-keysound'

" 启动 Vim 时自动启动
let g:keysound_enable = 1

" 设置默认音效主题，可以选择：default, typewriter, mario, bubble, sword
let g:keysound_theme = 'default'

" 设置 python 版本：2 或者3 默认会自动检测
let g:keysound_py_version = 2

" 设置音量：0-1000
let g:keysound_volume = 500
```





**依赖**
- Vim 支持 Python 2/3
- PySDL2（用于提供低延迟音效播放）




**安装依赖**

Ubuntu 下面使用 apt-get 安装 `python-sdl2` or `python3-sdl2` 即可，注意版本和 Vim 支持的 python 版本对应:

`apt-get install python-sdl2`

or

`apt-get install python3-sdl2`

## Windows

先使用 pip 安装 pysdl2 ，同样注意 python的版本号和 Vim 的对应:

`pip install pysdl2`

or

`pip3 install pysdl2`

下载 [SDL2.dll](https://link.zhihu.com/?target=https%3A//www.libsdl.org/download-2.0.php) 以及 [SDL2_mixer.dll](https://link.zhihu.com/?target=https%3A//www.libsdl.org/projects/SDL_mixer/) 把这两个文件放到 python 安装目录即可。可以通过命令  `python -c "import sdl2"` 来验证是否安装成功，如果没有报错，代表成功安装。






