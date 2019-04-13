
# 服务器下的多任务窗口--screen - lpty的博客 - CSDN博客

2017年07月10日 10:00:00[lpty](https://me.csdn.net/sinat_33741547)阅读数：772



## 前言
在服务器上执行一些时间较长的任务时，我们希望可以暂时离开，等待任务执行一段时间后再查看任务状况。
## 安装
`yum install screen`
## 使用介绍
创建一个新窗口
`screen -S name`暂时离开该窗口
`CTRL+A+D`查看所有窗口
`screen -ls`返回窗口
`screen -r name`退出目前窗口
`exit`窗口共享
`screen -x name`

