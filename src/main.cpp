#include <Arduino.h>
#include "BlastGateController.h"

// pin mapping
constexpr uint8_t INPUT1_PIN        = 2;

BlastGateController controller;

// timer1 interrupt handler for 10ms intervals
#if defined(TIMER1_COMPA_vect)
ISR(TIMER1_COMPA_vect) {
#else
ISR(TIM1_COMPA_vect) {
#endif
    controller.debounce();
}

// initialize timer1 to generate CTC compare-A interrupts every 10 ms
// assumes 16 MHz CPU and prescaler 64: ticks = 16e6 / 64 = 250000 Hz
// required ticks per 10ms = 250000 * 0.01 = 2500 -> OCR1A = 2499
void initTimer1_10ms() {
    cli();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    // CTC mode: WGM12 = 1 (CTC with OCR1A as TOP)
    // compare value for 10 ms (for 16 MHz CPU, prescaler 64)
#if defined(WGM12)
    TCCR1B |= (1 << WGM12);
#endif
#if defined(OCR1A)
    OCR1A = 2499;
#endif
#if defined(CS11) && defined(CS10)
    TCCR1B |= (1 << CS11) | (1 << CS10);
#endif
#if defined(TIMSK1)
    TIMSK1 |= (1 << OCIE1A);
#elif defined(TIMSK)
    TIMSK |= (1 << OCIE1A);
#endif
    sei();
}

void setup() {
    initTimer1_10ms();
    Serial.begin(debugBaudRate);
    controller.begin();
}

void loop() {
    controller.update();
}
