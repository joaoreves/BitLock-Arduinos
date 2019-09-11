#include <RH_ASK.h>
#include <SPI.h>

char encript_msg[RH_ASK_MAX_MESSAGE_LEN];
char receive_msg[RH_ASK_MAX_MESSAGE_LEN];
char decript_msg[RH_ASK_MAX_MESSAGE_LEN];
String palavra = "abre de cesamo";
long decript_index;

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

    pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN,LOW);
    pinMode(3,OUTPUT);
    pinMode(4,INPUT);
}

void loop()
{
    receive();
    Serial.println(decript_msg);
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
    delay(5000);
    
    if (digitalRead(4) == HIGH)
    {
        Serial.println("aberto");
        send("aberto");
    }
    else if(digitalRead(4) == LOW)
    {
        Serial.println("fechado");
        send("fechado");
    }
    delay(5000);
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

void decript()
{
    decript_index = receive_msg[0];
    for (int i = 0; receive_msg[i + 1] != '\0'; i++)
    {
        decript_msg[i] = receive_msg[i + 1] - key[decript_index][i];
        decript_msg[i+1] = '\0';
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
        receive_msg[i++] = '\0';
        decript();
    }
}

void send(char msg[])
{
    encript(msg);
    char final_msg[50];
    final_msg[0] = '0' + decript_index;
    for (int i = 1; i < sizeof(encript_msg) ; i++)
    {
        final_msg[i] = encript_msg[i-1];
    }

    driver.send((uint8_t *)final_msg, strlen(final_msg));
    driver.waitPacketSent();
    delay(1000);
}
