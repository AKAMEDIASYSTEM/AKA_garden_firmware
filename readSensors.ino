void readSensors(){


  // read the sensors
  bed_temp = 1023 - analogRead(BED_TEMP);
  bed_temp = convertC(bed_temp);

  reservoir_temp = 1023 - analogRead(RESERVOIR_TEMP);
  reservoir_temp = convertC(reservoir_temp);

  // need to check if the following circuit was done right
  reservoir_level = analogRead(RESERVOIR_LEVEL);

  // also need to check this one to make sure it's done right
  photocell = analogRead(PHOTOCELL);
  
  sensors[0] =(float)bed_temp;
  sensors[1] = (float)reservoir_temp; 
  sensors[2] = (float)reservoir_level;
  sensors[3] = (float)photocell;
}

