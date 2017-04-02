#include "VCNL4040.h"

VCNL4040::VCNL4040(PinName sda, PinName scl) : i2c(sda, scl) {
    i2c.frequency(80 * 1000);
    // Standard mode freq [10, 100] Hz -> Fast mode [10, 400] Hz
}

void VCNL4040::set_als_cmd_disable_int(void) {
    /*              --ALS_CONF--Low byte--
        ALS_SD = [0] , Power on
        ALS_INT = [0] , ALS interrupt disabled
        ALS_PERS = [0,1] , ALS interrupt persistence = 2
        Reserved = [0,0] , Default
        ALS_IT = [0,1] , ALS integration time = 160 ms
                    --0100,0100--
    */
    /*              --ALS_CONF--high byte--
        Reserve = [0] , Power on
                    --0000,0000--
    */
    char lsb = 0x44;
    char msb = 0x00;
    send[0] = ALS_CONF;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
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
    char lsb = 0x46;
    char msb = 0x00;
    send[0] = ALS_CONF;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
}

void VCNL4040::set_als_int_thd(short thd_high, short thd_Low) {
    char lsb = (char)(thd_high & 0x00ff);
    char msb = (char)(thd_high & 0 >> 8);
    send[0] = ALS_THDH;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
    lsb = (char)(thd_Low & 0x00ff);
    msb = (char)(thd_Low & 0 >> 8);
    send[0] = ALS_THDL;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
}

void VCNL4040::set_ps_cmd1(void) {
    /*              --PS_CONF1--Low byte--
        PS_SD = [0] , PS Power on
        PS_IT = [1,1,0] , PS IT = 4T
        PS1_PERS = [0,0] , PS1 interrupt persistence = 1
        PS_DUTY = [1,0] , PS1 and PS2 duty = 1/160
                    --1000,1100--
    */
    /*              --PS_CONF2--high byte--
        PS1_INT = [0,0] , disable
        Reserved = [0]
        PS_HD = [1], PS output is 16 bits
        Reserved = [0,0]
        Reserved = [0,0]
                    --0000,1000--
    */
    char lsb = 0x8C;
    char msb = 0x08;
    send[0] = PS_CONF1_2;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
}

void VCNL4040::set_ps_cmd2(void) {
    /*              --PS_CONF3--Low byte--
        Reserved = [0]
        Reserved = [0,0]
        PS_FOR = [0] , Active force mode disable
        PS_SMART_PERS = [0] , disable smart persistence
        PS_AF = [0] , PS auto mode
        PS_TRIG = [0] ,No Trigger
        Reserved = [0, 0]
                    --0000,0000--
    */
    /*              --PS_MS--high byte--
        Reserve = [0], White channel enable
        PS_MS = [0] , Proximity normal operation with interrupt function
        Reserve = [0,0,0]
        LED_I = [1,0,1]., LED current = 160mA
                    --0000,0101--
    */
    char lsb = 0x00;
    char msb = 0x06;
    send[0] = PS_CONF3;
    send[1] = lsb;
    send[2] = msb;
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
}

void VCNL4040::set_ps_thd(short ps_thdl, short ps_thdh) {
    send[0] = PS_THDL;
    send[1] = (char) (0x00ff & ps_thdl);
    send[2] = (char) ((0xff00 & ps_thdl) >> 8);
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
    send[0] = PS_THDH;
    send[1] = (char) (0x00ff & ps_thdh);
    send[2] = (char) ((0xff00 & ps_thdh) >> 8);
    i2c.write(VCNL4040_SLAVE_ADD, send, 3);
}

short VCNL4040::read_VCNL4040_als(void) {
    short als_value;
    send[0] = ALS_DATA;
    i2c.write(VCNL4040_SLAVE_ADD, send, 1, true);
    i2c.read(VCNL4040_SLAVE_ADD, received, 2);
    char lsb = received[0];
    char msb = received[1];
    als_value = (short) msb;
    als_value = ((als_value << 8) | (short) lsb);
    return als_value;
}

void VCNL4040::read_VCNL4040_int_flag(void) {
    char msb;
    char send = INT_FLAG;
    char received[2];
    i2c.write(VCNL4040_SLAVE_ADD, &send, 1, true);
    i2c.read(VCNL4040_SLAVE_ADD, received, 2);
    msb = received[1];
    for (int i = 0; i < 8; i++) {
        if ((msb >> i) & 0x01)
            int_flag[i] = TRUE;
        else
            int_flag[i] = FALSE;
    }
}

void VCNL4040::read_VCNL4040_ID(short *id) {
    char num = 0x0C;
    i2c.write(VCNL4040_SLAVE_ADD, &num, 1, true);
    i2c.read(VCNL4040_SLAVE_ADD, received, 2);
    *id = ((short) (received[1]) << 8) | (short) (received[0]);
}

short VCNL4040::read_VCNL4040_ps(void) {
    short ps_value;
    char send = PS_DATA;
    i2c.write(VCNL4040_SLAVE_ADD, &send, 1, true);
    i2c.read(VCNL4040_SLAVE_ADD, received, 2);
    ps_value = (short) received[1];
    ps_value = ((ps_value << 8) | (short) received[0]);
    return ps_value;
}
