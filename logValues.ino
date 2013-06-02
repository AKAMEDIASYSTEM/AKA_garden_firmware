// log format is [millis],[bed_temp in C],[reservoir_temp in C],[reservoir level (relative)],[photocell value (relative)],[pumpStatus]

void logValues(){
  
  if(firstTimeLogging){
    logSerial.println("time*logInterval(1 min), bed_temp, reservoir_temp, reservoir_level, photocell_value, pumpStatus, gallons_added");
    firstTimeLogging = false; 
  }

  logSerial.print(log_count);
  logSerial.print(",");
  logSerial.print(bed_temp);
  logSerial.print(",");
  logSerial.print(reservoir_temp);
  logSerial.print(",");
  logSerial.print(reservoir_level);
  logSerial.print(",");
  logSerial.print(photocell);
  logSerial.print(",");
  logSerial.print(isPumping);
  logSerial.print(",");
  logSerial.println(gals_added);

  gals_added = 0; // reset this value so we don't overcount added gallons
}

