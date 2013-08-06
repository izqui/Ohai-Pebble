#Ohai-Pebble
![](http://dl.dropboxusercontent.com/s/xcdm7r52hbw80c1/ohaipebble1.png) 
![](http://dl.dropboxusercontent.com/s/b8oed02uoxpibwy/ohaipebble2.png)


Ohai for App.net is a really cool journaling app [Steve Streza](http://twitter.com/stevestreza) built on top of App.net API.

I had been thinking about building an app for the Pebble to log all the places I go, since I have my Pebble all the time in my wrist. When I saw Ohai I decided I would build the app on top of it. 

Since the limitations of the Pebble, the connection between App.net (Ohai) and the Pebble has to be done in an external server (nodeJS) that everybody who uses the app has to deploy. This server handles the fetch of places nearby and the checkin to app.net. 

The Pebble app connects to the internet though [httpebble](http://kathar.in/httpebble/), so you have to download the app for [iOS](http://itunes.apple.com/us/app/httpebble/id650174711?ls=1&mt=8) or [Android](https://play.google.com/store/apps/details?id=com.lukekorth.httpebble)


###Installation

######Server

First of all, you need an App.net account, a free one will work just fine. 

Then, you have to download an Ohai compatible app (by now, it's only [Ohai for iOS](http://ohaiapp.net) which is $4.99). By logging in the app, a diary channel (net.app.ohai.journal) will be created in your App.net account.

Now you have to download the code, and set up your server that will talk to App.net:
```
$ git clone https://github.com/izqui/Ohai-Pebble.git
$ cd Ohai-Pebble/server
```
In this directory you have a keys.json file that you can write yourself with your token and channel_id. But to make it easier I build a simple web app to generate this files. Just go to [pebbleohai.izqui.me](http://pebbleohai.izqui.me), follow the auth flow and it will download a keys.json file you need to place in this directory replacing the original one.

Now you need to deploy the server somewhere. I'd recommend Heroku because it's really easy and fast to get the server running (and since it will be just for yourself, the free tier will be fine). So, in order to deploy to Heroku you need to sign up for an account and set up your machine ([quick start guide](https://devcenter.heroku.com/articles/quickstart)). Once you are all set, follow these steps to deploy:

```
$ cd Ohai-Pebble/server
$ git init
$ git add .
$ git commit -a -m "Initial commit"
$ heroku create
$ git push heroku master
$ heroku ps:scale web=1
```
Take note of your server URL by executing  `$ heroku domains`, you will need that later.

######Pebble

For compiling the app, the easiest way is to use [CloudPebble](http://cloudpebble.net). Just create a free account and import the project (Ohai-Pebble/pebble). Once there, edit the `main.c`file and enter your server URL in the three lines needed replacing `[YOUR URL]` (lines 136, 193, 213).

That's it, compile the app and run it in your Pebble. 
