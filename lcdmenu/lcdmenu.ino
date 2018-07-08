//myMenu
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Menu
String menu_name = "myMenu";
String menu_feature[4][2] = { {"1 Blue", "Inside Blue"}, {"2 Orange", "Inside Orange"}, {"3 Red", "Inside Red"}, {"4 Yellow", "Inside Yellow"} };
String button_name[5] = { "Left", "Right", "Up", "Down", "Select" };
int menu_page_curr = 1;
int menu_page_prev = 1;
int menu_page_total = 4;

//Keypad buttons
#define BTN_UP 200
#define BTN_DOWN 400
#define BTN_LEFT 600
#define BTN_RIGHT 60
#define BTN_SELECT 800

//Other
int button_type;
int animation_speed = 250;
int button_delay = 250;

//Custom characters
byte chr_arrowleft[8] = {
	0x00, 0x04, 0x08, 0x1F,
	0x08, 0x04, 0x00, 0x00
};
byte chr_arrowright[8] = {
	0x00, 0x04, 0x02, 0x1F,
	0x02, 0x04, 0x00, 0x00
};
byte chr_arrowup[8] = {
	0x00, 0x04, 0x0E, 0x15,
	0x04, 0x04, 0x00, 0x00
};
byte chr_arrowdown[8] = {
	0x00, 0x04, 0x04, 0x15,
	0x0E, 0x04, 0x00, 0x00
};
byte chr_slash[8] = {
	0x00, 0x01, 0x02, 0x04,
	0x08, 0x10, 0x00, 0x00
};
byte chr_indleft[8] = {
	0x00, 0x04, 0x08, 0x10,
	0x08, 0x04, 0x00, 0x00
};
byte chr_indright[8] = {
	0x00, 0x04, 0x02, 0x01,
	0x02, 0x04, 0x00, 0x00
};
byte chr_plus[8] = {
	0x00, 0x04, 0x04, 0x1F,
	0x04, 0x04, 0x00, 0x00
};
byte chr_minus[8] = {
	0x00, 0x00, 0x00, 0x1F,
	0x00, 0x00, 0x00, 0x00
};

/*
	Prints text to display
	desc > Will wrap to second display line
		   if text is bigger than 16 chars.
		   Doesn't work for text with more
		   than 32 chars (lcd resets).
	text: String > Text to print
	index: int > Index to print text at
	line: int > Line to print text at
*/
void printStr(String text, int index, int line) {
	if(text.length() > 16) {
		lcd.setCursor(0, 0);
		lcd.print(text);
		lcd.setCursor(0, 1);
		lcd.print(text.substring(17, text.length()));
	}
	else {
		lcd.setCursor(index, line);
		lcd.print(text);
	}
}

/*
	Prints custom character to display
	character: int > Text to print
	index: int > Index to print character at
	line: int > Line to print character at
*/
void printChr(int character, int index, int line) {
	lcd.setCursor(index, line);
	lcd.write(character);
}

/*
	Clears a desired display line
	line: int > Line to clear
*/
void clearLine(int line) {
	lcd.setCursor(0, line);
	lcd.print("                ");
}

//Initializes custom characters
void initChars() {
	lcd.createChar(0, chr_arrowleft);
	lcd.createChar(1, chr_arrowright);
	lcd.createChar(2, chr_arrowup);
	lcd.createChar(3, chr_arrowdown);
	lcd.createChar(4, chr_slash);
	lcd.createChar(5, chr_indleft);
	lcd.createChar(6, chr_indright);
	lcd.createChar(7, chr_plus);
	lcd.createChar(8, chr_minus);
}

//Initializes the display
void initLCD() {
	lcd.clear();
	initChars();
	lcd.begin(16, 2);
	printStr(menu_name, 0, 0);
}

/*
	Prints loading animation
	desc > Supports user defined string for
		   up to 13 characters.
	text: String > Text to print
*/
void loadingAnimation(String text) {
	int text_length = text.length();

	if (text_length > 13) {
		printStr(String("Loading"), 0, 1);
		delay(animation_speed*4);
	}
	else {
		while (true) {

			printStr(text, 0, 1);
			delay(animation_speed);

			printStr(text, 0, 1);
			printStr(String("."), text_length, 1);
			delay(animation_speed);

			printStr(text, 0, 1);
			printStr(String(".."), text_length, 1);
			delay(animation_speed);

			printStr(text, 0, 1);
			printStr(String("..."), text_length, 1);
			delay(animation_speed);

			clearLine(1);
			break;
		}
	}
}

//Menu features
void printMenu() {
	clearLine(1);
	printChr(2, 13, 1);
	printChr(4, 14, 1);
	printChr(3, 15, 1);
	switch (menu_page_curr) {
		case 1:
			printStr(menu_feature[0][0], 0, 1);
			break;
		case 2:
			printStr(menu_feature[1][0], 0, 1);
			break;
		case 3:
			printStr(menu_feature[2][0], 0, 1);
			break;
		case 4:
			printStr(menu_feature[3][0], 0, 1);
			break;
	}
}

//Menu feature settings
void printFeatures() {
	clearLine(1);
	printChr(8, 15, 1);
	switch (menu_page_curr) {
		case 1:
			printStr(menu_feature[0][1], 0, 1);
			break;
		case 2:
			printStr(menu_feature[1][1], 0, 1);
			break;
		case 3:
			printStr(menu_feature[2][1], 0, 1);
			break;
		case 4:
			printStr(menu_feature[3][1], 0, 1);
			break;
	}
}

//Menu logic
void menuLogic() {
	if (button_type < BTN_DOWN) {
		menu_page_curr++;
		if (menu_page_curr > menu_page_total) {
			menu_page_curr = 1;
		}
		delay(button_delay);
	} 
	else if (button_type < BTN_UP) {
		menu_page_curr--;
		if (menu_page_curr == 0) {
			menu_page_curr = menu_page_total;
		}
		delay(button_delay);
	}
	else if (button_type < BTN_SELECT) {
		printFeatures();
		delay(button_delay);
	}
	else if (button_type < BTN_LEFT) {
		//printStr(button_name[0], 0, 1);
		//delay(button_delay);
	} 
	else if (button_type < BTN_RIGHT) {
		//printStr(button_name[1], 0, 1);
		//delay(button_delay);
	}

	if (menu_page_curr != menu_page_prev) {
		printMenu();
		menu_page_prev = menu_page_curr;
	}
}

void setup() {
	initLCD();

	loadingAnimation(String("Setup"));

	printMenu();
}

void loop() {
	button_type = analogRead(0);

	menuLogic();
}