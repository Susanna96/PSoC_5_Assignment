/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
//Include header
#include "InterruptRoutines.h"

//Include required header files
#include "project.h"

CY_ISR(Custom_ISR_ADC)
{
    Timer_ReadStatusRegister();
    
    if (status_register & DATA_AVAILABLE)
        {
        flag_ISR=1; 
        }
    
}   


  
/* [] END OF FILE */
