/* YourDuino.com Example Software Sketch
 TEST LCD Display with Pushbuttons
 Based on code by Mark Bramwell and debouncing by peterstrobl
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ICMPPing.h>

byte destIp[] = { 192 , 168 , 1 , 1 } ; //192.168.1.5 Laptop
// prepare for tcp connection
byte mac[] = { 0x00, 0x08, 0xDC, 0x00, 0x00, 0x09 } ;
// prepare for the ping
SOCKET socket = 0 ;
int pingTimeout = 4 ;
char strLineResult[256] ;

String menu[2] = {"Ip Config","Ping Address"};

/*-----( Declare objects )-----*/
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //These are the pins used on this shield

/*-----( Declare Constants )-----*/
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

/*-----( Declare Variables )-----*/
int lcd_key       = 0;
int adc_key_in    = 0;
int adc_key_prev  = 0;

static int menu_level = 0;//menu level 0=start ML
static int level_val = 0;//Level Value 0-1
static int ip_array_val = 0; // ip array value 0-3


void setup()   /*----( SETUP: RUNS ONCE )----*/
{  
  lcd.begin(16, 2);              // start the lcd object
  
  //lcd.setCursor(0,0);
  //lcd.print("Main Menu ");
  
}/*--(end setup )---*/

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{ 
  /*
  //Menu and ip array values
  lcd.setCursor(5,0);
  lcd.print("ML");
  lcd.print(menu_level);
  
  lcd.setCursor(8,0);
  lcd.print("LV");
  lcd.print(level_val);
  
  lcd.setCursor(11,0);
  lcd.print("IPAV");
  lcd.print(ip_array_val);
  delay(100);
  lcd.setCursor(0,2);
  //lcd.print(menu[level_val]);
  */
  
  //lcd.setCursor(7,1);            // move cursor to second line "1" and 7 spaces over
  //lcd.print(millis()/1000);      // display seconds elapsed since power-up

  adc_key_prev = lcd_key;       // Looking for changes
  lcd_key = read_LCD_buttons();  // read the buttons
  
  //Main Menu

  if(menu_level == 0) // Opening Screen
  {
    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);
    lcd.print("Menu");
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(menu[level_val]);
    
    /*
        while(read_LCD_buttons()==btnNONE)
      {
       read_LCD_buttons(); 
      }
    */
    lcd.setCursor(11,1);            // move to the begining of the second line

    switch (lcd_key)               // depending on which button was pushed, we perform an action
    {
    case btnUP:
     level_val--;
      { 
         if (level_val<0)
          {
            level_val = 1;
          }
          break;
      }
    case btnDOWN:
    
      level_val++;
      {
        if (level_val>1)
          {
            level_val = 0;
          }
          break;
      }
    case btnSELECT:
        if(level_val==1)
          {
            menu_level=2;
          }
        else
          {
            menu_level=1;
            break;
          }
    case btnNONE:
      {
        break;
      }
    }/* --(end switch )-- */
  }// End Else
  
  
  //ip address meunu
  if (menu_level==1) //if the ip address is selected
  {
    
    lcd.setCursor(0,0);
    lcd.print("SetIp");

    adc_key_prev = lcd_key ;       // Looking for changes
    lcd_key = read_LCD_buttons();  // read the buttons
    
    lcd.setCursor(0,1);
    lcd.print("                ");
    
    //lcd.print(adc_key_in);
    lcd.setCursor(0,1);
    lcd.print(destIp[0]);
    lcd.print(".");
    lcd.print(destIp[1]);
    lcd.print(".");
    lcd.print(destIp[2]);
    lcd.print(".");
    lcd.print(destIp[3]);
    
    lcd.setCursor(0,1);            // move to the begining of the second line
    
    
    //Switch for Ip address change
    switch (lcd_key)               // depending on which button was pushed, we perform an action
    {
      case btnRIGHT:
        {
          if(++ip_array_val > 3)
          {
            ip_array_val=0;
          }
          break;
        }  
    case btnLEFT:
      {
        menu_level=0;
        break;
      }
    case btnUP:
      {
      destIp[ip_array_val]++;
      
      break;
      }
    case btnDOWN:
      {
        destIp[ip_array_val]--;

        break;
      }
    case btnSELECT:
      {
        //SAVE IP ADDRESS HERE
         lcd.setCursor(0,1);
         lcd.print("                ");
         lcd.setCursor(0,1);
         delay(1000);
         lcd.print("IP Saved!");
         delay(1000);
         menu_level=0;
         break;
      } 
    case btnNONE:
      {
        break;
      }
    }/* --(end switch )-- */
  }//End if ip address menu
  
  if (menu_level==2)
    {
        
        //clear both screens
        lcd.setCursor(0,0);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("                ");
        lcd.setCursor(0,0);
        lcd.print( "Initializing... " ) ;
        delay(1000);
        if ( !Ethernet.begin( mac ) )
        {
          lcd.setCursor(0,0);
          lcd.print("            "); 
          lcd.println( "FAILED" ) ;
          while ( true ) ;
        }
        
        //clear both screens
        lcd.setCursor(0,0);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("                ");
        
        //Display ip addresses
        lcd.setCursor(0,0);
        lcd.print( "Local IP : " ) ;
        lcd.setCursor(0,1);
        lcd.print( Ethernet.localIP() ) ;
        delay(3000);
        
        //clear both screens
        lcd.setCursor(0,0);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("                ");
        
        lcd.setCursor(0,0);
        lcd.print( "Remote IP : " ) ;
        lcd.setCursor(0,1);
        lcd.print( destIp[0] ) ;
        lcd.print( "." ) ;
        lcd.print( destIp[1] ) ;
        lcd.print( "." ) ;
        lcd.print( destIp[2] ) ;
        lcd.print( "." ) ;
        lcd.print( destIp[3] ) ;
        delay(3000);

        //clear both screens
        lcd.setCursor(0,0);
        lcd.print("                ");
        lcd.setCursor(0,1);
        lcd.print("                ");
      while(read_LCD_buttons() != btnLEFT)
        for(int i=0;i<1000;i++)
        {
          // build a ping object with the defined socket handler
          ICMPPing ping( socket ) ;
          
          // try to ping the destination ip of the google.com
          ping( pingTimeout , destIp , strLineResult ) ;  
          
          // the ping result will store into the string line result
          // display it on the console terminal
          Serial.println( strLineResult );
          lcd.setCursor(0,0);
          lcd.print("                ");
          lcd.setCursor(0,1);
          lcd.print("                ");
          lcd.setCursor(0,0);
          lcd.print("Ip ");
          lcd.print( destIp[0] ) ;
          lcd.print( "." ) ;
          lcd.print( destIp[1] ) ;
          lcd.print( "." ) ;
          lcd.print( destIp[2] ) ;
          lcd.print( "." ) ;
          lcd.print( destIp[3] ) ;
          lcd.println() ;
          lcd.setCursor(0,1);
          lcd.print(strLineResult);  

          delay( 1000 ) ;//dely between pings
        }
    }
    
  delay (5);
  
 /* lcd.setCursor(0,0);
  lcd.print("                ");//Clear Second Row
  lcd.setCursor(0,1);
  lcd.print("                ");//Clear Second Row
  */
}/* --(end main loop )-- */




/*-----( Declare User-written Functions )-----*/

int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor 
  delay(50); //switch debounce delay. Increase this delay if incorrect switch selections are returned.
  int k = (analogRead(0) - adc_key_in); //gives the button a slight range to allow for a little contact resistance noise
  if (5 < abs(k)) return btnNONE;  // double checks the keypress. If the two readings are not equal +/-k value after debounce delay, it tries again.
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (adc_key_in < 50)   return btnRIGHT;  
  if (adc_key_in < 195)  return btnUP; 
  if (adc_key_in < 380)  return btnDOWN; 
  if (adc_key_in < 555)  return btnLEFT; 
  if (adc_key_in < 790)  return btnSELECT;   
  return btnNONE;  // when all others fail, return this...
}

