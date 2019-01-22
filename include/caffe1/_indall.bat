@echo off

ren proto\caffe_proto.h caffe_proto.h_
ren proto\caffe_proto.cc caffe_proto.cc_

for %%i in (inl h c hpp cpp cxx) do (
 if exist *.%%i (
  call ind.bat *.%%i
 )
)


for /f %%a in ('dir /b /ad') do (

for %%i in (inl h c hpp cpp cxx) do (
 if exist %%a/*.%%i (
  pushd %%a
  call ind.bat *.%%i
  popd
 )
)

)

ren proto\caffe_proto.h_ caffe_proto.h
ren proto\caffe_proto.cc_ caffe_proto.cc
