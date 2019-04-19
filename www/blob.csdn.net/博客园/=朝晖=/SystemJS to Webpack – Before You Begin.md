# SystemJS to Webpack – Before You Begin - =朝晖= - 博客园
# [SystemJS to Webpack – Before You Begin](https://www.cnblogs.com/dhcn/p/7642618.html)
http://angularfirst.com/systemjs-to-webpack-before-you-begin/
This is a primer discussing why to move from SystemJS to webpack in your Angular project. The post also describes some of the hurdles you may run into with this effort.
While it doesn't go into specific webpack configuration, this article aims to provide an overview for someone who has heard of webpack but doesn't quite understand why or how to get started. Specific details configuring webpack with ASP.NET Core are coming in future posts.
## SystemJS – Starting Point
There are several posts on this blog with [examples using SystemJS](http://angularfirst.com/your-first-angular-2-asp-net-core-project-in-visual-studio-code-part-1/) to [mimic Node.js module resolution](http://angularfirst.com/using-ecmascript-2015-modules-in-typescript-with-systemjs/) in the browser. While TypeScript understands how to resolve types by npm package name during development, SystemJS fills this need in the browser by mapping package names to specific script files within the npm packages. This is an effective and minimal way to setup an Angular 2 application to load its dependencies from npm and get going quickly.
After starting with SystemJS, applications (even small ones) quickly grow to contain hundreds of ES2015 modules that are then downloaded by the browser to execute the application. This is probably not a problem during development when the browser and the server are on the same machine.
At some point, however, you may want to host your application for others to see. The performance hit of loading this many modules becomes noticeable. This situation leads to creating some type of build combining these hundreds of requests into only a handful.
Whereas SystemJS manipulates the browser to understand node module resolution, webpack (and similar build tools) manipulate *the code* to accommodate the browser's needs. Whether SystemJS or webpack, you continue to code the way that makes you the most productive with as many ES2015 modules in as many files as make you happy. Unlike SystemJS, webpack provides the browser with a streamlined payload to load the application as efficiently as possible.
## Destination Webpack
Webpack at its core is a JavaScript module bundler meaning it takes JavaScript modules from separate files and combines them into one file. Webpack can be extended, though, by using *loaders* which allow you to bundle other types of files. So, while webpack doesn't understand TypeScript natively, it processes `.ts` files by using a loader.
Webpack has another extensibility mechanism called *plugins*. Where loaders handle specific file types, plugins process the contents of the loaded files. Plugins, for example, are responsible for minifying code where variable names are shortened and whitespace is removed.
One aspect of webpack that is different from a tool such as grunt or gulp is that it is more *declarative* meaning you configure an object literal and that object defines what should happen when you run the webpack command. The declarative nature of webpack ideally requires less configuration code and should lead you into a build that is more optimized than if you had to wire the pieces up yourself.
There are two other npm packages that carry the webpack name but are not the bundler, itself. These packages work with the bundler to improve the development experience. They are `webpack-dev-server` and `webpack-dev-middleware`.
The `webpack-dev-server` npm package is a lightweight development server based on [Node.js](https://nodejs.org/en/) and [Express](http://expressjs.com/). It has all the basic development server features and includes *hot module replacement*. Hot module replacement (HMR) is a process where the development server uses a WebSocket connection to replace the code of a given module in the browser when it has changed in the development environment. This does not require a full page refresh to reflect the updates making it seamless to view your changes in the browser during development.
The `webpack-dev-middleware` npm package is a little more esoteric. It runs as an in-memory store containing the build output as opposed to writing the output to disk. This can decrease the time between when you modify a file and when you can see the changes running in the browser.
## Configuration Troubles
Currently in early 2017, webpack is moving from version 1 to version 2. This can lead to incompatibilities in the webpack tool chain. When configuring a webpack build yourself, expect to find out-of-date examples and see errors processing your build.
Not all loaders and plugins support both webpack 1 or webpack 2 and you may find yourself trying different versions of these npm packages with different versions of webpack. Throw in the fact that TypeScript is adding significant new features through all of this and it's a recipe for configuration troubles.
This isn't meant to scare you off, just be prepared. Expect to search through the GitHub issues of some of these frameworks looking for answers or expect to post issues yourself. It's not always clear which combination of settings are correct but the webpack community is by-and-large responsive and committed to this tooling.
This is an example of an issue you may find. While the TypeScript compiler allows comments in the `tsconfig.json` file as of version 1.8, some of the TypeScript webpack loaders will blow up if there are comments in this file. There is no helpful error message, it just throws an error that a property is not defined on a null reference.
It's likely these issues are resolved over time, but you should understand what you are up against.
## Replacing SystemJS
Now that you know about webpack's advantages and potential pitfalls, what are the general steps required to replace SystemJS? This is a summary:
- Move polyfill scripts from index.html into the webpack bundle
- Compile and bundle the TypeScript files
- Move any external component HTML templates and stylesheets inline
- Add a reference to index.html for the new bundled resource
### Move polyfill scripts from index.html into the webpack bundle
Many Angular tutorials instruct you to put polyfill dependencies directly in the `index.html` (or alternatively named default HTML) file. This approach works and will continue to work with webpack but you should consider moving these dependencies into the webpack configuration.
First, including these dependencies into a bundled request improves performance. Second, having all your script dependencies in one place decreases the time to find a given dependency – thus increasing maintainability. Third, webpack resolves dependencies through node module resolution meaning you add the package name to the configuration and you're done. Using SystemJS, you often must identify the correct script to load from within the npm package.
### Compile and bundle the TypeScript files
While you could continue to compile TypeScript with the TypeScript compiler, the idea is to move everything into webpack. There are several loaders that handle TypeScript files and are generally straightforward to configure. A popular TypeScript loader in the Angular community now appears to be [awesome-typescript-loader](https://www.npmjs.com/package/awesome-typescript-loader).
### Move external component HTML templates and stylesheets inline
This one is Angular-specific. When you have components with external templates and/or stylesheets, Angular loads these files as separate requests. By using a loader like the [angular2-template-loader](https://www.npmjs.com/package/angular2-template-loader), the build now can combine the contents of the external templates and stylesheets into the component script itself.
> 
Note: Understand that by moving the templates and stylesheets inline, setting the `@Component` directive's `moduleId` property is unnecessary. Leaving the `moduleId` property set in the component can lead to issues. While the available tooling should most likely handle this, you may find you must account for this yourself by removing the settings manually or creating automation.
### Add a reference to index.html for the new bundled resource
Finally, once you define the file(s) that webpack outputs, you must reference the new bundled file(s) in your main HTML file. As you may have guessed, there are even webpack plugins to handle this as well based off the webpack configuration (see [html-webpack-plugin](https://www.npmjs.com/package/html-webpack-plugin)).
### Additional Configuration
While the previous sections listed the necessary pieces of a basic webpack build. You have so many more options available to you to optimize your code. Some build steps to consider are: [Ahead-of-time (AOT) compilation](https://angular.io/docs/ts/latest/cookbook/aot-compiler.html), using the wwwroot folder in ASP.NET Core, tree-shaking, running tests, and using the webpack development server with hot module replacement.
## Tackling the Beast
So, you still want to get going with webpack? Where do you begin? There are many projects on Github and other sites that provide insight into how this tool works. It appears to still be hit or miss as far as quality. Looking at it from an ASP.NET developer's perspective, the guidance options are fewer. With that said, here are some links to get started:
- [Webpack 1 – Getting Started](http://webpack.github.io/docs/tutorials/getting-started/)
- [Webpack 2 – Getting Started](https://webpack.js.org/get-started/)
- [Angular Webpack Documentation](https://angular.io/docs/ts/latest/guide/webpack.html)
- [ASP.NET Core JavaScript Services](https://github.com/aspnet/JavaScriptServices)
None of these resources paint the full picture but they provide a good overview. In addition to these general resources, visit the documentation for the individual loaders and plugins to get details regarding compatibility with other packages.
You should consider creating your own webpack build from scratch. There are advantages to doing it this way. If you like knowing how these tools work, this is a rewarding way to learn, albeit a time-intensive one.
Focus on one piece of the build at a time. It's easier to identify which pieces of the build are failing and you can more easily investigate or replace that specific loader or plugin to compare results. Once you stabilize one part, move on to the next until you have the build that you want.
Start with the pieces outlined in the [Replacing SystemJS](http://angularfirst.com/systemjs-to-webpack-before-you-begin/#replacing-systemjs) section and continue from there.
## More on the Way
You've learned why you should consider moving to webpack and the general approach to creating a webpack-based build. Again, this is just a primer. The plan is to ultimately provide you with a webpack configuration that works well with Angular and with ASP.NET Core. You will see more posts about webpack here soon.
For now, what are your favorite webpack resources? Did you build a template that others may find useful? Please share in the comments.

