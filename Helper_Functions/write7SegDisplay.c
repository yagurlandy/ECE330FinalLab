//Helper function -- takes 7 segment code and writes to display
/* TODO
* write to all 8 7 seg displays (not just lower 4)
* we have a function that outputs 7 seg code -- now need to write to GPIO w/ Seven_Segment
*/

void Seven_Segment_Digit (unsigned char digit, unsigned char hex_char)
{
	/*******************************************************************************
	Code to mask and bit shift 0-7 value of digit and 0-15 value of hex_char
	to output correct bit pattern to GPIO_Output
	*******************************************************************************/
        uint16_t segment_pattern;
        uint16_t digit_select_mask;

        // ensure valid range
        digit &= 7;        // digits 0–7
        hex_char &= 0x0F;  // hex values 0–F

        // invert because segments are active-low on this board
        segment_pattern = ((uint16_t) (~getSevenSegCodeFromMaps(/*TODO: add parameters :D*/))) & 0x00FF;

        // select line is PE(8+digit)
        digit_select_mask = (uint16_t)(1U << (digit + 8));

        // all selects HIGH + data present
        GPIOE->ODR = (uint16_t)(0xFF00 | segment_pattern);

        // pull selected digit LOW
        GPIOE->ODR = (uint16_t)((0xFF00 & ~digit_select_mask) | segment_pattern);

        // rising edge latch (back HIGH)
        GPIOE->ODR = (uint16_t)(0xFF00 | segment_pattern);
}


int getSevenSegCodeFromMaps(horizontal[][], vertical[][], displayNumber) {

    int sevenSegCode = 0x0; // initialize seven segment code to 0
    
    int horizontalXShift = 7-displayNumber; // find offset for x value for indexing horizontal matrix

    int verticalXShift = (displayNumber-7)*2; // find offset for x value for indexing vertical matrix


    sevenSegCode |= ((horizontal[horizontalXShift][2]) << 0); //set segment A to the value in horizontal at 'a' position for the appropriate display
    sevenSegCode |= ((vertical[1 + verticalXShift][1]) << 1); //segment b
    sevenSegCode |= ((vertical[1 + verticalXShift][0]) << 2); // segment c
    sevenSegCode |= ((horizontal[horizontalXShift][0]) << 3); // segment d
    sevenSegCode |= ((vertical[verticalXShift][0]) << 4); // segment e
    sevenSegCode |= ((vertical[verticalXShift][1]) << 5); // segment f
    sevenSegCode |= ((horizontal[horizontalXShift][1]) << 6); // segment g

    return sevenSegCode; // return code for seven segment value, now with all of the segments set appropriately, and correct select line on

}

void Seven_Segment(unsigned int sevenSegmentPattern)
{
/******************************************************************************
Use a for loop to output SevenSegmentPattern to each digit
*******************************************************************************/
	// Send hex values to lower 4 digits
    //TODO: change this

    unsigned char digit;
    for (digit = 0; digit < 8; digit++)
    {
        unsigned char seven_seg_char = (sevenSegmentPattern >> (4 * digit)) & 0x0F;
        Seven_Segment_Digit(digit, seven_seg_char);
    }
}


// ---------------------------------------------- Reference -- Code from Week 08 ---------------------------------------------//
void Seven_Segment_Digit (unsigned char digit, unsigned char hex_char)
{
	/*******************************************************************************
	Code to mask and bit shift 0-7 value of digit and 0-15 value of hex_char
	to output correct bit pattern to GPIO_Output
	*******************************************************************************/
        uint16_t segment_pattern;
        uint16_t digit_select_mask;
        // ensure valid range
        digit &= 7;        // digits 0–7
        hex_char &= 0x0F;  // hex values 0–F
        // invert because segments are active-low on this board
        segment_pattern = ((uint16_t)(~_7SEG[hex_char])) & 0x00FF;
        // select line is PE(8+digit)
        digit_select_mask = (uint16_t)(1U << (digit + 8));
        // all selects HIGH + data present
        GPIOE->ODR = (uint16_t)(0xFF00 | segment_pattern);
        // pull selected digit LOW
        GPIOE->ODR = (uint16_t)((0xFF00 & ~digit_select_mask) | segment_pattern);
        // rising edge latch (back HIGH)
        GPIOE->ODR = (uint16_t)(0xFF00 | segment_pattern);
}

void Seven_Segment(unsigned int HexValue)
{
/******************************************************************************
Use a for loop to output HexValue to 7 segment display digits
*******************************************************************************/
	// Send hex values to lower 4 digits
    unsigned char digit;
    for (digit = 0; digit < 8; digit++)
    {
        unsigned char hex_char = (HexValue >> (4 * digit)) & 0x0F;
        Seven_Segment_Digit(digit, hex_char);
    }
}