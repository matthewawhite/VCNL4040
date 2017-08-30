# VCNL4040
This is the code developed for the VCNL4040 proximity sensor for the purpose of increasing better ball detection for the robots.

# Setup
Check the pin locations for the sensor in the datasheet and connect to the respective pins on a microcontroller (the software was developed with the ARM Mbed for testing and may not work with other microcontrollers)

# Data
At the moment, the data the sensor is being returned is not in any understandable units such as centimeter (cm), and so data was collected and analyzed using Excel to create a rough formula that converts the returned value to cm however this needs to be vastly improved.
