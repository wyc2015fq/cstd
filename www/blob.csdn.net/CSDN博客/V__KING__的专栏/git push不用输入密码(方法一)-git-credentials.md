# git push不用输入密码(方法一)-git-credentials - V__KING__的专栏 - CSDN博客





2018年04月27日 18:07:51[v__king__](https://me.csdn.net/V__KING__)阅读数：2055








./install_git_credentials.sh 
**命令步骤：**

1. `touch ~/.git-credentials`

2. `echo "http://$username:$password@$localhost" >> ~/.git-credentials`

3. `git config --global credential.helper store`
**总结成脚本如下：**

```bash
#! /bin/bash
# Edit by liu.bao.long.vk@gmail.com

echo '##################################################'
echo '###该脚本可以永久的配置http和https协议的git密码###'
echo '###          power by liu.bao.long.vk@gmail.com###'
echo '##################################################'

echo '请输入remote url, 例如:"http://example.com"' 
read localhost
echo "请输入username"
read username
echo "请输入password"
read password

if test -e ~/.git-credentials
then
    echo '!!~/.git-credentials already exist!'
else
    echo 'Create ~/.git-credentials..'
    touch ~/.git-credentials
fi

echo 'Configure username and password..'
echo "http://$username:$password@$localhost" >> ~/.git-credentials
git config --global credential.helper store
echo 'Sucess!'
```




