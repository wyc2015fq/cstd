
::call sd "s/ [ ]*/ /g" *.inl
call ind2 --recursive *.inl *.h *.c
sd "/^[ ]*$/d" *.inl
