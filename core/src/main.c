
#include "main.h"
#include <stdlib.h>
#include <stm32f2xx.h>
#include <stm32f207xx.h>


int main(void) {
    initClockingHSE();
    initGPIO();
    
    while (1) {
        GPIOB->ODR ^= (1 << 9);
        wait(1700000);
    }
}

void init(void) {

}

void initClockingHSE(void){
    //включение HSE (кварцевый генератор)
    RCC->CR |= RCC_CR_HSEON_Msk;
    while((RCC->CR & RCC_CR_HSERDY_Msk) != 0) {}    //ожидание стабилизации
    //включение буфера предварительной выборки
    FLASH->ACR &= ~FLASH_ACR_PRFTEN_Msk;
    FLASH->ACR |= FLASH_ACR_PRFTEN_Msk;
    //настройка количества состояний ожидания (rm - 2.3.4 Flash memory read interface)
    FLASH->ACR |= (FLASH_ACR_LATENCY_0WS << FLASH_ACR_LATENCY_Pos);

    // отключение возможности использования внешнего генератора
    RCC->CR &= ~RCC_CR_HSEBYP_Msk;
    
    //Настройка значений делителей после system clock mux
    RCC->CFGR |= (RCC_CFGR_PPRE1_Msk & RCC_CFGR_PPRE1_DIV1);    //APB Low speed prescaler (APB1)
    RCC->CFGR |= (RCC_CFGR_PPRE2_Msk & RCC_CFGR_PPRE2_DIV1);    //APB high-speed prescaler (APB2)
    RCC->CFGR |= (RCC_CFGR_HPRE_Msk & RCC_CFGR_HPRE_DIV1);      //AHB prescaler

    //выбор источника тактирования 
    RCC->CFGR |= (RCC_CFGR_SW_HSE << RCC_CFGR_SW_Pos);
    while (((RCC->CR & RCC_CFGR_SWS_Msk) >> RCC_CFGR_SWS_Pos) != 0) {}

    //отключение PLL
    RCC->CR &= ~RCC_CR_PLLON_Msk;
    // while((RCC->CR & RCC_CR_PLLRDY_Msk) != 0) {}
    //отключение PLLI2S
    RCC->CR &= ~RCC_CR_PLLI2SON_Msk;
    // while((RCC->CR & RCC_CIR_PLLI2SRDYC_Msk) != 0) {}
    //отключение внутреннего генератора
    RCC->CR &= ~RCC_CR_HSION_Msk;
    // while ((RCC->CR & RCC_CR_HSIRDY_Msk) != 0) {}

    //сброс флагов всех прерываний от RCC
    RCC->CSR |= RCC_CSR_RMVF_Msk;
    //запрет прерываний от RCC
    RCC->CIR = 0;
}

void initGPIO(void) {
    //тактирование GPIO (AHB1)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    //обнуление значений режима работы вывода
    GPIOB->MODER &= ~GPIO_MODER_MODER9_Msk;
    GPIOB->MODER |= (0x01 << GPIO_MODER_MODER9_Pos); //General purpose output mode
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT9; // 
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD9); //
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR9_0; //
}

void wait(uint32_t val) {
    volatile uint32_t index = 0;
    while (index < val) {  index++;    }
}
 
