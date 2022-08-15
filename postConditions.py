import sqlite3
import tweepy
import datetime
from datetime import time

con = sqlite3.connect('name.db')                                                        # connect to sqlite3 database
cursor = con.cursor()

consumer_key = 'xxx'                                                                    # replace xxx with your twitter developer keys and tokens
consumer_secret = 'xxx'
access_token = 'xxx'
access_token_secret = 'xxx'

auth = tweepy.OAuthHandler(consumer_key, consumer_secret)                               # use twitter keys and tokens to authenticate twitter access
auth.set_access_token(access_token, access_token_secret)
api = tweepy.API(auth)

t = datetime.datetime.now()                                                             # get real time
minutes = t.strftime("%M")                                                              # get minutes
min = float(minutes)%1                                                                  # check if minutes are divisible by 1

cursor.execute("SELECT temperature FROM weather ORDER BY row_num DESC LIMIT 1")         # get newest temperature value from weather table
temperatur = cursor.fetchone()                                                          # select row from table
temperature = ''.join(temperatur)                                                       # get rid of characters before or after temperature value

cursor.execute("SELECT humidity FROM weather ORDER BY row_num DESC LIMIT 1")            #
humidit = cursor.fetchone()                                                             # select row from table
humidity = ''.join(humidit)                                                             # get rid of characters before or after humidity value

cursor.execute("SELECT time FROM weather ORDER BY row_num DESC LIMIT 1")                #
tim = cursor.fetchone()                                                                 # select row from table
time = ''.join(tim)                                                                     # get rid of characters before or after time value


if (min == 0):                                                                                      # if minutes are divisible by one
    tweettopublish = 'Temperature: '+temperature+'°F \nHumidity: '+humidity+'% \nTime: '+time       # create tweet containing temperature, humidity, and time
    
api.update_status(tweettopublish)                                                       # update tweet to twitter
print(tweettopublish)                                                                   # print tweet
