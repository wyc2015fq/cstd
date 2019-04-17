# What is IP Routing? - Keith - CSDN博客





2014年03月15日 23:01:48[ke1th](https://me.csdn.net/u012436149)阅读数：611








# What Is IP Routing?






# More information

## Related page
[IP routing software](http://network-technologies.metaswitch.com/ip-routing-unicast/default.aspx)





**IP Routing** is an umbrella term for the set of **protocols** that determine the path that data follows in order to travel across multiple networks from its source to its destination. Data is routed from its source to its destination
 through a series of routers, and across multiple networks. The IP Routing protocols enable routers to build up a forwarding table that correlates final destinations with next hop addresses.


These protocols include:
- [**BGP** (Border Gateway Protocol)](http://network-technologies.metaswitch.com/ip-routing-unicast/what-is-bgp.aspx)
- [**IS-IS** (Intermediate
 System - Intermediate System)](http://network-technologies.metaswitch.com/ip-routing-unicast/what-is-isis.aspx)
- [**OSPF** (Open Shortest Path First)](http://network-technologies.metaswitch.com/ip-routing-unicast/what-is-ospf.aspx)
- [**RIP** (Routing Information Protocol)](http://network-technologies.metaswitch.com/ip-routing-unicast/what-is-rip.aspx)


When an IP packet is to be forwarded, a router uses its forwarding table to determine the next hop for the packet's destination (based on the destination IP address in the IP packet header), and forwards the packet appropriately. The next router then repeats
 this process using its own forwarding table, and so on until the packet reaches its destination. At each stage, the IP address in the packet header is sufficient information to determine the next hop; no additional protocol headers are required.


The Internet, for the purpose of routing, is divided into Autonomous Systems (ASs). An AS is a group of routers that are under the control of a single administration and exchange routing information using a common routing protocol. For example, a corporate
 intranet or an ISP network can usually be regarded as an individual AS. The Internet can be visualized as a partial mesh of ASs. An AS can be classified as one of the following three types.
- A **Stub AS** has a single connection to one other AS. Any data sent to, or received from, a destination outside the AS must travel over that connection. A small campus network is an example of a stub AS.


- A **Transit AS** has multiple connections to one or more ASs, which permits data that is not destined for a node within that AS to travel through it. An ISP network is an example of a transit AS.


- A **Multihomed AS** also has multiple connections to one or more ASs, but it does not permit data received over one of these connections to be forwarded out of the AS again. In other words, it does not provide a transit service to other ASs.
 A Multihomed AS is similar to a Stub AS, except that the ingress and egress points for data traveling to or from the AS can be chosen from one of a number of connections, depending on which connection offers the shortest route to the eventual destination.
 A large enterprise network would normally be a multihomed AS.


An **Interior Gateway Protocol (IGP)** calculates routes within a single AS. The IGP enables nodes on different networks within an AS to send data to one another. The IGP also enables data to be forwarded across an AS from ingress to egress, when
 the AS is providing transit services.


Routes are distributed between ASs by an **Exterior Gateway Protocol (EGP)**. The EGP enables routers within an AS to choose the best point of egress from the AS for the data they are trying to route.


The EGP and the IGPs running within each AS cooperate to route data across the Internet. The EGP determines the ASs that data must cross in order to reach its destination, and the IGP determines the path within each AS that data must follow to get from the
 point of ingress (or the point of origin) to the point of egress (or the final destination).


The diagram below illustrates the different types of AS in a network. OSPF, IS-IS and RIP are IGPs used within the individual ASs; BGP is the EGP used between ASs.

![Autonomous Systems in IP Routing](http://network-technologies.metaswitch.com/ip-routing-unicast/images/ipras.gif)














from://http://network-technologies.metaswitch.com/ip-routing-unicast/what-is-ip-routing.aspx



