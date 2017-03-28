## Make a test Twine Story & upload to `/var/www/html`

Use an installed version of `Twine v2.0.8-11` to keep things offline

We can use the built in SugarCube or add the SugarCube-2 format from [here](http://www.motoslave.net/sugarcube/2/docs/twine-2.html#installation), both work, but add it locally `/SugarCube-2/format.js` otherwise it will need to access the `format.js` online.

Make a new adventure. We'll be putting javascript in the Twine page body to poll data from a Cockle Server in some `<<script>><</script>>` tags in twine then use `<span id="#mycustomid">something</span>` in the html of each Twine page to display results from JSON.

We won't be adding the `JQUERY` calls globally in the `Edit Story JavaScript` as they'll only be called once and then ignored and potentially forgotten about in the rest of the story. It would also stop the data being updated. So we will simply use `<<script>>` tags in the Twine body instead. 


### At present, you can:

Read timestamped temperature data from the waterproof DS1820B Dallas Temperature sensor in the Tiffin tin and serve it to [domesticscience.local:5000}(http://domesticscience.local:5000")

You can access the temperature data at [/hightown-ainsdale-brazil](http://domesticscience.local:5000/hightown-ainsdale-brazil)

You can access observation data at [domesticscience.local:5000/location](http://domesticscience.local:5000/location) where `location` is the observations location which you defined when making the observation.

It's best to not use `-` or `SPACE` characters in the location as it can confuse the JSON and mean you have to replace every `SPACE` with `%20`

Here's some example Twine content in Portuguese in the `Start` location  with `html` and `JQUERY` inside it

```
Ola! A temperatura é de <span id="weather-temp">???</span> graus centígrados.  Foi neste momento <span id="weather-timestamp">sometime or other</span>.

Está <span id="weather-parse">some description of temperature</span>

Hwa Young observedo: '<span id="observation">something</span>' as <span id="observation-timestamp">algum ou outra</span>.

<span id="hidden">Você pode ver a [[floresta quente|forest]]</span>

<span id="hidden">You can see the [[hot forest|forest]]</span>

<<script>>
//access the observation endpoint at /location
$.ajax({
    dataType: "json",
    url: "http://domesticscience.local:5000/location",
    success: function( result ) {
        $( "#observation" ).html( "<strong>" + result.value + "</strong>");
        $( "#observation-timestamp" ).html( "<strong>" + result.recorded_at + "</strong>");
        }
});


$.ajax({
    dataType: "json",
    url: "http://domesticscience.local:5000/hightown-ainsdale-brazil",
    success: function( result ) {
        // put the code you want to run each time you visit this twine page
        $( "#weather-temp" ).html( "<strong>" + result.value + "</strong>" );
        $( "#weather-timestamp" ).html( "<strong>" + result.recorded_at + "</strong>" );
        // for example
        if (result.value > 24){
            $( "#weather-parse" ).addClass("hot");
            $( "#weather-parse" ).html("hot");
            // use .show() to reveal the text in <span id = "hidden"></span>
            $( "#hidden" ).show();
            } else {
                $( "#weather-parse" ).html("<strong>cold</strong>");
                // use .hide() to hide the text in <span id = "hidden"></span>
                $( "#hidden" ).hide();
                $( "#weather-parse" ).removeClass("hot");
                }
            }
        });
<</script>>
```
## Moment of Truth

Now after copy and pasting that in to your Twine app, Test or Play it: if you're 
connected to the `domesticscience` WiFi and the CockleRake server is running it should work and the temperature and observation data should be displayed and parsed in the case of temperature with the if/else code.

Once your happy with it you need to send it to the pi apache server 

`HwaYoungsMac$ scp /path/to/my/amazingfile pi@domesticscience.local:/home/pi`

You'll need the password `raspberry` again.

You can then move it from `/home/pi` to the apache server `html` folder over ssh on the pi. So when connected by ssh to the pi:

`pi@domesticscience:~ $ sudo mv ~/amazingfile /var/www/html`

You'll need the password and you may need to change permissions of the new file with `sudo chmod 644 /location/of/myamazingfile.html`

You can then edit `/var/www/html/index.html` with vim so that one of the buttons can take you to your adventure.


### Good luck!


