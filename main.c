/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include "main.h"
#include <stdio.h>
#include "stm32l432xx.h"


// Audrey Vo
// avo@g.hmc.edu
// 10/31/22 
/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/



// Function used by printf to send characters to the laptop
int _write(int file, char *ptr, int len) {
  int i = 0;
  for (i = 0; i < len; i++) {
    ITM_SendChar((*ptr++));
  }
  return len;
}

int deltat;
int cw;


int main(void) {
    // Enable button as input
    gpioEnable(GPIO_PORT_A);
    pinMode(ASIGNAL, GPIO_INPUT);
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD5, 0b01); // Set PA5 as pull-up

    pinMode(BSIGNAL, GPIO_INPUT);
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD6, 0b01); // Set PA6 as pull-up

   // Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initTIM(DELAY_TIM);

    // Initialize timer
    // enable timer 16 (pg 244)
    RCC->APB2ENR |= (1 << 17);
    initTIM2(COUNTER_TIM);

    // TODO
    // 1. Enable SYSCFG clock domain in RCC
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // 2. Configure EXTICR for the input button interrupt
   // SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI2, 0b000); // Select PA2
    SYSCFG->EXTICR[2] |= _VAL2FLD(SYSCFG_EXTICR2_EXTI5, 0b000); // Select PA5
    SYSCFG->EXTICR[2] |= _VAL2FLD(SYSCFG_EXTICR2_EXTI6, 0b000); // Select PA6

    // Enable interrupts globally
    __enable_irq();

    // TODO: Configure interrupt for falling edge of GPIO pin for button
    // 1. Configure mask bit
    EXTI->IMR1 |= (1 << gpioPinOffset(ASIGNAL)); // Configure the mask bit
    // 2. Disable rising edge trigger
    EXTI->RTSR1 &= ~(1 << gpioPinOffset(ASIGNAL));// Disable rising edge trigger
    // 3. Enable falling edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(ASIGNAL));// Enable falling edge trigger
    // 4. Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI9_5_IRQn);

    // TODO: Configure interrupt for falling edge of GPIO pin for button
    // 1. Configure mask bit
    EXTI->IMR1 |= (1 << gpioPinOffset(BSIGNAL)); // Configure the mask bit
    // 2. Disable rising edge trigger
    EXTI->RTSR1 &= ~(1 << gpioPinOffset(BSIGNAL));// Disable rising edge trigger
    // 3. Enable falling edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(BSIGNAL));// Enable falling edge trigger
    // 4. Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI9_5_IRQn);

float speed;
float period;
int i;
   while(1){  
    if (cw) {
    period = (4.0*deltat/1000000.0);
    speed = 1.0/(120.0*period); 
    printf("clockwise \n");
   }
    else {
    period = ((4.0/3.0)*(deltat/1000000.0));
    speed = 1.0/(120.0*period); 
    printf("counter-clockwise \n");
    } 
      // printf function call
    printf("Speed:  %f\n", speed);
    delay_millis(TIM2, 200);

}
}

// interrupt for A
void EXTI9_5_IRQHandler(void){

    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << 5)){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << 5);
        TIM16->CNT = 0;
        TIM16->EGR |= (1 << 0);
        // if b is high, set direction to clockwise +
        if (digitalRead(PA6)) {cw = 1;}
        // if b is low set, direction to counterclockwise -
        else {cw = 0;}
    }
     // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << 6)){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << 6);
     
       
        deltat = TIM16->CNT;
      
        // reset counter and update register
        TIM16->CNT = 0;
        TIM16->EGR |= (1 << 0);
       
    }
}







/*************************** End of file ****************************/
