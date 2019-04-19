# AngularJS + RequireJS - =朝晖= - 博客园
# [AngularJS + RequireJS](https://www.cnblogs.com/dhcn/p/7120767.html)
http://www.startersquad.com/blog/[AngularJS](http://lib.csdn.net/base/angularjs)-requirejs/
While delivering [software projects for startups](http://www.startersquad.com/startups/), we’ve come to love [angularjs](http://lib.csdn.net/base/angularjs). We’ve also come to struggle with clean modularity, both the parts that Angular does really well, and the parts that are somewhat missing. RequireJS does a great job where Angular leaves some things to be desired, but using them together is not entirely trivial. What follows is our take at the problem.
## Why?
Working with Angular you could worry about a good way to organize code. There are already great how-tos on that, check out [this mainly theoretical post](http://briantford.com/blog/huuuuuge-angular-apps.html) by Brian Ford and [this practical guide](http://cliffmeyers.com/blog/2013/4/21/code-organization-angularjs-javascript) by Cliff Meyers if you haven’t already. I’ll share the way I’m managing code in Angular applications with RequireJS.
Continue reading if you want to:
- stop worrying about including script tags in the right order when building Angular apps;
- to load your javascript asynchronously;
- to compile code into single minified js file;
## Who?
I assume that you already know what AngularJS is and that you’ve at least heard of AMD and RequireJS. To illustrate the approach I’ll first enable RequireJS for [Angular Seed](https://github.com/angular/ngseed) and explain process. Angular Seed structures code by splitting files by type and so will I. It’s also possible to apply this approach if you write modules by entities (you’ll see it from `app.controllers`module implementation).
## How?
### Angular Seed Project
Let’s check how Angular Seed structures code. Check out the example [in your browser](http://www.startersquad.com/examples/angularjs-requirejs-1/) or [on github](https://github.com/StarterSquad/startersquad.com/tree/master/examples/angularjs-requirejs-1) (copied from Seed):
- `app.js` file to bootstrap and set app config;
- actual implementation files – controllers, services, directives and filters;
- `index.html` with all script tags included in right order;
- or `index-async.html` that makes use of `angular-loader.js` and 3-rd party`$script` loader library to load dependencies asyncronously.
Let’s start the party.
### Add RequireJS
Checkout the example [in your browser](http://www.startersquad.com/examples/angularjs-requirejs-2/) or [on github](https://github.com/StarterSquad/startersquad.com/tree/master/examples/angularjs-requirejs-2).
#### Installing dependencies
I used [bower](http://bower.io/) to do this for me. See `bower.json` file:
{
  "name": "AngularJS + RequireJS Example",
  "version": "0.1",
  "main": "index.html",
  "ignore": [
    "**/.*",
    "libs"
  ],
  "dependencies": {
    "angular": "latest",
    "requirejs": "latest",
    "requirejs-domready": "latest"
  }
}
Put the `.bowerrc` file next to `bower.json`, run `bower install` and – poof, we have all we need under `libs` folder.
#### index.html
Destruction is a good start. Open Angular Seed’s `index.html` and remove all the `<script>` tags. Looks cleaner, doesn’t it? Now switch to creation mode and add single script before closing `</body>` that will load RequireJS and instruct it to look for config in `[js](http://lib.csdn.net/base/javascript)/main.js` with the [data-main attribute](http://requirejs.org/docs/api.html#data-main):
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>My AngularJS AngularJS + RequireJS App</title>
  <link rel="stylesheet" href="css/app.css">
</head>
<body>
  <ul class="menu">
    <li><a href="#/view1">view1</a></li>
    <li><a href="#/view2">view2</a></li>
  </ul>
<div data-ng-view></div>
<div>Angular Require seed app: v<span app-version></span></div>
<script src="lib/requirejs/require.js" data-main="js/main.js"></script>
</body>
</html>
That’s all there’s to it. You can close `index.html` now, as there is nothing more we need to add to it.
#### main.js
Time to setup RequireJS config.
require.config({
  // alias libraries paths
    paths: {
        'domReady': '../lib/requirejs-domready/domReady',
        'angular': '../lib/angular/angular'
    },
    // angular does not support AMD out of the box, put it in a shim
    shim: {
        'angular': {
            exports: 'angular'
        }
    },
    // kick start application
    deps: ['./bootstrap']
});
What just happened? In [paths](http://requirejs.org/docs/api.html#config-paths) we set aliases for the libraries and plugins used, then we defined that angular should be [shimmed](http://requirejs.org/docs/api.html#config-shim) and that`bootstrap.js` should be loaded to start the application.
#### bootstrap.js
We’re bootstrapping angular manually now, that’s what bootstrap.js is for. Note that you don’t need `ng-app` in your html anymore. Also `routes.js`, which contains angular routes configuration is included into dependencies list.
Note that in this require module we almost have no use of asynchronous loading and we’ll always have chain of angular -> app -> routes, as they depend on each other: angular needs to be present on a page before setting up application module, which is required to exist when defining routes config.
/**
 * bootstraps angular onto the window.document node
 */
define([
    'require',
    'angular',
    'app',
    'routes'
], function (require, ng) {
    'use strict';
    require(['domReady!'], function (document) {
        ng.bootstrap(document, ['app']);
    });
});
We use [domReady](https://github.com/requirejs/domReady) RequireJS plugin to make sure that DOM is ready when we start the app. Note that before doing so we’re loading the `app.js`dependency, in there the main application is defined.
#### app.js
`app.js` wraps the definition of the top-level app module and loads the dependencies of its submodules.
define([
    'angular',
    './controllers/index',
    './directives/index',
    './filters/index',
    './services/index'
], function (ng) {
    'use strict';
    return ng.module('app', [
        'app.services',
        'app.controllers',
        'app.filters',
        'app.directives'
    ]);
});
We agreed to have 4 modules by files types: controllers, directives, filters, services – we require these modules to be loaded before defining the main module.
#### routes.js
Top level routes definition lives here. It is also possible to have modules to set up their own routes (this case is omitted for now in favour of simplicity).
define(['./app'], function (app) {
    'use strict';
    return app.config(['$routeProvider', function ($routeProvider) {
        $routeProvider.when('/view1', {
            templateUrl: 'partials/partial1.html',
            controller: 'MyCtrl1'
        });
        $routeProvider.when('/view2', {
            templateUrl: 'partials/partial2.html',
            controller: 'MyCtrl2'
        });
        $routeProvider.otherwise({
            redirectTo: '/view1'
        });
    }]);
});
#### Module structure
A module consists of 3 parts:
- definition;
- component;
- loader.
Let’s use the `app.controllers` module as example.
#### module definition (`controllers/module.js`)
It’s just like top level `app.js`: it defines a module.
define(['angular'], function (ng) {
    'use strict';
    return ng.module('app.controllers', []);
});
This file will be used by the module components to attach themselves to (see next section).
#### module loader (`controllers/index.js`)
That’s just an empty define block with all module components included. You don’t need to mention `module.js` here as it’s already required by components. Loader is included as dependency of top level `app` module. And that’s actually how RequireJS knows about files to load.
define([
    './my-ctrl-1',
    './my-ctrl-2'
], function () {});
#### module components (`controllers/my-ctrl-1.js`)
In the case with the `app.controllers` module it’ll be controllers. Example of controller wrapped in define is:
define(['./module'], function (controllers) {
    'use strict';
    controllers.controller('MyCtrl1', [function ($scope) {}]);
});
Note that we used reference to `./module.js` to attach component to its module.
## Conclusion
That’s it. Now you have working Angular application powered by RequireJS. You can enjoy the power of not tracking the order of your scripts anymore and you get some powerful minification tooling to boot.
In next articles I’ll show you how to test this application properly, how to compile it into single file and automate workflows with grunt. All this is already enabled in [StarterSquad Angular + Require Seed](https://github.com/StarterSquad/angularjs-requirejs-seed/) check it out if you can’t wait (I’m a slow typist).
## About StarterSquad
StarterSquad is a community of distributed development teams consisting of freelance developers. We specialize in [startups](http://www.startersquad.com/tag/startups/) and [lean innovation](http://www.startersquad.com/tag/lean-innovation/). If you want to know more, read [more about how we work](http://www.startersquad.com/what-makes-startersquad-different/) and if you need any help with Angular, checkout my [team](http://bit.ly/spotless-dmitry).

