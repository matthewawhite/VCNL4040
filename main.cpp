#include "VCNL4040.h"
#include "mbed.h"
// NOTES:
// PS = proximity sensor, ALS = ambient light sensor,
// Reg_num = register number, THD = threshold, CONFG = configuration
// INT = interrupt (active high),
// Persistence = num consecutive hits to pull INT low,
// I2C diagram shows data byte low then byte high all after command code
// For the PS, the output will be subtracted by the value set in PS_CANC
// Max PS detection with Kodak white card = 200 mm
// White channel can help determine kind of light source (yellowish-orange best)
// Interrupt pin goes to mbed GPIO pin
// Read is | 1 and write is | 0


// extern BYTE I2Cm_bWriteBytes(BYTE SlaveAddr, BYTE Reg_num, BYTE Data_lsb, BYTE Data_msb, BYTE bCnt);
// extern BYTE I2Cm_fReadBytes(BYTE SlaveAddr, BYTE Reg_num, BYTE * Data, BYTE bCnt);

#include "SRF08.h"

LocalFileSystem fs("fs");

DigitalIn intPin(INT_PIN); //interupt pin
DigitalOut led(LED1);
Serial pc(USBTX, USBRX);
VCNL4040 device(p9, p10);
SRF08 srf08(p28, p27, 0xFA);

//----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------
int main() { //Put this at end of file instead of top, now returns int


    pc.baud(115200);
    WORD ps_value;
    WORD id;

    FILE *fd = fopen("/fs/distance.csv", "w");
    FILE *fc = fopen("/fs/counts.csv", "w");

    // TODO: find out good threshold values
    //device.set_ps_thd(0x03, 0x08); // Changed from x08 to 0x08

    device.read_VCNL4040_ID(&id);
    device.set_ps_cmd1();
    device.set_ps_cmd2();

    pc.printf("Device id is: %d\n\r", id);
    if(id == 390) {
        /*while (true) {
            ps_value = device.read_VCNL4040_ps();
            wait(0.5);
            pc.printf("The value read from the PS is: %d\n\r", ps_value);
        }*/
        for (int i = 0; i<1000; i++) {
            ps_value = device.read_VCNL4040_ps();
            fprintf(fd, "%.4f\n", srf08.read());
            fprintf(fc, "%d\n", ps_value);
            wait(0.01);
        }
        led = 1;

        fclose(fd);
        fclose(fc);
    }

    // while(1) {
    //     if (intPin.read() == 0) {
    //
    //     }
    // }
}



/**#include "VCNL4040.h"
// NOTES:
// PS = proximity sensor, ALS = ambient light sensor,
// Reg_num = register number, THD = threshold, CONFG = configuration
// INT = interrupt (active high),
// Persistence = num consecutive hits to pull INT low,
// I2C diagram shows data byte low then byte high all after command code
// For the PS, the output will be subtracted by the value set in PS_CANC
// Max PS detection with Kodak white card = 200 mm
// White channel can help determine kind of light source (yellowish-orange best)
// Interrupt pin goes to mbed GPIO pin
// Read is | 1 and write is | 0


// extern BYTE I2Cm_bWriteBytes(BYTE SlaveAddr, BYTE Reg_num, BYTE Data_lsb, BYTE Data_msb, BYTE bCnt);
// extern BYTE I2Cm_fReadBytes(BYTE SlaveAddr, BYTE Reg_num, BYTE * Data, BYTE bCnt);

DigitalIn intPin(INT_PIN); //interupt pin
Serial pc(USBTX, USBRX);

//----------------------------------------------------------------------------
// C++ main line
//----------------------------------------------------------------------------
int main() {
    VCNL4040 device(p9, p10);

    pc.baud(115200);
    WORD ps_value;
    WORD id;

    // TODO: find out good threshold values
    //device.set_ps_thd(0x03, 0x08); // Changed from x08 to 0x08

    device.read_VCNL4040_ID(&id);
    device.set_ps_cmd1();
    device.set_ps_cmd2();

    pc.printf("Device id is: %d\n\r", id);
    if(id == 390) {
        while (true) {
            ps_value = device.read_VCNL4040_ps();
            wait(0.5);
            pc.printf("The value read from the PS is: %d\n\r", ps_value);
        }
    }

    // while(1) {
    //     if (intPin.read() == 0) {
    //
    //     }
    // }
}


*/










// OLD MAIN METHOD BELOW

/*
int main() { //Put this at end of file instead of top, now returns int
    BYTE buff[3];
    WORD als_value;

    set_als_cmd_disable_int();
    //als_value = read_VCNL4040_als();
    //set_als_int_thd((als_value + 300), (als_value - 300));
    set_ps_thd(0x03, 0x08); // Changed from x08 to 0x08
    set_ps_cmd1();
    set_ps_cmd2();
    set_als_cmd_enable_int();

    // CHANGED all panel lines to comments
    while(1) {
        if (INT_PIN == LOW) {
            read_VCNL4040_int_flag();

            // Below uses ALS and has pseudocode for device brightness
            if (int_flag[1])
                PANEL_OFF;
            if ((int_flag[0])&&(int_flag[2]))
                PANEL_ON;
            if (int_flag[4]) {
                als_value = read_VCNL4040_als();
                if (als_value > previous_data) // CHANGED als_data to als_value
                PANEL_brightness++;
            }
            if (int_flag[5]) {
                als_value = read_VCNL4040_als();
                if (als_value < previous_data) // CHANGED als_data to als_value
                PANEL_brightness--;
            }

        }
    }
} */
