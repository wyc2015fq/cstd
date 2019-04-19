# putty上执行后台运行命令失效 - sxf_123456的博客 - CSDN博客
2017年09月05日 21:44:48[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1423
                
nohup命令失效，关闭putty后无法在后台运行
用putty连接ubuntu，要执行一个后台命令
正常用nohup直接运行命令，就可以在后台运行
nohup ./1.sh &
但关闭putty后，发现命令也停止了
问题出在，直接点putty的右上角X关闭按钮，nohup也会停止
解决：直接在putty中输入exit退出即可
            
