#include <RH_ASK.h>
#include <SPI.h>

char receive_msg[RH_ASK_MAX_MESSAGE_LEN];
char decript_msg[RH_ASK_MAX_MESSAGE_LEN];
char encript_msg[RH_ASK_MAX_MESSAGE_LEN];
const char msg[] = "abre de cesamo";
long decript_index;

char key[][50] =
{
    "nj098anw12n12jn4jndnklenw24325jopji320ave93u2nlbkn",
    "janfnbwi23470801in3wq23nbfwq12424jnjnini0032r80ian",
    "zxy7njsaoj21klwnqmklfqw4alnqwnndaho28niuh328nwbeh2",
    "12kjnndsviwonnwopjnf12j48932b89219001injjn1j2ej2jn",
    "43646bsjhbhjbnkwh8912r1ub80089sdhasj09wss7fdvs7vs7",
    "ksn212nnsl24m5sklm24slknlkni2n3inr23oiooi1ni0nofin",
    "o3290ikmmkknin21490sdnanijiowaj09isnifjasnifowenfo",
    "nfjj9wqjiwh0qhrqh0hh121e2142i1h210h21h12h1h1h14h5h"
};

RH_ASK driver;

void setup()
{
    Serial.begin(9600);
    randomSeed(analogRead(0));
    if (!driver.init())
         Serial.println("init failed");

    pinMode(2, INPUT);
}

void loop()
{
    if(digitalRead(2) == HIGH)
    {
        send();
    }

    delay(1500);
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

void decript()
{
    decript_index = receive_msg[0];
    for (int i = 0; receive_msg[i + 1] != '\0'; i++)
    {
        decript_msg[i] = receive_msg[i + 1] - key[decript_index][i];
    }
    Serial.print("Receive: ");
    Serial.println(decript_msg);
}

void encript()
{
    decript_index = random(0, 7);
    for (int i = 0; i < sizeof(msg) - 1; i++)
    {
        encript_msg[i] = msg[i] + key[decript_index][i];
    }
}

void send()
{
    encript();
    char final_msg[50];
    final_msg[0] = decript_index;
    for (int i = 1; i < sizeof(encript_msg); i++)
    {
        final_msg[i] = encript_msg[i-1];
    }

    driver.send((uint8_t *)final_msg, strlen(final_msg));
    driver.waitPacketSent();
    delay(1000);
}
