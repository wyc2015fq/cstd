# wpa_supplicant对wep,wpa,wpa2的psk和隐藏ap的scan_ssid扫描配置 - DoubleLi - 博客园





# 请不要修改下面这一行内容，否则将不能正常工作
ctrl_interface=/var/run/wpa_supplicant

# 确保只有root用户能读取WPA的配置
ctrl_interface_group=0

# 使用wpa_supplicant来扫描和选择AP
ap_scan=1

# 简单的情形：WPA-PSk密码验证方式，PSK是ASCII密码短语，所有合法的加密方式都允许连接
network={
  ssid="simple"
  psk="very secret passphrase"
  # 优先级越高，就能越早匹配到。
  priority=5
}

# 与前面的设置相同，但要求对特定的SSID进行扫描（针对那些拒绝广播SSID的AP 也就我们常说的隐藏ap）
network={
  ssid="second ssid"
  scan_ssid=1
  psk="very secret passphrase"
  priority=2
}

# 仅使用WPA-PSK方式。允许使用任何合法的加密方式的组合
network={
  ssid="example"
  proto=WPA
  key_mgmt=WPA-PSK
  pairwise=CCMP TKIP
  group=CCMP TKIP WEP104 WEP40
  psk=06b4be19da289f475aa46a33cb793029d4ab3db7a23ee92382eb0106c72ac7bb
  priority=2
}

# 明文连接方式（不使用WPA和IEEE802.1X）
network={
  ssid="plaintext-test"
  key_mgmt=NONE
}

# 共享WEP密钥连接方式（不使用WPA和IEEE802.1X）
network={
  ssid="static-wep-test"
  key_mgmt=NONE
  # 引号包含的密钥是ASCII密钥
  wep_key0="abcde"
  # 没有引号包含的密钥是十六进制密钥
  wep_key1=0102030405
  wep_key2="1234567890123"
  wep_tx_keyidx=0
  priority=5
}

# 共享WEP密钥连接方式（无WPA和IEEE802.1X），使用共享密钥IEEE802.11验证方式
network={
  ssid="static-wep-test2"
  key_mgmt=NONE
  wep_key0="abcde"
  wep_key1=0102030405
  wep_key2="1234567890123"
  wep_tx_keyidx=0
  priority=5
  auth_alg=SHARED
}

# 在IBSS/ad-hoc网络中使用WPA-None/TKIP
network={
  ssid="test adhoc"
  mode=1
  proto=WPA
  key_mgmt=WPA-NONE
  pairwise=NONE
  group=TKIP
  psk="secret passphrase"
}








