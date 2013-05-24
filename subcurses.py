#!/usr/bin/python

# subcurses - MQTT subscriber test for Pawsey systems
# uses curses to keep info in one place
# Andrew Elwell <Andrew.Elwell@ivec.org> 2013-05-24


import mosquitto
import curses
import time
topiclist = []

# CHANGE THESE TO SUIT ENVIRONMENT
broker = "test.mosquitto.org"
topics = "$SYS/broker/load/#"

def on_connect(mosq, userdata, rc):
    stdscr.addstr(curses.LINES-1,0,"Connection to %s returned %s" % (broker,rc))
    stdscr.noutrefresh()
    curses.doupdate()

def on_message(mosq, userdata, msg):
    if msg.topic in topiclist:
        offset = topiclist.index(msg.topic)
    else:
        topiclist.append(msg.topic)
        offset = topiclist.index(msg.topic)
    stats_txt.addstr(offset, 0, msg.topic)
    stats_txt.addstr(offset, 50, msg.payload)
    stats_txt.noutrefresh()
    curses.doupdate()

client = mosquitto.Mosquitto()
client.on_message = on_message
client.on_connect = on_connect
client.connect(broker)
client.subscribe(topics)

# curses handling
stdscr = curses.initscr()
stdscr.nodelay(1)
curses.curs_set(0)
if curses.has_colors():
    curses.start_color()


# start 'er up capt'n
stdscr.addstr("Pawsey Centre Monitoring (via MQTT)", curses.A_REVERSE)
stdscr.chgat(-1,curses.A_REVERSE)
stdscr.addstr(curses.LINES-1,curses.COLS-10,"q to Quit")

stats = curses.newwin(curses.LINES-2,curses.COLS,1,0)
stats_txt = stats.subwin(curses.LINES-6,curses.COLS-4,2,2)

stats.box()

stdscr.noutrefresh()
stats.noutrefresh()

while client.loop() == 0:
    if stdscr.getch() == ord("q"): 
        break
    pass


curses.nocbreak()
stdscr.keypad(0)
curses.echo()
curses.endwin()
