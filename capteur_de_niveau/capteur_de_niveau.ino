/*!
 * @file  SEN0205.ino
 * @brief  This example is to get liquid level. (Liquid Level Sensor-FS-IR02)
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  jackli(Jack.li@dfrobot.com)
 * @version  V1.0
 * @date  2016-1-30
 */

int liquidLevel = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop()
{
  liquidLevel = digitalRead(A0);
  Serial.print("liquidLevel= "); Serial.println(liquidLevel, DEC);
  delay(500);
}
