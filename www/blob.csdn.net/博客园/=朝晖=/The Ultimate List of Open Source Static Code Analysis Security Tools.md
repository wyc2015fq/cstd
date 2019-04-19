# The Ultimate List of Open Source Static Code Analysis Security Tools - =朝晖= - 博客园
# [The Ultimate List of Open Source Static Code Analysis Security Tools](https://www.cnblogs.com/dhcn/p/9606769.html)
https://www.checkmarx.com/2014/11/13/the-ultimate-list-of-open-source-static-code-analysis-security-tools/
Doing security the right way demands an army – of developers, security teams, and the tools that each uses to help create and maintain secure code.
With the increasingly important mindset of creating quality, secure code from the start, we’ve seen a greater shift towards the adoption of tools designed to detect flaws as quickly as possible in the software development lifecycle (SDLC).
One of those tools is [static code analysis](https://www.checkmarx.com/technology/static-code-analysis-sca/). The true strength of static source code analysis (SCA) is in quickly and automatically checking everything “under the hood” without actually executing the code. Because it works to discover issues that can be hard to discover manually, it’s a perfect companion to the human eye. Even the most senior security people still miss security flaws. After all – we are still *human*, so the combination of machine and man make for better coverage.
It takes a strong source code analysis tool (and probably several for full coverage, especially if we’re talking open source) to help get the job of securing an application done. While we would normally have a perfect product for these sorts of issues (hint, hint), commercial tools just aren’t for everyone, for any number of reasons. Whether you’re a developer or part of a security team, there is an array of open source static source code analysis options out there, but only a few are really good.
To help those searching for an open source static source code analysis tool (quite a mouthful!), we’ve compiled a list of the best tools for different languages. Before you start the hunt, though, there are some things to consider.
![New Call-to-action](https://cdn2.hubspot.net/hubfs/146169/hub_generated/resized/26f30bc2-49a7-4b97-b42b-3fca87872dd8.png)
#### Questions to ask yourself when choosing an open-source static code analysis tool:
- Does the tool support your language (s)?
- What types of vulnerabilities and code issues do you need to look for in your code?
- Will this tool work within your IDE for immediate feedback and faster remediation?
- What’s the learning curve involved with the tool?
- Will you and/or your developers have time to learn, fine-tune and customize the tool to its full potential? *Consider: Time is money, and the more time you have to spend learning how to configure and use a tool can lead to lots of lost $.*
- How much support are you looking for from the tool’s developers? *Note: As many of these open source tools are passion/side projects, they may not be the best idea if you’re interested in receiving a certain level of support.*
- Will this tool supplement your other tools or will it take a lot of work integrating them together?
- What’s the rate of false positives associated with the tool?
- Finally, keep in mind that the majority of the tools developers and security minds around the world will tell you that one of these tools is not enough to do the job needed and release a secure product. If you’re looking into open-source options, it’s worth it to find a few solutions that work best for all your needs.
For a nice guide through the process, the WASC (Web Application Security Consortium) also offers an invaluable [evaluation checklist](http://projects.webappsec.org/w/page/66094278/Static%20Analysis%20Technologies%20Evaluation%20Criteria) for choosing the tool best suited for your needs.
#### The 9 Most Popular Open Source Static Source Code Analysis Tools for Developers & Security Teams:
## Multiple Languages
### [VisualCodeGrepper](http://visualcodegrepp.sourceforge.net/)
This automated code security tool works with C++, C#, VB, PHP and Java to identify insecurities and other issues in the code.  Developed by Nick Dunn ([@N1ckDunn](https://twitter.com/N1ckDunn)), the tool quickly scans and describes – in detail – the issues it finds, offering an easy-to-use interface.
The Good:
- Allows for custom configurations for your own queries
- Tells you the security level of the vulnerabilities it finds
- Searches intelligently for specific violations of [OWASP](https://www.checkmarx.com/glossary/owasp-top-10/) recommendations
- Consistently updated since its creation in 2012
The Not-As-Good:
- While it can analyze many languages, you have to tell it the language you’re scanning
- Scans for a set list of vulnerabilities that cannot be modified
- Isn’t fully automated
### [YASCA](https://github.com/scovetta/yasca)
YASCA (Yet Another Source Code Analyzer) analyzes Java, and C/C++ primarily, with other languages and JavaScript for security flaws and other bugs. Its’ creator, Michael Scovetta, aggregated many other popular static analysis tools and made it easy-to-integrate with a variety of other tools, including others on this list: FindBugs, CppCheck, and more. The tool was created in 2008 to help developers in looking for security bugs by automating part of their code review and finding the “low hanging fruit.”
For more info on Yasca, check out this [presentation](https://www.owasp.org/images/1/1f/NYPHP-Yasca.pdf) that the creator, Michael Scovetta gave at the NY PHP Conference in ’09.  The latest version, 3.0.4, was released in 2012. [See the GitHub repository](https://github.com/scovetta/yasca) here.
The Good:
- The fact that YASCA is an aggregated tool from other powerful tools, it took the best parts of each and combined for broader coverage
The Not-As-Good:
- Broader does not mean deeper: Keep in mind that this tool was built to look for low-hanging fruits like SQL injections and XSS, so be wary of missing more serious issues.
## JAVA
![source: owasp.org](http://checkmarx.x.sergata.com/wp-content/uploads/2014/11/lapse-300x186.png)source: owasp.org
### [OWASP LAPSE+](https://www.owasp.org/index.php/OWASP_LAPSE_Project)
The tool, which names stands for Lightweight Analysis for Program Security in Eclipse, is an [OWASP](https://www.checkmarx.com/glossary/owasp-top-10/) security scanner, developed as an Eclipse plugin, which detects vulnerabilities in Java EE Applications. LAPSE+ is liscenced under the GNU General Public License v.3 and was originally developed by Stanford University.
The Good:
- Tests validation logic without compiling your code
- Offers results as three steps: Vulnerability Source, Vulnerability Sink and Provenance Tracker
The Not-As-Good:
- Doesn’t identify  compilation errors
- Limited to Eclipse IDE’s only
- Project was taken over in early 2014 but no new version since 2012
## PHP
![source: rips-scanner.sourceforge.net/](http://checkmarx.x.sergata.com/wp-content/uploads/2014/11/RIPS-300x153.jpg)source: rips-scanner.sourceforge.net/
### [RIPS](http://rips-scanner.sourceforge.net/)
This tool, developed by Johannes Dahse, tokenizes and parses source code to detect sinks and other vulnerabilities including SQLi, XSS, RCE and more. Check out Johannes’ [presentation](https://websec.files.wordpress.com/2010/11/rips-slides.pdf) on the tool for further info.
The Good:
- Fast results with range of security controls
- Nice reporting with visualizations makes fixing vulnerabilities faster
The Not-as-Good:
- Can be limited with regards to data flow
- As of this year RIPS 0.5 development has been abandoned with a re-write of the tool on deck but still unavailable
### [DevBug](http://www.devbug.co.uk/)
Created by ethical hacker Ryan Dewhurst ([@ethicalhack3r](https://twitter.com/ethicalhack3r)) for his undergraduate thesis, DevBug is a very simple online PHP static code analysis tool. Written in JavaScript, it was designed to make SCA easy and pulls inspiration (as well as Taint Analysis data) from RIPS.
The Good:
- Easy to use with instant results
- Nice use of OWASP wiki page links for more info on any found vulnerability
The Not-As-Good:
- Simplistic and is only meant for light analysis
## C/C++
### [Flawfinder](http://www.dwheeler.com/flawfinder)
Designed to be simple and easy to use, FlawFinder reports well-known security issues in applications written in C, sorted by risk level. Developed by open-source and secure software expert David Wheeler, the tool itself is written in Python and uses a command line interface. FlawFinder is officially [CWE](http://cwe.mitre.org/)compatible.
The Good:
- Ability to check only the changes made to code for faster, more accurate results
- Long history, released in 2001 with consistent updates
The Not-As-Good:
- A number of false positives
- Requires Python 1.5
![source: code.google.com](https://www.checkmarx.com/wp-content/uploads/2014/11/cppeclipse-300x205.gif)source: code.google.com
### [CPPCheck](http://sourceforge.net/projects/cppcheck/)
This tool, available under a GNU General Public License, was developed to check non-standard code that compilers would normally not detect. Created by Daniel Marjamäki, CPPCheck offers a command line mode as well as a GUI mode and has a number of possibilities for environment integration.
The Good:
- Plugins and integrations for a number of IDEs: Eclipse, Hudson, Jenkins, Visual Studio.
- Daniel’s plan is to release a new version every other month or so, and he’s been keeping up with that goal.
- Available in many world languages, including English, Dutch, Finnish, Swedish, German, Russian, Serbian and Japanese.
The Not-As-Good:
- Doesn’t detect a large number of bugs (as with most of the other tools)
- Customization requires good deal of effort
- Results take longer than other tools
## Ruby on Rails
### [Brakeman](http://brakemanscanner.org/)
This tool, developed in 2010 by Justin Collins ([@presidentbeef](https://twitter.com/presidentbeef)) is specifically for finding vulnerabilities and security issues in Ruby on Rails apps at any development stage.
Brakeman is [used by](http://brakemanscanner.org/brakeman_users/) the likes of Twitter (where Justin is employed), GitHub, and Groupon to look for vulnerabilities.  Justin gave a talk at RailsConf 2012 that’s [worth watching](http://www.confreaks.com/videos/890-railsconf2012-keeping-rails-applications-on-track-with-brakeman) describing the value of using SCA early on and how Brakeman accomplishes that.
The Good:
- Easy setup and configuration and fast scans.
- Because it’s specifically built for Ruby on Rails apps, it does a great job at checking configuration settings for best practices.
- With the ability to check only certain subsets, each code analysis is able to be customizable to specific issues.
- The developer has been maintaining and updating the tool on a regular basis since its first release.
The Not-So-Good:
- Because of its suspicious nature, the tool can show a high rate of false positives
- As written on the tool’s [FAQ page](http://brakemanscanner.org/docs/faq/), just because a report shows zero warnings doesn’t mean your application is flaw-free; “There may be vulnerabilities Brakeman does not test for or did not discover. No security tool has 100% coverage.”

