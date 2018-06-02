(for /f "tokens=1,2 delims=)( " %%a in (XUnzip.inl) do @if %%a==#define echo #undef %%b)>aaa.txt
