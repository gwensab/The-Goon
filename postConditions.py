import sqlite3
import tweepy
import datetime
from datetime import time

con = sqlite3.connect('irish.db')
cursor = con.cursor()

consumer_key = 'xxx'
consumer_secret = 'xxx'
access_token = 'xxx'
access_token_secret = 'xxx'

auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
auth.set_access_token(access_token, access_token_secret)
api = tweepy.API(auth)

t = datetime.datetime.now()
minutes = t.strftime("%M")
min = float(minutes)%1

cursor.execute("SELECT temperature FROM weather ORDER BY row_num DESC LIMIT 1")
temperatur = cursor.fetchone()
temperature = ''.join(temperatur)

cursor.execute("SELECT humidity FROM weather ORDER BY row_num DESC LIMIT 1")
humidit = cursor.fetchone()
humidity = ''.join(humidit)

cursor.execute("SELECT time FROM weather ORDER BY row_num DESC LIMIT 1")
tim = cursor.fetchone()

time = ''.join(tim)


if (min == 0):
    tweettopublish = 'Temperature: '+temperature+'°F \nHumidity: '+humidity+'% \nTime: '+time
    
api.update_status(tweettopublish)
print(tweettopublish)