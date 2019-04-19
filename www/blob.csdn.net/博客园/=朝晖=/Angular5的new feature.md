# Angular5的new feature - =朝晖= - 博客园
# [Angular5的new feature](https://www.cnblogs.com/dhcn/p/7793649.html)
https://blog.angular.io/version-5-0-0-of-angular-now-available-37e414935ced
# Version 5.0.0 of Angular Now Available
We are pleased to announce version 5.0.0 of Angular, pentagonal-donut. This is a [major release](http://angularjs.blogspot.com/2016/10/versioning-and-releasing-angular.html) containing new features and bugfixes. This release continues our focus on making Angular smaller, faster, and easier to use.
![](https://cdn-images-1.medium.com/max/1600/1*nbJ41jD1-r2Oe6FsLjKaOg.png)
Here’s a breakdown of some of the biggest changes in v5. For the full list, please see the [changelog](https://github.com/angular/angular/blob/master/CHANGELOG.md).
### Build Optimizer
As of 5.0.0, production builds created with the CLI will now apply the build optimizer by default.
The build optimizer is a tool included in our CLI for making your bundles smaller using our semantic understanding of your Angular application.
The build optimizer has two main jobs. First, we are able to mark parts of your application as `pure`, this improves the tree shaking provided by the existing tools, removing additional parts of your application that aren’t needed.
The second thing the build optimizer does is to remove Angular decorators from your application’s runtime code. Decorators are used by the compiler, and aren’t needed at runtime and can be removed. Each of these jobs decrease the size of your JavaScript bundles, and increase the boot speed of your application for your users.
### Angular Universal State Transfer API and DOM Support
You can now more easily share application state between the server side and client side versions of your application.
Angular Universal is a project focused on helping developers to perform server side rendering (SSR) of Angular applications. By rendering your Angular applications on the server and then bootstrapping on top of the generated HTML, you can add support for scrapers and crawlers that don’t support JavaScript, and you can increase the perceived performance of your application.
In 5.0.0, the team has added `ServerTransferStateModule` and the corresponding `BrowserTransferStateModule`. This module allows you to generate information as part of your rendering with platform-server, and then transfer it to the client side so that this information does not need to be regenerated. This is useful for cases such as when your application fetches data over HTTP. By transferring state from the server, this means developers do not need to make a second HTTP request once the application makes it to the client. Documentation for state transfer is forthcoming in the next few weeks.
Another change from the Angular Universal team is the addition of [Domino](https://github.com/fgnass/domino) to platform-server. Domino means that we support more DOM manipulations out of the box within server side contexts, improving our support for 3rd party JS and Component libraries that aren’t server-side aware.
### Compiler Improvements
We’ve improved the Angular compiler to support incremental compilation. This provides faster rebuilds, especially for production builds and builds with AOT. We’ve also added features to our Decorators and made it possible to ship smaller bundles by removing whitespace.
TypeScript Transforms
Under the hood, the Angular compiler now operates as a TypeScript transform, making incremental rebuilds dramatically faster. TypeScript transforms were a new feature introduced as part of TypeScript 2.3 that allows us to hook into the standard TypeScript compilation pipeline.
You can take advantage of this by running `ng serve` with the AOT flag turned on.
ng serve --aot
We recommend everyone give this a try. This will become the default in a future release of the CLI. There are some known speed issues with projects with more than a thousand components. We want to be sure projects of all sizes will experience these improvements.
When performing an incremental AOT build of [https://angular.io](https://angular.io/), the new compiler pipeline saves 95% of the build time (from more than 40 seconds to less than 2 seconds on our development machines).
Our goal was to make AOT compilation fast enough so that developers could use it for development, eliminating the differences that developers sometimes run into when trying to go to production for the first time. The team has hit its 2 second incremental AOT rebuild performance targets, and will be turning AOT on by default in a future release of the CLI.
As part of this transition to transforms, we don’t need a `genDir` anymore, and `outDir` has changed: we are now always emitting generated files for packages in node_modules.
Preserve Whitespace
Historically tabs, newlines, and spaces in your templates have been faithfully recreated and included in your build by the compiler. You can now choose whether or not to preserve whitespace coming from your components and your application.
You can specify this as part of each component’s decorator, where it currently defaults to *true*.
Or you can specify it application wide in your tsconfig.json, where it also currently defaults to *true*.
In general, the component-level specifications override application-wide specifications. In the future the team hopes to default to `false` to save space for developers by default. Don’t worry about your `<pre>` tags, these are handled intelligently.
[Read more about `preserveWhitespaces` on our docs site](https://angular.io/api/core/Component#preserveWhitespaces).
#### Improved Decorator Support
We now support expression lowering in decorators for lambdas and the value of `useValue`, `useFactory` and `data` in object literals. This allows you to use values that can only be calculated at runtime in decorators for expressions that are lowered.
You can now use a lambda instead of a named function, meaning you can execute code without affecting your d.ts or your public API.
We will also lower expressions as part of useValue.
### Internationalized Number, Date, and Currency Pipes
We have built new number, date, and currency pipes that increase standardization across browsers and eliminate the need for i18n polyfills.
In Angular we have relied on the browser to provide number, date, and currency formatting using browser i18n APIs. This resulted in the need for a polyfill for most developers, meant that users were seeing inconsistent results across browsers, and we received comments that common formats (such as the currency pipe) didn’t match developer expectations out of the box.
In 5.0.0 we’ve updated the pipes to use our own implementation, relying on the [CLDR](http://cldr.unicode.org/) to provide extensive locale support and configurations for any locales you want to support. We’ve produced [a document comparing the pipe behavior between v4 and v5](https://docs.google.com/spreadsheets/d/12iygt-_cakNP1VO7MV9g4lq9NsxVWG4tSfc98HpHb0k/edit#gid=0).
If you aren’t ready for the new pipes, you can import `DeprecatedI18NPipesModule` to get access to the old behavior.
[Read more about the changes to our i18n pipes in the changelog](https://github.com/angular/angular/blob/master/CHANGELOG.md#i18n-pipes)
### Replace the ReflectiveInjector with StaticInjector
In order to remove even more polyfills, we’ve replaced the ReflectiveInjector with the StaticInjector. This injector no longer requires the Reflect polyfill, reducing application size for most developers.
Before
`ReflectiveInjector.resolveAndCreate(providers);`
After
`Injector.create(providers);`
### Zone speed improvements
We’ve made zones faster by default, and we’ve made it possible to bypass zones entirely for performance focused applications.
To bypass zones, bootstrap your application with ‘noop’ as your ngZone.
For a full example, take a look [the example ng-component-state project](https://stackblitz.com/edit/ng-component-state).
### exportAs
We’ve added support for multiple names for your components / directives. This can be very useful for helping your users to migrate without breaking changes. By exporting a directive with multiple names, you can make new names available in the Angular microsyntax without breaking existing code. This has been used as a part of the Angular Material project in its prefix migration, and can help other component authors as well.
Example
### HttpClient
In version 4.3 we shipped HttpClient in `@angular/common` as a smaller, easier, and more powerful way to make web requests in Angular. The new HttpClient has gotten some great praise from developers, so we’re now recommending HttpClient for all applications, and deprecating the previous `@angular/http library`.
To update to HttpClient, you’ll need to replace `HttpModule` with `HttpClientModule` from `@angular/common/http` in each of your modules, inject the HttpClient service, and remove any `map(res => res.json())` calls, which are no longer needed.
### CLI v1.5
Starting with v1.5 of the Angular CLI, we have added support for Angular v5.0.0 and will generate v5 projects by default.
With this minor release we have turned on the build optimizer by default, so developers can benefit from smaller bundles.
We’re also updating the way we use `tsconfig.json` files to follow TypeScript standards more strictly. Previously if we detected a lazy loaded route and you were manually specifying a list of `files` or `include` in your `tsconfig.json`, we would automatically add these routes, but we now follow the TypeScript specification and no longer do this. By default, the CLI configures TypeScript without `files` or `include` sections, so most developers won’t be affected by this.
### Angular Forms adds updateOn Blur / Submit
You can now run validation and value updates on `blur` or on `submit`, instead of on every input event.
Forms are a very important part of many applications, and if you have any sort of server side validation, or any heavier processes triggered by validation or value changes that you want to run less often, you can now take control of the validation and value change timing at the control level, or specify it for an entire form.
Additionally, you can now specify `asyncValidators` directly in the options object, rather than specifying it as the third parameter.
#### Template Driven Forms
Before
`<input name="firstName" ngModel>`
After
`<input name="firstName" ngModel [ngModelOptions]="{updateOn: 'blur'}">`
or
`<form [ngFormOptions]="{updateOn: 'submit'}">`
#### Reactive Forms
Before
```
new FormGroup(value);
new FormControl(value, [], [myValidator])
```
After
`new FormGroup(value, {updateOn: 'blur'}));`
`new FormControl(value, {updateOn: 'blur', asyncValidators: [myValidator]})`
### RxJS 5.5
We’ve updated our use of RxJS to 5.5.2 or later. This recent release of RxJS fully empowers developers to avoid the side effects of the previous import mechanism with a new way of using RxJS called “[lettable operators](https://github.com/ReactiveX/rxjs/blob/master/doc/lettable-operators.md)”. These new operators eliminate the side effects and the code splitting / tree shaking problems that existed with the previous ‘patch’ method of importing operators.
Instead of
You can now
Additionally, RxJS now distributes a version using ECMAScript Modules. The new Angular CLI will pull in this version by default, saving considerably on bundle size. But if you’re not using the Angular CLI, you should still point to the new distribution. Documentation can be found in the [Build and Treeshaking](https://github.com/ReactiveX/rxjs/blob/master/doc/lettable-operators.md#build-and-treeshaking) section of the lettable operators documentation.
### New Router Lifecycle Events
We’ve added new lifecycle events to the router, allowing developers to track the cycle of the router from the start of running guards through to completion of activation. These events could be used for things such as showing a spinner on a specific router outlet when a child is updating or to measure performance of guards and/or resolvers.
The new events (in sequence) are `GuardsCheckStart`, `ChildActivationStart`, `ActivationStart`, `GuardsCheckEnd`, `ResolveStart`, `ResolveEnd`, `ActivationEnd`, `ChildActivationEnd`. An example of using these events to start/stop a spinner might look like this:
### How do I update?
We built the [Angular Update Guide](https://angular-update-guide.firebaseapp.com/) to help guide you through the process and to learn about any changes you’ll want to make in your code before you start the update process, the steps to update your app, and information on preparing for future versions of Angular.
We’ve removed many previously deprecated APIs (like `OpaqueToken` ) and released several new deprecations. This guide will walk you through the changes you’ll need to make to your application.
### Known Issues
There are known issues with production build source maps. Some source maps may result in undefined sources for errors.
[https://github.com/angular/angular/issues/19840](https://github.com/angular/angular/issues/19840)

