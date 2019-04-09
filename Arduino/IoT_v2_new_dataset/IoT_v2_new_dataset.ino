#include "CurieIMU.h"
#include "CuriePME.h"
#include <CurieBLE.h>
//#include "MemoryFree.h"
#include <avr/pgmspace.h>

const unsigned int sampleRateHZ = 100;

const unsigned int vectorNumBytes = 128;

const unsigned int samplesPerVector = (vectorNumBytes / 3);


const unsigned int sensorBufSize = 2048;
const int IMULow = -32768;
const int IMUHigh = 32767;


BLEPeripheral blePeripheral;
BLEService fitnessService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEUnsignedCharCharacteristic fitnessTypeChar("19B10001-E8F2-537E-4F6C-D104768A1214",
    BLERead | BLENotify);     

void setup()
{
  Serial.begin(9600);
  //while (!Serial);
  Serial.println("INIT COMPL");

  CurieIMU.begin();
  CuriePME.begin();

  CurieIMU.setAccelerometerRate(sampleRateHZ);
  CurieIMU.setAccelerometerRange(1);

  //CuriePME.setClassifierMode(CuriePME.KNN_Mode);

  /*category 1 = still*/
  static byte subject102_1_0[] PROGMEM = {185, 184, 185, 184, 179, 179, 183, 180, 185, 184, 183, 186, 184, 184, 187, 187, 188, 189, 181, 179, 187, 186, 182, 184, 184, 185, 189, 193, 196, 194, 188, 182, 184, 180, 185, 186, 184, 183, 185, 182, 181, 183, 183, 184, 183, 182, 176, 182, 187, 183, 183, 183, 178, 175, 175, 179, 188, 195, 195, 199, 193, 182, 182, 181, 184, 187, 192, 192, 168, 192, 184, 181, 182, 183, 186, 188, 187, 188, 187, 188, 184, 186, 183, 184, 184, 191, 197, 179, 182, 182, 179, 177, 183, 187, 189, 186, 184, 185, 187, 186, 185, 186, 186, 186, 185, 184, 185, 186, 185, 186, 186, 186, 189, 190, 195, 190, 187, 186, 185, 188};
  CuriePME.learn(subject102_1_0, 120, 1);

  static byte subject102_1_1[] PROGMEM = {186, 186, 185, 185, 185, 185, 185, 185, 186, 186, 186, 186, 186, 186, 186, 186, 185, 185, 186, 186, 186, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 186, 186, 186, 186, 185, 185, 186, 185, 185, 185, 186, 186, 185, 186, 186, 185, 185, 185, 186, 186, 185, 185, 185, 185, 186, 185, 185, 186, 186, 186, 185, 185, 186, 186, 185, 185, 185, 185, 186, 186, 186, 185, 186, 185, 185, 185, 186, 186, 186, 186, 186, 186, 185, 186, 186, 186, 186, 185, 185, 185, 186, 186, 186, 185, 185, 186, 186, 186, 186, 186, 185, 186, 186, 186, 185, 185, 185, 186, 186, 186, 186, 186, 185, 186, 186, 186, 186, 185};
  CuriePME.learn(subject102_1_1, 120, 1);

  static byte subject102_1_2[] PROGMEM = {186, 186, 185, 185, 185, 185, 185, 185, 186, 186, 186, 186, 186, 186, 186, 186, 185, 185, 186, 186, 186, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 185, 186, 186, 186, 186, 185, 185, 186, 185, 185, 185, 186, 186, 185, 186, 186, 185, 185, 185, 186, 186, 185, 185, 185, 185, 186, 185, 185, 186, 186, 186, 185, 185, 186, 186, 185, 185, 185, 185, 186, 186, 186, 185, 186, 185, 185, 185, 186, 186, 186, 186, 186, 186, 185, 186, 186, 186, 186, 185, 185, 185, 186, 186, 186, 185, 185, 186, 186, 186, 186, 186, 185, 186, 186, 186, 185, 185, 185, 186, 186, 186, 186, 186, 185, 186, 186, 186, 186, 185};
  CuriePME.learn(subject102_1_2, 120, 1);

  

  /*category = 3  walk*/
  static byte subject102_3_0[] PROGMEM = {220, 217, 201, 183, 173, 170, 188, 189, 195, 197, 185, 172, 166, 168, 169, 171, 174, 177, 179, 179, 180, 181, 183, 188, 197, 205, 211, 217, 214, 204, 192, 180, 180, 184, 186, 192, 188, 181, 174, 169, 165, 164, 167, 170, 175, 179, 183, 186, 187, 185, 182, 180, 184, 195, 204, 216, 225, 215, 198, 178, 170, 173, 187, 189, 193, 191, 183, 176, 173, 173, 173, 172, 173, 176, 179, 181, 183, 184, 184, 187, 194, 200, 204, 207, 209, 205, 197, 188, 183, 185, 186, 189, 190, 185, 178, 172, 169, 161, 164, 172, 180, 184, 184, 182, 181, 181, 182, 185, 187, 192, 196, 205, 214, 220, 216, 191, 178, 173, 186, 192};
  CuriePME.learn(subject102_3_0, 120, 3);

  static byte subject102_3_1[] PROGMEM = {171, 175, 180, 181, 183, 182, 182, 184, 186, 193, 202, 211, 216, 212, 204, 190, 182, 185, 185, 186, 185, 183, 177, 174, 171, 167, 165, 167, 171, 176, 178, 179, 185, 191, 192, 184, 184, 187, 197, 207, 218, 218, 213, 193, 183, 177, 180, 188, 191, 195, 190, 180, 170, 167, 167, 170, 173, 174, 175, 175, 178, 182, 184, 184, 185, 191, 200, 212, 222, 222, 207, 189, 178, 174, 191, 197, 199, 194, 186, 178, 173, 169, 164, 161, 165, 168, 173, 175, 181, 187, 189, 188, 190, 196, 205, 212, 217, 217, 203, 183, 172, 167, 183, 195, 200, 201, 188, 176, 169, 165, 165, 168, 173, 178, 182, 184, 184, 182, 180, 185};
  CuriePME.learn(subject102_3_1, 120, 3);

  static byte subject102_3_2[] PROGMEM = {190, 189, 184, 182, 178, 174, 172, 169, 166, 165, 167, 173, 179, 185, 189, 188, 184, 181, 179, 182, 192, 202, 213, 221, 226, 211, 190, 170, 174, 188, 194, 196, 189, 180, 173, 170, 170, 171, 173, 176, 179, 181, 184, 184, 180, 178, 181, 188, 200, 209, 218, 217, 204, 190, 180, 180, 186, 190, 190, 186, 180, 174, 168, 164, 163, 167, 174, 179, 181, 183, 186, 186, 184, 184, 189, 198, 209, 217, 222, 217, 193, 181, 170, 182, 191, 196, 198, 189, 178, 171, 169, 168, 169, 171, 174, 176, 181, 184, 183, 180, 181, 187, 197, 210, 220, 214, 202, 191, 182, 184, 190, 191, 192, 186, 179, 175, 170, 166, 166, 169};
  CuriePME.learn(subject102_3_2, 120, 3);

  static byte subject102_3_3[] PROGMEM = {166, 171, 175, 178, 180, 182, 184, 182, 185, 190, 198, 207, 213, 211, 202, 192, 187, 184, 187, 189, 189, 187, 182, 178, 173, 168, 167, 170, 174, 178, 181, 181, 182, 183, 183, 181, 183, 189, 197, 206, 213, 219, 217, 195, 179, 169, 178, 189, 197, 201, 192, 181, 175, 173, 173, 173, 175, 179, 181, 181, 180, 179, 178, 178, 182, 189, 195, 201, 204, 208, 205, 196, 188, 179, 189, 193, 194, 196, 191, 182, 174, 167, 162, 161, 165, 173, 181, 186, 186, 185, 184, 183, 182, 182, 187, 200, 209, 218, 220, 214, 192, 178, 171, 182, 191, 198, 201, 192, 180, 173, 172, 172, 173, 174, 175, 176, 177, 178, 179, 180};
  CuriePME.learn(subject102_3_3, 120, 3);

  static byte subject102_3_4[] PROGMEM = {178, 181, 180, 178, 176, 176, 180, 189, 200, 208, 213, 216, 216, 201, 184, 170, 168, 187, 198, 206, 197, 180, 171, 167, 165, 167, 171, 176, 179, 179, 179, 177, 177, 183, 191, 200, 208, 216, 220, 208, 189, 173, 179, 192, 200, 197, 189, 181, 176, 173, 171, 169, 169, 174, 177, 179, 178, 176, 176, 178, 184, 194, 202, 208, 216, 219, 207, 184, 166, 160, 184, 203, 214, 203, 186, 173, 168, 167, 168, 173, 178, 180, 180, 179, 179, 178, 179, 183, 189, 194, 203, 210, 214, 209, 197, 186, 181, 185, 190, 194, 192, 187, 181, 175, 171, 169, 171, 176, 179, 181, 181, 179, 176, 177, 181, 184, 191, 198, 203, 206};
  CuriePME.learn(subject102_3_4, 120, 3);

  static byte subject102_3_5[] PROGMEM = {181, 188, 189, 184, 181, 178, 179, 183, 189, 194, 204, 213, 222, 223, 200, 177, 160, 168, 186, 195, 204, 197, 184, 176, 172, 171, 172, 174, 176, 178, 181, 184, 185, 182, 178, 181, 185, 188, 194, 203, 211, 211, 202, 192, 182, 177, 183, 191, 195, 196, 192, 185, 177, 170, 164, 162, 167, 175, 182, 185, 185, 184, 184, 183, 181, 185, 195, 209, 221, 229, 220, 202, 176, 156, 161, 180, 193, 204, 199, 188, 180, 174, 172, 171, 169, 172, 176, 179, 180, 182, 183, 182, 185, 189, 196, 204, 211, 215, 207, 194, 182, 173, 181, 190, 198, 201, 193, 182, 174, 167, 164, 164, 166, 171, 176, 181, 183, 185, 186, 185};
  CuriePME.learn(subject102_3_5, 120, 3);

  static byte subject102_3_6[] PROGMEM = {185, 185, 187, 185, 183, 185, 191, 199, 208, 215, 209, 217, 196, 182, 166, 177, 191, 200, 201, 193, 173, 165, 162, 163, 166, 170, 175, 180, 184, 185, 184, 180, 179, 181, 188, 196, 207, 216, 214, 213, 195, 182, 172, 190, 194, 199, 194, 184, 168, 168, 160, 160, 168, 175, 182, 186, 187, 186, 185, 181, 181, 183, 189, 199, 213, 221, 221, 207, 192, 164, 161, 171, 187, 207, 208, 196, 183, 170, 167, 169, 170, 172, 173, 177, 178, 182, 181, 179, 179, 182, 187, 198, 208, 218, 217, 204, 193, 180, 180, 189, 192, 198, 196, 189, 180, 174, 170, 167, 168, 171, 175, 179, 182, 184, 182, 180, 181, 186, 194, 204};
  CuriePME.learn(subject102_3_6, 120, 3);

  static byte subject102_3_7[] PROGMEM = {196, 203, 207, 211, 206, 197, 185, 178, 184, 194, 195, 206, 195, 185, 177, 171, 167, 165, 167, 171, 177, 181, 182, 182, 183, 184, 180, 182, 187, 197, 208, 217, 219, 214, 194, 175, 165, 171, 192, 204, 205, 194, 181, 171, 167, 166, 167, 170, 176, 183, 184, 187, 184, 175, 176, 182, 192, 203, 211, 215, 209, 198, 188, 185, 191, 191, 189, 189, 187, 181, 178, 174, 170, 168, 169, 172, 176, 178, 179, 178, 178, 182, 186, 198, 219, 223, 220, 226, 203, 166, 164, 166, 188, 195, 218, 193, 176, 169, 165, 164, 166, 170, 175, 179, 181, 182, 179, 181, 186, 194, 203, 210, 215, 210, 201, 189, 185, 188, 191, 191};
  CuriePME.learn(subject102_3_7, 120, 3);

  static byte subject102_3_8[] PROGMEM = {208, 209, 196, 178, 168, 162, 158, 159, 165, 172, 179, 184, 184, 183, 182, 179, 184, 199, 211, 216, 217, 214, 203, 186, 168, 165, 184, 198, 207, 200, 187, 173, 165, 162, 162, 166, 172, 178, 182, 185, 185, 184, 185, 185, 188, 197, 205, 215, 214, 201, 187, 178, 183, 196, 203, 200, 195, 186, 178, 172, 167, 166, 168, 173, 178, 182, 182, 180, 177, 175, 179, 183, 190, 197, 202, 209, 221, 223, 202, 177, 163, 175, 193, 200, 195, 186, 177, 172, 170, 169, 173, 177, 180, 182, 180, 180, 179, 177, 179, 184, 192, 199, 206, 211, 210, 196, 186, 181, 181, 186, 190, 195, 192, 187, 178, 172, 168, 172, 177, 181};
  CuriePME.learn(subject102_3_8, 120, 3);

  static byte subject102_3_9[] PROGMEM = {190, 196, 205, 213, 212, 200, 190, 184, 181, 183, 189, 189, 188, 186, 183, 179, 175, 172, 171, 175, 180, 183, 183, 181, 179, 178, 178, 181, 184, 185, 185, 188, 192, 198, 207, 215, 220, 201, 185, 173, 170, 180, 186, 192, 193, 188, 181, 176, 176, 175, 175, 175, 178, 179, 179, 180, 180, 181, 182, 181, 180, 182, 183, 189, 197, 205, 213, 213, 204, 196, 188, 179, 181, 187, 193, 189, 183, 176, 171, 169, 169, 170, 173, 177, 181, 184, 184, 182, 181, 186, 194, 200, 204, 205, 205, 204, 205, 195, 186, 183, 190, 195, 195, 187, 179, 173, 168, 167, 168, 170, 171, 173, 177, 179, 180, 182, 190, 198, 207, 211};
  CuriePME.learn(subject102_3_9, 120, 3);

  static byte subject102_3_10[] PROGMEM = {216, 197, 179, 168, 180, 191, 196, 201, 184, 172, 164, 162, 162, 163, 166, 170, 176, 182, 186, 190, 195, 201, 211, 220, 221, 208, 196, 185, 188, 196, 197, 189, 181, 172, 163, 159, 158, 161, 167, 175, 180, 185, 189, 188, 190, 200, 212, 216, 217, 213, 200, 188, 177, 181, 191, 196, 194, 180, 172, 167, 167, 168, 168, 169, 172, 175, 179, 183, 188, 197, 207, 218, 221, 215, 199, 179, 169, 190, 209, 214, 197, 182, 165, 158, 158, 162, 164, 170, 175, 179, 182, 185, 187, 190, 196, 210, 220, 225, 223, 204, 188, 175, 181, 191, 195, 190, 180, 172, 168, 165, 164, 164, 166, 170, 175, 180, 184, 185, 188, 195};
  CuriePME.learn(subject102_3_10, 120, 3);

  static byte subject102_3_11[] PROGMEM = {203, 208, 204, 192, 175, 163, 157, 156, 157, 164, 170, 175, 178, 181, 185, 186, 188, 200, 211, 215, 219, 224, 199, 182, 168, 180, 196, 201, 194, 182, 175, 172, 171, 170, 168, 167, 168, 171, 173, 178, 184, 191, 201, 209, 214, 218, 213, 195, 176, 170, 189, 206, 211, 204, 188, 171, 159, 155, 152, 157, 168, 177, 186, 192, 188, 185, 182, 183, 193, 207, 214, 218, 212, 201, 189, 180, 182, 194, 198, 199, 188, 178, 169, 165, 165, 167, 169, 172, 175, 177, 180, 182, 189, 196, 207, 215, 218, 207, 192, 178, 178, 193, 209, 217, 197, 182, 168, 160, 156, 154, 158, 164, 173, 182, 189, 193, 191, 186, 195, 209};
  CuriePME.learn(subject102_3_11, 120, 3);

  static byte subject102_3_12[] PROGMEM = {176, 178, 186, 204, 204, 187, 179, 159, 153, 159, 163, 168, 173, 179, 184, 188, 188, 188, 194, 196, 199, 207, 211, 221, 224, 195, 176, 171, 186, 187, 197, 190, 178, 170, 169, 167, 168, 168, 169, 171, 175, 179, 182, 188, 197, 203, 205, 212, 215, 206, 198, 185, 175, 189, 192, 201, 193, 179, 167, 164, 158, 159, 159, 169, 174, 187, 195, 188, 186, 185, 193, 198, 204, 208, 210, 214, 207, 193, 181, 176, 181, 193, 196, 186, 179, 174, 170, 167, 165, 166, 169, 171, 176, 180, 186, 193, 199, 205, 211, 221, 222, 204, 190, 181, 186, 183, 201, 186, 178, 172, 165, 159, 158, 161, 166, 174, 179, 185, 186, 186};
  CuriePME.learn(subject102_3_12, 120, 3);

  static byte subject102_3_13[] PROGMEM = {192, 201, 211, 225, 222, 205, 184, 167, 177, 188, 194, 192, 182, 180, 171, 169, 165, 166, 163, 170, 178, 182, 185, 182, 185, 188, 193, 200, 205, 213, 221, 222, 206, 185, 169, 170, 182, 190, 196, 192, 182, 173, 165, 161, 159, 163, 170, 175, 180, 185, 188, 189, 197, 201, 207, 213, 222, 217, 203, 177, 166, 177, 201, 200, 195, 185, 173, 167, 162, 159, 161, 160, 172, 181, 185, 185, 185, 184, 193, 194, 199, 210, 223, 234, 221, 193, 167, 160, 173, 191, 199, 198, 184, 179, 169, 163, 162, 160, 165, 172, 176, 180, 183, 190, 191, 195, 200, 208, 217, 219, 205, 192, 176, 187, 194, 196, 190, 180, 172, 168};
  CuriePME.learn(subject102_3_13, 120, 3);

  static byte subject102_3_14[] PROGMEM = {208, 214, 207, 189, 176, 178, 188, 197, 198, 192, 180, 176, 171, 168, 166, 169, 174, 178, 179, 178, 180, 185, 192, 199, 205, 208, 215, 209, 200, 183, 173, 183, 194, 202, 198, 184, 163, 161, 159, 160, 163, 168, 174, 180, 185, 188, 189, 191, 194, 195, 199, 207, 221, 226, 216, 190, 169, 169, 183, 194, 192, 181, 174, 174, 171, 168, 166, 168, 172, 175, 178, 183, 187, 191, 196, 203, 208, 216, 219, 210, 190, 173, 178, 191, 205, 204, 189, 175, 165, 160, 160, 160, 163, 169, 178, 184, 186, 183, 184, 191, 200, 203, 210, 221, 229, 210, 183, 172, 181, 192, 198, 188, 176, 173, 171, 167, 164, 163, 166, 170};
  CuriePME.learn(subject102_3_14, 120, 3);


  /*category 4 =  run
    static byte subject102_4_0[] PROGMEM = {219, 158, 118, 183, 150, 128, 160, 145, 132, 137, 138, 135, 114, 133, 142, 95, 126, 144, 96, 121, 135, 111, 121, 129, 128, 128, 138, 139, 130, 147, 156, 140, 148, 203, 162, 134, 255, 155, 115, 254, 169, 141, 255, 173, 156, 255, 179, 144, 250, 171, 114, 220, 157, 106, 190, 149, 110, 168, 144, 117, 140, 139, 121, 115, 131, 125, 104, 129, 124, 105, 129, 121, 106, 130, 120, 112, 135, 126, 125, 137, 126, 138, 139, 113, 164, 152, 97, 213, 166, 76, 255, 140, 91, 254, 176, 77, 255, 177, 97, 255, 177, 106, 255, 166, 104, 231, 156, 111, 193, 150, 122, 162, 148, 124, 138, 142, 130, 120, 136, 137};
    CuriePME.learn(subject102_4_0, 120, 4);


    static byte subject102_4_2[] PROGMEM = {254, 176, 165, 255, 147, 161, 253, 153, 126, 232, 142, 106, 205, 146, 113, 183, 149, 111, 158, 136, 108, 127, 132, 116, 106, 130, 117, 94, 111, 122, 105, 117, 129, 115, 129, 126, 136, 118, 121, 153, 134, 112, 192, 157, 90, 255, 167, 66, 254, 164, 82, 255, 184, 75, 252, 185, 67, 222, 186, 81, 196, 160, 102, 191, 159, 109, 182, 143, 126, 156, 124, 146, 135, 127, 155, 118, 132, 132, 114, 138, 123, 99, 130, 130, 107, 122, 132, 133, 110, 125, 158, 131, 124, 180, 148, 123, 244, 160, 125, 254, 175, 158, 255, 170, 157, 254, 160, 131, 252, 160, 98, 215, 149, 99, 198, 139, 115, 178, 129, 116};
    CuriePME.learn(subject102_4_2, 120, 4);


    static byte subject102_4_4[] PROGMEM = {254, 179, 72, 255, 171, 90, 254, 166, 103, 245, 159, 111, 210, 146, 123, 175, 142, 131, 154, 139, 136, 135, 135, 139, 117, 132, 141, 100, 125, 140, 100, 125, 138, 112, 129, 137, 130, 130, 140, 148, 134, 141, 172, 150, 144, 230, 175, 142, 255, 156, 113, 254, 164, 107, 254, 189, 162, 245, 182, 149, 216, 169, 109, 208, 154, 104, 181, 152, 111, 158, 146, 117, 139, 139, 118, 123, 137, 119, 112, 134, 123, 107, 131, 125, 106, 128, 125, 116, 130, 119, 140, 138, 111, 186, 156, 101, 242, 176, 84, 255, 151, 91, 255, 186, 80, 249, 174, 92, 247, 164, 106, 242, 156, 112, 213, 153, 123, 178, 151, 133};
    CuriePME.learn(subject102_4_4, 120, 4);

    static byte subject102_4_5[] PROGMEM = {153, 158, 170, 173, 166, 158, 224, 147, 164, 255, 168, 190, 255, 173, 180, 254, 183, 153, 235, 173, 146, 207, 163, 143, 188, 153, 135, 174, 147, 129, 158, 132, 130, 134, 128, 117, 118, 118, 109, 114, 116, 121, 107, 127, 127, 105, 123, 124, 128, 113, 125, 153, 130, 128, 169, 163, 100, 206, 145, 101, 255, 171, 117, 254, 172, 129, 255, 179, 127, 254, 196, 87, 214, 190, 85, 179, 163, 106, 186, 148, 116, 194, 134, 134, 170, 132, 138, 142, 130, 128, 113, 125, 124, 94, 136, 128, 100, 133, 134, 118, 130, 134, 138, 138, 139, 157, 146, 130, 195, 160, 121, 214, 149, 153, 248, 161, 172, 255, 168, 181};
    CuriePME.learn(subject102_4_5, 120, 4);

    static byte subject102_4_6[] PROGMEM = {193, 149, 123, 211, 155, 148, 254, 158, 131, 254, 190, 75, 255, 165, 90, 246, 149, 112, 216, 142, 101, 206, 154, 107, 181, 143, 113, 163, 133, 131, 139, 130, 142, 122, 127, 141, 111, 127, 135, 112, 136, 133, 116, 118, 134, 121, 113, 139, 140, 134, 147, 177, 165, 150, 203, 142, 138, 252, 156, 153, 254, 159, 187, 255, 163, 182, 254, 166, 151, 241, 157, 139, 207, 151, 128, 170, 145, 121, 148, 138, 114, 136, 133, 121, 126, 123, 128, 114, 115, 126, 110, 124, 123, 111, 126, 120, 128, 119, 120, 151, 127, 134, 189, 126, 138, 233, 159, 112, 255, 177, 74, 254, 188, 78, 255, 197, 78, 237, 189, 93};
    CuriePME.learn(subject102_4_6, 120, 4);
  */
  // Serial.println(freeMemory());

  blePeripheral.setLocalName("Activity");
  blePeripheral.setAdvertisedServiceUuid(fitnessService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(fitnessService);
  blePeripheral.addAttribute(fitnessTypeChar);

  // assign event handlers for connected, disconnected to peripheral
  fitnessTypeChar.setValue(20);

  // advertise the service
  blePeripheral.begin();

  Serial.println("Bluetooth device active, waiting for connections...");

}



void loop ()
{
  blePeripheral.poll();
  BLECentral central = blePeripheral.central();

  byte vector[120];
  unsigned int category;
  
  if (central) {
    Serial.print(F("Connected to central: "));
    Serial.println(central.address());

    while (central.connected()) {
      readVectorFromIMU(vector);
      category = CuriePME.classify(vector, 120);

      switch (category) {
        case 1:
          Serial.println(F("Still"));
          break;
        case 3:
          Serial.println(F("Walk"));
          break;
        case 4:
          Serial.println(F("Run"));
          break;
        default:
          Serial.println(F("Unknown"));
      }

      char bigstring[370];
      bigstring[0] = 0;
      int i;
      for (i = 0; i < 120; i++) {
        Serial.print(vector[i]);
        Serial.print(", ");
      }
      Serial.println();

//      if ((category == 1 || category == 3 || category == 4)) {
        fitnessTypeChar.setValue((byte) category);
        Serial.println(fitnessTypeChar.value());
  //    }

    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
  //  match();
}

/*void match()
{


  while (1) {

    BLEDevice central = BLE.central();

    if (central) {
      Serial.print(F("Connected to central: "));
      Serial.println(central.address());

      while (central.connected()) {
        byte vector[120];
        unsigned int category;
        readVectorFromIMU(vector);

        category = CuriePME.classify(vector, 120);

        switch (category) {
          case 1:
            Serial.println(F("Still"));
            break;
          case 3:
            Serial.println(F("Walk"));
            break;
          case 4:
            Serial.println(F("Run"));
            break;
          default:
            Serial.println(F("Unknown"));
        }
        //    Serial.println(freeMemory());

        char bigstring[370];
          bigstring[0] = 0;
          int i;
          for (i = 0; i < 120; i++) {
          Serial.print(vector[i]);
          Serial.print(", ");
          }
          Serial.println();
        
        if ((category == 1 || category == 3 || category == 4)) {
          fitnessTypeChar.setValue(category);
        }

      }
      Serial.print(F("Disconnected from central: "));
      Serial.println(central.address());
    }

  }
}
*/
void undersample(byte samples[], int numSamples, byte vector[])
{
  int i, j;
  for (i = 0; i < 120; i++) {
    j = i * 2;
    vector[i] = samples[j];
  }
}

void readVectorFromIMU(byte vector[])
{
  byte accel[250];
  int raw[3];

  unsigned int samples = 0;
  unsigned int i = 0;

  while (samples<250) {
    if (CurieIMU.dataReady()) {

      CurieIMU.readAccelerometer(raw[0], raw[1], raw[2]);

      /* Map raw values to 0-255 */
      accel[i] = (byte) map(raw[0], IMULow, IMUHigh, 0, 255);
      // accel[i + 1] = (byte) map(raw[1], IMULow, IMUHigh, 0, 255);
      // accel[i + 2] = (byte) map(raw[2], IMULow, IMUHigh, 0, 255);

      i += 1;
      ++samples;

      /* If there's not enough room left in the buffers
        for the next read, then we're done
        if (i + 3 > sensorBufSize) {
        break;
        }*/
    }
  }
  undersample(accel, samples, vector);
}
