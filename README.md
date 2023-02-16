# Hovercraft control interface
This is a control interface for a hovercraft with one motor for elevation, one motor for throttle (for going forward) and one servo for steering.

## Troubleshooting
If you have problems with uploading the files from the data folder then try adding the ```index.js``` and the ```style.css``` in the ```index.html```. Then take the code from the ```index.html``` and put it inside of this lines of code (you will have to add them in your ```hovercraft.ino``` file above the setup method):
````
const char index_html[] PROGMEM = R"rawliteral(
    //put your html here
)rawliteral";
````
Then put these lines in the setup method in the ```hovercreft.ino```:
````
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", index_html);
            Serial.println("HTML loaded"); });
````