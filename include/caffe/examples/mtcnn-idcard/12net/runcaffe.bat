@echo off
set PYTHONPATH=%PYTHONPATH%;D:\code\git\cstd\include\caffe\examples\mtcnn-idcard\12net
caffe-cpu train --solver=./solver.prototxt

pause
