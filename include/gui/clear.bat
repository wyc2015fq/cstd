@echo off
::*.htm *.bmp *.jpg *.t *.rar
@(for /f "delims=*" %%a in ('dir /b /s /ad *debug* *Release* *package* *.CS_') do @rd /q /s "%%a") 2>nul
@del /q /s /ah Thumbs.db 2>nul
@(del /s .xdcenv.mak .dep .dlls .executables .interfaces cscope.out .libraries *.x64P *~ *64P *470MV .source_list *.exe *.bak *.dll *.so *.o *.exp *.pcf *.orig *.positions *.ncb *.opt *.plg *.bsc *.obj *.ilk *.pdb *.vpj *.vpw *.vpwhist *.vtg *.aps *.pch *.res *.sbr *.idb *.001 *.paf2 2>nul)>nul
for /f %%a in ('dir /b /s *.dsp') do @(
  del %%~dpna.vcproj* %%~dpna.sln %%~dpna.suo
  del /ah %%~dpna.suo
) 2>nul
