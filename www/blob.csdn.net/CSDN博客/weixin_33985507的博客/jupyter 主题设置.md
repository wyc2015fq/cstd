# jupyter 主题设置 - weixin_33985507的博客 - CSDN博客
2017年08月18日 15:15:30[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
![clipboard.png](https://image-static.segmentfault.com/284/882/2848823896-5996535eb5d67_articlex)
## 安装
`pip install jupyterthemes`
## 命令行
```
jt  [-h] [-l] [-t THEME] [-f MONOFONT] [-fs MONOSIZE] [-nf NBFONT]
    [-nfs NBFONTSIZE] [-tf TCFONT] [-tfs TCFONTSIZE] [-dfs DFFONTSIZE]
    [-m MARGINS] [-cursw CURSORWIDTH] [-cursc CURSORCOLOR] [-vim]
    [-cellw CELLWIDTH] [-lineh LINEHEIGHT] [-altp] [-P] [-T] [-N]
    [-r] [-dfonts]
```
- 
`jt -h` -- 获取帮助
- 
`jt -l` -- 列出可用主题
```
Available Themes:
   chesterish
   grade3
   monokai
   oceans16
   onedork
   solarizedd
   solarizedl
```
- 
`jt -t onedork` -- 设置指定主题
- 
`jt -r` -- 重置默认主题
参考：[jupyter-themes](https://github.com/dunovank/jupyter-themes)
## 使用
设置好主题和其他参数后，仍运行
`jupytr notebook`
中间更换主题，直接刷新 `notebook` 页面即可。
