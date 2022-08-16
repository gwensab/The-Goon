# Step By Step Instructions


### Step 1 : Setting Up Raspberry Pi

* This will require the Raspberry Pi, Raspberry Pi battery pack,  micro SD card, mouse, keyboard, and monitor
* Insert micro SD card into computer and install the [Raspberry Pi Imager](https://www.raspberrypi.com/software/) for your device
* Insert micro SD card into Raspberry Pi
* Connect keyboard, mouse, monitor, and power to Raspberry Pi
* Follow prompts that appear on your monitor to finalize the set up of Raspberry Pi

I found the instructions at [projects.raspberrypi.org](https://projects.raspberrypi.org/en/projects/raspberry-pi-setting-up/0) to be helpful

### Step 2 : Connect All Components

insert image of wiring here
* Connect Arduino Mega board to Raspberry Pi, breadboard, DHT11, BTS7960, and SR04 according to the wiring table and image above
* Arrange components onto the base of the robot

### Step 3 : Program Arduino Using Raspberry Pi
![Screenshot (1)](https://user-images.githubusercontent.com/109180573/184966235-4a23b9b9-42e2-42c9-b986-61d1d2e17b15.jpg)
*  Download Arduino IDE by opening [arduino.cc](https://www.arduino.cc/en/software/OldSoftwareReleases#previous) in the Raspberry Pi desktop
*  I used the newest update ``Arduino 1.8.18`` then selected ``Linux ARM 32`` 
*  Open a terminal and type  ``tar -xvzf arduino-1.8.18-linuxarm.tar.xz`` to extract the file
*  Then type ``cd arduino-1.8.18`` to go to the directory
*  In the directory. type ``./arduino`` to run the Arduino IDE

I found the instructions at [create.arduino.cc](https://create.arduino.cc/projecthub/ruchir1674/how-to-interface-arduino-with-raspberrypi-504b06) to be helpful

### Step 4 : Download Arduino Libraries

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

* To install SQLite3, type ``sudo apt install sqlite3`` into the terminal
* Type ``sqlite3 test.db`` into the terminal to either create or open a database named ``test`` 
* Once in this database, create a table using the following code, ``CREATE TABLE weather(row_num INTEGER PRIMARY KEY, temperature TEXT, humidity TEXT, time TEXT);``

I found the instructions at [pimylifeup.com](https://pimylifeup.com/raspberry-pi-sqlite/) to be helpful

### Step 7 : Twitter Developer Account & Tweepy

* To create a twitter developer account

### Step 8 : Python Files 

* From Raspberry Pi desktop, open the ``Applications Menu`` -> ``Programming`` -> ``Thonny Python IDE``
* Create a new python file and input the [get temp](get_temp.py) code
* Test that this works properly by running ``python3 ./get_temp.py`` in the terminal (this should add data to sqlite table which can be viewed [here](https://sqliteviewer.app/))
* Create another new python file and input the [post twitter](post_twitter.py) code
* Test that this works properly by checking the twitter account you are using for new posts

### Step 9 : Cronjob

* Create a cronjob to run the python codes every x minutes
* Type 'crontab -e' in the terminal to create and edit a file
* At the bottom of the file that opens, type ``*/x * * * * python3 ./.py && python3 postConditions.py``
(where x is the minute interval you want to run the code at)
