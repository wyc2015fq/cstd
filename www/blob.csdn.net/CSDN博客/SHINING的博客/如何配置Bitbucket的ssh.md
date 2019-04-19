# 如何配置Bitbucket的ssh - SHINING的博客 - CSDN博客
2013年02月25日 17:23:49[snsn1984](https://me.csdn.net/snsn1984)阅读数：11538
                在bitbucket上使用https协议，经常会在提交代码的时候出错，让人很着急上火，但是用ssh就要方便很多。
下面介绍一下设置ssh的方法：
1.在终端中运行ssh-keygen。
2.然后一路enter，直接到结束。不要理会中间的输入。
3 打开用户目录下.ssh/id_rsa.pub文件，复制其内容。
4.Bitbucket上点右上角的小头像，然后选择Manage account.
5.左边目录选择SSH Keys， 然后选择add key。将刚才复制的内容粘贴进去，保存。
6. 回到工程页面，将使用的协议选择为SSH。
现在就可以使用SSH了。
