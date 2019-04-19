# Commandline OpenVPN client on Mac OSX with macports - =朝晖= - 博客园
# [Commandline OpenVPN client on Mac OSX with macports](https://www.cnblogs.com/dhcn/p/7121390.html)
http://www.tuicool.com/articles/FjuyQj  注：文中有些内容做了修改,特别是那个配置文件，不能直接抄着用。
Most people use TunnelBrick to setup OpenVPN client connections on Mac OSX, i prefer using the command line.
To get OpenVPN up and running off the command line is a simple process. The commands below need to be run as a privileged user if your root account is not enabled use sudo to run the commands.
## Install OpenVPN
To install OpenVPN 2 from macports run:
```
sudo port install openvpn2
```
## Install TunTap
To install [TunTap](http://tuntaposx.sourceforge.net/) from macports run:
```
sudo port install tuntaposx
```
Configure it to startup at boot:
```
sudo launchctl load -w /Library/LaunchDaemons/org.macports.tuntaposx.plist
```
You need [TunTap](http://tuntaposx.sourceforge.net/) as it allows you to create virtual interfaces using the supplied kernel extensions. If you don't install TunTap you will get the error *Cannot allocate TUN/TAP dev dynamically* when you try and make a OpenVPN connection.
## Configuration
Create a directory to hold your configuration and keys.
```
mkdir /opt/local/etc/openvpn
```
Place your keys and configuration files in /opt/local/etc/openvpn/
A sample client configuration is provided below.
client
dev tun
proto tcp
remote vpn.home.topdog-software.com 1194
nobind
resolv-retry infinite
tls-client
ca /opt/local/etc/openvpn/ca.crt
cert /opt/local/etc/openvpn/client.crt
key /opt/local/etc/openvpn/client.key
ns-cert-type server
cipher BF-CBC
tls-cipher DHE-RSA-AES256-SHA
tls-remote vpn.home.topdog-software.com
tls-auth /opt/local/etc/openvpn/tls-auth.key 1
remote-cert-tls server
comp-lzo
persist-key
persist-tun
mute-replay-warnings
verb 3
mlock
## Connecting
To connect simply run:
`sudo openvpn --config /opt/local/etc/openvpn/openvpn.conf`
