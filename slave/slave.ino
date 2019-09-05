#include <RH_ASK.h>
#include <SPI.h>

char encript_msg[RH_ASK_MAX_MESSAGE_LEN];
char receive_msg[RH_ASK_MAX_MESSAGE_LEN];
char decript_msg[RH_ASK_MAX_MESSAGE_LEN];
const char *msg = "aberto";
const char *msg1 = "fechado";
String palavra = "abre de cesamo";

RH_ASK driver;

void setup()
{
    Serial.begin(9600);
    if (!driver.init())
         Serial.println("init failed");
         
    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);
    pinMode(3,OUTPUT);
    //pinMode(4,INPUT);
}

void loop()
{
    receive();
    if (palavra.equals(decript_msg))
    {
        digitalWrite(LED_BUILTIN,HIGH);
        digitalWrite(3, HIGH);
        decript_msg[0] = '\0';
    }
    else
    {
        digitalWrite(LED_BUILTIN,LOW);
        digitalWrite(3, LOW);
    }

    /*if (digitalRead(4) == HIGH)
    {

    }*/
    delay(500);
}

void encript()
{
    int i = 0;
    for (i; i < sizeof(msg) - 1; i++)
    {
        encript_msg[i] = msg[i] + 3;
    }
}

void decript()
{
    for (int i = 0; receive_msg[i] != '\0'; i++)
    {
        decript_msg[i] = receive_msg[i] - 3;
    }
    Serial.print("Receive: ");
    Serial.println(decript_msg);
}

void receive()
{
    uint8_t buff[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t bufflen = sizeof(buff);

    if(driver.recv(buff, &bufflen))
    {
        int i = 0;
        for(i; i < bufflen; i++)
        {
            receive_msg[i] = buff[i];
        }
        receive_msg[i++] = '\0';
        Serial.print("receive: ");
        Serial.println(receive_msg);
        decript();
    }
}

void send()
{
    driver.send((uint8_t *)encript_msg, strlen(encript_msg));
    driver.waitPacketSent();
    delay(1000);
}
