#!/usr/local/bin/python3

# wrapper to gather latest (text format) BLANkET data - https://epa.tas.gov.au/epa/air/monitoring-air-pollution/real-time-air-quality-data-for-tasmania and stick in DB

influxdb = 'http://192.168.1.250:8086/write?db=epa&precision=s'
pm_url   = 'https://epa.tas.gov.au/air/live/epa_tas_latest_particle_data.txt'
met_url  = 'https://epa.tas.gov.au/air/live/epa_tas_latest_met_data.txt'

import requests
import time
import csv

data = requests.get(pm_url)

cr = csv.reader(data.text.splitlines(), skipinitialspace=True, delimiter=',')
my_list = list(cr)
postdata = ''
for row in my_list:
    if (row[0].startswith('#')):
        if ('YYYY' in row[0]):
            #print(row)
            fdate=row[0][-8:]
            ftime=row[1][8:14]
        continue
    #print(row)
    if (row[1] == "999999"): # Bail out, false
        continue
    tstr = fdate + row[1] + "UTC" # not really UTC, but seems to work magically
    t_tuple = time.strptime(tstr,"%Y%m%d%H%M%S%Z")
    ts = time.mktime(t_tuple)
    if (row[1] > ftime): # we've rolled over header but not reading timestamp
        ts -= 86400 # knock a day off
    postdata += ("airquality,site={} pm25={}i,pm10={}i {}\n".format(row[0],row[2],row[3],int(ts)))

r = requests.post(influxdb, data=postdata)
if (r.status_code != 204):
    print(r.text)
