//Andy's code -- from Week 08

static int pressed_once(uint16_t pin_mask)
{
    /* Buttons are normally HIGH and go LOW when pressed */
    if ((GPIOC->IDR & pin_mask) == 0)
    {
        HAL_Delay(20); // debounce
        if ((GPIOC->IDR & pin_mask) == 0)
        {
            while ((GPIOC->IDR & pin_mask) == 0) { } // wait release
            HAL_Delay(20);
            return 1;
        }
    }
    return 0;
}