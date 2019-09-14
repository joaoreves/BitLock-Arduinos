String string_wifi;
String abrir("open");

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    pinMode(3, OUTPUT);
    digitalWrite(3,LOW);
    pinMode(6,INPUT);
}

void loop()
{
    if (Serial.available() > 0)
    {
      delay(1000);
      string_wifi = Serial.readStringUntil('\n');
      Serial.flush();
    }

    if(abrir.equals(string_wifi))
    {
        digitalWrite(3,HIGH);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(5000);
        digitalWrite(3,LOW);
        digitalWrite(LED_BUILTIN,LOW);
    }
    Serial.println("aberto");  // aviso o esp que o trinco esta aberto

    delay(500);

    if(digitalRead(6) == HIGH)
    {
        Serial.println("reset");  // reset esp8266's eeprom ("factory reset")
    }
}
