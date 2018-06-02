
call sd "s/  / /g;s/  / /g;s/  / /g;s/  / /g;/^\s*$/d" *.inl

call ind *.inl
call ind *.h
call ind *.c
call ind *.cpp

