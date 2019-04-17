# Solve Error: node postinstall  sh: node: command not found - Grandyang - 博客园







# [Solve Error: node postinstall  sh: node: command not found](https://www.cnblogs.com/grandyang/p/10018366.html)







When install the yeoman using the following command:



```
npm install -g yo
```



You might have the following error:

```
npm WARN deprecated cross-spawn-async@2.2.5: cross-spawn no longer requires a build toolchain, use it instead
/root/.nvm/versions/node/v9.3.0/bin/yo -> /root/.nvm/versions/node/v9.3.0/lib/node_modules/yo/lib/cli.js
/root/.nvm/versions/node/v9.3.0/bin/yo-complete -> /root/.nvm/versions/node/v9.3.0/lib/node_modules/yo/lib/completion/index.js

> spawn-sync@1.0.15 postinstall /root/.nvm/versions/node/v9.3.0/lib/node_modules/yo/node_modules/spawn-sync
> node postinstall

sh: node: command not found
npm ERR! file sh
npm ERR! code ELIFECYCLE
npm ERR! errno ENOENT
npm ERR! syscall spawn
npm ERR! spawn-sync@1.0.15 postinstall: `node postinstall`
npm ERR! spawn ENOENT
npm ERR!
npm ERR! Failed at the spawn-sync@1.0.15 postinstall script.
npm ERR! This is probably not a problem with npm. There is likely additional logging output above.

npm ERR! A complete log of this run can be found in:
npm ERR! /root/.npm/_logs/2018-11-25T16_49_27_843Z-debug.log
```



Try the following commands instead:



```
npm install -g try-thread-sleep
npm install -g yo --ignore-scripts spawn-sync
```



Now try to type 'yo' to see if yeoman installed correctly, if you happened to have following error:

```
/root/.nvm/versions/node/v9.3.0/lib/node_modules/yo/node_modules/write-file-atomic/index.js:197
throw err
^

Error: EACCES: permission denied, open '/root/.config/insight-nodejs/insight-yo.json.3393582327'
at Object.fs.openSync (fs.js:663:18)
at Function.writeFileSync [as sync] (/root/.nvm/versions/node/v9.3.0/lib/node_modules/yo/node_modules/write-file-atomic/index.js:180:17)
at Conf.set store [as store] (/root/.nvm/versions/node/v9.3.0/lib/node_modules/yo/node_modules/conf/index.js:142:19)
at Conf.set (/root/.nvm/versions/node/v9.3.0/lib/node_modules/yo/node_modules/conf/index.js:64:14)
at Insight.set optOut [as optOut] (/root/.nvm/versions/node/v9.3.0/lib/node_modules/yo/node_modules/insight/lib/index.js:56:15)
at Object.<anonymous> (/root/.nvm/versions/node/v9.3.0/lib/node_modules/yo/lib/cli.js:198:18)
at Module._compile (module.js:660:30)
at Object.Module._extensions..js (module.js:671:10)
at Module.load (module.js:573:32)
at tryModuleLoad (module.js:513:12)
```



Try the following command to fix:



```
chmod g+rwx /root/.config/insight-nodejs/
```



If everything works fine, after type 'yo', you should see the following output:



```
❯ yo
? ==========================================================================
We're constantly looking for ways to make yo better!
May we anonymously report usage statistics to improve the tool over time?
More info: https://github.com/yeoman/insight & http://yeoman.io
========================================================================== Yes
? 'Allo Grandyang! What would you like to do? Install a generator
? Search npm for generators: Express
? Here's what I found. Official generator → ෴
  Install one? Return home
? 'Allo Grandyang! What would you like to do? Get me out of here!

     _-----_     ╭───────────────────────╮
    |       |    │      Bye from us!     │
    |--(o)--|    │       Chat soon.      │
   `---------´   │      Yeoman team      │
    ( _´U`_ )    │    http://yeoman.io   │
    /___A___\   /╰───────────────────────╯
     |  ~  |
   __'.___.'__
 ´   `  |° ´ Y `
```














