#!/usr/bin/env sh
#set -e

caffe-cpu train --solver=lenet_solver_python_layer.prototxt
pause
