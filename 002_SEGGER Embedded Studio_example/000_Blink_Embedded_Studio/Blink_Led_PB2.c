#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>

int main(void)
{
    systick_config();    
    rcu_periph_clock_enable(RCU_GPIOB);    
    /* GPIOB output */
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    
    while(1){
        // gpio_bit_set(GPIOB, GPIO_PIN_2);
        GPIO_BOP(GPIOB) |= (1 << 2);
        delay_1ms(500);
        //gpio_bit_reset(GPIOB, GPIO_PIN_2);
        GPIO_BC(GPIOB)  |= (1 << 2);
        delay_1ms(500);
    }
}

/*************************** End of file ****************************/
