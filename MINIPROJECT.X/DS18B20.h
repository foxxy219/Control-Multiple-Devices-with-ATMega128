

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

#define THERM_CMD_CONVERTTEMP 0x44 
#define THERM_CMD_RSCRATCHPAD 0xbe 
#define THERM_CMD_WSCRATCHPAD 0x4e 
#define THERM_CMD_CPYSCRATCHPAD 0x48 
#define THERM_CMD_RECEEPROM 0xb8 
#define THERM_CMD_RPWRSUPPLY 0xb4 
#define THERM_CMD_SEARCHROM 0xf0 
#define THERM_CMD_READROM 0x33 
#define THERM_CMD_MATCHROM 0x55 
#define THERM_CMD_SKIPROM 0xcc 
#define THERM_CMD_ALARMSEARCH 0xec

#define DS18B20_9BIT_RES 0  // 9 bit thermometer resolution
#define DS18B20_10BIT_RES 1 // 10 bit thermometer resolution
#define DS18B20_11BIT_RES 2 // 11 bit thermometer resolution
#define DS18B20_12BIT_RES 3 // 12 bit thermometer resolution


#define THERM_PORT PORTF
#define THERM_DDR DDRF
#define THERM_PIN PINF 
#define THERM_INPUT_MODE() THERM_DDR&=~(1<<DDF0) 
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<DDF0) 
#define THERM_LOW() THERM_PORT&=~(1<<PORTF0) 
#define THERM_HIGH() THERM_PORT|=(1<<PORTF0)

unsigned char temp_Register[2]; //store 2 byte of temp
float temp1; // nhiet do moi truong
float temp2;
float temp3;


unsigned char therm_reset(){ 
    unsigned char i; 

        THERM_LOW(); THERM_OUTPUT_MODE(); _delay_us(480);
        //Release line and wait for 60uS 
        THERM_INPUT_MODE(); _delay_us(60);
        //Store line value and wait until the completion of 480uS period 
        i=(THERM_PIN & (1<<PORTF0)); 
        _delay_us(420); //Return the value read from the presence pulse (0=OK, 1=WRONG) 
        return i; }


void ds18b20_Write_Bit(unsigned char a)
{
 if(a==1)
 {
     THERM_DDR = 1; //input mode    
     THERM_PORT = 0;    //
     _delay_us(6);
     
     THERM_PORT = 1;
     _delay_us(64); 
 }
 else if(a==0)
 {
     THERM_DDR = 1;
     THERM_PORT = 0;
     _delay_us(60);     
     THERM_PORT = 1;
     _delay_us(10); 
 }
}

unsigned char ds18b20_Read_Bit()
{
    unsigned char result;
    THERM_DDR = 1;
    THERM_PORT = 0;
    _delay_us(6);

    THERM_PORT = 1;
    _delay_us(9);

    THERM_DDR = 0;
    result = THERM_PIN; 
    _delay_us(55);

    return result;
}

void ds18b20_Write_Byte(unsigned char a)    //loop write byte 1 bit 8 times
{
 unsigned char i=8;
 while(i--)
 {
   ds18b20_Write_Bit(a & 0x01); 
   a >>= 1; 
 }
}

unsigned char ds18b20_Read_Byte()   //loop read byte 1 bit 8 times
{
 unsigned char i=8, result = 0;
 while(i--)
 {
    result >>= 1;
    result |= ds18b20_Read_Bit()<<7;
 }
 return result;
}

float DS18B20(char i){
    unsigned char therm1[] = {0x28,0x90,0x11,0x19,0x00,0x00,0x00,0xB5};
    unsigned char therm2[] = {0x28,0x91,0x11,0x19,0x00,0x00,0x00,0x82};
    unsigned char therm3[] = {0x28,0x92,0x11,0x19,0x00,0x00,0x00,0xDB};
   
     
        if (i==1){
             while (1){
     
     while(therm_reset());
     ds18b20_Write_Byte(THERM_CMD_MATCHROM);
     for (unsigned char x=0;x<8;x++){
        ds18b20_Write_Byte(therm1[x]);//28h Family Code
       _delay_ms(14);
     }    
     ds18b20_Write_Byte(THERM_CMD_CONVERTTEMP);
     _delay_ms(94);  
     
     
     temp_Register[0]= therm_reset();
     ds18b20_Write_Byte(THERM_CMD_MATCHROM);
     for (unsigned char x=0;x<8;x++){
        ds18b20_Write_Byte(therm1[x]);
       _delay_ms(14);
     }    
     ds18b20_Write_Byte(THERM_CMD_RSCRATCHPAD);
     temp_Register[0] = ds18b20_Read_Byte();
     temp_Register[1] = ds18b20_Read_Byte();  
     
     temp1 = (temp_Register[0] + temp_Register[1] *(256))/16; 
     
     return temp1;
        }
        }
        
        if (i==2){
            while (1){
     
     while(therm_reset());
     ds18b20_Write_Byte(THERM_CMD_MATCHROM);
     for (unsigned char x=0;x<8;x++){
        ds18b20_Write_Byte(therm2[x]);//28h Family Code
       _delay_ms(14);
     }    
     ds18b20_Write_Byte(THERM_CMD_CONVERTTEMP);
     _delay_ms(94);   
     
     
     temp_Register[0]= therm_reset();
     ds18b20_Write_Byte(THERM_CMD_MATCHROM);
     for (unsigned char x=0;x<8;x++){
        ds18b20_Write_Byte(therm2[x]);//28h Family Code
       _delay_ms(14);
     }    
     ds18b20_Write_Byte(THERM_CMD_RSCRATCHPAD);
     temp_Register[0] = ds18b20_Read_Byte();
     temp_Register[1] = ds18b20_Read_Byte();  
     
     temp2 = (temp_Register[0] + temp_Register[1] *(256))/16; 
     
     return temp2;
        }
        }
        
        if (i==3){
            while(1){
     
     while(therm_reset());
     ds18b20_Write_Byte(THERM_CMD_MATCHROM);
     for (unsigned char x=0;x<8;x++){
        ds18b20_Write_Byte(therm3[x]);//28h Family Code
       _delay_ms(14);
     }    
     ds18b20_Write_Byte(THERM_CMD_CONVERTTEMP);
     _delay_ms(94);   
     
     //Ket noi den Slave, bat dau doc du lieu nhiet do
     temp_Register[0]= therm_reset();
     ds18b20_Write_Byte(THERM_CMD_MATCHROM);
     for (unsigned char x=0;x<8;x++){
        ds18b20_Write_Byte(therm3[x]);//28h Family Code
       _delay_ms(14);
     } ds18b20_Write_Byte(THERM_CMD_RSCRATCHPAD);
     temp_Register[0] = ds18b20_Read_Byte();
     temp_Register[1] = ds18b20_Read_Byte();  
     
     temp3 = (temp_Register[0] + temp_Register[1] *(256))/16; //lay nhiet do tu temp_Register
     
     return temp3;
        }
        }
}     

        
        
        

    
     
       

        



//
//unsigned char ds18b20_Reset()
//{
//    unsigned char result;
//    ds18b20_DDR = 1; 
//    ds18b20_PORT = 0x00;
//    _delay_us(480);
//    ds18b20_PORT = 0x01;
//    _delay_us(60);
//
//    ds18b20_DDR = 0;
//    result = ds18b20_PIN;
//    _delay_us(420);
//
//    return result;
//}
//
//void ds18b20_Write_Bit(unsigned char a)
//{
// if(a==1)
// {
//     ds18b20_DDR = 1;
//     ds18b20_PORT = 0x00;
//     _delay_us(6);
//     
//     ds18b20_PORT = 0x01;
//     _delay_us(64); 
// }
// else if(a==0)
// {
//     ds18b20_DDR = 1;
//     ds18b20_PORT = 0x00;
//     _delay_us(60);
//     
//     ds18b20_PORT = 0x01;
//     _delay_us(10); 
// }
//}
//
//unsigned char ds18b20_Read_Bit()
//{
//    unsigned char result1;
//    ds18b20_DDR = 1;
//    ds18b20_PORT = 0x00;
//    _delay_us(6);
//
//    ds18b20_PORT = 0x01;
//    _delay_us(9);
//
//    result1 = ds18b20_PIN; 
//    _delay_us(55);
//
//    return result1;
//}
//
//void ds18b20_Write_Byte(unsigned char a)
//{
// unsigned char i=8;
// while(i--)
// {
//   ds18b20_Write_Bit(a & 0x01);
//   a >>= 1; 
// }
//}
//
//unsigned char ds18b20_Read_Byte()
//{
// unsigned char i=8, result = 0;
// while(i--)
// {
//    result >>= 1;
//    result |= ds18b20_Read_Bit()<<7;
// }
// return result;
//}
//
//float DS18B20(){
//    while (1)
//     { 
//     //Ket noi den Slave, bat dau qua trinh chuyen doi nhiet do
//     while(ds18b20_Reset());
//     ds18b20_Write_Byte(DS18B20_SKIP_ROM);
//     ds18b20_Write_Byte(DS18B20_CONVERT_T);
//     _delay_ms(94);   //che do 9 bit mat 93.75ms de chuyen doi
//     
//     //Ket noi den Slave, bat dau doc du lieu nhiet do
//     while(ds18b20_Reset());
//     ds18b20_Write_Byte(DS18B20_SKIP_ROM);
//     ds18b20_Write_Byte(DS18B20_READ_SCRATCH_PAD);
//     
//     temp_Register = ds18b20_Read_Byte();
//     temp_Register = (ds18b20_Read_Byte() <<8) |temp_Register;  
//     
//     temp = (temp_Register >> 4)+ (temp_Register & 0x000F)*1.0f/16; //lay nhiet do tu temp_Register
//     
//     return temp;
//}
//}