
call sd "s/ [ ]*/ /g" *.inl
call ind *.inl
call ind *.h
call ind *.c
call sd "/^[ ]*$/d" *.inl
call sd "/^[ ]*$/d" *.h
