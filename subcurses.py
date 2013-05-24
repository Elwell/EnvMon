#!/usr/bin/python

# subcurses - MQTT subscriber test for Pawsey systems
# uses curses to keep info in one place
# Andrew Elwell <Andrew.Elwell@ivec.org> 2013-05-24


import mosquitto
import curses
import time
topiclist = []


def on_connect(mosq, userdata, rc):
    print("Connection returned " + str(rc))

def on_message(mosq, userdata, msg):
    if msg.topic in topiclist:
        offset = 3 + topiclist.index(msg.topic)
    else:
        topiclist.append(msg.topic)
        offset = 3 + topiclist.index(msg.topic)
    stats_txt.addstr(offset, 2, msg.topic)
    stats_txt.addstr(offset, 52, msg.payload)
    stats_txt.refresh()
    #print "%s Topic: %s, Value %s)" % (offset,msg.topic, msg.payload)

client = mosquitto.Mosquitto("TestSubscriber-AE_001")
client.on_message = on_message
client.on_connect = on_connect
#client.connect("breathalyser.ivec.org")
client.connect("test.mosquitto.org")
client.subscribe("$SYS/broker/load/#")

# curses handling
stdscr = curses.initscr()
curses.noecho()
curses.cbreak()
curses.curs_set(0)
if curses.has_colors():
    curses.start_color()


# start 'er up capt'n
stdscr.addstr("Pawsey Centre Monitoring (via MQTT)", curses.A_REVERSE)
stdscr.chgat(-1,curses.A_REVERSE)

stats = curses.newwin(curses.LINES-2,curses.COLS,1,0)
stats_txt = stats.subwin(curses.LINES-6,curses.COLS-4,3,2)

stats.box()

stdscr.noutrefresh()
stats.noutrefresh()

curses.doupdate()
while client.loop() == 0:
    pass


curses.nocbreak()
stdscr.keypad(0)
curses.echo()
curses.endwin()
