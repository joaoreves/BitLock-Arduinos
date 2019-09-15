String string_wifi;

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    pinMode(3, OUTPUT);
    digitalWrite(3,LOW);
    pinMode(6,INPUT);
    digitalWrite(6,LOW);
}

void loop()
{
    if (Serial.available() > 0)
    {
      delay(1000);
      string_wifi = Serial.readStringUntil('\n');
      Serial.flush();
    }
    Serial.println(string_wifi);

    if(string_wifi.startsWith("open"))
    {
        digitalWrite(3,HIGH);
        digitalWrite(LED_BUILTIN,HIGH);
        Serial.println("aberto");  // aviso o esp que o trinco esta aberto
        delay(5000);
        digitalWrite(3,LOW);
        digitalWrite(LED_BUILTIN,LOW);
        string_wifi = "";
    }
    string_wifi = "";

    delay(500);

    if(digitalRead(6) == HIGH)
    {
        Serial.println("reset");  // reset esp8266's eeprom ("factory reset")
    }
}
