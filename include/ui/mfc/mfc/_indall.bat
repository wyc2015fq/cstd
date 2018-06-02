
call sd "s/ [ ]*/ /g" *.inl
call ind *.inl
call ind *.h
call ind *.c
sd "/^[ ]*$/d" *.inl
