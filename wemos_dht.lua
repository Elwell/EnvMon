-- DHT22 to MQTT Publisher
-- Andrew Elwell, 2016-06-08

cID='ESP-'..node.chipid()
m = mqtt.Client(cID, 60)
m:lwt("status/"..cID, "offline", 0, 1)

m:connect("10.1.1.251", 1883, 0, 1, function(client) print("connected")  end, 
                                 function(client, reason) print("failed reason: "..reason) end)

m:on("connect", function(client) m:publish("status/"..cID, "online", 0, 1) end)

function readDHT()
-- Read Temp from DHT22 on pin 4
status, temp, humi, temp_dec, humi_dec = dht.read(4)
if status == dht.OK then
    payload = ('{ "temp": '..temp..', "humidity": '..humi..' }')
    print(payload)
    m:publish('sensors/'..cID..'/json', payload,0,0)
elseif status == dht.ERROR_CHECKSUM then
    print( "DHT Checksum error." )
elseif status == dht.ERROR_TIMEOUT then
    print( "DHT timed out." )
end
end


tmr.alarm(0, 3000, tmr.ALARM_AUTO, function() readDHT(); end)
