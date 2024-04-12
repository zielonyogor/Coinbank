#include <avr/io.h>
#include <util/delay.h>

#define LEDpin PB0
#define PhotoresistorPin PC0
#define Motor1_1 PD0
#define Motor1_2 PD1
#define Motor2_1 PD2
#define Motor2_2 PD3


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
    // Set photoresistor pin as input
    DDRC &= ~(1 << PhotoresistorPin);
}

uint16_t ADC_Read(uint8_t channel) {
    // Select ADC channel
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    // Start single conversion
    ADCSRA |= (1 << ADSC);
    // Wait for conversion to complete
    while (ADCSRA & (1 << ADSC));
    // Return ADC result
    return ADC;
}

void takeCoin(){
	//Light LED
	PORTB |= (1 << LEDpin);

	//Activate first motor forward
	PORTD |= (1 << Motor1_1);
	PORTD &= ~(1 << Motor1_2);
	_delay_ms(1000);
	PORTD &= ~(1 << Motor1_1);

	_delay_ms(400);

	//Activate second motor
	PORTD |= (1 << Motor2_1);
	//PORTD &= ~(1 << Motor2_2); //idk if neccessary
	_delay_ms(1500);
	PORTD &= ~(1 << Motor2_1);

	_delay_ms(300);

	//Activate first motor backward
	PORTD |= (1 << Motor1_2);
	PORTD &= ~(1 << Motor1_1);
	_delay_ms(1000);
	PORTD &= ~(1 << Motor1_2);

	//Turn LED off
    PORTB &= ~(1 << LEDpin);

    _delay_ms(500);
}

int main() {

    // Initialize ADC and I/O
    ADC_Init();
    IO_Init();

    uint16_t value;

    while (1) {
        // Read value from photoresistor
        value = ADC_Read(PhotoresistorPin);
        if(value < 100) takeCoin();

        _delay_ms(20);
    }

    return 0;
}
