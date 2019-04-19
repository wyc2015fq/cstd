# Webpack vs Browersify vs SystemJs for SPAs - =朝晖= - 博客园
# [Webpack vs Browersify vs SystemJs for SPAs](https://www.cnblogs.com/dhcn/p/7642621.html)
https://engineering.velocityapp.com/webpack-vs-browersify-vs-systemjs-for-spas-95b349a41fa0
Right now, there are at [least 8](https://www.slant.co/topics/3900/~frontend-javascript-module-bundlers) powerful open source Javascript bundlers; a few years ago there were only a couple. In building a large application at [Velocity](https://velocityapp.com/) , I ultimately decided to review the following three build systems: [Webpack](https://github.com/webpack/webpack), [Browserify](http://browserify.org/), [Systemjs](https://github.com/systemjs/systemjs). After analysing the three, I ultimately switched bundlers and it has saved us days of development time.
A Brief Comparison
A typical bundler will import application source code and it’s dependencies through an entry point. This entry point allows us to import only the relevant files and included dependencies into our final bundle and concatenate and minify them to make them available to use in the browser. Each bundler does this differently, but in the end all become single javascript file or group of files to serve when necessary. In the following review, I considered the speed of the watch task (compiling the application on the fly after changing the code), the ease of setup/configuration, size of the bundle, the flexibility of the builder to customise the environment etc, and time it takes to build. Keep in mind, the build times of these bundlers vary significantly in configuration and project size.
Browserify
Browserify is perhaps the simplest bundler because it uses NodeJS require syntax to import javascript. It does not deal with any other assets other than pure javascript modules. You can also bundle your typescript/es6 app with Browserify plugins. However, you’ll definitely need to add a task manager like gulp or grunt for a production ready application as you’ll want to also hash or bundle file, add environments, regex replaces, further minification and asset management. Below is a simple Browserify gulp task that takes in a set of Browserify settings and entry point and outputs a bundle.js file. Gulp handles where the files go:
While parts of the gulp task are convoluted, the browserify part of the configuration is only a couple of lines. In comparison to the others, I would use browserify for javascript/typescript projects that do not require a lot of asset management and aren’t really complicated or large.
In review:
*The Speed of Browserify:*
- Building for Production: I’d say Browserify is almost on par with Webpack and SystemJs. However, it only bundles code, not assets. Also, the transforms can really slow down the builds.
- Watching — Browserify out of the box does not watch well, it’s very slow. You need to add [watchify](https://github.com/substack/watchify) to incrementally watch your code changes. If you have any transforms this is still at least 5–6 seconds for a large application. You can configure [hot module reloading](https://github.com/AgentME/browserify-hmr) (the loading of a small module or part of your code, instead of the entire code), but there aren’t many examples how to get the best out of this, nor is it widely used or integrated into the core like webpack.
- Verdict:* Good* for pure Javascript applications, not Typescript or any apps that require a transforms.
*Setup:*
Browserify is by far the simplest setup in comparison to SystemJs or Webpack. There is still a bit of magic in that everything seems to function like NodeJs, but otherwise it does the job a bundler is supposed to do.
Verdict: Easiest to Setup
*Bundle Size:*
Browserify offers no built-in tree shaking or minification plugins, thus it’s bundle is quite large and you’ll need to use plugins or write a lot of gulp tasks.
Verdict: Large bundle, but plain javascript, you’ll have to figure it out yourself.
*Flexibility*:
The flexibility and customisability of Browserify is almost as good as Webpack, in that the plugins and easy integration with task managers, but the lack of asset management and the poor performance of plugins really make it difficult to leverage the power of the build system.
Verdit: Good, but not top dog.
### SystemJs
Like Browserify, SystemJs uses the require syntax, but it is more compatible with other times of files. On [GitHub](https://github.com/systemjs/systemjs), it calls itself the ‘universal loader’, in that it loads ES6 modules, AMD, CommonJS and global scripts in the browser and NodeJS. Meaning, it has a lot more built-in support for including different types of files than Webpack or Browserify. However, this comes at a cost. You have to directly tell SystemJs where to look for its dependencies and you end up with a config file like so:
Above is an Angular 2 SystemJs example. Including all your dependencies one by one is not ideal for large application, thus I recommend that you’d only want to use SystemJs for smaller, more diverse projects and save yourself sometime not having to write out this configuration.
In Review:
*The Speed of SystemJs:*
- Builds — Despite having to be able to load in any module type, SystemJs is surprisingly fast for production builds because it does one job very well. I’d say it’s faster than Browserify a lot of the time.
- Watching — For watching, it is also quite snappy and no magic happens under the hood. In some cases it’s faster than Browserify for large applications. You can also configure relevant [hot module reloading](https://github.com/capaj/systemjs-hot-reloader) with SystemJs as well, which is awesome and there are lots of Angular 2 examples online on how to do this.
Verdict: Fast, but not the fastest.
*Setup:*
The setup is probably the most tedious and also maintaining the SystemJs config is also quite difficult as you have to manually add dependencies and make sure your file paths don’t change.
Verdict: Simple, yet tedious and manual.
*Bundle Size:*
SystemJs seems less mysterious when bundling and the size isn’t really different than browserify because there are few to no additional add-ons that can do ‘magical’ things for you.
Verdict: Like Browserify, know your minification and tree-shaking gulp/node plugins.
*Flexibility*:
SystemJs is not as flexible or customisable as Browserify or Webpack.
Verdict: Do everything separately with Gulp.
### Webpack
Webpack treats everything as a module. Your css, html, javascript, fonts, images. Anything and everything can be inlined into your code if you desire. In addition to making everything a module, it also allows to you specify how you load these modules with ‘loaders’ and bundle them together into your bundle or index.html. Because webpack can bundle your entire app together , it makes tree shaking and optimising your code easy. Below, is an example from the [Webpack Angular 2 starter](https://angular.io/docs/ts/latest/guide/webpack.html). The Angular team has also recently switched to Webpack from SystemJs in their [angular-cli](https://github.com/angular/angular-cli). Below, is a sample webpack configuration file:
In Review:
*The Speed of Webpack:*
- Builds — Webpack’s build time directly depends on the configuration, but out of the box it is really fast, despite also including html and styles into the bundling process.
- Watching — For watching, it is also quite snappy. Many misconfigure webpack to run production build during its watches, thus it is important to read the documentation and make sure to only use webpack.optimize for production builds, not development. You can also configure relevant [hot module reloading](https://github.com/capaj/systemjs-hot-reloader) and with React you can hot reload your state — keeping the state of your application as you develop, which is awesome!
Verdict: Fast, configurable, and ready for production use.
*Setup:*
The first-time setup is difficult to grasp, with terms like ‘loaders’, but after understanding such and how the bundling works, setting up other webpack projects is easy. You won’t even need gulp. Just bundle everything with webpack and some npm commands. In the end, you’ll end up writing less code than gulp or any other bundler.
Verdict: Hard to understand at first, but well worth it.
*Bundle Size:*
Because of webpack’s built-in optimisation plugins it does an amazing job and getting rid of unused code and tree shaking.
Verdict: The best for production builds as it has built-in de-duping, uglifying, and tree-shaking (2.0).
*Flexibility*:
Webpack is one of the most flexible bundlers because there are loaders pretty much for anything — so you can shim or include any type of file you want. It’s more flexible than browersify in that you can decide more entry points and support different types of assets.
Verdict: The most flexible and powerful of the 3, but again learning how each part of it works is hard.
### Conclusion
#### I recommend using Webpack 1 for bundling large web applications.
Webpack is also my first choice for single page web applications. At [Velocity](https://velocityapp.com/), we switched from Browserify to Webpack and reduced our bundle size by 10–20%, watch times by seconds, and builds by minutes. Also we’ve reduced the number of gulp tasks in our builds and decreased the complexity.
Overall, webpack is the clear winner when it comes to speed and flexibility. Webpack 2 offers more features and performance outside of the box, it is in beta and not ready for production as some of the sourcemapping for scss is broken and not all loaders support it.
Browserify is good for quick small applications. SystemJS is great for small to medium sized applications that rely on a variety of different dependencies. However, with Webpack you can support all of what SystemJs or Browserify support with external loaders.
Please share your thoughts on my comparison and code samples. Also, if you know any speed or improvements that I can make to my examples, please let me know. Also below, I listed some other bundlers you may want to take a look at.
Cheers,
Evan Gillogley
Web Developer at Velocity
*Other Great Bundlers*:
[Rollup](https://github.com/rollup/rollup)(great tree shaking, primarily used for ES6)
[Lasso](https://github.com/lasso-js/lasso#another-client-side-bundler)— made by eBay — influenced by Browserify, but bundles all the assets.
[JSPM](https://github.com/jspm) — Like SystemJs with npm integration.
For more bundlers, check out this[page](https://www.slant.co/topics/3900/~frontend-javascript-module-bundlers).

