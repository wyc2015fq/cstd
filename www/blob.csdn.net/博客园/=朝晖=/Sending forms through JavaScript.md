# Sending forms through JavaScript - =朝晖= - 博客园
# [Sending forms through JavaScript](https://www.cnblogs.com/dhcn/p/7805627.html)
https://developer.mozilla.org/en-US/docs/Learn/HTML/Forms/Sending_forms_through_JavaScript
As in the [previous article](https://developer.mozilla.org/en-US/docs/HTML/Forms/Sending_and_retrieving_form_data), HTML forms can send an [HTTP](https://developer.mozilla.org/en-US/docs/HTTP) request declaratively. But forms can also prepare an HTTP request to send via JavaScript. This article explores ways to do that.
## A form is not always a form[Edit](https://developer.mozilla.org/en-US/docs/Learn/HTML/Forms/Sending_forms_through_JavaScript$edit#A_form_is_not_always_a_form)
With [open Web apps](https://developer.mozilla.org/en-US/docs/Open_Web_apps_and_Web_standards), it's increasingly common to use [HTML forms](https://developer.mozilla.org/en-US/docs/HTML/Forms) other than literal forms for humans to fill out — more and more developers are taking control over transmitting data.
### Gaining control of the global interface
Standard HTML form submission loads the URL where the data was sent, which means the browser window navigates with a full page load. Avoiding a full page load can provide a smoother experience by hiding flickering and network lag.
Many modern UIs only use HTML forms to collect input from the user. When the user tries to send the data, the application takes control and transmits the data asynchronously in the background, updating only the parts of the UI that require changes.
Sending arbitrary data asynchronously is known as [AJAX](https://developer.mozilla.org/en-US/docs/AJAX), which stands for "Asynchronous JavaScript And XML."
### How is it different?
[AJAX](https://developer.mozilla.org/en-US/docs/AJAX) uses the [`XMLHttpRequest`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest) (XHR) DOM object. It can build HTTP requests, send them, and retrieve their results.
Note: Older AJAX techniques might not rely on [`XMLHttpRequest`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest). For example, [JSONP](http://en.wikipedia.org/wiki/JSONP) combined with the [`eval()`](https://developer.mozilla.org/en-US/docs/Core_JavaScript_1.5_Reference:Global_Functions:eval) function. It works, but it's not recommended because of serious security issues. The only reason to use this is for legacy browsers that lack support for [`XMLHttpRequest`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest) or [JSON](https://developer.mozilla.org/en-US/docs/JSON), but those are very old browsers indeed! Avoid such techniques.
Historically, [`XMLHttpRequest`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest) was designed to fetch and send [XML](https://developer.mozilla.org/en-US/docs/XML) as an exchange format. However, [JSON](https://developer.mozilla.org/en-US/docs/JSON) superseded XML and is overwhelmingly more common today.
But neither XML nor JSON fit into form data request encoding. Form data (`application/x-www-form-urlencoded`) is made of URL-encoded lists of key/value pairs. For transmitting binary data, the HTTP request is reshaped into `multipart/form-data`.
If you control the front-end (the code that's executed in the browser) and the back-end (the code which is executed on the server), you can send JSON/XML and process them however you want.
But if you want to use a third party service, it's not that easy. Some services only accept form data. There are also cases where it's simpler to use form data. If the data is key/value pairs, or raw binary data, existing back-end tools can handle it with no extra code required.
So how to send such data?
## Sending form data[Edit](https://developer.mozilla.org/en-US/docs/Learn/HTML/Forms/Sending_forms_through_JavaScript$edit#Sending_form_data)
There are 3 ways to send form data, from legacy techniques to the newer [`FormData`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/FormData) object. Let's look at them in detail.
### Building a DOM in a hidden iframe
The oldest way to asynchronously send form data is building a form with the DOM API, then sending its data into a hidden [`<iframe>`](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/iframe). To access the result of your submission, retrieve the content of the [`<iframe>`](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/iframe).
Warning: Avoid using this technique. It's a security risk with third-party services because it leaves you open to [script injection attacks](http://en.wikipedia.org/wiki/Cross-site_scripting). If you use HTTPS, it can affect [the same origin policy](https://developer.mozilla.org/en-US/docs/JavaScript/Same_origin_policy_for_JavaScript), which can render the content of an [`<iframe>`](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/iframe) unreachable. However, this method may be your only option if you need to support very old browsers.
Here is an example:
``` html line-numbers  language-html
`<button onclick="sendData({test:'ok'})">Click Me!</button>`
```
``` js line-numbers  language-js
```
// Create the iFrame used to send our data
var iframe = document.createElement("iframe");
iframe.name = "myTarget";
// Next, attach the iFrame to the main document
window.addEventListener("load", function () {
  iframe.style.display = "none";
  document.body.appendChild(iframe);
});
// This is the function used to actually send the data
// It takes one parameter, which is an object populated with key/value pairs.
function sendData(data) {
  var name,
      form = document.createElement("form"),
      node = document.createElement("input");
  // Define what happens when the response loads
  iframe.addEventListener("load", function () {
    alert("Yeah! Data sent.");
  });
    
  form.action = "http://www.cs.tut.fi/cgi-bin/run/~jkorpela/echo.cgi";
  form.target = iframe.name;
  for(name in data) {
    node.name  = name;
    node.value = data[name].toString();
    form.appendChild(node.cloneNode());
  }
  // To be sent, the form needs to be attached to the main document.
  form.style.display = "none";
  document.body.appendChild(form);
  form.submit();
  // Once the form is sent, remove it.
  document.body.removeChild(form);
}
```
```
Here's the live result:
Open in CodePenOpen in JSFiddle
### Building an XMLHttpRequest manually
[`XMLHttpRequest`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest) is the safest and most reliable way to make HTTP requests. To send form data with [`XMLHttpRequest`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest), prepare the data by URL-encoding it, and obey the specifics of form data requests.
Note: To learn more about `XMLHttpRequest`, these articles may interest you: [An introductory article to AJAX](https://developer.mozilla.org/en-US/docs/AJAX/Getting_Started)and a more advanced tutorial about [using `XMLHttpRequest`](https://developer.mozilla.org/en-US/docs/DOM/XMLHttpRequest/Using_XMLHttpRequest).
Let's rebuild our previous example:
``` html line-numbers  language-html
`<button type="button" onclick="sendData({test:'ok'})">Click Me!</button>`
```
As you can see, the HTML hasn't really changed. However, the JavaScript is completely different:
``` js line-numbers  language-js
```
function sendData(data) {
  var XHR = new XMLHttpRequest();
  var urlEncodedData = "";
  var urlEncodedDataPairs = [];
  var name;
  // Turn the data object into an array of URL-encoded key/value pairs.
  for(name in data) {
    urlEncodedDataPairs.push(encodeURIComponent(name) + '=' + encodeURIComponent(data[name]));
  }
  // Combine the pairs into a single string and replace all %-encoded spaces to 
  // the '+' character; matches the behaviour of browser form submissions.
  urlEncodedData = urlEncodedDataPairs.join('&').replace(/%20/g, '+');
  // Define what happens on successful data submission
  XHR.addEventListener('load', function(event) {
    alert('Yeah! Data sent and response loaded.');
  });
  // Define what happens in case of error
  XHR.addEventListener('error', function(event) {
    alert('Oups! Something goes wrong.');
  });
  // Set up our request
  XHR.open('POST', 'https://example.com/cors.php');
  // Add the required HTTP header for form data POST requests
  XHR.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
  // Finally, send our data.
  XHR.send(urlEncodedData);
}
```
```
Here's the live result:
Open in CodePenOpen in JSFiddle
Note: This use of [`XMLHttpRequest`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest) is subject to the same origin policy if you want to send data to a third party web site. For cross-origin requests, you'll need [CORS and HTTP access control](https://developer.mozilla.org/en-US/docs/HTTP/Access_control_CORS).
### Using XMLHttpRequest and the FormData object
Building an HTTP request by hand can be overwhelming. Fortunately, a recent [XMLHttpRequest specification](http://www.w3.org/TR/XMLHttpRequest/) provides a convenient and simpler way to handle form data requests with the [`FormData`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/FormData) object.
The [`FormData`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/FormData) object can be used to build form data for transmission, or to get the data within a form element to manage how it's sent. Note that [`FormData`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/FormData) objects are "write only", which means you can change them, but not retrieve their contents.
Using this object is detailed in [Using FormData Objects](https://developer.mozilla.org/en-US/docs/DOM/XMLHttpRequest/FormData/Using_FormData_Objects), but here are two examples:
#### Using a standalone FormData object
``` html line-numbers  language-html
`<button type="button" onclick="sendData({test:'ok'})">Click Me!</button>`
```
You should be familiar with that HTML sample.
``` js line-numbers  language-js
```
function sendData(data) {
  var XHR = new XMLHttpRequest();
  var FD  = new FormData();
  // Push our data into our FormData object
  for(name in data) {
    FD.append(name, data[name]);
  }
  // Define what happens on successful data submission
  XHR.addEventListener('load', function(event) {
    alert('Yeah! Data sent and response loaded.');
  });
  // Define what happens in case of error
  XHR.addEventListener('error', function(event) {
    alert('Oups! Something went wrong.');
  });
  // Set up our request
  XHR.open('POST', 'https://example.com/cors.php');
  // Send our FormData object; HTTP headers are set automatically
  XHR.send(FD);
}
```
```
Here's the live result:
Open in CodePenOpen in JSFiddle
#### Using FormData bound to a form element
You can also bind a `FormData` object to a [`<form>`](https://developer.mozilla.org/en-US/docs/Web/HTML/Element/form) element. This creates a `FormData` that represents the data contained in the form.
The HTML is typical:
``` html line-numbers  language-html
```
<form id="myForm">
  <label for="myName">Send me your name:</label>
  <input id="myName" name="name" value="John">
  <input type="submit" value="Send Me!">
</form>
```
```
But JavaScript takes over the form:
``` js line-numbers  language-js
```
window.addEventListener("load", function () {
  function sendData() {
    var XHR = new XMLHttpRequest();
    // Bind the FormData object and the form element
    var FD = new FormData(form);
    // Define what happens on successful data submission
    XHR.addEventListener("load", function(event) {
      alert(event.target.responseText);
    });
    // Define what happens in case of error
    XHR.addEventListener("error", function(event) {
      alert('Oups! Something goes wrong.');
    });
    // Set up our request
    XHR.open("POST", "https://example.com/cors.php");
    // The data sent is what the user provided in the form
    XHR.send(FD);
  }
 
  // Access the form element...
  var form = document.getElementById("myForm");
  // ...and take over its submit event.
  form.addEventListener("submit", function (event) {
    event.preventDefault();
    sendData();
  });
});
```
```
Here's the live result:
Open in CodePenOpen in JSFiddle
## Dealing with binary data[Edit](https://developer.mozilla.org/en-US/docs/Learn/HTML/Forms/Sending_forms_through_JavaScript$edit#Dealing_with_binary_data)
If you use a [`FormData`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/FormData) object with a form that includes `<input type="file">` widgets, the data will be processed automatically. But to send binary data by hand, there's extra work to do.
There are many sources for binary data on the modern Web: [`FileReader`](https://developer.mozilla.org/en-US/docs/Web/API/FileReader), [`Canvas`](https://developer.mozilla.org/en-US/docs/Web/API/HTMLCanvasElement), and [WebRTC](https://developer.mozilla.org/en-US/docs/WebRTC/navigator.getUserMedia), for example. Unfortunately, some legacy browsers can't access binary data or require complicated workarounds. Those legacy cases are out of this article's scope. If you want to know more about the `FileReader` API, read [Using files from web applications](https://developer.mozilla.org/en-US/docs/Using_files_from_web_applications).
Sending binary data with support for [`FormData`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/FormData) is straightfoward. Use the `append()` method and you're done. If you have to do it by hand, it's trickier.
In the following example, we use the [`FileReader`](https://developer.mozilla.org/en-US/docs/Web/API/FileReader) API to access binary data and then build the multi-part form data request by hand:
``` html line-numbers  language-html
```
<form id="myForm">
  <p>
    <label for="i1">text data:</label>
    <input id="i1" name="myText" value="Some text data">
  </p>
  <p>
    <label for="i2">file data:</label>
    <input id="i2" name="myFile" type="file">
  </p>
  <button>Send Me!</button>
</form>
```
```
As you see, the HTML is a standard `<form>`. There's nothing magical going on. The "magic" is in the JavaScript:
``` js line-numbers  language-js
```
// Because we want to access DOM node,
// we initialize our script at page load.
window.addEventListener('load', function () {
  // These variables are used to store the form data
  var text = document.getElementById("i1");
  var file = {
        dom    : document.getElementById("i2"),
        binary : null
      };
 
  // Use the FileReader API to access file content
  var reader = new FileReader();
  // Because FileReader is asynchronous, store its
  // result when it finishes to read the file
  reader.addEventListener("load", function () {
    file.binary = reader.result;
  });
  // At page load, if a file is already selected, read it.
  if(file.dom.files[0]) {
    reader.readAsBinaryString(file.dom.files[0]);
  }
  // If not, read the file once the user selects it.
  file.dom.addEventListener("change", function () {
    if(reader.readyState === FileReader.LOADING) {
      reader.abort();
    }
    
    reader.readAsBinaryString(file.dom.files[0]);
  });
  // sendData is our main function
  function sendData() {
    // If there is a selected file, wait it is read
    // If there is not, delay the execution of the function
    if(!file.binary && file.dom.files.length > 0) {
      setTimeout(sendData, 10);
      return;
    }
    // To construct our multipart form data request,
    // We need an XMLHttpRequest instance
    var XHR = new XMLHttpRequest();
    // We need a separator to define each part of the request
    var boundary = "blob";
    // Store our body request in a string.
    var data = "";
    // So, if the user has selected a file
    if (file.dom.files[0]) {
      // Start a new part in our body's request
      data += "--" + boundary + "\r\n";
      // Describe it as form data
      data += 'content-disposition: form-data; '
      // Define the name of the form data
            + 'name="'         + file.dom.name          + '"; '
      // Provide the real name of the file
            + 'filename="'     + file.dom.files[0].name + '"\r\n';
      // And the MIME type of the file
      data += 'Content-Type: ' + file.dom.files[0].type + '\r\n';
      // There's a blank line between the metadata and the data
      data += '\r\n';
      
      // Append the binary data to our body's request
      data += file.binary + '\r\n';
    }
    // Text data is simpler
    // Start a new part in our body's request
    data += "--" + boundary + "\r\n";
    // Say it's form data, and name it
    data += 'content-disposition: form-data; name="' + text.name + '"\r\n';
    // There's a blank line between the metadata and the data
    data += '\r\n';
    // Append the text data to our body's request
    data += text.value + "\r\n";
    // Once we are done, "close" the body's request
    data += "--" + boundary + "--";
    // Define what happens on successful data submission
    XHR.addEventListener('load', function(event) {
      alert('Yeah! Data sent and response loaded.');
    });
    // Define what happens in case of error
    XHR.addEventListener('error', function(event) {
      alert('Oups! Something went wrong.');
    });
    // Set up our request
    XHR.open('POST', 'https://example.com/cors.php');
    // Add the required HTTP header to handle a multipart form data POST request
    XHR.setRequestHeader('Content-Type','multipart/form-data; boundary=' + boundary);
    // And finally, send our data.
    XHR.send(data);
  }
  // Access our form...
  var form = document.getElementById("myForm");
  // ...to take over the submit event
  form.addEventListener('submit', function (event) {
    event.preventDefault();
    sendData();
  });
});
```
```
Here's the live result:
Open in CodePenOpen in JSFiddle
## Conclusion[Edit](https://developer.mozilla.org/en-US/docs/Learn/HTML/Forms/Sending_forms_through_JavaScript$edit#Conclusion)
Depending on the browser, sending form data through JavaScript can be easy or difficult. The [`FormData`](https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/FormData) object is generally the answer, and don't hesitate to use a polyfill for it on legacy browsers:
- [This gist](https://gist.github.com/3120320) is polyfills `FormData` with [`Web Workers`](https://developer.mozilla.org/en-US/docs/Web/API/Using_web_workers).
- [HTML5-formdata](https://github.com/francois2metz/html5-formdata) attempts to polyfill the `FormData` object, but it requires the [File API](http://www.w3.org/TR/FileAPI/)
- [This polyfill](https://github.com/jimmywarting/FormData) provides most of the new methods FormData has (entries, keys, values, and support of `for...of`)

