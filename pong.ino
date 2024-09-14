
#include <Arduino.h>
#include <U8g2lib.h>

#include <Wire.h> // library requires for IIC communication

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);
int x = 2, y = 2, xDir = 1, yDir = 1;
int compPaddleX = 10, compPaddleY = 25, userPaddleX = 116, userPaddleY = 25;
int compPaddleWidth = 2, compPaddleLength = 15;
int compPaddleYDir = 1;
int controller = A0, controller2 = A1;
int buzzer = 5;
int compScore = 0, userScore = 0;

void drawBigPixel(int xpos, int ypos)
{
  u8g2.drawPixel(xpos,ypos);
  u8g2.drawPixel(xpos+1,ypos+1);
  u8g2.drawPixel(xpos,ypos+1);
  u8g2.drawPixel(xpos+1,ypos);
}



void setup(void) {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(5,OUTPUT);
  u8g2.begin();
  u8g2.setFont(u8g2_font_tom_thumb_4x6_t_all);
}

void loop(void) {

//  u8g2.firstPage();
  //do {
    
u8g2.clearBuffer();
 u8g2.setFont(u8g2_font_tom_thumb_4x6_t_all);
/* debug code
*/
   // u8g2.setFont(u8g2_font_unifont_t_symbols);
   // u8g2.drawGlyph(x,y,0x2603);

    u8g2.setCursor(100, 32); //sets cursor to middle of screen

//u8g2.print("y: "); //prints "RPM: "
//u8g2.setCursor(118, 38); //sets cursor to middle of screen
//u8g2.print(userPaddleY);// rpm from button press 
    drawBigPixel(x,y);
    u8g2.drawBox(compPaddleX,compPaddleY,compPaddleWidth,compPaddleLength);
    u8g2.drawBox(userPaddleX,userPaddleY,compPaddleWidth,compPaddleLength);

u8g2.setCursor(64, 8); //sets cursor to middle of screen
u8g2.print("|"); //prints "RPM: "
u8g2.setCursor(70, 8); //sets cursor to middle of screen
u8g2.print(userScore);// rpm from button press
u8g2.setCursor(58, 8); //sets cursor to middle of screen
u8g2.print(compScore);// rpm from button press


u8g2.setCursor(116, 32); //sets cursor to middle of screen

 // } while ( u8g2.nextPage() );

if (y == 60 || y == 0)
{
  yDir *= -1; 
    tone(5, 500, 25);
    delay(25);
    noTone(5);
}

int value = analogRead(controller);
int value2 = analogRead(controller2);
Serial.print("value: ");
Serial.print(value2);



//u8g2.setCursor(101, 38); //sets cursor to middle of screen
//u8g2.print(value2);// rpm from button press

if(value2 < 400)
  {
  if(!(compPaddleY-3 < 0))
  compPaddleY -= 3;
  }
  else if ((value2 > 550))
  {
    if(!(compPaddleY+3+compPaddleLength > 64))
    compPaddleY +=3;
  }


if(value < 500)
  {
  if(!(userPaddleY-3 < 0))
  userPaddleY -= 3;
  }
  else if ((value > 550))
  {
    if(!(userPaddleY+3+compPaddleLength > 64))
    {
    userPaddleY +=3;
    }
  }



if ( ((x == userPaddleX-1) && (y > userPaddleY && y < userPaddleY+compPaddleLength ) || ((x == compPaddleX+1) && (y > compPaddleY && y < compPaddleY+compPaddleLength)  ) ) )//&& y >= userPaddleY-15 )))
{
  xDir *= -1;
  tone(5, 550, 25);
  delay(25);
  noTone(5);
}


if (x < 0)
{
  userScore++;
  x = 64;
  y = 32;
}
if (x > 128)
{
  compScore++;
  x = 64;
  y = 32;
}


x += xDir;
y += yDir;

//if (compPaddleY == 50 || compPaddleY == 0)
//{
//  compPaddleYDir *= -1;
//}
//compPaddleY += compPaddleYDir;

u8g2.sendBuffer();




}



