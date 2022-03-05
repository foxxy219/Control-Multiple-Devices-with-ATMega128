// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stddef.h>


#ifdef	__cplusplus
 extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */


unsigned char SegCode[] ={0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,  0x80, 0x90, 0x88, 0x83, 0XC6, 0xA1, 0x86, 0x8E}; // 0 1 2 3 4 5 6 7 8 9 A B C D E F
unsigned char lock;
#define Upper   *(unsigned char *)(0x91A)
#define Lower   *(unsigned char *)(0x91B)
#define Channel *(unsigned char *)(0x91C)
#define Lock1 *(unsigned char *)(0x91D)
#define Lock2 *(unsigned char *)(0x91E)

#define LCDE_H  (PORTG |= (1<<2))
#define LCDE_L  (PORTG &= ~(1<<2))

#define STOP 0
#define FORWARD 1
#define REWARD 2

#define ON 1
#define OFF 0
#define READKEY ( *(unsigned char *) (&Buttons) = PINB) //read buttons
#define ACTIVATION (PORTE = *(unsigned char *) (&motorsrelays)) //assign porte to address of motorsrelay
struct Button{
    unsigned char LEFT: 1;
    unsigned char RIGHT: 1;
    unsigned char UP: 1;
    unsigned char DOWN: 1;
    unsigned char GO: 1; 
}Buttons;


struct bits{
    unsigned char Motor1: 2;
    unsigned char Motor2: 2;
    unsigned char RL1: 1;
    unsigned char RL2: 1;
    unsigned char RL3: 1;
    unsigned char RL4: 1;
};

union RL{   //map 2 bien vo cung 1 dia chi   
    struct bits devices;
};

union RL motorsrelays;
#define Device_Relay1 0
#define Device_Relay2 1
#define Device_Relay34 2
#define Device_Motor1 3
#define Device_Motor2 4
#define Device_Motor 5
#define BeforeChannel 6
#define SensorSet 7
#define DeviceDisplay 8
#define SensorSetValue 9
#define DefaultValue 25
void DevicesActivation (char Devices, char Status, float n)
{

    switch (Devices){
        case Device_Relay1: if(Status) motorsrelays.devices.RL1=1;
        else motorsrelays.devices.RL1=0;
        ACTIVATION;
            break;
            
        case Device_Relay2: if(Status) motorsrelays.devices.RL2=1;
        else motorsrelays.devices.RL2=0;
        ACTIVATION;
            break;
            
        case Device_Relay34: 
            if(Status) {
            motorsrelays.devices.RL3=1;
            motorsrelays.devices.RL4=1;}
        else {
            motorsrelays.devices.RL3=0;
            motorsrelays.devices.RL4=0;}
        ACTIVATION;
            break;
    
        case Device_Motor1: 
            if ((n >= Lower) && (n < Upper) && (Status)){
            Lock1 = ON;
            motorsrelays.devices.Motor1 = FORWARD;}
            else if ((n >= Upper) && (Status)){
                Lock1 = ON;
                motorsrelays.devices.Motor1 = REWARD;}
            else {
                Lock1 = OFF;
                motorsrelays.devices.Motor1 = STOP;}
        ACTIVATION;
            break; 
            
        case Device_Motor2:
        if ((n >= Lower) && (n < Upper) && (Status)){
            Lock2 = ON;
            motorsrelays.devices.Motor2 = FORWARD;}
            else if ((n >= Upper) && (Status)){
                Lock2 = ON;
                motorsrelays.devices.Motor2 = REWARD;}
            else    {
                Lock2 = OFF; 
                motorsrelays.devices.Motor2 = STOP;}
        ACTIVATION;
            break;
        
        case Device_Motor: 
            if (Status){
            Lock1 = ON;
            Lock2 = ON;}
            if (Status==0){
                Lock1 = OFF;
                Lock2 = OFF;
               motorsrelays.devices.Motor1 = STOP;
                motorsrelays.devices.Motor2 = STOP;
                ACTIVATION;}
            break;
    }
}


//void DevicesActivationAuto (char Devices, char Status, float n)
//{
//
//    switch (Devices){       
//        case Device_Motor: 
//            if ((n >= Lower) && (n < Upper) && (Status)){
//            motorsrelays.devices.Motor1 = FORWARD;
//            motorsrelays.devices.Motor2 = FORWARD;
//            }
//            else if ((n >= Upper) && (Status)){
//                motorsrelays.devices.Motor1 = REWARD;
//            motorsrelays.devices.Motor2 = REWARD;}
//            else {motorsrelays.devices.Motor1 = STOP;
//            motorsrelays.devices.Motor2 = STOP;}
//        ACTIVATION;
//            break; 
//           
//    }
//}

void DevicesActivationAuto(float temp)
{     
            if ((temp >= Lower) && (temp < Upper)){
            motorsrelays.devices.Motor1 = FORWARD;
            motorsrelays.devices.Motor2 = FORWARD;
            }
            else if ((temp >= Upper)){
                motorsrelays.devices.Motor1 = REWARD;
            motorsrelays.devices.Motor2 = REWARD;}
            else {motorsrelays.devices.Motor1 = STOP;
            motorsrelays.devices.Motor2 = STOP;}
        ACTIVATION;
}





typedef struct Linker {
    char MenuID;
    struct Linker *pre;
    char Title[20];
    char List1[20]; 
    struct Linker *Menulist1; void (*ActivationON) (char, char, float); unsigned char (*Value);
    char List2[20];
    struct Linker *Menulist2; void (*ActivationOFF) (char, char, float); 
    char List3[20];
    struct Linker *Menulist3; 
} Menu ;
Menu MainMenu,
     AutoMenu,SettingMenu1,UpperMenu,LowerMenu,SetCMenu1,SetValueMenu,
     ManualMenu,MotorsMenu,Motor2,Motor1,RelaysMenu,Relay1,Relay2,Relay34,SetCMenu21,
     DisplayMenu,VUpperMenu, VLowerMenu, ChannelMenu;
Menu MainMenu ={
    NULL,
    NULL,
    "      MAIN MENU    ",
    "    Auto           ",&AutoMenu,NULL,NULL,
    "    Manual         ",&ManualMenu,NULL,
    "    Display        ",&DisplayMenu,
       
};





/*AUTO MENU-----------------------------------------------------*/
Menu AutoMenu ={
    Device_Motor,
    &MainMenu,
    "      Auto        ",
    "    ON            ",NULL,&DevicesActivation,NULL,
    "    OFF           ",NULL,&DevicesActivation,
    "    Setting       ",&SettingMenu1,     
};

Menu SettingMenu1 ={
    BeforeChannel,
    &AutoMenu,
    "    Setting         ",
    "  Upper threshold   ",&UpperMenu,NULL,&Channel,
    "  Lower threshold   ",&LowerMenu,NULL,
    "  Channel           ",&SetCMenu1,  
};

Menu UpperMenu ={
    SensorSet,
    &SettingMenu1,
    "    Upper threshold ",
    "   Set              ",&SetValueMenu,NULL,&Upper,
    "   Clear            ",NULL,NULL,
    "   None             ",NULL  
};

Menu LowerMenu ={
    SensorSet,
    &SettingMenu1,
    "    Lower threshold ",
    "   Set              ",&SetValueMenu,NULL,&Lower,
    "   Clear            ",NULL,NULL,
    "   None             ",NULL  
};

Menu SetValueMenu ={
    SensorSetValue,
    &UpperMenu,
    "    Set value      ",
    "   Value:          ",NULL,NULL,NULL,
    "   UP:+            ",NULL,NULL,
    "   DOWN:-          ",NULL,  
};

Menu SetCMenu1 ={
    NULL,
    &SettingMenu1,
    "    Set channel    ",
    "    Value:         ",NULL,NULL,&Channel,
    "    UP:+           ",NULL,NULL,
    "    DOWN:-         ",NULL,
       
};



/*MANUAL-----------------------------------------------------*/


Menu ManualMenu ={
    BeforeChannel,
    &MainMenu,
    "      Manual     ",
    "    MOTORS       ",&MotorsMenu,NULL,&Channel,
    "    RELAYS       ",&RelaysMenu,NULL,
    "    Channel      ",&SetCMenu21,
       
};
     
/*MotorsMenu-----------------------------------------------------*/
Menu MotorsMenu ={
    NULL,
    &ManualMenu,
    "      MOTORS     ",
    "    Motors 01    ",&Motor1,NULL,NULL,
    "    Motors 02    ",&Motor2,NULL,
    "    Back         ",&ManualMenu,
       
};

Menu Motor1 ={
    Device_Motor1,
    &MotorsMenu,
    "      MOTOR 01   ",
    "    On           ",NULL,&DevicesActivation,NULL,
    "    OFF          ",NULL,&DevicesActivation,
    "    Back         ",&MotorsMenu,
       
};

Menu Motor2 ={
    Device_Motor2,
    &MotorsMenu,
    "      MOTOR 02    ",
    "    On            ",NULL,&DevicesActivation,NULL,
    "    OFF           ",NULL,&DevicesActivation,
    "    Back          ",&MotorsMenu,
       
};


/*-------------------------------------------------------*/
Menu RelaysMenu ={
    NULL,
    &ManualMenu,
    "      RELAYS      ",
    "    RELAY 1       ",&Relay1,NULL,NULL,
    "    RELAY 2       ",&Relay2,NULL,
    "    RELAY 3+4     ",&Relay34,
       
};
Menu Relay1 ={
    Device_Relay1,
    &RelaysMenu,
    "      RELAYS 1    ",
    "    ON            ",NULL,&DevicesActivation,NULL,
    "    OFF           ",NULL,&DevicesActivation,
    "    Back          ",&RelaysMenu,
       
};
Menu Relay2 ={
    Device_Relay2,
    &RelaysMenu,
    "      RELAYS 2    ",
    "    ON            ",NULL,&DevicesActivation,NULL,
    "    OFF           ",NULL,&DevicesActivation,
    "    Back          ",&RelaysMenu,
       
};
Menu Relay34 ={
    Device_Relay34,
    &RelaysMenu,
    "      RELAYS 3+4  ",
    "    ON            ",NULL,&DevicesActivation,NULL,
    "    OFF           ",NULL,&DevicesActivation,
    "    Back          ",&RelaysMenu,
         
};

Menu SetCMenu21 ={
    NULL,
    &ManualMenu,
    "      Set channel  ",
    "    Value:         ",NULL,NULL,&Channel,
    "    UP:+           ",NULL,NULL,
    "    DOWN:-         ",NULL,
         
};


/*----------------------------*/
Menu DisplayMenu ={
    NULL,
    &MainMenu,
    "      Display       ",
    "   Channel          ",&ChannelMenu,NULL,&Channel,
    "   Upper threshold  ",&VUpperMenu,NULL,
    "   Lower threshold  ",&VLowerMenu,
       
};

Menu ChannelMenu ={
    DeviceDisplay,
    &DisplayMenu,
    "      Channel       ",
    "   Channel:         ",NULL,NULL,&Channel,
    "   Back             ",&DisplayMenu,NULL,
    "   None             ",NULL,  
};

Menu VUpperMenu ={
    DeviceDisplay,
    &DisplayMenu,
    "    Upper threshold ",
    "   value:           ",NULL,NULL,&Upper,
    "   Back             ",&DisplayMenu,NULL,
    "   None             ",NULL,  
       
};

Menu VLowerMenu ={
    DeviceDisplay,
    &DisplayMenu,
    "    Lower threshold ",
    "   value:           ",NULL,NULL,&Lower,
    "   Back             ",&DisplayMenu,NULL,
    "   None             ",NULL,  
       
};

void ValueDisplay (unsigned char *value){
    unsigned char Chuc, DonVi;
    Chuc=*value/10; 
    DonVi=*value%10;
    HangChuc(Chuc); 
    HangDonVi(DonVi); 
}

void HangChuc(unsigned char c){
    switch (c){
    case 0: PrintL("0",1,13);   break;
    case 1: PrintL("1",1,13);   break;
    case 2: PrintL("2",1,13);   break;
    case 3: PrintL("3",1,13);   break;
    case 4: PrintL("4",1,13);   break;
    case 5: PrintL("5",1,13);   break;
    case 6: PrintL("6",1,13);   break;
    case 7: PrintL("7",1,13);   break;
    case 8: PrintL("8",1,13);   break;
    case 9: PrintL("9",1,13);   break;
}
    }

void HangDonVi(unsigned char d){
    switch (d){
    case 0: PrintL("0",1,14);   break;
    case 1: PrintL("1",1,14);   break;
    case 2: PrintL("2",1,14);   break;
    case 3: PrintL("3",1,14);   break;
    case 4: PrintL("4",1,14);   break;
    case 5: PrintL("5",1,14);   break;
    case 6: PrintL("6",1,14);   break;
    case 7: PrintL("7",1,14);   break;
    case 8: PrintL("8",1,14);   break;
    case 9: PrintL("9",1,14);   break;
}
    }


void UpDown(Menu *menu, unsigned char *n)
{
    do
    {
        if (!Buttons.DOWN)
        {
            while (!Buttons.DOWN) READKEY;
            if (*n>1)
            *n=*n-1;
            ValueDisplay(menu->Value);
        } else
        if (!Buttons.UP)
        {
            while (!Buttons.UP) READKEY;
            if ((*n < 3 ) || (n!=0x91C)){            
            *n=*n+1;
            ValueDisplay(menu->Value);           
        }
        
        }
        
   READKEY; }
    while (Buttons.LEFT);
}

//void DirectActivation (Menu *menu,unsigned char lock, float temp){
//    
//    if(lock){
//        if (menu->MenuID==SensorSetValue){
//            DevicesActivationAuto(Device_Motor,ON,temp);
//        }
//    }
//    
//}
