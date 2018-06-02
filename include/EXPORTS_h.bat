@echo off
SET OUT="%1"
SET IN="%2"
SET NAME=NONAME
SET NAME=
cat %IN%|sed "/#define/d;/while/d;/\\$/d;s/ /\n/g"|findstr "[a-zA-Z][0-9a-zA-Z]*("|sed "s/(.*$//g"|sort|uniq>~tmp.tmp
echo.EXPORTS>%OUT%
cmd /e:on /v:on /c "(set id=1) & for /f %%a in (~tmp.tmp) do @((echo %%a @!id!  %NAME%) & @(set /a id=!id!+1>nul))">>%OUT%
del ~tmp.tmp
