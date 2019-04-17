# svn Can't revert without reverting children 解决方案 - DoubleLi - 博客园






EMZ3.0 qrh$ svn commit -m ""
svn: E155010: Commit failed (details follow):
svn: E155010: '/Users/qrh/Desktop/work/svn/emz/iOS/trunk/EMZ3.0/TravelGuide/zxing-master' is scheduled for addition, but is missing
jokey:EMZ3.0 qrh$ svn revert /Users/qrh/Desktop/work/svn/emz/iOS/trunk/EMZ3.0/TravelGuide/zxing-master
svn: E155038: Try 'svn revert --depth infinity' instead?
svn: E155038: Can't revert '/Users/qrh/Desktop/work/svn/emz/iOS/trunk/EMZ3.0/TravelGuide/zxing-master' without reverting children
jokey:EMZ3.0 qrh$ svn rm --keep-local /Users/qrh/Desktop/work/svn/emz/iOS/trunk/EMZ3.0/TravelGuide/zxing-master
D         TravelGuide/zxing-master
D         TravelGuide/zxing-master/cpp
D         TravelGuide/zxing-master/cpp/.gitignore
D         TravelGuide/zxing-master/cpp/.svnignore
D         TravelGuide/zxing-master/cpp/.valgrind.supp
D         TravelGuide/zxing-master/cpp/.valgrindrc
D         TravelGuide/zxing-master/cpp/README
D         TravelGuide/zxing-master/cpp/SConscript









