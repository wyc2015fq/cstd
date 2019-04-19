# Using RequireJS in AngularJS Applications - =朝晖= - 博客园
# [Using RequireJS in AngularJS Applications](https://www.cnblogs.com/dhcn/p/7120763.html)
http://www.sitepoint.com/using-requirejs-[AngularJS](http://lib.csdn.net/base/angularjs)-applications/
While writing large [JavaScript](http://lib.csdn.net/base/javascript) applications, one of the simplest things one can do is divide the code base into several files. Doing this improves maintainability of the code but increases the chances of missing or misplacing the script tag on your main HTML document. Keeping track of the dependencies becomes difficult as the number of files increases. This issue persists in large [angularjs](http://lib.csdn.net/base/angularjs) applications as well. We have a number of tools in place that take care of loading dependencies in the application.
In this article, we will see how to use RequireJS with AngularJS to make the job of loading dependencies simpler. We’ll also examine how to use Grunt to generate combined files containing the RequireJS modules.
## A Brief Introduction to RequireJS
RequireJS is a [javascript](http://lib.csdn.net/base/javascript) library that helps in lazily loading JavaScript dependencies. Modules are just JavaScript files with some RequireJS syntactic sugar in them. RequireJS implements [Asynynchronous Modules](http://wiki.commonjs.org/wiki/Modules/AsynchronousDefinition) specified by CommonJS. RequireJS offers simple APIs to create and refer to modules.
RequireJS needs a main file that contains the basic configuration data such as paths to modules and shims. The following snippet shows the skeleton of a `main.[js](http://lib.csdn.net/base/javascript)` file:
```java
require.config({
  map:{
    // Maps
  },
  paths:{
    // Aliases and paths of modules
  },
  shim:{
    // Modules and their dependent modules
  }
});
```
All modules in the application need not be specified in the paths section. Others can be loaded using their relative paths. To define a module, we need to use the `define()` block.
```java
define([
  // Dependencies
], function(
  // Dependency objects
){
  function myModule() {
    // Can use the dependency objects received above
  }
  return myModule;
});
```
A module may have some dependent modules. In general, an object is returned at the end of a module, but it is not mandatory.
## Angular’s Dependency Injection vs RequireJS Dependency Management
One of the common questions that I hear from Angular developers regards the difference between Angular’s dependency management and that of RequireJS. It is important to remember that the purpose of both the libraries is totally different. The dependency injection system built into AngularJS deals with the objects needed in a component; while dependency management in RequireJS deals with the modules or, JavaScript files.
When RequireJS attempts to load a module, it checks for all dependent modules and loads them first. Objects of loaded modules are cached and they are served when same modules are requested again. On the other hand, AngularJS maintains an injector with a list of names and corresponding objects. An entry is added to the injector when a component is created and the object is served whenever it is referenced using the registered name.
## Using RequireJS and AngularJS together
The [downloadable code](https://github.com/jsprodotcom/source/blob/master/AngularRequireJsSample.zip) included with this article is a simple application containing two pages. It has the following external dependencies:
- RequireJS
- jQuery
- AngularJS
- Angular Route
- Angular Resource
- Angular UI ngGrid
These files should be loaded directly on the page in the order they are listed here. We have five custom script files containing code of the required AngularJS components. Let’s take a look at how these files are defined.
## Defining AngularJS Components as RequireJS Modules
Any AngularJS component consists of:
- A function definition
- Dependency Injection
- Registering to an Angular module
Out of the above three tasks, we will perform the first two tasks inside the individual modules, while the third task will be performed in a separate module that is responsible for creating the AngularJS module.
First, let’s define a config block. The config block doesn’t depend on any other blocks, and returns the config function in the end. But, before we load config module inside another module, we need to load everything that is needed for the config block. The following code is contained in `config.js`:
```java
define([],function(){
  function config($routeProvider) {
    $routeProvider.when('/home', {templateUrl: 'templates/home.html', controller: 'ideasHomeController'})
      .when('/details/:id',{templateUrl:'templates/ideaDetails.html', controller:'ideaDetailsController'})
      .otherwise({redirectTo: '/home'});
  }
  config.$inject=['$routeProvider'];
  return config;
});
```
Notice the way dependency injection is performed in the above snippet. I used `$inject` to get the dependencies injected as the config function defined above is a plain JavaScript function. Before closing the module, we return the config function so that it can be sent to the dependent module for further use.
We follow the same approach for defining any other type of Angular component as well, as we don’t have any component specific code in these files. The following snippet shows the definition of a controller:
```java
define([], function() {
  function ideasHomeController($scope, ideasDataSvc) {
    $scope.ideaName = 'Todo List';
    $scope.gridOptions = {
      data: 'ideas',
        columnDefs: [
         {field: 'name', displayName: 'Name'},
         {field: 'technologies', displayName: 'Technologies'},
         {field: 'platform', displayName: 'Platforms'},
         {field: 'status', displayName: 'Status'},
         {field: 'devsNeeded', displayName: 'Vacancies'},
         {field: 'id', displayName: 'View Details', cellTemplate: '<a ng-href="#/details/{{row.getProperty(col.field)}}">View Details</a>'}
        ],
        enableColumnResize: true
        };
    ideasDataSvc.allIdeas().then(function(result){
      $scope.ideas=result;
    });
  }
  ideasHomeController.$inject=['$scope','ideasDataSvc'];
  return ideasHomeController;
});
```
The Angular module for the application depends on each of the modules defined up to this point. This file gets objects from all other files and hooks them with an AngularJS module. This file may or may not return anything as the result of this file, the Angular module can be referenced from anywhere using`angular.module()`. The following code block defines an Angular module:
```java
define(['app/config',
  'app/ideasDataSvc',
  'app/ideasHomeController',
  'app/ideaDetailsController'],
  function(config, ideasDataSvc, ideasHomeController, ideaDetailsController){
    var app = angular.module('ideasApp', ['ngRoute','ngResource','ngGrid']);
    app.config(config);
    app.factory('ideasDataSvc',ideasDataSvc);
    app.controller('ideasHomeController', ideasHomeController);
    app.controller('ideaDetailsController',ideaDetailsController);
});
```
The Angular application cannot be bootstrapped using the `ng-app` directive as the required script files are loaded asynchronously. The right approach here is to use manual bootstrapping. This has to be done in a special file called `main.js`. This needs the file defining the Angular module to be loaded first. The code for this file is shown below.
```java
require(['app/ideasModule'],
  function() {
    angular.bootstrap(document, ['ideasApp']);
  }
);
```
## Configuring Grunt to Combine RequireJS Modules
While deploying a JavaScript heavy application, the script files should be combined and minified to optimize download speed of the script files. Tools like Grunt come handy to automate these tasks. It has a number of tasks defined to make any process of front-end deployment easier. It has a task, grunt-contrib-requirejs for combining RequireJS files modules in the right order and then minifying the resulting file. Just like any other grunt task, it can be configured to behave different for each stage of deployment. The following configuration can be used in the demo application:
```java
requirejs: {
  options: {
    paths: {
      'appFiles': './app'
    },
    removeCombined: true,
    out: './app/requirejs/appIdeas-combined.js',
    optimize: 'none',
    name: 'main'
  },
  dev:{
    options:{
      optimize:'none'
    }
  },
  release:{
    options:{
      optimize:'uglify'
    }
  }
}
```
This configuration would produce an unminified file when Grunt is run with the dev option, and a minified file when grunt is run with the release option.
## Conclusion
Managing dependencies becomes challenging when the size of the application grows beyond a certain number of files. Libraries like RequireJS make it easier to define the dependency and not worry about the order of loading of the files. Dependency management is becoming an integral part of the JavaScript applications. AngularJS 2.0 is going to have built-in support for AMD.

