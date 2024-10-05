// main.h
// Audrey Vo
// avo@g.hmc.edu
// 10/31/22 

#ifndef MAIN_H
#define MAIN_H

#include "lib/STM32L432KC.h"
#include <stm32l432xx.h>
#include "lib/STM32L432KC_RCC.h"
#include "lib/STM32L432KC_GPIO.h"
#include "lib/STM32L432KC_FLASH.h"
#include "lib/STM32L432KC_TIM.h"
///////////////////////////////////////////////////////////////////////////////
// Custom defines
///////////////////////////////////////////////////////////////////////////////

#define LED_PIN PA5
#define BUTTON_PIN PA4
#define DELAY_TIM TIM2

#endif // MAIN_H