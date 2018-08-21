@echo off
astyle --style=kr -N -k1 -p -H -W2 -j -n -a -s2  -xd -a -I -O --brackets=attach %*
