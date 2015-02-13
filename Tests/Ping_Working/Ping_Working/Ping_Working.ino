//This is a test for git hub

#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>
#include <LiquidCrystal.h>


LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// prepare for tcp connection
byte mac[] = { 0x00, 0x08, 0xDC, 0x00, 0x00, 0x09 } ;

// prepare for the ping
SOCKET socket = 0 ;
int pingTimeout = 4 ;
byte destIp[] = { 192 , 168 , 1 , 1 } ; //192.168.1.5 Laptop
char strLineResult[256] ;

void setup() {
  
   // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  // the program start when the serial debug terminal connected
  Serial.begin( 9600 ) ;
  while ( !Serial ) ;

  // initialized the tcp connection, the ethernet shield
  // shall got the local ip from dhcp server
  Serial.print( "Initializing... " ) ;
  delay( 2000 ) ;
  if ( !Ethernet.begin( mac ) ) {
    Serial.println( "FAILED" ) ;
    while ( true ) ;
  }
  Serial.println( "OK" ) ;
  
  // validate by display the local ip on board
  Serial.print( "Local IP : " ) ;
  delay( 2000 ) ;
  Serial.println( Ethernet.localIP() ) ;
  
  // display the remote ip ( debug purpose )
  Serial.print( "Remote IP : " ) ;
  delay( 2000 ) ;
  Serial.print( destIp[0] ) ;
  Serial.print( "." ) ;
  Serial.print( destIp[1] ) ;
  Serial.print( "." ) ;
  Serial.print( destIp[2] ) ;
  Serial.print( "." ) ;
  Serial.print( destIp[3] ) ;
  Serial.println() ;
  
}

void loop() {
  
  // build a ping object with the defined socket handler
  ICMPPing ping( socket ) ;
  
  // try to ping the destination ip of the google.com
  ping( pingTimeout , destIp , strLineResult ) ;  
  
  // the ping result will store into the string line result
  // display it on the console terminal
  Serial.println( strLineResult ) ;
  lcd.clear();
  lcd.print(strLineResult);
  delay(1000);
  
  // ping every one second until die die... :-P
  delay( 1000 ) ;
  
}
