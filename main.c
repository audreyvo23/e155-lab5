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



/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/


#define ALT_FUNC 6


// Function used by printf to send characters to the laptop
int _write(int file, char *ptr, int len) {
  int i = 0;
  for (i = 0; i < len; i++) {
    ITM_SendChar((*ptr++));
  }
  return len;
}

int deltat = 0;
int speed;
int direction;

int main(void) {
    // Enable button as input
    gpioEnable(GPIO_PORT_A);
    pinMode(BUTTON_PIN, GPIO_INPUT);
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD2, 0b01); // Set PA2 as pull-up

   // Initialize timer
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initTIM(DELAY_TIM);

    // TODO
    // 1. Enable SYSCFG clock domain in RCC
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // 2. Configure EXTICR for the input button interrupt
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI2, 0b000); // Select PA2

    // Enable interrupts globally
    __enable_irq();

    // TODO: Configure interrupt for falling edge of GPIO pin for button
    // 1. Configure mask bit
    EXTI->IMR1 |= (1 << gpioPinOffset(BUTTON_PIN)); // Configure the mask bit
    // 2. Disable rising edge trigger
    EXTI->RTSR1 &= ~(1 << gpioPinOffset(BUTTON_PIN));// Disable rising edge trigger
    // 3. Enable falling edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(BUTTON_PIN));// Enable falling edge trigger
    // 4. Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI2_IRQn);

    while(1){   
        delay_millis(TIM2, 200);
    }
   

int i;


  for (i = 0; i < 100; i++) {
          // printf function call
    printf("Hello World %d!\n", i);
  }
  do {
    i++;
  } while (1);
}

// interrupt for A
void EXTI5_IRQHandler(void){
    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << 5)){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << 5);
        TIM2->CNT = 0;
        TIM2->EGR |= (1 << 0);
        deltat = TIM2->CNT;
       // Then toggle the LED
        togglePin(LED_PIN);

    }
}

// interrupt for b
void EXTI6_IRQHandler(void){
    // Check that the button was what triggered our interrupt
    if (EXTI->PR1 & (1 << 6)){
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << 6);
        deltat = TIM2->CNT;
        int freq = 1/(deltat*4);
        speed = freq/120;
        TIM2->CNT = 0;
        TIM2->EGR |= (1 << 0);
        // Then toggle the LED
        togglePin(LED_PIN);

    }
}





/*************************** End of file ****************************/
