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
#ifndef __INTERRUPT_ROUTINES_H
   #define __INTERRUPT_ROUTINES_H

    #include "cytypes.h"
    #include "stdio.h"

    
    CY_ISR_PROTO(Custom_ISR_ADC);
    
    #define DATA_AVAILABLE 0x08 //bit 3 of status register is 1 when new data is available
    
    volatile uint8 flag_ISR;
    
#endif

/* [] END OF FILE */
