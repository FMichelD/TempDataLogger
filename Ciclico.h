//#ifndef CICLICO_H
//#define CICLICO_H
//
//#include "SdFs.h"
//
////#if SD_FAT_TYPE == 1
////  File tempCicle;
////#elif SD_FAT_TYPE == 2
////  ExFile tempCicle;
////#elif SD_FAT_TYPE == 3
////  FsFile tempCicle;
////#endif  // SD_FAT_TYPE
//
//FsFile tempCicle;
//
//String buffer;
//
//void readTempCiclic(void)
//{
//  static int lineCount = 0;
//
//  Serial.println("Lendo arquivo de ciclos");
//  
//  if (!tempCicle.open("TempCicle.csv", FILE_READ)) 
//  {
//      //error("open failed");
//      Serial.println("SD File TempCicle.csv fail");
//  }
//
//  tempCicle.rewind();
//  
//  Serial.println("Fechando arquivo de ciclos");
//  Serial.println(digitalRead(SS));
//  delay(100);
//  while(tempCicle.available())
//  {
//    buffer = tempCicle.readStringUntil('\n');
//    Serial.print("Linha: ");
//    Serial.println(lineCount);
//    Serial.println(buffer);
//    lineCount++;
//  }
//  tempCicle.close();
//  Serial.println(digitalRead(SS));
//  delay(100);
//}
//
//#endif
