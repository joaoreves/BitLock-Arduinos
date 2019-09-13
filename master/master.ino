#include <RH_ASK.h>
#include <SPI.h>

char receive_msg[RH_ASK_MAX_MESSAGE_LEN];
char decript_msg[RH_ASK_MAX_MESSAGE_LEN];
char encript_msg[RH_ASK_MAX_MESSAGE_LEN];
long decript_index;
String rec = "register";
String recok = "register id ok";
String id;
String id1;
String id2;

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

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    String string_wifi;
    
    if(Serial.available() > 0)
    {
        string_wifi = Serial.readStringUntil('\n');
    }
    if(string_wifi[0] == 'i' && string_wifi[1] == 'd' && string_wifi[2] == '=')
    {
        int ok = 0;
        String coded = "id=";

        coded += "{q-";
        for (int u = 0; u < string_wifi.length() - 3; u++)
        {
            coded += string_wifi[u + 3];
        }

        Serial.println(coded);
        
        while(ok == 0)
        {
            receive();
            digitalWrite(LED_BUILTIN,HIGH);
            delay(1000);
            digitalWrite(LED_BUILTIN,LOW);

            if(recok.equals(decript_index))
            {
                ok = 1;
                Serial.println("Registo do ID realizado com sucesso!");
            }
            else
            {
                send(coded);
                delay(1000);
            }
        }
    }
    else if (string_wifi[0] != '\0');
    {
        send(string_wifi);
    }
    delay(1000);

    receive();

    if(decript_msg[0] != '\0')
    {
        Serial.println(decript_msg);
        if(decript_msg[0] == 'a' && decript_msg[1] == 'b' && decript_msg[2] == 'e' && decript_msg[3] == 'r' && decript_msg[4] == 't' && decript_msg[5] == 'o')
        {
            digitalWrite(LED_BUILTIN,HIGH);
            delay(5000);
            digitalWrite(LED_BUILTIN,LOW);
        }
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

void encript(String msg)
{
    decript_index = random(0, 9);
    for (int i = 0; msg[i]; i++)
    {
        encript_msg[i] = msg[i] + key[decript_index][i];
        encript_msg[i + 1] = '\0';
    }
}

void send(String msg)
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
