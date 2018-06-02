@echo off
SET OUT="%1"
echo.EXPORTS>%OUT%
::cmd /e:on /v:on /c "(set id=1) & for %%a in (%*) do @if not %%~xa==.def ((echo %%a @!id!  NONAME) & @(set /a id=!id!+1>nul))">>%OUT%
cmd /e:on /v:on /c "(set id=1) & for %%a in (%*) do @if not %%~xa==.def ((echo %%a @!id!) & @(set /a id=!id!+1>nul))">>%OUT%
