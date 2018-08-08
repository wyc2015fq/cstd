@echo off
::https://libgen.pw/search?q=An+Easy+Path+to+Convex+Analysis+and+Applicantions
::https://libgen.pw/item/adv/5a7007363a04464df78f5472
::wget -O aaa.html "https://libgen.pw/item/adv/5a7007363a04464df78f5472"
::wget -O sss.html "https://libgen.pw/search?q=An+Easy+Path+to+Convex+Analysis+and+Applicantions"
::ÂíÊÏ¹ý³Ì
::https://libgen.pw/search?q=%E9%A9%AC%E6%B0%8F%E8%BF%87%E7%A8%8B
::https://libgen.pw/search?q=%E9%A9%AC%E6%B0%8F%E8%BF%87%E7%A8%8B
md htm
md txt
md pdf

::for /f %%a in ('pclip') do (	codecvt /url "%%a">q.txt )
::cat q.txt
::goto end
pclip|sed "s/ /+/g">q.txt
cat q.txt
del sss.html
for /f "tokens=1 delims=?" %%i in (q.txt) do (
	wget -O sss.html "https://libgen.pw/search?q=%%i"
)

cat sss.html |sed "s/[""]/\n/g"| finds "item/detail/id">list.txt
for /f "tokens=4 delims=/" %%i in (list.txt) do (
	echo %%i
	if not exist htm/%%i.html (
		wget -O htm/%%i.html "https://libgen.pw/item/adv/%%i"
	)
	if not exist pdf/*{%%i}*.* (
		wget -O pdf/{%%i}.bin "https://libgen.pw/download/book/%%i"
	)
	if not exist txt/%%i.txt (
		cat htm/%%i.html|sed "1,/@book/d;/<\/textarea\>/,999d;s/:/-/g">txt/%%i.txt
		cat txt/%%i.txt
	)
	if exist txt/%%i.txt (
		for /f "tokens=2 delims={}" %%a in ('finds "title =" txt/%%i.txt') do (
			for /f "tokens=2 delims={}" %%b in ('finds "author =" txt/%%i.txt') do (
				for /f "tokens=2 delims={}" %%c in ('finds "type =" txt/%%i.txt') do (
					echo {%%i},{%%a},{%%b}.%%c
					mv "pdf/{%%i}.bin" "pdf/{%%i},{%%a},{%%b}.%%c"
				)
			)
		)
	)
)

::curl  "https://libgen.pw/download/book/5a7007363a04464df78f5472"

:end


