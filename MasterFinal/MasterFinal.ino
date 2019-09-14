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
        Serial.println("reset");
    }
}
