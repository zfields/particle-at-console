/*
 * Project: Particle AT Console
 * Description: Submit AT command via the Particle Console to be executed and logged.
 * Author: Zachary J. Fields
 * Date: 23 APR 2019
 */

#include <Particle.h>
#include <spark_wiring_cellular.h>

String at_command;
volatile bool at_command_pending;

// Enable logging
SerialLogHandler logHandler(LOG_LEVEL_ALL);

// Cloud functions must return int and take one String
int executeAtCommand(String at_command_)
{
  at_command = at_command_;
  at_command_pending = true;
  return 0;
}
// setup() runs once, when the device is first turned on.
void setup()
{
  // Put initialization like pinMode and begin functions here.
  at_command_pending = false;
  while (!Particle.function("execAtCmd", executeAtCommand))
  {
    Log("ERROR: Unable to register cloud function!");
    delay(5000);
  }
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  // The core of your code will likely live here.
  int error_code = 0;

  if (at_command_pending)
  {
    Log("/%%%%%%%%%%%%%%%%%%%%%% START CLOUD REQUEST %%%%%%%%%%%%%%%%%%%%%%/");
    if (RESP_OK != (error_code = Cellular.command("%s\r\n", at_command.c_str())))
    {
      Log("ERROR: Failed to send AT command! (error code: %d)", error_code);
    }
    Log("/%%%%%%%%%%%%%%%%%%%%%%%% END CLOUD REQUEST %%%%%%%%%%%%%%%%%%%%%%%%/");
    at_command_pending = false;
  }
}
