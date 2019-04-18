# [Parcel] Bundle a React App with Parcel - weixin_33985507的博客 - CSDN博客
2017年12月11日 16:06:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
[Parcel](https://parceljs.org/) comes in as the new cool kid in the bundlers world. Unlike other bundlers which take lots of explicit configuration, Parcel works out of the box and requires almost zero-configuration by intelligently inferring it depending on what you use in your code.
This lessons shows you how to bundle a simple [React](https://reactjs.org/) app starting from scratch and having hot-reloading, dev-server and generating an optimised bundle with no configuration and only 4 dependencies.
Install:
```
yarn global add parcel-bundler
npm install -g parcel-bundler
```
Package.json:
```
{
  "name": "react-parcel",
  "version": "1.0.0",
  "main": "index.js",
  "scripts": {
    "start": "parcel index.html",
    "build": "parcel build index.html"
  },
  "author": "",
  "license": "MIT",
  "devDependencies": {
    "babel-preset-react-app": "^3.1.0",
    "parcel-bundler": "^1.1.0"
  },
  "dependencies": {
    "react": "^15.6.1",
    "react-dom": "^15.6.1"
  }
}
```
