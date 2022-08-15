#!/usr/bin/env python3
import sqlite3
import serial
import time
import datetime

con = sqlite3.connect('irish.db')
arduino = serial.Serial("/dev/ttyACM0")
arduino.baudrate = 9600
data = arduino.readline()
time.sleep(2)
data = arduino.readline()
pieces = data.split(" ".encode(), 1)

temp = pieces [0]
temp = temp.decode(encoding='utf-8', errors='strict')

hum = pieces[1]
hum = hum.decode(encoding='utf-8', errors='strict')
hum = hum.strip()

t = datetime.datetime.now()
ti = t.strftime("%H:%M:%S")

with con:
    cursor = con.cursor()
    insert_nums = """INSERT INTO weather(temperature, humidity, time) VALUES (?,?,?);"""
    nums = (temp, hum, ti)
    cursor.execute(insert_nums, nums)
    con.commit()
    cursor.close()


