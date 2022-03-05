/*
 * File:   Minimain.c
 * Author: quanb
 *
 * Created on October 17, 2021, 2:40 PM
 */

#include <xc.h>
#include "miniprojectheader.h"
#include <stddef.h>
#define F_CPU 1000000
#include <util/delay.h>
#include "DS18B20.h"




void SevenSegmentDisplay (unsigned char value){
 PORTA=SegCode[(value/10)%10]; 
 PORTC=SegCode[value%10];

}

//motor
    //Motors.DC1=STOP;
    //Motors.DC2=STOP;
    //MOTORACTIVATION;
   

void LCDInit(){
    PORTD = 0x38; LCDE_H; _delay_us(1); LCDE_L; _delay_us(1);// set 8-bit data length
    _delay_us(200);
    PORTD = 0x0C; LCDE_H; _delay_us(1); LCDE_L; _delay_us(1); //Display On, Cursor off, Blink off
    _delay_us(200);
    PORTD = 0x06; LCDE_H; _delay_us(1); LCDE_L; _delay_us(1); //Entry Mode Set
    _delay_us(200);
    PORTD = 0x01; LCDE_H; _delay_us(1); LCDE_L; _delay_us(1);//clear display
    _delay_ms(20);
}

void PrintL(const char *str, unsigned line, unsigned col)
{
    unsigned char add;
    switch (line)
    {
        case 0: add = 0x80; break;  //1st line
        case 1: add = 0xC0; break;  //2nd line  
        case 2: add = 0x94; break;  //3rd line
        case 3: add = 0xD4; break;  //4th line
    }
    PORTD = add + col; LCDE_H; _delay_us(1); LCDE_L; _delay_us(50); //output all the lines
    
    while (*(str) != '\0'){
    PORTG=0x01; //Data input
    PORTD = *str++; LCDE_H; _delay_us(1); LCDE_L; _delay_us(10);    //output data
    PORTG=0x00; //Instruction Input 
    }
}

void MenuDisplay(Menu *menu, unsigned char select)
{
    PrintL(menu->Title,0,0);    //title menu
    PrintL(menu->List1,1,0);    //row 1 
    PrintL(menu->List2,2,0);    //row 2
    PrintL(menu->List3,3,0);    //row 3
    PrintL(">",select,0);
    if ((menu->MenuID==DeviceDisplay) || (menu->MenuID==SensorSetValue)) {
        ValueDisplay(menu->Value);}
}




int main(void) {
    float temp;
    unsigned char select=1;
    Menu *menu;
    DDRA = 0xff;    //output 
    DDRC = 0xff;
    DDRE = 0xff;
    DDRD = 0xff;
    DDRB = 0x00;    //input
    DDRF = ~0x01;   //input FortF0
    DDRG = 0x0f;    
    Lower=DefaultValue;
    Upper=DefaultValue;
    Channel=1;
    Lock1 = 0;
    Lock2 = 0;

    

    menu = &MainMenu;
    LCDInit();
    MenuDisplay(menu, select);  
    
    while(1){ 
    therm_reset();
    SevenSegmentDisplay(temp);
    temp = DS18B20(Channel);
        READKEY; 
        if (!Buttons.DOWN) 
        {
            while (!Buttons.DOWN) READKEY; 
            select = (select==3)?1:select+1; 
           
          MenuDisplay(menu,select);
          
        }
        if (!Buttons.UP)   
        {
            while (!Buttons.UP) READKEY;
            select = (select==1)?1:select-1; 
            MenuDisplay(menu,select);
           
        }
        
        
        if (!Buttons.RIGHT)
        {
            while (!Buttons.RIGHT) READKEY;
            if ((menu->MenuID == SensorSet) &(select ==1))
                {
                    MenuDisplay(menu->Menulist1,1);
                    ValueDisplay(menu->Value);
                    UpDown(menu, menu->Value);
                    select=1;
                    MenuDisplay(menu,select); 
                    while (!Buttons.LEFT) READKEY;
                }
                else       
                if ((menu->MenuID == BeforeChannel)& (select ==3))
                {
                    MenuDisplay(menu->Menulist3,1);
                    ValueDisplay(menu->Value);
                    UpDown(menu, menu->Value);
                    select=1;
                    MenuDisplay(menu,select);
                    READKEY;
                    DS18B20(Channel);
                    while (!Buttons.LEFT) READKEY;
                }                        
            else
            {
            switch (select)
            {
                case 1: menu=(menu->Menulist1 == NULL)? menu: menu->Menulist1;break;
                case 2: menu=(menu->Menulist2 == NULL)? menu: menu->Menulist2;break;
                case 3: menu=(menu->Menulist3 == NULL)? menu: menu->Menulist3;break;
            }
            
            MenuDisplay(menu, select); 
        }
        }
        if (!Buttons.LEFT) 
        {
            while (!Buttons.LEFT) READKEY;
            menu=(menu->pre == NULL)? menu: menu->pre;
            MenuDisplay(menu, select);  
            
            
            }
        if (!Buttons.GO) 
        {
            while (!Buttons.GO) READKEY; 
            if ((menu->MenuID==SensorSet) && (select==2)){
                    *(menu->Value) = 25;
                    MenuDisplay(menu,select);
                }
            
            else
            switch (select){            
                case 1: 
                    if (menu->ActivationON!=NULL) menu->ActivationON(menu->MenuID,ON,temp);
                    break;
                case 2: 
                    if (menu->ActivationOFF!=NULL) menu->ActivationOFF(menu->MenuID,OFF,temp);
                    break;
            }         
        }
        if ((Lock1)&&(Lock2)){
        DevicesActivationAuto(temp);   
        }
    }     
    }
