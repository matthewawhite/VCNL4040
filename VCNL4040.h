#include "mbed.h"

#ifndef VCNL4040_H
#define VCNL4040_H

//-----------------------------------------------------------------------------
// Global definition for VCNL4040 Registers
//-----------------------------------------------------------------------------
#define VCNL4040_SLAVE_ADD 0xC0 //0x60 << 1
#define ALS_CONF 0x00
#define ALS_THDH 0x01
#define ALS_THDL 0x02
#define PS_CONF1_2 0x03
#define PS_CONF3 0x04
#define PS_CANC 0x05
#define PS_THDL 0x06
#define PS_THDH 0x07
#define PS_DATA 0x08
#define ALS_DATA 0x09
#define White_DATA 0x0A
#define INT_FLAG 0x0B
#define VCNL4040_ID 0x0C
#define TRUE 0x01
#define FALSE 0x02
#define INT_PIN p5

static char int_flag[8];

class VCNL4040 {
    public:
        VCNL4040(PinName sda, PinName scl);
        //----------------------------------------------------------------------// FUNCTION NAME: set_als_cmd_disable_int
        //
        // DESCRIPTION:
        // Configures ALS for personal settings and specifically for disabling
        // the interrupt pin //----------------------------------------------------------------------
        void set_als_cmd_disable_int(void);

        //----------------------------------------------------------------------
        // FUNCTION NAME: set_als_cmd_enable_int
        //
        // DESCRIPTION:
        // Configures ALS for personal settings and specifically for enabling
        // the interrupt pin
        //----------------------------------------------------------------------
        void set_als_cmd_enable_int(void);

        //----------------------------------------------------------------------
        // FUNCTION NAME: set_als_int_thd
        //
        // DESCRIPTION:
        // Sets thresholds for the ALS to set value of INT pin
        //----------------------------------------------------------------------
        void set_als_int_thd(short thd_high, short thd_Low);

        //----------------------------------------------------------------------
        // FUNCTION NAME: set_ps_cmd1
        //
        // DESCRIPTION:
        // Configures PS to custom settings such as INT triggers, output size,
        // interrupt
        //----------------------------------------------------------------------
        void set_ps_cmd1(void);

        //----------------------------------------------------------------------
        // FUNCTION NAME: set_ps_cmd2
        //
        // DESCRIPTION:
        // Further configures PS for active force trigger
        //----------------------------------------------------------------------
        void set_ps_cmd2(void);

        //----------------------------------------------------------------------
        // FUNCTION NAME: set_ps_thd
        //
        // DESCRIPTION:
        // Configure high and low threshold values for PS INT pin
        //----------------------------------------------------------------------
        void set_ps_thd(short ps_thdl, short ps_thdh);

        //----------------------------------------------------------------------
        // FUNCTION NAME: read_VCNL4040_als
        //
        // DESCRIPTION:
        // Reading ambient light sensor values, not being implemented for now
        //----------------------------------------------------------------------
        short read_VCNL4040_als(void);

        //----------------------------------------------------------------------
        // FUNCTION NAME: read_VCNL4040_ps
        //
        // DESCRIPTION:
        // Reading the 16 bit data from the PS
        //----------------------------------------------------------------------
        short read_VCNL4040_ps(void);

        //----------------------------------------------------------------------
        // FUNCTION NAME: read_VCNL4040_int_flag
        //
        // DESCRIPTION:
        //      int_flag[0] = PS1_IF_AWAY
        //      int_flag[1] = PS1_IF_CLOSE
        //      int_flag[2] = Reserved
        //      int_flag[3] = Reserved
        //      int_flag[4] = ALS_IF_H
        //      int_flag[5] = ALS_IF_L
        //      int_flag[6] = PS1_SPFLAG -> protection mode?
        //      int_flag[7] = Reserved
        //----------------------------------------------------------------------
        void read_VCNL4040_int_flag(void);

        //----------------------------------------------------------------------
        // FUNCTION NAME: int read_VCNL4040_ID
        //
        // DESCRIPTION:
        // Reads the device ID register
        //----------------------------------------------------------------------
        void read_VCNL4040_ID(short *id);

    private:
        I2C i2c;
        char send[3];
        char received[2];
};

#endif
