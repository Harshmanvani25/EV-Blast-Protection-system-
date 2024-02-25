# EV-Blast-Protection-system-

Team Members:
1) Nisarg Patel  :- https://github.com/Nisargkpatel
2) Harsh Manvani :- https://github.com/Harshmanvani25

Project Overview:
The EV Blast Protection System is designed to mitigate the risks of electric vehicle (EV) battery explosions caused by internal electrical misbalance and external shocks. The system integrates various components including an ESP32 module, relay module, and DS18B20 temperature sensor to monitor and control the battery temperature effectively.

Key Components:
ESP32 Module: 
Central processing unit responsible for data processing and decision-making based on input from sensors.
Relay Module: 
Controls the BLDC motor cutoff mechanism to prevent battery explosion in case of overheating.
DS18B20 Temperature Sensor: 
Monitors the temperature of the battery in real-time to detect any anomalies.
Web Server Integration: Provides real-time monitoring of battery temperature and cutoff energy system readings to users.

Functionality:
Temperature Monitoring:

The DS18B20 temperature sensor continuously monitors the temperature of the EV battery.
Real-time temperature data is transmitted to the ESP32 module for analysis.

Cutoff Mechanism:
If the battery temperature exceeds the predefined cutoff threshold, the relay module cuts off power to the BLDC motor.
This prevents further charging or discharging of the battery, mitigating the risk of explosion.

Web Server Integration:
An integrated web server displays real-time battery temperature and cutoff energy system readings.
Users can remotely monitor the status of the battery and system operation through the web interface.

Safety Measures:
The system prioritizes consumer safety by preemptively cutting off power to the battery upon detecting abnormal temperature levels.
It aims to prevent potential explosions caused by internal electrical misbalance or external shocks.

Future Extension:
Implementation of the project on real life battery and developing system for external demage monitoring system.

Conclusion:
The EV Blast Protection System, developed by Nisarg Patel and Harsh Manwani, offers a robust solution to enhance the safety of electric vehicles. By leveraging advanced sensing and control mechanisms, the system effectively mitigates the risks associated with battery explosions, ensuring a safer and more reliable EV operation.
