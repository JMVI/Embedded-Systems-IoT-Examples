/* -----------------------------------------------------------------------------
 * Copyright (C) 2022 Jaime M. Villegas I. <jmvillegasi96@gmail.com>
 * -----------------------------------------------------------------------------
 * Filename      : esp32_ap_mode.ino
 * Description   : ESP32 board setup as WiFi access point.
 * Version       : 01.00
 * Revision      : 00
 * Last modified : November, 2022
 * -----------------------------------------------------------------------------
 */

//----------------------------------------------------------------------------//
//                                Header files                                //
//----------------------------------------------------------------------------//

#include <WiFi.h>      // WiFi library

//----------------------------------------------------------------------------//
//                                  Macros                                    //
//----------------------------------------------------------------------------//

/************************* Access point parameters ****************************/

#define ESP32_SSID     "ESP32-AP"         // Wireless access point SSID
#define ESP32_PASSWORD "TestEsp32"        // Access point password


/******************************* Web server ***********************************/

#define SERVER_PORT 80                    // HTTP port

/************************ Communication parameters ****************************/

#define BAUD_RATE 115200                  // Serial port baudrate

//----------------------------------------------------------------------------//
//                            General definitions                             //
//----------------------------------------------------------------------------//

/******************************* Web server ***********************************/

WiFiServer APserver(SERVER_PORT);         // WiFi server handler on HTTP port
String header;                            // HTTP request

//----------------------------------------------------------------------------//
//                              Local functions                               //
//----------------------------------------------------------------------------//

/**
@brief  Function description
@param  List of parameters
@retval Return value
@note   Additional information
*/

//----------------------------------------------------------------------------//
//                                Sketch setup                                //
//----------------------------------------------------------------------------//

/**
@brief Access point setup operation
@note  Serial port and WiFi access point initialization  
*/
void setup()
{
  // Initialize serial port  
  Serial.begin(BAUD_RATE);
  
  // Start access point
  Serial.print("Setting up Access Point…");
  WiFi.softAP(ESP32_SSID, ESP32_PASSWORD);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  APserver.begin();
}

//----------------------------------------------------------------------------//
//                                 Main loop                                  //
//----------------------------------------------------------------------------//

/**
@brief  Main program for ESP32 in Access Point mode
@note   None
*/
void loop()
{
  // Checks for incoming clients
  WiFiClient client = APserver.available();

  // Received data
  String currentLine = "";
  char rxChar = ' ';

  if(client)
  {
    Serial.println("New Client.");

    while(client.connected())
    {
      if(client.available())
      {
        // Reads received character        
        rxChar = client.read();
        Serial.write(rxChar);
        header += rxChar;

        if(rxChar == '\n')
        {
          if (currentLine.length() == 0) 
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println("</style></head>");
            client.println("<body><h1>ESP32 Web Server Example for Access Point mode</h1>");
            client.println("</body></html>");
            
            // Blank line marks end of HTTP response
            client.println();

            break;          
          }
          else
          {
            // Clear buffer to read a new line
            currentLine = "";
          }
          
        }
        else if (rxChar != '\r')
        {
          currentLine += rxChar;
        }
      }
    }

    // Clear the header buffer
    header = "";
    
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }  
}

