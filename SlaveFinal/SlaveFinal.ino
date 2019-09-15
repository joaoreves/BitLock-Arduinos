/*this Arduino receives the "open" comand from the ESP8266
*if "open" is received the Arduino activates the relay and the door unlocks fo 5 seconds
*it also resets the ESP8266's EEPROM if a button is pressed (pin6), to clear the initial register
*it communicates with the ESP8266 via Serial
*/

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
      delay(500);
      string_wifi = Serial.readStringUntil('\n');  //receives the "open" command from the ESP8266
      Serial.flush();
    }
    Serial.println(string_wifi);

    if(string_wifi.startsWith("open"))
    {
        digitalWrite(3,HIGH);           //activates the relay
        digitalWrite(LED_BUILTIN,HIGH);
        Serial.println("aberto");  // warns the esp8266 that the door is open
        delay(5000);  //stays unlocked for 5 seconds
        digitalWrite(3,LOW);
        digitalWrite(LED_BUILTIN,LOW);
        Serial.println("fechado");
        string_wifi = "";   //clears the string
    }
    string_wifi = "";  //clears the string

    delay(500);

    if(digitalRead(6) == HIGH)
    {
        Serial.println("reset");  // reset esp8266's eeprom ("factory reset")
    }
}
