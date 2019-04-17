# iwpriv - DoubleLi - 博客园






AuthMode             {OPEN,SHARED,WEPAUTO,WPAPSK,WPA2PSK,WPANONE}

                        ::Set Authentication Mode 

EncrypType          
{NONE,WEP,TKIP,AES}           
::Set Encryption Type



DefaultKeyID        
{1~4}                               
::Set Default Key ID



Key1                 
{5 ascii characters or 10 hex number or 
                      13 ascii characters or 26 hex numbers}
                      ::Set Key1 String                                                                                                                        


Key2                 {5 ascii characters or 10 hex number or 
                      13 ascii characters or 26 hex numbers}                                                                                                                        
                      ::Set Key2 String

Key3                 {5 ascii characters or 10 hex number or 
                      13 ascii characters or 26 hex numbers}                                                                                                                        
::Set Key3 String

Key4                 {5 ascii characters or 10 hex number or 
                      13 ascii characters or 26 hex numbers}                                                                                                                        
                        ::Set Key4 String



WPAPSK              
{8~63 ascii or 64 hex characters}  
::WPA Pre-Shared Key

Examples
===================================================================
-------------------------------------------------------------------------------------------------------
a> Config STA to link with AP which is OPEN/NONE(Authentication/Encryption)
1. iwpriv ra0 set NetworkType=Infra
2. iwpriv ra0 set AuthMode=OPEN
3. iwpriv ra0 set EncrypType=NONE
4. iwpriv ra0 set SSID="AP's SSID"

b> Config STA to link with AP which is SHARED/WEP(Authentication/Encryption)
1. iwpriv ra0 set NetworkType=Infra
2. iwpriv ra0 set AuthMode=SHARED
3. iwpriv ra0 set EncrypType=WEP
4. iwpriv ra0 set DefaultKeyID=1
5. iwpriv ra0 set Key1="AP's wep key"
6. iwpriv ra0 set SSID="AP's SSID"

c> Config STA to link with AP which is WPAPSK/TKIP(Authentication/Encryption)
1. iwpriv ra0 set NetworkType=Infra
2. iwpriv ra0 set AuthMode=WPAPSK
3. iwpriv ra0 set EncrypType=TKIP
4. iwpriv ra0 set SSID="AP's SSID"
5. iwpriv ra0 set WPAPSK="AP's wpa-preshared key"
6. iwpriv ra0 set SSID="AP's SSID"

d> Config STA to link with AP which is WPAPSK/AES(Authentication/Encryption)
1. iwpriv ra0 set NetworkType=Infra
2. iwpriv ra0 set AuthMode=WPAPSK
3. iwpriv ra0 set EncrypType=AES
4. iwpriv ra0 set SSID="AP's SSID"
5. iwpriv ra0 set WPAPSK="AP's wpa-preshared key"
6. iwpriv ra0 set SSID="AP's SSID"


e> Config STA to link with AP which is WPA2PSK/TKIP(Authentication/Encryption)
1. iwpriv ra0 set NetworkType=Infra
2. iwpriv ra0 set AuthMode=WPA2PSK
3. iwpriv ra0 set EncrypType=TKIP
4. iwpriv ra0 set SSID="AP's SSID"
5. iwpriv ra0 set WPAPSK=12345678
6. iwpriv ra0 set SSID="AP's SSID"

f> Config STA to create/link as adhoc mode, which is OPEN/NONE(Authentication/Encryption)
1. iwpriv ra0 set NetworkType=Adhoc
2. iwpriv ra0 set AuthMode=OPEN
3. iwpriv ra0 set EncrypType=NONE
4. iwpriv ra0 set SSID="Adhoc's SSID"


g>  Config STA to create/link as adhoc mode, which is WPANONE/TKIP(Authentication/Encryption)
    1. iwpriv ra0 set NetworkType=Adhoc
2. iwpriv ra0 set AuthMode=WPANONE
3. iwpriv ra0 set EncrypType=TKIP
4. iwpriv ra0 set SSID="AP's SSID"
5. iwpriv ra0 set WPAPSK=12345678
6. iwpriv ra0 set SSID="AP's SSID"

h> Get site survey 
usage: iwpriv ra0 get_site_survey









