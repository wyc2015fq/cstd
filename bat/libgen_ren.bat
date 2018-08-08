@echo off
::https://libgen.pw/search?q=An+Easy+Path+to+Convex+Analysis+and+Applicantions
::https://libgen.pw/item/adv/5a7007363a04464df78f5472
::wget -O aaa.html "https://libgen.pw/item/adv/5a7007363a04464df78f5472"
::wget -O sss.html "https://libgen.pw/search?q=An+Easy+Path+to+Convex+Analysis+and+Applicantions"

for /f "tokens=1 delims={}" %%i in ('dir /b pdf\*.bin') do (
	echo %%~ni
	if not exist htm/%%~ni.html (
		wget -O htm/%%i.html "https://libgen.pw/item/adv/%%i"
	)
	if not exist txt/%%i.txt1 (
		cat htm/%%i.html|sed "1,/@book/d;/<\/textarea\>/,999d;s/:/-/g;s/ø/o/g;s/\?/./g">txt/%%i.txt
		pushd txt
		codecvt /gb %%i.txt
		popd
		cat txt/%%i.txt
	)
	if exist txt/%%i.txt (
		for /f "tokens=2 delims={}" %%a in ('finds "title =" txt/%%i.txt') do (
			for /f "tokens=2 delims={}" %%b in ('finds "author =" txt/%%i.txt') do (
				for /f "tokens=2 delims={}" %%c in ('finds "type =" txt/%%i.txt') do (
					echo {%%i},{%%a},{%%b}.%%c
					if exist "pdf/{%%i}.bin" mv "pdf/{%%i}.bin" "pdf/{%%i},{%%a},{%%b}.%%c"
					if exist "pdf/{%%i}.bin" mv "pdf/{%%i}.bin" "pdf/{%%i},{%%a}.%%c"
					if exist "pdf/{%%i}.bin" mv "pdf/{%%i}.bin" "pdf/{%%i}.%%c"
				)
			)
		)
	)
)

::curl  "https://libgen.pw/download/book/5a7007363a04464df78f5472"

:end


