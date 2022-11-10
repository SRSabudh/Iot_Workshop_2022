import paho.mqtt.client as mqttclient
from datetime import datetime
import time
import json
import csv

connected = False

fieldnames_1 = ["x_axis", "v1"]

with open('val_1_data_csv.csv', 'w') as csv_file:
    csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames_1)
    csv_writer.writeheader()

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("client is connected")
        global connected
        connected = True
    else:
        print("client is not connected")


def on_message(client, userdata, message):
    TimeStamp = datetime.now()
    print("Message recieved : " + str(message.payload.decode("utf-8")))
    print("message topic : " + str(message.topic))

    if(str(message.topic) == "esp32/value1"):

        val1 = float(message.payload.decode("utf-8"))

        with open("val_1_data_csv.csv", 'a') as csv_file:
            csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames_1)

            val1_data = {
                "x_axis": TimeStamp,
                "v1": val1
            }

            csv_writer.writerow(val1_data)
            

    # elif(str(message.topic) == "esp32/value2"):
    #     val2 = float(message.payload.decode("utf-8"))
    #     with open("val_2_data.json", "r+") as f2:
    #         val2_list_object = []
    #         val2_list_object = json.load(f2)
    #         val2_list_object.append({
    #         "TimeStamp" : TimeStamp,
    #         "val1" : val2
    #     })
    #         f2.seek(0)
    #         json.dump(val2_list_object, f2, indent=4, separators=(',',':'))
    #         # f2.close()
    # else:
    #     print("Not a target topic..!") # useless code


broker_address = "192.168.0.148" # ip address of broker
port = 1883 # port number in integer
user = ""
password = ""

client = mqttclient.Client("MQTT")
client.username_pw_set(user, password=password)
client.on_connect = on_connect
client.on_message=on_message
client.connect(broker_address,port=port)

client.loop_start()

client.subscribe("esp32/value1")
client.subscribe("esp32/value2")

while connected != True:
    time.sleep(0.2)

try:
    while True:
        time.sleep(0.2)
 
except KeyboardInterrupt:
    client.disconnect()
    client.loop_stop()