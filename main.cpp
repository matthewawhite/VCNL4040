#include "VCNL4040.h"

DigitalIn intPin(INT_PIN);
Serial pc(USBTX, USBRX);

int main() {
    VCNL4040 device(p9, p10);

    pc.baud(115200);
    short ps_value;
    short id;

    device.read_VCNL4040_ID(&id);
    device.set_ps_cmd1();
    device.set_ps_cmd2();

    pc.printf("Device id is: %d\n\r", id);
    if(id == 390) {
        while (true) {
            ps_value = device.read_VCNL4040_ps();
            double val = ps_value;
            // Below conversion to cm found by plotting collected data in excel
            val = -3.2468 * (log(val) - 7.34833);
            wait(0.5);
            pc.printf("The value read from the PS is: %f\n\r", val);
        }
    }
}
