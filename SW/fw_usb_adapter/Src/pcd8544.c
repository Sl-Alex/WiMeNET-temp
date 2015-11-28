#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"
#include "misc.h"

#include "pcd8544.h"
#include "font6x8.h"	  // шрифт
#include "font4x6.h"	  // шрифт
#include "delay_hw.h"

unsigned char lcd8544_buff[84*6] = {
0x00, 0x00, 0x00, 0xC1, 0x22, 0x22, 0x14, 0x14, 0x1C, 0x08, 0x08, 0x00, 0x00, 0x00, 0xC0, 0xE0,
0xF0, 0x78, 0x3C, 0x1C, 0x9E, 0x0E, 0x2E, 0x07, 0x0F, 0xC7, 0x27, 0x07, 0x8E, 0x0E, 0x2E, 0x1C,
0x9C, 0x38, 0x78, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0x78, 0x38,
0xBC, 0x1C, 0x2E, 0x0E, 0x8E, 0x07, 0xC7, 0x77, 0xCF, 0x07, 0x2E, 0x0E, 0x9E, 0x1C, 0x3C, 0x78,
0xF0, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0x40, 0x40, 0x20, 0x20, 0x18, 0x07, 0x01,
0x07, 0x18, 0x20, 0x20, 0x07, 0x1C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0xFF, 0xFF, 0x0F, 0x81, 0x00, 0x22, 0x00, 0x88, 0x08, 0x1C, 0x14, 0x63, 0xC1, 0x63, 0x14,
0x9C, 0x08, 0x2A, 0x00, 0x88, 0x00, 0x22, 0x00, 0x89, 0x07, 0x3F, 0x3F, 0xFC, 0x3F, 0x3F, 0x07,
0x89, 0x00, 0x22, 0x20, 0x70, 0x50, 0x52, 0x88, 0x88, 0x06, 0x01, 0x00, 0x01, 0x06, 0x88, 0x8A,
0xD8, 0x50, 0x72, 0x20, 0xA9, 0x07, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02,
0x02, 0x0C, 0x70, 0xC0, 0x70, 0x0C, 0x02, 0x02, 0x00, 0x00, 0x80, 0x80, 0x60, 0x38, 0x60, 0x80,
0x80, 0x00, 0x00, 0x00, 0x03, 0x3F, 0xFF, 0xFC, 0xE8, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00,
0x88, 0x01, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00,
0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00, 0x88, 0x03, 0x1C, 0x70,
0x1C, 0x83, 0x20, 0x00, 0x88, 0x00, 0x22, 0x00, 0xC8, 0xFC, 0xFF, 0x3F, 0x07, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x80, 0xE0, 0x80, 0x01, 0x01, 0x03, 0x02,
0x0C, 0x38, 0x0C, 0x02, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x3F, 0x7E, 0xF8,
0xE8, 0xC0, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x40, 0x48, 0xE0, 0xA2, 0x18,
0x0E, 0x18, 0xA2, 0xE0, 0x48, 0x40, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00,
0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x80, 0xE8, 0xF0, 0xFE, 0x3F, 0x1F, 0x07, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x0E, 0x0A, 0x31, 0xE0, 0x31,
0x00, 0x00, 0x00, 0xE0, 0x38, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1E, 0x3C, 0x78, 0xF2, 0xE0, 0xE8, 0xC0, 0xA2, 0x00,
0x88, 0x00, 0x20, 0x03, 0x8E, 0x03, 0x20, 0x00, 0x88, 0x00, 0x22, 0x00, 0x88, 0x00, 0x22, 0x00,
0x88, 0x00, 0xA2, 0xC0, 0xC8, 0xE0, 0xF2, 0x78, 0x3C, 0x1E, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x60, 0x1C, 0x07, 0x1C, 0x60, 0x80, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x83, 0x00, 0x00, 0x00, 0x83, 0x44, 0x44, 0x28, 0x28, 0x38,
0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
0x01, 0x03, 0x07, 0x07, 0x0E, 0x1E, 0x1E, 0x3C, 0x38, 0x78, 0x72, 0xF0, 0xE8, 0xF0, 0x72, 0x78,
0x38, 0x3C, 0x1E, 0x1E, 0x0F, 0x07, 0x07, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x07, 0x05, 0x05, 0x08, 0x08, 0x30, 0xC0, 0x00, 0xC0, 0x30,
0x08, 0x08, 0x05, 0x05, 0x07, 0x02, 0x02, 0x00,
}; // буфер дисплея

// отправка данных\команд на дисплей
void lcd8544_senddata(unsigned char data) {

#ifdef LCD_TO_SPI2
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2, data);
#else
	unsigned char i;
	for(i=0; i<8; i++) {
		if (data & 0x80) LCD_MOSI1;
		            else LCD_MOSI0;
		data = data<<1;
		LCD_SCK0;
		LCD_SCK1;
	}
#endif
}



// очистка дисплея
void lcd8544_refresh(void) {
#ifdef LCD_SEL_AUTO
	LCD_CS0;            // СS=0 - начали сеанс работы с дисплеем
#endif

	LCD_DC0;            // передача комманд
	lcd8544_senddata(0x40); // установка курсора в позицию Y=0; X=0
	lcd8544_senddata(0x80);

#ifdef LCD_TO_SPI2
	while(SPI2->SR & SPI_SR_BSY);
#endif

	LCD_DC1;            // передача данных

#ifndef LCD_SPI2_DMA // режим DMA не используется
	unsigned char y, x;

	for (y=0;y<6;y++) for (x=0;x<84;x++) lcd8544_senddata(lcd8544_buff[y*84+x]);
    #ifdef LCD_TO_SPI2
    	 while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
    #endif
    #ifdef LCD_SEL_AUTO
	   LCD_CS1;			// СS=1 - закончили сеанс работы с дисплеем
    #endif

#else // режим DMA
    	 DMA_Cmd(DMA1_Stream4, ENABLE);
#endif
}



// очистка дисплея
void lcd8544_clear(void) {
   unsigned char x,y;
   for (y=0;y<6;y++) for(x=0;x<84;x++) lcd8544_buff[y*84+x]=0;
}



// Инициализация
void lcd8544_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LCD_APB2_GPIO, ENABLE); // тактирование GPIOB

	SPI_InitTypeDef  SPI_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  // тактирование SPI2

	GPIO_InitStructure.GPIO_Pin 			= GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode 			= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed           = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    SPI_Cmd(SPI2, DISABLE); // Sl-Alex
	//SPI_DeInit(SPI2); // сбрасываем настройки SPI2 перед заданием конфигурации
	SPI_InitStructure.SPI_Mode        		= SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction   		= SPI_Direction_1Line_Tx;
	SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL 				= SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA 				= SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS 				= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI2_BaudRatePrescaler;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;

	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);

	GPIO_InitStructure.GPIO_Pin  			=(1<<LCD_CSE_PIN) | (1<<LCD_A0_PIN) | (1<<LCD_RST_PIN);
    GPIO_InitStructure.GPIO_Mode 			=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed			=GPIO_Speed_50MHz;
	GPIO_Init(LCD_GPIO, &GPIO_InitStructure);


	LCD_CS0;            // CS=0  - начали сеанс работы с дисплеем
	LCD_DC0;			// A0=0  - переход в командный режим

	// сброс дисплея
	LCD_RST0;                   // RST=0
	delay_hw_ms(1);
	LCD_RST1;                   // RST=1
	delay_hw_ms(LCD_RST_DLY);

	// инициализация дисплея
	lcd8544_senddata(0x21);      // переход в расширенный режим
	lcd8544_senddata(0xC1);

	lcd8544_senddata(0x06);		// температурный коэффициент, от 4 до 7

	lcd8544_senddata(0x14);		// Bias 0b0001 0xxx - работает как контрастность

	lcd8544_senddata(0x20); 	// переход в обычный режим
	lcd8544_senddata(0b1100);	// 0b1100 - normal mode
		                        // 0b1101 - invert mode
		                        // 0b1001 - полностью засвеченный экран
		                        // 0b1000 - чистый экран

	while(SPI2->SR & SPI_SR_BSY);

#ifdef LCD_SEL_AUTO
	LCD_CS1;
#endif
}



// вывод пиксела
void lcd8544_putpix(unsigned char x, unsigned char y, unsigned char mode) {
	if ((x>84) || (y>47)) return;

	unsigned int adr=(y>>3)*84+x;
	unsigned char data=(1<<(y&0x07));

	if (mode) lcd8544_buff[adr]|=data;
   else lcd8544_buff[adr]&=~data;
}



// процедура рисования линии
void lcd8544_line(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode) {
signed char   dx, dy, sx, sy;
unsigned char  x,  y, mdx, mdy, l;


  dx=x2-x1; dy=y2-y1;

  if (dx>=0) { mdx=dx; sx=1; } else { mdx=x1-x2; sx=-1; }
  if (dy>=0) { mdy=dy; sy=1; } else { mdy=y1-y2; sy=-1; }

  x=x1; y=y1;

  if (mdx>=mdy) {
     l=mdx;
     while (l>0) {
         if (dy>0) { y=y1+mdy*(x-x1)/mdx; }
            else { y=y1-mdy*(x-x1)/mdx; }
         lcd8544_putpix(x,y,mode);
         x=x+sx;
         l--;
     }
  } else {
     l=mdy;
     while (l>0) {
        if (dy>0) { x=x1+((mdx*(y-y1))/mdy); }
          else { x=x1+((mdx*(y1-y))/mdy); }
        lcd8544_putpix(x,y,mode);
        y=y+sy;
        l--;
     }
  }
  lcd8544_putpix(x2, y2, mode);

}



// рисование прямоугольника (не заполненного)
void lcd8544_rect(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode) {
	lcd8544_line(x1,y1, x2,y1, mode);
	lcd8544_line(x1,y2, x2,y2, mode);
	lcd8544_line(x1,y1, x1,y2, mode);
	lcd8544_line(x2,y1, x2,y2, mode);
}



// печать десятичного числа
void lcd8544_dec(unsigned int numb, unsigned char dcount, unsigned char x, unsigned char y, unsigned char mode, unsigned char small) {
	unsigned int divid=10000;
	unsigned char i;

	for (i=5; i!=0; i--) {

		unsigned char res=numb/divid;

		if (i<=dcount) {
			lcd8544_putchar(x, y, res+'0', mode, small);
            if (!small)
                x=x+6;
            else
                x=x+4;
		}

		numb%=divid;
		divid/=10;
	}
}



// вывод символа на экран по координатам
void lcd8544_putchar(unsigned char px, unsigned char py, unsigned char ch, unsigned char mode, unsigned char small) {
	const unsigned char *fontpointer;
	unsigned char fontwidth = 6;

		if (px>83) return;

    if (small)
    {
        fontpointer=NewFontSmall; ch = ch - 0x20;
        fontwidth = 4;
    }
    else
    {
        if (ch<127) {	// знакогенератор латиницы
            fontpointer=NewFontLAT; ch=ch-32; }
            else	{	// знакогенератор русских букв
            fontpointer=NewFontRUS; ch=ch-192;}
    }


    unsigned char lcd_YP=7- (py & 0x07);    // битовая позиция символа в байте
    unsigned char lcd_YC=(py & 0xF8)>>3; 	// байтовая позиция символа на экране
    if (lcd_YC > 5) return;
    if (px > 83) return;
    unsigned char x;
	for (x=0; x<fontwidth; x++) {

		unsigned char temp=*(fontpointer+ch*fontwidth+x);

		if (mode!=0) {
			temp=~temp;
			if (py>0) lcd8544_putpix(px, py-1, 1);	// если печать в режиме инверсии - сверху отчертим линию
		}

		temp&=0x7F;

//		lcd8544_buff[lcd_YC*84+px]=lcd8544_buff[lcd_YC*84+px] | (temp<<(7-lcd_YP)); 	// печать верхней части символа
//	    if (lcd_YP<7) lcd8544_buff[(lcd_YC+1)*84+px]=lcd8544_buff[(lcd_YC+1)*84+px] | (temp>>(lcd_YP+1)); 	// печать нижней части символа
//        if (!small)
        {
            lcd8544_buff[lcd_YC*84+px] = (lcd8544_buff[lcd_YC*84+px] & (~(0xFF << (7 - lcd_YP)))) | (temp<<(7-lcd_YP)); 	// печать верхней части символа
            if (lcd_YP<7)
            {
                if (lcd_YC < 5)
                lcd8544_buff[(lcd_YC+1)*84+px]=(lcd8544_buff[(lcd_YC+1)*84+px] & (~(0xFF >> (lcd_YP+1)))) | (temp>>(lcd_YP+1)); 	// печать нижней части символа
            }
        }
//        else
        {
            lcd8544_buff[lcd_YC*84+px] = (lcd8544_buff[lcd_YC*84+px] & (~(0x1F << (7 - lcd_YP)))) | (temp<<(7-lcd_YP)); 	// печать верхней части символа
            if (lcd_YP<7)
            {
                if (lcd_YC < 5)
                lcd8544_buff[(lcd_YC+1)*84+px]=(lcd8544_buff[(lcd_YC+1)*84+px] & (~(0x1F >> (lcd_YP+1)))) | (temp>>(lcd_YP+1)); 	// печать нижней части символа
            }
        }

		px++;
		if (px>83) return;
	}
}



// линия левее символа для печати в инверсе
void lcd8544_leftchline(unsigned char x, unsigned char y) {
	if (x>0) lcd8544_line(x-1, y-1, x-1, y+6, 1);
}


// вывод строки
void lcd8544_putstr(unsigned char x, unsigned char y, const char str[], unsigned char mode, unsigned char small) {
	if (mode) lcd8544_leftchline(x, y);
	while (*str!=0) {
		lcd8544_putchar(x, y, *str, mode, small);
		if (!small)
            x=x+6;
        else
            x=x+4;
		str++;
	}
}
