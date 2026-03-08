#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "chewyFont36.h"
#include "chewyFont.h"
#include "FreeMonoBoldOblique96pt7b.h"
#include "FreeSans18pt7b.h"
#include "icons.h"

//new fonts
//#include "FreeMonoBoldOblique12pt7b.h"
//#include "FreeMonoBoldOblique9pt7b.h"

#define monoB &Open_Sans_Hebrew_Condensed_Extra_Bold_72
#define sansT &Free_Sans_18pt_7b
#define chewy &Chewy_Regular_16

//my colors
#define col1 0x4B1   // game field
#define col2 0xFE00  //score
#define col3 0xE8EC  //pong #2
#define col4 0x3A96  //player paddle
#define col5 0xE8EC  // ball
#define pcol 0xF800 //player paddle 2


class Buttons{
    private:
    int reading;
    int press;
    int lastBtnState = HIGH;
    unsigned long debounceDelay = 40;
    unsigned long lastDebounceTime =0;
    int btnState = HIGH;
    int pin_no;
public:
Buttons(int pin_No){
    pin_no = pin_No;
    pinMode(pin_No, INPUT_PULLUP);
}
bool pushed(){
    reading = digitalRead(pin_no);
    if(reading != lastBtnState){
        lastDebounceTime = millis();
    }
    if((millis() -lastDebounceTime)> debounceDelay){
        if(reading != btnState){
            btnState = reading;
            if(btnState == LOW){
                lastBtnState = btnState;
                return true;
            }

        }



    }

    lastBtnState = reading;
    return false;
}
bool pressed(){
    press = digitalRead(pin_no);
    if(press == LOW){

        return true;
    }
    return false;

}

};

//buttons
#define up 33
const int down = 32;
#define left 34
#define right 35
//#define A 26
//#define B 27
//#define select 25
int upState =0;
int downState= 0;

Buttons leftBtn(34);
Buttons rightBtn(35);
Buttons upBtn(33);
Buttons downBtn(32);
Buttons selectBtn(25);
Buttons Abtn(26);
Buttons Bbtn(27);

//
//bool mySetup, hiScore, about;

TFT_eSPI tft = TFT_eSPI();//constructor initialization

//function declarations
void display_title();
void display_menu();
void move_cursor();
void redraw_cursor(int x, int y);
void redraw_rect(int x, int y, int w, int h);
void redraw_Frect(int x, int y, int w, int h);
void start_game();
void setupMenu();
void HiScoreMenu();
void AboutMenu();

//frame timing
int interval = 20;
unsigned long elapsed;
//button timing
bool lastDownState = HIGH;
unsigned long lastDownDebounce = 0;
const unsigned long debounceDelay = 40;



//cursor variables
int curX = 120;
int curY = 88;
int curW = 80;
int curH = 18;

//up button timing
int lastUpState = HIGH;
int btnState;
unsigned long duration = 40;
unsigned long lastPress = 0;

//int oldX;
//int oldY;

//main menu
int selection = 0;

//gameplay variables
//frame
 int frameX =10, frameY =30, frameW = 300, frameH =200;
 //ball
int ballX = frameW/2, ballY =frameH/2, ballW = 5, ballH =5;
//ball velocity
int velX = 3, velY =4;
int oldY = frameW/2;
int oldX = frameH/2;
//playerX
int pX =12, pY = 74, pH =50, pW =6;

int pVel = 5;
int oldpX, oldpY;

int player_score, cpu_score;
//cpu
int cX = 302, cY =146, cH =50, cW =6;
int CvelY;
void setup(){
  Serial.begin(115200); //initialize serial communication
  Serial.println("Let's do this!!");

    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);
    //define buttons as inputs
    //pinMode(up, INPUT_PULLUP);
    pinMode(down, INPUT_PULLUP);
    //lastUpState = digitalRead(up);
   

}

void loop(){
    if(millis() - elapsed >= interval){
      elapsed = millis();

      
    switch (selection)
    {
    case 0:
      display_title();
      display_menu();
      move_cursor();
      break;
    case 1:
        start_game();
    break;
    case 2:
    tft.fillScreen(TFT_BLACK);
    selection= 1;
    break;
    case 3:
    tft.fillScreen(TFT_BLACK);
    selection =0;
    break;
    case 4:
    tft.fillScreen(TFT_BLACK);
    selection =5;
    break;

    case 5:
    setupMenu();
    break;

    case 6:
    tft.fillScreen(TFT_BLACK);
    selection =7;

    case 7:
    HiScoreMenu();
    break;
    
    case 8:
    tft.fillScreen(TFT_BLACK);
    selection =9;

    case 9:
    AboutMenu();
    break;

    case 10:
    tft.fillScreen(TFT_BLACK);
    selection =0;
    break;

    default:
        break;
    }



    }





}

void display_title(){

    tft.setFreeFont(monoB);
    tft.setTextColor(col1);
    tft.drawString("P", 90, 4 );
    tft.setTextColor(col2);
    tft.drawString("o", 125, 3 );
    tft.setTextColor(col3);
    tft.drawString("n", 160, 3 );
    tft.setTextColor(col4);
    tft.drawString("g", 195, 3 );




}
void display_menu(){

    tft.setFreeFont(&Chewy_Regular_16);
    tft.setTextColor(TFT_GREEN);
    //my cursor
    //tft.fillCircle( curX, curY , 4, TFT_BLUE);

    tft.drawRect(curX,curY, curW, curH, TFT_GREEN);

    tft.drawString("    Start     ",110, 90 );
    tft.drawString("    Setup     ",110, 110 );
    tft.drawString("    Hi-Score ",110, 130 );
    tft.drawString("    About     ",110, 150 );


}
void start_game(){
    //menu
    tft.setFreeFont(chewy);
    tft.setTextColor(col1);
    tft.drawString("P", 120, 2 );
    tft.setTextColor(col2);
    tft.drawString("o", 130, 2 );
    tft.setTextColor(col3);
    tft.drawString("n", 140, 2 );
    tft.setTextColor(col4);
    tft.drawString("g", 150, 2 );
   
    //tft.fillScreen(TFT_BLACK);
    //draw game frame
    tft.drawRect(frameX, frameY, frameW, frameH, col1);
    //draw mid-section
    for(int i =0; i< 40; i++){
        if(i%2 == 0){
            tft.fillRect(frameW/2, frameY + i*5, 4, 5, col1);
        }
        

    }
    //pong ball
    
    ballX+= velX;  
    ballY+= velY;

    Serial.print("X pos: ");
    Serial.println(ballX);
    Serial.print("Y pos: ");
    Serial.println(ballY);

    tft.fillRect(ballX, ballY, ballW, ballH, col5);
    //redraw function 
    redraw_Frect(ballX, ballY, ballW, ballH);

    //boundary conditions with frame
    if(ballX<= frameX ){
        velX*=-1;
        cpu_score+=1;
    }
    if(ballX+ ballW> frameW +frameX){
        velX*=-1;
        player_score+=1;
    }

    if(ballY<= frameY+5 || ballY+ballH+5> frameY + frameH){
        velY*=-1;
    }

    //player paddle
     tft.fillRect(oldpX, oldpY, pW, pH, TFT_BLACK);
    
   
    if(upBtn.pressed() && pY> frameY+4){
        
        
        pY -= pVel;

    }
 
    else if(downBtn.pressed() && pY + pH < frameY + frameH-5){
        Serial.print("playerY:");
        Serial.println(pY + pH);
        pY += pVel;


    }
    tft.fillRect(pX, pY, pW, pH, pcol);
  
    oldpX = pX;
    oldpY= pY;

    //collision between ball and player
    if(ballX < pX + pW +1 && ballY > pY -3 && ballY < pY + pH+3){ 
        velX*= -1;
    
    }
     

    //Score
    tft.setFreeFont(sansT);
    tft.setTextColor(col2, TFT_BLACK);
    tft.drawString(String(player_score), 100, 33);//player

    tft.drawString(String(cpu_score), 170, 33);//cpu
    
    //if(cpu_score > 10){ selection =3;}
    
   
    tft.fillRect(cX, cY, cW, cH, TFT_BLACK);
    //collision between ball and cpu
    if(ballX + ballW> cX  && ballY > cY -3 && ballY < cY + cH+3){ 
        velX*= -1;
    
    }
    //cpu boundaries
    if(cY <= frameY){
        cY = frameY;
    }
    else if(cY >= frameY + frameH){
        cY = frameY + frameH;

    }
    //cpu detects ball
    if(ballX > frameW/2 && ballY < cY){
        cY-=5;

    }
     if(ballX > frameW/2 && ballY > cY){

         cY+=5;

    }
    Serial.print("cpu velocity: ");
    Serial.println(CvelY);
    
    //cpu paddle
    tft.fillRect(cX, cY, cW, cH, col4);

}

void move_cursor() {
    bool reading = digitalRead(down);
    if(upBtn.pushed()){
        Serial.println("Up has been pressed!");
             if(curY == 148){
                //redraw_cursor(curX, curY);
                redraw_rect(curX, curY, curW, curH);
                curY = 128;
             }
              else if(curY == 128){
                //redraw_cursor(curX, curY);
                redraw_rect(curX, curY, curW, curH);
                curY = 108;
             }
              else if(curY == 108){
                //redraw_cursor(curX, curY);
                redraw_rect(curX, curY, curW, curH);
                curY = 88;
             }
              else if(curY == 88){
                //redraw_cursor(curX, curY);
                redraw_rect(curX, curY, curW, curH);
                curY = 108;
             }

    }
    if(selectBtn.pushed() && curY == 88){
        Serial.println("The game has started!!!");
        selection = 2;

    }
    else if(selectBtn.pressed() && curY == 108){
        Serial.println("Setup Selected!!!");
        selection = 4;
      

    }
     else if(selectBtn.pressed() && curY == 128){
        Serial.println("Hi-Score Selected!!!");
        selection = 6;
      

    }
     else if(selectBtn.pressed() && curY == 148){
        Serial.println("About Selected!!!");
        selection = 8;
       

    }


 

    /*
     //up button
    //if(millis() - lastPress >= duration){
    int upReading = digitalRead(up);
        
    if(upReading!= lastUpState){
        lastPress= millis();
    }
    
    
    if((millis()- lastPress)> duration){
      if ( upReading != btnState) {
      btnState = upReading;
          if(btnState == LOW){

             Serial.println("Up has been pressed!");
             if(curY == 155){
                redraw_cursor(curX, curY);
                curY = 135;
             }
              if(curY == 135){
                redraw_cursor(curX, curY);
                curY = 115;
             }
              if(curY == 115){
                redraw_cursor(curX, curY);
                curY = 195;
             }
              if(curY == 95){
                redraw_cursor(curX, curY);
                curY = 115;
             }
        }
        
    
    }}

    lastUpState = upReading;
    
    
    */
    
   
    
    
  
       


    
    
    // detect change
    if (reading != lastDownState) {
        lastDownDebounce = millis();
    }

    // check if stable long enough
    if ((millis() - lastDownDebounce) > debounceDelay) {

        static bool downStateStable = HIGH;

        // stable change → edge occurred
        if (reading != downStateStable) {
            downStateStable = reading;

            // JUST pressed (LOW)
            if (downStateStable == LOW) {
                // --- ONE PRESS ONLY ---
                if(curY == 88){
                    //redraw_cursor(curX, curY);
                    redraw_rect(curX, curY, curW, curH);
                    curY = 108;
                    Serial.println("down press → moved to 108");
                }
                else if(curY == 108){
                    //redraw_cursor(curX, curY);
                    redraw_rect(curX, curY, curW, curH);
                    curY = 128;
                    Serial.println("down press → moved to 128");
                }
                else if(curY == 128){
                    //redraw_cursor(curX, curY);
                    redraw_rect(curX, curY, curW, curH);
                    curY = 148;
                    Serial.println("down press → moved to 148");
                }
                else if(curY == 148){
                    //redraw_cursor(curX, curY);
                    redraw_rect(curX, curY, curW, curH);
                    curY = 128;
                    Serial.println("up - down");

                }
            }
        }
    }

    lastDownState = reading;
}
/*
void move_cursor(){

upState = digitalRead(up);
Serial.print("Cur Y: ");
Serial.println(curY);

downState = digitalRead(down);

//int curPos = 90;

if(downState == LOW && curY == 90 ){
    redraw_cursor(curX, curY);
    curY = 110;
    Serial.println("down button has been pressed");
}
else if(downState == LOW && curY == 110){
    redraw_cursor(curX, curY);
    curY = 130;
    Serial.println("down button has been pressed again");

}
else if(downState == LOW  && curY == 130){
    redraw_cursor(curX, curY);
    curY = 150;
    Serial.println("again and again the down button has been pressed");

}

}*/

void redraw_cursor(int x, int y){
    int oldY = curY;
    int oldX = curX;

     tft.fillCircle( oldX, oldY , 4, TFT_BLACK);

}
void redraw_rect(int x, int y, int w, int h){
    int oldY = y;
    int oldX = x;

    tft.drawRect(x,y, w, h, TFT_BLACK);



}
void redraw_Frect(int x, int y, int w, int h){
    

    tft.fillRect(oldX,oldY, w, h, TFT_BLACK);

    oldX =x;
    oldY = y;

}
void setupMenu(){


tft.drawRoundRect(16, 28, 290, 202, 10, 0xFFFF);
tft.setTextColor(0x4B1);
tft.setTextSize(1);
tft.setFreeFont(&FreeMonoBoldOblique12pt7b);
tft.drawString(" Button Mapping", 42, 46);
tft.drawBitmap(93, 167, epd_bitmap_image_play_hover_bits_R, 19, 20, 0xFFFF);
tft.drawBitmap(45, 168, epd_bitmap_image_play_hover_bits_L , 19, 20, 0xFFFF);
tft.drawBitmap(72, 147, epd_bitmap_image_play_hover_bits_U , 20, 19, 0xFFFF);
tft.drawBitmap(69, 190, epd_bitmap_image_play_hover_bits_D, 20, 19, 0xFFFF);
tft.drawBitmap(256, 160, epd_bitmap_image_off_hover_bits, 19, 20, 0xFFFF);
tft.drawBitmap(152, 169, epd_bitmap_select_btn_hvr, 16, 18, 0xFFFF);
tft.drawBitmap(221, 184, epd_bitmap_image_off_hover_bits, 19, 20, 0xFFFF);
tft.setTextColor(0xF206);
tft.setFreeFont(&FreeMonoBoldOblique9pt7b);
tft.drawString("Up", 69, 125);
tft.drawString("down", 56, 207);
tft.drawString("L", 30, 168);
tft.drawString("R", 116, 167);
tft.drawString("Select", 138, 153);
tft.drawString("A", 225, 162);
tft.drawString("B", 260, 141);
tft.drawBitmap(259, 43, epd_bitmap_menu_settings_gear, 32, 32, 0x4B1);
tft.drawBitmap(24, 44, epd_bitmap_menu_tools, 28, 32, 0x4B1);
 if(Bbtn.pushed()){
        Serial.println("Back to  main menu");
        selection = 10;
    //this returns the user to the game menu
    }

}
void HiScoreMenu(){


tft.drawRoundRect(13, 11, 298, 218, 10, 0x2C3);
tft.setTextColor(0xF206);
tft.setTextSize(1);
tft.setFreeFont(&FreeMonoBoldOblique12pt7b);
tft.drawString("Hi-Score", 96, 21);
tft.drawBitmap(256, 19, epd_bitmap_hour_glass_75, 22, 32, 0xF206);
 if(Bbtn.pushed()){
        Serial.println("Back to  main menu");
        selection = 10;

    }


}
void AboutMenu(){

 

tft.drawRoundRect(20, 20, 289, 210, 10, 0x4B1);
tft.setTextColor(0xE8EC);
tft.setTextSize(1);
tft.setFreeFont(&FreeMonoBoldOblique12pt7b);
tft.drawString("About", 108, 28);
tft.setTextColor(0xFFFF);
tft.setFreeFont(&FreeMonoBoldOblique9pt7b);
tft.drawString("This is a quick game of", 40, 62);
tft.drawString("pong. Play against the A.I ", 23, 75);
tft.drawString("to test your skill. ;)", 24, 94);
tft.drawBitmap(192, 27, epd_bitmap_file, 24, 32, 0xE8EC);

 if(Bbtn.pushed()){
        Serial.println("Back to  main menu");
        selection = 10;

    }



}






