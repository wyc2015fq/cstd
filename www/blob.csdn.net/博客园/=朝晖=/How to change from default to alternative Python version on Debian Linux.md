# How to change from default to alternative Python version on Debian Linux - =朝晖= - 博客园
# [How to change from default to alternative Python version on Debian Linux](https://www.cnblogs.com/dhcn/articles/7453260.html)
https://linuxconfig.org/how-to-change-from-default-to-alternative-python-version-on-debian-linux
Your Debian Linux installation may include multiple python versions and thus also include multiple python binary executables. You can run the following `ls` command to find out what python binary executables are available on your system:
$ ls /usr/bin/python*
/usr/bin/python  /usr/bin/python2  /usr/bin/python2.7  /usr/bin/python3  /usr/bin/python3.4  /usr/bin/python3.4m  /usr/bin/python3m
To check what is your default python version execute:
$ python --version
Python 2.7.8
## Change python version on per user basis
To change a python version on per user basis you simply create an `alias` within user's home directory. Open `~/.bashrc` file and add new alias to change your default python executable:
alias python='/usr/bin/python3.4'
Once you make the above change, re-login or source your `.bashrc` file:
$ . ~/.bashrc
Check your default python version:
$ python --version
Python 3.4.2
## Change python version system-wide
To change python version system-wide we can use `update-alternatives` command. Logged in as a root user, first list all available python alternatives:
# update-alternatives --list python
update-alternatives: error: no alternatives for python
The above error message means that no python alternatives has been recognized by `update-alternatives`command. For this reason we need to update our alternatives table and include both `python2.7` and `python3.4`:
# update-alternatives --install /usr/bin/python python /usr/bin/python2.7 1
update-alternatives: using /usr/bin/python2.7 to provide /usr/bin/python (python) in auto mode
# update-alternatives --install /usr/bin/python python /usr/bin/python3.4 2
update-alternatives: using /usr/bin/python3.4 to provide /usr/bin/python (python) in auto mode
The `--install` option take multiple arguments from which it will be able to create a symbolic link. The last argument specified it priority means, if no manual alternative selection is made the alternative with the highest priority number will be set. In our case we have set a priority 2 for `/usr/bin/python3.4` and as a result the `/usr/bin/python3.4` was set as default python version automatically by `update-alternatives` command.
# python --version
Python 3.4.2
Next, we can again list all python alternatives:
# update-alternatives --list python
/usr/bin/python2.7
/usr/bin/python3.4
From now on, we can anytime switch between the above listed python alternative versions using below command and entering a selection number:
# update-alternatives --config python
![debian linux change alternative python version](https://linuxconfig.org/images/change-python-alternative-version-debian-linux.png)# python --version
Python 2.7.8
## Appendix
In case we no longer have the alternative python version installed on our system we can remove its `update-alternatives` listing. For example let's remove python2.7 version:
# update-alternatives --remove python /usr/bin/python2.7
update-alternatives: removing manually selected alternative - switching python to auto mode
update-alternatives: using /usr/bin/python3.4 to provide /usr/bin/python (python) in auto mode
