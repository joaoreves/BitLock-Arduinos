#include <RH_ASK.h>
#include <SPI.h>

char receive_msg[RH_ASK_MAX_MESSAGE_LEN];

void receive();


void setup()
{
    Serial.begin(9600);
}


void loop()
{

}


void receive()
{
    uint8_t buff[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t bufflen = sizeof(buff);

    driver.recv(buff, &bufflen));

    int i = 0;
    for(i; i < bufflen; i++)
    {
        receive_msg[i] = buff[i];
    }

    receive[i] = '\0';
}
