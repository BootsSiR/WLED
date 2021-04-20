#include "wled.h"
/*
 * Register your v2 usermods here!
 *   (for v1 usermods using just usermod.cpp, you can ignore this file)
 */

/*
 * Add/uncomment your usermod filename here (and once more below)
 * || || ||
 * \/ \/ \/
 */
//#include "usermod_v2_example.h"
#ifdef USERMOD_DALLASTEMPERATURE
#include "../usermods/Temperature/usermod_temperature.h"
#endif
//#include "usermod_v2_empty.h"
#ifdef USERMOD_BUZZER
#include "../usermods/buzzer/usermod_v2_buzzer.h"
#endif

#include "../mymods/piezo/piezo_mod.h"
#include "../mymods/temp/temp_mod.h"
#include "../mymods/passiveir_mod/passiveir_mod.h"
#include "../mymods/lightsensor_mod/lightsensor_mod.h"
#include "../mymods/pin_test_mod/pin_test_mod.h"
#include "../mymods/mcp3208/mcp3208_mod.h"
#include "../mymods/current_monitoring_mod/current_monitoring_mod.h"
//#include "../usermods/PIR_sensor_switch/usermod_PIR_sensor_switch.h"

void registerUsermods()
{
  /*
   * Add your usermod class name here
   * || || ||
   * \/ \/ \/
   */
  
  //usermods.add(new MyExampleUsermod());
  #ifdef USERMOD_DALLASTEMPERATURE
  //usermods.add(new UsermodTemperature());
  #endif
  //usermods.add(new UsermodRenameMe());
  #ifdef USERMOD_BUZZER
  usermods.add(new BuzzerUsermod());
  #endif

  //usermods.add(new PinTestUsermod());
  usermods.add(new PiezoMod());
  usermods.add(new TempMod());
  usermods.add(new PassiveIRMod());
  usermods.add(new MCP3208Mod());
  usermods.add(new CurrentMonitoringMod());
  //usermods.add(new LightSensorMod());
  //usermods.add(new PIRsensorSwitch());
}