
#include <Arduino.h>
#include <U8g2lib.h>

#include <Wire.h> // library requires for IIC communication

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);
int x = 2, y = 2, xDir = 2, yDir = 2;
int compPaddleX = 10, compPaddleY = 25, userPaddleX = 116, userPaddleY = 25;
int compPaddleWidth = 2, compPaddleLength = 15, userPaddleLength = 15;
int compPaddleYDir = 1;
int controller = A0, controller2 = A1;
int buzzer = 5;
int compScore = 0, userScore = 0;
int controller1switch = 8;
int controller2switch = 9;
bool userReady = false, compReady = false;


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
  pinMode(8,INPUT);
  digitalWrite(8, HIGH);
  pinMode(9,INPUT);
  digitalWrite(9, HIGH);
  pinMode(5,OUTPUT);
  u8g2.begin();
  u8g2.setFont(u8g2_font_tom_thumb_4x6_t_all);

  

}

void loop(void) {



while(!(userReady && compReady))
  {
    u8g2.clearBuffer();
    if (!userReady){
      u8g2.setCursor(80,50);
      u8g2.print("P2 Not Ready");
    }
    else{
      u8g2.setCursor(80,50);
      u8g2.print("P2 Ready!");
    }
    if (!compReady){
      u8g2.setCursor(5,50);
      u8g2.print("P1 Not Ready");
    }
    else{
      u8g2.setCursor(5,50);
      u8g2.print("P1 Ready!");
    }
    u8g2.setCursor(45,35);
    u8g2.print("uno pong");



    u8g2.sendBuffer();

    if (digitalRead(8) == 0)
    {
      userReady = !userReady;
      delay(100);
    }
    if (digitalRead(9) == 0)
    {
      compReady = !compReady;
      delay(100);
    }

  }

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
    u8g2.drawBox(userPaddleX,userPaddleY,compPaddleWidth,userPaddleLength);

u8g2.setCursor(64, 8); //sets cursor to middle of screen
u8g2.print("|"); //prints "RPM: "
u8g2.setCursor(70, 8); //sets cursor to middle of screen
u8g2.print(userScore);// rpm from button press
u8g2.setCursor(58, 8); //sets cursor to middle of screen
u8g2.print(compScore);// rpm from button press


u8g2.setCursor(116, 32); //sets cursor to middle of screen

 // } while ( u8g2.nextPage() );

//check horizontal walls
if (y+yDir >= 60 || y+yDir <= 0)
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


// compPaddleY = y-7; FOR UNBEATABLE AI

//make boundaries for the paddles
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



if ( ((x == userPaddleX-1 || x == userPaddleX) && (y > userPaddleY && y < userPaddleY+userPaddleLength ) || ((x == compPaddleX+1 || x == compPaddleX) && (y > compPaddleY && y < compPaddleY+compPaddleLength)  ) ) )//&& y >= userPaddleY-15 )))
{
  xDir *= -1;
  tone(5, 550, 25);
  delay(25);
  noTone(5);
}

//calculate scoring
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

//check win
if(userScore == 7)
{
  u8g2.setCursor(35,20);
  u8g2.print("Player 2 Wins!");
}

if(compScore == 7)
{
  u8g2.setCursor(35,20);
  u8g2.print("Player 1 Wins!");
}

if(compScore > 7 || userScore > 7)
{
   userScore = 0;
  compScore = 0;
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



