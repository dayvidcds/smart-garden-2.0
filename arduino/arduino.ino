#include "canal.cpp";

#define caixa1 2
#define caixa2 3
#define caixa3 4

#define SENSOR_UMI_SOLO1 A1
#define SENSOR_UMI_SOLO2 A2
#define SENSOR_UMI_SOLO3 A3

#define SMS_VCC 7
#define SMS_GND 6

#define SOLO_SECO 40
#define SOLO_UMIDO 60

Canal canal1(1, SENSOR_UMI_SOLO1, SENSOR_UMI_SOLO1, SENSOR_UMI_SOLO1, SMS_VCC, SMS_GND);
Canal canal2(2, SENSOR_UMI_SOLO2, SENSOR_UMI_SOLO2, SENSOR_UMI_SOLO2, SMS_VCC, SMS_GND);
Canal canal3(3, SENSOR_UMI_SOLO3, SENSOR_UMI_SOLO3, SENSOR_UMI_SOLO3, SMS_VCC, SMS_GND);

#define QUANT_CANAIS 3
Canal canais[3] = { canal1, canal2, canal3 };

int MES = 1;

int getCaixaByCanalAndMes(int mes, int canal, char sequencia[56][3]) {
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

char sequencia[56][3] = {
  1,1,1,
  
  2,1,2,
  2,2,1,
  
  3,1,3,
  3,2,2,
  3,3,1,
  
  4,1,4,
  4,2,3,
  4,3,1,
  4,3,2,
  
  5,1,5,
  5,2,4,
  5,3,1,
  5,3,2,
  5,3,3,
  
  6,1,6,
  6,2,5,
  6,3,1,
  6,3,2,
  6,3,3,
  6,3,4,
  
  7,1,1,
  7,2,6,
  7,3,2,
  7,3,3,
  7,3,4,
  7,3,5,
  
  8,1,2,
  8,2,1,
  8,3,3,
  8,3,4,
  8,3,5,
  
  9,1,3,
  9,2,2,
  9,3,1,
  9,3,4,
  9,3,5,
  9,3,6,
  
  10,1,4,
  10,2,3,
  10,3,1,
  10,3,2,
  10,3,5,
  10,3,6,
  
  11,1,5,
  11,2,4,
  11,3,1,
  11,3,2,
  11,3,3,
  11,3,6,
  
  12,1,6,
  12,2,4,
  12,3,1,
  12,3,2,
  12,3,3,
  12,3,4,
};

void setup() {
  // put your setup code here, to run once:
  pinMode(caixa1, OUTPUT);
  pinMode(caixa2, OUTPUT);
  pinMode(caixa3, OUTPUT);
  Serial.begin(9600);
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
  Serial.println("Retornando Conteudo...");
  return conteudo;
}
  
void loop() {
  if (Serial.available() > 0){
    Serial.println("Recebendo mes...");
    String recebido = leStringSerial();
    MES = recebido.toInt();
    for (int i = 0; i < QUANT_CANAIS; i++) {
      Serial.print("Lendo canal ");
      Serial.println(i+1);
      int media = canais[i].getMedia();
      Serial.print("MEDIA: ");
      Serial.println(media);
      if (media < SOLO_SECO) {
        Serial.println("Solo seco...");
        Serial.println("Solicitar irrigacao!");
        int resul = getCaixaByCanalAndMes(MES, canais[i].getNome(), sequencia);   
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
  }
}
