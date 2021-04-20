/*
 * WLED Arduino IDE compatibility file.
 *
 * Where has everything gone?
 *
 * In April 2020, the project's structure underwent a major change.
 * Global variables are now found in file "wled.h"
 * Global function declarations are found in "fcn_declare.h"
 *
 * Usermod compatibility: Existing wled06_usermod.ino mods should continue to work. Delete usermod.cpp.
 * New usermods should use usermod.cpp instead.
 */
#include "wled.h"

//#include "soc/soc.h"
//#include "soc/rtc_cntl_reg.h"

void setup() {
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  WLED::instance().setup();
}

void loop() {
  WLED::instance().loop();
}
