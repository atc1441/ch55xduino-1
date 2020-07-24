#define ARDUINO_MAIN
#include "wiring_private.h"
#include "pins_arduino_include.h"


uint8_t analogRead(uint8_t pin)
{
    pin = analogPinToChannel(pin);
    
    if (pin == NOT_ANALOG) return 0;
    
    ADC_CFG = bADC_EN | bADC_CLK;
    
    ADC_CTRL = (ADC_CTRL & ~(0x03)) | (0x03&pin);
    
    ADC_START = 1;
    
    while(ADC_START);
    
    return ADC_DATA;
}


// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint8_t pin, __xdata uint16_t val)
{
    // We need to make sure the PWM output is enabled for those pins
    // that support it, as we turn it off when digitally reading or
    // writing with them.  Also, make sure the pin is in output mode
    // for consistenty with Wiring, which doesn't require a pinMode
    // call for the analog output pins.
    pinMode(pin, OUTPUT);
    if (val == 0)
    {
        digitalWrite(pin, LOW);
    }
    else if (val >= 256)
    {
        digitalWrite(pin, HIGH);
    }
    else
    {
        switch(digitalPinToPWM(pin))
        {
            case PIN_PWM1:
                PIN_FUNC &= ~(bPWM1_PIN_X);
                PWM_CTRL |= bPWM1_OUT_EN;
                PWM_DATA1 = val;
                break;
            case PIN_PWM2:
                PIN_FUNC &= ~(bPWM2_PIN_X);
                PWM_CTRL |= bPWM2_OUT_EN;
                PWM_DATA2 = val;
                break;
            case PIN_PWM1_:
                PIN_FUNC |= (bPWM1_PIN_X);
                PWM_CTRL |= bPWM1_OUT_EN;
                PWM_DATA1 = val;
                break;
            case PIN_PWM2_:
                PIN_FUNC |= (bPWM2_PIN_X);
                PWM_CTRL |= bPWM2_OUT_EN;
                PWM_DATA2 = val;
                break;
            case NOT_ON_PWM:
            default:
                if (val < 128) {
                    digitalWrite(pin, LOW);
                } else {
                    digitalWrite(pin, HIGH);
                }
        }
    }
}

