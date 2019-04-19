# GPU Accelerated Computing with Python - =朝晖= - 博客园
# [GPU Accelerated Computing with Python](https://www.cnblogs.com/dhcn/p/7130996.html)
https://developer.nvidia.com/how-to-cuda-[Python](http://lib.csdn.net/base/python)
[python](http://lib.csdn.net/base/python) is one of the fastest growing and most popular programming languages available. However, as an interpreted language, it has been considered too slow for high-performance computing.  That has now changed with the release of the NumbaPro Python compiler from Continuum Analytics.
CUDA Python – Using the [NumbaPro](https://docs.continuum.io/numbapro/quickstart) Python compiler, which is part of the [Anaconda Accelerate](https://developer.nvidia.com/anaconda-accelerate) package from Continuum Analytics, you get the best of both worlds: rapid iterative development and all other benefits of Python combined with the speed of a compiled language targeting both CPUs and NVIDIA GPUs.
## Getting Started
- If you are new to Python, the python.org website is an excellent source for [getting started](http://www.python.org/about/gettingstarted) material.
- Read this [blog post](http://blogs.nvidia.com/blog/2012/09/10/what-is-cuda-2/) if you are unsure what CUDA or GPU Computing is all about.
- Try CUDA by taking a self-paced lab on [nvidia.qwiklab.com](http://nvidia.qwiklab.com/). These labs only require a supported web browser and a network that allows Web Sockets. [Click here](http://websocketstest.com/) to verify that your network & system support Web Sockets in section "Web Sockets (Port 80)", all check marks should be green.
- Watch the first CUDA Python CUDACast:
- Install Anaconda Accelerate
- First install the free Anaconda package from [this location](http://www.continuum.io/downloads).
- Once Anaconda is installed, you can install a trial-version of the Accelerate package by using Anaconda’s package manager and running conda install accelerate.  See [here](https://store.continuum.io/cshop/accelerate/) for more detailed information.  Please note that the Anaconda Accelerate package is [free](https://store.continuum.io/cshop/academicanaconda) for Academic use.
## Learning CUDA
- For documentation, see the Continuum website for these various topics:
- Learn more about [libraries](http://docs.continuum.io/numbapro/cudalib.html)
- See how to use [vectorize](http://docs.continuum.io/numbapro/CUDAufunc.html) to automatically accelerate functions
- Writing CUDA [directly](http://docs.continuum.io/numbapro/CUDAJit.html) in Python code
- Browse through the following code examples:
- You can download the following [IPython Notebooks](http://ipython.org/notebook.html) and (after installing Anaconda Accelerate) execute them locally on your own system which has an NVIDIA GPU:
- [A NumbaPro Mandelbrot Example](http://nbviewer.ipython.org/f5707335f40af9463c43)
- [A Monte Carlo Option Pricer Example](http://nbviewer.ipython.org/835a8ca39ced77fe751d)
- Browse and ask questions on NVIDIA’s [DevTalk forums](https://devtalk.nvidia.com/default/board/57/cuda-programming-and-performance/), or ask at [stackoverflow.com](http://stackoverflow.com/questions/tagged/numbapro).
So, now you’re ready to deploy your application?
You can [register today](http://bit.ly/RegDevPages) to have FREE access to NVIDIA TESLA K40 GPUs.
Develop your codes on the fastest accelerator in the world. Try a Tesla K40 GPU and accelerate your development.
## Performance/Results
- It’s possible to get enormous speed-up, 20x-2000x, when moving from a pure Python application to accelerating the critical functions on the GPUs.  In many cases, with little changes required in the code.  Some simple examples demonstrating this can be found here:- A MandelBrot [example](http://nbviewer.ipython.org/f5707335f40af9463c43) accelerated with CUDA Python.  19x speed-up over the CPU-only accelerated version using GPUs and a 2000x speed-up over pure interpreted Python code.
- A Monte Carlo Option Pricer [example](http://nbviewer.ipython.org/835a8ca39ced77fe751d) accelerated with CUDA Python.  Achieved a 30x speed-up over interpreted Python code after accelerating on the GPU.
## Alternative Solution - PyCUDA
Another option for accelerating Python code on a GPU is [PyCUDA](http://mathema.tician.de/software/pycuda).  This library allows you to call the CUDA Runtime API or kernels written in CUDA C from Python and execute them on the GPU.  One use case for this is using Python as a wrapper to your CUDA C kernels for rapid development and testing.

