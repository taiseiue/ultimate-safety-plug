#include "M5Atom.h"
#include "main.h"
#include "DisplayUtility.h"

void setup() {
  // void M5Atom::begin(bool SerialEnable , bool I2CEnable , bool DisplayEnable )
  M5.begin(true, false, true);
  Serial.begin(115200);
  DisplayUtility::Clear();
}

int printcount=0;
//メインループ
void loop(){
  //出力
  int raw=analogRead(33);
  Serial.println(raw);
  /*
  Serial.print(raw);
  Serial.print(",");
  Serial.print(a);
  Serial.println("");
  */
  
}
