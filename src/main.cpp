#include <Arduino.h>
#define LED 5
#define TASTER 3
#define POTI A0
#define LDR A1

void setup()
{
    Serial.begin(115200);
    Serial.println("...Start...");

    pinMode(LED, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(TASTER, INPUT);
}

class bounce
{

    public:
    bool in, out, vorher, db_vorher, pos, neg = false;
    uint8_t taster, debounceTime;
    unsigned long last = 0;
    
    bounce(uint8_t _taster, uint8_t _debounceTime)
    {
        taster = _taster;
        debounceTime = _debounceTime;
    }

    void poll()
    {
        in = digitalRead(taster);

        if (millis() - last > debounceTime)
        {
            if (in && db_vorher)
            {
                out = in;
            }
            else
                out = false;
            db_vorher = in;
            last = millis();
        }
        pos = false;
        neg = false;
        if (out && !vorher)
            pos = true;
        if (!out && vorher)
            neg = true;
        vorher = out;
    }
};

bounce taster(TASTER, 50);
bool toggle, blink = false;
uint16_t frequenz;
int ldr;
unsigned long last;

void loop()
{
    frequenz = map(analogRead(POTI), 0, 1023, 50, 500);
    taster.poll();
    if (taster.pos)
    {
        toggle = !toggle;
        digitalWrite(LED, toggle);
        Serial.print("Die Led ist: ");
        toggle ? Serial.print("ein") : Serial.print("aus");
        ldr = analogRead(LDR);
        float vOut = (ldr * 5.0) / 1023.0;
        Serial.print(" und die Spannung ist = ");
        Serial.println(vOut);
    }
    if (millis() - last > frequenz)
    {
        blink = !blink;
        digitalWrite(LED_BUILTIN, blink);
        last = millis();
    }
}
