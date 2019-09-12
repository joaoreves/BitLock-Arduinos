#include <RH_ASK.h>
#include <SPI.h>
#include <EEPROM.h>

char encript_msg[RH_ASK_MAX_MESSAGE_LEN];
char receive_msg[RH_ASK_MAX_MESSAGE_LEN];
char decript_msg[RH_ASK_MAX_MESSAGE_LEN];
String palavra = "abre de cesamo";
char open[] = "aberto";
long decript_index;
char id[33];
char memid[36];
int eeprom_empty;

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
    digitalWrite(LED_BUILTIN, LOW);
    pinMode(3, OUTPUT);
    pinMode(4, INPUT);

    eeprom_check();
}

void loop()
{
    if(eeprom_empty == 1)
    {
        send("receive");
        Serial.println("receive");

        delay(3000);

        receive();

        if(decript_msg[0] == 'i' && decript_msg[1] == 'd' && decript_msg[2] == '=')
        {
            for(int u = 0; u < 33; u++)
            {
                id[u] = decript_msg[u + 3];
            }
        }

        eeprom_write(id);

        delay(3000);

        eeprom_check();
    }

    else
    {
        receive();
        Serial.println(decript_msg);
        String palavra_id;
        for (int j = 0; j < 36; j++)
        {
            palavra_id[15 + j] = id[j];
        }
        if (palavra_id.equals(decript_msg))
        {
            digitalWrite(LED_BUILTIN, HIGH);
            digitalWrite(3, HIGH);
            delay(5000);
            digitalWrite(LED_BUILTIN, LOW);
            digitalWrite(3, LOW);
            decript_msg[0] = '\0';
        }

        delay(1000);

        if (digitalRead(4) == HIGH)
        {
            char open_id[43];
            for (int j = 0; j < 36; j++)
            {
                open_id[strlen(open) + j] = id[j];
            }
            Serial.println("aberto2");
            send(open_id);
        }

        delay(1500);
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

void decript()
{
    decript_index = receive_msg[0];
    for (int i = 0; receive_msg[i + 1] != '\0'; i++)
    {
        decript_msg[i] = receive_msg[i + 1] - key[decript_index][i];
        decript_msg[i + 1] = '\0';
    }
}

void receive()
{
    uint8_t buff[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t bufflen = sizeof(buff);

    if (driver.recv(buff, &bufflen))
    {
        int i = 0;
        for (i; i < bufflen; i++)
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
        final_msg[i] = encript_msg[i - 1];
    }

    driver.send((uint8_t *)final_msg, strlen(final_msg));
    driver.waitPacketSent();
    delay(1000);
}

void eeprom_check()
{
    int i = 0;
    int address = 0;
    while (address < 36)
    {
        memid[i] = EEPROM.read(address);
        i++;
        address++;
    }
    address = 0;

    if(memid[0] != '}' || memid[1] != 'q' || memid[2] != '-')
    {
        eeprom_empty = 0;
        for(int u = 0; u < 33; u++)
        {
            id[u] = memid[u + 3];
        }
    }
    else
    {
        eeprom_empty = 1;
    }
}

void eeprom_write(char val[])
{
    int addr = 0;
    EEPROM.write(0,'}');
    EEPROM.write(1,'q');
    EEPROM.write(2,'-');
    for (int a = 3; a < strlen(val) + 3; a++)
    {
        EEPROM.write(a,val[addr]);
        addr++;
    }

    if (addr == EEPROM.length())
    {
        addr = 0;
    }
}
