# CockleRake Installation

Steps required to install CockleRake (on a Linux-based system at least)

1. Install `virtualenv`
1. Set up the virtualenv environment
 
 ```
    cd ..
    virtualenv CockleRake
 ```
 
1. Activate the virtualenv environment

 ```
    cd CockleRake
    source bin/activate
 ```

1. Now install Flask for our web server

 ```
    pip install flask
 ```

1. Configure any custom settings by editing `settings.cfg`.  The DATABASE setting must match the filename you use in the following step (for example 'cockles.db').  The most likely setting you should configure is the WORKSHOP_NAME.
1. Tell the server where to find the settings file (change `settings.cfg` to point to the correct settings file if you've changed its name or location)

 ```
    export COCKLE_SETTINGS=settings.cfg
 ```

1. Create a database to hold any registered Cockles and their readings

 ```
    sqlite3 cockles.db < schema.sql
 ```

1. Run the CockleRake server

 ```
    python cockle-rake.py
 ```

## Running Automatically

On a Raspberry Pi you can use the cocklerake.service and run_server files to make the CockleRake server start when the computer boots up.

To set that up...

**Note: This assumes that CockleRake has been installed in /home/pi/cocklecraft-of-things/CockleRake.  If you've installed it elsewhere you'll need to modify the cocklerake.service file and update the WorkingDirectory**

1. Install the service script

```
    sudo cp cocklerake.service /lib/systemd/system/
```

1. Enable the service

```
    sudo systemctl enable cocklerake.service
```

1. Start the service

```
    sudo service cocklerake start
```

1. You can check on the status of the service at any time with

```
    sudo service cocklerake status
```

