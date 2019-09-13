//#include <RH_ASK.h>
//#include <SPI.h>

#include <VirtualWire.h>

char receive_msg[VW_MAX_MESSAGE_LEN];
char decript_msg[VW_MAX_MESSAGE_LEN];
char encript_msg[VW_MAX_MESSAGE_LEN];
long decript_index;
char rec[] = "register";
char recok[] = "register id ok";
String string_wifi;
String coded;

uint8_t programState;

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

//RH_ASK driver;

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));
  //if (!driver.init())
  //     Serial.println("init failed");

  vw_set_ptt_inverted(true);
  vw_setup(500);
  vw_set_tx_pin(12);
  vw_set_rx_pin(11);
  vw_rx_start();

  pinMode(LED_BUILTIN, OUTPUT);
  programState = 1;
  //send("ababab");
}

void loop()
{
  string_wifi = "";
  Serial.println("loop");
  if(vw_have_message()) {
    Serial.println("i has message");
    receive();
    Serial.println(decript_msg);
  }

  if (Serial.available() > 0) {
    delay(1000);
    string_wifi = Serial.readStringUntil('\n');
    Serial.flush();
    programState = 0;
  }
  else {
    programState = 1;
  }

  Serial.println(string_wifi);

  if (string_wifi[0] == 'i' && string_wifi[1] == 'd' && string_wifi[2] == '=')
  {
    programState = 2;
  }

  switch (programState) {
    case 0:
      Serial.println("programState 0");
      if (string_wifi.length() > 0) {
        send(string_wifi);
        programState = 1;
      }
      delay(1500);
      break;
    case 1:
      Serial.println("programState 1");
      receive();
      if (decript_msg[0] != '\0' && strcmp(decript_msg, "aberto") == 0) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(5000);
        digitalWrite(LED_BUILTIN, LOW);
      } else if (strcmp(decript_msg, "register") == 0) {
        Serial.println("register");
      }
      programState = 0;
      Serial.println(decript_msg);
      delay(1500);
      break;
    case 2:
      Serial.println("programState 2");
      send(string_wifi);
      programState = 3;
      delay(1500);
      break;
    case 3:
      Serial.println("programState 3");
      receive();
      Serial.println("received");
      if (strcmp(recok, decript_msg) == 0) {
        programState = 0;
        Serial.println("Registo do ID realizado com sucesso!");
      }
      else {
        programState = 2;
      }
      delay(1500);
      break;
  }

  delay(1000);
}

void receive()
{
  vw_wait_rx();
  char buff[VW_MAX_MESSAGE_LEN];
  uint8_t bufflen = 0;

  if (vw_get_message((uint8_t *)buff, &bufflen))
  {
    Serial.println(buff);
    int i = 0;
    for (i; i < bufflen; i++)
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
  decript_msg[0] = '\0';
  for (int i = 0; receive_msg[i + 1] != '\0'; i++)
  {
    decript_msg[i] = receive_msg[i + 1] - key[decript_index][i];
    decript_msg[i + 1] = '\0';
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
    final_msg[i] = encript_msg[i - 1];
  }

  vw_send((uint8_t *)final_msg, strlen(final_msg) + 1);
  vw_wait_tx();
  //driver.send((uint8_t *)final_msg, 50);
  //driver.waitPacketSent();
}
