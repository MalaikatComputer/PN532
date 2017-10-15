/**************************************************************************/
/*! 
    @mail     malaikatcomputerx@gmail.com
    
    This library works with the Adafruit NFC breakout 
    ----> https://www.adafruit.com/products/364
    configuration pin
  ========================================================================  
    PN532   ||   ARDUINO UNO  ||  OUTPUT PIN  ||  KETERANGAN  ||
  ========================================================================  
    IRQ     ||        2       ||      4       ||  BUZER
    RST     ||        3       ||
    SDA     ||       A4       ||
    SCL     ||       A5       ||
    VCC     ||       VCC      ||
    GND     ||       GND      ||
  =========================================================================
*/
/**************************************************************************/
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield
#define Led   (4)  // Door Lock
Adafruit_NFCShield_I2C nfc(IRQ, RESET);

void setup()
{
  Serial.begin(115200);
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) 
  {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("WELCOME TO SMART HOUSE"); //Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print(' '); Serial.println(' ');
  Serial.print("Temukan Beragam Kemudahan Disini"); //Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print(' '); Serial.println(' ');
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();
    
  Serial.println("please input your ID card...");
}

void loop() {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) 
    {
      String hex_value = "";
      for (uint8_t i=0; i < uidLength; i++) 
        {
          hex_value += (String)uid[i];
        }
      Serial.println("Number card = "+hex_value);
      
 ========== Output Analog =====================================================================================     
  if(hex_value == "18120670213") 
        {
          Serial.println("Akses Berhasil");
          analogWrite(A0, 255);
          delay(6000); //wait 5 seconds
          analogWrite(A0, 0);       
        }
 ========== Output Digital ====================================================================================
      else if(hex_value == "197228214101") 
        {
          Serial.println("Akses Gagal");
          digitalWrite(5, HIGH);//turn on
          delay(500); //wait 5 seconds
          digitalWrite(5, LOW);//      
        }
    }
  else
    {
      Serial.println("Waiting for a card...");
    }
}
