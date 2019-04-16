@echo off
pclip>>urllist.txt
echo.>>urllist.txt
python down_csdn.py 1000
sleep 5