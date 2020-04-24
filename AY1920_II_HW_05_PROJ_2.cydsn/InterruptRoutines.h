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
    
    #define DATA_AVAILABLE 0x08
    
    volatile uint8 flag_ISR;
    uint8_t status_register;
#endif

/* [] END OF FILE */
