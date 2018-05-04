#include <Arduino.h>
#include "sensor_umi_solo.cpp";

class Canal {
  private:
    Solo *solo1;
    Solo *solo2;
    Solo *solo3;
    int nome;
  public:
    Canal(int nome, unsigned char SENSOR_UMI_SOLO1, unsigned char SENSOR_UMI_SOLO2, unsigned char SENSOR_UMI_SOLO3, unsigned char SMS_VCC, unsigned char SMS_GND) {
      this->solo1 = new Solo(SENSOR_UMI_SOLO1, SMS_VCC, SMS_GND);
      this->solo2 = new Solo(SENSOR_UMI_SOLO2, SMS_VCC, SMS_GND);
      this->solo3 = new Solo(SENSOR_UMI_SOLO3, SMS_VCC, SMS_GND);
      this->nome = nome;
    }
    int getNome() {
      return this->nome;
    }
    int getMedia() {
      solo1->ler();
      solo2->ler();
      solo3->ler();
      return ((solo1->getValor() + solo2->getValor() + solo3->getValor()) / 3);
    }
    int getValor(const int sensor) {
      if (sensor == 1) {
        return solo1->getValor();
      }
      else if (sensor == 2) {
        return solo2->getValor();
      }
      else if (sensor == 3) {
        return solo3->getValor();
      }
      else {
        return -1;
      }
    }
};
