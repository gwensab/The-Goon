#!/usr/bin/env python3
import sqlite3
import serial
import time
import datetime

con = sqlite3.connect('name.db')                                                      # connect to sqlite3 database
arduino = serial.Serial("/dev/***")                                                   # connect to arduino through serial port, replace *** with correct serial info
arduino.baudrate = 9600                                                               # set same baudrate as the arduino code
data = arduino.readline()                                                             # read arduino serial data      
time.sleep(2)
data = arduino.readline()                                                             # read arduino serial data
pieces = data.split(" ".encode(), 1)                                                  # split serial data into two parts, both bytes 

temp = pieces [0]                                                                     # temp value is piece 1 at spot 0
temp = temp.decode(encoding='utf-8', errors='strict')                                 # convert byte to string

hum = pieces[1]                                                                       # hum value is piece 2 at spot 1
hum = hum.decode(encoding='utf-8', errors='strict')                                   # convert byte to string
hum = hum.strip()                                                                     # remove white space from before and after hum data

t = datetime.datetime.now()                                                           # get current time
ti = t.strftime("%H:%M:%S")                                                           # format time in hours:minutes:seconds  

with con:
    cursor = con.cursor()
    insert_nums = """INSERT INTO weather(temperature, humidity, time) VALUES (?,?,?);"""        # where data will be inserted into sqlite3 table
    nums = (temp, hum, ti)                                                                      # what data will be inserted into sqlite3 table
    cursor.execute(insert_nums, nums)                                                           # excecute where and what data to insert
    con.commit()    
    cursor.close()


