#include <avr/io.h>
#include <util/delay.h>

#define LEDpin PB0
#define PhotoresistorPin PC0
#define Motor1_1 PD0
#define Motor1_2 PD1
#define Motor2_1 PD2
#define Motor2_2 PD3
#define PWMpin PB3

#define DOT 100
#define LINE 300
#define PAUSE_SYMBOL 100
#define PAUSE_LETTER 300

void ADC_Init() {
    // Set ADC reference to AVCC
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void IO_Init() {
    // Set LED pin as output
    DDRB |= (1 << LEDpin);
    //Set motors' pin as output
    DDRD |= (1 << Motor1_1)
    		| (1 << Motor1_2)
			| (1 << Motor2_1)
			| (1 << Motor2_2);
    //Set motors' speed as output
	DDRB |= (1 << PWMpin);
    // Set photoresistor pin as input
    DDRC &= ~(1 << PhotoresistorPin);
}

void PWM_Init(){
	// Set Timer/Counter2 Fast PWM mode, non-inverting mode
	TCCR2 |= (1 << WGM20) | (1 << WGM21) | (1 << COM21) | (1 << CS22);
}

uint16_t ADC_Read(uint8_t channel) {
    // Select ADC channel
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    // Start single conversion
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void lightLEDbutMEOW(){
	//M letter
	PORTB |= (1 << LEDpin);
	_delay_ms(LINE);
    PORTB &= ~(1 << LEDpin);
    _delay_ms(PAUSE_SYMBOL);
    PORTB |= (1 << LEDpin);
    _delay_ms(LINE);
    PORTB &= ~(1 << LEDpin);
    _delay_ms(PAUSE_LETTER);

    //E letter
    PORTB |= (1 << LEDpin);
    _delay_ms(DOT);
    PORTB &= ~(1 << LEDpin);
    _delay_ms(PAUSE_LETTER);

    //O letter
    PORTB |= (1 << LEDpin);
    _delay_ms(LINE);
    PORTB &= ~(1 << LEDpin);
    _delay_ms(PAUSE_SYMBOL);
    PORTB |= (1 << LEDpin);
    _delay_ms(LINE);
    PORTB &= ~(1 << LEDpin);
    _delay_ms(PAUSE_SYMBOL);
    PORTB |= (1 << LEDpin);
    _delay_ms(LINE);
    PORTB &= ~(1 << LEDpin);
    _delay_ms(PAUSE_LETTER);

    //W letter
    PORTB |= (1 << LEDpin);
    _delay_ms(DOT);
    PORTB &= ~(1 << LEDpin);
    _delay_ms(PAUSE_SYMBOL);
    PORTB |= (1 << LEDpin);
    _delay_ms(LINE);
    PORTB &= ~(1 << LEDpin);
    _delay_ms(PAUSE_SYMBOL);
    PORTB |= (1 << LEDpin);
    _delay_ms(LINE);
    PORTB &= ~(1 << LEDpin);
    _delay_ms(PAUSE_LETTER);

}

void takeCoin(){
	//Change speed of motors
	OCR2 = 255;

	//Light LED
	lightLEDbutMEOW();

	PORTB |= (1 << LEDpin);

	//Activate first motor forward
	PORTD |= (1 << Motor1_1);
	PORTD &= ~(1 << Motor1_2);
	_delay_ms(1180);
	PORTD &= ~(1 << Motor1_1);

	_delay_ms(1500);

	//Activate second motor
	PORTD |= (1 << Motor2_2);
	PORTD &= ~(1 << Motor2_2);
	_delay_ms(865);
	PORTD &= ~(1 << Motor2_2);

	_delay_ms(2000);

	//Activate first motor backward
	PORTD |= (1 << Motor1_2);
	PORTD &= ~(1 << Motor1_1);
	_delay_ms(200);
	PORTD &= ~(1 << Motor1_2);

	//Turn LED off
    PORTB &= ~(1 << LEDpin);

    OCR2 = 0;

    _delay_ms(1000);
}

int main() {

    // Initialize ADC and I/O
    ADC_Init();
    IO_Init();
    PWM_Init();

    uint16_t value;

    while (1) {
        // Read value from photoresistor
        value = ADC_Read(PhotoresistorPin);
        if(value < 10) takeCoin();

        _delay_ms(100);
    }

    return 0;
}
