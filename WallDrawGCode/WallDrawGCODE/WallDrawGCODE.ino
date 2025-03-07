#include "QH_Configuration.h"
#include "gcode_parser.h"
#include "QHStepper.h"
#include "pen.h"




String gcode_command="";
float destination[XYZ] = {0,0,0};
float current_position[XYZ] = {0,0,0};
long current_steps_M1 = 0, current_steps_M2 = 0;
char buffer[40];

void setup() {
  Serial.begin(115200);
  stepper_init();
  setup_pen();
  delay(1200);
  Serial.println("Grbl 1.1h ['$' for help]");
  delay(1200);
  Serial.println("<Idle|MPos:0.000,0.000,0.000|FS:0,0|Ov:100,100,100>");
}

void loop() {
  if( get_command() > 0 ){
    position_status("Run");
    process_parsed_command();
    gcode_command = "";
    position_status("Idle");
    Serial.println("ok");
  } 
  
 
}

byte get_command(){
  static int len = 0;
  static char tmpC = 0;
  while(Serial.available() > 0){
    tmpC = Serial.read();
    if(tmpC == 0x3f) continue;
    if(tmpC == 10 || len > 35 ){  len = 0; return 1;}
    gcode_command += tmpC;
    len++;
  }
  return 0;
}

void position_status (String Status)
{
  Serial.print("<");
  Serial.print(Status);
  Serial.print("|MPos:");
  Serial.print(current_position[X_AXIS]);
  Serial.print(",");
  Serial.print(current_position[Y_AXIS]);
  Serial.print(",");
  Serial.print(current_position[Z_AXIS]);
  Serial.println("|FS:0,0|Ov:100,100,100>");
}
