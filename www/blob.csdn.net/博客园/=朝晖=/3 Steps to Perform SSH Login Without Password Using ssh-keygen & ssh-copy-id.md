# 3 Steps to Perform SSH Login Without Password Using ssh-keygen & ssh-copy-id - =朝晖= - 博客园
# [3 Steps to Perform SSH Login Without Password Using ssh-keygen & ssh-copy-id](https://www.cnblogs.com/dhcn/p/7523161.html)
http://www.thegeekstuff.com/2008/11/3-steps-to-perform-ssh-login-without-password-using-ssh-keygen-ssh-copy-id
You can login to a remote Linux server without entering password in 3 simple steps using ssky-keygen and ssh-copy-id as explained in this article.
``
**ssh-keygen** creates the public and private keys.** ssh-copy-id** copies the local-host’s public key to the remote-host’s authorized_keys file. ssh-copy-id also assigns proper permission to the remote-host’s home, ~/.ssh, and ~/.ssh/authorized_keys.
``
This article also explains 3 minor annoyances of using ssh-copy-id and how to use ssh-copy-id along with ssh-agent.
### Step 1: Create public and private keys using ssh-key-gen on local-host
jsmith@local-host$ [Note: You are on local-host here]
jsmith@local-host$ **ssh-keygen**Generating public/private rsa key pair.
Enter file in which to save the key (/home/jsmith/.ssh/id_rsa):[Enter key]
Enter passphrase (empty for no passphrase): [Press enter key]
Enter same passphrase again: [Pess enter key]
Your identification has been saved in /home/jsmith/.ssh/id_rsa.
Your public key has been saved in /home/jsmith/.ssh/id_rsa.pub.
The key fingerprint is:
33:b3:fe:af:95:95:18:11:31:d5:de:96:2f:f2:35:f9 jsmith@local-host
### Step 2: Copy the public key to remote-host using ssh-copy-id
jsmith@local-host$ **ssh-copy-id -i ~/.ssh/id_rsa.pub remote-host**
jsmith@remote-host's password:
Now try logging into the machine, with "ssh 'remote-host'", and check in:
.ssh/authorized_keys
to make sure we haven't added extra keys that you weren't expecting.
**Note:** ssh-copy-id **appends** the keys to the remote-host’s .ssh/authorized_key.
### Step 3: Login to remote-host without entering the password
jsmith@local-host$ **ssh remote-host**
Last login: Sun Nov 16 17:22:33 2008 from 192.168.1.2
[Note: SSH did not ask for password.]
jsmith@remote-host$ [Note: You are on remote-host here]
``
The above 3 simple steps should get the job done in most cases.
``
We also discussed earlier in detail about performing SSH and SCP from [openSSH to openSSH](http://www.thegeekstuff.com/2008/06/perform-ssh-and-scp-without-entering-password-on-openssh/) without entering password.
``
If you are using SSH2, we discussed earlier about performing SSH and SCP without password from [SSH2 to SSH2](http://www.thegeekstuff.com/2008/06/comprehensive-guide-for-ssh2-key-based-authentication-setup/) , from [OpenSSH to SSH2](http://www.thegeekstuff.com/2008/07/ssh-key-based-authentication-setup-from-openssh-to-ssh2/) and from [SSH2 to OpenSSH](http://www.thegeekstuff.com/2008/10/perform-ssh-and-scp-without-password-from-ssh2-to-openssh/).
``
### Using ssh-copy-id along with the ssh-add/ssh-agent
When no value is passed for the *option -i* and If *~/.ssh/identity.pub* is not available, *ssh-copy-id* will display the following error message.
jsmith@local-host$ ssh-copy-id -i remote-host
/usr/bin/ssh-copy-id: ERROR: No identities found
``
If you have loaded keys to the *ssh-agent* using the *ssh-add*, then *ssh-copy-id* will get the keys from the *ssh-agent* to copy to the remote-host. i.e, it copies the keys provided by ***ssh-add -L*** command to the remote-host, when you don’t pass *option -i* to the *ssh-copy-id*.
jsmith@local-host$ **ssh-agent $SHELL**
jsmith@local-host$ **ssh-add -L**
The agent has no identities.
jsmith@local-host$ **ssh-add**
Identity added: /home/jsmith/.ssh/id_rsa (/home/jsmith/.ssh/id_rsa)
jsmith@local-host$ **ssh-add -L**
ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAQEAsJIEILxftj8aSxMa3d8t6JvM79DyBV
aHrtPhTYpq7kIEMUNzApnyxsHpH1tQ/Ow== /home/jsmith/.ssh/id_rsa
jsmith@local-host$ **ssh-copy-id -i remote-host**
jsmith@remote-host's password:
Now try logging into the machine, with "ssh 'remote-host'", and check in:
.ssh/authorized_keys
to make sure we haven't added extra keys that you weren't expecting.
[Note: This has added the key displayed by ssh-add -L]
### Three Minor Annoyances of ssh-copy-id
Following are few minor annoyances of the ssh-copy-id.
``
- **Default public key:** ssh-copy-id uses ~/.ssh/identity.pub as the default public key file (i.e when no value is passed to *option -i*). Instead, I wish it uses id_dsa.pub, or id_rsa.pub, or identity.pub as default keys. i.e If any one of them exist, it should copy that to the remote-host. If two or three of them exist, it should copy identity.pub as default.
- **The agent has no identities:** When the *ssh-agent* is running and the *ssh-add -L* returns “The agent has no identities” (i.e no keys are added to the ssh-agent), the ssh-copy-id will still copy the message “The agent has no identities” to the remote-host’s authorized_keys entry.
- **Duplicate entry in authorized_keys:** I wish ssh-copy-id validates duplicate entry on the remote-host’s authorized_keys. If you execute ssh-copy-id multiple times on the local-host, it will keep appending the same key on the remote-host’s authorized_keys file without checking for duplicates. Even with duplicate entries everything works as expected. But, I would like to have my authorized_keys file clutter free.

