# How Microservices are Transforming Python Development - =朝晖= - 博客园
# [How Microservices are Transforming Python Development](https://www.cnblogs.com/dhcn/p/7851115.html)
https://blog.appdynamics.com/engineering/how-microservices-are-transforming-python-development/
Summary
The goal of any tech business worth its salt is to provide the best product or service to its clients in the most efficient and cost-effective way possible. This is just as true in the development of software products as it is in other product design services.
![](https://46zwyrvli634e39iq2l9mv8g-wpengine.netdna-ssl.com/wp-content/themes/jv-superblog/images/article-summary-divider.png)
The goal of any tech business worth its salt is to provide the best product or service to its clients in the most efficient and cost-effective way possible. This is just as true in the development of software products as it is in other product design services.
[Microservices](http://www.fullstackpython.com/microservices.html), an app architecture style that leans mostly on independent, self-contained programs, are quickly becoming the new norm, so to speak. With this change comes a declining reliance on older SOAs like COBRA, a push toward more sustainable API approaches and fewer monolithic development and deployment models.
So why are microservices suddenly at the forefront of the software architecture conversation? They are changing how Python-based developers are getting things done in a way that’s far more efficient than before, and in more ways than one.
### The Differences Between Microservices and SOAs
Diving deeper into the [differences between microservices and SOAs](http://stackoverflow.com/questions/25501098/difference-between-microservices-architecture-and-soa), you have to remember that, at their core, microservices are essentially an offshoot of SOAs, although they both act and deploy independently from each other.
SOAs also follow four major tenets during the development and deployment phases:
- 
Their boundaries are inherently explicit.
- 
They provide autonomous services.
- 
Those services share both schema and contract but not class.
- 
The compatibility of those services is policy-based.
Once you’ve established these distinctions, you can then make a far more accurate comparison between microservices and SOAs in that SOAs are architectural patterns that use their respective components to provide services to other components, within or without the same application. In microservices, only services independent of the application in question deploy those same components.
Although microservices are not a novel or inherently “new” architecture style, as much of their roots derive from the founding design principles found in Unix language, there are still several implications of an increase in productivity and innovation if more developers use microservices.
### The Evolution of Microservices
Overall, the timeline for the evolution of Python-based apps, from monolithic to microservices, has been a relatively short one. On top of that, much of the evolution was born out of a necessity for forward progression and increased ease among developers.
It is widely accepted that microservices have more substance attached to them because they’ve done away with bulky XML-based schemas that large corporations are known for using in favor of slimmer applications that rely far less on bloat. Ultimately, microservices have become more common over time because they:
- 
Can deploy independently of the core application
- 
Can function properly while remaining separate from dependent responsibilities
- 
Possess strong backward compatibility, making them less prone to breakage
### Development team advantages include:
- 
Allow for the decentralization of data management so teams and subteams can be responsible for maintenance on a far more granular level
- 
Enable the use of infrastructure automation, from testing to deployment, without much need for human supervision
- 
Faster ramp-up time for new team members means they can learn processes faster by focusing on smaller chunks of data.
There are still widely accepted, monolithic-first approaches within the development phase that development teams can break down into SOAs and, further still, into microservices. Some of the more successful applications still employ monolith-first patterns but in conjunction with the use of microservices and even nano-services.
### A Word (or Two) on Nano Services
There’s plenty of support (and animosity) toward just how deep developers should go down the rabbit hole concerning [the development and use of nano-services](http://arnon.me/2014/03/services-microservices-nanoservices/). Just as you’d think, nano-services are simply components that designers have drilled down to an even more granular level than their microservices predecessors.
For some, it is a virtual splitting of hairs while, for others, it is yet another landscape that we have yet to understand fully and, therefore, properly utilize. Both sides can agree, however, that the status quo will likely share neither sentiment anytime soon.
### Advantages of Microservices
While developers and the enterprise-level businesses they work for tend to flock toward the main codebase approach of a monolithic architecture for its benefits, there are also some pros that come with building a software product incrementally.
Microservices come with a unique set of advantages, some briefly mentioned earlier, that allow developers to create building blocks that they can then retrofit into an existing codebase as needed. Other significant advantages include:
- 
There’s ability to change the implementation of a public API, without breaking it, the moment you define it and others start using it
- 
The services are so small that they make maintenance from one developer to another easier to facilitate and understand.
- 
There are no development language limitations, so you can use what’s best for you and your team.
- 
It is easier to upgrade systems one microservice at a time than it is to upgrade a monolithic system.
- 
Cross-implementation compatibility allows you to prototype in one language and re-implement in another.
- 
Regardless of the size of your operation, if most of your product builds require more detailed components and adaptive development, then microservices are a better approach.
### Current Microservices Implementations
Arguably, the [go-to implementation](https://www.docker.com/) for most microservices today, Python-based or otherwise, is Docker. Aside from increased agility and control, many of today’s developers are embracing the ability to work remotely, so naturally any implementation that allows for more portability than the competition is greatly appreciated by the developer community.
Other popular implementations include but are not limited to:
- 
Flask
- 
MicroService4Net
- 
Microsoft Service Fabric
- 
NetKernel
- 
Nirmata
- 
Spring Cloud
Currently, the trend toward component development and product compartmentalization will continue as the need for customizable applications, and modular design becomes more prevalent.
### Why Python?
Most Python developers who implement microservices during development likely use the [RESTful](https://speakerdeck.com/gnrfan/restful-microservices-with-python) approach to creating an API, which is an all-inclusive way of utilizing available Web protocols and software to remotely search and manipulate objects.
Reverse engineered by Dr. Roy Fielding in 2000, RESTful microservices has a basic premise that follows three distinct canons:
- 
You are required to use any provided links or other resources, making your application’s API browseable.
- 
You are expected to recognize the uniform interface of HTTP.
- 
You are expected to use each of the verbs (e.g., get, post, put, delete) without violating their own semantics.
### Python’s Development Advantages
As mentioned before, you can implement and re-implement microservices across virtually every language, but with Python, there are several advantages that make working within it straightforward and convenient. They include:
- 
So long as the API is formatted correctly, prototyping is easier and quicker than in other languages.
- 
Instead of having to rely on full-fledged installations of implementations, like Django, you can use lighter installs that are just as powerful, like Flask and others.
- 
Looking toward the future, it is a fantastic opportunity to start coding in Python 3, if you do not already.
- 
Backward compatibility with legacy languages, like PHP and ASP, allows you to build Web service front ends to a host of microservices.
Furthermore, microservices help to optimize the performance of Python-developed applications two-fold:
- 
They become easier to monitor, due to the fact apps are now broken up into components.
- 
Performance issues become easier to identify, allowing for more granular diagnoses of flawed, bottlenecked or buggy services.
Moreover, for a design pattern that’s used by the likes of Google, Amazon, Microsoft, Netflix, Uber and more, there are no signs of this architecture going anywhere any time soon.
### The Future of Microservices
Although it is easy to agree that the future of software architecture development is moving in the direction of increased modularity and microservices, that does not mean it will not come with its fair share of complications. This is doubly true for larger companies that have created much of their codebase with a monolithic approach in mind.
There are [plenty of reasons](http://martinfowler.com/articles/microservices.html#HowBigIsAMicroservice) why an industry-wide shift to microservices might fail, but there are a few challenges that developers and software architects should be mindful of in the coming years:
- 
Complete software compatibility: With componentization, much of software’s success depends on its compatibility with its respective components and vice versa. Moving code from service to service becomes difficult, and a development team would need to orchestrate complete coordination.
- 
Clean composition: If microservice components do not compose code cleanly, you are simply over complicating the inner workings of connections between their respective components. This shifts unnecessary stress and complexities to an area that’s harder to control.
- 
Evolutionary design considerations: When you decide to work in an environment where you can break down components, you are faced with the challenge of figuring out how and where to break them down. It calls for making the risky decision of knowing what you can scrap or save from version 1.0 to version 5.1 and beyond far ahead of time.
- 
Required skill sets: Not all teams are created equal. This means that because one team may have the skills required to adopt new techniques does not mean your team will. Foisting an ill-equipped team into uncharted territory could prove disastrous for your entire infrastructure.
Although the distinctions between SOAs and microservices might seem a bit minute, each one still has its intended purposes, whether you are using Python or any other development language.
The fact remains that microservices are only going to become more of necessity as development projects require more specific (and complex) functions and harness the interconnectedness that can come with coding for mutually independent services; however, these changes can, and may, come with a unique collection of augmentations and subsequent headaches.
