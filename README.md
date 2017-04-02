#VCNL4040

NOTES regarding notation used and other facts about sensor:
PS = proximity sensor, ALS = ambient light sensor,
Reg_num = register number, THD = threshold, CONFG = configuration
INT = interrupt (active high),
Persistence = num consecutive hits to pull INT low,
I2C diagram shows data byte low then byte high all after command code
For the PS, the output will be subtracted by the value set in PS_CANC
Max PS detection with Kodak white card = 200 mm
White channel can help determine kind of light source
Read is | 1 and write is | 0
