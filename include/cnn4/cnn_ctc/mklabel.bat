@echo off
cat label.txt|sed "s/^/\"/g;s/$/\",/g">label1.txt
::cat label.txt|sed "s/^/\"/g;s/$/\"/g">label1.txt
