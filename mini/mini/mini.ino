#include "rele.cpp"

#define BOMBA 7
#define DATA_INPUT 2

#define PIN_SOLENOIDE1 11 // Solenoide da caixa
#define NIVEL_LOGICO_RELE_SOLENOIDE1 LOW

#define PIN_SOLENOIDE2 12 // Solenoide das plantas
#define NIVEL_LOGICO_RELE_SOLENOIDE2 LOW

#define PIN_BOMBA 13
#define NIVEL_LOGICO_RELE_BOMBA LOW
#define TEMPO_BOMBA_LIGADA 5 // Tempo em segundos

Rele bomba(PIN_BOMBA, NIVEL_LOGICO_RELE_BOMBA); // Instanciando o rele da bomba
Rele solenoide_caixa(PIN_SOLENOIDE1, NIVEL_LOGICO_RELE_SOLENOIDE1);
Rele solenoide_plantas(PIN_SOLENOIDE2, NIVEL_LOGICO_RELE_SOLENOIDE2);

#define QUANT_VERIFICACOES 5

int count = 0;

void liberarSolucao();
void agitarSolucao();

void setup() {                
  pinMode(BOMBA, OUTPUT);
  pinMode(DATA_INPUT, INPUT);
  pinMode(PIN_BOMBA, OUTPUT);
  pinMode(PIN_SOLENOIDE1, OUTPUT);
  pinMode(PIN_SOLENOIDE2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println("Aguardando dados...");
  if (digitalRead(DATA_INPUT) == HIGH) {
    count++;
    Serial.println("Dado recebido!");
    delay(5000);
    if (count == QUANT_VERIFICACOES) {
      Serial.println("Recebido confirmacao para irrigar!");
      digitalWrite(BOMBA, HIGH);
      delay(1000);
      Serial.println("Agitando solucao...");
      agitarSolucao();
      Serial.println("Liberando solucao...");
      liberarSolucao();
      count = 0;
      Serial.println("Fim do processo!");
    }
  }
  digitalWrite(BOMBA, LOW);
  delay(20);
}

void liberarSolucao() {
  bomba.desligar();
  solenoide_caixa.desligar();
  delay(1000);
  solenoide_plantas.ligar();
  bomba.ligar();
  delay(2000);
  unsigned long tempo = 0;
  while (tempo != TEMPO_BOMBA_LIGADA) {
    tempo++;
    delay(1000);
  }
  bomba.desligar();
  delay(2000);
  solenoide_plantas.desligar();
  delay(2000);
}

void agitarSolucao() {
  solenoide_plantas.desligar();
  solenoide_caixa.ligar();
  delay(2000);
  bomba.ligar();
  delay(2000);
  unsigned long tempo = 0;
  while (tempo != 5) {
    tempo++;
    delay(1000);
  }
  bomba.desligar();
  delay(2000);
  solenoide_caixa.desligar();
  delay(2000);
}

