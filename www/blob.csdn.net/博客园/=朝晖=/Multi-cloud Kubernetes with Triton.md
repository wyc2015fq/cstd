# Multi-cloud Kubernetes with Triton - =朝晖= - 博客园
# [Multi-cloud Kubernetes with Triton](https://www.cnblogs.com/dhcn/p/10521517.html)
https://www.joyent.com/blog/triton-kubernetes-multicloud
While running an experimental [Kubernetes](https://kubernetes.io/docs/concepts/overview/what-is-kubernetes/) cluster is fairly simple, operationalizing K8s environments for production is not for the faint of heart. Some of us are also looking to expand their K8s environments across multiple clouds, private or public, for purposes of scalability, workload distribution, disaster recovery, etc. However managing cross-cloud K8s environments from a single control plane is quite challenging and lacks mature out of box solutions.
In this blog post we introduce [Triton Kubernetes](https://github.com/joyent/triton-kubernetes/tree/v0.8), the first truly **multi-cloud Kubernetes** solution that we are working on here at Joyent, and walk through steps to get up and running with a unified set of kubernetes clusters across 4 different public clouds. We will also show how simple it is to deploy an app to environments managed by this multi-cloud K8s solution.
Please actively follow the [project](https://github.com/joyent/triton-kubernetes) for updates on roadmap items like backup and DR, workload migration, federation, full stack monitoring, auto-scaling hosts, alerting and notifications, et all.
The current solution version supports the following public cloud: Triton, AWS, Azure, and GCP. Triton running on-premises as a private or hybrid clouds is also supported. Bare Metal support is limited at this time, but available through professional services engagements. In near future, we plan to add automation to run the solution seamlessly on bare metal servers.
## Getting started in three simple steps:
You can get up and running with production grade kubernetes across multiple-clouds by following the steps listed below. Please see details in the **Quickstart** section below.
### Step one: set up the pre-requisites
To start, you must create a **Triton** account and install the **Triton CLI**, **Terraform**, and the **Kubernetes CLI**. To install the solution across multiple clouds like AWS, Azure or GCP, you must already have or create accounts for these cloud providers respectively.
[Triton](https://www.joyent.com/triton/compute) is Joyent's hybrid and open source cloud and [Terraform](https://www.terraform.io/) is an open source tool that enables you to safely and predictably create, change, and improve production infrastructure. We use Terraform to provision virtual machines, set up root access, and install `python`.
### Step two: create a Global K8s Cluster Manager.
The next step will be to create a highly available Global Cluster Manager, which provides the cross-cloud control plane. This Manager provides scaling, push button upgrades, role based access control, ci/cd integration and monitoring of cluster health. [Triton Kubernetes](https://github.com/joyent/triton-kubernetes/tree/v0.8) automates the provisioning of virtual machine hosts, Docker Engines, database server, networks and sets up the global cluster manager for you. By default, the installer deploys the control plane on Triton, however you can modify the underlying [Terraform](https://www.terraform.io/) templates to install on any cloud.
### Step three: create K8s Environmental Clusters.
The final step will be to provision Kubernetes environments on any region/datacenter on any cloud (having a set of published API endpoints) and have them managed by the global cluster manager. [Triton Kubernetes](https://github.com/joyent/triton-kubernetes/tree/v0.8) does this for you as well. Each environmental cluster is self-sustaining with built-in features like high availability, auto-healing, clustering of etcd and orchestration services and the ability to run specific services on dedicated hosts. The installer automates the provisioning vms, docker, and networking based on the chosen cloud provider and leverages [Rancher](http://rancher.com/kubernetes) as the underlying middleware to provision production grade, supportable (and easily upgradable) Kubernetes Environments.
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/chart.1514543263.jpg)
## Quickstart
Follow along as we walk through these three easy steps in detail below. Complete them on your own, leveraging our [free trial](https://lpage.joyent.com/Triton-Free-Trial.html?utm_source=website) offer to get started on Triton. You will have your very own, 100% open source, production-grade, multi-cloud Kubernetes stack.
**NOTE**: You may encounter an error if you try to run this demo immediately after signing up for the free trial, as we automatically set [provisioning limits](https://docs.joyent.com/public-cloud/getting-started/limits). Request for your limit to be increased or remove any existing instances by [contacting support](https://docs.joyent.com/public-cloud/getting-started/support).
Before you get going, you can also watch a brief demo:
## Pre-requisites
### Install Triton
In order to install Triton, you must have a [Triton account](https://lpage.joyent.com/Triton-Free-Trial.html?utm_source=website). It's important that you have your [billing information](https://my.joyent.com/main/#!/account/payment) handy and [add an ssh key](https://my.joyent.com/main/#!/account) to your account. If you need instructions for how to generate and SSH key, [read our documentation](https://docs.joyent.com/public-cloud/getting-started).
- Install [Node.js](https://nodejs.org/en/download/) and run `npm install -g triton` to install Triton CLI.
- `triton` uses profiles to store access information. You'll need to set up profiles for relevant data centers.
- `triton profile create` will give a [step-by-step walkthrough](https://docs.joyent.com/public-cloud/api-access/cloudapi) of how to create a profile.
- Choose a profile to use for your Triton Kubernetes setup.
- Get into the Triton environment with `eval $(triton env <profile name>)`.
- Run `triton info` to test your configuration.
### Install Terraform
Triton supports all recent versions of Terraform. [Download Terraform](https://www.terraform.io/downloads.html) and unzip the package.
Terraform runs as a single binary named terraform. The final step is to make sure that the terraform binary is available on the PATH. See [this](https://stackoverflow.com/questions/14637979/how-to-permanently-set-path-on-linux) page for instructions on setting the PATH on Linux and Mac.
Test your installation by running `terraform`. You should see an output similar to:
```
$ terraform
Usage: terraform [--version] [--help] <command> [args]
The available commands for execution are listed below.
The most common, useful commands are shown first, followed by
less common or more advanced commands. If you're just getting
started with Terraform, stick with the common commands. For the
other commands, please read the help and docs before usage.
Common commands:
    apply              Builds or changes infrastructure
    console            Interactive console for Terraform interpolations
# ...
```
> 
`terraform` gets downloaded automatically if it isn't found in `$PATH`.
### Install the Kubernetes CLI
This tool is not required to provision, but rather needed to connect to a provisioned kubernetes env. There are different ways to [install `kubectl`](https://kubernetes.io/docs/tasks/kubectl/install/), but the simplest way is via `curl`:
```bash
# OS X
curl -LO https://storage.googleapis.com/kubernetes-release/release/$(curl -s https://storage.googleapis.com/kubernetes-release/release/stable.txt)/bin/darwin/amd64/kubectl
# Linux
curl -LO https://storage.googleapis.com/kubernetes-release/release/$(curl -s https://storage.googleapis.com/kubernetes-release/release/stable.txt)/bin/linux/amd64/kubectl
# Windows
curl -LO https://storage.googleapis.com/kubernetes-release/release/$(curl -s https://storage.googleapis.com/kubernetes-release/release/stable.txt)/bin/windows/amd64/kubectl.exe
```
## Create a Global K8s Cluster Manager
Triton Kubernetes uses `triton`, and `terraform` to set up a global cluster manager and kubernetes environmental clusters. Once those have been installed, you can download the [Triton Kubernetes](https://github.com/joyent/triton-kubernetes/tree/v0.8)package, run the script `triton-kubernetes.sh -c` and answer the prompted questions to start a cluster manager.
> 
`triton-kubernetes -c` can be passed an optional [**conf**](https://github.com/joyent/triton-kubernetes/blob/v0.8/template_clustermanager.conf) file for a silent install.
Default values will be shown in parentheses and if no input is provided, defaults will be used.
```bash
$ git clone https://github.com/joyent/triton-kubernetes.git -b v0.8
Cloning into 'triton-kubernetes'...
remote: Counting objects: 574, done.
remote: Compressing objects: 100% (67/67), done.
remote: Total 574 (delta 43), reused 50 (delta 18), pack-reused 487
Receiving objects: 100% (574/574), 6.57 MiB | 2.46 MiB/s, done.
Resolving deltas: 100% (260/260), done.
Note: checking out 'c77a983d85bfa10033a18e1ad2b77fa76692caab'.
You are in 'detached HEAD' state. You can look around, make experimental
changes and commit them, and you can discard any commits you make in this
state without impacting any branches by performing another checkout.
If you want to create a new branch to retain commits you create, you may
do so (now or later) by using -b with the checkout command again. Example:
  git checkout -b <new-branch-name>
$ cd triton-kubernetes
$ ./triton-kubernetes.sh -c
Downloading Terraform v0.10.8 ...
Extracting Terraform executable
Using /Users/<username>/work/triton-kubernetes/bin/terraform ...
```
If `terraform` is not found in `$PATH`, it will be downloaded.
> 
If a triton profile has been set (`eval "$(triton env)"`), you will not be prompted for triton account details.
```bash
Your Triton account login name: <username> (example fayazg)
The Triton CloudAPI endpoint URL:<region name> (example https://us-east-1.api.joyent.com)
Your Triton account key id: <keyid> (example 2c:53:bc:63:97:9e:79:3f:91:35:5e:f4:c8:23:89:37)
```
Global cluster managers run on Triton so the first few questions prompted will be account related. This is the same information provided by running `triton profile get`.
```bash
Name your Global Cluster Manager: (global-cluster) gclustermanager
```
Provide a name for the Global Cluster Manager and press Enter.
```bash
Do you want to set up the Global Cluster Manager in HA mode? (yes | no) yes
```
Global cluster manager can run in HA mode or non-HA configuration. In HA mode, there will be a two node cluster manager with a database as can be seen in the architectural diagram above.
```bash
Which Triton networks should be used for this environment: (Joyent-SDC-Public)
```
Triton CLI is used here to pull all the active networks (public, private, fabric - sdn) for the current data center defined in the Triton profile. Provide an option for which networks should the global cluster manager use. We are going to use the default so press Enter.
```bash
Which Triton package should be used for Global Cluster Manager server(s): (k4-highcpu-kvm-1.75G) k4-highcpu-kvm-3.75G
Which Triton package should be used for Global Cluster Manager database server: (k4-highcpu-kvm-1.75G) k4-highcpu-kvm-3.75G
```
Since the global cluster manager is set up in HA mode, Triton Kubernetes will prompt for package names to use for the two HA nodes and the database server. Here we are going to use the *k4-highcpu-kvm-3.75G* package for both. For a production install, you should pick instance sizes appropriate to the expected workload and may need bigger resource packages.
```bash
docker-engine install script: (https://releases.rancher.com/install-docker/1.12.sh)
############################################################
Cluster Manager gclustermanager will be created on Triton.
gclustermanager will be running in HA configuration and provision three Triton machines ...
    gclustermanager-master-1 k4-highcpu-kvm-3.75G
    gclustermanager-master-2 k4-highcpu-kvm-3.75G
    gclustermanager-mysqldb k4-highcpu-kvm-3.75G
Do you want to start the setup? (yes | no) yes
```
The last prompt before verification of inputs is the docker-engine installation script that should be used. Leave the default here and press Enter.
```bash
Apply complete! Resources: 4 added, 0 changed, 0 destroyed.
The state of your infrastructure has been saved to the path
below. This state is required to modify and destroy your
infrastructure, so keep it safe. To inspect the complete state
use the `terraform show` command.
State path: terraform.tfstate
Outputs:
masters = [
    <cluster manager node1 ip>,
    <cluster manager node2 ip>
]
Cluster Manager gclustermanager has been started.
This is an HA Active/Active setup so you can use either of the IP addresses.
    http://<cluster manager node1 ip>:8080/settings/env
Next step is adding Kubernetes environments to be managed here.
To start your first environment, run:
    ./triton-kubernetes.sh -e
```
Once verification is finished, `terraform` will download the modules, initialize provider plugins, start provisioning three machines and configure a two node HA cluster manager. Next we will create two Kubernetes clusters (one on Triton and another on GCP), which will be managed by this global cluster manager.
## Create a HA, 5 Node, K8s environmental cluster on Triton
Kubernetes environmental clusters in HA mode run their **etcd** and Kubernetes **orchestration services**(apiserver, scheduler, controller-manager...) in dedicated three node clusters. Once the global cluster manager is up and running, from the same directory, start `triton-kubernetes.sh -e` and answer the prompted questions to start provisioning the first environment.
Default values will be shown in parentheses and if no input is provided, defaults will be used.
```bash
$ ./triton-kubernetes.sh -e
Using triton-kubernetes/bin/terraform ...
From clouds below:
1. Triton
2. AWS
3. Azure
4. GCP
Which cloud do you want to run your environment on: (1)
```
We want the first environmental cluster on Triton; so keep the default and press Enter.
```bash
Your Triton account login name: <username> (example fayazg)
The Triton CloudAPI endpoint URL: <region name> (example https://us-east-1.api.joyent.com)
Your Triton account key id: <key id> (example 2c:53:bc:63:97:9e:79:3f:91:35:5e:f4:c8:23:88:37)
```
This environment will be running on Triton Cloud so the first few questions prompted will be account related. This is the same information provided by running `triton profile get`.
```bash
Name your environment: (triton-test) devcluster
Do you want this environment to run in HA mode? (yes | no) yes
```
Provide a alphanumeric name for this environment and start it in HA mode.
```bash
Number of compute nodes for devcluster environment: (3) 5
```
Provide the number of worker nodes to create for this environment and press Enter.
```bash
Which Triton networks should be used for this environment: (Joyent-SDC-Public)
```
Leave the default value and press Enter.
```bash
Which Triton package should be used for devcluster environment etcd nodes: (k4-highcpu-kvm-1.75G) k4-highcpu-kvm-3.75G
Which Triton package should be used for devcluster environment orchestration nodes running apiserver/scheduler/controllermanager/...: (k4-highcpu-kvm-1.75G) k4-highcpu-kvm-3.75G
Which Triton package should be used for devcluster environment compute nodes: (k4-highcpu-kvm-1.75G) k4-highcpu-kvm-3.75G
```
Since this environment is going to run in HA mode, there will be three different type of VMs.
- etcd nodes: dedicated nodes that will will run an etcd cluster
- orchestration nodes: dedicated nodes that will be running Kubernetes services like the apiserver, scheduler, controllermanager ...
- compute nodes: these are the Kubernetes worker nodes where the compute (deployments) will run
Triton Kubernetes will prompt for package names to use for these nodes. Here we are going to use the *k4-highcpu-kvm-3.75G* package for all nodes.
```bash
docker-engine install script: (https://releases.rancher.com/install-docker/1.12.sh)
############################################################
Environment devcluster will be created on Triton.
devcluster will be running in HA configuration ...
6 dedicated hosts will be created ...
    devcluster-etcd-[123] k4-highcpu-kvm-3.75G
    devcluster-orchestration-[123] k4-highcpu-kvm-3.75G
5 compute nodes will be created for this environment ...
    devcluster-compute-# k4-highcpu-kvm-3.75G
Do you want to start the setup? (yes | no) yes
```
The last prompt before verification of inputs is the docker-engine installation script that should be used. Leave the default here and press Enter.
```bash
Apply complete! Resources: 15 added, 0 changed, 0 destroyed.
The state of your infrastructure has been saved to the path
below. This state is required to modify and destroy your
infrastructure, so keep it safe. To inspect the complete state
use the `terraform show` command.
State path: terraform.tfstate
Outputs:
masters = [
    <cluster manager node1 ip>,
    <cluster manager node2 ip>
]
Environment devcluster has been started.
This is an HA setup of Kubernetes cluster so there are 3 dedicated etcd and 3 orchestration nodes.
Cluster Manager URL:
    http://<cluster manager node1 ip>:8080/settings/env
Kubernetes Hosts URL:
    http://<cluster manager node1 ip>:8080/env/1a7/infra/hosts?mode=dot
Kubernetes Health:
    http://<cluster manager node1 ip>:8080/env/1a7/apps/stacks?which=cattle
NOTE: Nodes might take a few minutes to connect and come up.
To start another environment, run:
    ./triton-kubernetes.sh -e
```
Once verification is finished, `terraform` will start provisioning and configuring a 5 node HA Kubernetes environment. Next we are going create a non-HA Kubernetes environment on GCP.
## Create a non-HA, 3 node, K8s environment on GCP
Non-HA Kubernetes environments run their **etcd** and Kubernetes **orchestration services** (apiserver, scheduler, controller-manager...) on the compute nodes and don't provision dedicated hosts. Once the global cluster manager is up and running, from the same directory, start `triton-kubernetes.sh -e` and answer the prompted questions to start provisioning another Kubernetes environment.
Default values will be shown in parentheses and if no input is provided, defaults will be used.
```bash
./triton-kubernetes.sh -e
Using /Users/fayaz.ghiasy/work/triton-kubernetes/bin/terraform ...
From clouds below:
1. Triton
2. AWS
3. Azure
4. GCP
Which cloud do you want to run your environment on: (1) 4
```
This environment will be starated on GCP so enter 4 and press Enter.
```bash
Path to GCP credentials file: /tmp/test-project-155122-credentials.json
GCP Project ID: test-project-155122
```
This environment will be running on GCP so the first two questions prompted will be account/project related. GCP credentials file should be the absolute path for your [authentication json credentials file](https://www.terraform.io/docs/providers/google/index.html#authentication-json-file) for your GCP account. Also providet the project ID to use where the machiens will be provisioned and press Enter.
```bash
Name your environment: (gcp-test)
Do you want this environment to run in HA mode? (yes | no) no
```
Provide a alphanumeric name for this environment and start it in non-HA mode.
```bash
Number of compute nodes for gcp-test environment: (3)
```
Provide the number of worker nodes to create for this kubernetes environment and press Enter.
```bash
Compute Region: (us-west1)
Instance Zone: (us-west1-a)
```
For this environment we can use the default [Compute Region and Instance Zone](https://cloud.google.com/compute/docs/regions-zones/), and press Enter.
```bash
What size hosts should be used for gcp-test environment compute nodes: (n1-standard-1)
```
We will use the default [machine type](https://cloud.google.com/compute/docs/machine-types) here, press Enter.
```bash
docker-engine install script: (https://releases.rancher.com/install-docker/1.12.sh)
############################################################
Environment gcp-test will be created on GCP.
gcp-test will be running in non-HA configuration ...
3 compute nodes will be created for this environment ...
    gcp-test-compute-# n1-standard-1
Do you want to start the setup? (yes | no) yes
```
The last prompt before verification of inputs is the docker-engine installation script that should be used. Leave the default here and press Enter.
Once verification is finished, `terraform` will start provisioning and configuring a 3 node non-HA Kubernetes environmental cluster. In this environment, the etcd and Kubernetes components all run on the compute (worker) nodes and share resources with deployments.
## Deploying your first multi-cloud K8s application.
Now we are ready to deploy our first application on our multi-cloud Kubernetes environment using either the Kubernetes CLI `kubectl` or the [Kubernetes Dashboard](https://kubernetes.io/docs/user-guide/ui/).
In this section we will walk through the deployment of a "Ghost blog" app using the Kubernetes Dashboard and the example Kubernetes "Guestbook" app using `kubectl`.
### Deploy an app using the Kubernetes Dashboard (Web UI)
The [Kubernetes Dashboard](https://kubernetes.io/docs/user-guide/ui/) can be used to get an overview of applications running on your cluster, as well as to create or modify individual Kubernetes resources. The Kubernetes Dashboard also provides information on the state of Kubernetes resources in your cluster.
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image12.1514543263.png)
Now, let's deploy Ghost using the Kubernetes Dashboard.
First, get the URL for the Kubernetes Dashboard by going to the *Kubernetes Hosts URL* provided at the end of the [devcluster](https://www.joyent.com/blog/triton-kubernetes-multicloud#start-an-ha-5-node-kubernetes-environmental-cluster-on-triton-cloud) cluster setup, and click **Dashboard** under the **Kubernetes** menu at the top. Note, every environment has a unique Dashboard URL printed to the console output.
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image15.1514543263.png)
In this next page, click **Kubernetes UI** button to open the dashboard. Once you are in the Kubernetes Dashboard you should see a **CREATE** button at the top. Click the **CREATE** button to begin the process of deploying an app on your Kubernetes Environment.
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image08.1514543263.png)
Next, enter the details requested, using the inputs provided in the below image, and then click **Deploy**.
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image14.1514543263.png)
That's it! Kubernetes should now be starting up your Ghost app and you should see something that looks like this:
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image04.1514543263.png)
Your app is configured to be exposed externally on port 8080. So, you should see the app URL under the services screen. Once the deployment is complete and pods are up, the app should be available.
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image00.1514543263.png)
### Deploy an app using the Kubernetes CLI
Now, let's deploy the example Kubernetes Guestbook app using the Kubernetes CLI.
First, get the URL to the Kubernetes CLI config page, which will generate a `kubectl` config file, that the Triton Kubernetes provided at the end of the environment setup. Note that each K8s environment has it's own unique kubectl URL printed on the console output.
Go to the Kubernetes CLI config URL and click on **Generate Config**:
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image01.1514543263.png)
From the next screen click **Copy to Clipboard** and paste the content to the `~/.kube/config` file:
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image06.1514543263.png)
Now you should be able to use the `kubectl` command to deploy your app.
The app we will deploy is called [Guestbook](https://github.com/kubernetes/examples/tree/master/guestbook). Clone the repository to your local machine, and navigate to the app's directory in your terminal. We'll make one minor change to the configuration file so that we can interact with it using a public IP address for this demo:
```bash
git clone https://github.com/kubernetes/examples.git
cd examples/guestbook
vi all-in-one/guestbook-all-in-one.yaml
```
In that configuration file (`all-in-one/guestbook-all-in-one.yaml`), uncomment the frontend service type, [`# type: LoadBalancer`](https://github.com/kubernetes/examples/blob/master/guestbook/all-in-one/guestbook-all-in-one.yaml#L108), so that it runs as a load balancer:
```bash
spec:
  # if your cluster supports it, uncomment the following to automatically create
  # an external load-balanced IP for the frontend service.
  type: LoadBalancer
  ports:
    # the port that this service should serve on
  - port: 80
  selector:
    app: guestbook
    tier: frontend
```
Be sure to save the file.
Now you should be able to use `kubectl` to deploy the app and get the external URL for the frontend service, which can be used to access the app once the pods are up:
```bash
# Deploy guestbook app
 $ kubectl create -f all-in-one/guestbook-all-in-one.yaml
service "redis-master" created
deployment "redis-master" created
service "redis-slave" created
deployment "redis-slave" created
service "frontend" created
deployment "frontend" created
# Make sure that the pods are up and running
 $ kubectl get deployments
NAME           DESIRED   CURRENT   UP-TO-DATE   AVAILABLE   AGE
frontend       3         3         3            3           2m
redis-master   1         1         1            1           2m
redis-slave    2         2         2            2           2m
 $ kubectl get pods
NAME                           READY     STATUS    RESTARTS   AGE
frontend-88237173-b23b9        1/1       Running   0          2m
frontend-88237173-cq5jz        1/1       Running   0          2m
frontend-88237173-sbkrb        1/1       Running   0          2m
redis-master-343230949-3ll61   1/1       Running   0          2m
redis-slave-132015689-p54lv    1/1       Running   0          2m
redis-slave-132015689-t6z7z    1/1       Running   0          2m
# Get the external service IP/URL
 $ kubectl get services
NAME           CLUSTER-IP      EXTERNAL-IP      PORT(S)        AGE
frontend       10.43.80.176    165.225.175.75   80:30896/TCP   14s
kubernetes     10.43.0.1       <none>           443/TCP        7m
redis-master   10.43.0.176     <none>           6379/TCP       15s
redis-slave    10.43.141.195   <none>           6379/TCP       15s
```
We can see above, for this demo, all pods are running and the only service exposed externally is the frontend service on `165.225.175.75:80`.
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image11.1514543263.png)
The deployment status for all the pods and services can also be viewed using the Kubernetes Dashboard. To check using Dashboard, go to the URL for the Web UI.
![](https://www.joyent.com/content/blog/20180102-triton-kubernetes-multicloud/image07.1514543263.png)
For more information on Kubernetes itself, dig into the [official Kubernetes user guide](https://kubernetes.io/docs/user-guide/) or the `kubectl`[cheatsheet](https://kubernetes.io/docs/user-guide/kubectl-cheatsheet/).

