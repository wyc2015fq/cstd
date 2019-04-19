# ERROR: The Python ssl extension was not compiled. Missing the OpenSSL lib? - =朝晖= - 博客园
# [ERROR: The Python ssl extension was not compiled. Missing the OpenSSL lib?](https://www.cnblogs.com/dhcn/p/7634685.html)
       官方已经给出解决方案：https://github.com/pyenv/pyenv/wiki/Common-build-problems#error-the-python-ssl-extension-was-not-compiled-missing-the-openssl-lib
       在Mac上且用homebrew装了openssl的情况下，命令行如下：
```
CFLAGS="-I$(brew --prefix openssl)/include" \
LDFLAGS="-L$(brew --prefix openssl)/lib" \
pyenv install -v 3.6.2
```
       再补充一句，pyenv前面不要加sudo。否则编译仍旧报错通不过。

