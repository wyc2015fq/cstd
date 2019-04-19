# 6 Multi-Cloud Architecture Designs for an Effective Cloud - =朝晖= - 博客园
# [6 Multi-Cloud Architecture Designs for an Effective Cloud](https://www.cnblogs.com/dhcn/p/10506525.html)
https://www.simform.com/multi-cloud-architecture/
Enterprises increasingly want to take advantage of the flexibility and choice of multiple cloud offerings in order to use best cloud services while achieving satisfactory cost reduction benefits. In fact, the[International Data Corporation predicted](https://www.idc.com/research/viewtoc.jsp?containerId=US41863916) in a futurescape report last year that more than 85% of enterprise IT corporations will invest in and adopt multi-cloud architecture by 2018. With the enterprise hurtling towards digital transformation at breakneck speeds, the adoption of cloud, both public and private, has accelerated. And working across different clouds across various platforms brings a whole lot of challenges in its wake.
This is the first blog of our three blog series on multi-cloud. In this blog, you will get to know about multi cloud and its various architectures for different organizational requirements. In the second blog, We have discussed [Strategies to manage Multi cloud environment effectively](https://www.simform.com/multi-cloud-management/).
## Multi-Cloud Strategy
Multi-cloud strategy is the concomitant use of two or more cloud services such as AWS, Azure, Google Cloud and more.
This means you can use Google cloud to serve your US users and Microsoft Azure for your customers in Europe.
Or you might use Azure SQL for your databases and Cognito for user management while using AWS EC2 instances and Load Balancing, all for a single application.
In addition, you can run your app primarily on Digital Ocean but is completely replicated and backed up on AWS.
You can run different app on different clouds. You can have your development and test environments on one cloud, and your production environment on another.
But to take advantage of multi cloud benefits, it is very important to manage and monitor your cloud deployment and costs.
Multi-cloud strategy is particularly effective if enterprises deal with below challenges:
- Users are not located near any data center, or widely distributed geographically.
- Facing regulations limit in particular countries for storing data, e.g., EU.
- Environment where public clouds are used with on-premises resources.
- Cloud-based application is not resilient which can affect disaster recovery when loss of a single data center.
## Multi-Cloud Architecture
In order to build highly scalable and reliable applications, a multi-cloud architecture is appropriate. Our objective is to provide architectural guidance for migrating cloud-based systems that run on multiple independent clouds. Let’s take a look at some of the prominent multi cloud architectures and migration strategy.
### Cloudification
In this architecture application component is hosted on-premise and after migration, it can use different cloud services of other cloud platforms to improve performance.
![Multi-Cloud Architecture](http://www.simform.com/wp-content/uploads/2017/11/Cloudification-2.png)
Here application component C1 is been hosted on-premise but after adopting multi-cloud, it uses AWS storage service AWS S3 and for compute it uses Azure virtual machines.
Benefits: Improves availability as application re-hosting in multiple cloud platforms and avoid vendor lock-in.
### Multi-Cloud Relocation
In this architecture application component is re-hosted on cloud platform and use other cloud services of multiple cloud platform to enhance capabilities.
![Multi-Cloud Architecture](http://www.simform.com/wp-content/uploads/2017/11/Cloudification-4.png)
Here application component C1 is re-hosted on AWS platform after migration and open to use environmental services of Azure. It is using AWS S3 for storage and has option available for compute either AWS or Azure.
Benefits: Improves availability as application re-hosting in multiple cloud platforms and avoid vendor lock-in.
### Multi-Cloud Refactor
To provide better QoS, An on-premise application is re-architected for deployment on multiple cloud platforms. Here application needs to re-architected as fine-grained components so that deployment of high-usage components can be optimized independently. Here deployment of high-usage components is optimized independently of low-usage ones. The parallel design enables better throughtput to multi cloud platforms.
![Multi-Cloud Architecture](http://www.simform.com/wp-content/uploads/2017/11/Cloudification-5.png)
Here AC1 and AC2 are two application components hosted on-premise before migration . As both the components are independent integrity units, AC1 is deployed on AWS using AWS S3. On the other hand, AC2 is deployed on Azure and it can use any Azure’s cloud service as per requirements.
Benefits: Optimal scalability/performance, range of multi-cloud deployment options, agility to respond to business/IT change.
Challenges: On-premise application is modernized in isolation. Modernization is performed primarily for technical reasons. Component architecture which is only determined bottom-up may need to be re-evaluated because of multi-cloud environment.
### Multi-Cloud Rebinding
A re-architected application is deployed partially on multiple cloud environments and enables the application to continue to function using secondary deployment when there is a failure with the primary platform.
![Multi-Cloud Architecture](http://www.simform.com/wp-content/uploads/2017/11/Cloudification-10.png)
Here AC1 and AC2 are two application components hosted on-premise before migration. As both the components are independent integrity units, AC1 is remains on-premise while two AC2 are deployed on AWS and Azure for disaster recovery. AC1 and two AC2 components are connected via EBS or Service bus.
Benefits: As unhealthy services become healthy again, traffic can be delivered, returning system responsiveness to maximum.
Challenges: This architecture does not guarantee seamless failover. There will be downtime.
### Multi-Cloud Rebinding with Cloud Brokerage
A re-architected application is deployed partially on multiple cloud environments. This enables the application to continue to function using secondary deployment when there is a failure with the primary platform using  cloud brokerage services.
![Multi-Cloud Architecture](http://www.simform.com/wp-content/uploads/2017/11/Cloudification-11.png)
In this architecture AC1 has been deployed on-premise and two re-architected AC2 are deployed on two cloud platforms AWS and Azure. Here Cloud broker services integrates all three components and provides flexibility to choose services from multiple providers (Cloud platforms AWS and Azure).
Benefits: As unhealthy services become healthy again, traffic can be delivered, returning system responsiveness to maximum.
Challenges: This architecture does not guarantee seamless failover. There will be downtime.
What is a Cloud Broker and Cloud Services Brokerage?
A cloud broker is an individual or business that consults and facilitates the selection of cloud computing solutions as an intermediary on behalf of an organization.
Gartner defines the Cloud Service Broker function as adding value to cloud services by providing a marketplace for enterprise-approved services, integrating cloud services with on-premise applications and with each other, and ensuring corporate data is secure in the cloud.
A cloud broker combines and integrates multiple services into one or more new services. The broker ensures the security of data and provides data integration in transition between the cloud customer and multiple cloud providers.
Benefits of using a cloud broker for a business or technical purpose include the following:
- Cloud interoperability – Provides Integration between various cloud offerings.
- Cloud portability – You can move application(s) between different cloud vendors.
- Reducing dependency on one cloud provider.
- Increase SLAs by leveraging multiple cloud providers.
- Cost savings – Most IaaS clouds offer 10% – 40% discounts to those who have purchased a large number of instances. For example, Amazon provides 20% or even higher discounts for EC2 instances. Due to the sheer volume of the aggregated demand, the cloud broker can easily qualify for such discounts, which further reduces the cost of serving all the users.
Pay for as per requirement: Cloud brokers provide a selected assortment of cloud services required by the consumer.
### Multi-Application Modernization
Different on-premise applications A1/A2, AC1 are re-architected as a portfolio and deployed on cloud environment.
![Multi-Cloud Architecture](http://www.simform.com/wp-content/uploads/2017/10/Cloudification-15.png)
Benefits: It provides consistent information and rules in shared components. Reduced operation and maintenance costs for shared components is another benefit.
Challenges: Lack of business commitment to shared capabilities.
## Multi cloud vs Hybrid cloud
On the first pass, both may appear same and these terms used interchangeably by some, but they are not the same. Here we’ve explained the differences, which are subtle but very distinct, between the two.
### Hybrid cloud
Hybrid cloud is a mix of public and private cloud that connects the public cloud such as AWS to your on-premise system and is orchestrated to perform together for a single task. In this scenario, you’re optimizing your workload so it runs in the right environment at the right time. Using Hybrid cloud, organizations can access highly elastic compute resources from the chosen provider, perhaps for managing and storing additional workloads at peak times and for general day to applications. But all the mission-critical stuff remains on-premise infrastructure for multiple reasons like privacy regulations and security.
The way in which public cloud services and private cloud operations are utilized is dependent on organizational needs and priorities. The extent to which public cloud services are utilized can be as minimal as an offsite backup, or as extensive as being the primary component of data storage and processing. The process of finding an appropriate balance between public and private should take into consideration your organization’s IT budget, the strength of internet infrastructure in the areas in which your organization operates, and needs for regulatory compliance.
Why implement Hybrid cloud?
For certain use cases, organizations require a combination of a private and public cloud to leverage unique benefits offered by them. In such circumstances, a hybrid cloud may prove to be the better choice.
Organizations can leverage cloudbusting, in which application workloads burst into the public cloud for additional compute resources after they reach to a threshold level in private cloud.
It makes sense for organizations to use public cloud resources for a new, untested application before embarking on the capital expenditure associated with launching in a private cloud. Once an organization defines a steady-state workload pipeline to run an application, it may choose to bring the application to on-premise systems.
Cloud customers can, however, use hybrid clouds to promote high availability (HA) and disaster recovery (DR). For example, in a Disaster recovery scenario, an organization can keep its recovery environment in a public cloud and production environment in a private cloud, ready to spin up as necessary. The organization replicates data across to the public cloud, but all other resources remain non-operational until needed.
Hybrid cloud architecture provides maximum agility for meeting the needs of the organizations by providing infrastructure where IT operations can be automated to improve the user experience.
### Multi cloud
Multi cloud represents more than 1 cloud deployment of the same type and it can be public or private cloud, sourced from different cloud providers. Business adopt Multi-cloud to mix and match a range of public and private clouds to use best of breed applications and services.
These cloud approaches are not mutually exclusive: You can have both, simultaneously. In fact, Most organizations seek to improve security and performance through an expanded portfolio of environments.
Why implement Multi cloud?
Multi cloud offers IT teams increases flexibility and control over workloads and data.
As multi cloud offers flexible cloud environment, organizations can meet specific workload or application requirements- both technically and commercially by adopting it. It is obvious that not every team, department, business functions or workload require similar performance, security, privacy or geographic reach for their cloud. Being able to use multiple cloud providers that meet their various application and data needs is critical as cloud computing has become more mature and mainstream.
Organizations see geographic advantages to using multiple providers, to address app latency problems, for example. Also, some companies may start using specific cloud providers for short time to achieve short-term goals and then stop using it.
Additionally, vendor lock-in concerns and possible cloud provider outages are two issues that pop up frequently when IT leaders advocate for multi-cloud strategy.
## Tips for Managing Your Multi-Cloud Environment
You may be running a workload that requires large pools of storage and networking resources on a private cloud. At the same time, you may have another workload running on Amazon’s service and yet another on Microsoft’s. Each workload is running on the ideal cloud, but now you have multiple clouds to manage.
Multi-cloud environments come with their challenges—complexity, resources, expertise, cost, and management issues, to name a few—and management seems to be the common denominator. Here are 5 things to keep in mind when building an effective multi cloud environment.
- Analyze your entire network, and then identify which service of particular cloud provider is the best for your specific requirement. Get a clear picture of all the cloud provider’s role in your overall system-management strategy to avoid system complexity and poor utilization of resources.
- Automate low-level monitoring and maintenance tasks. Having multiple cloud providers increases low-level maintenance and monitoring tasks. To reduce the amount of human oversight, you can use tools like [BetterClouds](https://platform.doublehorn.com/)for performance monitoring.
- Focus on automating policy across cloud environments. The best way to use multiple cloud services is to have a single standard for policies that are applied automatically to each cloud environment. The policies cover such areas as data storage, workloads, traffic flows, virtual servers, compliance/regulation, security and reporting. If you have a single configuration across different infrastructures it becomes easier to apply changes and updates because the changes and updates propagate seamlessly from environment to environment.
- Use integrated data center management system designed for virtual environments. It helps building system in which your server, network, storage, operations, security, and applications teams work according to common goals.
- Identify the apps in your organization that are best suited for multi cloud environment. Traditional apps are inflexible, run as Virtual Machines, and use a scale-up architecture. Generally these type of applications are difficult to develop, deploy and maintain. On the other hand, cloud-native apps are more flexible and service-oriented, comprised of collections of containers and services, based on a scale-out architecture. Also, cloud native applications are easy to automate, move, and scale.
## Benefits of Multi Cloud Architecture
### Disaster recovery
When an organization uses one cloud platform, all If all the organizational resources use one cloud platform, a cyber attack can take down all the operations for a long time leaving end users inaccessible until it resolves. When you use multi-cloud architecture, it makes your company’s services resilient against these types cyber attacks because there are other clouds available to take the workloads when one cloud goes down.
### Avoiding vendor lock-in
The multi-cloud platform allows organizations to select the best services from every cloud platform, create custom infrastructure specific to their organizational goals. Rather than managing business processes to fit a specific provider’s setup and execution, businesses can explore different providers to find the best match for each part of business operations.
### Data management
There are different data types generated in the organization. For example, some database requires cold storage which is not accessed regularly while hot data accessed 10 times a day and must be stored in frequently-accessed storage,e.g., AWS S3 standard storage. Rather than lump all your data into one cloud, you can diversify to take advantage of the right service for the right function.
### Cloud cost optimization
Before adopting the Multicloud strategy, you should do a performance analysis of your workloads that are either on-premises or already in the cloud and comparing that to what’s available in each cloud. As a result, you can identify the options that will meet your workload performance requirements at the cheapest cost. For example, You can save money by running spot instances for workloads designed to be fault tolerant, while running legacy workloads on reserved instances.
### Low latency
When application users are distributed worldwide and data transfer is done from single data center, lots of users will experience slow response time. There will delays when data traffic has to travel across several nodes before reaching end-users. This delay, called latency is inherent in cloud services delivered from servers at distant locations.
In a multi cloud environment, cloud architects can deploy data centers to multiple regions according to user locations. The datacenter closest to end-users can serve the requested data with minimum server hops. This capability is especially useful for global organizations that need to serve data across geographically disparate locations while maintaining a unified end-user experience.
## Conclusion
Multi-Cloud architecture provides an environment where businesses can build secure and powerful cloud environments outside the traditional infrastructure. Maximizing the impact of multi-cloud, however, means tackling the challenges of app sprawl, unique portals, compliance, migration and security head-on. The main goal multi cloud strategy is to use as many clouds as per requirements to address the limitations of using one cloud from a single provider. Though toggling between cloud providers to perform tasks can be complicated, especially right out of the gate, cloud service providers are working to make toggling between clouds increasingly efficient. The more efficient it becomes, the more multi-cloud computing will evolve.

