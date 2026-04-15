//Ellyn Tran
//Contents:
  // ADC read function + ADC code that goes in main
  // ADC convert function: raw ADC output -> integer for x,y + update xpos, ypos

  // need: xpos, ypos transposition function (so cursor doesn't jump)


//Helper Function: Reads potentiometer input to determine vertical and horizontal scroll for cursor.

#include "stdbool.h"
#include "math.h"

// -------- VARIABLES and CONSTANTS ----------- //
char cursor_x_raw = 0;
char cursor_y_raw = 0;
int xpos, ypos;
bool isVertical;

//Dimensions of grid in vertical and horizontal modes
#define XWIDTH_HOR 16
#define YHEIGHT_HOR 2
#define XWIDTH_VERT 8
#define YHEIGHT_VERT 3
#define ADC_max_ouput 255
// --------------------------------------------//

// ----------------- ADC read function ------------------------ //
// Reads one ADC channel at a time and returns an 8-bit value between 0x00 and 0xFF
//TODO: don't need channel 3 -- delete later

uint8_t ADC_Read_8bit(uint8_t channel)
{
    ADC1->SQR1 = 0;                // one conversion in sequence
    ADC1->SQR3 = channel;          // select which ADC channel to convert
    ADC1->SR = 0;                  // clear ADC status flags
    ADC1->CR2 |= (1 << 30);        // start conversion using SWSTART

    while ((ADC1->SR & (1 << 1)) == 0)
    {
        // wait until conversion is done
    }

    return (uint8_t)(ADC1->DR & 0xFF); // return 8-bit result [between 0 and 255]
}
// --------------------------------------------------------------//

// -------------------------- MAIN FUNCTION ----------------------- //

  GPIOA->MODER |= 0x000000FC; // use PA1, PA2, PA3 for final lab part

  /*** Configure ADC1 ***/
  RCC->APB2ENR |= 1<<8;  // Turn on ADC1 clock by forcing bit 8 to 1 while keeping other bits unchanged
  ADC1->CR1 = 0;          // clear ADC1 control register 1
  ADC1->CR2 = 0;          // clear ADC1 control register 2
  ADC1->SMPR2 = 0;        // clear sample time register for channels 0-9

  ADC1->CR1 |= (1 << 25); // set ADC to 8-bit resolution

  ADC1->SMPR2 |= (1 << 3); // sample time for channel 1
  ADC1->SMPR2 |= (1 << 6); // sample time for channel 2
  ADC1->SMPR2 |= (1 << 9); // sample time for channel 3

  ADC1->SQR1 = 0;         // one conversion in regular sequence
  ADC1->CR2 |= 1;        // Turn on ADC1 by forcing bit 0 to 1 while keeping other bits unchanged

  while (1)
    {
        cursor_x_raw = ADC_Read_8bit(1);     // read channel 1 for red brightness
        cursor_y_raw = ADC_Read_8bit(2);   // read channel 2 for green brightness
        // BLUE_BRT = ADC_Read_8bit(3);    // read channel 3 for blue brightness
        HAL_Delay(1);
    }

  // -------------------------------------------------------------//

  // ------------------------- ADC Converter/Update xpos and ypos --------------------- //
  //May need to change variable handling/scoping 

  //Helper function: Converts raw ADC input into integer, depending on selected range
  int Convert_Raw_ADC(char cursor_raw, int range) {
    int partition = (0xFF / range);
    return floor (cursor_raw / parition);
  }

  //Updates xpos and ypos
  void Update_XY(char cursor_x_raw, char cursor_y_raw)
  {
    //Vertical Mode: returns x between 0-7 and y between 0-2
    if (isVertical) {
      xpos = Convert_Raw_ADC(cursor_x_raw, XWIDTH_VERT);
      ypos = Convert_Raw_ADC(cursor_y_raw, YHEIGHT_VERT);
    } else {
    // Horizontal Mode: returns x between 0-15 and y between 0-1
      xpos = Convert_Raw_ADC(cursor_x_raw, XWIDTH_HOR);
      ypos = Convert_Raw_ADC(cursor_y_raw, YHEIGHT_HOR);
    }
  }