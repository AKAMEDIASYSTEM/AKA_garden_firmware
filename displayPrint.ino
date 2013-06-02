void displayPrint(int elapsed){
  displaySerial.write('v'); // clear display
//
//  //  int count = intervalDuration-elapsed;
//  int count = (int)elapsed/1000.0;
//  //  char tempString[10];
//  String sCount = String(count);
//  //  Serial.print(sCount);
//  //  Serial.println(" elapsed");
//  if(sCount.length() > 4){ // needs zero padding
//    sCount = sCount.substring(0,3);
//    //    sprintf(tempString,"%04d",sCount);
//  }
//  displaySerial.print(sCount);
char tempString[10];
sprintf(tempString,"%04d",log_count);
displaySerial.print(tempString);
}

