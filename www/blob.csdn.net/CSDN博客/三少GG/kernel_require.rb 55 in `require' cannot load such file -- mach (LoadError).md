# kernel_require.rb:55:in `require': cannot load such file -- mach (LoadError) - 三少GG - CSDN博客
2016年05月30日 11:04:20[三少GG](https://me.csdn.net/scut1135)阅读数：4929
https://github.com/Homebrew/legacy-homebrew/issues/49879
```
**sudo chown -R $(whoami):admin /usr/localcd $(brew --prefix) && git fetch origin && git reset --hard origin/master**
```
then I could
```
brew update
```
and carry on as normal.
