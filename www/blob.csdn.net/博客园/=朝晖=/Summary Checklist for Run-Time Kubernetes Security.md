# Summary Checklist for Run-Time Kubernetes Security - =朝晖= - 博客园
# [Summary Checklist for Run-Time Kubernetes Security](https://www.cnblogs.com/dhcn/p/10641498.html)
Here is a convenient checklist summary of the security protections to review
for securing Kubernetes deployments during run-time. This list does not cover
the build phase vulnerability scanning and registry protection requirements.
PRE-PRODUCTION
❏ Use namespaces
❏ Restrict Linux capabilities
❏ Enable SELinux
❏ Utilize Seccomp
❏ Configure Cgroups
❏ Use R/O Mounts
❏ Use a minimal Host OS
❏ Update system patches
❏ Conduct security auditing and compliance checks with CIS benchmark tests
RUN-TIME
❏ Enforce isolation by application / service
❏ Inspect network connections for application attacks
❏ Monitor containers for suspicious process or file system activity
❏ Protect worker nodes from host privilege escalations, suspicious processes or
file system activity
❏ Capture packets for security events
❏ Quarantine or remediate compromised containers
❏ Scan containers & hosts for vulnerabilities
❏ Alert, log, and respond in real-time to security incidents
❏ Conduct security auditing and compliance checks with CIS benchmark tests
KUBERNETES SYSTEM
❏ Review all RBACs
❏ Protect the API Server
❏ Restrict Kubelet permissions
❏ Secure external ports
❏ Whitelist non-authenticated services
❏ Limit/restrict console access
❏ Monitor system container connections and processes in production

