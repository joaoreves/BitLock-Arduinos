#include <VirtualWire.h>
#include <SPI.h>
#include <EEPROM.h>


char encript_msg[VW_MAX_MESSAGE_LEN];
char receive_msg[VW_MAX_MESSAGE_LEN];
char decript_msg[VW_MAX_MESSAGE_LEN];
String palavra = "abre de cesamo";
char open[] = "aberto";
long decript_index;
char id[33];
char memid[36];
int eeprom_empty;
char program_state;

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


void resetEEPROM() {
  String val = "aahhaha";
  int addr = 0;
  for (int a = 0; a < val.length(); a++)
  {
      EEPROM.write(a,val[addr]);
      addr++;
  }
  if (addr == EEPROM.length())
  {
      addr = 0;
  }
}

void setup()
{
    Serial.begin(9600);
    randomSeed(analogRead(0));
    vw_setup(500); 
    vw_set_ptt_inverted(true);
    vw_set_rx_pin(11);
    vw_set_tx_pin(12);

    
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    pinMode(3, OUTPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    
    program_state = '0';
}

void loop()
{
  Serial.println(program_state);
  switch(program_state) {
    case '0':
      Serial.println("register");
      send("register");
      program_state = '1';
      delay(500);
      break;
    case '1':
      eeprom_check();
      if(eeprom_empty == 0)
      {
          program_state = '2';
          delay(500);
          break;
      }
      
      receive();
      Serial.println(decript_msg);
      if(decript_msg[0] == 'i' && decript_msg[1] == 'd' && decript_msg[2] == '=')
      {
          eeprom_write(decript_msg);
          eeprom_check();
          program_state = '2';
          Serial.println("registered");
      }
      
      else 
      {
          Serial.println("not registered");
          program_state = '0';
      }
      delay(1500);
      break;
    case '2':
      send("register id ok");
      program_state = '3';  
      delay(1500);
      break;
    case '3':
      receive();
      Serial.println(decript_msg);
      String palavra_id = "abre de cesamo";
      palavra_id += id;
      Serial.println(id);
      if (palavra_id.equals(decript_msg))
      {
          digitalWrite(LED_BUILTIN, HIGH);
          digitalWrite(3, HIGH);
          delay(5000);
          digitalWrite(LED_BUILTIN, LOW);
          digitalWrite(3, LOW);
          decript_msg[0] = '\0';
      }
      program_state = '4';
      delay(1500);
      break;
    case '4':
      if (digitalRead(4) == HIGH)
      {
          char open_id[43] = "aberto";
          for (int j = 0; j < 33; j++)
          {
              open_id[6 + j] = id[j];
          }
          Serial.println("aberto1");
          send(open_id);
      }
      delay(1500);
      break;
  }

  if(digitalRead(5) == HIGH) { 
    resetEEPROM();
  }
  delay(1500);
  /*
    eeprom_check();
    if(eeprom_empty == 1)
    {
        send("register");
        Serial.println("register");

        delay(3000);

        receive();
        Serial.println(decript_msg);

        if(decript_msg[0] == 'i' && decript_msg[1] == 'd' && decript_msg[2] == '=')
        {
             eeprom_write(decript_msg);
        }

        delay(3000);

        eeprom_check();

        if(eeprom_empty == 0)
        {
            send("register id ok");
        }
    }

    else
    {
        receive();
        Serial.println(decript_msg);
        String palavra_id = "abre de cesamo";
        for (int j = 0; j < 36; j++)
        {
            palavra_id[14 + j] = id[j];
        }
        Serial.println(id);
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
            char open_id[43] = "aberto";
            for (int j = 0; j < 33; j++)
            {
                open_id[6 + j] = id[j];
            }
            Serial.println("aberto1");
            send(open_id);
        }

        delay(1500);
    }*/

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
    vw_rx_start();
    uint8_t buff[VW_MAX_MESSAGE_LEN];
    uint8_t bufflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buff, &bufflen))
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

    vw_send((uint8_t *)final_msg, strlen(final_msg));
    vw_wait_tx();
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

    if(memid[0] != 'i' || memid[1] != 'd' || memid[2] != '=')
    {
        eeprom_empty = 1;
    }
    else
    {
        eeprom_empty = 0;
        for(int u = 0; u < 33; u++)
        {
            id[u] = memid[u + 3];
        }
    }
}

void eeprom_write(char val[])
{
    int addr = 0;
    for (int a = 0; a < strlen(val); a++)
    {
        EEPROM.write(addr,val[a]);
        addr++;
    }

    if (addr == EEPROM.length())
    {
        addr = 0;
    }
}
