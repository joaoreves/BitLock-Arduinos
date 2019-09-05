#include <RH_ASK.h>
#include <SPI.h>

char receive_msg[RH_ASK_MAX_MESSAGE_LEN];
char decript_msg[RH_ASK_MAX_MESSAGE_LEN];
char *key = "wi4142oen24io14qwfen23n55gjfj25322n5235moaosnfinwe00";

void receive();

void decript();

RH_ASK driver;

void setup()
{
    Serial.begin(9600);
}


void loop()
{
    receive();
    decript();
    Serial.println(decript_msg);
}


void receive()
{
    uint8_t buff[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t bufflen = sizeof(buff);

    driver.recv(buff, &bufflen);

    int i = 0;
    for(i; i < bufflen; i++)
    {
        receive_msg[i] = buff[i];
    }

    receive_msg[i] = '\0';
}

void decript()
{
    for (int i = 0; i < sizeof(receive_msg); i++)
    {
        decript_msg[i] = receive_msg[i] - key[i];
    }
}
