mkdir build
cd build
cmake ..
::make
set path="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE";%path%
devenv HELLO.sln /Build "Release|Win32"
