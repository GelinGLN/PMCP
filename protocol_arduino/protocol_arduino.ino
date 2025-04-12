#include <ArduinoJson.h>

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
  digitalWrite(2, LOW);
  
  // Confirma que o Arduino está pronto
  Serial.println("Arduino pronto para receber comandos");
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    Serial.print("Recebido: ");
    Serial.println(input);
    
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, input);
    
    if (error) {
      Serial.print("Erro ao processar JSON: ");
      Serial.println(error.c_str());
      return;
    }
    
    if (!doc.is<JsonArray>()) {
      Serial.println("Erro: JSON não é um array");
      return;
    }
    
    for (JsonObject obj : doc.as<JsonArray>()) {
      if (!obj.containsKey("cmd")) {
        Serial.println("Comando sem chave 'cmd'");
        continue;
      }
      
      String comando = obj["cmd"];
      Serial.print("Executando comando: ");
      Serial.println(comando);
      
      if (comando == "shieldLed_on") {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("LED ligado");
      } 
      else if (comando == "shieldLed_off") {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("LED desligado");
      } 
      else if (comando == "delay") {
        if (obj.containsKey("param")) {
          int tempo = obj["param"];
          Serial.print("Delay de: ");
          Serial.println(tempo);
          delay(tempo);
        } else {
          Serial.println("Erro: parâmetro de delay não especificado");
        }
      } 
      else {
        Serial.print("Comando desconhecido: ");
        Serial.println(comando);
      }
    }
  }
}