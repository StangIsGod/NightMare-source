//Menu base created by 2much4u

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <stddef.h>
#include <sys/socket.h>
#include "netdb.h"
#include "Network.h"
#include "WebIN.h"
#include <spu_printf.h>
#include <sys/process.h>
#include <cell/cell_fs.h>
#include <sys/ss_get_open_psid.h>
#include <arpa/inet.h>

#include <sys/spu_initialize.h>
#include <cell/dbgfont.h>
#include <cell/sysmodule.h>
#include <cell/pad/libpad.h>
#include <sysutil/sysutil_sysparam.h>
#include <PSGL/psglu.h>

#include <cellstatus.h>
#include <sys/prx.h>
#include <sys/ppu_thread.h>
#include <string.h>
#include <sys/memory.h>
#include <sys/syscall.h>
#include <cell/cell_fs.h>

#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

#include <stdarg.h>
#include <stdio.h>
#include <cstdlib>
#include "Enums.h"
#include "Natives.h"
#include "Dialog.h"
#include "Functions.h"


SYS_MODULE_INFO("GTAV_Native_Invoker", 0, 1, 1);
SYS_MODULE_START(PRX_ENTRY);



#pragma region------Internals------
void HookNative(int native, int dest) {
	int FuncBytes[1];
	FuncBytes[0] = *(int*)dest;
	memcpy((void*)native, FuncBytes, 4);
}
int write_process(void* destination, const void* source, size_t size)
{
	system_call_4(905, (uint64_t)sys_process_getpid(), (uint64_t)destination, size, (uint64_t)source);
	return_to_user_prog(int);
}
void PatchInJump(uint64_t Address, int Destination, bool Linked)
{
	int FuncBytes[4];													// Use this data to copy over the address.
	Destination = *(int*)Destination;									// Get the actual destination address (pointer to void).
	FuncBytes[0] = 0x3D600000 + ((Destination >> 16) & 0xFFFF);			// lis %r11, dest>>16
	if (Destination & 0x8000)											// if bit 16 is 1...
		FuncBytes[0] += 1;
	FuncBytes[1] = 0x396B0000 + (Destination & 0xFFFF);					// addi %r11, %r11, dest&OxFFFF
	FuncBytes[2] = 0x7D6903A6;											// mtctr %r11
	FuncBytes[3] = 0x4E800420;											// bctr
	if (Linked)
		FuncBytes[3] += 1;												// bctrl
	write_process((void*)Address, FuncBytes, 4 * 4);
	//memcpy((void*)Address, FuncBytes, 4 * 4);
}
unsigned int FindNativeTableAddress()
{
	unsigned int  l_uiNativeTableAddress = 0;
	unsigned int l_uiStartAddress = 0x390000;
	unsigned int l_uiAddress = 0;
	for (unsigned int i = 0; i < 0x10000; i++)
	{
		if (*(unsigned int*)(l_uiStartAddress + i) == 0x3C6072BD)
			if (*(unsigned int*)(l_uiStartAddress + i + 8) == 0x6063E002)
			{
				l_uiAddress = *(unsigned int*)(l_uiStartAddress + i + 0x10);
				l_uiAddress &= 0xFFFFFF;
				l_uiAddress += (l_uiStartAddress + i + 0x10) - 1;
				printf("address: 0x%08x\n", l_uiAddress);
				break;
			}
	}
	l_uiNativeTableAddress = (*(unsigned int*)(l_uiAddress + 0x24) << 16) + (*(unsigned int*)(l_uiAddress + 0x2C) & 0xFFFF);
	l_uiNativeTableAddress -= 0x10000;
	return l_uiNativeTableAddress;
}
int NativeAddress(int Native, bool PatchInJump = true)
{
	int Hash = Native & 0xFF;
	int Table = *(int*)(FindNativeTableAddress() + (Hash * 4));
	while (Table != 0)
	{
		int NativeCount = *(int*)(Table + 0x20);
		for (int i = 0; i < NativeCount; i++)
		{
			if (*(int*)((Table + 0x24) + (i * 4)) == Native)
			{
				if (PatchInJump)
				{
					int NativeLocation = *(int*)(*(int*)((Table + 4) + (i * 4)));
					for (int i = 0; i < 50; ++i)
					{
						short CurrentPlace = *(short*)(NativeLocation + (i * 4));
						if (CurrentPlace == 0x4AE6 || CurrentPlace == 0x4AA8 || CurrentPlace == 0x4AE4 || CurrentPlace == 0x4AE5)
						{
							return (((*(int*)(NativeLocation + (i * 4))) - 0x48000001) + (NativeLocation + (i * 4))) - 0x4000000;
						}
					}
				}
				else
				{
					return *(int*)((Table + 4) + (i * 4));
				}

			}
		}
		Table = *(int*)(Table);
	}
}
void sleep(usecond_t time)  //1 second = 1000
{
	sys_timer_usleep(time * 1000);
}
void set_text_component(char *text)
{
	GRAPHICS::_0x3AC9CB55("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	GRAPHICS::_0x386CE0B8();
}
void instructionsSetup()
{
	mov = GRAPHICS::REQUEST_SCALEFORM_MOVIE("instructional_buttons");
	GRAPHICS::_0x7B48E696(mov, 255, 255, 255, 0);
	GRAPHICS::_0x215ABBE8(mov, "CLEAR_ALL");
	GRAPHICS::_0x02DBF2D7();
	GRAPHICS::_0x215ABBE8(mov, "SET_CLEAR_SPACE");
	GRAPHICS::_0x716777CB(200);
	GRAPHICS::_0x02DBF2D7();
	instructCount = 0;
}
void addInstruction(int button, char *text)
{
	if (!instructionsSetupThisFrame)
	{
		instructionsSetup();
		instructionsSetupThisFrame = true;
	}
	GRAPHICS::_0x215ABBE8(mov, "SET_DATA_SLOT");
	GRAPHICS::_0x716777CB(instructCount);
	GRAPHICS::_0x716777CB(button);
	set_text_component(text);
	GRAPHICS::_0x02DBF2D7();
	instructCount++;
}
void instructionsClose()
{
	GRAPHICS::_0x215ABBE8(mov, "DRAW_INSTRUCTIONAL_BUTTONS");
	GRAPHICS::_0x02DBF2D7();
	GRAPHICS::_0x215ABBE8(mov, "SET_BACKGROUND_COLOUR");
	GRAPHICS::_0x716777CB(0);
	GRAPHICS::_0x716777CB(0);
	GRAPHICS::_0x716777CB(0);
	GRAPHICS::_0x716777CB(80);
	GRAPHICS::_0x02DBF2D7();
}

void playSound(char* sound)
{
	if (menuSounds)
		AUDIO::PLAY_SOUND_FRONTEND(-1, sound, "HUD_FRONTEND_DEFAULT_SOUNDSET");
}

void startKeyboard(int action, char *defaultText, int maxLength)
{
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(0, "FMMC_KEY_TIP8", "", defaultText, "", "", "", maxLength);
	keyboardAction = action;
	keyboardActive = true;
}
void changeSubmenu(int newSubmenu)
{
	lastSubmenu[submenuLevel] = submenu;
	lastOption[submenuLevel] = currentOption;
	currentOption = 1;
	submenu = newSubmenu;
	submenuLevel++;
}

void addTitle(char *title)
{
	drawText("NightMare", 4, menuXCoord, 0.09f, 0.8f, 0.8f, 255, 0, 0, bannerTextOpacity, true);
	drawText(title, optionsFont, textXCoord, infobox, 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
	GRAPHICS::DRAW_RECT(menuXCoord, 0.1175f, MenuXSize, 0.083f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity);
	DrawSprite1("commonmenu", "header_gradient_script", menuXCoord, Ycoords, MenuXSize, sizeY, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
	DrawSprite1("commonmenu", "header_gradient_script", menuXCoord, Ycoords, MenuXSize, sizeY, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
	DrawSprite1("commonmenu", "header_gradient_script", menuXCoord, Ycoords, MenuXSize, sizeY, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
	DrawSprite1("commonmenu", "header_gradient_script", menuXCoord, Ycoords, MenuXSize, sizeY, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
	DrawSprite1("commonmenu", "header_gradient_script", menuXCoord, Ycoords, MenuXSize, sizeY, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
	DrawSprite1("commonmenu", "header_gradient_script", menuXCoord, Ycoords, MenuXSize, sizeY, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
}
#pragma region Option Helpers
void addOption(char *option, char *info = NULL)
{
	char unti[300];

	optionCount++;
	if (currentOption == optionCount)
	{
		sprintf(unti, "%s", option);
		infoText = info;
	}
	else
	{
		sprintf(unti, "%s", option);
	}
	if (currentOption <= maxOptions && optionCount <= maxOptions)
		drawText(unti, optionsFont, textXCoord, (optionCount * menuYCoords + infobox), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
	else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
		drawText(unti, optionsFont, textXCoord, ((optionCount - (currentOption - maxOptions)) * menuYCoords + infobox), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
}
void DoAdminations(char *option, char *dict, char* anim)
{
	addOption(option);
	if (currentOption == optionCount && optionPress)
	{
		int targ_ped = PLAYER::GET_PLAYER_PED(selectedPlayer);
		STREAMING::REQUEST_ANIM_DICT(dict);
		if (STREAMING::HAS_ANIM_DICT_LOADED(dict))
		{
			Vector3 coords = ENTITY::GET_ENTITY_COORDS(selectedPlayer, 1);
			int sceneID = NETWORK::NETWORK_CREATE_SYNCHRONISED_SCENE_3(coords.x, coords.y, coords.z, 0, 0, 0, 2, 0, 1, 1.0f, 0.0f, 1.0f);
			NETWORK::NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE_3(targ_ped, sceneID, dict, anim, 8.0f, -8.0f, 120, 0, 100, 1);
			NETWORK::NETWORK_START_SYNCHRONISED_SCENE(sceneID);
		}
	}
}
void addNeonsOption(char *option, int R, int G, int B)
{
	addOption(option);
	if (currentOption == optionCount && optionPress)
	{
		NeonR = R;
		NeonG = G;
		NeonB = B;
	}
}
#pragma region addPlayerList
void addPlayerList()
{
	char *MYNAME = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_PED_ID());
	for (int i = 0; i < 18; i++)
	{
		char Friend[100];
		char My[100];
		char Normal[100];
		char FriendCount[100];
		char NormalCount[100];
		char MyCount[100];
		char *NAME = PLAYER::GET_PLAYER_NAME(i);
		int playerid = PLAYER::GET_PLAYER_PED(i);

		if (playerid != PLAYER::PLAYER_PED_ID())
		{
			if (NETWORK::NETWORK_IS_FRIEND_ONLINE2(NAME))
			{
				switch (PLAYER::GET_PLAYER_MAX_ARMOUR(i))
				{
				case 1220:sprintf(Friend, "~r~[NMRA]~f~[フレンド]~r~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2055:sprintf(Friend, "~r~[NMRS]~f~[フレンド]~r~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2001:sprintf(Friend, "~r~[PE]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2870:sprintf(Friend, "~g~[GC]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2871:sprintf(Friend, "~g~[GCP]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2872:sprintf(Friend, "~g~[GCA]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2900:sprintf(Friend, "~f~[HDC]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2901:sprintf(Friend, "~f~[HG]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2902:sprintf(Friend, "~f~[HGP]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2903:sprintf(Friend, "~f~[HGA]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2585:sprintf(Friend, "~o~[HNTz]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 3000:sprintf(Friend, "~d~[GNXKS]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2002:sprintf(Friend, "~d~[GNXKSv2]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 1996:sprintf(Friend, "~d~[POWER S]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2777:sprintf(Friend, "~o~[Xenoverse]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 1109:sprintf(Friend, "~d~[TKMN]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2599:sprintf(Friend, "~d~[TKMN]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2000:sprintf(Friend, "~d~[TKMN]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2009:sprintf(Friend, "~m~[Zedd]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 3025:sprintf(Friend, "~m~[Paix]~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				default:sprintf(Friend, "~f~[フレンド]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;

				}
				sprintf(FriendCount, "%i", i);
				addOption(Friend, FriendCount);
			}
			else
			{
				switch (PLAYER::GET_PLAYER_MAX_ARMOUR(i))
				{
				case 1220:sprintf(Normal, "~r~[NMRA]~r~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2055:sprintf(Normal, "~r~[NMRS]~r~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2001:sprintf(Normal, "~r~[PE]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2870:sprintf(Normal, "~g~[GC]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2871:sprintf(Normal, "~g~[GCP]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2872:sprintf(Normal, "~g~[GCA]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2900:sprintf(Normal, "~f~[HDC]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2901:sprintf(Normal, "~f~[HG]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2902:sprintf(Normal, "~f~[HGP]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2903:sprintf(Normal, "~f~[HGA]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2585:sprintf(Normal, "~o~[HNTz]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 3000:sprintf(Normal, "~d~[GNXKS]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2002:sprintf(Normal, "~d~[GNXKSv2]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 1996:sprintf(Normal, "~d~[POWER S]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2600:sprintf(Normal, "[FINE S]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2777:sprintf(Normal, "~o~[Xenoverse]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 1109:sprintf(Normal, "~d~[TKMN]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2599:sprintf(Normal, "~d~[TKMN]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2000:sprintf(Normal, "~d~[TKMN]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 2009:sprintf(Normal, "~m~[Zedd]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				case 3025:sprintf(Normal, "~m~[Paix]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
				default:sprintf(Normal, "%s", PLAYER::GET_PLAYER_NAME(i)); break;
				}
				sprintf(NormalCount, "%i", i);
				addOption(Normal, NormalCount);
			}

		}
		else
		{
			switch (PLAYER::GET_PLAYER_MAX_ARMOUR(i))
			{
			case 1220:sprintf(My, "~r~[NMRA]~y~[自分]~r~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2055:sprintf(My, "~r~[NMRS]~y~[自分]~r~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2001:sprintf(My, "~w~[PE]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2870:sprintf(My, "~g~[GC]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2871:sprintf(My, "~g~[GCP]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2872:sprintf(My, "~g~[GCA]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2901:sprintf(My, "~f~[HG]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2902:sprintf(My, "~f~[HGP]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2903:sprintf(My, "~f~[HGA]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2585:sprintf(My, "~o~[HNTz]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 3000:sprintf(My, "~d~[GNXKS]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2002:sprintf(My, "~d~[GNXKSv2]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 1996:sprintf(My, "~d~[POWER s]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2600:sprintf(My, "[FINE S]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2777:sprintf(My, "~o~[Xenoverse]~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 1109:sprintf(My, "~d~[TKMN]~f~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2599:sprintf(My, "~d~[TKMN]~f~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2000:sprintf(My, "~d~[TKMN]~f~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 2009:sprintf(My, "~m~[Zedd]~f~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			case 3025:sprintf(My, "~m~[Paix]~f~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			default:sprintf(My, "~y~[自分]~w~%s", PLAYER::GET_PLAYER_NAME(i)); break;
			}
			sprintf(MyCount, "%i", i);
			addOption(My, MyCount);
		}
	}
}
#pragma endregion

void addSubmenuOption(char *option, int newSubmenu, char *info = NULL)
{
	addOption(option, info);
	if (currentOption == optionCount)
	{
		DrawSprite1("commonmenu", "arrowright", Checkbox, (optionCount * menuYCoords + infobox + 0.018), 0.025, 0.025, 0, /*R*/255, 255, 0, 255);
		xInstruction = true;
		if (optionPress)
			changeSubmenu(newSubmenu);
	}
	else if (currentOption <= maxOptions && optionCount <= maxOptions)
		DrawSprite1("commonmenu", "arrowright", Checkbox, (optionCount * menuYCoords + infobox + 0.018), 0.025, 0.025, 0, /*R*/255, 255, 255, 255);
	else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
		DrawSprite1("commonmenu", "arrowright", Checkbox, (optionCount * menuYCoords + infobox + 0.018), 0.025, 0.025, 0, /*R*/255, 255, 255, 255);
}
void addCheckBool(char *option, bool b00l1, char *info = NULL)
{
	char buf[60];
	if (b00l1)
	{
		snprintf(buf, sizeof(buf), "%s", option);
		if (currentOption <= maxOptions && optionCount <= maxOptions)
			DrawSprite1("commonmenu", "shop_box_tick", Checkbox, (optionCount * menuYCoords + infobox + 0.045), 0.025, 0.04, 0, /*R*/255, 255, 255, 255);
		//"Shop_Health_Icon_A"
		//DrawSprite("commonmenu", "shop_box_tick", Checkbox, (optionCount * 0.035f + 0.175f), 0.025, 0.04, 0, /*R*/255, 255, 255, 255);
		else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
			DrawSprite1("commonmenu", "shop_box_tick", Checkbox, (optionCount * menuYCoords + infobox + 0.045), 0.025, 0.04, 0, /*R*/255, 255, 255, 255);
		addOption(buf, info);
	}
	else
	{
		snprintf(buf, sizeof(buf), "%s", option);
		if (currentOption <= maxOptions && optionCount <= maxOptions)
			DrawSprite("commonmenu", "shop_box_blank", Checkbox, (optionCount * menuYCoords + infobox + 0.045), 0.025, 0.04, 0, /*R*/0, 0, 0, 255);
		else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
			DrawSprite("commonmenu", "shop_box_blank", Checkbox, (optionCount * menuYCoords + infobox + 0.045), 0.025, 0.04, 0, /*R*/0, 0, 0, 255);
		addOption(buf, info);
	}
}
void WechselBoolFunction(char *optionOFF, char *OptionOn, bool b00l, int *var, int min, int max, char **From, char *info = NULL)
{
	char buf[300];

	if (b00l)
	{
		char buf[300];
		snprintf(buf, sizeof(buf), "%s: ~r~%s~w~ ", OptionOn, From[*var]);
		addOption(buf, info);
		if (currentOption == optionCount)
		{
			lrInstruction = true;
			if (rightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + 1;

			}
			else if (leftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - 1;

			}
		}
	}
	else
	{
		snprintf(buf, sizeof(buf), "%s: ~r~Off", optionOFF);
		addOption(buf, info);
	}
}
void SpawnCarOption(char *option, char *veh, int *var, int max, char *info = NULL)
{

	char buffff[200];
	optionCount++;
	if (currentOption == optionCount)
	{
		sprintf(buffff, "%s", option);
	}
	else
	{
		sprintf(buffff, "%s", option);
	}

	if (currentOption <= maxOptions && optionCount <= maxOptions)
	{
		char buf23[60];
		drawText(buffff, optionsFont, textXCoord, (optionCount * menuYCoords + infobox), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
		drawText(ItoS(*var), optionsFont, 0.905f, (optionCount * menuYCoords + infobox), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
		drawText(ItoS(max), optionsFont, 0.925f, (optionCount * menuYCoords + infobox), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
		//sprintf(buf23, "%d | %d", var,max);
		//drawText(buf23, optionsFont, (textXCoord + 0.170), (optionCount * 0.035f + 0.125f), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
	}

	else if ((optionCount > (currentOption - maxOptions)) && optionCount <= currentOption)
	{
		char buf23[60];
		drawText(buffff, optionsFont, textXCoord, ((optionCount - (currentOption - maxOptions)) * menuYCoords + infobox), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
		drawText(ItoS(*var), optionsFont, 0.905f, ((optionCount - (currentOption - maxOptions)) * menuYCoords + infobox), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
		drawText(ItoS(max), optionsFont, 0.925f, ((optionCount - (currentOption - maxOptions)) * menuYCoords + infobox), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
		//sprintf(buf23, "%i / %i", var, max);
		//drawText(buf23, optionsFont, (textXCoord + 0.2), ((optionCount - (currentOption - maxOptions)) * 0.035f + 0.125f), 0.4f, 0.4f, optionsRed, optionsGreen, optionsBlue, optionsOpacity, false);
	}

	if (currentOption == optionCount && optionPress)
	{
		Vehicle_String = veh;
		Loading_Vehicle = true;

	}
}

void addCarCharSwap(char *option, char **From, char **From2, int*var, int min, int max, bool any = false, bool unti = false)
{
	char buf[200];
	char buf2[200];

	if (!any)
	{
		if (unti)
		{
			snprintf(buf, sizeof(buf), "%s: ~r~%s~w~ ", option, From2[*var]);
		}
		else
		{
			char *VehicleName = UI::_0x95C4B5AD(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(GAMEPLAY::GET_HASH_KEY(From[*var])));
			snprintf(buf, sizeof(buf), "%s: ~r~%s~w~ ", option, VehicleName);
		}

		SpawnCarOption(buf, From[*var], var, max);
		if (currentOption == optionCount)
		{
			lrInstruction = true;
			if (rightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + 1;
			}
			else if (leftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - 1;
			}
		}
	}
	else
	{
		snprintf(buf, sizeof(buf), "%s: - ~b~%s~w~ +", option, From[*var]);
		SpawnCarOption(buf, From[*var], var, max);
		if (currentOption == optionCount)
		{
			lrInstruction = true;
			if (rightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + 1;
			}
			else if (leftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - 1;
			}
		}
	}
}

void addCharSwap(char *option, char **From, int*var, int min, int max, bool any = false, char *info = NULL)
{
	char buf[300];
	if (!any)
	{
		snprintf(buf, sizeof(buf), "%s: ~r~%s~w~ ", option, From[*var]);
		addOption(buf, info);
		if (currentOption == optionCount)
		{
			lrInstruction = true;
			if (rightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + 1;
			}
			else if (leftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - 1;
			}
		}
	}
	else
	{
		snprintf(buf, sizeof(buf), "%s: - ~b~%s~w~  + ", option, From[*var]);
		addOption(buf, info);
		if (currentOption == optionCount)
		{
			lrInstruction = true;
			if (rightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + 1;
			}
			else if (leftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - 1;
			}
		}
	}

}
void addBoolOption(char *option, bool b00l, char *info = NULL)
{
	char buf[100];
	if (b00l)
	{
		snprintf(buf, sizeof(buf), "%s: On", option);
		addOption(buf, info);
	}
	else
	{
		snprintf(buf, sizeof(buf), "%s: Off", option);
		addOption(buf, info);
	}
}
void WechselBoolIntFunction(char *optionOFF, char *OptionOn, bool b00l, int *var, int min, int max, int movement, char *info = NULL)
{
	char buf[300];

	if (b00l)
	{
		int var1 = *var;
		char buf[300];
		snprintf(buf, sizeof(buf), "%s:< %i >", OptionOn, var1);
		addOption(buf, info);
		if (currentOption == optionCount)
		{
			lrInstruction = true;
			if (rightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + movement;

			}
			else if (leftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - movement;

			}
		}
	}
	else
	{
		snprintf(buf, sizeof(buf), "%s: ~r~Off", optionOFF);
		addOption(buf, info);
	}
}

template<typename TYPE>
void addCustomBoolOption(char *option, TYPE value, TYPE match, char *info = NULL)
{
	char buf[30];
	if (value == match)
	{
		snprintf(buf, sizeof(buf), "%s: On", option);
		addOption(buf, info);
	}
	else
	{
		snprintf(buf, sizeof(buf), "%s: Off", option);
		addOption(buf, info);
	}
}
void addIntOption(char *option, int *var, int min, int max, bool keyboard, char *info = NULL)
{
	char buf[100];
	snprintf(buf, sizeof(buf), "%s: < %i >", option, *var);
	addOption(buf, info);
	if (currentOption == optionCount)
	{
		lrInstruction = true;
		if (rightPress)
		{
			playSound("NAV_UP_DOWN");
			if (*var >= max)
				*var = min;
			else
				*var = *var + 1;
		}
		else if (leftPress)
		{
			playSound("NAV_UP_DOWN");
			if (*var <= min)
				*var = max;
			else
				*var = *var - 1;
		}
		if (keyboard)
		{
			squareInstruction = true;
			if (squarePress)
			{
				startKeyboard(0, "", (strlen(ItoS(max)) + 1));
				keyboardVar = var;
			}
		}
	}
}
void addIntOption2(char *option, int *var, int min, int max, bool keyboard, bool FastMove, char *info = NULL)
{
	char buf[1000];
	snprintf(buf, sizeof(buf), "%s: < %i >", option, *var);
	addOption(buf, info);
	if (currentOption == optionCount)
	{
		lrInstruction = true;
		if (currentOption == optionCount)
		{
			if (FastMove)
			{
				if (fastRightPress)
				{
					playSound("NAV_UP_DOWN");
					if (*var >= max)
						*var = min;
					else
						*var = *var + 1;
				}
				else if (fastLeftPress)
				{
					playSound("NAV_UP_DOWN");
					if (*var <= min)
						*var = max;
					else
						*var = *var - 1;
				}
			}
		}
		if (keyboard)
		{
			squareInstruction = true;
			if (squarePress)
			{
				startKeyboard(0, "", (strlen(ItoS(max)) + 1));
				keyboardVar = var;
			}
		}
	}
}

void addIntOption3(char *option, int *var, int min, int max, int Movement = 1000)
{
	char buf[100];
	snprintf(buf, sizeof(buf), "%s < %i >", option, *var);
	addOption(buf);
	if (currentOption == optionCount)
	{
		if (rightPress)
		{
			//_snprintf("NAV_UP_DOWN");
			if (*var >= max)
				*var = min;
			else
				*var = *var + Movement;
			rightPress = false;
		}
		else if (leftPress)
		{
			//_snprintf("NAV_UP_DOWN");
			if (*var <= min)
				*var = max;
			else
				*var = *var - Movement;
			leftPress = false;
		}
	}
}

void addFloatOption(char *option, float *var, float min, float max, bool holdPress, char *info = NULL)
{
	char buf[30];
	snprintf(buf, sizeof(buf), "%s: < %s >", option, FtoS(*var));
	addOption(buf, info);
	if (currentOption == optionCount)
	{
		lrInstruction = true;
		if (holdPress)
		{
			if (fastRightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + 0.01f;
			}
			else if (fastLeftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - 0.01f;
			}
		}
		else
		{
			if (rightPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var >= max)
					*var = min;
				else
					*var = *var + 0.01f;
			}
			else if (leftPress)
			{
				playSound("NAV_UP_DOWN");
				if (*var <= min)
					*var = max;
				else
					*var = *var - 0.01f;
			}
		}
	}
}
void addKeyboardOption(char *option, int action, char *defaultText, int maxLength, char *info = NULL)
{
	addOption(option, info);
	if (currentOption == optionCount)
	{
		xInstruction = true;
		if (optionPress)
			startKeyboard(action, defaultText, maxLength);
	}
}
#pragma endregion
int getOption()
{
	xInstruction = true;
	if (optionPress)
		return currentOption;
	else return 0;
}
void monitorButtons()
{
	SetupTimer(20);
	if (submenu == Closed)
	{
		if (IsTimerReady())
		{
			if (isPressed(Button_X) && isPressed(Dpad_Right))
			{
				submenu = Main_Menu;
				submenuLevel = 0;
				currentOption = 1;
				playSound("YES");
			}
			ResetTimer();
		}
	}
	else {
		if (IsTimerReady())
		{
			if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_B))
			{
				if (submenu == Main_Menu)
				{
					submenu = Closed;
				}
				else {
					submenu = lastSubmenu[submenuLevel - 1];
					currentOption = lastOption[submenuLevel - 1];
					submenuLevel--;
				}
				playSound("Back");
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_A))
			{
				optionPress = true;
				playSound("SELECT");
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Dpad_Up))
			{
				currentOption--;
				if (currentOption < 1)
				{
					currentOption = optionCount;
				}
				playSound("NAV_UP_DOWN");
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Dpad_Down))
			{
				currentOption++;
				if (currentOption > optionCount)
				{
					currentOption = 1;
				}
				playSound("NAV_UP_DOWN");
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Dpad_Right))
			{
				rightPress = true;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Dpad_Left))
			{
				leftPress = true;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Dpad_Right))
			{
				fastRightPress = true;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Dpad_Left))
			{
				fastLeftPress = true;
			}
			else if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button_X))
			{
				squarePress = true;
			}
			ResetTimer();
		}
	}
}
void Information(int selectedPlayer)
{
	if (selectedUI)
	{
		for (int i = 1; i < 7; i++)
		{
			GRAPHICS::DRAW_RECT(menuXCoord - SelectedUICoords, (SelectedUITitleY + 0.04 * i), SelectedUITitleSizeX, SelectedUITitleSizeY, 0, 0, 0, 100);
		}
		GRAPHICS::DRAW_RECT(menuXCoord - SelectedUICoords, SelectedUIYcoords, SelectedUITitleSizeX, SelectedUIMenuYSize, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity);
		DrawSprite1("commonmenu", "header_gradient_script", menuXCoord - SelectedUICoords, SelectedUIYcoords, SelectedUITitleSizeX, SelectedUIMenuYSize, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
		DrawSprite1("commonmenu", "header_gradient_script", menuXCoord - SelectedUICoords, SelectedUIYcoords, SelectedUITitleSizeX, SelectedUIMenuYSize, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
		DrawSprite1("commonmenu", "header_gradient_script", menuXCoord - SelectedUICoords, SelectedUIYcoords, SelectedUITitleSizeX, SelectedUIMenuYSize, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
		DrawSprite1("commonmenu", "header_gradient_script", menuXCoord - SelectedUICoords, SelectedUIYcoords, SelectedUITitleSizeX, SelectedUIMenuYSize, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
		DrawSprite1("commonmenu", "header_gradient_script", menuXCoord - SelectedUICoords, SelectedUIYcoords, SelectedUITitleSizeX, SelectedUIMenuYSize, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
		DrawSprite1("commonmenu", "header_gradient_script", menuXCoord - SelectedUICoords, SelectedUIYcoords, SelectedUITitleSizeX, SelectedUIMenuYSize, 90, GradationRed, GradationGreen, GradationBlue, GradationOpacity);
		//GRAPHICS::DRAW_RECT(menuXCoord - SelectedUICoords, (0.084f + 0.04 * i), 0.19f, 0.04f, 0, 0, 0, 20);
		GRAPHICS::DRAW_RECT(menuXCoord - SelectedUICoords, 0.0862f - menunameuicoords, 0.19f, 0.04f, 200, 0, 0, 100);

		char Armor[100];
		switch (PLAYER::GET_PLAYER_MAX_ARMOUR(selectedPlayer))
		{
		case 3000: snprintf(Armor, sizeof(Armor), "SPRX : GNXKS"); break;
		case 2002: snprintf(Armor, sizeof(Armor), "SPRX : GNXKS v2"); break;
		case 1996: snprintf(Armor, sizeof(Armor), "SPRX : Power sprx"); break;
		case 2001: snprintf(Armor, sizeof(Armor), "SPRX : Project Eke"); break;
		case 2600: snprintf(Armor, sizeof(Armor), "SPRX : FINE SPRX"); break;
		case 2777: snprintf(Armor, sizeof(Armor), "SPRX : Xenoverse"); break;
		case 2870: snprintf(Armor, sizeof(Armor), "SPRX : GC"); break;
		case 2871: snprintf(Armor, sizeof(Armor), "SPRX : GC Pro"); break;
		case 2872: snprintf(Armor, sizeof(Armor), "SPRX : GC Admin"); break;
		case 2900: snprintf(Armor, sizeof(Armor), "SPRX : Hyper Dx Crack"); break;
		case 2901: snprintf(Armor, sizeof(Armor), "SPRX : HG Normal"); break;
		case 2902: snprintf(Armor, sizeof(Armor), "SPRX : HG Pro"); break;
		case 2903: snprintf(Armor, sizeof(Armor), "SPRX : HG Master"); break;
		case 2055: snprintf(Armor, sizeof(Armor), "SPRX : NightMare Free"); break;
		case 1220: snprintf(Armor, sizeof(Armor), "SPRX : NightMare Admin"); break;
		case 1109: snprintf(Armor, sizeof(Armor), "SPRX : TKMN SPRX"); break;
		case 2599: snprintf(Armor, sizeof(Armor), "SPRX : TKMN SPRX"); break;
		case 2000: snprintf(Armor, sizeof(Armor), "SPRX : TKMN SPRX"); break;
		case 2009: snprintf(Armor, sizeof(Armor), "SPRX : Zedd SPRX"); break;
		case 3025: snprintf(Armor, sizeof(Armor), "SPRX : Paix SPRX"); break;
		default:   snprintf(Armor, sizeof(Armor), "SPRX : 不明(%i)", PLAYER::GET_PLAYER_MAX_ARMOUR(selectedPlayer)); break;
		}
		Vector3 MyPos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), false);
		int XCoord = MyPos.x;
		int YCoord = MyPos.y;
		int ZCoord = MyPos.z;
		Hash StreetName;
		Hash CrossingRoad;
		char buffer[100];
		char buffer1[100];
		char streetcoords[200];
		char streetcoords2[200];
		char wantedlevel[200];
		char *Headset;
		sprintf(wantedlevel, "手配度:~w~%i", PLAYER::GET_PLAYER_WANTED_LEVEL(selectedPlayer));
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 0);
		PATHFIND::GET_STREET_NAME_AT_COORD(coords.x, coords.y, coords.z, &StreetName, &CrossingRoad);
		sprintf(streetcoords, "居場所: ~w~%s", UI::GET_STREET_NAME_FROM_HASH_KEY(StreetName));
		sprintf(streetcoords2, "道路名:~w~ %s", UI::GET_STREET_NAME_FROM_HASH_KEY(CrossingRoad));

		if (NETWORK::NETWORK_PLAYER_HAS_HEADSET(selectedPlayer))
		{
			if (NETWORK::NETWORK_IS_PLAYER_TALKING(selectedPlayer))
			{
				Headset = "VC:~HUD_COLOUR_BLUELIGHT~会話中";
			}
			else
			{
				Headset = "VC:~y~装着済";
			}
		}
		else
		{
			Headset = "VC:~w~装着なし";
		}
		int health = ENTITY::GET_ENTITY_HEALTH(PLAYER::GET_PLAYER_PED(selectedPlayer));
		int pedArmour = PED::GET_PED_ARMOUR(PLAYER::GET_PLAYER_PED(selectedPlayer));
		int playerCar = PED::GET_VEHICLE_PED_IS_IN(PLAYER::GET_PLAYER_PED(selectedPlayer), 0);
		int thisPlayersCar = ENTITY::GET_ENTITY_MODEL(playerCar);
		char healthtext1[200];
		char healthtext2[200];
		char healthtext3[200];
		char entityXcoordstext[200];
		char entityYcoordstext[200];
		char entityZcoordstext[200];
		char Rank[200];
		char Money[200];
		char playercar[200];

		if (submenu != Closed)
		{
			Vector3 theircoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS2(PLAYER::GET_PLAYER_PED(selectedPlayer), 0.0f, 0.0f, 1.6f);
			GRAPHICS::DRAW_MARKER3(MarkerTypeChevronUpx2, theircoords.x, theircoords.y, theircoords.z, 0.0f, 0.0f, 0.0f, 180.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, markerRed, markerGreen, markerBlue, markerOpacity, 1, 0, 2, 1, 0, 0, 0);
			GRAPHICS::DRAW_MARKER3(MarkerTypeHorizontalCircleFat, theircoords.x, theircoords.y, theircoords.z - 2.45f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, markerRed, markerGreen, markerBlue, markerOpacity, 1, 0, 2, 1, 0, 0, 0);
		}
		sprintf(entityXcoordstext, "X 座標 : ~w~%d", XCoord);
		sprintf(entityYcoordstext, "Y 座標 :~w~ %d", YCoord);
		sprintf(entityZcoordstext, "Z 座標 : ~w~%d", ZCoord);
		sprintf(healthtext1, "体力 : ~w~%i", health);
		sprintf(healthtext2, "アーマー : ~w~%i", pedArmour);

		char *VehicleName = UI::_0x95C4B5AD(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(thisPlayersCar));
		if (strstr(VehicleName, "NULL") != NULL)
		{
			VehicleName = "徒歩";
		}
		sprintf(healthtext3, "車両 : ~w~%s", VehicleName);
		int RText = 255;
		int GText = 255;
		int BText = 255;
		int AText = 255;
		bool aaa;
		bool bbb;
		if (PED::IS_PED_MALE(PLAYER::GET_PLAYER_PED(selectedPlayer)))
		{
			aaa = true;
		}
		else
		{
			aaa = false;
		}
		snprintf(Rank, sizeof(Rank), "ランク:~w~%i", (int)getPlayerRank(selectedPlayer));
		snprintf(Money, sizeof(Money), "金:~w~%i", (int)getCash(selectedPlayer));
		Talker = PLAYER::GET_PLAYER_NAME(selectedPlayer);
		//drawText(Talker, 1, 0.620f, 0.145f, 0.40f, 0.40f, 0, 255, 51, 255, true);
		drawText(Talker, 1, menuXCoord - 0.2f, 0.076f, 0.40f, 0.45f, 255, 255, 255, 255, true, false);
		drawText(healthtext1, 1, 0.560f, 0.1f + 0.01, 0.40f, 0.40f, RText, GText, BText, AText, false, false);
		drawText(healthtext2, 1, 0.560f, 0.125f + 0.01, 0.40f, 0.40f, RText, GText, BText, AText, false, false);
		drawText(entityXcoordstext, 1, 0.560f, 0.15f + 0.01, 0.40f, 0.40f, RText, GText, BText, AText, false, false);
		drawText(entityYcoordstext, 1, 0.560f, 0.175 + 0.01, 0.40f, 0.40f, RText, GText, BText, AText, false, false);
		drawText(entityZcoordstext, 1, 0.560f, 0.2f + 0.01, 0.40f, 0.40f, RText, GText, BText, AText, false, false);
		drawText(healthtext3, 1, 0.560f, 0.225f + 0.01, 0.40f, 0.40f, RText, GText, BText, AText, false, false);

		drawText(Armor, 1, 0.560f, 0.25f + 0.01, 0.4f, 0.4f, RText, GText, BText, AText, false, false);
		drawText(streetcoords, 1, 0.560f, 0.275f + 0.01, 0.4f, 0.4f, RText, GText, BText, AText, false, false);
		drawText(streetcoords2, 1, 0.560f, 0.30f + 0.01, 0.4f, 0.4f, RText, GText, BText, AText, false, false);
		drawText(Money, 1, 0.660f, 0.1f + 0.01, 0.4f, 0.4f, RText, GText, BText, AText, false, false);
		drawText(Rank, 1, 0.660f, 0.125f + 0.01, 0.4f, 0.4f, RText, GText, BText, AText, false, false);
		drawText(wantedlevel, 1, 0.660f, 0.15 + 0.01, 0.4f, 0.4f, RText, GText, BText, AText, false, false);
		drawText(Headset, 1, 0.660f, 0.175 + 0.01, 0.4f, 0.4f, RText, GText, BText, AText, false, false);
		if (aaa)
		{
			drawText("Sex : ~b~Male", 1, 0.660f, 0.2 + 0.01, 0.4f, 0.4f, RText, GText, BText, AText, false, false);
		}
		else
		{
			drawText("Sex : ~r~Female", 1, 0.660f, 0.2 + 0.01, 0.4f, 0.4f, RText, GText, BText, AText, false, false);
		}
	}
}
void normalMenuActions()
{
	UI::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(0);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(10);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(6);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(7);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(9);
	UI::HIDE_HUD_COMPONENT_THIS_FRAME(8);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 19);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 20);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 21);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 27);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 54);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 123);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 124);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 125);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 126);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 138);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 139);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 140);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 177);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 178);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 179);
	CONTROLS::DISABLE_CONTROL_ACTION(0, 207);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 166);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 167);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 177);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 178);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 193);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 194);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 195);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 202);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 203);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 204);
	CONTROLS::SET_INPUT_EXCLUSIVE(2, 205);
	GRAPHICS::DRAW_RECT(menuXCoord, (((maxOptions * menuYCoords) / 2) + 0.159f), MenuXSize, (maxOptions * MenuYSize), backgroundRed, backgroundGreen, backgroundBlue, backgroundOpacity); //Background
	GRAPHICS::DRAW_RECT(menuXCoord, (((maxOptions + 1) * menuYCoords) + 0.143f), MenuXSize, MenuYSize, 0, 0, 0, 64); //Info Box
	if (currentOption != optionCount)
	{
		//GRAPHICS::DRAW_RECT(menuXCoord, ((maxOptions * menuYCoords) + 0.161f), MenuXSize, DownIndicator, indicatorRed, indicatorGreen, indicatorBlue, indicatorOpacity); //Down Indicator
	}

	if (optionCount > maxOptions)
	{
		drawText(infoText, optionsFont, menuXCoord, ((maxOptions + 1)* MenuYSize + infobox), 0.4f, 0.4f, bannerTextRed, bannerTextGreen, bannerTextBlue, bannerTextOpacity, true);
		GRAPHICS::DRAW_RECT(menuXCoord, (((maxOptions + 3) * menuYCoords) + 0.1415f), MenuXSize, MenuYSize, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity); //Info Box
	}
	else
	{
		drawText(infoText, optionsFont, menuXCoord, ((maxOptions + 1) * MenuYSize + infobox), 0.4f, 0.4f, bannerTextRed, bannerTextGreen, bannerTextBlue, bannerTextOpacity, true);
		GRAPHICS::DRAW_RECT(menuXCoord, (((maxOptions + 2) * menuYCoords) + 0.1415f), MenuXSize, MenuYSize, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity); //Info Box
	}

	SetupTimer2(20);
	if (instructions)
	{
		if (IsTimerReady2())
		{
			if (xInstruction)
				addInstruction(BUTTON_B, "Select");
			if (squareInstruction)
				addInstruction(BUTTON_X, "Keyboard");
			if (lrInstruction)
				addInstruction(BUTTON_DPAD_LEFT_RIGHT, "Scroll");
			addInstruction(BUTTON_DPAD_UP_DOWN, "Scroll");
			addInstruction(BUTTON_A, "Back");
			instructionsClose();
			ResetTimer2();
		}
	}
}
void resetVars()
{
	if (submenu != Closed)
		normalMenuActions();
	optionPress = false;
	rightPress = false;
	leftPress = false;
	fastRightPress = false;
	fastLeftPress = false;
	squarePress = false;
	infoText = NULL;
	instructionsSetupThisFrame = false;
	squareInstruction = false;
	xInstruction = false;
	lrInstruction = false;
}
#pragma endregion

#pragma region ------Other Loop------
void otherLoop()
{
	if (selectedUI)
	{
		if (submenu == Players_List)
		{
			Information(currentOption - 1);
		}
		else
		{
			if (strstr(PLAYER::GET_PLAYER_NAME(selectedPlayer), "**Invilid**"))
			{
				Information(PLAYER::PLAYER_ID());
			}
			else
			{
				Information(selectedPlayer);
			}
		}
	}
	if (Neons_) { drawneons(NeonR, NeonG, NeonB); }
	if (isvehJetpack)
	{
		if (!VehJet)
		{
			if (isPressed(Button_L3))
			{
				ENTITY::FREEZE_ENTITY_POSITION(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID3()), true);
				VehJet = true;
			}
			drawText("Press L3 to Start Vehicle Jetpack", 0, 0.5, 0.2, 0.4, 0.4, 200, 0, 0, 255, true);
		}
		else
		{
			Entity MyVehicle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID3());
			RequestNetworkControl(MyVehicle);
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID3(), false))
			{
				if (PED::IS_PED_ON_ANY_BIKE(PLAYER::PLAYER_PED_ID3()))
				{
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
					GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
					GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", MyVehicle, 0, 1, -0.3, 0, +90, 0, 3.0f, 0, 0, 0);
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
					GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
					GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", MyVehicle, 0, -1, -0.3, 0, +90, 0, 3.0f, 0, 0, 0);
				}
				else if (PED::IS_PED_IN_ANY_BOAT(PLAYER::PLAYER_PED_ID3()))
				{
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
					GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
					GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", MyVehicle, 0, 2, -0.3, 0, +90, 0, 3.0f, 0, 0, 0);
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
					GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
					GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", MyVehicle, 0, -2, -0.3, 0, +90, 0, 3.0f, 0, 0, 0);
				}
				else
				{
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
					GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
					GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", MyVehicle, -0.5, 1.5, -0.3, 0, +90, 0, 3.0f, 0, 0, 0);
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
					GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
					GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", MyVehicle, +0.5, 1.5, -0.3, 0, +90, 0, 3.0f, 0, 0, 0);
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
					GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
					GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", MyVehicle, -0.5, -1.5, -0.3, 0, +90, 0, 3.0f, 0, 0, 0);
					STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
					GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
					GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", MyVehicle, +0.5, -1.5, -0.3, 0, +90, 0, 3.0f, 0, 0, 0);
				}
			}
			Vector3 InCamera = get_rotation_from_cam(4);
			ENTITY::SET_ENTITY_ROTATION(MyVehicle, 0, 0, InCamera.z, true, true);
			if (isPressed(Button_A))
			{
				//ENTITY::FREEZE_ENTITY_POSITION(MyVehicle, false);
				//Vector3 MyVehicleCoords = ENTITY::GET_ENTITY_COORDS(MyVehicle, true);
				//ENTITY::SET_ENTITY_COORDS_1(MyVehicle, MyVehicleCoords.x, MyVehicleCoords.y, MyVehicleCoords.z + 2,true,true,true, true);
				ENTITY::FREEZE_ENTITY_POSITION(MyVehicle, false);
				APPLY_FORCE(MyVehicle, 0, 0, 0.3 * vehjetpackvar, 0, 0, 0);

			}
			else
			{
				ENTITY::FREEZE_ENTITY_POSITION(MyVehicle, true);
			}
			if (isPressed(Button_B))
			{
				//ENTITY::FREEZE_ENTITY_POSITION(MyVehicle, false);
				Vector3 MyVehicleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS2(MyVehicle, 0, 0, -1);
				ENTITY::SET_ENTITY_COORDS_1(MyVehicle, MyVehicleCoords.x, MyVehicleCoords.y, MyVehicleCoords.z - 0.4 * vehjetpackvar, false, false, false, true);


			}
			if (isPressed(Button_R2))
			{
				//ENTITY::FREEZE_ENTITY_POSITION(MyVehicle, false);
				Vector3 MyVehicleCoords = get_coords_from_cam(7 * vehjetpackvar);
				ENTITY::SET_ENTITY_COORDS_1(MyVehicle, MyVehicleCoords.x, MyVehicleCoords.y, MyVehicleCoords.z - 0.3, false, false, false, true);
				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
				GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", MyVehicle, 0, -1.5, 0, +90, +90, 0, 3.0f, 0, 0, 0);
				//APPLY_FORCE(MyVehicle, 0, 0.4, 0, 0, 0, 0);

			}

			if (isPressed(Button_R3))
			{
				ENTITY::FREEZE_ENTITY_POSITION(MyVehicle, false);
				VehJet = false;
			}

			drawText("R2 Drive", 0, 0.5, 0.125, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("× Up", 0, 0.5, 0.150, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("○ Down", 0, 0.5, 0.175, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("Press R3 to Finish Vehicle Jetpack", 0, 0.5, 0.2, 0.4, 0.4, 200, 0, 0, 255, true);
		}
	}
	if (L3boost) 
	{ 
		if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, Button_L3)) VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 100);
	}
	if (LowerWhip) 
	{ 
		if (PED::IS_PED_IN_VEHICLE) 
		{ 
			int VehHandle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); 
			PED::IS_PED_IN_ANY_VEHICLE(VehHandle, 0); Vector3 Force = { 0, 0, -0.2 }; 
			ENTITY::APPLY_FORCE_TO_ENTITY(VehHandle, 1, Force.x, Force.y, Force.z, 0, 0, 0, 0, 1, 1, 1, 0, 1); 
		} 
	}
	if (showInfos)
	{
		if (frameCache_time + 1000 < GAMEPLAY::GET_GAME_TIMER())
		{
			frameCache_time = GAMEPLAY::GET_GAME_TIMER();
			frameCache_old = frameCache_new;
			frameCache_new = GAMEPLAY::GET_FRAME_COUNT();
		}
		char bufFPS[50];
		fps = frameCache_new - frameCache_old - 1;
		if (19 <= fps)
		{

			sprintf(bufFPS, "~r~%i FPS", fps);
		}
		else
		{
			sprintf(bufFPS, "~r~%i FPS", fps);
		}
		draw_showhost();
		drawInfos(HST, NUM, bufFPS);
	}
	if (talking)
	{
		char TLK[100];
		memset(TLK, 0, sizeof(TLK));
		char* Talker;
		for (int i = 0; i < 16; i++)
		{
			if (NETWORK::NETWORK_PLAYER_HAS_HEADSET(i))
			{
				Talker = PLAYER::GET_PLAYER_NAME(i);
				if (NETWORK::NETWORK_IS_PLAYER_TALKING(i))
				{
					snprintf(TLK, sizeof(TLK), "~r~Talking: %s", Talker);
				}
				else
				{
					snprintf(TLK, sizeof(TLK), "~r~Headset: %s", Talker);
				}
				drawTalker(TLK, i);
			}
		}
	}
	if (spawnsetswat)
	{
		int hash = GAMEPLAY::GET_HASH_KEY("s_m_y_swat_01");
		Vector3 mycoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), true);
		STREAMING::REQUEST_MODEL(hash);
		if (STREAMING::HAS_MODEL_LOADED(hash))
		{
			spawnedped = PED::CREATE_PED(21, hash, mycoords.x, mycoords.y, mycoords.z, 29, 0.5f, true);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
			PED::SET_PED_COMBAT_ABILITY(spawnedped, 100);
			AI::TASK_COMBAT_PED(spawnedped, PLAYER::GET_PLAYER_PED(selectedPlayer), 0, 16);
			PED::SET_PED_CAN_SWITCH_WEAPON(spawnedped, false);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(spawnedped, 0);
			ENTITY::SET_ENTITY_INVINCIBLE(spawnedped, true);
			SET_PED_CAN_RAGDOLL(spawnedped, 0);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(spawnedped, 0);
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(spawnedped, false);
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(spawnedped, 0xB1CA77B1, 9999, 1);
		}
		spawnsetswat = false;
	}
	if (spawnsetchop)
	{
		int hash = GAMEPLAY::GET_HASH_KEY("A_C_Chop");
		Vector3 Pos1 = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), true);
		STREAMING::REQUEST_MODEL(hash);
		if (STREAMING::HAS_MODEL_LOADED(hash))
		{
			spawnedped = PED::CREATE_PED(21, hash, Pos1.x, Pos1.y, Pos1.z, 29, 0.5f, true);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
			PED::SET_PED_COMBAT_ABILITY(spawnedped, 100);
			AI::TASK_COMBAT_PED(spawnedped, PLAYER::GET_PLAYER_PED(selectedPlayer), 0, 16);
			PED::SET_PED_CAN_SWITCH_WEAPON(spawnedped, false);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(spawnedped, 0);
			ENTITY::SET_ENTITY_INVINCIBLE(spawnedped, true);
			SET_PED_CAN_RAGDOLL(spawnedped, 0);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(spawnedped, 0);
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(spawnedped, false);
		}
		spawnsetchop = false;
	}
	if (spawnsetmtlion)
	{
		int hash = GAMEPLAY::GET_HASH_KEY("A_C_MtLion");
		Vector3 mycoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), true);
		STREAMING::REQUEST_MODEL(hash);
		if (STREAMING::HAS_MODEL_LOADED(hash))
		{
			spawnedped = PED::CREATE_PED(21, hash, mycoords.x, mycoords.y, mycoords.z, 29, 0.5f, true);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
			AI::TASK_COMBAT_PED(spawnedped, PLAYER::GET_PLAYER_PED(selectedPlayer), 0, 16);
			PED::SET_PED_CAN_SWITCH_WEAPON(spawnedped, true);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(spawnedped, 0);
			ENTITY::SET_ENTITY_INVINCIBLE(spawnedped, true);
			SET_PED_CAN_RAGDOLL(spawnedped, 0);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(spawnedped, 0);
			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(spawnedped, false);
		}
		spawnsetmtlion = false;
	}
	if (frezzeplayer) { AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED(selectedPlayer)); }

	if (forcefield) { Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), true); FIRE::ADD_EXPLOSION(coords.x, coords.y, coords.z, 0x08, 640.00, 0, 1, 0); }

	if (explosionloop) { ExplodePlayer(selectedPlayer); }

	if (KEYBOARD_PROMPT)
	{
		if (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 1)
		{
			KEYBOARD_PROMPT = false;
			doKeyboardFunc(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT(), KeyboardIndex, selectedPlayer);
		}
		else if (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 2)
		{
			KEYBOARD_PROMPT = false;
		}
		else if (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 3)
		{
			KEYBOARD_PROMPT = false;
			PRINT("~r~ERROR: Keyboard not found?", 2000);
		}
	}
	if (StealClothes)
	{
		if (SET_PLAYER_MODEL(ENTITY::GET_ENTITY_MODEL(PLAYER::GET_PLAYER_PED(selectedPlayer))))
		{
			CopyingOutfit();
			StealClothes = false;
		}
	}
	if (effectp)
	{
		if (PTFXTimeout <= GAMEPLAY::GET_GAME_TIMER())
		{
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_trevor1");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_trevor1");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 31086, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_trevor1");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_trevor1");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 18905, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_trevor1");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_trevor1");

			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 57005, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_trevor1");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_trevor1");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, SKEL_L_Foot, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_trevor1");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_trevor1");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, SKEL_R_Foot, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_trevor1");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_trevor1");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_trev1_trailer_boosh", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, SKEL_Spine_Root, 0.1f, 0, 0, 0);
			GRAPHICS::SET_PARTICLE_FX_NON_LOOPED_COLOUR(72, 236, 232);
			PTFXTimeout = GAMEPLAY::GET_GAME_TIMER() + 1000;
		}

	}
	if (effectp2)
	{
		if (PTFXTimeout2 <= GAMEPLAY::GET_GAME_TIMER())
		{
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_michael2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_michael2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_abattoir_ped_sliced", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 31086, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_michael2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_michael2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_abattoir_ped_sliced", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 18905, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_michael2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_michael2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_abattoir_ped_sliced", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 57005, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_michael2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_michael2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_abattoir_ped_sliced", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, SKEL_L_Foot, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_michael2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_michael2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_abattoir_ped_sliced", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, SKEL_R_Foot, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_michael2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_michael2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_abattoir_ped_sliced", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, SKEL_Spine_Root, 0.1f, 0, 0, 0);
			GRAPHICS::SET_PARTICLE_FX_NON_LOOPED_COLOUR(72, 236, 232);
			PTFXTimeout2 = GAMEPLAY::GET_GAME_TIMER() + 2000;
		}
	}
	if (effectp3)
	{
		if (PTFXTimeout3 <= GAMEPLAY::GET_GAME_TIMER())
		{
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 31086, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 18905, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 57005, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, SKEL_L_Foot, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, SKEL_R_Foot, 0.1f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
			GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, SKEL_Spine_Root, 0.1f, 0, 0, 0);
			GRAPHICS::SET_PARTICLE_FX_NON_LOOPED_COLOUR(72, 236, 232);
			PTFXTimeout3 = GAMEPLAY::GET_GAME_TIMER() + 1000;
		}

	}
	if (loopptfxmod)
	{
		LoopPtfxMod();
	}
	if (loopptfxmod2)
	{
		LoopPtfxMod2();
	}
	if (isAnimationStart)
	{
		if (doAnimation(Animation1[AnimationV], Animation2[AnimationV]))
		{
			isAnimationStart = false;
		}
	}
	if (doForceField_)
	{
		doForceField(PLAYER::PLAYER_PED_ID());
	}
	if (hasBossMode)
	{
		BossMode();
	}
	if (ChangeModelHash)
	{
		ChangeModelHash = !SET_PLAYER_MODEL(ModelHash);
		if (blackmodel)
		{
			for (int i = 0; i < 14; i++)
			{
				Modelo(Family[i], 0, 2, 0);
			}
		}
	}
	if (Loading_Vehicle) {
		Vector3 Pos;
		if (Create_Vehicle(Vehicle_String) == 1) {

			Vehicle_String = "";
			Loading_Vehicle = false;
		}
	}
	if (RapidFire)
	{
		if (GAMEPLAY::GET_GAME_TIMER() > RapidFireDelay)
		{
			int Handle = PLAYER::PLAYER_PED_ID();

			Hash weaponHash;

			WEAPON::GET_CURRENT_PED_WEAPON(Handle, &weaponHash, 0);
			if (PED::IS_PED_ON_FOOT(Handle) && CONTROLS::IS_CONTROL_PRESSED(0, Button_R2))
			{

				Vector3 FromCoord = PED::GET_PED_BONE_COORDS(PLAYER::PLAYER_PED_ID(), 57005, 0, 0, 0);
				Vector3 ToCoord = GetCoordsInfrontOfCam(75);
				GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(FromCoord.x, FromCoord.y, FromCoord.z, ToCoord.x, ToCoord.y, ToCoord.z, 250, 0, weaponHash, PLAYER::PLAYER_PED_ID(), 1, 0, 250);
				RapidFireDelay = GAMEPLAY::GET_GAME_TIMER() + 50;

			}

		}
	}
	if (megivemoneyg)
	{
		GiveMoneyGunMe();
	}
	if (TeleportGunLooping)
	{
		MeTeleportGun();
	}
	if (hashgun)
	{
		if (PLAYER::_GET_AIMED_ENTITY(PLAYER::PLAYER_ID(), &Target))
		{
			if (Target != 0 && GAMEPLAY::GET_GAME_TIMER() > HashCoolDown)
			{
				char buffer2121[200];
				sprintf(buffer2121, "Object Hash: 0x%X", ENTITY::GET_ENTITY_MODEL(Target));
				//DRAW_TEXT(buffer, 0.5, 0.05, 0.6, 0.6, 0, true, false, true, false, SelectColour.R, SelectColour.G, SelectColour.B, 255);
				PRINT(buffer2121);
				HashCoolDown = GAMEPLAY::GET_GAME_TIMER() + 2000;
			}
		}
	}
	if (deletegun)
	{
		if (PLAYER::_GET_AIMED_ENTITY(PLAYER::PLAYER_ID(), &Target) && PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
		{
			if (NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Target) && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Target))
			{
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Target, 0, 1);
				if (ENTITY::IS_ENTITY_A_PED(Target))
				{
					if (PED::IS_PED_IN_ANY_VEHICLE(Target, false))
					{
						Target = PED::GET_VEHICLE_PED_IS_IN(Target, false);
						int NumSeats = VEHICLE::_GET_VEHICLE_MODEL_MAX_NUMBER_OF_PASSENGERS(ENTITY::GET_ENTITY_MODEL(Target));
						for (int i = -1; i < NumSeats - 1; i++)
						{
							if (!VEHICLE::IS_VEHICLE_SEAT_FREE(Target, i))
							{
								int PedID = VEHICLE::GET_PED_IN_VEHICLE_SEAT(Target, i);
								NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PedID);
								PED::DELETE_PED(&PedID);
							}
						}
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Target);
						VEHICLE::DELETE_VEHICLE(&Target);
						ENTITY::SET_ENTITY_COORDS(Target, 6000.0, 6000.0, 6000.0, true, false, false, true);
					}
					else
					{
						PED::DELETE_PED(&Target);
						ENTITY::SET_ENTITY_COORDS(Target, 6000.0, 6000.0, 6000.0, true, false, false, true);
					}
				}
				else if (ENTITY::IS_ENTITY_A_VEHICLE(Target))
				{
					VEHICLE::DELETE_VEHICLE(&Target);
					ENTITY::SET_ENTITY_COORDS(Target, 6000.0, 6000.0, 6000.0, true, false, false, true);
				}
				else if (ENTITY::IS_ENTITY_AN_OBJECT(Target))
				{
					if (NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Target) && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Target))
					{
						ENTITY::DETACH_ENTITY(Target, 1, 1);
						ENTITY::SET_ENTITY_COORDS(Target, 6000.0, 6000.0, 6000.0, true, false, false, true);
						ENTITY::DELETE_ENTITY(&Target);
					}
				}
				else
				{
					ENTITY::DELETE_ENTITY(&Target);
					ENTITY::SET_ENTITY_COORDS(Target, 6000.0, 6000.0, 6000.0, true, false, false, true);
				}
			}
		}
	}

	if (UnlimitedAmmo)
	{
		WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), 1);
	}

	if (Psychokinetic)
	{
		if (isIronMan)
		{
			AnimationLoad = true;
			WEAPON::REMOVE_ALL_PED_WEAPONS(PLAYER::PLAYER_PED_ID3(), true);
			ENTITY::SET_ENTITY_HAS_GRAVITY(PLAYER::PLAYER_PED_ID3(), false);
			DrawSprite("darts", "dart_reticules_zoomed", 0.5f, 0.5f, 0.04f, 0.0600f, 0, 255, 255, 255, 255);
			Vector3 InCamera = get_rotation_from_cam(4);
			ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 0, 0, InCamera.z, true, true);
			drawText("Move Stick !!!!", 0, 0.5, 0.125, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("□ Down", 0, 0.5, 0.150, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("X Up", 0, 0.5, 0.175, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("R1 Beam Shot", 0, 0.5, 0.2, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("L2 Rocket Shot", 0, 0.5, 0.2, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("R2 Superman", 0, 0.5, 0.225, 0.4, 0.4, 200, 0, 0, 255, true);

			if (isPressed(Button_R1))
			{

				TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "skydive@parachute@", "chute_idle");
				//TASK_PLAY_ANIM_1(PLAYER::PLAYER_PED_ID3(), "amb@world_human_muscle_flex@arms_at_side@idle_a", "idle_b", 9);
				TASK_PLAY_ANIM_1(PLAYER::PLAYER_PED_ID3(), "amb@lo_res_idles@", "world_human_maid_clean_lo_res_base", 9);
				AI::SET_ANIM_RATE(PLAYER::PLAYER_PED_ID3(), 0, 0, 0);
				if (ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID3(), "amb@lo_res_idles@", "world_human_maid_clean_lo_res_base", 3))
				{
					int VehicleHandle = PLAYER::PLAYER_PED_ID3();
					int launchDistance = 250;
					int weaponHash = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_PLAYER_HUNTER");
					float launchSpeed = 500.0f;
					float Distance = 200;
					float Pitch = CAM::GET_GAMEPLAY_CAM_RELATIVE_PITCH();
					Vector3 Start = ENTITY::_0x7C6339DF(PLAYER::PLAYER_PED_ID3(), PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID3(), 57005));
					Vector3 RelativePos;
					Vector3 endPosition = get_coords_from_cam(80.0);
					//float Start[] = { spawnPosition.x, spawnPosition.y, spawnPosition.z };
					float End[] = { endPosition.x, endPosition.y, endPosition.z };
					GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_1(Start.x, Start.y, Start.z, End[0], End[1], End[2], launchDistance, 0, weaponHash, PLAYER::PLAYER_PED_ID3(), 1, 1, launchSpeed);
				}

			}
			else if (isPressed(Button_L2))
			{
				TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "skydive@parachute@", "chute_idle");
				//TASK_PLAY_ANIM_1(PLAYER::PLAYER_PED_ID3(), "amb@world_human_muscle_flex@arms_at_side@idle_a", "idle_b", 9);
				TASK_PLAY_ANIM_1(PLAYER::PLAYER_PED_ID3(), "amb@lo_res_idles@", "world_human_maid_clean_lo_res_base", 9);
				AI::SET_ANIM_RATE(PLAYER::PLAYER_PED_ID3(), 0, 0, 0);
				if (ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID3(), "amb@lo_res_idles@", "world_human_maid_clean_lo_res_base", 3))
				{
					if (IronManTimeout < GAMEPLAY::GET_GAME_TIMER())
					{
						int VehicleHandle = PLAYER::PLAYER_PED_ID3();
						int launchDistance = 250;
						int weaponHash = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_SPACE_ROCKET");
						float launchSpeed = 500.0f;
						float Distance = 200;
						float Pitch = CAM::GET_GAMEPLAY_CAM_RELATIVE_PITCH();
						Vector3 Start = ENTITY::_0x7C6339DF(PLAYER::PLAYER_PED_ID3(), PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID3(), 57005));
						Vector3 RelativePos;
						Vector3 endPosition = get_coords_from_cam(80.0);
						//float Start[] = { spawnPosition.x, spawnPosition.y, spawnPosition.z };
						float End[] = { endPosition.x, endPosition.y, endPosition.z };
						GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_1(Start.x, Start.y, Start.z, End[0], End[1], End[2], launchDistance, 0, weaponHash, PLAYER::PLAYER_PED_ID3(), 1, 1, launchSpeed);
						IronManTimeout = GAMEPLAY::GET_GAME_TIMER() + 2000;
					}
				}
			}
			else if (isPressed(Button_R2))
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 1, 0, 0, 0, 0);
				TASK_PLAY_ANIM_1(PLAYER::PLAYER_PED_ID3(), "skydive@base", "ragdoll_to_free_idle", 8);
			}
			else
			{
				TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "skydive@parachute@", "chute_idle");
				TASK_PLAY_ANIM_1(PLAYER::PLAYER_PED_ID3(), "amb@world_human_muscle_flex@arms_at_side@idle_a", "idle_b", 9);
				AI::SET_ANIM_RATE(PLAYER::PLAYER_PED_ID3(), 0, 0, 0);
				//AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID3(), dict1, anim1, false);

			}

			if (isPressed(Button_X))
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 0, -1, 0, 0, 0);
				if (isPressed(Button_R2))
				{
					APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 1, 0, 0, 0, 0);
					TASK_PLAY_ANIM_1(PLAYER::PLAYER_PED_ID3(), "skydive@base", "ragdoll_to_free_idle", 8);
				}
			}

			Vector3 Left = ENTITY::_0x7C6339DF(PLAYER::PLAYER_PED_ID3(), PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID3(), 14201));
			Vector3 Right = ENTITY::_0x7C6339DF(PLAYER::PLAYER_PED_ID3(), PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID3(), 52301));
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
			GRAPHICS::START_PARTICLE_FX_LOOPED_AT_COORD("muz_assault_rifle", Left.x, Left.y, Left.z, 0, +90, 0, 1.0f, 0, 0, 0, true);
			//GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", PLAYER::PLAYER_PED_ID3(), -0.1, -0.3, 0, 0, +90, 0, 1.0f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
			GRAPHICS::START_PARTICLE_FX_LOOPED_AT_COORD("muz_assault_rifle", Right.x, Right.y, Right.z, 0, +90, 0, 1.0f, 0, 0, 0, true);
			//GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", PLAYER::PLAYER_PED_ID3(), +0.1, -0.3, 0, 0, +90, 0, 1.0f, 0, 0, 0);




			if (Analog(189) == 0)// Up
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 0.5 * JetpackSpeed, 0, 0, 0, 0);
			}
			else if (Analog(189) == 254)
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, -0.5 * JetpackSpeed, 0, 0, 0, 0);
			}
			else if (Analog(188) == 0)//Left
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), -0.5 * JetpackSpeed, 0, 0, 0, 0, 0);
			}
			else if (Analog(188) == 254)
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0.5 * JetpackSpeed, 0, 0, 0, 0, 0);
			}

			if (isPressed(Button_A))
			{

				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 0, 0.5, 0, 0, 0);
				if (isPressed(Button_R2))
				{
					APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 1, 0, 0, 0, 0);
					TASK_PLAY_ANIM_1(PLAYER::PLAYER_PED_ID3(), "skydive@base", "ragdoll_to_free_idle", 8);
				}
			}




			drawText("Press R3 to Finish Jetpack", 0, 0.5, 0.1, 0.4, 0.4, 200, 0, 0, 255, true);
			if (isPressed(Button_R3))
			{
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());
				SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID3(), false);
				isIronMan = false;
			}

		}
		else
		{
			if (isPressed(Button_L3))
			{
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());
				SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID3(), false);
				isIronMan = true;
			}
		}
	}
	if (FlashMan)
	{
		FlashMod();
	}
	if (isSneak)
	{
		if (!isPressed(Button_L2))
		{
			Vector3 InCamera = get_rotation_from_cam(4);
			ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 0, 0, InCamera.z, true, true);
		}
		if (isPressed(Button_R2))
		{
			//AI::CLEAR_PED_SECONDARY_TASK(PLAYER::PLAYER_PED_ID3());
			//AI::TASK_AIM_GUN_SCRIPTED(PLAYER::PLAYER_PED_ID(), GAMEPLAY::GET_HASH_KEY("SCRIPTED_GUN_TASK_PLANE_WING"), 1, 1);
		}


		if (Analog(189) <= 126)// Up
		{
			if (!sneakmode)
			{
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "move_crawl", "onfront_fwd", 8, -4, -1, 9, 0, false, false, false);
				sneakmode = true;
			}
		}
		else if (Analog(189) >= 128)
		{
			if (!sneakmode)
			{
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "move_crawl", "onfront_bwd", 8, -4, -1, 9, 0, false, false, false);
				sneakmode = true;
			}
		}
		else
		{
			sneakmode = false;
			AI::TASK_AIM_GUN_SCRIPTED(PLAYER::PLAYER_PED_ID(), GAMEPLAY::GET_HASH_KEY("SCRIPTED_GUN_TASK_PLANE_WING"), 1, 1);
		}
	}
	if (AnimationLoad)
	{
		if (!STREAMING::HAS_ANIM_DICT_LOADED("amb@world_human_muscle_flex@arms_at_side@idle_a"))
		{
			STREAMING::REQUEST_ANIM_DICT("amb@world_human_muscle_flex@arms_at_side@idle_a");
		}
		if (!STREAMING::HAS_ANIM_DICT_LOADED("skydive@freefall"))
		{
			STREAMING::REQUEST_ANIM_DICT("skydive@freefall");
		}
		if (!STREAMING::HAS_ANIM_DICT_LOADED("skydive@parachute@"))
		{
			STREAMING::REQUEST_ANIM_DICT("skydive@parachute@");
		}
		if (!STREAMING::HAS_ANIM_DICT_LOADED("amb@lo_res_idles@"))
		{
			STREAMING::REQUEST_ANIM_DICT("amb@lo_res_idles@");
		}

		if (!STREAMING::HAS_ANIM_DICT_LOADED("move_crawl"))
		{
			STREAMING::REQUEST_ANIM_DICT("move_crawl");
		}
		if (!STREAMING::HAS_ANIM_DICT_LOADED("move_ped_crouched"))
		{
			STREAMING::REQUEST_ANIM_SET("move_ped_crouched");
		}
		if (!STREAMING::HAS_ANIM_DICT_LOADED("move_ped_crouched_strafing"))
		{
			STREAMING::REQUEST_ANIM_SET("move_ped_crouched_strafing");
		}
		if (!STREAMING::HAS_ANIM_DICT_LOADED("misstrevor3"))
		{
			STREAMING::REQUEST_ANIM_DICT("misstrevor3");
		}
		if (!STREAMING::HAS_ANIM_DICT_LOADED("melee@knife@streamed_core"))
		{
			STREAMING::REQUEST_ANIM_DICT("melee@knife@streamed_core");
		}
	}
	if (JetPackMod)
	{
		AnimationLoad = true;
		if (isJetpack)
		{
			if (!PED::IS_PED_FALLING(PLAYER::PLAYER_PED_ID()))
			{
				PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_ID(), true);
				PED::RESET_PED_RAGDOLL_TIMER(PLAYER::PLAYER_PED_ID());
				//AI::CLEAR_PED_TASKS(PLAYER::PLAYER_PED_ID());
			}
		}
		if (!ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(JetpackID, PLAYER::PLAYER_PED_ID()))
		{
			if (!STREAMING::HAS_MODEL_LOADED(0x5D9377D3))
			{
				STREAMING::REQUEST_MODEL(0x5D9377D3);
			}
			/*if (!STREAMING::HAS_ANIM_DICT_LOADED(Animation3))
			{
			STREAMING::REQUEST_ANIM_DICT(Animation3);
			}
			if (!STREAMING::HAS_ANIM_DICT_LOADED(Animation5))
			{
			STREAMING::REQUEST_ANIM_DICT(Animation5);
			}*/
		}
		//combat@chg_stance
	}
	else
	{
		isJetpack = false;
	}

	//melee@knife@streamed_core
	//misstrevor3
	if (JetPackMod)
	{
		if (!isJetpack)
		{
			drawText("Press L3 to Start Jetpack", 0, 0.5, 0.1, 0.4, 0.4, 200, 0, 0, 255, true);
			if (isPressed(Button_L3))
			{
				NoRagdoll = true;
				SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), true);
				isJetpack = true;
			}
		}
		else
		{

			if (isPressed(Button_R2))
			{
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "misstrevor3", "horny_biker_loop", 1.0f, 0.0f, -1, 9, 8, 0, 0, 0);
				//AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "melee@knife@streamed_core", "knife_short_range_90", 1.0f, 0.0f, -1, 9, 8, 0, 0, 0);
				//AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), dict1, anim1, 1.0f, 0.0f, -1, 9, 8, 0, 0, 0);
				//AI::SET_ANIM_RATE(PLAYER::PLAYER_PED_ID3(), 5, 0, 0);
				//AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID3(), dict1, anim1, false);

				//AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), Animaton1, Animation2, 1000.0f, 0.0f, 0, 16, 8, 0, 0, 0);
				//AI::SET_ANIM_RATE(PLAYER::PLAYER_PED_ID3(), 5, 0, 0);
				//AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID3(), Animaton1, Animation2, false);

				//AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID3(), "amb@world_human_push_ups@male@base", "base", false);
				Vector3 spawnPosition = get_coords_from_cam(5.0);
				Vector3 endPosition = get_coords_from_cam(80.0);
				float Start[] = { spawnPosition.x, spawnPosition.y, spawnPosition.z };
				float End[] = { endPosition.x, endPosition.y, endPosition.z };
				GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_1(Start[0], Start[1], Start[2], End[0], End[1], End[2], 250, 1, GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_PLAYER_HUNTER"), PLAYER::PLAYER_PED_ID(), 1, 0, 400);

			}
			else if (isPressed(Button_L2))
			{
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "misstrevor3", "horny_biker_loop", 1.0f, 0.0f, -1, 9, 8, 0, 0, 0);
			}
			else
			{
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "misstrevor3", "horny_biker_loop", 1.0f, 0.0f, -1, 9, 8, 0, 0, 0);
				AI::SET_ANIM_RATE(PLAYER::PLAYER_PED_ID3(), 0, 0, 0);
				//AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID3(), dict1, anim1, false);
				//AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), Animation3, Animation4, 1.0f, 0.0f, -1, 9, 8, 0, 0, 0);
				//AI::SET_ANIM_RATE(PLAYER::PLAYER_PED_ID3(), 5, 0, 0);
			}

			if (isPressed(Button_X))
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 0, -1, 0, 0, 0);
			}
			//AI::TASK_PARACHUTE(PLAYER::PLAYER_PED_ID3(), false);
			//AI::TASK_PAUSE(PLAYER::PLAYER_PED_ID3(), 1000);
			//PED::SET_PED_KEEP_TASK(PLAYER::PLAYER_PED_ID3(), true);
			//WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID3(), GAMEPLAY::GET_HASH_KEY("GADGET_PARACHUTE"), 1, true, true);
			//AI::TASK_PARACHUTE(PLAYER::PLAYER_PED_ID3(), false);
			//PED::_0xA819680B(PLAYER::PLAYER_PED_ID3());

			//AI::SET_ANIM_RATE(PLAYER::PLAYER_PED_ID3(), 5, 0, 0);
			//AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID3(), "mp_am_hold_up", "handsup_base", false);
			//AI::CLEAR_PED_SECONDARY_TASK(PLAYER::PLAYER_PED_ID3());
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
			//GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("muz_assault_rifle", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 24818, 3.0f, 0, 0, 0);
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", JetpackID, -0.1, 0, 0, 0, +90, 0, 1.0f, 0, 0, 0);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_fbi5a");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_fbi5a");
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_assault_rifle", JetpackID, +0.1, 0, 0, 0, +90, 0, 1.0f, 0, 0, 0);
			//ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID3(), true);
			//Vector3 Infront = GetCoordsInfrontOfCam(JetpackSpeed * 0.3);
			Vector3 InCamera = get_rotation_from_cam(4);
			ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 0, 0, InCamera.z, true, true);
			drawText("Move Stick !!!!", 0, 0.5, 0.125, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("□ Down", 0, 0.5, 0.150, 0.4, 0.4, 200, 0, 0, 255, true);
			drawText("X Up", 0, 0.5, 0.175, 0.4, 0.4, 200, 0, 0, 255, true);



			if (Analog(189) == 0)// Up
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 0.5 * JetpackSpeed, 0, 0, 0, 0);
			}
			else if (Analog(189) == 254)
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, -0.5 * JetpackSpeed, 0, 0, 0, 0);
			}
			else if (Analog(188) == 0)//Left
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), -0.5 * JetpackSpeed, 0, 0, 0, 0, 0);
			}
			else if (Analog(188) == 254)
			{
				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0.5 * JetpackSpeed, 0, 0, 0, 0, 0);
			}

			if (isPressed(Button_A))
			{

				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 0, 0.5, 0, 0, 0);
			}




			drawText("Press R3 to Finish Jetpack", 0, 0.5, 0.1, 0.4, 0.4, 200, 0, 0, 255, true);
			if (isPressed(Button_R3))
			{
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());
				SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID3(), false);
				isJetpack = false;
			}
			if (Timeout <= GAMEPLAY::GET_GAME_TIMER())
			{

				APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 0, 0.4, 0, 0, 0);
				Timeout = GAMEPLAY::GET_GAME_TIMER() + 50;
			}
		}
	}

	if (ALLESP)
	{
		bool Name;
		bool Line;
		bool Box;
		switch (ALLESPTYPES)
		{
		case 0:Line = true; break;
		case 1:Name = true; break;
		case 2:Box = true; break;
		case 3:Line = true; Name = true; break;
		case 4:Line = true; Box = true; break;
		case 5:Line = true; Box = true; Name = true; break;
		}
		for (int i = 0; i < 16; i++)
		{
			if (PLAYER::IS_PLAYER_PLAYING(i))
			{
				int playerid = PLAYER::GET_PLAYER_PED(i);
				if (playerid != PLAYER::PLAYER_PED_ID())
				{
					if (Name)
					{
						NameESP(i);
					}
					if (Line)
					{
						Linie(i);
					}
					if (Box)
					{
						BoxESP(i);
					}
				}
			}
		}
	}
	if (Camera_fixed)
	{
		Vector3 InCamera = get_rotation_from_cam(4);
		ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 0, 0, InCamera.z, true, true);
	}
	if (SuperRun)
	{
		if (CONTROLS::IS_CONTROL_PRESSED(2, Button_A))
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), true, 0.0f, 0.5f, 0.0, 0.0f, 0.0f, 0.0f, true, true, true, true, false, true);
			SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), FALSE);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), FALSE); PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), 1);
		}
	}
	if (Superjump)
	{
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());
	}
	if (ExplosiveMelee)
	{
		GAMEPLAY::SET_EXPLOSIVE_MELEE_THIS_FRAME(PLAYER::PLAYER_ID());
	}
	if (FellisFreeze)
	{
		FellisProtect();
	}
	if (fellisfreezebutton)
	{
		drawText("Press L1 to Remove Fellis Wheel", 0, 0.5, 0.1, 0.4, 0.4, 255, 255, 255, 255, true);
		if (isPressed(Button_L1))
		{

			Vector3 MyLocation = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			ENTITY::CREATE_MODEL_HIDE(MyLocation.x, MyLocation.y, MyLocation.z, 0, GAMEPLAY::GET_HASH_KEY("prop_Ld_ferris_wheel"), true);
		}
	}
	if (propattach)
	{

		if (GAMEPLAY::GET_GAME_TIMER() >= PropAttachTime)
		{
			PropAttachProtect();
			PropAttachTime = GAMEPLAY::GET_GAME_TIMER() + 800;
		}
	}
	int SINESINE = 0x3F039CBA;
	if (ClearAreaPed)
	{
		Ped Closest;
		Vector3 MyPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
		if (PED::GET_CLOSEST_PED(MyPos.x, MyPos.y, MyPos.z, 10000, true, true, &Closest, false, true, -1))
		{
			if (ENTITY::GET_ENTITY_MODEL(Closest) == 0x3F039CBA)
			{
				if (ENTITY::DOES_ENTITY_EXIST(Closest))
				{
					PED::DELETE_PED(&Closest);
				}
			}
		}
		//0x3F039CBA

		if (PED::GET_CLOSEST_PED(MyPos.x, MyPos.y, MyPos.z, 10000, true, true, &Closest, false, true, -1))
		{
			if (ENTITY::GET_ENTITY_MODEL(Closest) == 0x3F039CBA)
			{
				ENTITY::DELETE_ENTITY(&Closest);
				ENTITY::DETACH_ENTITY(Closest, 1, 1);
				ENTITY::SET_ENTITY_COORDS(Closest, 6000.0, 6000.0, 6000.0, true, false, false, true);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Closest, 1, 1);
				PED::DELETE_PED(&Closest);
				ENTITY::DELETE_ENTITY(&Closest);
				PRINT("フリーズモデルを削除しました。", 400);
			}
		}
	}
	if (Pedattach)
	{
		Vector3 MyPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
		//0x3F039CBA
		Ped Closest;

		if (PED::GET_CLOSEST_PED(MyPos.x, MyPos.y, MyPos.z, 10000000, true, true, &Closest, false, true, -1))
		{
			if (ENTITY::GET_ENTITY_MODEL(Closest) == 0x3F039CBA)
			{
				ENTITY::DELETE_ENTITY(&Closest);
				ENTITY::DETACH_ENTITY(Closest, 1, 1);
				ENTITY::SET_ENTITY_COORDS(Closest, 6000.0, 6000.0, 6000.0, true, false, false, true);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Closest, 1, 1);
				PED::DELETE_PED(&Closest);
				ENTITY::DELETE_ENTITY(&Closest);
				PRINT("フリーズモデルを削除しました。", 400);
			}
		}
		if (PED::GET_CLOSEST_PED(MyPos.x, MyPos.y, MyPos.z, 5, true, true, &Closest, false, true, -1))
		{
			if (ENTITY::GET_ENTITY_MODEL(Closest) == ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()))
			{
				if (!PED::IS_PED_A_PLAYER(Closest))
				{
					if (ENTITY::DOES_ENTITY_EXIST(Closest))
					{
						if (NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Closest))
						{
							if (NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Closest))
							{
								ENTITY::DETACH_ENTITY(Closest, 1, 1);
								ENTITY::SET_ENTITY_COORDS(Closest, 6000.0, 6000.0, 6000.0, true, false, false, true);
								ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Closest, 1, 1);
								PED::DELETE_PED(&Closest);
								ENTITY::DELETE_ENTITY(&Closest);

							}
						}
					}
				}
			}
		}
	}
	if (VehicleAttachProtect)
	{
		VehicleFreezeProtection();
	}
	if (ClearAreaAll)
	{
		Vector3 MyPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
		GAMEPLAY::CLEAR_AREA(MyPos.x, MyPos.y, MyPos.z, 111, true, 0, 0, 0);
		GAMEPLAY::CLEAR_AREA_OF_COPS(MyPos.x, MyPos.y, MyPos.z, 111, 0);
		GAMEPLAY::CLEAR_AREA_OF_OBJECTS(MyPos.x, MyPos.y, MyPos.z, 111, 0);
		GAMEPLAY::CLEAR_AREA_OF_VEHICLES(MyPos.x, MyPos.y, MyPos.z, 111, 0, 0, 0, 0, 0);
		GAMEPLAY::CLEAR_AREA_OF_PEDS(MyPos.x, MyPos.y, MyPos.z, 111, 0);
		GAMEPLAY::CLEAR_AREA_OF_PROJECTILES(MyPos.x, MyPos.y, MyPos.z, 111, 0);
	}
	if (Godmode)
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 1);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(PLAYER::PLAYER_PED_ID(), 0);
		ENTITY::SET_ENTITY_PROOFS(PLAYER::PLAYER_PED_ID(), 1, 1, 1, 1, 1, 1, 1, 1);
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);
	}
	else
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), 0);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(PLAYER::PLAYER_PED_ID(), 1);
		ENTITY::SET_ENTITY_PROOFS(PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 0, 0);
		ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), false);
	}

	if (invisible)
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true);
	}

	if (NoCops)
	{
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
	}
	else
	{
		NoCops = false;
	}

	if (MoneyLoopInt)
	{
		Vector3 MyCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 0);
		float Loc[3];
		STREAMING::REQUEST_MODEL(0x113FD533);
		if (STREAMING::HAS_MODEL_LOADED(0x113FD533) == 1)
		{
			OBJECT::CREATE_AMBIENT_PICKUP(0xCE6FDD6B, MyCoords.x, MyCoords.y, MyCoords.z, 0, money_, 0x113FD533, 0, 1);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(0x113FD533);
		}
	}
	if (Money40K)
	{
		Vector3 mycoords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), true);
		STREAMING::REQUEST_MODEL(0x113FD533);
		if (STREAMING::HAS_MODEL_LOADED(0x113FD533) == 1)
		{
			OBJECT::CREATE_AMBIENT_PICKUP(0xCE6FDD6B, mycoords.x, mycoords.y, mycoords.z + 1, 0, 40000, 0x113FD533, 0, 1);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(0x113FD533);
		}
	}
	if (SPECTATE)
	{
		AIWanderCar(PLAYER::PLAYER_PED_ID());
		SpectateMode(true, PLAYER::GET_PLAYER_PED(selectedPlayer));
	}
	if (sendCash1)
	{
		sendCash(selectedPlayer, 20000);
	}
	if (resetPlayerView1)
	{
		if (GAMEPLAY::GET_GAME_TIMER() > Timer9)
		{
			resetPlayerView(selectedPlayer);
			PTFXTime = GAMEPLAY::GET_GAME_TIMER() + 550;
		}
	}

	if (keyboardActive)
	{
		if (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 1)
		{
			keyboardActive = false;
			switch (keyboardAction)
			{
			case 0: //addIntOption
				*keyboardVar = StoI(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT());
				break;
			}
		}
		else if (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 2 || GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 3)
		{
			keyboardActive = false;
		}
	}
}
#pragma endregion

int Hook()
{
	monitorButtons();
	otherLoop();
	optionCount = 0;
	switch (submenu)
	{
#pragma region ------Main Menu------

	case Closed:
		if (isOpenMenu)
		{
			drawText("NightMare SPRX", bannerTextFont, 0.85, 0.13f, 0.4f, 0.4f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity, true);
			drawText("Open Menu Square + Right", bannerTextFont, 0.85, 0.15f, 0.4f, 0.4f, bannerRectRed, bannerRectGreen, bannerRectBlue, bannerRectOpacity, true);
			GRAPHICS::DRAW_RECT(0.85, 0.15f, 0.17f, 0.08, 0, 0, 0, 20);
		}
		break;
	case Main_Menu:
		addTitle("Main Menu");
		addSubmenuOption("Player List", Players_List, "プレイヤーリスト");
		addSubmenuOption("Self Options", Self_Options, "セルフオプション");
		addSubmenuOption("Fun Menu", Fun_Menu, "ファンメニュー");
		addSubmenuOption("Weapon Options", Weapon_Options, "武器オプション");
		addSubmenuOption("Vehicle Spawner", vehiclespawner, "車をスポーン");
		addSubmenuOption("Vehicle Options", Vehicle_Options, "車両オプション");
		addSubmenuOption("Teleport Options", Teleport_Option, "テレポートオプション");
		addSubmenuOption("Models Menu", Models_Menu, "モデルメニュー");
		addSubmenuOption("Animation Menu", Animation_Menu, "アニメーションメニュー");
		//addSubmenuOption("Upper body Animation", Upper_body_Animation, "上半身アニメーション");
		addSubmenuOption("Particle FX", Particle_Options, "パーティクルオプション");
		//addSubmenuOption("Object Menu", Object_Menu, "オブジェクトメニュー");
		addSubmenuOption("World Options", Weather_Options, "ワールドオプション");
		addSubmenuOption("Protection", Protection_Menu, "プロテクション");
		addSubmenuOption("Settings", Settings, "設定");
		addSubmenuOption("Apply All Option", ApplyAll, "一括");
		addSubmenuOption("Credit", Credit, "クレジット");
		break;
#pragma endregion
#pragma region ApplyAll
	case ApplyAll:
		addTitle("Apply All Option");
		addOption("Apply All Protections");
		addOption("Apply All Off Protections");
		addOption("Apply All SelfOptions");
		addOption("Apply All Off SelfOptions");
		switch (getOption())
		{
		case 1: PTFX = true; PTFXs(); TaskP = true; Tasks(); ClearTaskP = true; ClearTaskPs(); ExplosionP = true; ExplosionPs(); KickVote = true; KickVotes(); VSpeedP = true; VSpeedPs(); RemoveWeaponP = true; RemoveWeaponProtect(); freezecar = true; ClearAreaAll = true; ClearAreaPed = true; RPprotection = true; propattach = true; FellisFreeze = true; SendFreezeProtect(); break;
		case 2: PTFX = false; PTFXs(); TaskP = false; Tasks(); ClearTaskP = false; ClearTaskPs(); ExplosionP = false; ExplosionPs(); KickVote = false; KickVotes(); VSpeedP = false; VSpeedPs(); RemoveWeaponP = false; RemoveWeaponProtect(); freezecar = false; ClearAreaAll = false; ClearAreaPed = false; RPprotection = false; propattach = false; FellisFreeze = false; SendFreezeProtect(); break;
		case 3: Godmode = true; NoCops = true; Superjump = true; RunSpeed = true; NoRagdoll = true; NoTarget = true; Functions(); break;
		case 4: Godmode = false; NoCops = false; Superjump = false; RunSpeed = false; NoRagdoll = false; NoTarget = false; Functions(); break;
		}
		break;
#pragma endregion
#pragma region Credit
	case Credit:
		addTitle("Credit");
		addOption("~r~りんご");
		addOption("~f~すたんぐさん");
		addOption("~p~レミリア");
		addOption("~f~Aqua");
		addOption("~y~Angelworld_-");
		addOption("~o~Reeze");
		addOption("~f~NGU");
		switch (getOption())
		{
		case 1: drawNotification("~r~@xR1nGo_HDMD: SPRX Dev Leader"); break;
		case 2: drawNotification("~f~@71s9: SPRX Dev Sub Leader"); break;
		case 3: drawNotification("~p~@Rem1ria_Modder: デバッガー"); break;
		case 4: drawNotification("~f~@AvSq_z: デバッガー + GTAVのロード画面の画像加工"); break;
		case 5: drawNotification("~y~@angelworld20 : GTAVのロード画面の画像加工"); break;
		case 6: drawNotification("~o~@Reeze_HDMD: SPRXの負担軽減処理担当"); break;
		case 7: drawNotification("~f~NGU"); break;
		}
		break;
#pragma endregion 

#pragma region Vehicle Options
	case Vehicle_Options:
		addTitle("Vehicle Options");
		addOption("Change Color ~r~+");
		addOption("Fix Vehicle");
		addBoolOption("God Mode Vehicle", VehicleGodMode);
		addBoolOption("No Collision", hasNoCollision);
		addBoolOption("Invisible Vehicle", carinvstext);
		addBoolOption("Drive on Air", MeinCode);
		addBoolOption("Drive on Water", MeinCodewat);
		addBoolOption("Lower Ride", LowerWhip);
		addBoolOption("Boost (L3)", L3boost);
		addIntOption("Open Vehicle Door", &vehdoor, 0, 6, true);
		addOption("Vehicle Neons Menu ~r~+");
		WechselBoolIntFunction("Vehicle Jetpack", "Speed", isvehJetpack, &vehjetpackvar, 1, 4, 1);
		switch (getOption())
		{
		case 1: changeSubmenu(Paint_Vehicle); break;
		case 2: uint Vehicle = PED::GET_VEHICLE_PED_IS_USING(PLAYER::GET_PLAYER_PED(PLAYER::PLAYER_ID())); break;
		case 3: VehicleGodMode = !VehicleGodMode; Functions(); break;
		case 4: hasNoCollision = !hasNoCollision; Functions(); break;
		case 5: carinvstext = !carinvstext; Functions(); break;
		case 6: LowerWhip = !LowerWhip; break;
		case 7: L3boost = !L3boost; break;
		case 8: VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), vehdoor, 0, 1); break;
		case 9: changeSubmenu(Neons); break;
		case 10:isvehJetpack = !isvehJetpack; break;
		}
		break;
#pragma endregion

#pragma region Paint Vehicle
	case Paint_Vehicle:
		addTitle("Change Color");
		addIntOption("Change Color Red", &redcolor, 0, 255, true);
		addIntOption("Change Color Green", &greencolor, 0, 255, true);
		addIntOption("Change Color Blue", &bluecolor, 0, 255, true);
		addIntOption("Change Pearl color", &Pearlescant, 0, 255, true);
		addOption("Apply Primary Color");
		addOption("Apply Secondary Color");
		addOption("Apply Pearl Color");
		addOption("Apply Chrome");
		addOption("Apply Gold");
		addOption("Apply Aluminum");
		addOption("Apply Black Steel");
		addOption("Apply Epsilon Blue");
		int VEHICLE = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
		switch (getOption())
		{
		case 5: VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), redcolor, greencolor, bluecolor); break;
		case 6: VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), redcolor, greencolor, bluecolor); break;
		case 7: VEHICLE::SET_VEHICLE_EXTRA_COLOURS(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), Pearlescant, RimColor); break; //RimColor
		case 8: VEHICLE::SET_VEHICLE_MOD_KIT(VEHICLE, 0); VEHICLE::SET_VEHICLE_COLOURS(VEHICLE, 120, 120); PRINT("Color: ~b~Chrome", 2000); break;
		case 9: VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(VEHICLE); VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(VEHICLE); VEHICLE::SET_VEHICLE_MOD_KIT(VEHICLE, 0); VEHICLE::SET_VEHICLE_COLOURS(VEHICLE, 37, 37); PRINT("Color: ~b~Gold", 2000); break;
		case 10: VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(VEHICLE); VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(VEHICLE); VEHICLE::SET_VEHICLE_MOD_KIT(VEHICLE, 0); VEHICLE::SET_VEHICLE_COLOURS(VEHICLE, 119, 119); PRINT("Color: ~b~Aluminum", 2000); break;
		case 11: VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(VEHICLE); VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(VEHICLE); VEHICLE::SET_VEHICLE_MOD_KIT(VEHICLE, 0); VEHICLE::SET_VEHICLE_COLOURS(VEHICLE, 118, 118); PRINT("Color: ~b~Black Steel", 2000); break;
		case 12: VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(VEHICLE); VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(VEHICLE); VEHICLE::SET_VEHICLE_MOD_KIT(VEHICLE, 0); VEHICLE::SET_VEHICLE_COLOURS(VEHICLE, 157, 157); PRINT("Color: ~b~Epsilon Blue", 2000); break;
		}
		break;
#pragma endregion

#pragma region Neons
	case Neons:
		addTitle("Neons Menu");
		addBoolOption("Neons", Neons_);
		addNeonsOption("~b~Blue", 0, 43, 255);
		addNeonsOption("~r~Red", 255, 0, 0);
		addNeonsOption("~g~Green", 0, 255, 43);
		addNeonsOption("~o~Orange", 255, 128, 0);
		addNeonsOption("~p~Purple", 196, 0, 255);
		switch (getOption())
		{
		case 1: Neons_ = !Neons_; break;
		}
		break;
#pragma endregion

#pragma region Particle Part
	case Particle_part:
		addTitle("一部位のみパーティクル");
		addIntOption2("ピンクのもやもや", &loopptfxType1, 0, 26, true, true);
		addIntOption2("花火", &loopptfxType2, 0, 26, true, true);
		switch (getOption())
		{
		case 1: loopptfxmod = !loopptfxmod; break;
		case 2: loopptfxmod2 = !loopptfxmod2;  break;
		}
		break;
#pragma endregion
#pragma region Particle All
	case Particle_All:
		addTitle("全身パーティクル");
		addCheckBool("電気", effectp);
		addCheckBool("血しぶき", effectp2);
		addCheckBool("ピンクのもやもや", effectp3);

		switch (getOption())
		{
		case 1: effectp = !effectp; effectp2 = false; effectp3 = false;  break;
		case 2: effectp2 = !effectp2; effectp = false; effectp3 = false;  break;
		case 3: effectp3 = !effectp3; effectp = false; effectp2 = false;  break;
		}
		break;
#pragma endregion
#pragma region World Option
	case Weather_Options:
		addTitle("World Options");
		addOption("CLEAR", "晴れ");
		addOption("Extra Sunny", "すごい晴れ");
		addOption("Light Snow", "明るい雪");
		addOption("Blizzard", "すごい雪");
		addOption("Fog", "煙たい");
		addOption("Neutral", "異世界");
		addOption("Cloudy", "曇り");
		addOption("Overcast", "曇り");
		addOption("Smog", "スモッグ");
		addOption("Thunder", "雷雨");
		addOption("Rain", "雨");
		addOption("Snow", "雪");
		addBoolOption("Snow on Ground", Tunable_Snow, "銀世界");
		addOption("Noon", "昼");
		addOption("Night", "夜");
		switch (getOption())
		{
		case 1:  GAMEPLAY::SET_OVERRIDE_WEATHER("CLEAR"); break;
		case 2:  GAMEPLAY::SET_OVERRIDE_WEATHER("EXTRASUNNY"); break;
		case 3:  GAMEPLAY::SET_OVERRIDE_WEATHER("SNOWLIGHT"); break;
		case 4:  GAMEPLAY::SET_OVERRIDE_WEATHER("BLIZZARD"); break;
		case 5:  GAMEPLAY::SET_OVERRIDE_WEATHER("FOGGY"); break;
		case 6:  GAMEPLAY::SET_OVERRIDE_WEATHER("NEUTRAL"); break;
		case 7:  GAMEPLAY::SET_OVERRIDE_WEATHER("CLOUDS"); break;
		case 8:  GAMEPLAY::SET_OVERRIDE_WEATHER("OVERCAST"); break;
		case 9:  GAMEPLAY::SET_OVERRIDE_WEATHER("SMOG"); break;
		case 10: GAMEPLAY::SET_OVERRIDE_WEATHER("THUNDER"); break;
		case 11: GAMEPLAY::SET_OVERRIDE_WEATHER("RAIN"); break;
		case 12: GAMEPLAY::SET_OVERRIDE_WEATHER("SNOW"); break;
		case 13: Tunable_Snow = !Tunable_Snow; Functions(); break;
		case 14: NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(12, 0, 0); break;
		case 15: NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(0, 0, 0); break;
		}
		break;
#pragma endregion
#pragma region Particle Option
	case Particle_Options:
		addTitle("Particle FX");
		addSubmenuOption("一部位のみパーティクル", Particle_part);
		addSubmenuOption("全身パーティクル", Particle_All);
		addCheckBool("パーティクルマン", particleman);
		addIntOption("カスタムパーティクルマン", &PXType, 0, 8, true);
		switch (getOption())
		{
		case 3:particleman = !particleman; break;
		}
		break;
#pragma endregion
#pragma region AnimationMenu
	case Animation_Menu:
		addTitle("Animations");
		addCheckBool("Is Upper Body Animation?", upperbody);
		addCharSwap("Animation 1", Animation, &AnimationV, 0, 3);
		switch (getOption())
		{
		case 1:upperbody = !upperbody; break;
		case 2:isAnimationStart = true; break;

		}
		break;
#pragma endregion
#pragma region Costume Editor
	case OutfitEditor:
		addTitle("コスチュームエディター");
		addCharSwap("変える場所", Familying, &Familyingvar, 0, 14, true);
		addIntOption("番号", &HATSvar, 0, 99, true);
		addIntOption("テクスチャの番号", &HATSTexturevar, 0, 20, true);
		addOption("反映");
		switch (getOption())
		{
		case 2:Modelo(Family[Familyingvar], HATSvar, HATSTexturevar, 0); break;
		case 3:Modelo(Family[Familyingvar], HATSvar, HATSTexturevar, 0); break;
		case 4:Modelo(Family[Familyingvar], HATSvar, HATSTexturevar, 0); break;
		}
		break;
#pragma endregion
#pragma region Model Menu
	case Models_Menu:
		addTitle("モデルチェンジャー");
		addSubmenuOption("コスチュームエディター", OutfitEditor);
		addOption("初期モデル 女");
		addOption("初期モデル 男");
		addOption("ゾンビ");
		addOption("サル");
		addOption("ピエロ");
		addOption("ハッカー");
		addOption("ポゴ");
		addOption("ムキムキ");
		addOption("警官");
		addOption("ビッチガール");
		addOption("ビーチガール");
		addCheckBool("モデルを黒化", blackmodel);
		switch (getOption())
		{
		case 2:ChangeModelHash = true; ModelHash = 0x9C9EFFD8; break;
		case 3:ChangeModelHash = true; ModelHash = 0x705E61F2; break;
		case 4: ChangeModelHash = true; ModelHash = GAMEPLAY::GET_HASH_KEY("u_m_y_zombie_01");  break;
		case 5: ChangeModelHash = true; ModelHash = GAMEPLAY::GET_HASH_KEY("A_C_Chimp"); break;
		case 6: ChangeModelHash = true; ModelHash = GAMEPLAY::GET_HASH_KEY("S_M_Y_Clown_01"); break;
		case 7: ChangeModelHash = true; ModelHash = GAMEPLAY::GET_HASH_KEY("S_M_Y_Dealer_01"); break;
		case 8: ChangeModelHash = true; ModelHash = GAMEPLAY::GET_HASH_KEY("U_M_Y_pogo_01"); break;
		case 9: ChangeModelHash = true; ModelHash = GAMEPLAY::GET_HASH_KEY("S_M_Y_Marine_02"); break;
		case 10: ChangeModelHash = true; ModelHash = GAMEPLAY::GET_HASH_KEY("S_F_Y_Cop_01"); break;
		case 11: ChangeModelHash = true; ModelHash = GAMEPLAY::GET_HASH_KEY("A_F_Y_Juggalo_01"); break;
		case 12: ChangeModelHash = true; ModelHash = GAMEPLAY::GET_HASH_KEY("a_f_y_beach_01"); break;
		case 13: blackmodel = !blackmodel; break;
		}
		break;
#pragma endregion
#pragma region Favorite
	case favorite:
		addTitle("Favorite");
		addOption("Airport");
		addOption("Maze Bank");
		addOption("Trevor's Air Field");
		addOption("Grapeseed Air Field");
		addOption("Mount Chiliad");
		addOption("The Edge Of The Map");
		addOption("In the Sky");
		addOption("Island");
		addOption("Fort Zancundo");
		switch (getOption())
		{
		case 1: TeleportToLoc(-1102.2910, -2894.5160, 13.9467); break;
		case 2: TeleportToLoc(-74.9424, -818.63446, 326.1743); break;
		case 3: TeleportToLoc(1741.4960, 3269.2570, 41.6014); break;
		case 4: TeleportToLoc(2124.0330, 4800.5860, 41.0031); break;
		case 5: TeleportToLoc(413.4706, 5572.8210, 779.6819); break;
		case 6: TeleportToLoc(62.88864, 7245.876, 2.15644); break;
		case 7: TeleportToLoc(-74.3454, -821.6638, 2590.2230); break;
		case 8: TeleportToLoc(17.8323, 7630.0140, 13.5068); break;
		case 9: TeleportToLoc(-2012.847f, 2956.527f, 32.8101f); break;
		}
		break;
#pragma endregion

#pragma region Shops
	case Shops:
		addTitle("Shops");
		addOption("Ponsonbys");
		addOption("Ammunation");
		addOption("LS Customs");
		addOption("Beekers Garage");
		switch (getOption())
		{
		case 1: TeleportToLoc(-159.2996, -304.3292, 39.7333); break;
		case 2: TeleportToLoc(247.3652, -45.8777, 69.9411); break;
		case 3: TeleportToLoc(-371.40, -130.12, 38.65); break;
		case 4: TeleportToLoc(175.98, 6603.27, 31.38); break;
		}
		break;
#pragma endregion

#pragma region Buildings
	case Buildings:
		addTitle("Buildings");
		addOption("FIB Building Inside");
		addOption("IAA Building Inside");
		addOption("Apartment");
		addOption("Michaels House");
		addOption("10 cars garage");
		addOption("Trevors Office Strip Club");
		addOption("Trevors House");
		addOption("Franklins House (New)");
		addOption("Franklins House (OLD)");
		addOption("Lesters House");
		addOption("Trevors Meth Lab");
		addOption("Humane Lab");
		addOption("Police Station");
		addOption("Bank Vault");
		addOption("Blaine County Bank");
		addOption("Waynes Cousins House");
		addOption("Airport Terminal");
		addOption("Humane Lab 2");
		addOption("Inside casino");
		addOption("Behind bar in strip club");
		addOption("Empty closed in room");
		addOption("Gun range");
		addOption("Miltary Base Tower");
		addOption("Franklins Bedroom");
		addOption("Building Wireframe");
		addOption("Garage (second room)");
		addOption("Humane Lab (Waterbody)");
		addOption("Garage car tools");
		addOption("Private Garage");
		addOption("Union Depository");
		addOption("Inside random building");
		addOption("Alta St apartment Enter");
		addOption("Private Hangout(Creek)");
		addOption("Inside Jonny Tung");
		addOption("Fort Zancudo (Inside Tower)");
		addOption("Elysian Island Base");
		addOption("Prison");
		addOption("Prison Yard");
		addOption("Trapped in box");
		addOption("Prison Tower");
		addOption("IOU Tower");
		addOption("Creek (Mountain Tower)");
		switch (getOption())
		{
		case 1: TeleportToLoc(135.5220, -749.0009, 260.0000); break;
		case 2:	TeleportToLoc(113.9319, -618.5334, 206.0465); break;
		case 3: TeleportToLoc(-878.5444, -445.3285, 125.6352); break;
		case 4: TeleportToLoc(-813.6030, 179.4738, 72.1589); break;
		case 5: TeleportToLoc(229.3570, -992.9106, -98.9999); break;
		case 6: TeleportToLoc(97.2707, -1290.9940, 29.2688); break;
		case 7: TeleportToLoc(1974.7580, 3819.4570, 33.4363); break;
		case 8: TeleportToLoc(7.4150, 535.5486, 176.0279); break;
		case 9:	TeleportToLoc(-14.9693, -1436.4430, 31.1185); break;
		case 10: TeleportToLoc(1248.1830, -1728.1040, 56.0000); break;
		case 11: TeleportToLoc(1397.5240, 3607.4230, 38.9419); break;
		case 12: TeleportToLoc(3617.2310, 3739.8710, 28.6901); break;
		case 13: TeleportToLoc(441.2090, -983.1136, 30.6895); break;
		case 14: TeleportToLoc(255.5819, 217.6896, 101.6834); break;
		case 15: TeleportToLoc(-104.2072, 6469.2650, 31.6267); break;
		case 16: TeleportToLoc(-1159.0340, -1521.1800, 10.6327); break;
		case 17: TeleportToLoc(-1561.5250, -3232.3460, 26.3361); break;
		case 18: TeleportToLoc(3531.7030, 3673.6420, 28.1212); break;
		case 19: TeleportToLoc(937.4756, 42.4248, 80.8990); break;
		case 20: TeleportToLoc(126.1211, -1278.5130, 29.2696); break;
		case 21: TeleportToLoc(134.1213, -1289.5810, 29.2696); break;
		case 22: TeleportToLoc(22.8730, -1073.8800, 29.7970); break;
		case 23: TeleportToLoc(-2501.9390, 3309.8980, 91.9665); break;
		case 24: TeleportToLoc(2.6014, 525.5179, 170.6172); break;
		case 25: TeleportToLoc(-129.3836, -951.1331, 218.8816); break;
		case 26: TeleportToLoc(222.5924, -968.1003, -98.9999); break;
		case 27: TeleportToLoc(3524.0700, 3711.9500, 20.9913); break;
		case 28: TeleportToLoc(211.2500, -962.7500, 24.9667); break;
		case 29: TeleportToLoc(3601.3830, 3668.7930, 33.8718); break;
		case 30: TeleportToLoc(-20.4297, -685.1064, 32.3381); break;
		case 31: TeleportToLoc(-278.6654, 6185.8690, 31.4861); break;
		case 32: TeleportToLoc(-266.0524, -968.6304, 31.2243); break;
		case 33: TeleportToLoc(-463.6622, 4483.6540, 36.0373); break;
		case 34: TeleportToLoc(-879.0649, -247.7447, 40.1937); break;
		case 35: TeleportToLoc(-2356.0940, 3248.6450, 101.4505); break;
		case 36: TeleportToLoc(574.3914, -3121.3220, 18.7687); break;
		case 37: TeleportToLoc(1679.0490, 2513.7110, 45.5649); break;
		case 38: TeleportToLoc(1640.7910, 2530.0440, 45.5649); break;
		case 39: TeleportToLoc(-655.6762, -160.8557, 42.1480); break;
		case 40: TeleportToLoc(1541.6290, 2470.1400, 62.8751); break;
		case 41: TeleportToLoc(-866.4975, -251.8129, 40.1102); break;
		case 42: TeleportToLoc(2784.6060, 6000.8770, 357.2007); break;
		}
		break;
#pragma endregion

#pragma region Glitches
	case Glitches:
		addTitle("Glitches");
		addOption("Under the bridge");
		addOption("FIB Roof");
		addOption("City Wall");
		addOption("Mechanic");
		addOption("Apartment wall");
		addOption("Under Parking Garage");
		addOption("Another Bridge");
		addOption("The Ocean");
		addOption("Unknown Building");
		addOption("Location for Deer");
		addOption("Wall Glitch");
		addOption("Wall Glitch 2");
		addOption("Building GLitch");
		addOption("Building wall");
		addOption("Building wall 2");
		addOption("Under the bridge");
		addOption("400k Apartment");
		switch (getOption())
		{
		case 1: TeleportToLoc(-721.6599, -1000.6510, 23.5455); break;
		case 2: TeleportToLoc(135.9541, -749.8984, 258.1520); break;
		case 3: TeleportToLoc(-254.9432, -147.3534, 42.7314); break;
		case 4: TeleportToLoc(546.0365, -183.3675, 54.4982); break;
		case 5: TeleportToLoc(220.5997, -1006.7580, -98.9999); break;
		case 6: TeleportToLoc(-359.4955, -781.3347, 31.7852); break;
		case 7: TeleportToLoc(-615.2986, -577.6776, 30.6693); break;
		case 8: TeleportToLoc(4499.9470, -3946.6510, -100016.9000); break;
		case 9: TeleportToLoc(-11.5143, -691.2623, 54.7947); break;
		case 10: TeleportToLoc(159.1143, 3226.0780, 40.9329); break;
		case 11: TeleportToLoc(-134.4927, -667.1520, 36.177); break;
		case 12: TeleportToLoc(-135.0291, -668.5015, 35.0691); break;
		case 13: TeleportToLoc(-91.6870, 33.0948, 71.4655); break;
		case 14: TeleportToLoc(-224.9551, -79.6451, 80.0779); break;
		case 15: TeleportToLoc(-224.9551, -79.6451, 80.0779); break;
		case 16: TeleportToLoc(721.6599, -1000.6510, 23.5455); break;
		case 17: TeleportToLoc(-785.4869, 314.7161, 90.2189); break;
		}
		break;
#pragma endregion

#pragma region Roofs
	case Roofs:
		addTitle("Roofs");
		addOption("FIB Building roof");
		addOption("IAA Building roof");
		addOption("Police parking roof");
		addOption("400K apartment roof");
		addOption("Rooftop Rumble drop off");
		addOption("Mount Chiliad");
		addOption("Los Santos Customs roof 1");
		addOption("Roof (Scenic View)");
		addOption("BuildingTop/ledge(Scenic)");
		addOption("Rooftop brawl");
		addOption("Cannibal Camp roof");
		addOption("Under Roof");
		addOption("Emergency HeliPad");
		addOption("Police HeliPad");
		addOption("Maze Bank Helipad");
		addOption("Ontop of Vinewood Logo");
		addOption("Hospital Helipads");
		addOption("In the Sky");
		addOption("Above Clouds");
		addOption("HeliPad");
		switch (getOption())
		{
		case 1: TeleportToLoc(135.5220, -749.0009, 266.6102); break;
		case 2: TeleportToLoc(130.6824, -634.9446, 262.8510); break;
		case 3: TeleportToLoc(334.2101, -1644.7660, 98.4960); break;
		case 4: TeleportToLoc(-769.8004, 331.2076, 234.2694); break;
		case 5: TeleportToLoc(-1025.4400, 697.5500, 161.7000); break;
		case 6: TeleportToLoc(449.3493, 5568.5380, 796.1312); break;
		case 7: TeleportToLoc(-354.8817, -150.8228, 74.2741); break;
		case 8: TeleportToLoc(-118.8224, -195.4712, 94.8497); break;
		case 9: TeleportToLoc(201.9763, 567.6195, 131.6675); break;
		case 10: TeleportToLoc(-149.7626, -943.4242, 254.0494); break;
		case 11: TeleportToLoc(-1143.1450, 4951.1880, 230.1531); break;
		case 12: TeleportToLoc(-1883.7390, 2062.8590, 144.8217); break;
		case 13: TeleportToLoc(308.9238, -1458.9330, 46.5095); break;
		case 14: TeleportToLoc(369.4300, -1601.8320, 36.9502); break;
		case 15: TeleportToLoc(-74.9424, -818.63446, 326.1743); break;
		case 16: TeleportToLoc(776.8780, 1175.6080, 345.9564); break;
		case 17: TeleportToLoc(322.0547, -1448.0380, 46.5095); break;
		case 18: TeleportToLoc(-74.3454, -821.6638, 2590.2230); break;
		case 19: TeleportToLoc(-73.4489, -833.5170, 5841.4240); break;
		case 20: TeleportToLoc(-736.7500, -1437.750, 5.0003); break;
		}
		break;
#pragma endregion

#pragma region Land
	case Land:
		addTitle("Land");
		addOption("Mount Chiliad");
		addOption("Airport");
		addOption("Trevor's Air Field");
		addOption("Grapeseed Air Field");
		addOption("Fort Zacundo");
		addOption("Crane 1");
		addOption("Crane 2");
		addOption("Crane 3");
		addOption("Mount Chiliad Jump");
		addOption("Mount Chiliad Skyline");
		addOption("Mount Chiliad");
		addOption("Construction site on building");
		addOption("Far away beach");
		addOption("Coral Reef");
		addOption("Far end beach island");
		addOption("Far Island");
		addOption("Underwater ocean");
		addOption("Dirtbike trail");
		addOption("Zancudo river");
		addOption("Private hangout 2");
		addOption("Creek swimming");
		switch (getOption())
		{
		case 1: TeleportToLoc(495.0000, 5586.0000, 794.0000); break;
		case 2: TeleportToLoc(-1102.2910, -2894.5160, 13.9467); break;
		case 3: TeleportToLoc(1741.4960, 3269.2570, 41.6014); break;
		case 4: TeleportToLoc(2124.0330, 4800.5860, 41.0031); break;
		case 5: TeleportToLoc(-2012.8470, 2956.5270, 32.8101); break;
		case 6: TeleportToLoc(-167.9822, -1001.9265, 296.2061); break;
		case 7: TeleportToLoc(-120.3508, -977.8608, 304.2478); break;
		case 8: TeleportToLoc(-119.859985, -976.43866, 306.3385); break;
		case 9: TeleportToLoc(430.2037, 5614.7340, 766.1684); break;
		case 10: TeleportToLoc(446.5404, 5571.6250, 781.1896); break;
		case 11: TeleportToLoc(413.4706, 5572.8210, 779.6819); break;
		case 12: TeleportToLoc(-147.5930, -964.6947, 254.1325); break;
		case 13: TeleportToLoc(178.3295, 7041.8220, 1.8671); break;
		case 14: TeleportToLoc(106.6972, 7282.0550, 1.8821); break;
		case 15: TeleportToLoc(-125.6544, 7271.8940, 16.7366); break;
		case 16: TeleportToLoc(17.8323, 7630.0140, 13.5068); break;
		case 17: TeleportToLoc(103.4720, 7744.1870, -158.1106); break;
		case 18: TeleportToLoc(-1202.0910, 2802.4400, 14.8256); break;
		case 19: TeleportToLoc(-558.9814, 2945.7010, 14.5917); break;
		case 20: TeleportToLoc(-597.9525, 4475.2910, 25.6890); break;
		case 21: TeleportToLoc(-589.5320, 4395.3900, 18.1481); break;
		}
		break;
#pragma endregion

#pragma region Teleport Options
	case Teleport_Option:
		addTitle("Teleport Options");
		addOption("Favorite ~r~+");
		addOption("Shops ~r~+");	
		addOption("Buildings ~r~+");
		addOption("Glitches ~r~+");
		addOption("Roofs ~r~+");
		addOption("Land ~r~+");
		switch (getOption())
		{
		case 1: changeSubmenu(favorite); break;
		case 2: changeSubmenu(Shops); break;
		case 3: changeSubmenu(Buildings); break;
		case 4: changeSubmenu(Glitches); break;
		case 5: changeSubmenu(Roofs); break;
		case 6: changeSubmenu(Land); break;
		}
		break;
#pragma endregion
#pragma region
	case spawnattachlist:
		
		addTitle("スポーンアタッチリスト");
		//ESPing2(currentOption - 1);
		addPlayerList();
		if (optionPress && PLAYER::IS_PLAYER_PLAYING(currentOption - 1))
		{
			char View[400];
			AttachSpawnPlayer = currentOption - 1;
			snprintf(View, sizeof(View), "Selected！:%s", PLAYER::GET_PLAYER_NAME(AttachSpawnPlayer));
			PRINT(View, 1000);
		}
		break;
#pragma endregion
#pragma region
	case VehicleSpawnOptions:
		addTitle("スポーン時の設定");
		addCheckBool("フルアップグレード", MaxUpgrades);
		addCheckBool("スポーン時に乗る", TeleAutomIncar);
		addCheckBool("無敵", spawnGodmode);
		addCheckBool("クローム", ChromeSpawn);
		addCheckBool("透明化", isinvisible);
		addCheckBool("スポーンアタッチ", isattach);
		addSubmenuOption("スポーン or アタッチ", spawnattachlist);
		addCheckBool("プレイヤーへスポーン", spawnplayer);
		switch (getOption())
		{
		case 1:
			MaxUpgrades = !MaxUpgrades;
			break;
		case 2:
			TeleAutomIncar = !TeleAutomIncar;
			break;
		case 3:
			spawnGodmode = !spawnGodmode;

			break;
		case 4:
			ChromeSpawn = !ChromeSpawn;
			break;
		case 5:isinvisible = !isinvisible; break;
		case 6:isattach = !isattach; spawnplayer = false; break;
		case 8:spawnplayer = !spawnplayer; isattach = false; break;
		}
		break;
#pragma endregion

#pragma region ------Vehicle Spawner------
	case vehiclespawner:
		addTitle("車をスポーン");
		addSubmenuOption("出す時の設定", VehicleSpawnOptions);
		addCarCharSwap("スーパー", SuperCar, SuperCar2, &SuperCarVar, 0, 10);
		addCarCharSwap("スポーツ", sport, sport2, &sportVar, 0, 28);
		addCarCharSwap("クラシック", classic, classic2, &classicVar, 0, 19);
		addCarCharSwap("クラシックスポーツ", Muscle, Muscle2, &MuscleVar, 0, 11);
		addCarCharSwap("セダン", Sedan, Sedan2, &SedanVar, 0, 22);
		addCarCharSwap("クーペ", Coupes, Coupes2, &CoupeVar, 0, 9);
		addCarCharSwap("コンパクト", Compact, Compact2, &CompactVar, 0, 7);
		addCarCharSwap("SUV", Suv, Suv2, &SuvVar, 0, 27);
		addOption("Page 2");
		switch (getOption())
		{
		case 10:changeSubmenu(vehiclespawner2); break;
		}break;
	case vehiclespawner2:
		addTitle("車をスポーンPage 2");
		addCarCharSwap("トラック", Tracks, Tracks2, &TrackVar, 0, 18);
		addCarCharSwap("バン", Van, Van2, &VanVar, 0, 30);
		addCarCharSwap("警察車両", Emergency, Emergency2, &EmergencyVar, 0, 17);
		addCarCharSwap("サービス", Service, Service2, &ServiceVar, 0, 5);
		addCarCharSwap("ミリタリー", Military, Military2, &MilitaryVar, 0, 6);
		addCarCharSwap("バイク", Bike, Bike2, &BikeVar, 0, 20);
		addCarCharSwap("自転車", Bicycle, Bicycle2, &BicycleVar, 0, 6);
		addCarCharSwap("宣伝用", Commercial, Commercial2, &CommercialVar, 0, 38);
		addCarCharSwap("ヘリ", Helicopter, Helicopter2, &HelicopterVar, 0, 13);
		addCarCharSwap("プレーン", Plane, Plane2, &PlaneVar, 0, 18);
		addCarCharSwap("ボート", Boat, Boat2, &BoatVar, 0, 13);
		addCarCharSwap("トレーラー", Trailers, Trailers2, &TrailersVar, 0, 26, false, true);
		switch (getOption())
		{

		}
		break;
#pragma endregion

#pragma region ------Players List------
	case Players_List:
		addTitle("Players List");
		//ESPing2(currentOption - 1);
		addPlayerList();
		if (optionPress && PLAYER::IS_PLAYER_PLAYING(currentOption - 1))
		{
			selectedPlayer = currentOption - 1;
			changeSubmenu(Player_Options);
		}
		break;
#pragma endregion

#pragma region ------Player Options------
	case Player_Options:
		addTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		addOption("Teleport Options");
		addOption("Good Options");
		addOption("Bad Options");
		addOption("Force Animations");
		addOption("Stat Editor");
		switch (getOption())
		{
		case 1: changeSubmenu(PTeleport_Option); break;
		case 2: changeSubmenu(Good_Options); break;
		case 3: changeSubmenu(Bad_Options); break;
		case 4: changeSubmenu(F_Animations); break;
		case 5:changeSubmenu(StatMenu); break;
		}
		break;
#pragma endregion

#pragma region
	case StatMenu:
		addTitle("Stat Editor");
		addOption("Skill MAX");
		addOption("Combat Rolling");
		addOption("Disable Combat Rolling");
		switch (getOption())
		{
		case 1:PlayerStatMAX(selectedPlayer); break;
		case 2:RollingInfinite(selectedPlayer); break;
		case 3:RollingInfiniteNot(selectedPlayer); break;
		}
		break;
#pragma endregion

#pragma region Force Animation
	case F_Animations:
		addTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		addOption("~r~Stop Animation");
		DoAdminations("Mugic", "amb@world_human_yoga@male@base", "base");
		DoAdminations("Honor Me Now", "oddjobs@bailbond_mountain", "excited_idle_c");
		DoAdminations("Hands Up", "mp_am_hold_up", "handsup_base");
		DoAdminations("Get Fucked", "rcmpaparazzo_2", "shag_loop_poppy");
		DoAdminations("Private Striper Dance", "mini@strip_club@private_dance@part1", "priv_dance_p1");
		DoAdminations("Pole Dance Striper", "mini@strip_club@pole_dance@pole_dance1", "pd_dance_01");
		DoAdminations("Push-Ups", "amb@world_human_push_ups@male@base", "base");
		DoAdminations("Sit-Ups", "amb@world_human_sit_ups@male@base", "base");
		DoAdminations("Car Sex", "ODDJOBS@ASSASSINATE@VICE@SEX", "frontseat_carsex_loop_m");
		DoAdminations("Electrocution", "ragdoll@human", "electrocute");
		DoAdminations("Suicide", "mp_suicide", "pistol");
		DoAdminations("Shower", "mp_safehouseshower@male@", "male_shower_idle_b");
		DoAdminations("Jogging", "rcmfanatic1", "jogging_up");
		DoAdminations("Balcony Fight", "random@domestic", "balcony_fight_male");
		DoAdminations("Sex Giver", "rcmpaparazzo_2", "shag_loop_a");
		DoAdminations("Dog Fucking", "missfra0_chop_find", "hump_loop_chop");
		DoAdminations("Dog Pissing", "creatures@rottweiler@move", "pee_right_idle");
		DoAdminations("ped on fire", "ragdoll@human", "on_fire");
		DoAdminations("lap dance sex", "mini@strip_club@pole_dance@pole_dance2", "pd_dance_02");
		DoAdminations("lap dance player", "mp_am_stripper", "lap_dance_player");
		DoAdminations("damage vehicle", "stungun@sitting", "damage_vehicle");
		DoAdminations("sex loop male", "mini@prostitutes@sexnorm_veh", "sex_loop_male");
		DoAdminations("Prostitute sex loop", "mini@prostitutes@sexnorm_veh", "sex_loop_prostitute");
		DoAdminations("Prostitute male", "mini@prostitutes@sexnorm_veh", "bj_loop_male");
		DoAdminations("Prostitute looping", "mini@prostitutes@sexnorm_veh", "bj_loop_prostitute");
		DoAdminations("Prostitute Car Sex 1", "random@drunk_driver_2", "cardrunksex_loop_m");
		DoAdminations("Prostitute Car Sex 2", "random@drunk_driver_2", "cardrunksex_loop_f");
		DoAdminations("Dog Sitting", "creatures@retriever@amb@world_dog_sitting@base", "base");
		DoAdminations("emp", "amb@world_human_statue@base", "base");
		DoAdminations("human Statue Muscles", "amb@world_human_statue@enter", "enter");
		DoAdminations("Human Statue Thanks", "amb@world_human_statue@idle_b", "idle_f");
		DoAdminations("zumbie Special", "special_ped@zombie@monologue_8@monologue_8g", "whosayscosplayisa_6");
		DoAdminations("trevor jerking", "switch@trevor@jerking_off", "trev_jerking_off_loop");
		DoAdminations("trevor Crazy", "switch@trevor@drunk_howling", "loop");
		DoAdminations("refen Loop", "misstrevor2ig_5c", "plead_loop");
		DoAdminations("approach no ball", "mini@golfai", "putt_approach_no_ball");
		DoAdminations("Barrel Rolls Loop", "misschinese2_barrelroll", "barrel_roll_loop_A");
		switch (getOption())
		{
		case 1: AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::GET_PLAYER_PED(selectedPlayer)); break;
		}
		break;
#pragma endregion

#pragma region TP Option
	case PTeleport_Option:
		addTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		addOption("Teleport To Player");
		addOption("Force Teleport To Me");
		addOption("Force Teleport To Airport");
		addOption("Force Teleport To Mount Chiliad");
		addOption("Force Teleport To LSC");
		addOption("Force Teleport To Fort Zancundo");
		addOption("Force Teleport To Sky");
		addOption("Force Teleport To Job Pilot");
		addOption("Force Teleport To Maze Bank");
		switch (getOption())
		{
		case 1: TeleportToPLayer(selectedPlayer); break;
		case 2: teleportToMeFoot(selectedPlayer); break;
		case 3: int playerid5 = PLAYER::GET_PLAYER_PED(selectedPlayer); telefoot(playerid5, "mini@strip_club@private_dance@part3", "priv_dance_p3", -1102.2910, -2894.5160, 13.9467); break;
		case 4: int playerid6 = PLAYER::GET_PLAYER_PED(selectedPlayer); telefoot(playerid6, "mini@strip_club@private_dance@part3", "priv_dance_p3", 495.0000, 5586.0000, 794.0000); break;
		case 5: int playerid7 = PLAYER::GET_PLAYER_PED(selectedPlayer); telefoot(playerid7, "mini@strip_club@private_dance@part3", "priv_dance_p3", -382.6777, -100.6703, 38.26721); break;
		case 6: int playerid8 = PLAYER::GET_PLAYER_PED(selectedPlayer); telefoot(playerid8, "mini@strip_club@private_dance@part3", "priv_dance_p3", -2012.847f, 2956.527f, 32.8101f); break;
		case 7: int playerid10 = PLAYER::GET_PLAYER_PED(selectedPlayer); telefoot(playerid10, "mini@strip_club@private_dance@part3", "priv_dance_p3", -75.04851, -819.073669, 2500); break;
		case 8: int playerid11 = PLAYER::GET_PLAYER_PED(selectedPlayer); telefoot(playerid11, "mini@strip_club@private_dance@part3", "priv_dance_p3", -1018.25812, -2699.20459, 13.3915539); break;
		case 9: int playerid12 = PLAYER::GET_PLAYER_PED(selectedPlayer); telefoot(playerid12, "mini@strip_club@private_dance@part3", "priv_dance_p3", -2301.88, 216.14, 167.60); break;
		}
		break;
#pragma endregion

#pragma region Good Option
	case Good_Options:
		addTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		addOption("Drop Options");
		addOption("Clear WantedLevel");
		addOption("Send Cash");
		addBoolOption("Send Cash", sendCash1);
		addOption("Copy Costume");
		addOption("Player Send Message");
		switch (getOption())
		{
		case 1: changeSubmenu(Drop_Option); break;
		case 2: removePlayerWantedLevel(selectedPlayer); break;
		case 3: sendCash(selectedPlayer, 20000); break;
		case 4: sendCash1 = !sendCash1; break;
		case 5: StealClothes = true;
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_teleport", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false); break;
		case 6: changeSubmenu(Player_Send_Message); break;
		}
		break;
#pragma endregion

#pragma region Player Send Message
	case Player_Send_Message:
		addTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		addOption("Custom MSG");
		addOption("ありがとう!!");
		addOption("ごめんなさい");
		addOption("やめるね～");
		addOption("お疲れ様～");
		addOption("はい");
		addOption("何か入れて欲しいメッセージがあれば連絡ください。");
		switch (getOption())
		{
		case 1: KeyboardIndex = 1; GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(0, "PM_NAME_CHALL", "", "", "", "", "", 150); KEYBOARD_PROMPT = true; break;
		case 2: TextMessages(selectedPlayer, "ありがとう!!", 7); break;
		case 3: TextMessages(selectedPlayer, "ごめんなさい", 7); break;
		case 4: TextMessages(selectedPlayer, "やめるね～", 7); break;
		case 5: TextMessages(selectedPlayer, "お疲れ様～", 7); break;
		case 6: TextMessages(selectedPlayer, "はい", 7); break;
		}
		break;
#pragma endregion

#pragma region Drop Option
	case Drop_Option:
		addTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		addBoolOption("40K MONEY", Money40K);
		addIntOption3("40K MONEY Custom", &money_, 1, 40000);
		addOption("Drop Health");
		addOption("Drop Armor");
		addOption("Drop Parachute");
		addOption("Drop Snacks");
		addOption("Drop Ammo");
		addOption("Drop Melee Weapons");
		switch (getOption())
		{
		case 1: Money40K = !Money40K; break;
		case 2: MoneyLoopInt = !MoneyLoopInt; break;
		case 3: dropPickup(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), "prop_ld_health_pack", "PICKUP_HEALTH_STANDARD", 100, "Health", selectedPlayer); break;
		case 4: dropPickup(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), "prop_armour_pickup", "PICKUP_ARMOUR_STANDARD", 100, "Armor", selectedPlayer); break;
		case 5: dropPickup(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), "prop_parachute", "PICKUP_PARACHUTE", 1, "Parachute", selectedPlayer); break;
		case 6: dropPickup(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), "prop_ld_snack_01", "PICKUP_HEALTH_SNACK", 10, "Snacks", selectedPlayer); break;
		case 7: dropPickup(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), "prop_box_ammo01a", "PICKUP_AMMO_BULLET_MP", 10, "Ammo Box", selectedPlayer); break;
		case 8:
			dropPickup(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), "prop_knife", "PICKUP_WEAPON_KNIFE", 10, "Knife", selectedPlayer); break;
			dropPickup(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), "prop_ing_crowbar", "PICKUP_WEAPON_CROWBAR", 10, "Crowbar", selectedPlayer); break;
			dropPickup(ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), "prop_tool_hammer", "PICKUP_WEAPON_HAMMER", 10, "Hammer", selectedPlayer); break;
		}
		break;
#pragma endregion

#pragma region Bad Option
	case Bad_Options:
		addTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		addBoolOption("Explosion loop", explosionloop);
		addOption("Explode Player");
		addBoolOption("Forcefield", forcefield);
		addOption("Clone Player");
		addBoolOption("Freeze Player", frezzeplayer);
		addBoolOption("Attach/Detach Player", PlayerAttached);
		addOption("SWAT Player");
		addOption("Set Chop On Player");
		addOption("Set Lion On Player");
		addOption("Bad Options 2 ~r~+");
		switch (getOption())
		{
		case 1: explosionloop = !explosionloop; break;
		case 2: ExplodePlayer(selectedPlayer); break;
		case 3: forcefield = !forcefield; break;
		case 4: ClonePed(PLAYER::GET_PLAYER_PED(selectedPlayer)); break;
		case 5: frezzeplayer = !frezzeplayer; break;
		case 6: toggleAttach(selectedPlayer); break;
		case 7: spawnsetswat = true; break;
		case 8: spawnsetchop = true; break;
		case 9: spawnsetmtlion = true; break;
		case 10: changeSubmenu(Bad_Options2); break;
		}
		break;
#pragma endregion

#pragma region Bad Option2
	case Bad_Options2:
		addTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		addOption("RP Option ~r~+", "");
		addOption("Give All Weapons");
		addOption("Take All Weapons");
		addOption("Take All Weapons V2");
		addOption("Force Golf");
		addOption("Force Wrestling");
		addOption("Force Darts");
		addBoolOption("ResetPlayerView", resetPlayerView1);
		addOption("ResetPlayerView");
		addOption("Play Sound");
		addBoolOption("Spectate Player", SPECTATE);
		addOption("SetPlayerMental MAX");
		addOption("~r~Modder Kill");
		addOption("~r~Non Host Kick");
		addOption("~r~Freeze Console");
		if (isAdmin)
		{
			addOption("~r~Force Tutorial");
		}
		switch (getOption())
		{
		case 1: changeSubmenu(RP_Options); break;
		case 2: GiveAllWeaponsClients(selectedPlayer); break;
		case 3: TakeAllWeapons(selectedPlayer); break;
		case 4: TakeAllWeaponsV2(selectedPlayer); break;
		case 5: teleportPlayerIntoGolfEvent(selectedPlayer);  break;
		case 6: teleportPlayerIntoArmWrestlingEvent(selectedPlayer);  break;
		case 7: teleportPlayerIntoDartsEvent(selectedPlayer);  break;
		case 8: resetPlayerView1 = !resetPlayerView1;  break;
		case 9: resetPlayerView(selectedPlayer);  break;
		case 10: PlaySound(selectedPlayer, 0); break;
		case 11: SPECTATE = !SPECTATE; Spectateoff(); break;
		case 12: setPlayerMentalStat(selectedPlayer, 133); break;
		case 13: KickFromCar(selectedPlayer); int playerid3 = PLAYER::GET_PLAYER_PED(selectedPlayer); telefoot(playerid3, "mini@strip_club@private_dance@part3", "priv_dance_p3", 2900.238, -9999.34994, -10.8816416); break;
		case 14: kickPlayerNonHost(selectedPlayer); break;
		case 15: doClown = true; if (spawnClown()) { doClown = false; } break;
		case 16: TSEvent2(0x3, selectedPlayer, 20, 0, 0, selectedPlayer); break;
		}
		break;
#pragma endregion

#pragma region RP Option
	case RP_Options:
		addTitle(PLAYER::GET_PLAYER_NAME(selectedPlayer));
		addOption("+10.000RP");
		addOption("+50.000RP");
		addOption("+100.000RP");
		addOption("+1.000.000RP");
		addOption("+15.000.000RP");
		addOption("+999.999.999RP");
		addOption("----------------------");
		addOption("-10.000RP");
		addOption("-50.000RP");
		addOption("-100.000RP");
		addOption("-1.000.000RP");
		addOption("-15.000.000RP");
		addOption("-999.999.999RP");
		addOption("----------------------");
		addIntOption3("Give RP", &RankUp, 1, 1000000);
		addIntOption3("Take RP", &RankDown, 1, 1000000);
		switch (getOption())
		{
		case 1: givePlayerRP(selectedPlayer, 10000); break;
		case 2: givePlayerRP(selectedPlayer, 50000); break;
		case 3: givePlayerRP(selectedPlayer, 100000); break;
		case 4: givePlayerRP(selectedPlayer, 1000000); break;
		case 5: givePlayerRP(selectedPlayer, 15000000); break;
		case 6: givePlayerRP(selectedPlayer, 999999999); break;
		case 8: givePlayerRP(selectedPlayer, -10000); break;
		case 9: givePlayerRP(selectedPlayer, -50000); break;
		case 10: givePlayerRP(selectedPlayer, -100000); break;
		case 11: givePlayerRP(selectedPlayer, -1000000); break;
		case 12: givePlayerRP(selectedPlayer, -15000000); break;
		case 13: givePlayerRP(selectedPlayer, -999999999); break;
		case 15: TSEvent(0xD8, selectedPlayer, -RankUp, 0, 0, selectedPlayer); break;
		case 16: TSEvent(0xD8, selectedPlayer, RankDown, 0, 0, selectedPlayer); break;
		}
		break;
#pragma endregion

#pragma region ------Self Options------
	case Self_Options:
		addTitle("Self Options");
		addCheckBool("God Mode", Godmode, "無敵");
		addCheckBool("Invisibility", invisible, "透明");
		addCheckBool("No Cops", NoCops, "手配度無効");
		addCheckBool("Super Jump", Superjump, "スーパージャンプ");
		addCheckBool("SuperRun", SuperRun, "スーパーラン");
		addCheckBool("Faster Run", RunSpeed, "走る速さ x2");
		addCheckBool("Explosive Melee", ExplosiveMelee,"殴ると爆発");
		addCheckBool("NoRagdoll", NoRagdoll, "転ばなくなる");
		addCheckBool("Off The Radar", OffRadar, "レーダーから消える");
		addCheckBool("Reveal Players", RevealPlayers, "プレイヤー表示");
		addCheckBool("Cops turn blind eye", Copsturnblindeye, "警察が黙認");
		addCheckBool("Boss Mode", hasBossMode, "自分をL2で狙った人を爆破");
		addIntOption("Boss Explode Type", &type_, 1, 30,true, "Boss Modeのタイプ");
		addCheckBool("Mobile Radio", mobileRadio, "歩きながらラジオ");
		addCheckBool("Not Targeted", NoTarget, "ターゲットにされない");
		addCheckBool("Force Field", doForceField_, "自分の周りが爆発");
		addCheckBool("Camera fixed", Camera_fixed, "カメラ固定");
		addOption("Blue Shark", "ブルシャーク");
		addOption("Clean Yourself", "体を綺麗にする");
		addOption("Regenerate Health/Armour", "体力/アーマー回復");
		switch (getOption())
		{
		case 1: Godmode = !Godmode; break;
		case 2: invisible = !invisible; break;
		case 3: NoCops = !NoCops; break;
		case 4: Superjump = !Superjump; break;
		case 5: SuperRun = !SuperRun; break;
		case 6: RunSpeed = !RunSpeed; break;
		case 7: ExplosiveMelee = !ExplosiveMelee;  break;
		case 8: NoRagdoll = !NoRagdoll; Functions(); break;
		case 9: OffRadar = !OffRadar; Functions(); break;
		case 10: RevealPlayers = !RevealPlayers; Functions(); break;
		case 11: Copsturnblindeye = !Copsturnblindeye; Functions(); break;
		case 12:  WriteInt32(ReadInt32(0x1E70394) + 0x24C18, 5); break;
		case 13: hasBossMode = !hasBossMode; break;
		case 14: toggleMobileRadio(); break;
		case 15: NoTarget = !NoTarget; Functions(); break;
		case 16: doForceField_ = !doForceField_; break;
		case 17: Camera_fixed = !Camera_fixed; break;
		case 18: PS3::WriteInt32(PS3::ReadInt32(0x1E70394) + 0x24C18, 5); break;
		case 19:
			PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID()); PED::RESET_PED_VISIBLE_DAMAGE(PLAYER::PLAYER_PED_ID());
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_teleport", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false); break;
		case 20:
			ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 1000); PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 10000);
			STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry1");
			GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry1");
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("scr_alien_teleport", PLAYER::PLAYER_PED_ID(), 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, 1.0, false, false, false); break;
		}
		break;
#pragma endregion

#pragma region ------Fun_Menu------
	case Fun_Menu:
		addTitle("Fun Menu");
		addCheckBool("Load IPL", LDIPL, "ノースヤンクトン等を読み込む");
		addCharSwap("Rader Color", raderjapanese, &RaderColorVar, 0, 8, "左下のレーダーの色を変更する");
		WechselBoolFunction("All ESP", "ESP Types", ALLESP, &ALLESPTYPES, 0, 5, ALLESPCHAR, "画面上に相手の位置を表示する");
		addCheckBool("Black Out", BlackOut, "町全体を暗くする");
		if (JetPackMod)
		{
			WechselBoolIntFunction("JetPack Mod", "Speed", JetPackMod, &JetpackSpeed, 1, 10, 1, "ジェットパックのスピードを変更する");
		}
		else
		{
			WechselBoolIntFunction("JetPack Mod", "Speed", JetPackMod, &JetpackSpeed, 1, 10, 1, "ジェットパック");
		}
		addCheckBool("Sneak Mode", isSneak, "匍匐前進");
		addCheckBool("The Flash Man Mod", FlashMan, "Xでクソ走る、そのまま□で飛ぶ");
		addCheckBool("Crouch Mode", isCa, "しゃがむ");
		addCheckBool("Ironman mod", Psychokinetic_anim, "アイアンマン");
		switch (getOption())
		{
		case 1:LDIPL = !LDIPL; LoadIPL(); break;
		case 2:aaa(); break;
		case 3:ALLESP = !ALLESP; break;
		case 4:BlackOut = !BlackOut; BlackOut2(); break;
		case 5:JetPackMod = !JetPackMod; PutOnJetpack(); break;
		case 6:isSneak = !isSneak; SneakMode(isSneak); break;
		case 7: FlashMan = !FlashMan; FlashMod(); break;
		case 8: isCa = !isCa; ShootTest(); break;
		case 9:Psychokinetic_anim = !Psychokinetic_anim; IronManMOD(); break;
		}
		break;
#pragma endregion

#pragma region Weapon Options
	case Weapon_Options:
		addTitle("Weapon Options");
		addCheckBool("Unlimited Ammo", UnlimitedAmmo, "弾無限");
		addCheckBool("Unlimited Ammo 2", UnlimitedAmmo1, "弾無限(リロードあり)");
		addCheckBool("Hash Gun", hashgun, "ハッシュガン");
		addCheckBool("Delete Gun", deletegun, "デリートガン");
		addCheckBool("Teleport Gun", TeleportGunLooping, "テレポートガン");
		addCheckBool("Money Gun", megivemoneyg, "マネーガン");
		addCheckBool("RapidFire", RapidFire, "ラピッドファイア");
		switch (getOption())
		{
		case 1: UnlimitedAmmo = !UnlimitedAmmo; break;
		case 2: UnlimitedAmmo1 = !UnlimitedAmmo1; UnlimitedAmmo2(); break;
		case 3: hashgun = !hashgun; break;
		case 4: deletegun = !deletegun; break;
		case 5: TeleportGunLooping = !TeleportGunLooping; break;
		case 6: megivemoneyg = !megivemoneyg; break;
		case 7: RapidFire = !RapidFire; break;
		}
		break;
#pragma endregion

#pragma region ------Protections------
	case Protection_Menu:
		addTitle("プロテクション");
		addCheckBool("Anti SpawnEffect", PTFX, "エフェクト削除");
		addCheckBool("Anti Teleport", TaskP, "強制テレポ無効");
		addCheckBool("Anti ClearTask", ClearTaskP, "クリアタスク(アニメーションキル)無効");
		addCheckBool("Anti Explosion", ExplosionP, "他人の爆破エフェクト無効");
		addCheckBool("Anti Unknown(Kick Vote)", KickVote, "不明(KickVote)");
		addCheckBool("Anti ControlVehicle", VSpeedP, "車体操作無効");
		addCheckBool("Anti TakeWeapon", RemoveWeaponP, "武器を取り上げるのを無効");
		addCheckBool("Delete FreezeVehicles", freezecar, "飛行機系とフライングカーを削除");
		addCheckBool("Clear Area of MyLocation", ClearAreaAll, "周りの一般車両と一般市民を削除(軽量化)");
		addCheckBool("Delete AttachedVehicle", VehicleAttachProtect, "アタッチされた車を削除(フライングカーは効きます)");
		addCheckBool("Delete AttachedPed", Pedattach, "アタッチされたペッドを削除(フリーズコンソールは効きます)");
		addCheckBool("Anti FreezeConsole", ClearAreaPed, "フリーズコンソールを防ぐ&された場合通知します。");
		addCheckBool("Anti ForceScript", RPprotection, "強制キック、強制負け犬、強制RP etc...を防ぎます");
		addCheckBool("Hide AttachedProps", propattach, "アタッチされたプロップを自分目線で消します。");
		addCheckBool("Hide AttachedFellisWheel", FellisFreeze, "アタッチされた観覧車を一瞬で不可視化します。");
		addCheckBool("L1 DeleteAttachedFellisWheel", fellisfreezebutton, "L1ボタンでアタッチされた観覧車を消します。");
		addOption("Disable Noidle Kick", "放置キックを無効化します。");
		FreezeCarProtect();
		switch (getOption())
		{
		case 1: PTFX = !PTFX; PTFXs(); break;
		case 2: TaskP = !TaskP; Tasks(); break;
		case 3: ClearTaskP = !ClearTaskP; ClearTaskPs(); break;
		case 4: ExplosionP = !ExplosionP; ExplosionPs(); break;
		case 5: KickVote = !KickVote; KickVotes(); break;
		case 6: VSpeedP = !VSpeedP; VSpeedPs(); break;
		case 7: RemoveWeaponP = !RemoveWeaponP; RemoveWeaponProtect(); break;
		case 8:freezecar = !freezecar; SendFreezeProtect(); break;
		case 9: ClearAreaAll = !ClearAreaAll; break;
		case 10:VehicleAttachProtect = !VehicleAttachProtect; break;
		case 11:Pedattach = !Pedattach; break;
		case 12:ClearAreaPed = !ClearAreaPed; break;
		case 13:RPprotection = !RPprotection; RPprotection2(); break;
		case 14: propattach = !propattach; break;
		case 15:FellisFreeze = !FellisFreeze;  break;
		case 16:fellisfreezebutton = !fellisfreezebutton; break;
		case 17:DisableIdleKick(); break;
		}
		break;
#pragma endregion

#pragma region ------Settings------
	case Settings:
		addTitle("Settings");
		addCheckBool("Open Menu", isOpenMenu, "オープンメニューのガイドを表示");
		addCheckBool("Talking Player", talking);
		addCheckBool("Lobby Info", showInfos);
		addCheckBool("Player UI", selectedUI);
		switch (getOption())
		{
		case 1:isOpenMenu = !isOpenMenu; break;
		case 2: talking = talking; break;
		case 3:showInfos = !showInfos; break;
		case 4:selectedUI = !selectedUI; break;
		}
		break;
#pragma endregion

	}
	resetVars();
	return true;
}
void Thread(uint64_t nothing)
{
	sleep(30000); //Wait until native table initialization
	g_Natives = (Native_s**)FindNativeTableAddress();
	PatchInJump(NativeAddress(0x9FAB6729, true), (int)Hook, false); //IS_PLAYER_ONLINE
	sys_ppu_thread_exit(nothing);
}
int _console_write(const char * s)
{
	uint32_t len;
	system_call_4(403, 0, (uint64_t)s, std::strlen(s), (uint64_t)&len);
	return_to_user_prog(int);
}
extern "C" int PRX_ENTRY()
{
	sys_ppu_thread_t ThreadModuleID;
	sys_ppu_thread_create(&ThreadModuleID, Thread, 0, 10, 0x10, 0, "");
	return SYS_PRX_RESIDENT;
}