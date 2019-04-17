# gdb查看线程堆栈信息 - DoubleLi - 博客园






查看堆栈：gdb -quiet -batch -ex='thread apply all bt' -p pid
查看运行位置：gdb -quiet -batch -ex='thread apply all x $pc' -p pid









