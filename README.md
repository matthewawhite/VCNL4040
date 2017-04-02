# VCNL4040

NOTES regarding notation used and other facts about sensor: <br>
PS = proximity sensor <br>
ALS = ambient light sensor <br>
Reg_num = register number <br>
THD = threshold <br>
CONFG = configuration <br>
INT = interrupt (active high) <br>
Persistence = num consecutive hits to pull INT low <br>
I2C diagram shows data byte low then byte high all after command code <br>
For the PS, the output will be subtracted by the value set in PS_CANC <br>
Max PS detection with Kodak white card = 200 mm <br>
White channel can help determine kind of light source <br>
Read is | 1 and write is | 0
