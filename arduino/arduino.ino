#include "canal.cpp";
#include "matriz.cpp";
#include "sensor_de_temp_umi.cpp";
#include "sensor_de_luz.cpp";
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

#define SENSOR_LUZ A0
#define NOITE 80

Ldr ldr1(SENSOR_LUZ);

#define DHTPIN 8
#define DHTTYPE DHT11 // DHT 11

DHT dhtParam(DHTPIN, DHTTYPE);
TempUmi dht(&dhtParam); // Passando sensor DHT como parâmetro para o nosso objeto

Canal canal1(1, SENSOR_UMI_SOLO1, SENSOR_UMI_SOLO1, SENSOR_UMI_SOLO1, SMS_VCC, SMS_GND);
Canal canal2(2, SENSOR_UMI_SOLO2, SENSOR_UMI_SOLO2, SENSOR_UMI_SOLO2, SMS_VCC, SMS_GND);
Canal canal3(3, SENSOR_UMI_SOLO3, SENSOR_UMI_SOLO3, SENSOR_UMI_SOLO3, SMS_VCC, SMS_GND);

#define QUANT_CANAIS 3
Canal canais[3] = { canal1, canal2, canal3 };

int MES = 1;
int comando = 0;

int getCaixaByCanalAndMes(int mes, int canal);
String leStringSerial();
void iniciarProcesso();

void setup() {
  pinMode(SENSOR_UMI_SOLO1, INPUT);
  pinMode(SENSOR_LUZ, INPUT);
  pinMode(caixa1, OUTPUT);
  pinMode(caixa2, OUTPUT);
  pinMode(caixa3, OUTPUT);
  digitalWrite(SMS_VCC, LOW);
  digitalWrite(SMS_GND, LOW);
  Serial.begin(9600);
  dht.iniciar();
  delay(2000);
  MES = EEPROM.read(0);
}
  
void loop() {
  if (Serial.available() > 0){
      String recebido = leStringSerial();
      if (recebido == "s") {
        Serial.println("*** Dados dos sensores extras *** ");
        Serial.println();
        if(!dht.ler()) {
          Serial.print("Temperatura: ");
          Serial.println(dht.getTemp());
          Serial.print("Umidade: ");
          Serial.println(dht.getUmi()); 
        }
        ldr1.ler();
        Serial.print("Luminosidade: ");
        Serial.println(ldr1.getValor());
      }
      else {
       Serial.println(); 
       Serial.print("*** Lendo configuracoes para o mes ");
       MES = recebido.toInt();
       Serial.print(MES);
       Serial.println(" ***");
       if (MES < 1) {
        Serial.println(" %%% Mes enviado esta invalido! %%% ");
        MES = EEPROM.read(0);
       }else {
        EEPROM.write(0, MES);
        iniciarProcesso();
       }
       delay(1000);
    }
  }
  iniciarProcesso();
  delay(20);
}

void iniciarProcesso() {
  ldr1.ler();
  if (ldr1.getValor() < NOITE) {
    Serial.println();
    Serial.println("%%% Nao eh necessario irrigar (NOITE) %%%");
    Serial.println();
    delay(2000);
  }
  else {
   Serial.println();
  Serial.println();
   Serial.print("%%% Iniciando Rotina para mes ");
   Serial.print(MES);
   Serial.println(" %%%");
   Serial.println("-------------------");
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
      Serial.println("-------------------");
      Serial.println();
    }
    Serial.println("*** Fim do processo ***");
    delay(50);
    digitalWrite(caixa1, LOW);
    digitalWrite(caixa2, LOW);
    digitalWrite(caixa3, LOW);
    Serial.println();
    Serial.println(); 
  }
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
  while(Serial.available() > 0) {
    caractere = Serial.read();
    if (caractere != '\n'){
      conteudo.concat(caractere);
    }
    delay(10);
  }
  return conteudo;
}
