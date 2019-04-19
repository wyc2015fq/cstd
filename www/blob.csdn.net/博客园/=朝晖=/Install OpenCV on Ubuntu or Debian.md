# Install OpenCV on Ubuntu or Debian - =朝晖= - 博客园
# [Install OpenCV on Ubuntu or Debian](https://www.cnblogs.com/dhcn/p/7124944.html)
http://milq.github.io/install-[OpenCV](http://lib.csdn.net/base/opencv)-ubuntu-debian/
转注：就用第一个方法吧，第二个方法的那个sh文件执行失败，因为我价格kurento.org的源，在安装第一个时，用到了这个源几个包。
Install [opencv](http://lib.csdn.net/base/opencv) on Ubuntu or Debian is a bit long but very easy. You can install OpenCV from the Ubuntu or Debian repository or from the official site.
**OPTION 1: INSTALL OPENCV FROM THE UBUNTU OR DEBIAN REPOSITORY**
You can install OpenCV from the Ubuntu or Debian repository:
sudo apt-get install libopencv-dev python-opencv
However, you will probably not have installed the latest version of OpenCV and you may miss some features (for example: [Python](http://lib.csdn.net/base/python) 3 bindings do not exist in the repository).
**OPTION 2: INSTALL OPENCV FROM THE OFFICIAL SITE**
To install the latest version of OpenCV be sure that you have removed the library from the repository with `sudo apt-get autoremove libopencv-dev [python](http://lib.csdn.net/base/python)-opencv`and follow the steps below.
**2.1. RUN AN INSTALLATION SCRIPT**
The most simple and elegant way to install a library is running an installation script.
Download the installation script [**install-opencv.sh**](https://github.com/milq/scripts-ubuntu-debian/blob/master/install-opencv.sh), open your terminal and execute:
bash install-opencv.sh
Type your *sudo* password and you will have installed OpenCV. This operation may take a long time due to the packages to be installed and the compilation process.
This script has been tested on Ubuntu 14.04 LTS, Ubuntu 16.04 LTS and Debian 8.0 "Jessie", with OpenCV 3.0.0 and OpenCV 3.1.0. You can freely change this script as you like or adapt it to other OS or OpenCV version.
**2.2. EXECUTE SOME OPENCV EXAMPLES**
[Go](http://lib.csdn.net/base/go) to your OpenCV directory and execute a **C++** example:
cd build/bin./cpp-example-edge ../../samples/data/fruits.jpg
Now, [go](http://lib.csdn.net/base/go) to your OpenCV directory and execute a **Python** example:
cd samples/pythonpython3 video.py
Finally, go to your OpenCV directory and execute a **[Java](http://lib.csdn.net/base/java)** example:
cd samples/java/antant -DocvJarDir=../../../build/bin -DocvLibDir=../../../build/lib
**2.3. COMPILE A DEMONSTRATION**
Download the files [**demo.cpp**](https://github.com/milq/computer-vision-resources/blob/master/code/demos/opencv/viz/demo.cpp) and [**CMakeLists.txt**](https://github.com/milq/computer-vision-resources/blob/master/code/demos/opencv/viz/CMakeLists.txt) and put them into a folder. Now, open your terminal, go to the folder and execute:
mkdir build && cd build && cmake .. && make
Finally, run the demo: `./demo`.
And that's it! You have installed OpenCV, run some examples, and compiled OpenCV code!

