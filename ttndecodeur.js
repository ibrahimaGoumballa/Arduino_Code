//Uplink
//Batterie - short
//Poids - short
//GPS (longitude, latitude) - long 
//Détection eau -bool
//Downlink
//Start Tamisage - bool
//End Tamisage - bool
/*
function decodeUplink(input) {
  // ------------- BATTERY ---------------
  var batteryBytes = (input.bytes[1] << 8 | input.bytes[0]);
  let batteryStateValue = batteryBytes >> 14;
  let batteryStateMsg = "";
  switch (batteryStateValue) {
    case 0:
      batteryStateMsg = "Ultra Low";
      break;
    case 1:
      batteryStateMsg = "Low";
      break;
    case 2:
      batteryStateMsg = "OK";
      break;
    case 3:
      batteryStateMsg = "Good";
      break;
  }
  let batteryVoltage = (batteryBytes & 0x3FFF)/100;

    // ------------- WEIGHT ---------------
  var weightBytes = (input.bytes[3] << 8 | input.bytes[2]);
  let weight = weightBytes/100;
  // ------------- SHORT VARIABLES ---------------
  let waterDetection = input.bytes[4] !== 0; // assuming non-zero value indicates water detection

  // ------------- LONG VARIABLES ---------------
  let latitude = (input.bytes[7] << 24 | input.bytes[8] << 16 | input.bytes[9] << 8 | input.bytes[10]) / 1e6;
  let longitude = (input.bytes[11] << 24 | input.bytes[12] << 16 | input.bytes[13] << 8 | input.bytes[14]) / 1e6;

  // ------------- OUTPUT ---------------
  return {
    data: {
      batteryState: batteryStateMsg,
      batteryVoltage_V: batteryVoltage,
      weight: weight,
      waterDetection: waterDetection,
      latitude: latitude,
      longitude: longitude,
    },
  };
}*/

function decodeUplink(input) {
  // ------------- BATTERY ---------------
  var batteryBytes = (input.bytes[1] << 8 | input.bytes[0]);
  let batteryStateValue = batteryBytes >> 14;
  let batteryStateMsg = "";
  switch (batteryStateValue) {
    case 0:
      batteryStateMsg = "Ultra Low";
      break;
    case 1:
      batteryStateMsg = "Low";
      break;
    case 2:
      batteryStateMsg = "OK";
      break;
    case 3:
      batteryStateMsg = "Good";
      break;
  }
  let batteryVoltage = (batteryBytes & 0x3FFF) / 100;

  // ------------- WEIGHT ---------------
  var weightBytes = (input.bytes[3] << 8 | input.bytes[2]);
  let weight = weightBytes / 100;

  // ------------- LIQUID VALUE ---------------
  let liquidValue = input.bytes[5]; // Assuming it's a single byte value

  // ------------- LONG VARIABLES ---------------
  let latitude = ((input.bytes[9] << 24 | input.bytes[8] << 16 | input.bytes[7] << 8 | input.bytes[6]))/ 10000000;
  let longitude = ((input.bytes[13] << 24 | input.bytes[12] << 16 | input.bytes[11] << 8 | input.bytes[10]))/ 10000000;


  // ------------- OUTPUT ---------------
  return {
    data: {
      batteryState: batteryStateMsg,
      batteryVoltage_V: batteryVoltage,
      weight: weight,
      liquidValue: liquidValue,
      latitude: latitude,
      longitude: longitude,
    },
  };
}
