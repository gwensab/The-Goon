# Instructions

### Step 1 : Assemble Components
![IMG_7684ggg](https://user-images.githubusercontent.com/109180573/185223412-cf8ddd1a-7b0b-4f34-8793-1fad3c96eb23.JPG)

* Wire according to the above image and the wiring table from the [read me file](https://github.com/gwensab/The-Goon/blob/main/README.md#arduino-wiring)
* Connect the negative and positive channels on the breadboard to the 5V and GND pins on the Arduino board
* Connect the two motor drivers first, using Arduino digital pins 5-12
* Then connect the three ultrasonic sensors, using Arduino digital pins 44-46
* Finally connect the temperature sensor, using Arduino digital pin 4

Final wiring & connections should look something like this:

![yyIMG_7630 (2)](https://user-images.githubusercontent.com/109180573/185226902-610aaf2e-62dd-48d2-a13f-4242fdbfbed7.JPG)![yyIMG_7632](https://user-images.githubusercontent.com/109180573/185226909-b304f14e-1c41-446b-b360-4ebca8283e68.JPG)![yyIMG_7642](https://user-images.githubusercontent.com/109180573/185226529-0a9f7ba4-5af3-4f45-b4c4-b2adbf5193df.JPG)![yyIMG_7648](https://user-images.githubusercontent.com/109180573/185226540-10f2317f-871d-4535-bef3-08c8e353438e.JPG)![yyIMG_7649](https://user-images.githubusercontent.com/109180573/185226551-40ccd65f-96f5-486f-a337-030fe36e88a9.JPG)![yyIMG_7655](https://user-images.githubusercontent.com/109180573/185226560-92567c0d-ec23-4d77-9ee7-33b1ab701a54.JPG)![yyIMG_7660](https://user-images.githubusercontent.com/109180573/185226569-ecbafe6f-1f21-4335-89cd-55e3d7b048b6.JPG)


### Step 2 : Set Up Raspberry Pi
![Screenshot (4)](https://user-images.githubusercontent.com/109180573/184982249-e00b766c-4fc0-4566-a87c-1a39a82448d7.png)
* This will require the Raspberry Pi, Raspberry Pi battery pack,  micro SD card, mouse, keyboard, and monitor
* Insert micro SD card into computer and install the [Raspberry Pi Imager](https://www.raspberrypi.com/software/) for your device
* Insert micro SD card into Raspberry Pi
* Connect keyboard, mouse, monitor, and power to Raspberry Pi
* Follow prompts that appear on your monitor to finalize the set up of Raspberry Pi

I found the instructions at [projects.raspberrypi.org](https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up/0) to be helpful

### Step 3 : Program Arduino Using Raspberry Pi
![Sscreenshot (1)](https://user-images.githubusercontent.com/109180573/184982452-6683fa86-a8aa-4e98-9e69-f50120d01984.jpg)

Arduino is a microcontroller board while Raspberry Pi is a microprocessor based mini computer. So we will use the Raspberry Pi to store the code for the Arduino and then the Arduino will run the motors, sensors, ect.

*  Download Arduino IDE by opening [arduino.cc](https://www.arduino.cc/en/software/OldSoftwareReleases#previous) in the Raspberry Pi desktop
*  I used the newest update ``Arduino 1.8.18`` then selected ``Linux ARM 32`` 
*  In the File Manager, extract the file by right clicking on it
*  Then type ``cd arduino-1.8.18`` to open to the directory
*  In the directory, type ``./arduino`` to run the Arduino IDE

I found the instructions at [create.arduino.cc](https://create.arduino.cc/projecthub/ruchir1674/how-to-interface-arduino-with-raspberrypi-504b06) to be helpful

### Step 4 : Download Arduino Libraries
![Screenshot (3)](https://user-images.githubusercontent.com/109180573/184982325-f3d4b353-7043-4f8a-9781-bb95bb17d683.png)
* In the Arduino window, select ``Sketch`` -> ``Include`` -> ``Manage Libraries`` then download
* * DHT sensor library  by  Adafruit
* * Adafruit Unified Sensor  by  Adafruit
* * BTS7960  by  Luis Llamas
* * NewPing  by  Tim Eckel

### Step 5 : Arduino Code

* Input code from **temp_and_drive.ino** into Arduino IDE
* In the ``Tools`` tab, check that ``Board``, ``Processor``, and ``Port`` all have the correct values
* Verify and Upload code to the board

### Step 6 : Install SQLite3
![gggIMG_7667 (2)](https://user-images.githubusercontent.com/109180573/185209796-42e819a3-f9af-4558-aaee-c14c819b97d5.jpg)
* To install SQLite3, type ``sudo apt install sqlite3`` into the terminal
* Type ``sqlite3 test.db`` into the terminal to either create or open a database named ``test`` 
* Once in this database, create a table using the following code, ``CREATE TABLE weather(row_num INTEGER PRIMARY KEY, temperature TEXT, humidity TEXT, time TEXT);``

I found the instructions at [pimylifeup.com](https://pimylifeup.com/raspberry-pi-sqlite/) to be helpful

### Step 7 : Twitter Developer Account 
![Sscreenshot (5)](https://user-images.githubusercontent.com/109180573/185198741-8f8f7dce-1b53-4fcc-a700-16c30b289921.png)
* To create a twitter developer account go to [developer.twitter.com](https://developer.twitter.com/en/application/use-case)
* Answer the prompted questions and navigate to the developer site from your dashboard to create an app
* When your app is created, twitter will generate consumer API keys and access tokens (make sure you copy these somewhere, they are only shown once)
* Ensure that access level is set to read and write
* Check that your developer account is linked to the twitter user that you want to post your tweets

I found the instructions at [towardsdatascience.com](https://towardsdatascience.com/building-a-twitter-bot-with-python-89959ef2607f) to be helpful

### Step 8 : Send Data To Database
![gggIMG_7668](https://user-images.githubusercontent.com/109180573/185209076-b4461f68-7a7d-474c-a99d-617f75a15228.JPG)
* From Raspberry Pi desktop, open the ``Applications Menu`` -> ``Programming`` -> ``Thonny Python IDE``
* Create a new python file and input the [get temp](get_temp.py) code
* This code will gather the temperature data from the arduino serial monitor, then store it in the table you created in the sqlite database
* Test that this works properly by running ``python3 ./get_temp.py`` in the terminal (this should add data to sqlite table which can be viewed [here](https://sqliteviewer.app/))

### Step 9 : Send To Twitter With Tweepy

* Follow the first bullet point from step 9 to create another new python file and input the [post twitter](post_twitter.py) code
* This code will pull data from the table in your sqlite database, format it into a readable tweet, and then upload that post to twitter
* Test that this works properly by running ``python3 ./post_twitter.py`` in the terminal (if a new tweet is posted to the linked twitter account, it's working)

### Step 10 : Cronjob
![gggIMG_7682](https://user-images.githubusercontent.com/109180573/185211644-c1af438f-6919-4b73-870f-677947c255d1.JPG)
* Create a cronjob to run the python codes every x minutes
* Type 'crontab -e' in the terminal to create and edit a file
* At the bottom of the file that opens, type ``*/x * * * * python3 ./get_temp.py && python3 post_twitter.py``
(where x is the minute interval you want to run the code at)

More information about cronjobs can be found at [raspiserver.com](https://raspiserver.com/crontab-on-raspberry-pi/)
