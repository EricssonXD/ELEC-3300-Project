/*
 * main_menu.c
 *
 *  Created on: May 5, 2024
 *      Author: ho
 */

#include "pacman.h"
#include "lcd.h"
#include "main_menu.h"

uint8_t MENUSELECTEDPREV = 1;
uint8_t MENUSELECTED = 1;
uint8_t Joined[4] = {1,0,0,0};

void SetSelectedMenu(uint8_t id, uint8_t selected);
void TogglePlayer(uint8_t id);
void HandleLCDMENU();

void ShowMainMenu(){
	LCD_Clear( 0, 0, LCD_Default_Max_COLUMN,LCD_Default_Max_PAGE , 0 );
	LCD_DrawString_Color(40, 50, "Players", BLACK, YELLOW);

	MENUSELECTEDPREV = 1;
	MENUSELECTED = 1;
	SetSelectedMenu(1, 1);
	SetSelectedMenu(2, 0);
	SetSelectedMenu(3, 0);
	SetSelectedMenu(4, 0);

	LCD_DrawString_Color(40, 280, "Start Game", 0, YELLOW);
}

uint8_t MenuHandleInput(char input){
	switch(input){
	case'2':
		if(MENUSELECTED == 1) break;
		MENUSELECTED -= 1;
		HandleLCDMENU();
		break;

	case'8':
		if(MENUSELECTED == 5) break;
		MENUSELECTED += 1;
		HandleLCDMENU();

		break;

	case'5':
		if(MENUSELECTED == 5){
			for (int i = 0; i<4; i++){
				MULTI_PACMAN_GAMEDATA.pacmans[i].joined = Joined[i];
			}
			MULTI_PACMAN_GAMEDATA.playerNum = Joined[0] + Joined[1] + Joined[2] + Joined[3];
			return 1;
		}
		TogglePlayer(MENUSELECTED);
		break;
	case 'S':
		for (int i = 0; i<4; i++){
			MULTI_PACMAN_GAMEDATA.pacmans[i].joined = Joined[i];
		}
		MULTI_PACMAN_GAMEDATA.playerNum = Joined[0] + Joined[1] + Joined[2] + Joined[3];
		return 1;
		break;
	}

	return 0;

}

void HandleLCDMENU(){

	SetSelectedMenu(MENUSELECTEDPREV, 0);
	SetSelectedMenu(MENUSELECTED, 1);


	MENUSELECTEDPREV = MENUSELECTED;
}

void TogglePlayer(uint8_t id){
	Joined[id-1] = !Joined[id-1];
	SetSelectedMenu(id, 1);

}

void SetSelectedMenu(uint8_t id, uint8_t selected){
	uint16_t background = BLACK;
	if(selected) background = BLUE;

	switch(id){
	case 1:
		if(Joined[0]){
			LCD_DrawString_Color(40, 80, "Players 1   ", background, YELLOW);
		} else{
			LCD_DrawString_Color(40, 80, "<Add Player>", background, YELLOW);
		}
		break;
	case 2:
		if(Joined[1]){
			LCD_DrawString_Color(40, 100, "Players 2   ", background, YELLOW);
		} else{
			LCD_DrawString_Color(40, 100, "<Add Player>", background, YELLOW);
		}
		break;
	case 3:
		if(Joined[2]){
			LCD_DrawString_Color(40, 120, "Players 3   ", background, YELLOW);
		} else{
			LCD_DrawString_Color(40, 120, "<Add Player>", background, YELLOW);
		}
		break;
	case 4:
		if(Joined[3]){
			LCD_DrawString_Color(40, 140, "Players 4   ", background, YELLOW);
		} else{
			LCD_DrawString_Color(40, 140, "<Add Player>", background, YELLOW);
		}
		break;
	case 5:
		LCD_DrawString_Color(40, 280, "Start Game", background, YELLOW);
		break;

	}
}
