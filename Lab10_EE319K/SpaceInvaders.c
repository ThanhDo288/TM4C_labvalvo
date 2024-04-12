// SpaceInvaders.c
// Runs on TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the ECE319K Lab 10

// Last Modified: 1/2/2023 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// buttons connected to PE0-PE3
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// LED on PD1
// LED on PD0

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "../inc/ADC.h"
#include "Images.h"
#include "../inc/wave.h"
#include "Timer1.h"
#include "../inc/PLL.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds


typedef enum {dead, alive } status_t;
struct sprite{
	int32_t x;
	int32_t y;
	int32_t vx,vy; // toc do pixels/30Hz
	const unsigned short *image;
	const unsigned short *black;
	status_t life;
	uint32_t w;// width
	uint32_t h;// height
	uint32_t needDraw;// true if need to draw
	};
typedef struct sprite sprite_t;
sprite_t Enemy[18];
sprite_t Gun;
	#define MAX_BULLETS 100 // Số lượng đạn tối đa
sprite_t Bullets[MAX_BULLETS];
	int Flag;
	int Anyalive;
	
uint32_t ADCData; // 12bit ADC
uint32_t ADCstatus;
void GameInit(void) 
	{
		int i; 
		Flag=1;
		for( i=0; i<6;i++ ){
			Enemy[i].x=20*i;
			Enemy[i].y=10;
			Enemy[i].x=20*i;
			Enemy[i].vx=0;
			Enemy[i].vy=0;
			Enemy[i].image=SmallEnemy10pointA;
			Enemy[i].black= BlackEnemy;
			Enemy[i].life=alive;
			Enemy[i].w=16;
			Enemy[i].h=10;
			Enemy[i].needDraw=1;
			}
		Enemy[3].vy=1;// down
		Enemy[5].vx=1;// right
		Enemy[5].vy=1;// down
		for(i=7; i<18;i++){
			Enemy[i].life=dead;
			}
			
		//Gun.x=45;
		Gun.y=155;
		Gun.image=x32;
		Gun.w=50;
		Gun.h=45;
		Gun.life=alive;
		Gun.needDraw=1;
		Gun.black= black45x50;
		
		}
void Bullet_Init(void) {
    // Khởi tạo đối tượng đạn
	for (int i = 0; i < MAX_BULLETS; ++i) {
    //Bullet.x = Gun.x+48/2;  // Đặt vị trí ban đầu của đạn bằng vị trí của PlayerShip0/Gun
    Bullets[i].y = 110;  // Đặt vị trí ban đầu của đạn bằng vị trí của PlayerShip0/Gun
    Bullets[i].image = BulletImage; // Thay thế 'BulletImage' bằng tên hình ảnh đạn bạn đã định nghĩa
		Bullets[i].black=BlackBullet;
		Bullets[i].w = 4; 
    Bullets[i].h = 4;  // Chiều cao của đạn
		Bullets[i].vy=1;
    Bullets[i].life = dead;  // Khởi tạo trạng thái của đạn là 'dead'
    Bullets[i].needDraw = 1;  // Khởi tạo cờ để vẽ đạn là 0 (không cần vẽ)
}
    // Khởi tạo các giá trị khác của đạn nếu cần
}

void FireBullet(void) {
    if (Gun.life == alive) {
			for (int i = 0; i < MAX_BULLETS; ++i) {
        // Nếu PlayerShip0/Gun còn sống, cho phép bắn đạn
			if(Bullets[i].life==alive){
     // Bullet.x = Gun_x+46/2;  // Đặt vị trí của đạn bằng vị trí của PlayerShip0/Gun
			Bullets[i].needDraw=1;
					Anyalive=1;
					if(Bullets[i].y>140) Bullets[i].life=dead;
					else {
						if (Bullets[i].y<12) Bullets[i].life=dead;
						 else {
							if (Bullets[i].x<0)Bullets[i].life=dead;
						 else {
							if (Bullets[i].x>102) Bullets[i].life=dead;
							else {
								//Bullet.x+=Bullet.vx;
								Bullets[i].y-=Bullets[i].vy;
								}
									}
									}			
								}
    }
			}
			}
}

void GameMove(void)
{
		int i;
		//Anyalive=0;
		for(i=0;i<18;i++)
		{
			if(Enemy[i].life==alive)
				{
					Enemy[i].needDraw=1;
					Anyalive=1;
					if(Enemy[i].y>140) Enemy[i].life=dead;
					else {
						if (Enemy[i].y<10) Enemy[i].life=dead;
						 else {
							if (Enemy[i].x<0) Enemy[i].life=dead;
						 else {
							if (Enemy[i].x>102) Enemy[i].life=dead;
							else {
								Enemy[i].x+=Enemy[i].vx;
								Enemy[i].y+=Enemy[i].vy;
								}
									}
									}			
								}
				}
			}
	}
void GameDraw(void){
	int i;
	for(i=0; i<18;i++){ 
		if(Enemy[i].needDraw){
			if(Enemy[i].life==alive){
			ST7735_DrawBitmap(Enemy[i].x,Enemy[i].y,
			Enemy[i].image,Enemy[i].w, Enemy[i].h);		
		}else {
			ST7735_DrawBitmap(Enemy[i].x,Enemy[i].y,
			Enemy[i].black,Enemy[i].w, Enemy[i].h);
			}
		Enemy[i].needDraw=0;
		}
	}
	if(Gun.needDraw){
			if(Gun.life==alive){
			ST7735_DrawBitmap(Gun.x,Gun.y,
			Gun.image,Gun.w, Gun.h);		
		}
}
		for (int i = 0; i < MAX_BULLETS; ++i) {
	if(Bullets[i].needDraw){
			if(Bullets[i].life==alive){
			ST7735_DrawBitmap(Bullets[i].x,Bullets[i].y,
			Bullets[i].image,Bullets[i].w, Bullets[i].h);		
		}else {
			ST7735_DrawBitmap(Bullets[i].x,Bullets[i].y,
			Bullets[i].black,Bullets[i].w, Bullets[i].h);
			}
		Bullets[i].needDraw=0;
		}
	}
}
uint32_t Convert(uint32_t adc_data){
	return (uint32_t)(adc_data * (128-50) / 4096);
}

// Battleship move
void Gun_Move(void){
	int32_t temp = Gun.x;
	Gun.x = Convert(ADCData); 
	if (temp != Gun.x){
	ST7735_DrawBitmap(temp,Gun.y, Gun.black,Gun.w, Gun.h);		
	}
	ST7735_DrawBitmap(Gun.x,Gun.y, Gun.image,Gun.w, Gun.h);		
}


	int CheckCollision(sprite_t* bullet, sprite_t* enemy) {
    // Tính toán các biên của đối tượng đạn và đối tượng Enemy
    int bulletLeft = bullet->x;
    int bulletRight = bullet->x + bullet->w;
    int bulletTop = bullet->y;
    int bulletBottom = bullet->y + bullet->h;

    int enemyLeft = enemy->x;
    int enemyRight = enemy->x + enemy->w;
    int enemyTop = enemy->y;
    int enemyBottom = enemy->y + enemy->h;

    // Kiểm tra va chạm giữa đạn và Enemy
    if (bulletRight >= enemyLeft && bulletLeft <= enemyRight &&
        bulletBottom >= enemyTop && bulletTop <= enemyBottom) {
        return 1; // Trả về 1 nếu có va chạm
    }
    return 0; // Trả về 0 nếu không có va chạm
}
	void CheckBulletEnemyCollision() {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (Bullets[i].life == alive) {
            for (int j = 0; j < 18; ++j) {
                if (Enemy[j].life == alive) {
                    // Kiểm tra va chạm giữa đạn và Enemy
                    if (CheckCollision(&Bullets[i], &Enemy[j])) {
                        // Xử lý khi đạn va chạm với Enemy ở đây
                        // Ví dụ: Gán trạng thái của đạn và Enemy là 'dead'
                        Bullets[i].life = dead;
                        Enemy[j].life = dead;
                        // Hoặc thực hiện các hành động khác khi va chạm xảy ra
                    }
                }
            }
        }
    }
}
	void GameTask(void)
	{
		//30hz
		//buttons, plays sound. slidepot
		// Cập nhật giá trị từ ADC vào biến analogValue (giả sử đã đọc giá trị ADC ở nơi khác)
		Gun_Move();
		Flag=1;  
		GameMove();
		FireBullet();
		CheckBulletEnemyCollision();
		
	}

void EdgeCounter_Init(void){       
  volatile int delay;
  SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTF_DIR_R &= ~0x10;    // (c) make PF4 in (built-in button)
  GPIO_PORTF_DEN_R |= 0x10;     //     enable digital I/O on PF4
  GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4
  GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R &= ~0x10;    //     PF4 falling edge event
  GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
//  EnableInterrupts();          // (i) Enable global Interrupt flag (I)
}

void Timer1A_Handler(void){ // can be used to perform tasks in background
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
   // execute user task
			ADCData = ADC_In();	
			GameTask();
			GPIO_PORTF_DATA_R ^= 0x04;

	
}
int count=0;
void GPIOPortF_Handler(void){
	GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
	Bullets[count].x=Convert(ADCData)+46/2;
	Bullets[count].life=alive;
	count++;
}
int main(void){
  DisableInterrupts();
  PLL_Init(Bus80MHz);     	// Bus clock is 80 MHz 
  Random_Init(1);
  Output_Init();
	EdgeCounter_Init(); // initialize GPIO Port F interrupt 
	GameInit();
	Bullet_Init();
	ADC_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
  
	//ST7735_DrawBitmap(128-45, 151, x3, 43,45); // player ship bottom
	ST7735_DrawBitmap(53, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  Timer1_Init(80000000/30,0);
	EnableInterrupts();

	do{
		//while(Flag==0){};
		if(Flag) {
			GameDraw();
			Flag=0;
		}
	} while(1);
		Delay100ms(50);
	ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom

  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
  ST7735_DrawBitmap(100, 9, SmallEnemy30pointB, 16,10);

  Delay100ms(50);              // delay 5 sec at 80 MHz

  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }

}


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}
typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};

int main1(void){ char l;
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(phrase_t myPhrase=HELLO; myPhrase<= GOODBYE; myPhrase++){
    for(Language_t myL=English; myL<= French; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Delay100ms(30);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Delay100ms(20);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }  
}