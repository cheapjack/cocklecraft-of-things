## Getting Started with Domestic Science 

Have a look in the `~/cocklecraft-of-things` folder for more detailed  documentation.

You should also look at the [sugarcube](http://www.motoslave.net/sugarcube/) documentation.

You can read about how to use the Cockle data in [Twine here](/TwineDemo.html)

Basically on this Pi Raspbian image we can

 * Use the Pi as a WiFi access point
 * Serve html and css from `/var/www/html` through the apache server
  * Serve a domesticscience portal `index.html` to Hwa Young's text adventure `brazil.html`
 * Use the network name `domesticscience` to allow most devices to access at `domesticscience.local`
 * Serve a cocklerake server

Other tools:

 * apache webserver
 * mcpi
 * node-red
 * pip
 * pocketmine server (outdated)
 * node-red to pocketmine
 * git
 * vim
 * virtualenv
 * sqlite3

### Using the cocklerake server

Probably best to start with the setup instructions in the `experimental` branch of the [cocklecraft-of-things](https://github.com/mcqn/cocklecraft-of-things/tree/experimental) to get to grips with the wiring and setup of the ESP8266 SparkFun thing which does all our temperature sensing with a Dallas DS1820B waterproof temperature sensor. We've added an arduino setup for a locally served CockleRake server.

## Basic System

ESP8266 SparkFun thing connects to a WiFi network `domesticscience` password `raspberry` and uses a Dallas Temperature sensor to send temperature readings to [domesticscience](http://domesicscience.local:5000), a flask server. Same server also allows you to record observations as Strings with a location which you set and a timestamp 

You can then pull these into Twine pages with JQUERY calls

### Getting going

 * Fire up the Pi & Join the `domesticscience` WiFi Network with password `raspberry`
 * Open a termimnal and login to the pi over ssh `HwaYoungsMac$ ssh pi@domesticscience.local` password as above

If your offline you may need to set the time and date

`$ sudo date --set '2016-10-29 18:26:00'`

By default the CockleRake server starts up automatically on boot but if you want to stop it etc:

 * **stop** `pi$ sudo service cocklerake stop`
 * **stort** `pi$ sudo service cocklerake start`
 * **re-stort** `pi$ sudo service cocklerake restart`
 * **status** `pi$ sudo service cocklerake status`

The startup service script is in `/CockleRake/run_server`.

A registered cockle called `hightown-ainsdale-brazil` is setup in your tiffin tin and will eventually connect to the Pi's WiFi Access point once turned on. 

These are the normal setup instructions without the auto service start for reference

 * Once logged in on the Pi `cd` into the `~/cocklecraft-of-things` with  `pi$ cd ~/cocklecraft-of-things/CockleRake`
 * `pi$ source bin/activate` or `pi$ . bin/activate` to activate the virtualenv
 * setup the settings with `pi$ export COCKLE_SETTINGS=settings.cfg`
 * Start the server at `domesticscience.local:5000` with `pi$ python cockle-rake.py

### Using the CockleRake Server

 * You can now on another device, log onto the Pi's WiFi and navigate to [domesticscience.local](http://domesticscience.local) which will serve a homepage and portal to your adventures which you could translate or modify however you like. You can look at the cocklerake server using the button link in the homepage or from [here](http://domesticscience.local:5000)
 * If the device wont see `domesticscience.local` try the ip address `172.24.1.1` instead.
 * You can also log any kind of observation as a string By clicking on the button in the homepage, the cocklerake server or [make an observation from here](http://domesticscience.local:5000/observe)
  * You can record a `location` as a string for your observation; this is for you to be able to easily find it later and display in game. This could be `river` or could be `chair` or it could be `feeling`. This string becomes the data **endpoint**. It's made so as long as the `cockles.db` database isn't deleted, all of these locations and their associated values stay there for as long as you need them. Don't delete it as it's got the cockles registration details in there, not just observations and temperatures. It's best to not use `-` or `SPACE` characters in the location as `-` confuses the JSON thinking its a cockle and using `SPACE` means you have to replace every `SPACE` with `%20`
  * Once you've submitted your observation the location defines a data endpoint (*ie* the **address** on the cocklerake server) for the observation: the observation is the **value** of the data: *ie* the string you've typed in which can be as long as you like. It also timestamps the observation string, the **recorded_at** so you could query that in your javascript.

So if you make an observation, the string **"I've just made a generic observation"** with the location **"location"** and submit it then you can find it at [/location](http://domesticscience.local:5000/location) and typically it would return the JSON:

```
{"recorded_at":"2016-10-25 16:25:21.877885", "value":I've just made a generic observation!}
```

### Editing the files, workflow etc

Your best working headless over ssh to edit stuff either directly in vim or build your adventures in the local Twine app on your laptop, then sending new files with secure copy `scp`. You just ssh through the `domesticscience` WiFi

### Uploading files

You can send any file from your laptop as long as you are on the `domesticscience.local` network, for example:

`HwaYoungsMac$ scp /path/to/my/amazingfile pi@domesticscience.local:/home/pi`

You'll need the password `raspberry` again.

You can then move it from `/home/pi` to the apache server `html` folder over ssh on the pi. So when connected by ssh to the pi:

`pi@domesticscience:~ $ sudo mv ~/amazingfile /var/www/html`

You'll need the password and you may need to change permissions of the new file with `sudo chmod 644 /location/of/myamazingfile.html`

You can put all yr web stuff in there `/var/www/html`

Your in the sudo'ers group so be careful, with great power comes great responsibility etc etc...

You can also send messages from the pi to your mac (but from the mac client side)

`HwaYoungsMac$ scp pi@domesticscience.local:/var/www/html/MyFileIWantToEdit.html /path/where/i/want/my/amazingfile `

## Making your adventures

Now head over to the [Twine 2 Demo](/TwineDemo.html) to see how to pull & interact with the data

### Pop up Pocketmine 

There is a `pocketmine` file in `/etc/init.d` that sets off the pop-up pocketmine server that sends messages to node-red but it's set to off as the servers outdated now.

To turn it off rename it to pocketmine.off
To turn it on rename it to pocketmine
