@echo off

ren proto\caffe.pb.h caffe.pb.h_
ren proto\caffe.pb.cc caffe.pb.cc_

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

ren proto\caffe.pb.h_ caffe.pb.h
ren proto\caffe.pb.cc_ caffe.pb.cc
