# @gwensab last edited 8/16/2022
# The purpose of this code is to access a twitter developer account remotely, gather data from sqlite table, format that data, and send it to twitter

import sqlite3
import tweepy
import datetime
from datetime import time

con = sqlite3.connect('name.db')                                                        # connect to sqlite3 database ("name" is a placeholder for your database name)
cursor = con.cursor()

consumer_key = '***'                                                                    # remember the twitter API keys and access tokens?
consumer_secret = '***'                                                                 # input those values here, in place of ***
access_token = '***'                                                                    
access_token_secret = '***'                                                             

auth = tweepy.OAuthHandler(consumer_key, consumer_secret)                               # use twitter API keys 
auth.set_access_token(access_token, access_token_secret)                                # and access tokens
api = tweepy.API(auth)                                                                  # to authenticate connection to your twitter account

t = datetime.datetime.now()                                                             # get real time
minutes = t.strftime("%M")                                                              # get minutes
min = float(minutes)%1                                                                  # convert to float value and check if minutes are divisible by 1

cursor.execute("SELECT temperature FROM table_name ORDER BY row_num DESC LIMIT 1")      # get newest temperature value from table
temperatur = cursor.fetchone()                                                          # select row from table
temperature = ''.join(temperatur)                                                       # turn value into a string

cursor.execute("SELECT humidity FROM table_name ORDER BY row_num DESC LIMIT 1")         # get newest humidity value from table
humidit = cursor.fetchone()                                                             # select row from table
humidity = ''.join(humidit)                                                             # turn value into a string

cursor.execute("SELECT time FROM table_name ORDER BY row_num DESC LIMIT 1")             # get newest time value from table
tim = cursor.fetchone()                                                                 # select row from table
time = ''.join(tim)                                                                     # turn value into a stirng


if (min == 0):                                                                                      # if minutes are divisible by one
    tweettopublish = 'Temperature: '+temperature+'??F \nHumidity: '+humidity+'% \nTime: '+time       # create formatted tweet containing temperature, humidity, and time
    
api.update_status(tweettopublish)                                                       # update tweet to twitter
print(tweettopublish)                                                                   # print tweet
