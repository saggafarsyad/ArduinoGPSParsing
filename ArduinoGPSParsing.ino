// link between the computer and the SoftSerial Shield
//at 9600 bps 8-N-1
//Computer is connected to Hardware UART
//SoftSerial Shield is connected to the Software UART:D2&D3
 
#include <SoftwareSerial.h>
 
SoftwareSerial SoftSerial(2, 3);
unsigned char buffer[64];                   // buffer array for data receive over serial port
int count = 0;                                // counter for buffer array
int separatorIndices[13];
int separatorCount=0;
unsigned char header[7] = "$GPRMC";
int no = 0;

void setup()
{
    SoftSerial.begin(9600);                 // the SoftSerial baud rate
    Serial.begin(9600);                     // the Serial port of Arduino baud rate.
}
 
void loop()
{
  if (SoftSerial.available() > 0) {
    byte tmp = SoftSerial.read();
    if (tmp == 13) {
      // Process line   
      // Check for header, buffer[0] is 10 (Line feed) so should start from buffer[1]
      byte match = 0;
      for (int i = 1; i < 7; i++) {
        if (buffer[i] == header[i-1]) match++;
      }
      // If header is GPRMC, get separator indexes
      if (match == 6) {
        // Get separator indices
        for (int i = 0; i < count; i++) {
          if (buffer[i] == 44) {
            separatorIndices[separatorCount] = i;
            separatorCount++;
          }
        }
        // Print No of result
        Serial.print(no); Serial.print(",");        
        // Print status
        printPart(1); Serial.print(",");
        // Print latitude
        printPart(2); Serial.print(",");
        // Print longitude
        printPart(4); Serial.print(",");
        // Print buffer usage
        Serial.println(count); 
        no++;
      }
      // Reset buffer and separator      
      resetBuffer();
      resetSeparatorIndices();
    } else {
      buffer[count] = tmp;
      count++;
    }
  }
}
 
void resetBuffer() {
  for (int i=0; i<count;i++) {
    buffer[i] = NULL;
  }
  
  count = 0;
}

void resetSeparatorIndices() {
  for (int i=0; i<13; i++) {
    separatorIndices[i] = NULL;
  }

  separatorCount = 0;
}

void printPart(byte part) {
  int start = separatorIndices[part];
  int end = separatorIndices[part+1] - 1;
  
  for (int i = start; i < end; i++) {
    Serial.write(buffer[i + 1]);
  }
}

