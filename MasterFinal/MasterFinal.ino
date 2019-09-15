/*this arduino is in charge off providing power to the esp8266
*and it also resets its EEPROM if a button is pressed if (digitalRead(3) == HIGH), to clear the initial register
*it communicates with the ESP8266 via Serial
*/

void setup()
{
    Serial.begin(9600);
    pinMode(3, INPUT);
    digitalWrite(3,LOW);
}

void loop()
{
    if (digitalRead(3) == HIGH)
    {
        Serial.println("reset");   // reset esp8266's eeprom ("factory reset")
    }

    delay(1000);
}
