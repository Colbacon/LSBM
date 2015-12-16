
//Codes that represent every own character created
#define CHAR0_CODE   "\x00"
#define CHAR1_CODE   "\x01" 
#define CHAR2_CODE   "\x02" 
#define CHAR3_CODE   "\x03" 
#define CHAR4_CODE   "\x04"  
#define CHAR5_CODE   "\x05" 
#define CHAR6_CODE   "\x06" 
#define CHAR7_CODE   "\x07" 

//8 bits that represent the instruction of IR that changes the AC addres to point
//to the CGRAM
#define CHAR0_ADDR  0x40
#define CHAR1_ADDR  0X48
#define CHAR2_ADDR  0X50
#define CHAR3_ADDR  0X58
#define CHAR4_ADDR  0X60
#define CHAR5_ADDR  0X68
#define CHAR6_ADDR  0X70
#define CHAR7_ADDR  0X78


struct MyChar{
    char addr;
    char pattern [8];
};


// Initialize LCD
void LCDInit();

// Clear LCD
void LCDClear();

// Move the cursor of the LCD to home
void LCDMoveHome();
// Move LCD cursor to first or second line.
void LCDMoveFirstLine();
void LCDMoveSecondLine();

// Writes a string to the alphanumeric LCD
void LCDPrint(char *str);

// Move LCD cursor left or right
void LCDMoveLeft();
void LCDMoveRight();

// Scroll LCD Screen left or right
void LCDScrollLeft();
void LCDScrollRight();

// Hide LCD cursor
void LCDHideCursor();
// Turn on underline LCD cursor
void LCDTurnOnUnderline();
// Turn on blinking-block LCD cursor
void LCDTurnOnBlinking();

//Write a own character on CGRAM
void LCDStoreCharPattern(struct MyChar ch);