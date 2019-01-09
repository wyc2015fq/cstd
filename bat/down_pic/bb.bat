@echo off
@echo off
for %%a in (%CD%) do (


if not exist htm.txt (
	del htm.txt
	for /f %%a in ('dir /b /s /ad %%~nxa') do (
		type %%a\*.*|find ".jpg">>htm.txt
	)
)

)





type htm.txt|sed "s/[""''()]/\n/g"|sed "s/http/\nhttp/g;s/jpg/jpg\n/g"|find ".jpg"|find "http://"|sort|uniq>list.txt

