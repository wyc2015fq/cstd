# Install PyMC on Ubuntu - 专注于数据挖掘算法研究和应用 - CSDN博客





2019年03月19日 11:00:27[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：55








[PyMC](https://github.com/pymc-devs/pymc) is a nice Python package for implementing MCMC sampling in Bayesian inference. I have used this package on a variety of projects in Ubuntu 10.04lts and will provide my notes on how to install the development version from the [github](https://livesoncoffee.wordpress.com/2012/10/08/install-git-and-configure-for-github/) repository. This is important for me because I have found a variety of instances where I needed the latest PyMC version to accomplish particular goals.

第一种：

To start, make sure some dependencies are installed:
|1|`sudo``apt-get ``install``python-dev ``gcc``gfortran`|
|----|----|

If, like me, you already have many Python packages installed you will have some or most of the above installed. Next, cd to a directory where you would like to keep the PyMC git repository on your machine and clone the repository:
|1|`git clone git:``//github``.com``/pymc-devs/pymc``.git`|
|----|----|

Finally, compile and install
|123|`cd``pymc/``sudo``python setup.py config_fc --fcompiler gfortran build``sudo``python setup.py ``install`|
|----|----|



第二种：

`sudo``apt-get ``install``python-dev ``gcc``gfortran`

`pip install pymc`





