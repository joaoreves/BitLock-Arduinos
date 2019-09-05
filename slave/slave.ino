#include <RH_ASK.h>
#include <SPI.h>

const char *msg = "Door Open Alfredo Cagado";
char encript_msg[RH_ASK_MAX_MESSAGE_LEN];
char *key = "wi4142oen24io14qwfen23n55gjfj25322n5235moaosnfinwe00";

void encript();

RH_ASK driver;

void setup()
{
    Serial.begin(9600);
    encript();
}


void loop()
{
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(200);
}

void encript()
{
    for (int i = 0; i < sizeof(msg); i++)
    {
        encript_msg[i] = msg[i] + key[i];
    }
}
