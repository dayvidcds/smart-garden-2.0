#include "canal.cpp";
#include "matriz.cpp";
#include "display.cpp";
#include "sensor_de_temp_umi.cpp";
#include <EEPROM.h>

#define caixa1 2
#define caixa2 3
#define caixa3 4

#define SENSOR_UMI_SOLO1 A1
#define SENSOR_UMI_SOLO2 A2
#define SENSOR_UMI_SOLO3 A3

#define SMS_VCC 6
#define SMS_GND 7

#define SOLO_SECO 40
#define SOLO_UMIDO 60

#define DHTPIN 8
#define DHTTYPE DHT11 // DHT 11

DHT dhtParam(DHTPIN, DHTTYPE);
TempUmi dht(&dhtParam); // Passando sensor DHT como parâmetro para o nosso objeto

Canal canal1(1, SENSOR_UMI_SOLO1, SENSOR_UMI_SOLO1, SENSOR_UMI_SOLO1, SMS_VCC, SMS_GND);
Canal canal2(2, SENSOR_UMI_SOLO2, SENSOR_UMI_SOLO2, SENSOR_UMI_SOLO2, SMS_VCC, SMS_GND);
Canal canal3(3, SENSOR_UMI_SOLO3, SENSOR_UMI_SOLO3, SENSOR_UMI_SOLO3, SMS_VCC, SMS_GND);

#define QUANT_CANAIS 3
Canal canais[3] = { canal1, canal2, canal3 };

LiquidCrystal_I2C lcdParam(0x27, 16, 2); // Instanciando Display LCD
Display lcd(&lcdParam, true); // Passando como parâmetro o objeto lcdParam no objeto lcd

int MES = 1;
int comando = 0;

void lerSensor();
int getCaixaByCanalAndMes(int mes, int canal);
String leStringSerial();

void setup() {
  pinMode(SENSOR_UMI_SOLO1, INPUT);
  pinMode(caixa1, OUTPUT);
  pinMode(caixa2, OUTPUT);
  pinMode(caixa3, OUTPUT);
  digitalWrite(SMS_VCC, LOW);
  digitalWrite(SMS_GND, LOW);
  Serial.begin(9600);
  dht.iniciar();
  lcd.iniciar();
  lcd.imprimir("Iniciando...");
   delay(2000);
  Serial.println("COMANDO ==> ");
  comando = EEPROM.read(1);
  if (comando == 0 || comando == 1) {
    lcd.imprimir("Comandos lidos"); 
  }
  else {
    comando = 0;
    lcd.imprimir("Aguardando comandos");
  }
  Serial.println(comando);
}
  
void loop() {
  if (Serial.available() > 0){
      String recebido = leStringSerial();
      if (recebido.toInt() != comando) {
        comando = recebido.toInt();
        EEPROM.write(1, comando);  
      }
  }
  switch (comando) {
    case 0:  
      lcd.apagarTudo();
      lcd.imprimir("comando 0");
      break;
     case 1:
      lcd.apagarTudo();
      dht.ler();
      lcd.imprimir("T:", 0, 0);
      lcd.imprimir(dht.getTemp(), 3, 0);
      break; 
  }
  delay (2000);
  /*if (Serial.available() > 0){
    Serial.println("Recebendo mes...");
    String recebido = leStringSerial();
    MES = recebido.toInt();
    EEPROM.write(0, MES);
    for (int i = 0; i < QUANT_CANAIS; i++) {
      Serial.print("Lendo canal ");
      Serial.println(i+1);
      int media = canais[i].getMedia();
      Serial.print("MEDIA: ");
      Serial.println(media);
      if (media < SOLO_SECO) {
        Serial.println("Solo seco...");
        Serial.println("Solicitar irrigacao!");
        int resul = getCaixaByCanalAndMes(MES, canais[i].getNome());   
        Serial.print("CAIXA: ");
        Serial.println(resul);
        if (resul == 1){
          digitalWrite(caixa1, HIGH);
          digitalWrite(caixa2, LOW);
          digitalWrite(caixa3, LOW);
        }
        else if (resul == 2){
          digitalWrite(caixa1, LOW);
          digitalWrite(caixa2, HIGH);
          digitalWrite(caixa3, LOW);
        }
        else if (resul == 3){
          digitalWrite(caixa1, LOW);
          digitalWrite(caixa2, LOW);
          digitalWrite(caixa3, HIGH);
        }
        else if (resul == -1) {
          Serial.println("Mes e canal nao possuem caixa ainda...");  
        }
        else {
          digitalWrite(caixa1, LOW);
          digitalWrite(caixa2, LOW);
          digitalWrite(caixa3, LOW);
        }
      }
      else {
        Serial.println("Nao eh necessario irrigar...");  
      }
      Serial.println();
    }
    Serial.println("Fim do processo!");
    delay(50);
    digitalWrite(caixa1, LOW);
    digitalWrite(caixa2, LOW);
    digitalWrite(caixa3, LOW);
    Serial.println();
    Serial.println();
  }*/
}

int getCaixaByCanalAndMes(int mes, int canal) {
    for (int i = 0; i <= 56; i++) {
        if (sequencia[i][0] == mes) {
            while (sequencia[i][0] == mes) {
                if (sequencia[i][2] == canal) {
                    return (int)sequencia[i][1];
                }
                i++;
            }
            return -1;
        }
    }
}

String leStringSerial(){
  char caractere;
  String conteudo;
  Serial.println("Lendo comandos");
  while(Serial.available() > 0) {
    caractere = Serial.read();
    if (caractere != '\n'){
      conteudo.concat(caractere);
    }
    delay(10);
  }
  return conteudo;
}

void lerSensor() {
  Serial.println("VALOR DO SENSOR BUTRO ==> ");
  Serial.println(analogRead(SENSOR_UMI_SOLO1));

  int anaValue;
  digitalWrite(SMS_VCC, LOW);
  digitalWrite(SMS_GND, HIGH);
  delay(3000);
  anaValue = analogRead(A1); // Lendo valor analógico da porta setada no construtor
  digitalWrite(SMS_VCC, HIGH);
  digitalWrite(SMS_GND, LOW);
  delay(3000);
  digitalWrite(SMS_VCC, LOW);
  digitalWrite(SMS_GND, LOW);

  Serial.println(anaValue);

  anaValue = map(anaValue, 1023, 0, 100, 0);

  Serial.println(anaValue);

}
