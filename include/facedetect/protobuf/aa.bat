@echo off
set path="D:\code\doc\protobuf\protobuf\lib\Release";%path%
:: protoc --help
protoc --version
protoc -I=. --cpp_out=.  human_face_report.proto
protoc -I=. --cpp_out=.  universal_report.proto

goto end
protoc -I=.\ --python_out=.\  .\human_face_report.proto 
protoc -I=.\ --python_out=.\  .\universal_report.proto



:end