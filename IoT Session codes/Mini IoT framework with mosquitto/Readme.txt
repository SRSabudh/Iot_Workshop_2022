This file is related to IoT with cloud baiscs session in sabudh by Saurabh Ritu.

First you need to install mosquitto on your system.

*> esp32 client code :- it is used to send data to mosquitto broker on a particular topic

*> mqtt_data_dumper_cvs.py :- it is a python code used to subscribe to a particular topic, fetch data and store it in csv file format / or anything up to you.

*> visualization_basic :- this python code is used to visulaize the real-time data in graph using matplotlib & its FuncaAnimation feature.

*> Basic Operation on Mosquitto:-
	#> Set up mosquitto Broker ( with and without -c & -v flags ) [ mymosquitto.conf is your custom configuration file ]
		=> mosquitto -c mymosquitto.conf -v

	#> Publish Msg to a topic
		=> mosquitto_pub -h localhost -m "on" -t home/myroom/sidebulb

	#> Subscribe to the topic 
		=> mosquitto_sub -h localhost -t home/myroom/sidebulb


*> Note:- you have to downoad paho library from pip before using it inside python,
	Use:- pip install paho-mqtt.

	Link More about PubSubClient :- https://pubsubclient.knolleary.net/api#PubSubClient

	Link Mosquitto :- https://mosquitto.org/download/	