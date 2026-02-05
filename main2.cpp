#include "mbed.h"
#include "arm_book_lib.h"

#define ON_TIME   30000ms

void code()
{
    DigitalOut greenled(LED1);
    DigitalOut redled(LED3);
    DigitalIn button0(D2);
    DigitalIn button1(D3);
    DigitalIn button2(D4);
    DigitalIn button3(D5);
    DigitalIn button4(D6);
    DigitalIn button5(D7);
    DigitalIn enterButton(BUTTON1);

    button0.mode(PullDown);
    button1.mode(PullDown);
    button2.mode(PullDown);
    button3.mode(PullDown);
    button4.mode(PullDown);
    button5.mode(PullDown);

   int wrongatt = 0;
bool warningMode = false;
Timer warningTimer;

while (true) {

    // ---- WARNING MODE ----
    if (warningMode) {
        redled = !redled;                 // slow blink
        ThisThread::sleep_for(500ms);

        if (warningTimer.elapsed_time() >= 30s) {
            warningMode = false;          // unblock system
            wrongatt = 0;
            redled = 0;
            warningTimer.stop();
            warningTimer.reset();
        }
        continue;                         // block new entries
    }

    // ---- CODE ENTRY ----
    if (enterButton) {

        if (button0 && button1 && button2 && button3) {
            greenled = 1;
            redled = 0;
            wrongatt = 0;                 // reset on success
        }
        else {
            greenled = 0;
            redled = 1;
            wrongatt++;
        }

        ThisThread::sleep_for(300ms);     // debounce
    }

    // ---- CHECK WRONG ATTEMPTS ----
    if (wrongatt >= 3) {
        warningMode = true;
        warningTimer.start();
        greenled = 0;
        redled = 1;
    }
}

}


int main(){
    code();
}
