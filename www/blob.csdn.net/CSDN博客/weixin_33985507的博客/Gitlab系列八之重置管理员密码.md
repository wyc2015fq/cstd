# Gitlab系列八之重置管理员密码 - weixin_33985507的博客 - CSDN博客
2017年06月27日 17:31:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
gitlab web登入密码忘记以后可以用如下方式修改密码
```
[root@gitlat-test gitlab]# gitlab-rails console production
Loading production environment (Rails 4.2.6)
```
irb(main):001:0> user = User.where(id: 1).first //id为1的是超级管理员
```
irb(main):001:0> user = User.where(id: 1).first
=> #<User id: 1, email: "admin@example.com", encrypted_password: "$2a$10$3sQjUs2XgoSN63xnR4La5eYU3vHxpHD66xf./Z46Bd2...", reset_password_token: nil, reset_password_sent_at: nil, remember_created_at: nil, sign_in_count: 0, current_sign_in_at: nil, last_sign_in_at: nil, current_sign_in_ip: nil, last_sign_in_ip: nil, created_at: "2017-06-27 07:23:03", updated_at: "2017-06-27 07:28:36", name: "Administrator", admin: true, projects_limit: 10, skype: "", linkedin: "", twitter: "", authentication_token: "tnxo3F3_QCqQScbpMWjQ", theme_id: 2, bio: nil, failed_attempts: 0, locked_at: nil, username: "root", can_create_group: true, can_create_team: false, state: "active", color_scheme_id: 1, notification_level: 1, password_expires_at: nil, created_by_id: nil, last_credential_check_at: nil, avatar: nil, confirmation_token: nil, confirmed_at: "2017-06-27 07:23:04", confirmation_sent_at: nil, unconfirmed_email: nil, hide_no_ssh_key: false, website_url: "", notification_email: "admin@example.com", hide_no_password: false, password_automatically_set: false, location: nil, encrypted_otp_secret: nil, encrypted_otp_secret_iv: nil, encrypted_otp_secret_salt: nil, otp_required_for_login: false, otp_backup_codes: nil, public_email: "", dashboard: 0, project_view: 0, consumed_timestep: nil, layout: 0, hide_project_limit: false, unlock_token: nil, otp_grace_period_started_at: nil, ldap_email: false, external: false>
```
irb(main):002:0>user.password = 'yourpassword' //密码必须至少8个字符
irb(main):003:0>user.save! // 如没有问题 返回true
密码修改完成，去登陆吧!
