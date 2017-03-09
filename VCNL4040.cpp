#include "VCNL4040.h"

VCNL4040::VCNL4040(PinName sda, PinName scl) : i2c(sda, scl) {
    i2c.frequency(80 * 1000); // 50 kHz, min is 10 kHz, max is 400 kHz
    // Standard mode freq (10, 100) -> Fast mode (10, 400)
}

void VCNL4040::set_als_cmd_disable_int(void) {
    /*              --ALS_CONF--Low byte--
        ALS_SD = [0] , Power on
        ALS_INT = [0] , ALS interrupt disabled: NOTE text said enabled
        ALS_PERS = [0,1] , ALS interrupt persistence = 2
        Reserved = [0,0] , Default
        ALS_IT = [0,1] , ALS integration time = 160 ms
                    --0100,0100--
    */
    /*              --ALS_CONF--high byte--
        Reserve = [0] , Power on
                    --0000,0000--
    */
    // I2Cm_bWriteBytes(VCNL4040_SLAVE_ADD, ALS_CONF, 0x44, 0x00, 2);
    // Combine lsb and msb:
    BYTE lsb = 0x44;
    BYTE msb = 0x00;
    send[0] = ALS_CONF;
    send[1] = lsb;
    send[2] = msb; //is the order correct?
    i2c.write(VCNL4040_SLAVE_ADD, (char*)send, 3);
 }

void VCNL4040::set_als_cmd_enable_int(void) {
    /*              --ALS_CONF--Low byte--
        ALS_SD = [0] , Power on
        ALS_INT = [1] , ALS interrupt enable
        ALS_PERS = [0,1] , ALS interrupt persistence = 2
        Reserved = [0,0] , Default
        ALS_IT = [0,1] , ALS integration time = 160 ms
                    --0100,0110--
    */
    /*              --ALS_CONF--high byte--
        Reserve = [0] , Power on
                    --0000,0000--
    */
    // I2Cm_bWriteBytes(VCNL4040_SLAVE_ADD, ALS_CONF, 0x46, 0x00, 2);

    BYTE lsb = 0x46;
    BYTE msb = 0x00;
    send[0] = ALS_CONF;
    send[1] = lsb;
    send[2] = msb; //is the order correct?
    i2c.write(VCNL4040_SLAVE_ADD, (const char*)send, 3);
}

void VCNL4040::set_als_int_thd(WORD thd_high, WORD thd_Low) {
    BYTE lsb, msb; // TODO: check if adding this helps
    lsb = (BYTE)(thd_high & 0x00ff);
    msb = (BYTE)(thd_high & 0 >> 8);
    send[0] = ALS_THDH;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, (const char*)send, 3);
    //I2Cm_bWriteBytes(VCNL4040_SLAVE_ADD, ALS_THDH, lsb, msb, 2);
    lsb = (BYTE)(thd_Low & 0x00ff);
    msb = (BYTE)(thd_Low & 0 >> 8);
    send[0] = ALS_THDL;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, (const char*)send, 3);
    //I2Cm_bWriteBytes(VCNL4040_SLAVE_ADD, ALS_THDL, lsb, msb, 2);
}

void VCNL4040::set_ps_cmd1(void) {
    /*              --PS_CONF1--Low byte--
        PS_SD = [0] , PS Power on
        PS_IT = [1,1,1] , PS IT = 8T
        PS1_PERS = [0,0] , PS1 interrupt persistence = 1
        PS_DUTY = [0,1] , PS1 and PS2 duty = 1/80
                    --0100,1110--
    */
    /*              --PS_CONF2--high byte--
        PS1_INT = [0,0] , disable
        Reserved = [0]
        PS_HD = [1], PS output is 16 bits //NOTE changed from 12 to 16 bits
        Reserved = [0,0]
        Reserved = [0,0]
                    --0000,1000-- CHANGED from --0000,0011-- (0x03) to (0x08)
    */
    // TODO: check if changing below PS_CONF1 to PS_CONF1_2 works
    // I2Cm_bWriteBytes(VCNL4040_SLAVE_ADD, PS_CONF1_2, 0x10, 0x0b, 2);
    char lsb = 0x4E;
    char msb = 0x08;
    char send[3];
    send[0] = PS_CONF1_2;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
}

void VCNL4040::set_ps_cmd2(void) {
    /*              --PS_CONF3--Low byte--
        Reserved = [0]
        Reserved = [0,0]
        PS_FOR = [0] , Active force mode disable  NOTE where did this come from?
        PS_SMART_PERS = [0] , disable smart persistence
        PS_AF = [0] , PS auto mode (CHANGED Active force mode disabled)
        PS_TRIG = [0] ,No Trigger
        Reserved = [0, 0]
                    --0000,0000--
    */
    /*              --PS_MS--high byte-- NOTE for logical operation
        Reserve = [0], White channel enable
        PS_MS = [0] , Proximity normal operation with interrupt function
        Reserve = [0,0,0]
        LED_I = [1,1,1]., LED current = 200mA
                    --0000,0111--  CHANGED from --0010,0000-- (check doc pg.11)
    */
    // TODO: research into active force trigger
    // CHANGED below 0x20 to 0x02
    //I2Cm_bWriteBytes(VCNL4040_SLAVE_ADD, PS_CONF3, 0x10, 0x02, 2);
    char lsb = 0x00;
    char msb = 0x07;
    char send[3];
    send[0] = PS_CONF3;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
}

void VCNL4040::set_ps_thd(char ps1_thdl, char ps1_thdh) {
    // NOTE before using function, learn what the values for threshold actually
    // represent, then implement the method

    // ps1_thdl = 0x03 , ps1_thdh = 0x08
    // I2Cm_bWriteBytes(BYTE SlaveAddr, BYTE Reg_num, BYTE Data_lsb, BYTE Data_msb, BYTE bCnt);
    //I2Cm_bWriteBytes(VCNL4040_SLAVE_ADD, PS1_THD, ps1_thdl, ps1_thdh, 2);
    // char send[3];
    // send[0] = PS1_THDL;
    // send[1] = ps1_thdl;
    // send[2] = ps1_thdh;
    //
    //
    // send[0] = PS_CONF3;
    // i2c.write(VCNL4040_SLAVE_ADD, send, 3);
}

WORD VCNL4040::read_VCNL4040_als(void) {
    BYTE lsb,msb;
    WORD als_value;
    send[0] = ALS_DATA;
    // I2Cm_fReadBytes(VCNL4040_SLAVE_ADD, ALS_DATA, buff,2);
    i2c.write(VCNL4040_SLAVE_ADD, (const char*)send, 1);
    i2c.read(VCNL4040_SLAVE_ADD, (char*)received, 2);
    lsb = received[0];
    msb = received[1];
    als_value = (WORD)msb;
    als_value = ((als_value << 8) | (WORD)lsb); // CHANGED from LSB to lsb
    return als_value;
}

void VCNL4040::read_VCNL4040_int_flag(void) {
    // I2Cm_fReadBytes(VCNL4040_SLAVE_ADD, INT_FLAG, buff,2);
    BYTE msb;
    send[0] = INT_FLAG;
    i2c.write(VCNL4040_SLAVE_ADD, send, 1);
    i2c.read(VCNL4040_SLAVE_ADD, received, 2);
    msb = received[1];
    for (int i = 0; i < 8; i++) { //Made i an int
        if ((msb >> i) & 0x01)
            int_flag[i] = TRUE;
        else
            int_flag[i] = FALSE;
    }
}

void VCNL4040::read_VCNL4040_ID(WORD *id) {
    char num;
    num = 0x0C;
    char rec[2];
    i2c.write(VCNL4040_SLAVE_ADD, &num, 1, true);
    i2c.read(VCNL4040_SLAVE_ADD, rec, 2);
    *id = ((WORD)(rec[1]) << 8) | (WORD)(rec[0]);
}

WORD VCNL4040::read_VCNL4040_ps(void) {
    WORD ps_value;
    // Read 2 bytes from PS_DATA register into array received
    // I2Cm_fReadBytes(VCNL4040_SLAVE_ADD, PS_DATA, buff, 2);
    char send = PS_DATA;
    char rec[2];
    i2c.write(VCNL4040_SLAVE_ADD, &send, 1, true);
    i2c.read(VCNL4040_SLAVE_ADD, rec, 2);

    ps_value = (WORD)rec[1];
    ps_value = ((ps_value << 8) | (WORD)rec[0]);
    // Example math:
    // msb = (WORD) 0000 0110 = 0000 0000 0000 0110
    // lsb = 0010 0101
    // ps_value << 8 = 0000 0110 0000 0000
    // (WORD)lsb = 0000 0000 0010 0101
    // ps_value | lsb = 0000 0110 0010 0101, return WORD
    return ps_value;
}
