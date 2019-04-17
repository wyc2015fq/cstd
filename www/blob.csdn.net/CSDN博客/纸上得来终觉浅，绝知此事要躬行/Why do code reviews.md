# Why do code reviews? - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 09:27:06[boonya](https://me.csdn.net/boonya)阅读数：715标签：[sonatype																[nexus																[github																[branch																[jira](https://so.csdn.net/so/search/s.do?q=jira&t=blog)
个人分类：[Wise reading](https://blog.csdn.net/boonya/article/category/1628867)






#### Thanks for Jamie Whitehouse sharing us about his intelligent views on the code reviews.

![Author, Jamie Whitehouse](http://www.sonatype.org/nexus/content/uploads/2015/04/JamieWhitehouse-590.jpg)




Jamie Whitehouse is Coffee Boy at Sonatype, doing what it takes to make our teams and product successful. Despite being from Ontario, Canada, he doesn't follow hockey, but does enjoy building with Lego.


From:[http://www.sonatype.org/nexus/2015/04/01/why-do-code-reviews-by-jamie-whitehouse/?utm_campaign=20150408%20Newsletter%20Nexus%20Now%20April&utm_medium=email&utm_source=Eloqua](http://www.sonatype.org/nexus/2015/04/01/why-do-code-reviews-by-jamie-whitehouse/?utm_campaign=20150408%20Newsletter%20Nexus%20Now%20April&utm_medium=email&utm_source=Eloqua)





*This article is part of a continuing series from the engineering team, talking about the process and tools they use in the distributed work environment. [TheNEXUS Community members](http://www.sonatype.org/nexus/members-only/) have immediate access to the live online [videos recorded for this series](http://www.sonatype.org/nexus/members-only/video-gallery-2/inside-the-sonatype-engineering-machine-the-process-and-the-tooling/).*

# Why Do Code Reviews?


There is [a lot of literature](https://www.google.com/search?q=why%20code%20review) on the value of code reviews.  In particular the benefits to our distributed
 agile team are:
- Building knowledge throughout the team
- Reducing functional silos
- Reducing software entropy

## Build knowledge throughout the team


When a new developer comes on to the team they get a mid-level overview of the system.  Nothing beats working with your team on a feature to really understand how things work.  In a remote organization we don’t have the water cooler or lunch time discussions
 that help to spread information and knowledge through a team.  Putting code ‘out there’ for others to review facilitates the spread of information.  This is often two way – the submitter getting general design feedback and specifics from those who know the
 area well; the reviewers getting to know more about the new feature and the existing code it’s interacting with.

## Reduce functional silos within the team


We have a multifunctional team where developers are capable of working with browser tech and server-side tech.  This allows us to move stories forward without a dependency on any single persons skills.  Reviews help to reduce the knowledge silos, where specific
 people know an area of functionality so well they become a dependency.  We encourage having two reviewers, increasing the surface area of knowledge and stimulating interest for future work.


Not only is code reviewed, we follow the same processes to review documentation.  In this way developers are more aware of the tone and written style of the documentation, and are capable of contributing content in order to move stories forward.

## Reduce software entropy


A review is a great time to validate that the technical design fits our short and long term objectives, and make any improvements before it settles into the release code.  We use code reviews to help ensure the code is maintainable by having a second (and third)
 person thinking about if they can makes sense of the code and would it be obvious to them in three months from now.  The reviewer doesn’t only consider the feature code, the tests are equally important.  A reviewer checks that the appropriate tests are in
 place, at the appropriate level.  This is also an opportunity to point out duplication or similarities within the system and make improvements.

# Anatomy of a code review


At Sonatype we have guidelines on the review process and expectations.  These are not hard-and-fast rules.  We encourage the teams to consider what is best for them, and for each review.  Aside from trivial changes (e.g. documentation typo), everything is reviewed
 before committed to a release branch.


We use JIRA to track our work and GitHub to track our code and reviews via pull requests (PRs).  The following is a typical review cycle, using examples from various reviews.

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-01.png)

## Develop on a branch, review pull requests


Use a branch to develop a piece of functionality that gets a task or story done.  When the code is ready for review open a pull request.  We use the term ‘[feature
 branch](http://martinfowler.com/bliki/FeatureBranch.html)’, but the change is often smaller than an entire feature.  Having small, logically isolated functionality, makes reviews easier to digest and quicker to approve, hence the code being integrated sooner.


In this example the branch is NEXUS-8301-checksum-validation and the PR opened is #1148.

## Supply context


Make it as easy as possible for a reviewer to find information and understand the state of the request.


Cross reference between tools.  Link from the PR to the JIRA issue:

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-02.png)


Link from the JIRA issue to the PR:

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-03.png)


Include screenshots or screencasts so that reviewers don’t have to guess at the UI impact or run the branch locally to see it working.

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-04.png)

## Seek approval from the bots


Validate the branch builds cleanly within our continuous integration environment so that everyone knows that it compiles and all quality checks are passing.  This is a vote of confidence that when it’s merged to the release branch there’s likely to be no disruption.


A Bamboo feature known as [plan branches](http://blogs.atlassian.com/2012/04/bamboofeature-branch-continuous-integration-hg-git/) ensures our feature branches use the same CI configuration
 as our release branches.


Reference the CI build in the PR, making it visible to reviewers.

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-06.png)

## Announce the review


Let the team know the review is ready by moving the JIRA issue to the review state.

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-07.png)


Prompt people as necessary through various mediums like GitHub mentions, stand-ups, and HipChat.

## Iterate on peer feedback


Reviewers comment within the PR or a synchronous conversation via voice or HipChat as appropriate.


Notice how Kelly referenced the commit that changed the code based on Chris’ feedback, making it easy to follow-up on.

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-09.png)


We use the variation of roman voting that is common in OSS development to seek approval for code changes: +1 to support; +0 okay with change, may not be knowledgeable in the area or tech; -1 to disapprove, a veto that needs resolution before being committed
 to the release branch.  We like to have two people review code, but this isn’t always appropriate and forcing it can be counter-productive.

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-10.png)




Having new teammates review code is important but they may not feel confident in giving a stamp of approval.  Using +0 is appropriate to let the team know that they’ve been lurking.

# Incorporate into release branch


To [merge](http://git-scm.com/book/en/v2/Git-Branching-Basic-Branching-and-Merging) or [squash](http://git-scm.com/book/en/v2/Git-Tools-Rewriting-History),
 that is the question.  Depending on the size of the PR, the changeset may be squashed before committing to the release branch.  This is useful to reduce the work-in-progress commits that [developers
 are fond of](https://github.com/sonatype/nexus-oss/pull/1136).


Be sure to clean-up the branch to reduce the clutter.  The PR is always available for historical purposes and can be [checked
 out locally](https://help.github.com/articles/checking-out-pull-requests-locally/) like a normal branch.

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-11.png)


When squashing, reference the PR number in the squashed commit.  This makes it easy to find the review when looking at the change history on the release branch.  Using these[GitHub
 references](https://help.github.com/articles/writing-on-github/#references) also causes GitHub to notice that the PR has been merged.

![Why Do Code Reviews?](http://www.sonatype.org/nexus/content/uploads/2015/04/WhyDoCodeReviews-12.png)

# An example code review

[NEXUS-8236](https://issues.sonatype.org/browse/NEXUS-8236) and the corresponding pull request [#1134](https://github.com/sonatype/nexus-oss/pull/1134) is
 a typical example.  Notice:
- The early feedback from Chris while the PR had the WIP label.
- Tamas provides a second vote once the PR is ready for review (the label change from WIP to REVIEW).
- Kelly is diligent about removing the branch once merged.

# Where’s the Testing?


You’ll notice that there was no mention of testing during the code review.  This is intentional.  We separate code review from testing in our workflow to provide more visibility of blocking states and allow the team to use the appropriate criteria and process
 for each state.


Once the code is merged the issue is now ready to be tested.  In summary the testing involves:
- Testing by someone who didn’t do the work
- Verifying the work was done
- Ensuring acceptance criteria are met


This doesn’t stop some of these activities from happening informally in the review state, while the code is on a branch.  This may happen for larger reviews, or when a reviewer wants to get an understanding of the functionality to see how the code relates to
 that (frequent with new teammates).




The following two tabs change content below.](https://so.csdn.net/so/search/s.do?q=branch&t=blog)](https://so.csdn.net/so/search/s.do?q=github&t=blog)](https://so.csdn.net/so/search/s.do?q=nexus&t=blog)](https://so.csdn.net/so/search/s.do?q=sonatype&t=blog)




