# Angular External js library calling Document.Ready - =朝晖= - 博客园
# [Angular External js library calling Document.Ready](https://www.cnblogs.com/dhcn/p/9272162.html)
**https://stackoverflow.com/questions/51094841/angular-external-js-library-calling-document-ready**
**Step 1**
Check if the external library is available on [npm](https://www.npmjs.com/). If so you may be able to import the desired function rather than altering a vendored file.
For example, it may provide an API such that:
*YourTsComponent.ts*
```
const CallMe = require('library').CallMe
// or
import { CallMe } from 'library'
// on route change
CallMe()
```
If something like that is available, great, otherwise...
**Step 2**
Confirm your theory with a global (attach `CallMe` to window temporarily). If your theory is correct, you should be able to get the desired behavior by calling this global variable on route change.
*Tester.js*
```
(function($) {
  "use strict";
  $(document).ready(function() {
    CallMe();
  });
  function CallMe() {
    console.log('HEY I GOT CALLED');
  }
  // TODO - remove (test only)
  window._CallMe = CallMe
})(jQuery);
```
*YourTsComponent.ts*
```
// on route change
window._CallMe()
```
If that **doesn't work**, you must reevaluate your theory.
but if it does ...
**Step 3**
Convert the vendored library to a module that can be consumed by your app. Your mileage may vary based on what (if any) module system you are using. For example, if you are using require.js:
*Tester.js*
```
(function(factory) {
  if (typeof define === 'function' && define.amd) {
    // AMD
    define(['jquery'], factory);
  } else if (typeof exports === 'object') {
    // CommonJS
    factory(require('jquery'));
  } else {
    // Browser globals
    factory(jQuery);
  }
}(function($) {
  "use strict";
  function CallMe() {
    console.log('HEY I GOT CALLED');
  }
  $(document).ready(function() {
    CallMe();
  });
  return CallMe
}));
```
*YourTsComponent.ts*
```
const CallMe = require('/path/to/tester.js')
// on route change
CallMe()
```
**If you're not keen on re-writing a vendored library**
You may consider overriding `.ready`'s default behavior so that it may be retriggered. [There Are a few answers here if you want to go this route](https://stackoverflow.com/questions/2238030/trigger-document-ready-so-ajax-code-i-cant-modify-is-executed), but be warned, overriding default jQuery behavior is probably much more error prone than editing a single vendored file.

