#include <RH_ASK.h>
#include <SPI.h>

char receive_msg[RH_ASK_MAX_MESSAGE_LEN];
char decript_msg[RH_ASK_MAX_MESSAGE_LEN];
char encript_msg[RH_ASK_MAX_MESSAGE_LEN];
long decript_index;
String open1 = "aberto1";
String open2 = "aberto2";
String rec = "receive";
String id;

char key[][52] =
{
    ".nj098anw12n12jn4jndnklenw24325jopji320ave93u2nlbkn",
    "-janfnbwi23470801in3wq23nbfwq12424jnjnini0032r80ian",
    "_zxy7njsaoj21klwnqmklfqw4alnqwnndaho28niuh328nwbeh2",
    "(12kjnndsviwonnwopjnf12j48932b89219001injjn1j2ej2jn",
    ")43646bsjhbhjbnkwh8912r1ub80089sdhasj09wss7fdvs7vs7",
    "{ksn212nnsl24m5sklm24slknlkni2n3inr23oiooi1ni0nofin",
    "}o3290ikmmkknin21490sdnanijiowaj09isnifjasnifowenfo",
    "+nfjj9wqjiwh0qhrqh0hh121e2142i1h210h21h12h1h1h14h5h",
    "@ksmedfmewmkefmkmefkmkem23k4k4nl2k3nk2k1lmk24nk3m2l",
    ")mwon123nkk132n12nkj3nkj13njk312nj2n3jk12n3j12nj312"
};

RH_ASK driver;

void setup()
{
    Serial.begin(9600);
    randomSeed(analogRead(0));
    if (!driver.init())
         Serial.println("init failed");

    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    receive();
    if(rec.equals(decript_index))
    {
        char sending[36];
        int ok = 0;
        while(ok == 0)
        {
            if(Serial.available() > 0)
            {
                id = Serial.readStringUntil('\n');
                ok = 1;
            }
        }
        sending[0] = 'i';
        sending[1] = 'd';
        sending[2] = '=';
        for (int u = 3; u < 36; u++)
        {
            sending[u] = id [u - 3];
        }
        send(sending);
    }

    else
    {
        Serial.println(decript_msg);
        if (open1.equals(decript_msg))
        {
            digitalWrite(LED_BUILTIN,HIGH);
            Serial.println("aberto1");
        }

        else if(open2.equals(decript_msg))
        {
            digitalWrite(LED_BUILTIN,HIGH);
            Serial.println("aberto2");
        }

        else
        {
            digitalWrite(LED_BUILTIN,LOW);
            Serial.println("fechado");
        }

        decript_msg[0] = '\0';

        delay(1000);

        if(digitalRead(2) == HIGH)
        {
            Serial.println("A enviar pass de abertura...1");
            send("abre de cesamo - 1");
        }

        if(digitalRead(3) == HIGH)
        {
            Serial.println("A enviar pass de abertura...2");
            send("abre de cesamo - 2");
        }

        delay(1500);

    }
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
        receive_msg[i] = '\0';
        decript();
    }
}

void decript()
{
    decript_index = receive_msg[0] - '0';
    for (int i = 0; receive_msg[i + 1] != '\0'; i++)
    {
        decript_msg[i] = receive_msg[i + 1] - key[decript_index][i];
        decript_msg[i+1] = '\0';
    }
}

void encript(char msg[])
{
    decript_index = random(0, 9);
    for (int i = 0; msg[i]; i++)
    {
        encript_msg[i] = msg[i] + key[decript_index][i];
        encript_msg[i + 1] = '\0';
    }
}

void send(char msg[])
{
    encript(msg);
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
