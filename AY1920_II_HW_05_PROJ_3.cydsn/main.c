/**
* \brief Main source file for the I2C-Master project.
*
* In this project we set up a I2C master device with
* to understand the I2C protocol and communicate with a
* a I2C Slave device (LIS3DH Accelerometer).
*
*/

// Include required header files
#include "I2C_Interface.h"
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"

/**
*   \brief 7-bit I2C address of the slave device.
*/
#define LIS3DH_DEVICE_ADDRESS 0x18

/**
*   \brief Address of the WHO AM I register
*/
#define LIS3DH_WHO_AM_I_REG_ADDR 0x0F

/**
*   \brief Address of the Status register
*/
#define LIS3DH_STATUS_REG 0x27

/**
*   \brief Address of the Control register 1
*/
#define LIS3DH_CTRL_REG1 0x20

/**
*   \brief Hex value to set normal mode to the accelerator
*/
#define LIS3DH_HR_MODE_CTRL_REG1 0x57  //High resolution mode, 100 Hz



/**
*   \brief Address of the Control register 4
*/
#define LIS3DH_CTRL_REG4 0x23

/**
*   \brief Hex value to set the FSR and activate the BDU
*/
#define LIS3DH_CTRL_REG4_BDU_ACTIVE 0x98 // [-4.0g, +4.0g] FSR, BDU active

/**
*   \brief Addresses of the Output registers 
*/
#define LIS3DH_OUT_X_L 0x28 
#define LIS3DH_OUT_X_H 0x29 
#define LIS3DH_OUT_Y_L 0x2A 
#define LIS3DH_OUT_Y_H 0x2B 
#define LIS3DH_OUT_Z_L 0x2C 
#define LIS3DH_OUT_Z_H 0x2D 

#define SENSITIVITY 0.002   // senitivity = 0.002g/digit
#define G 9.81              //gravitational acceleration = 9.81 m/s^2
#define DECIMALS 10000      //multiplication factor to keep 4 decimals


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Timer_Start();
    isr_ADC_StartEx(Custom_ISR_ADC);
    I2C_Peripheral_Start();
    UART_Debug_Start();
    
    flag_ISR=0; //initialization of flag_ISR
    
    CyDelay(5); //"The boot procedure is complete about 5 milliseconds after device power-up."
    
    // String to print out messages on the UART
    char message[50];

    // Check which devices are present on the I2C bus
    for (int i = 0 ; i < 128; i++)
    {
        if (I2C_Peripheral_IsDeviceConnected(i))
        {
            // print out the address is hex format
            sprintf(message, "Device 0x%02X is connected\r\n", i);
            UART_Debug_PutString(message); 
        }
        
    }
    
    /******************************************/
    /*            I2C Reading                 */
    /******************************************/
    
    /* Read WHO AM I REGISTER register */
    uint8_t who_am_i_reg;
    ErrorCode error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                                  LIS3DH_WHO_AM_I_REG_ADDR, 
                                                  &who_am_i_reg);
    if (error == NO_ERROR)
    {
        sprintf(message, "WHO AM I REG: 0x%02X [Expected: 0x33]\r\n", who_am_i_reg);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm\r\n");   
    }
    
    /*      I2C Reading Status Register       */
     
    uint8_t status_register;
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "STATUS REGISTER: 0x%02X\r\n", status_register);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read status register\r\n");   
    }
    
    /******************************************/
    /*        Read Control Register 1         */
    /******************************************/
    uint8_t ctrl_reg1; 
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
    /******************************************/
    /*            I2C Writing                 */
    /******************************************/
    
        
    UART_Debug_PutString("\r\nWriting new values..\r\n");
    
    if (ctrl_reg1 != LIS3DH_HR_MODE_CTRL_REG1)
    {
        ctrl_reg1 = LIS3DH_HR_MODE_CTRL_REG1;
    
        error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                             LIS3DH_CTRL_REG1,
                                             ctrl_reg1);
    
        if (error == NO_ERROR)
        {
            sprintf(message, "CONTROL REGISTER 1 successfully written as: 0x%02X\r\n", ctrl_reg1);
            UART_Debug_PutString(message); 
        }
        else
        {
            UART_Debug_PutString("Error occurred during I2C comm to set control register 1\r\n");   
        }
    }
    
    /******************************************/
    /*     Read Control Register 1 again      */
    /******************************************/

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG1,
                                        &ctrl_reg1);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 1 after overwrite operation: 0x%02X\r\n", ctrl_reg1);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register 1\r\n");   
    }
    
     /******************************************/
     /* I2C Reading Temperature sensor CFG reg */
     /******************************************/

    
    
    uint8_t ctrl_reg4;

    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    
    ctrl_reg4 = LIS3DH_CTRL_REG4_BDU_ACTIVE; // must be changed to the appropriate value
    
    error = I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG4,
                                         ctrl_reg4);
    
    error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_CTRL_REG4,
                                        &ctrl_reg4);
    
    
    if (error == NO_ERROR)
    {
        sprintf(message, "CONTROL REGISTER 4 after being updated: 0x%02X\r\n", ctrl_reg4);
        UART_Debug_PutString(message); 
    }
    else
    {
        UART_Debug_PutString("Error occurred during I2C comm to read control register4\r\n");   
    }
    
    int16_t Out_accX;
    float32 Out_accX2;
    int16_t Out_accY;
    float32 Out_accY2;
    int16_t Out_accZ;
    float32 Out_accZ2;
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray[14]; 
    uint8_t acc[6];
    
    int32 interoX;
    int32 interoY;
    int32 interoZ;
    
    OutArray[0] = header;
    OutArray[13] = footer;
    
    for(;;)
    {
        if(flag_ISR)
        { 
            error=I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                        LIS3DH_STATUS_REG,
                                        &status_register);
            
            if ((error == NO_ERROR) && ((status_register) & (DATA_AVAILABLE)))
            {
             
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                 LIS3DH_OUT_X_L,6,
                                                 &acc[0]);
        
       
                if(error == NO_ERROR)
                {
                    /******************************************/
                    /*               Acc_X                    */
                    /******************************************/
                    Out_accX = (int16)((acc[0] | (acc[1]<<8)))>>4;
                    
                    /*cast to a floating point in m/s^2 units (sensitivity=2mg/digit)*/
                    Out_accX2= (float32)Out_accX*G*SENSITIVITY;
                    
                    /*multiplication by a factor of 10000 to keep 4 decimals and cast to int32*/
                    interoX=(int32) (Out_accX2*DECIMALS);    
                    
                    /*divide the int32 in 4 bytes */ 
                    OutArray[1]=(uint8_t)(interoX & 0xFF);
                    OutArray[2]=(uint8_t)((interoX >> 8)& 0xFF);
                    OutArray[3]=(uint8_t)((interoX >> 16)& 0xFF);
                    OutArray[4]=(uint8_t)(interoX >> 24);
                    
                    /******************************************/
                    /*               Acc_Y                    */
                    /******************************************/
                    Out_accY = (int16)((acc[2] | (acc[3]<<8)))>>4;
                    
                    /*cast to a floating point in m/s^2 units (sensitivity=2mg/digit)*/
                    Out_accY2= (float32)Out_accY*G*SENSITIVITY;
                    
                    /*multiplication by a factor of 10000 to keep 4 decimals and cast to int32*/
                    interoY=(int32) (Out_accY2*DECIMALS); 
                    
                    /*divide the int32 in 4 bytes */
                    OutArray[5]=(uint8_t)(interoY & 0xFF);
                    OutArray[6]=(uint8_t)((interoY >> 8)& 0xFF);
                    OutArray[7]=(uint8_t)((interoY >> 16)& 0xFF);
                    OutArray[8]=(uint8_t)(interoY >> 24);
                    
                    /******************************************/
                    /*               Acc_Z                    */
                    /******************************************/
                    Out_accZ = (int16)((acc[4] | (acc[5]<<8)))>>4;
                    
                    /*cast to a floating point in m/s^2 units (sensitivity=2mg/digit)*/
                    Out_accZ2= (float32)Out_accZ*G*SENSITIVITY;
                    
                    /*multiplication by a factor of 10000 to keep 4 decimals and cast to int32*/
                    interoZ=(int32) (Out_accZ2*DECIMALS);
                    
                    /*divide the int32 in 4 bytes */
                    OutArray[9]=(uint8_t)(interoZ & 0xFF);
                    OutArray[10]=(uint8_t)((interoZ >> 8)& 0xFF);
                    OutArray[11]=(uint8_t)((interoZ >> 16)& 0xFF);
                    OutArray[12]=(uint8_t)(interoZ >> 24);
                    
                    /*send bytes to be plotted to Bridge Contol Panel*/
                    UART_Debug_PutArray(OutArray, 14);
                    
                    
                }
            }
        }
        flag_ISR=0;
    }
}

/* [] END OF FILE */
