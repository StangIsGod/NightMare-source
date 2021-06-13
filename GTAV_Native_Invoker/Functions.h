#pragma region ------メニュー設定------
int bannerTextRed = 255;			//タイトルテキスト赤要素
int bannerTextBlue = 0;			//タイトルテキスト緑要素
int bannerTextGreen = 255;			//タイトルテキスト青要素

int bannerTextFont = 1;				//タイトルテキストフォント
int bannerRectRed = 255;				//タイトルテキスト背景赤要素
int bannerRectBlue = 0;				//タイトルテキスト背景緑要素
int bannerRectGreen = 255;			//タイトルテキスト背景青要素
int bannerRectOpacity = 128;			//タイトルテキスト背景青要素

int subbannerRectRed = 0;			//サブタイトルテキスト背景赤要素
int subbannerRectGreen = 0;			//サブタイトルテキスト背景緑要素
int subbannerRectBlue = 0;			//サブタイトルテキスト背景青要素
int subbannerRectOpacity = 32;     //サブタイトルテキスト背景透明度
int backgroundRed = 0;				//テキスト背景赤要素
int backgroundBlue = 0;				//テキスト背景緑要素
int backgroundGreen = 0;			//テキスト背景青要素

int optionsRed = 255;					//テキスト赤要素
int optionsBlue = 0;				//テキスト緑要素
int optionsGreen = 255;				//テキスト青要素

int optionsFont = 1;				//テキストフォント
int scrollerRed = 255;				//スクロールバー赤要素
int scrollerBlue = 0;				//スクロールバー緑要素
int scrollerGreen = 255;				//スクロールバー青要素
int scrollerOpacity = 45;			//スクロールバー透明度

int indicatorRed = 255;				//?
int indicatorBlue = 0;			//?
int indicatorGreen = 255;			//?

int addPlusRed = 255;				//サブメニューの横のプラス赤要素
int addPlusBlue = 0;				//サブメニューの横のプラス緑要素
int addPlusGreen = 255;				//サブメニューの横のプラス青要素
int addPlusOpacity = 255;			//サブメニューの横のプラス透明度

int infoupdownRed = 255;			//下の上下表示赤要素
int infoupdownBlue = 0;			//下の上下表示緑要素
int infoupdownGreen = 255;			//下の上下表示青要素
int infoupdownOpacity = 255;		//下の上下表示透明度

int GradationRed = 255;
int GradationGreen = 255;
int GradationBlue = 0;
int GradationOpacity = 128;

int indicatorOpacity = 32/*20*/;
int optionsOpacity = 255;
int backgroundOpacity = 32/*20*/;
int bannerTextOpacity = 255;

float menuXCoord = 0.845f;
float textXCoord = 0.73f;
int maxOptions = 20;
#pragma endregion

#pragma region Unknowns
void APPLY_FORCE(Entity entity, float f, float o, float r, float p, float v, float s)
{
	float xyz[3] = { f, o, r };
	float pos[3] = { p, v, s };
	APPLY_FORCE_TO_ENTITY_ALT(entity, 1, xyz, pos, 0, 1, 1, 1, 0, 1);
}
#pragma endregion

#pragma region 比較処理
bool cstrcmp(const char* s1, const char* s2)
{
	while (*s1 && (*s1 == *s2))
		s1++, s2++;
	if (*(const unsigned char*)s1 - *(const unsigned char*)s2 == 0)
		return true;
	else
		return false;
}
int cstrcmp2(const char* s1, const char* s2)
{
	while (*s1 && (*s1 == *s2))
		s1++, s2++;
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
#pragma endregion

#pragma region プレイヤーのステータス変更の元
void setupDefaultStatEvent(int player, int *args) {
	args[0] = 150;
	args[1] = player;
	args[2] = player;//unknown value
	args[3] = 88;//int
	args[4] = 34;//bool
	args[5] = 1;//float
	args[6] = 2949;//int
	args[7] = 228;//bool
	args[8] = 228;//float
	args[9] = 0;//value to set for the int ones. This is added on to the previous value set for the stat
	args[10] = 0;//value to set for the float ones. This is added on to the previous value set for the stat
	args[11] = 0;//value to set for the bool ones
}
bool globalStatVersionType = 1;
void callScriptEvent(int *args, int argsCount, int playerBits)
{
	SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, argsCount, playerBits);
}
void setPlayerStatFloat(int player, int statIndex, float statValue, bool type = globalStatVersionType)
{
	int args[12];
	setupDefaultStatEvent(player, args);
	args[EVENT_STAT_VALUE_INT] = *(unsigned int*)(&statValue);
	args[EVENT_STAT_VALUE_BOOL] = statValue;
	args[EVENT_STAT_VALUE_FLOAT] = statValue;
	if (type == 0)
		args[EVENT_STAT_FLOAT_0] = statIndex;
	else
		args[EVENT_STAT_FLOAT_1] = statIndex;
	unsigned int bits = (1 << player);
	callScriptEvent(args, 12, bits);
}
void setPlayerStatInt(int player, int statIndex, int statValue, bool type = globalStatVersionType)
{
	statValue = 0xFFFFFFFF - statValue + 1;
	int args[12];
	setupDefaultStatEvent(player, args);
	printf("%i\n", args[6]);
	args[EVENT_STAT_VALUE_INT] = *(int*)(&statValue);
	args[EVENT_STAT_VALUE_BOOL] = statValue;
	args[EVENT_STAT_VALUE_FLOAT] = statValue;
	if (type == 0)
		args[EVENT_STAT_INT_0] = statIndex;
	else
		args[EVENT_STAT_INT_1] = statIndex;
	unsigned int bits = (1 << player);
	callScriptEvent(args, 12, bits);
}

#pragma endregion

#pragma region メッセージ表示系
void PRINT(char* Text, int Time = 2000)
{
	UI::_0xF42C43C7("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(Text);
	UI::_0x38F82261(Time, false);
}
void drawNotification(char* msg)
{
	if (menuXCoord == 0.845f)
	{
		UI::_0x574EE85C("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING(msg);
		UI::_SET_NOTIFICATION_MESSAGE("CHAR_SOCIAL_CLUB", "CHAR_SOCIAL_CLUB", true, 4, "NIGHTMARE SPRX", "");
		UI::_DRAW_NOTIFICATION(false, true);
	}
	else
	{
		UI::_0xF42C43C7("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING(msg);
		UI::_0x38F82261(3000, 1);
	}
}
#pragma endregion

#pragma region メモリー書き換え
void SetTunable(int index, int value)
{
	int address = (*(int*)0x1E70374) + 4;
	{
		if (address != 0)
		{
			int temp = address;
			temp += (index * 4);
			*(int*)temp = value;
		}
	}
}
void WriteBytes(int address, char* input, int length)
{

	for (int i = 0; i < length; i++)
	{
		*(char*)(address + (i)) = input[i];
	}

}
int intArray[100];
char byteArray[100];
int* ReadInt(int address, int length)
{
	for (int i = 0; i < length; i++)
	{
		intArray[i] = *(int*)(address + (i * 0x04));
	}
	return intArray;
}
void WriteInt(int address, int* input, int length)
{
	for (int i = 0; i < length; i++)
	{
		*(int*)(intArray + (i * 0x04)) = input[i];
	}
}
void WriteFloat(int address, float input)
{
	*(float*)(address) = input;
}
int32_t write_process(uint64_t ea, const void * data, uint32_t size)
{
	system_call_4(905, (uint64_t)sys_process_getpid(), ea, size, (uint64_t)data);
	return_to_user_prog(int32_t);
}

char* ReadBytes(int address, int length)
{
	for (int i = 0; i < length; i++)
	{
		byteArray[i] = *(char*)(address + (i));
	}
	return byteArray;
}
void WriteString(int address, char* string)
{
	int FreeMem = 0x1D00000;
	int strlength = std::strlen(string);
	char* strpointer = *(char**)FreeMem = string;
	char* StrBytes = ReadBytes(*(int*)FreeMem, strlength);
	WriteBytes(address, StrBytes, strlength);
	*((char*)address + strlength) = '\0';
}
char returnRead[100];
char* ReadString(int address)
{
	memset(&returnRead[0], 0, sizeof(returnRead));
	int strlength = 100;
	char* StrBytes = ReadBytes(address, strlength);
	for (int i = 0; i < strlength; i++)
	{
		if (StrBytes[i] != 0x00)
			returnRead[i] = StrBytes[i];
		else
			break;
	}
	return returnRead;
}
char returnR[100];
const char *ReadString1(int address)
{
	memset(&returnRead[0], 0, sizeof(returnR));
	int strlength = 100;
	const char *StrBytes = ReadBytes(address, strlength);
	for (int i = 0; i < strlength; i++)
	{
		if (StrBytes[i] != 0x00)
			returnRead[i] = StrBytes[i];
		else
			break;
	}
	return returnR;
}
void WriteInt32(int Address, int Input)
{
	*(int*)Address = Input;
}
int ReadInt32(int Address)
{
	return *(int*)Address;
}
void WriteUInt32(int Address, unsigned int Input)
{
	*(unsigned int*)Address = Input;
}
unsigned int ReadUInt32(int Address)
{
	return *(unsigned int*)Address;
}
float floatArray[100];
float* ReadFloats(int address, int length)
{
	for (int i = 0; i < length; i++)
	{
		floatArray[i] = *(float*)(address + (i * 0x04));
	}
	return floatArray;
}
#pragma endregion

#pragma region ------Variables------
Native_s** g_Natives;
int submenu = 0;
int submenuLevel;
int lastSubmenu[20];
int lastOption[20];
int currentOption;
int optionCount;
bool optionPress = false;
bool rightPress = false;
bool leftPress = false;
bool fastRightPress = false;
bool fastLeftPress = false;
bool squarePress = false;
int selectedPlayer;
bool menuSounds = true;
bool keyboardActive = false;
int keyboardAction;
int *keyboardVar = 0;
char *infoText;

bool newTimerTick = true;
int maxTimerCount;
bool newTimerTick2 = true;
int maxTimerCount2;

bool instructions = true;
int instructCount;
int mov;
bool instructionsSetupThisFrame;
bool xInstruction;
bool squareInstruction;
bool lrInstruction;
#pragma endregion

#pragma region メニュー描画系
char NUM[100];
char HST[100];
void DrawSprite(char  *Streamedtexture, char  *textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a) {
	if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(Streamedtexture))
		GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(Streamedtexture, false);
	else
		GRAPHICS::DRAW_SPRITE(Streamedtexture, textureName, x, y, width, height, rotation, r, g, b, a);
}
void drawText(char * text, int font, float x, float y, float scalex, float scaley, int r, int b, int g, int a, bool center,bool unko = false)
{
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scalex, scaley);
	UI::SET_TEXT_COLOUR(r, g, b, a);
	UI::SET_TEXT_WRAP(0.0f, 1.0f);
	UI::SET_TEXT_CENTRE(center);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_OUTLINE();
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(text);
	UI::_DRAW_TEXT(x, y);
}
void drawInfos(char* A, char* B, char* C)
{
	drawText(A, 4, 0.220, 0.840, 0.590, 0.590, 255, 255, 255, 255, false);
	drawText(B, 4, 0.220, 0.870, 0.590, 0.590, 255, 255, 255, 255, false);
	drawText(C, 4, 0.220, 0.750, 0.590, 0.590, 255, 255, 255, 255, false);
}
void drawTalker(char* Text, int Index)
{
	UI::SET_TEXT_FONT(4);
	UI::SET_TEXT_SCALE(0.500, 0.500);
	UI::SET_TEXT_COLOUR(255, 255, 255, 255);
	UI::SET_TEXT_OUTLINE();
	UI::SET_TEXT_WRAP(0, 1);
	UI::SET_TEXT_CENTRE(false);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(Text);
	UI::_DRAW_TEXT(0.070, 0.220 + (Index * 0.028));
}
int getHost()
{
	return NETWORK::NETWORK_GET_HOST_OF_SCRIPT("freemode", -1, 0);
}
char* get_player_name(int id)
{
	char* Name = PLAYER::GET_PLAYER_NAME(id);
	if (PLAYER::IS_PLAYER_PLAYING(id))
	{
		if (PED::_IS_PED_DEAD(PLAYER::GET_PLAYER_PED(id), true))
		{
			char *DeadPed;
			sprintf(DeadPed, "~r~死亡中 : %s", Name);
			return DeadPed;
		}
		else
		{
			return Name;
		}
	}
	else
	{
		return "プレイヤーが存在しません。";
	}
}
char* getHostName()
{
	return get_player_name(getHost());
}
//igur7djc5kfpfw36
int getNumPlayers()
{
	int Amount = 0;
	for (int i = 0; i < 16; i++)
	{
		if (!(strstr(PLAYER::GET_PLAYER_NAME(i), "**Invalid**")))
			Amount++;
	}
	return Amount;
}
int getFreeSpots()
{
	int amount = getNumPlayers();
	return (16 - amount);
}
void draw_showhost()
{
	char* Host = getHostName();
	if (cstrcmp(Host, "**Invilid**"))
		Host = "~r~SinglePlayer";
	snprintf(HST, 100, "Host : ~r~ %s", Host);
	snprintf(NUM, 100, "Null of Session : ~r~ %i", getFreeSpots());
}
#pragma endregion

#pragma region タイマー
void SetupTimer(int tickTime)
{
	if (newTimerTick) {
		maxTimerCount = GAMEPLAY::GET_GAME_TIMER() + tickTime;
		newTimerTick = false;
	}
}
bool IsTimerReady()
{
	return GAMEPLAY::GET_GAME_TIMER() > maxTimerCount;
}
void ResetTimer()
{
	newTimerTick = true;
}
void SetupTimer2(int tickTime)
{
	if (newTimerTick2) {
		maxTimerCount2 = GAMEPLAY::GET_GAME_TIMER() + tickTime;
		newTimerTick2 = false;
	}
}
bool IsTimerReady2()
{
	return GAMEPLAY::GET_GAME_TIMER() > maxTimerCount2;
}
void ResetTimer2()
{
	newTimerTick2 = true;
}
#pragma endregion

#pragma region ------int-------
int RimColor = 0;
int redcolor = 0;
int greencolor = 0;
int bluecolor = 0;
int Pearlescant = 0;
int NeonR = 0;
int NeonG = 0;
int NeonB = 0;
int vehjetpackvar = 2;
int vehdoor = 0;
int AirHeigt;
int FerrisWheele;
int markerRed = 200; //255
int markerGreen = 0; //51
int markerBlue = 0; //255
int markerOpacity = 200;
int frameCache_old;
int frameCache_new;
int frameCache_time;
int fps;
int RankUp;
int RankDown;
int spawnedped;
int loopptfxType1 = 0;
int loopptfxType1_[27] = { SKEL_ROOT, SKEL_Pelvis, SKEL_L_Thigh, SKEL_L_Calf, SKEL_L_Foot, SKEL_L_Toe0, IK_L_Foot, PH_L_Foot, MH_L_Knee, SKEL_R_Thigh, SKEL_R_Calf, SKEL_R_Foot, SKEL_R_Toe0, IK_R_Foot, PH_R_Foot, MH_R_Knee, RB_L_ThighRoll, RB_R_ThighRoll, SKEL_Spine_Root, SKEL_Spine0, SKEL_Spine1, SKEL_Spine2, SKEL_Spine3, SKEL_L_Clavicle, SKEL_L_UpperArm, SKEL_L_Forearm, SKEL_L_Hand };
int loopptfxType2 = 0;
int loopptfxType2_[27] = { SKEL_ROOT, SKEL_Pelvis, SKEL_L_Thigh, SKEL_L_Calf, SKEL_L_Foot, SKEL_L_Toe0, IK_L_Foot, PH_L_Foot, MH_L_Knee, SKEL_R_Thigh, SKEL_R_Calf, SKEL_R_Foot, SKEL_R_Toe0, IK_R_Foot, PH_R_Foot, MH_R_Knee, RB_L_ThighRoll, RB_R_ThighRoll, SKEL_Spine_Root, SKEL_Spine0, SKEL_Spine1, SKEL_Spine2, SKEL_Spine3, SKEL_L_Clavicle, SKEL_L_UpperArm, SKEL_L_Forearm, SKEL_L_Hand };
bool loopptfxmod;
bool loopptfxmod2;
int PTFXTimeout;
int PTFXTimeout2;
int PTFXTimeout3;
int ModelHash;
int RapidFireDelay;
int PropAttachTime = 0;
int FellisTimeout2;
int FellisTimeout;
int type_ = 0;
int money_ = 0;
int KeyboardIndex = 0;

int Timeou2t;
int IronManTimeout;
int HashCoolDown = 0;
int Read_Global_1(int a_uiGlobalID)
{
	int Ptr = *(int*)((0x1E70374 - 0x04) + (((a_uiGlobalID / 0x40000) & 0x3F) * 4));
	if (Ptr != 0)
		return *(int*)(Ptr + ((a_uiGlobalID % 0x40000) * 4));
	return 0;
}
int getPlayerStat(int player, int stat)
{
	return Read_Global_1(1581767 + (player * 306) + stat);
}
int getCash(int player)
{
	return Read_Global_1(1581949 + player * 306);
}
int getPlayerRank(int player)
{
	return getPlayerStat(player, 185);
}
#pragma endregion

#pragma region Hashes
Hash Props[133] = {
	0xB467C540, /*p_spinning_anus_s*/
	0xC2BC19CD, /*p_cs_mp_jet_01_s*/
	0xC42C019A, /*prop_ld_ferris_wheel*/
	0xFB631122, /*prop_bumper_car_01*/
	0x745F3383, /*prop_windmill_01*/
	0xBF8918DE, /*prop_jetski_ramp_01*/
	0xB157C9E4, /*prop_mp_ramp_01*/
	0xF4F1511E, /*prop_mp_ramp_02*/
	0x93948E5E, /*prop_mp_ramp_03*/
	0x6DB9599A, /*prop_bskball_01*/
	0x2E28CA22, /*p_tram_crash_s*/
	0xC079B265, /*prop_beach_fire*/
	0xE8032E4, /*prop_weed_pallet*/
	0x8973A868, /*prop_air_bigradar*/
	0x8E8C7A5B, /*prop_crashed_heli*/
	0xBD8AE007, /*prop_space_pistol*/
	0x6F9939C7, /*prop_ld_toilet_01*/
	0x4B3D240F, /*prop_wheelchair_01*/
	0x961CCC04, /*prop_cs_trolley_01*/
	0xE3BA450, /*Xmas Tree*/
	0x7121AC4, /*Large Xmas Tree*/
	0xB7C30A72,/*prop_tv_06*/
	0xF0A61994, /*prop_bong_01*/
	0xED2E79F0, /*prop_cs_ice_locker*/
	0x37FDC243, /*prop_mp_cone_01*/
	0x35AE1F39, /*prop_air_stair_03*/
	0xF7FCF988, /*prop_bank_vaultdoor*/
	0xE0264F5D,/*prop_roadcone01a*/
	0x6B795EBC, /*prop_alien_egg_01*/
	0x1405669B, /*prop_boxing_glove_01*/
	0x671C5C38, /*prop_boombox_01*/
	0xE6CB661E, /*prop_cs_dildo_01*/
	0xE2BA016F, /*prop_cs_katana_01*/
	0xEE95F258, /*prop_wheel_01*/
	0x52660DBB, /*prop_car_seat*/
	0xC89630B8, /*prop_minigun_01*/
	0xD733DF, /*root_scroll_anim_skel*/
	0xFA686C0E, /*prop_bin_04a*/
	0xF830B63E, /*prop_swiss_ball_01*/
	0xF3565180,/*prop_cs_meth_pipe*/
	0xF15FB3DD,/*prop_beggers_sign_01*/
	0xF046EA37,/*prop_gold_bar*/
	0xEFC4165A, /*prop_food_van_01*/
	0xEDA96F47,/*prop_distantcar_night*/
	0xEB7D0265,/*prop_golf_bag_01b*/
	0xE92E717E,/*prop_carwash_roller_vert*/
	0xE8E8AADA,/*prop_skip_01a*/
	0xE44D5CEC,/*prop_cash_crate_01*/
	0xE3CE09E2, /*prop_dummy_plane*/
	0xE1C17F6F,/*prop_kayak_01*/
	0xD541462D, /*p_ld_soc_ball_01*/
	0xD44295DD, /*p_cablecar_s*/
	0xCB74FE41,/*prop_roller_car_02*/
	0xCB290EE2,/*prop_fbibombplant*/
	0xC79A987E,/*prop_beach_volball01*/
	0xC54C0CD2,/*prop_huge_display_01*/
	0xC381B3E6,/*prop_lifeblurb_02*/
	0xC2451209,/*prop_air_lights_02a*/
	0xC20633F4,/*prop_cup_saucer_01*/
	0xBE862050, /*prop_portacabin01*/
	0xBB314853,/*prop_player_gasmask*/
	0xB60A04EC,/*prop_water_corpse_01*/
	0xB58BE281,/*prop_byard_rowboat1*/
	0xB58259BD,/*prop_security_case_01*/
	0xB39B99E2,/*prop_jet_bloodsplat_01*/
	0xB321DD82,/*prop_cs_leg_chain_01*/
	0xB21C3926,/*prop_hose_2*/
	0xB20E5785, /*prop_sculpt_fix*/
	0xB155FD87,/*prop_entityxf_covered*/
	0xB155B61D,/*prop_drop_armscrate_01*/
	0xA9BD0D16, /*prop_coke_block_01*/
	0xA67C935F,/*prop_bowling_pin*/
	0xA50DDDD0, /*prop_bball_arcade_01*/
	0xA22CEA2,/*prop_cs_bowie_knife*/
	0x9C762726, /*prop_lev_des_barge_02*/
	0x98752042,/*prop_ld_fan_01_old*/
	0x97A58869,/*prop_inflatearch_01*/
	0x927A5723, /*prop_fan_01*/
	0x922C2A43, /*prop_big_shit_01*/
	0x8E146BFF,/*prop_ld_monitor_01*/
	0x8DA1C0E, /*Orange Ball?*/
	0x8AF58425, /*prop_lev_des_barge_01*/
	0x8A451C5C,/*prop_ferris_car_01*/
	0x84898EFE,/*prop_big_shit_02*/
	0x7FFBC1E2, /*prop_dummy_01*/
	0x7F2B2371,/*prop_ld_fireaxe*/
	0x72F54E90,/*prop_jb700_covered*/
	0x72C8A3FB,/*prop_ld_shovel*/
	0x6F204E3A,/*prop_sprink_golf_01*/
	0x6B5FFA1D,/*prop_gravetomb_01a*/
	0x6AD326C2, /*prop_space_rifle*/
	0x699C8FC0,/*prop_ld_bomb_anim*/
	0x678FC2DB, /*prop_wheelchair_01_s*/
	0x60F90AD4,/*prop_cs_heist_bag_02*/
	0x5C1D75A6,/*prop_chair_05*/
	0x5B5C4263, /*prop_chip_fryer*/
	0x5A6DF96A,/*prop_hockey_bag_01*/
	0x58D3B4EA, /*prop_micro_01*/
	0x5869A8F8, /*prop_large_gold*/
	0x5571173D, /*prop_beer_neon_01*/
	0x5411322E,/*prop_ld_haybail*/
	0x532B1DD1, /*prop_rub_trolley01a*/
	0x50A607D0,/*prop_buck_spade_08*/
	0x4DA19524,/*prop_cs_cardbox_01*/
	0x4AF9D1D9,/*prop_shamal_crash*/
	0x478A8882,/*prop_rad_waste_barrel_01*/
	0x456AA864, /*prop_dj_deck_01*/
	0x434BFB7C,/*prop_burgerstand_01*/
	0x40F52369, /*p_v_43_safe_s*/
	0x3DC31836, /*prop_tv_flat_01*/
	0x3C3B89F0,/*prop_cs_bin_01_skinned*/
	0x39885BB5, /*prop_chickencoop_a*/
	0x392D62AA, /*prop_gold_cont_01*/
	0x37D80B0E,/*prop_cheetah_covered*/
	0x369D8410,/*prop_el_guitar_02*/
	0x34D5D3FD, /*prop_golf_bag_01*/
	0x2BE688E0,/*prop_dock_bouy_2*/
	0x2AE13DFA, /*prop_armchair_01*/
	0x29E362FF,/*prop_bucket_01a*/
	0x29CB0F3C, /*prop_armour_pickup*/
	0x240D3E4B,/*prop_pizza_box_01*/
	0x1F811CE1,/*prop_drug_package*/
	0x1F550C17, /*prop_chair_01a*/
	0x1D8A4162,/*prop_hotel_clock_01*/
	0x1AFA6A0A, /*prop_weed_01*/
	0x1AB39621,/*prop_lawnmower_01*/
	0x16A39A90, /*prop_dog_cage_01*/
	0x14E3D6EB, /*prop_inflategate_01*/
	0x113FD533,/*prop_money_bag_01*/
	0x0E8032E4, /*prop_weed_pallet*/
	0x0E3BA450, /*prop_xmas_tree_int*/
	0x07121AC4, /*prop_xmas_ext*/
	0x9F984011,
};
#pragma endregion

#pragma region Particle

void LoopPtfxMod()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
	if (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("scr_rcbarry2"))
	{
		GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
	}
	else
	{
		float f1;
		f1 = 1.0;
		GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
		GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, loopptfxType1_[loopptfxType1], 0.1f, 0, 0, 0);
	}
}
void LoopPtfxMod2()
{
	STREAMING::REQUEST_NAMED_PTFX_ASSET("proj_xmas_firework");
	if (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("proj_xmas_firework"))
	{
		GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("proj_xmas_firework");
	}
	else
	{
		float f1;
		f1 = 1.0;
		GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("proj_xmas_firework");
		GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_firework_xmas_repeat_burst_rgw", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, loopptfxType2_[loopptfxType2], 0.1f, 0, 0, 0);
	}
}
int PXType;
int ParticDelay2 = 0;
char *PXMenu[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12" };
int PXVar;
char *PXhsh[] = { "scr_clown_death", "scr_fam4_trailer_sparks", "scr_sol1_sniper_impact", "scr_trev1_trailer_boosh", "scr_trev4_747_blood_impact", "ptfx_smoke_billow_anim_rgba", "_explosion_clown_fireball_rgba", "ptfx_model_multi_objects_02", "scr_exp_clown_hot_debris"  "scr_exp_clown_base_smoke", "scr_clown_appears_spawn", "scr_firework_xmas_burst_rgw","scr_carsteal4_wheel_burnout" };
int SizeVar;
char *PXMenu2[] = { "scr_rcbarry2", "scr_family4", "scr_martin1", "scr_trevor1", "scr_solomon3", "scr_rcbarry2", "scr_rcbarry2", "scr_rcbarry2", "scr_rcbarry2", "scr_rcbarry2", "scr_rcbarry2", "proj_xmas_firework","scr_carsteal4" };
int ParticalDelay1 = 0;

#pragma endregion

#pragma region ------bool------
bool Neons_;
bool isvehJetpack;
bool VehJet;
bool carinvstext;
bool VehicleGodMode;
bool hasNoCollision;
bool talking = true;
bool showInfos;
bool selectedUI;
bool doClown;
bool spawnsetswat;
bool spawnsetchop;
bool spawnsetmtlion;
bool PlayerAttached;
bool frezzeplayer;
bool forcefield;
bool explosionloop;
bool Tunable_Snow;
bool effectp;
bool effectp2;
bool effectp3;
bool particleman;
bool isAnimationStart;
bool upperbody;
bool ChangeModelHash;
bool blackmodel;
bool isinvisible;
bool spawnGodmode;
bool ChromeSpawn;
bool MaxUpgrades;
bool isattach;
bool isManagerObject;
bool spawnplayer;
bool Loading_Vehicle;
bool BlackOut;
bool isJetpack;
bool AnimationLoad;
bool JetPackMod;
bool isOpenMenu = true;
bool isMT;
bool OpenLisence;
bool isAdmin;
bool isLisence;
bool isUpdate;
bool isOpenMT = true;
bool isOpenVersion = true;
bool megivemoneyg;
bool Godmode = false;
bool invisible = false;
bool NoCops = false;
bool Superjump = false;
bool RunSpeed = false;
bool ClearAreaPed;
bool PTFXP;
bool taskP;
bool freezecar;
bool PTFX;
bool FellisFreeze;
bool TaskP = false;
bool ClearTaskP = false;
bool ExplosionP = false;
bool KickVote = false;
bool VSpeedP = false;
bool RemoveWeaponP = false;
bool RPprotection;
bool ClearAreaAll;
bool VehicleAttachProtect;
bool Pedattach;
bool propattach;
bool fellisfreezebutton;
bool ExplosiveMelee = false;
bool NoRagdoll = false;
bool SuperRun;
bool OffRadar;
bool RevealPlayers;
bool Copsturnblindeye;
bool hasBossMode;
bool mobileRadio;
bool NoTarget;
bool doForceField_;
bool Camera_fixed = false;

bool UnlimitedAmmo;
bool UnlimitedAmmo1;
bool hashgun;
bool deletegun;
bool TeleportGunLooping;
bool RapidFire;
bool MeinCodewat;

bool isParachute;
bool Psychokinetic;
bool Psychokineticfree_forward;
bool Psychokinetic_anim;
bool isIronMan;
bool sneakmode;
bool FlashMan;
bool FlashPTFX;

bool flashflag;
bool flashflag3;
bool flashflag2;
bool isCa;
bool MoneyLoopInt = false;
bool Money40K = false;
bool SPECTATE = false;
bool sendCash1 = false;
int Timer9 = false;
int PTFXTime = false;
bool resetPlayerView1;
bool StealClothes;
bool KEYBOARD_PROMPT = false;
bool Zusatz = false;
bool MeinCode;
bool Spanlllll = false;
bool LowerWhip;
bool L3boost;
#pragma endregion

#pragma region ------Entity------
Entity NightStick;
Entity Target = 0;
Entity NightStick2;
Entity JetpackID;
#pragma endregion

#pragma region ------char------
char *Vehicle_String;
char ON[] = { 78,128,0,32 };
char OFF[] = { 124,8,2,166 };
char ONN[] = { 65,130,0,148 };
char OF[] = { 64,130,0,104 };

char *ALLESPCHAR[6] = { "Line","Name","Box","Line + Name","Line + Box","Line + Box + Name" };

char *raderjapanese[] = { "Black","Red","Yellow","Green","Lightblue","Blue","Pink","White","Normal" };
char coloara[] = { 64 };
char coloarb[] = { 20 };
char coloarc[] = { 22 };
char coloard[] = { 21 };
char coloare[] = { 25 };
char coloarf[] = { 23 };
char coloarg[] = { 24 };
char coloarh[] = { 1 };
char coloari[] = { 128 };
char radercolor1[] = { 64 ,20 ,22,19,25,23,24,1,128 };
char* Flash = { "DrivingFocusLight" };

char *dict1 = "move_m@generic";
char *anim1 = "sprint";
char *Talker;

char *Version = "Ver 1.2";
#pragma endregion

#pragma region ------Float------
float Checkbox = 0.92f;
float menuYCoords = 0.03f;
float MenuXSize = 0.24f;
float MenuYSize = 0.03f;
float infobox = 0.157f;
float Ycoords = 0.1319f;
float sizeY = 0.113f;
float SelectedUIMenuXSize = 0.18f;
float SelectedUIMenuYSize = 0.03;
float menunameuicoords = 0.88f;
float SelectedUIYcoords = 0.09f;
float SelectedUICoords = 0.215f;
float SelectedUITitleY = 0.086f;
float SelectedUITitleSizeX = 0.18f;
float SelectedUITitleSizeY = 0.04f;
float VehicleFreezeProtectionDelay;
#pragma endregion

#pragma region Explosion
void ExplodePlayer(int PedHandle)
{
	Vector3 Pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(PedHandle), true);
	FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z, 29, 0.5f, true, false, 5.0f);
}
#pragma endregion

#pragma region Clone
int ClonePed(int PedHandle)
{
	return PED::CLONE_PED(PedHandle, ENTITY::GET_ENTITY_HEADING(PedHandle), 1, 1);
}
#pragma endregion

#pragma region
void toggleAttach(int Player)
{
	if (!(cstrcmp(PLAYER::GET_PLAYER_NAME(Player), PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID()))))
	{
		if (!PlayerAttached)
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
			{
				int att = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
				int player = PLAYER::GET_PLAYER_PED(Player);
				Vector3 One, Two;
				One.x = 0;
				One.y = 0;
				One.z = 0;
				Two.x = 0;
				Two.y = 0;
				Two.z = 0;
				ENTITY::ATTACH_ENTITY_TO_ENTITY(att, player, 0, One.x, One.y, One.z, Two.x, Two.y, Two.z, 0, 1, 0, 0, 2, 1);
				ENTITY::SET_ENTITY_COLLISION(att, 0, 0);
			}
			else
			{
				int att = PLAYER::PLAYER_PED_ID();
				int player = PLAYER::GET_PLAYER_PED(Player);
				Vector3 One, Two;
				One.x = 0;
				One.y = -0.20;
				One.z = 0;
				Two.x = 0;
				Two.y = 0;
				Two.z = 0;
				ENTITY::ATTACH_ENTITY_TO_ENTITY(att, player, 0, One.x, One.y, One.z, Two.x, Two.y, Two.z, 0, 1, 0, 0, 2, 1);
				ENTITY::SET_ENTITY_COLLISION(att, 0, 0);
			}
			PlayerAttached = true;
		}
		else
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
			{
				int att = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
				int player = PLAYER::GET_PLAYER_PED(Player);
				ENTITY::DETACH_ENTITY(att, player, 1);
				ENTITY::SET_ENTITY_COLLISION(att, 1, 1);
			}
			else
			{
				int att = PLAYER::PLAYER_PED_ID();
				int player = PLAYER::GET_PLAYER_PED(Player);
				ENTITY::DETACH_ENTITY(att, player, 1);
				ENTITY::SET_ENTITY_COLLISION(att, 1, 1);
			}
			PlayerAttached = false;
		}
	}
}
#pragma endregion

#pragma region ------for network objects Functions------

bool RequestNetworkControl(uint vehID)
{
	int Tries = 0;
	bool
		hasControl = false,
		giveUp = false;
	do
	{
		hasControl = NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehID);
		Tries++;
		if (Tries > 300)
			giveUp = true;
	} while (!hasControl && !giveUp);

	if (giveUp)
		return false;
	else return true;
}

void PropAttachProtect()
{

	if (FellisTimeout2 < GAMEPLAY::GET_GAME_TIMER())
	{
		for (int i = 0; i < 132; i++)
		{

			Vector3 MyCoords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);

			Object obj = OBJECT::_GET_CLOSEST_OBJECT_OF_TYPE(MyCoords.x, MyCoords.y, MyCoords.z, 25.0f, Props[i], false, false, true);//prop_windmill_01
			RequestNetworkControl(obj);
			if (NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(obj) && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(obj))
			{
				/*ENTITY::DETACH_ENTITY(obj, 1, 1);
				ENTITY::SET_ENTITY_COORDS(obj, 6000.0, 6000.0, 6000.0, true, false, false, true);
				ENTITY::DELETE_ENTITY(&obj);*/
				if (ENTITY::IS_ENTITY_ATTACHED(obj))
				{
					ENTITY::CREATE_MODEL_HIDE(MyCoords.x, MyCoords.y, MyCoords.z, 0, Props[i], true);

					//ENTITY::DETACH_ENTITY(obj, 1, 1);
					//ENTITY::SET_ENTITY_COORDS(obj, 6000.0, 6000.0, 6000.0, true, false, false, true);
					//ENTITY::DELETE_ENTITY(&obj);

				}
			}

		}

		FellisTimeout2 = GAMEPLAY::GET_GAME_TIMER() + 1500;
	}
}
#pragma endregion

#pragma region 車スポーンタブ
int BusAttackDelay = 0;
bool TeleAutomIncar;
int AttachSpawnPlayer;
int Spawned_Vehicle;
int SuperCarVar;
int sportVar;
int VehicleHandle;
int classicVar;
int MuscleVar;
int SedanVar;
int CoupeVar;
int CompactVar;
int SuvVar;
int TrackVar;
char *SuperCar[] = { "adder", "entityxf", "cheetah", "infernus", "vacca", "bullet", "voltic", "zentorno", "turismor", "osiris", "t20" };
char *SuperCar2[] = { "Adder", "Entity XF", "Cheetah", "Infernus", "Vacca", "Bullet", "Voltic", "Zentorno", "Turismo R", "Osiris", "T20" };
char *sport[] = { "elegy2","khamelion","carbonizzare","feltzer2","feltzer3","rapidgt","rapidgt2","coquette","ninef","ninef2","surano","banshee","comet","schwarzer","fusilade","buffalo","buffalo2","penumbra","sultan","futo","furoregt","massacro","massacro2","jester","jester2","windsor","alpha","kuruma","kuruma2" };
char *sport2[] = { "Elegy RH8","Khamelion","Carbonizzare ","Feltzer 1","Feltzer 2 ","Rapid GT 1 ","Rapid GT 2","Coquette 1","9F Cabrio","Obey 9F","Surano","Banshee","Comet","Schwartzer","Fusilade","Buffalo","Buffalo 2","Penumbra","Sultan","Futo","Furore GT","Massacro 1","Massacro 2","Jester 1","Jester 2","Windsor","Alpha","Kuruma 1","Kuruma 2" };
char *classic[] = { "coquette2","coquette3","ztype","stingergt","stinger","monroe","jb700","tornado","tornado2","tornado3","tornado4","peyote","manana","virgo","btype","blade","glendale","pigalle","casco","chino" };
char *classic2[] = { "Coquette 2","Coquette 3","Z-Type","Stinger GT","Stinger","Monroe","JB 700","Tornado 1","Tornado 2","Tornado 3","Tornado 4","Peyote", "Manana","Virgo","Roosevelt","Blade","Glendale","Pigalle","Casco","Chino" };
char *Muscle2[] = { "Hotknife","Gauntlet","Vigero","Dominator","Buccaneer","Phoenix","Sabre Turbo","Ruiner","Voodoo","Picador","Rat Loader 1","Rat Loader 2" };
char *Muscle[] = { "hotknife","gauntlet","vigero","dominator","buccaneer","phoenix","sabregt","ruiner","voodoo2","picador","ratloader","ratloader2" };
char *Sedan[] = { "superd","oracle","oracle2","stretch","fugitive","surge","Schafter","asterope","intruder","washington","stanier","ingot","emperor","emperor2","emperor3","primo","regina","romero","tailgater","premier","stratum","asea","asea2" };
char *Sedan2[] = { "Super Diamond","Oracle I","Oracle II","Stretch","Fugitive","Surge","Schafter","Asterope","Intruder","Washington","Stanier","Ingot","Emperor I","Emperor II","Emperor III","Primo","Regina","Romero Hearse","Tailgater","Premier","Stratum","Asea I","Asea II" };
char *Coupes[] = { "exemplar","cogcabrio","felon2","felon","zion","zion2","sentinel","sentinel2","f620","jackal" };
char *Coupes2[] = { "Exemplar","Cabrio","Felon GT","Felon","Zion Cabri","Zion","Sentinel XS","Sentinel","F620","Jackal" };
char *Compact[] = { "dilettante","dilettante2","issi2","prairie","blista","rhapsody","warrener","panto" };
char *Compact2[] = { "Dilettante I","Dilettante II","Issi","Prairie","Blista","Rhapsody","Warrener","Panto" };

char *Suv[] = { "baller","baller2","rocoto","cavalcade","cavalcade2","dubsta","dubsta2","dubsta3","serrano","landstalker","fq2","patriot","habanero","radi","granger","mesa","mesa2","seminole","kalahari","gresley","bjxl","huntley","sadler","sadler2","guardian","insurgent2","insurgent","technical" };
char *Suv2[] = { "Baller 1","Baller 2","Rocoto","Cavalcade 1","Cavalcade 2","Dubsta 1","Dubsta 2","Dubsta 3","Serrano","Landstalker","FQ 2","Patriot","Habanero","Radius","Granger","Mesa 1","Mesa 2","Seminole","Kalahari","Gresley","BeeJay XL","Huntley","Sadler 1","Sadler 2","Guardian","Insurgent 1","Insurgent 2","Technical" };
char *Flyingcar[] = { "Baller 1","Mule 1","Felon","Stretch","Fugitive","Vigero","Stinger","Airport Bus","Barracks 1","Hexer" };
int Flyingcar2[] = { 0x402B49B0,0x402C3D70,0x402BEA90,0x402D29E0,0x402BFBA0,0x40068820,0x402CD700,0x402B3B10,0x402B44D0,0x402D66D0 };
char *Flyingcar3[] = { "baller","mule","felon","stretch","fugitive","vigero","stinger","airbus","barracks","hexer" };
int FlyingVar;
char *Tracks[] = { "monster","sandking","dune","dune2","bfinjection","bifta","blazer","blazer3","mesa3","sandking2","dloader","bodhi2","rancherxl","rancherxl2","rebel","rebel2","blazer2","brawler","enduro" };
char *Tracks2[] = { "Monster Truck","Sandking XL","Dune Buggy","Space Docker","Injection","Bifta","Blazer 1","Blazer 2 (Trevors)","Mesa (Merryweather)","Sandking SWB","Duneloader","Bodhi","Rancher XL 1","Rancher XL 2","Rebel 1","Rebel 2","Blazer Lifeguard","Brawler","Enduro" };
int VanVar;
char *Van[] = { "bison","bison2","bison3","paradise","journey","minivan","bobcatxl","rumpo","rumpo2","pony","pony2","burrito","burrito2","burrito3","burrito4","burrito5","burrito6","burrito7","speedo","speedo2","youga","boxville","boxville2","boxville3","boxville4","camper","taco","surfer","surfer2","slamvan","slamvan2" };
char *Van2[] = { "Bison 1","Bison 2","Bison 3","Paradise","Journey","Minivan","Bobcat XL","Rumpo 1","Rumpo 2","Pony 1","Pony 2","Burrito 1","Burrito 2","Burrito 3","Burrito 4","Burrito 5","Burrito 6","Burrito 7","Speedo 1","Speedo 2","Youga","Boxville 1","Boxville 2","Boxville 3","Boxville 4","Camper","Taco Van","BF Surfer","BF Surfer (Rusted)","Vapid Slamvan 1","Vapid Slamvan 2" };
int EmergencyVar;
char *Emergency[] = { "fbi","fbi2","firetruk","ambulance","police","police2","police3","police4","policeb","policeold1","policeold2","policet","riot","sheriff","sheriff2","pbus","pranger","lguard" };
char *Emergency2[] = { "FIB","FIB SUV","Firetruck","Ambulance","Police 1","Police 2","Police 3","Police 4","Police Bike","Police Old 1","Police Old 2","Police Van","Swat Truck","Sheriff 1","Sheriff 2","Prison Bus","Park Ranger","Life Guard SUV" };
char *Service2[] = { "Airport Bus","Dashhound","Bus","Rental Shuttle Bus","Tour Bus","Taxi" };
char *Service[] = { "airbus","coach","bus","rentalbus","tourbus","taxi" };
int ServiceVar;
int MilitaryVar;
char *Military[] = { "rhino","barracks","barracks2","barracks3","crusader","tanker","tanker2" };
char *Military2[] = { "Rhino Tank","Barracks 1","Barracks 2","Barracks 3 ","Canis Crusader","Tanker 1","Tanker 2" };
char *Bike2[] = { "Carbon RS", "Bati 801RR", "Bati 801", "Hexer", "Innovation", "Double-T", "Thrust", "Vindicator", "Ruffian", "Vader", "PCJ 600", "Hakuchou", "Akuma", "Sanchez (Decal)","Sanchez", "Faggio", "Daemon", "Bagger", "Nemesis", "Sovereign", "Principe Lectro" };
char *Bike[] = { "carbonrs","bati2","bati","hexer","innovation","double","thrust","vindicator","ruffian","vader","pcj","hakuchou","akuma","sanchez","sanchez2","faggio2","daemon","bagger","nemesis","sovereign","lectro" };
int BikeVar;
int BicycleVar;
int CommercialVar;
char *Commercial2[] = { "Mule 1", "Mule 2", "Mule 3", "Phantom", "Benson", "Packer", "Pounder", "Hauler", "Stockade 1", "Stockade 2", "Biff", "Dump", "Dozer", "Forklift", "Handler", "Cutter", "Truck 1", "Truck 2", "Truck 3", "Trashmaster 1","Trashmaster 2", "Towtruck 1", "Towtruck 2", "TipTruck 1", "TipTruck 2", "Mixer 1", "Mixer 2", "Flatbed Truck", "Airport Ripley","Rubble", "Vapid Scrap Truck", "Lawnmower", "Dock Tug", "Airport Tug", "Tractor 1", "Tractor 2", "Tractor 3", "Caddy 1", "Caddy 2" };
char *Commercial[] = { "mule","mule2","mule3","phantom","benson","packer","pounder","hauler","stockade","stockade3","biff","dump","bulldozer","forklift", "handler","cutter","utillitruck","utillitruck2","utillitruck3", "trash","trash2","towtruck","towtruck2","tiptruck","tiptruck2","mixer","mixer2","flatbed", "ripley","rubble","scrap","mower","docktug","airtug","tractor","tractor2","tractor3","caddy","caddy2" };
char *Bicycle2[] = { "Whippet Race Bike", "Tri-Cycles Race Bike", "Scorcher", "Endurex Race Bike", "Cruiser", "BMX", "Fixter" };
char *Bicycle[] = { "tribike","tribike3","scorcher","tribike2","cruiser","bmx","fixter" };
char *Helicopter[] = { "annihilator","buzzard","buzzard2","frogger","frogger2","maverick", "cargobob","cargobob2", "cargobob3", "polmav", "swift","swift2","valkyrie","savage" };
char *Helicopter2[] = { "Annihilator", "Buzzard 1", "Buzzard 2", "Frogger 1", "Frogger 2", "Maverick", "Cargobob 1","Cargobob 2", "Cargobob 3","Maverick","Swift 1","Swift 2", "Valkyrie", "Savage" };
int HelicopterVar;
char *Plane[] = { "titan","luxor", "luxor2", "shamal", "vestra", "miljet", "velum", "velum2", "mammatus", "duster", "stunt", "cuban800", "cargoplane", "blimp", "lazer", "skylift", "jet", "besra", "hydra" };
char *Plane2[] = { "Titan", "Luxor 1", "Luxor 2","Shamal","Vestra","Miljet","Velum 1","Velum 2","Mammatus","Duster","Mallard","Cuban 800","Cargo Plane","Blimp","P-996 Lazer","Skylift","Jet","Bersa","Hydra" };
int PlaneVar;
int BoatVar;
char *Trailers[] = { "armytanker","armytrailer","armytrailer2", "baletrailer",  "boattrailer", "cablecar", "docktrailer", "freight","freightcar",  "freightcont1", "freightcont2", "freightgrain", "graintrailer", "metrotrain", "proptrailer","raketrailer", "tankercar","tr2","tr3","tr4","trailerlogs","trailers", "trailers2","trailers3",  "trailersmall", "trflat", "tvtrailer" };
int TrailersVar;
char *Trailers2[] = { "Tanker", "Tanker 1", "Tanker 2", "Trailer", "Trailer2","Cablecar", "Dock Trailer", "Freight Car 1", "Freight Car 2", "Freight Container 1","Freight Container 2", "Freight Train Boxcar","Grain Trailer",  "Metro Train", "Mobile Home Trailer","Rake Trailer", "Train Fuel Tank Car","Car Carrier Trailer",  "Marquis Trailer", "Super Car Carrier Trailer", "Trailer Logs", "Trailers 1", "Trailers 2","Trailers 3", "Trailer Small","Trailer Flat", "Tv Trailer" };
char *Boat[] = { "marquis","jetmax","squalo","suntrap","tropic","seashark","seashark2","predator","submersible", "speeder","dinghy", "dinghy2","dinghy3","toro" };
char *Boat2[] = { "Dinka Marqui", "Shitzu Jetmax", "Shitzu Squalo, ", "Shitzu Suntrap", "Shitzu Tropic", "Speedophile Seashark 1", "Speedophile Seashark 2", "Police Predator", "Submarine", "Pegassi Speeder","Nagasaki Dinghy 1", "Nagasaki Dinghy 2","Nagasaki Dinghy 3", "Lampadati Toro" };

#pragma endregion

#pragma region Message
void TextMessages(int player, char *text,int font = 0)
{
	if (player != PLAYER::PLAYER_ID())
	{
		int NetHandleBuffer = 0x10070200;
		NETWORK::NETWORK_HANDLE_FROM_PLAYER2(player, NetHandleBuffer, 13);
		NETWORK::NETWORK_SEND_TEXT_MESSAGE2(text, NetHandleBuffer);
	}
}
#pragma endregion

#pragma region ------Vehicle Spawner------
bool Create_Vehicle(char* FahrzeugName, bool isBusAttack = false)
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(GAMEPLAY::GET_HASH_KEY(FahrzeugName)))
	{
		if (STREAMING::IS_MODEL_VALID(GAMEPLAY::GET_HASH_KEY(FahrzeugName)))
		{
			STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY(FahrzeugName));
			Vector3 coords;

			if (STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY(FahrzeugName)))
			{
				if (!isBusAttack)
				{
					if (spawnplayer)
					{
						coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(AttachSpawnPlayer), 1);
					}
					else
					{
						coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
					}
					if (!isManagerObject)
					{
						Spawned_Vehicle = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY(FahrzeugName), coords.x, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 0);
					}
					if (!isManagerObject)
					{
						if (ENTITY::DOES_ENTITY_EXIST(Spawned_Vehicle))
						{

							if (isattach)
							{
								TextMessages(AttachSpawnPlayer, "私があなたに車をアタッチしました。");
								
								ENTITY::ATTACH_ENTITY_TO_ENTITY(Spawned_Vehicle, PLAYER::GET_PLAYER_PED(AttachSpawnPlayer), 0x0, 0, 0, 0, 0, 0, 0, false, false, false, false, 2, true);
								//ENTITY::ATTACH_ENTITY_TO_ENTITY(Spawned_Vehiclet[Selected_Vehicle], PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), BIndex2, LocX, LocY, LocZ, RotX, RotY, RotZ, false, false, false, false, 2, true);			
							}
							VEHICLE::SET_VEHICLE_MOD_KIT(Spawned_Vehicle, 0);
							VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(Spawned_Vehicle, 0, 0, 0);
							VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(Spawned_Vehicle, 255, 255, 255);
							if (TeleAutomIncar)
							{
								bool AAAn;
								float Speed;
								Vector3 Rotation;
								int PedHandle = PLAYER::PLAYER_PED_ID();
								Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
								if (PED::IS_PED_IN_ANY_VEHICLE(PedHandle, 0))
								{
									AAAn = true;
									Speed = ENTITY::GET_ENTITY_SPEED(veh);
									VEHICLE::DELETE_VEHICLE(&veh);

								}
								PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), Spawned_Vehicle, -1);
								VEHICLE::SET_VEHICLE_ENGINE_ON(Spawned_Vehicle, true, true);
								if (AAAn)
								{
									VEHICLE::SET_VEHICLE_FORWARD_SPEED(Spawned_Vehicle, Speed);
									AAAn = false;
								}
							}
							if (MaxUpgrades)
							{
								VEHICLE::SET_VEHICLE_FIXED(Spawned_Vehicle);
								VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(Spawned_Vehicle);
								VEHICLE::SET_VEHICLE_DIRT_LEVEL(Spawned_Vehicle, 0);
								VEHICLE::SET_VEHICLE_MOD_KIT(Spawned_Vehicle, 0);
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 0, 1, 0);//Soiler
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 1, 1, 0);//Frontstoﾟstange
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 2, 1, 0);//Heckstoﾟstange
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 3, 1, 0);//Body
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 4, 2, 0);//Auspuff
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 5, 1, 0);//Ueberrollkaeffig
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 6, 1, 0);//Grill
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 7, 1, 0);//Motorhaube
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 8, 2, 0);//Schhuerze
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 9, 2, 0);//Rechte Schuerze
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 10, 1, 0);//Dach
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 11, 3, 0);//Motor9
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 12, 2, 0);//Bremsen
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 13, 2, 0);//Getriebe
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 14, 24, 0);//Hupe
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 15, 2, 0);//Federung
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 16, 4, 0);//Panzerung
								VEHICLE::TOGGLE_VEHICLE_MOD(Spawned_Vehicle, 18, 1);//Turbo
								VEHICLE::TOGGLE_VEHICLE_MOD(Spawned_Vehicle, 22, 1);//Xenon 
								VEHICLE::SET_VEHICLE_WHEEL_TYPE(Spawned_Vehicle, 7);//Reifen Typ
								VEHICLE::SET_VEHICLE_MOD(Spawned_Vehicle, 23, 18, 0);//Reifen
								VEHICLE::SET_VEHICLE_WINDOW_TINT(Spawned_Vehicle, 5);
								VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(Spawned_Vehicle, "STANG");
								VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(Spawned_Vehicle, 3);
								VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(Spawned_Vehicle, false);
								char buf[100];
								snprintf(buf, sizeof(buf), "Upgraded: ~b~%s~HUD_COLOUR_WHITE~!", FahrzeugName);
								PRINT(buf);
							}
							if (ChromeSpawn)
								VEHICLE::SET_VEHICLE_COLOURS(Spawned_Vehicle, 120, 120);
							if (spawnGodmode)
								ENTITY::SET_ENTITY_INVINCIBLE(Spawned_Vehicle, true);
							if (isinvisible)
							{
								ENTITY::SET_ENTITY_VISIBLE(Spawned_Vehicle, false);
							}
							char buf[100];
							snprintf(buf, sizeof(buf), "Spawned: ~b~%s~HUD_COLOUR_WHITE~!", FahrzeugName);
							PRINT(buf, 2000);
							return true;
						}
					}


				}
				else
				{
					coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);
					Spawned_Vehicle = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY(FahrzeugName), coords.x, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(PLAYER::GET_PLAYER_PED(selectedPlayer)), 1, 0);
					if (ENTITY::DOES_ENTITY_EXIST(Spawned_Vehicle))
					{
						RequestNetworkControl(Spawned_Vehicle);
						switch (BusAttackDelay)
						{
						case 0:ENTITY::SET_ENTITY_ROTATION(Spawned_Vehicle, 90, 0, 0, false, false); APPLY_FORCE(Spawned_Vehicle, 0, 60, 0, 0, 0, 0); break;
						case 1:ENTITY::SET_ENTITY_ROTATION(Spawned_Vehicle, 270, 0, 0, false, false); APPLY_FORCE(Spawned_Vehicle, 0, 60, 0, 0, 0, 0); break;
						case 2:ENTITY::SET_ENTITY_ROTATION(Spawned_Vehicle, 0, 90, 0, false, false); APPLY_FORCE(Spawned_Vehicle, 0, 60, 0, 0, 0, 0); break;
						case 3:ENTITY::SET_ENTITY_ROTATION(Spawned_Vehicle, 0, 270, 0, false, false); APPLY_FORCE(Spawned_Vehicle, 0, 60, 0, 0, 0, 0); break;
						}
						return true;
					}
				}
			}
			else return false;
		}
	}

}
#pragma endregion

#pragma region Force

bool spawnClown()
{
	if (doClown)
	{
		STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("s_m_y_clown_01"));
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(selectedPlayer), 1);
		if (STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("s_m_y_clown_01")))
		{
			int Ped = (PED::CREATE_PED(21, 0x3F039CBA, coords.x, coords.y, coords.z, 0, 1, 0));
			if (ENTITY::DOES_ENTITY_EXIST(Ped))
			{
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(GAMEPLAY::GET_HASH_KEY("s_m_y_clown_01"));
				ENTITY::ATTACH_ENTITY_TO_ENTITY(Ped, selectedPlayer, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				return true;
			}
			return false;
		}
		return false;
	}
}
void kickPlayerNonHost(int player) {
	PLAYER::GET_PLAYER_PED(player);

	unsigned int playerBit = (1 << player);

	int args[3] = { 0x2,player,0x35 };

	callScriptEvent(args, 3, playerBit);

}
void KickFromCar(int VehicleHandle)
{
	int player = PLAYER::GET_PLAYER_PED(selectedPlayer);
	if (player > 0 && player != PLAYER::PLAYER_PED_ID());
	AI::CLEAR_PED_TASKS_IMMEDIATELY(player);
}
void setPlayerMentalStat(int player, float amt)
{
	setPlayerStatFloat(player, 133, amt, true);
}
void SpectateMode(bool Active, Ped selectedPed)
{
	NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(Active, selectedPed);
}
void Spectateoff()
{
	AI::CLEAR_PED_TASKS(PLAYER::PLAYER_ID());
	SpectateMode(false, PLAYER::GET_PLAYER_PED(selectedPlayer));
}

void PlaySound(int player, int type)
{
	int args[3];
	args[0] = 30 + type;
	args[1] = player;
	unsigned int bits = (1 << player);
	callScriptEvent(args, 3, bits);
}
void TakeAllWeapons(int client)
{
	int pedid = PLAYER::GET_PLAYER_PED(client);
	REMOVE_ALL_PED_WEAPONS(pedid, 1);
}
void TakeAllWeaponsV2(int Client)
{
	int pedid = PLAYER::GET_PLAYER_PED(Client);
	REMOVE_ALL_PED_WEAPONS(pedid, 1);
	REMOVE_WEAPON_FROM_PED(pedid, 0xa2719263);//0xAB564B93
}
void teleportPlayerIntoGolfEvent(int player) {
	PLAYER::GET_PLAYER_PED(player);
	unsigned int bits = (1 << player);
	int args[3] = { 0x3 ,player ,0x37 };
	callScriptEvent(args, 3, bits);
}

void teleportPlayerIntoArmWrestlingEvent(int player) {
	PLAYER::GET_PLAYER_PED(player);
	unsigned int bits = (1 << player);
	int args[3] = { 0x3 ,player ,0x35 };

	callScriptEvent(args, 3, bits);
}
void teleportPlayerIntoDartsEvent(int player) {
	PLAYER::GET_PLAYER_PED(player);
	unsigned int bits = (1 << player);
	int args[3] = { 0x3,player , 0x36 };

	callScriptEvent(args, 3, bits);
}
#pragma endregion

#pragma region ------Vector------
void givePlayerRP(int player, int amount, int unk1 = -1, int unk2 = 1)
{
	PLAYER::GET_PLAYER_PED(player);
	unsigned int bits = (1 << player);
	int args[5];
	args[0] = 0xD8;//216
	args[1] = player;
	args[2] = 0xffffffff - amount + 1;//46
	args[3] = 0;//-1   0 is partner, else is gen
	args[4] = 0;//unk2;//1

	SCRIPT::TRIGGER_SCRIPT_EVENT(1, args, 5, bits);
}
#pragma endregion

#pragma region ------Buttons------
bool isJustPressed(int Button)
{
	return CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(0, Button);
}
bool isPressed(int Button)
{
	return CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, Button);
}
bool isJustPress(int Button)
{
	return CONTROLS::IS_CONTROL_JUST_PRESSED(0, Button);
}
bool isPress(int Button)
{
	return CONTROLS::IS_CONTROL_PRESSED(0, Button);
}
#pragma endregion

#pragma region ------Protection2------

void VehicleFreezeProtection(bool CheckAttached = true)
{
	Vector3 Myc = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	if (VehicleFreezeProtectionDelay < GAMEPLAY::GET_GAME_TIMER())
	{

		const int numElements = 10;
		const int arrSize = numElements + 1;  //Start at index 2, and the odd elements are padding
		int vehs[arrSize];
		vehs[0] = numElements;
		int count = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), vehs);
		for (int i = 0; i < count; ++i)
		{
			int offsettedID = i + 1;
			if (ENTITY::DOES_ENTITY_EXIST(vehs[offsettedID]))
			{

				bool Contin = true;
				if (CheckAttached)
				{
					if (!(ENTITY::IS_ENTITY_ATTACHED(vehs[offsettedID]) && ENTITY::GET_ENTITY_ATTACHED_TO(vehs[offsettedID]) == PLAYER::PLAYER_PED_ID()))
						Contin = false;
				}
				if (Contin)
				{
					if (NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehs[offsettedID]) && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(vehs[offsettedID]))
					{
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehs[offsettedID]);
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehs[offsettedID], 0, 1);
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehs[offsettedID]);
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehs[offsettedID]);
						NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(vehs[offsettedID]);
						ENTITY::DELETE_ENTITY(&vehs[offsettedID]);
						PRINT("~b~ Vehicle removed.", 1000);
					}
				}
			}
		}
		VehicleFreezeProtectionDelay = GAMEPLAY::GET_GAME_TIMER() + 500;
	}
}

void FellisProtect()
{

	if (FellisTimeout < GAMEPLAY::GET_GAME_TIMER())
	{
		Entity obj1;
		Vector3 Wheel = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);

		obj1 = OBJECT::_GET_CLOSEST_OBJECT_OF_TYPE(Wheel.x, Wheel.y, Wheel.z, 25.0f, GAMEPLAY::GET_HASH_KEY("prop_Ld_ferris_wheel"), false, false, true);//prop_windmill_01
		RequestNetworkControl(obj1);
		if (NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(obj1) && NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(obj1))
		{
			/*ENTITY::DETACH_ENTITY(obj1, 1, 1);
			ENTITY::SET_ENTITY_VISIBLE(false, true);
			ENTITY::SET_ENTITY_COORDS(obj1, 6000.0, 6000.0, 6000.0, true, false, false, true);
			ENTITY::DELETE_ENTITY(&obj1);*/
			if (ENTITY::IS_ENTITY_ATTACHED(obj1))
			{
				//ENTITY::REMOVE_MODEL_HIDE(Wheel.x, Wheel.y, Wheel.z, 0, GAMEPLAY::GET_HASH_KEY("prop_Ld_ferris_wheel"), true);
				ENTITY::CREATE_MODEL_HIDE(Wheel.x, Wheel.y, Wheel.z, 0, GAMEPLAY::GET_HASH_KEY("prop_Ld_ferris_wheel"), true);
				//ENTITY::DETACH_ENTITY(obj1, 1, 1);
				//ENTITY::SET_ENTITY_COORDS(obj1, 6000.0, 6000.0, 6000.0, true, false, false, true);
				//ENTITY::DELETE_ENTITY(&obj1);
			}
		}
		FellisTimeout = GAMEPLAY::GET_GAME_TIMER() + 4500;
	}
	/*else
	{
	Vector3 Wheel = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	ENTITY::REMOVE_MODEL_HIDE(Wheel.x, Wheel.y, Wheel.z, 250, GAMEPLAY::GET_HASH_KEY("prop_Ld_ferris_wheel"), true);
	}*/
}
#pragma endregion

#pragma region ------ボタン------
int Analog(int Value)
{
	// Left Analog X-Axis (188 = INPUT_SCRIPT_LEFT_AXIS_X)
	// 0 = Fully left 
	// 127 = Centered 
	// 254 = Fully right

	// Left Analog Y-Axis (189 = INPUT_SCRIPT_LEFT_AXIS_Y)
	// 0 = Fully up 
	// 127 = Centered 
	// 254 = Fully down


	// Right Analog X-Axis (190 = INPUT_SCRIPT_RIGHT_AXIS_X)
	// 0 = Fully left 
	// 127 = Centered 
	// 254 = Fully right


	// Right Analog Y-Axis (191 = INPUT_SCRIPT_RIGHT_AXIS_Y)
	// 0 = Fully up 
	// 127 = Centered 
	// 254 = Fully down


	return CONTROLS::GET_CONTROL_VALUE(0, Value);
}
#pragma endregion

#pragma region ------カメラから座標等を取得------
Vector3 get_rotation_from_cam(float distance = 2)
{
	Vector3 Rotation = CAM::GET_GAMEPLAY_CAM_ROT(distance);
	return Rotation;
}
Vector3 GetCoordsInfrontOfCam(float distance)
{
	Vector3 Rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	Vector3 Coord = CAM::GET_GAMEPLAY_CAM_COORD();

	Rot.y = distance * SYSTEM::COS(Rot.x);
	Coord.x = Coord.x + Rot.y * SYSTEM::SIN(Rot.z * -1.0f);
	Coord.y = Coord.y + Rot.y * SYSTEM::COS(Rot.z * -1.0f);
	Coord.z = Coord.z + distance * SYSTEM::SIN(Rot.x);

	return Coord;
}
Vector3 get_coords_from_cam(float distance)
{
	Vector3 Rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	Vector3 Coord = CAM::GET_GAMEPLAY_CAM_COORD();

	Rot.y = distance * SYSTEM::COS(Rot.x);
	Coord.x = Coord.x + Rot.y * SYSTEM::SIN(Rot.z * -1.0f);
	Coord.y = Coord.y + Rot.y * SYSTEM::COS(Rot.z * -1.0f);
	Coord.z = Coord.z + distance * SYSTEM::SIN(Rot.x);

	return Coord;
}
#pragma endregion

#pragma region ------for clients------
void GiveAllWeaponsClients(int Client)
{
	int pedid = PLAYER::GET_PLAYER_PED(Client);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xa2719263, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x99b507ea, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x678b81b1, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x4e875f73, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x958a4a8f, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x440e4788, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x84bd7bfd, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x1b06d571, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x5ef9fec4, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x22d8fe39, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x99aeeb3b, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x13532244, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x2be6766b, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xefe7e2df, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xbfefff6d, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x83bf0278, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xaf113f99, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x9d07f764, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x7fd62962, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x1d073a89, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x7846a318, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xe284c527, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x9d61e50f, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x3656c8c1, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x5fc3c11, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xc472fe2, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xa284510b, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x4dd2dc56, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xb1ca77b1, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x687652ce, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x42bf8a85, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x93e220bd, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x2c3731d9, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xfdbc8a50, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xa0973d5e, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x24b17070, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x60ec506, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x34a67b97, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x23c9f95c, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x497facc3, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xf9e6aa4b, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x61012683, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xc0a3098d, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xd205520e, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xbfd21232, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x7f229f94, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x92a27487, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x83839c4, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xa89cb99e, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x7f7497e5, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x3aabbbaa, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xc734385a, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x63ab0442, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0xaf2208a7, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x787f0bb, 9999, 1);
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(pedid, 0x47757124, 9999, 1);
	PRINT("~y~Gave All Weapons");
}
#pragma endregion

#pragma region ------Fun menu------
bool TASK_PLAY_ANIM(Ped Ped, char* AnimDict, char* Anim, float Speed = 8)
{
	STREAMING::REQUEST_ANIM_DICT(AnimDict);
	if (STREAMING::HAS_ANIM_DICT_LOADED(AnimDict))
	{
		AI::TASK_PLAY_ANIM(Ped, AnimDict, Anim, Speed, Speed, -1, 9, Speed, 0, 0, 0);
		return true;
	}
	else
		return false;
}
bool TASK_PLAY_ANIM_1(Ped Ped, char* AnimDict, char* Anim, int index = 8)
{
	AI::TASK_PLAY_ANIM(Ped, AnimDict, Anim, 1.0f, 0.0f, -1, index, 8, 0, 0, 0);
}
void IronManMOD()
{
	if (Psychokinetic_anim)
	{
		Psychokinetic = true;
	}
	else
	{
		GiveAllWeaponsClients(PLAYER::PLAYER_ID());
		ENTITY::SET_ENTITY_HAS_GRAVITY(PLAYER::PLAYER_PED_ID3(), true);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());
		isIronMan = false;
		Psychokinetic = false;
		Psychokineticfree_forward = false;
	}
}
void ShootTest()
{
	if (isCa)
	{
		PRINT("エイムや銃を撃つ等をすればしゃがみます。", 1000);
		PED::SET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID3(), "move_ped_crouched", 1.0f);
		PED::SET_PED_STRAFE_CLIPSET(PLAYER::PLAYER_PED_ID3(), "move_ped_crouched_strafing");
	}
	else
	{
		PED::RESET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID3(), 1);
		PED::RESET_PED_STRAFE_CLIPSET(PLAYER::PLAYER_PED_ID3());
	}
}
void FlashMod()
{
	if (FlashMan)
	{
		Vector3 InCamera = get_rotation_from_cam(4);
		ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 0, 0, InCamera.z, true, true);
		if (isPressed(Button_A))
		{

			if (!flashflag)
			{
				FlashPTFX = true;
				int pPlayer = PLAYER::PLAYER_PED_ID3();
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(pPlayer);
				GRAPHICS::SET_TIMECYCLE_MODIFIER(Flash);
				flashflag2 = true;
				AI::TASK_PLAY_ANIM(pPlayer, dict1, anim1, 1.0f, 0, -1, 9, 8, 0, true, 0);
				//ENTITY::PLAY_ENTITY_ANIM(PLAYER::PLAYER_PED_ID(), dict1, anim1, 1000, true, true, true, 0, 0);
				//PLAYER::SET_PLAYER_SPRINT(PLAYER::PLAYER_ID(), true);
				//ENTITY::SET_ENTITY_ANIM_SPEED(pPlayer, dict1, anim1, 100.0f);
				//AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID3(), dict1, anim1, false);
				flashflag = true;

			}
			else
			{
				if (isPressed(Button_X))
				{
					NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PLAYER::PLAYER_PED_ID3());
					APPLY_FORCE(PLAYER::PLAYER_PED_ID3(), 0, 3, 1, 0, 0, 0);
					AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID3(), dict1, anim1, false);
					AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "skydive@base", "ragdoll_to_free_idle", 1.0f, 0, -1, 9, 8, 0, true, 0);
				}
			}
			AI::SET_ANIM_RATE(PLAYER::PLAYER_PED_ID3(), 10, 0, 0);

		}
		else
		{
			if (flashflag2)
			{
				GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());
				flashflag2 = false;
				flashflag = false;
				FlashPTFX = false;
			}
		}
	}
	else
	{
		if (flashflag2)
		{
			GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
			AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());


			//flashflag = false;
		}
		flashflag2 = false;
		FlashPTFX = false;
		flashflag = false;
		GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
		//STREAMING::REMOVE_ANIM_DICT(Animaton1);
		AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID3(), dict1, anim1, false);
		//AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());
		//AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());
	}
}

bool isSneak;

int Timeout;
void SneakMode(bool toggle)
{
	if (!toggle)
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());
	}
}
void PutOnJetpack()
{
	if (JetPackMod)
	{
		SYSTEM::WAIT(10);
		//AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID3(), "misstrevor3", "horny_biker_loop", 1.0f, 0.0f, -1, 9, 8, 0, 0, 0);
		Vector3 MyCoordsRightNow = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID3(), true);
		JetpackID = OBJECT::CREATE_OBJECT((0x5D9377D3), MyCoordsRightNow.x, MyCoordsRightNow.y, MyCoordsRightNow.z, true, true, false);//p_s_scuba_tank_s
		ENTITY::ATTACH_ENTITY_TO_ENTITY(JetpackID, PLAYER::PLAYER_PED_ID(), PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID(), 24817), -0.13, -0.21, 0, -180, 90, 0, 1, 0, 0, 0, 2, 1);

		NightStick = OBJECT::CREATE_OBJECT((GAMEPLAY::GET_HASH_KEY("w_me_nightstick")), MyCoordsRightNow.x, MyCoordsRightNow.y, MyCoordsRightNow.z, true, true, false);
		NightStick2 = OBJECT::CREATE_OBJECT((GAMEPLAY::GET_HASH_KEY("w_me_nightstick")), MyCoordsRightNow.x, MyCoordsRightNow.y, MyCoordsRightNow.z, true, true, false);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(NightStick, PLAYER::PLAYER_PED_ID(), PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID(), 24817), 0, 0.3, -0.2, 70, 0, 0, 1, 0, 0, 0, 2, 1);

		ENTITY::ATTACH_ENTITY_TO_ENTITY(NightStick2, PLAYER::PLAYER_PED_ID(), PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID(), 24817), 0, 0.3, 0.2, 110, 0, 0, 1, 0, 0, 0, 2, 1);

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED((0x5D9377D3));//p_s_scuba_tank_s
	}
	else
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID3());
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(JetpackID);
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(NightStick);
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(NightStick2);
		ENTITY::DELETE_ENTITY(&JetpackID);
		ENTITY::DELETE_ENTITY(&NightStick);
		ENTITY::DELETE_ENTITY(&NightStick2);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED((0x5D9377D3));//p_s_scuba_tank_s
	}
}
void NameESP(int Index)
{
	Player playerHandle = PLAYER::GET_PLAYER_PED(Index);
	Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
	float ScreenX;
	float ScreenY;
	UI::_GET_SCREEN_COORD_FROM_WORLD_COORD(handleCoords.x, handleCoords.y, handleCoords.z, &ScreenX, &ScreenY);
	drawText(PLAYER::GET_PLAYER_NAME(Index), 0, ScreenX, ScreenY - 0.1f, 0.4, 0.4, 200, 0, 0, 200, true);
}
void BoxESP(int Index)
{
	Player playerHandle = PLAYER::GET_PLAYER_PED(Index);
	Vector3 handleCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerHandle, 0, 0, 0);
	float ScreenX;
	float ScreenY;
	UI::_GET_SCREEN_COORD_FROM_WORLD_COORD(handleCoords.x, handleCoords.y, handleCoords.z, &ScreenX, &ScreenY);
	if (ENTITY::IS_ENTITY_ON_SCREEN(PLAYER::GET_PLAYER_PED(Index)))
	{
		DrawSprite("visualflow", "crosshair", ScreenX, ScreenY, 0.03f, 0.04f, 0, 255, 0, 0, 255);
	}
	else
	{
		DrawSprite("visualflow", "crosshair", ScreenX, ScreenY, 0.03f, 0.04f, 0, 255, 255, 255, 255);
	}
}
void Linie(int Client)
{
	int PedID = PLAYER::GET_PLAYER_PED(Client);
	Vector3 LocalPed = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
	Vector3 TargetPed = ENTITY::GET_ENTITY_COORDS(PedID, 1);
	float start[] = { LocalPed.x, LocalPed.y, LocalPed.z };
	float end[] = { TargetPed.x, TargetPed.y, TargetPed.z };
	GRAPHICS::DRAW_LINE(start[0], start[1], start[2], end[0], end[1], end[2], 0, 255, 255, 255);

	//GRAPHICS::DRAW_LINE(end[0], end[1], end[2], end[0], end[1], 2000, 0, 255, 255, 255); プレイヤーの頭上に縦棒(使うなら斜線消して)
}


int JetpackSpeed = 1;

void BlackOut2()
{
	if (BlackOut)
	{
		GRAPHICS::_SET_BLACKOUT(true);
	}
	else
	{
		GRAPHICS::_SET_BLACKOUT(false);
	}
}

bool ALLESP;
int ALLESPTYPES;
int RaderColorVar;
void aaa()
{
	switch (RaderColorVar)
	{
	case 0:WriteBytes(0x01D6B357, coloara, 1); break;
	case 1:
		WriteBytes(0x01D6B357, coloarb, 1);
		//UI::_0xF6E7E92B(HUD_COLOUR_PURE_WHITE, 0, 0, 0, 40);
		//UI::_0xF6E7E92B(HUD_COLOUR_PAUSE_BG, 128, 0, 0, 40);
		//UI::_0xF6E7E92B(HUD_COLOUR_PAUSE_SINGLEPLAYER, 128, 128, 0, 40);
		break;
	case 2:WriteBytes(0x01D6B357, coloarc, 1); break;
	case 3:WriteBytes(0x01D6B357, coloard, 1); break;
	case 4:WriteBytes(0x01D6B357, coloare, 1); break;
	case 5:WriteBytes(0x01D6B357, coloarf, 1); break;
	case 6:WriteBytes(0x01D6B357, coloarg, 1); break;
	case 7:WriteBytes(0x01D6B357, coloarh, 1); break;
	case 8:WriteBytes(0x01D6B357, coloari, 1); break;
	}
}
#pragma endregion

#pragma region ------メソッド------

void DrawSprite1(char * Streamedtexture, char * textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a)
{
	GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(Streamedtexture, false);
	if (GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(Streamedtexture));
	GRAPHICS::DRAW_SPRITE(Streamedtexture, textureName, x, y, width, height, rotation, r, b, g, a);
}

int Read_Global(int a_uiGlobalID)
{
	int Ptr = *(int*)((0x1E70374 - 0x04) + (((a_uiGlobalID / 0x40000) & 0x3F) * 4));
	if (Ptr != 0)
		return *(int*)(Ptr + ((a_uiGlobalID % 0x40000) * 4));
	return 0;
}
bool Write_Global(int a_uiGlobalID, int a_uiValue)
{
	int Ptr = *(int*)((0x1E70374 - 0x04) + (((a_uiGlobalID / 0x40000) & 0x3F) * 4));
	if (Ptr != 0)
	{
		*(int*)(Ptr + ((a_uiGlobalID % 0x40000) * 4)) = a_uiValue;
		return true;
	}
	return false;
}


bool LDIPL;
void LoadIPL()
{
	if (IsTimerReady())
	{
		if (LDIPL)
		{
			STREAMING::REQUEST_IPL("airfield");
			STREAMING::REQUEST_IPL("AP1_04_TriAf01");
			STREAMING::REQUEST_IPL("bh1_16_refurb");
			STREAMING::REQUEST_IPL("BH1_47_JoshHse_Burnt");
			STREAMING::REMOVE_IPL("BH1_47_JoshHse_UnBurnt");

			STREAMING::REQUEST_IPL("bnkheist_apt_dest");
			STREAMING::REMOVE_IPL("bnkheist_apt_norm");
			STREAMING::REQUEST_IPL("burgershot_yoga");
			STREAMING::REQUEST_IPL("burnt_switch_off");
			STREAMING::REQUEST_IPL("cargoship");

			STREAMING::REMOVE_IPL("canyonriver01");//train
			STREAMING::REMOVE_IPL("railing_start");
			STREAMING::REQUEST_IPL("canyonriver01_traincrash");
			STREAMING::REQUEST_IPL("railing_end");

			STREAMING::REQUEST_IPL("chemgrill_grp1");
			STREAMING::REQUEST_IPL("CH1_07_TriAf03");
			STREAMING::REQUEST_IPL("CH3_RD2_BishopsChickenGraffiti");

			//STREAMING::REQUEST_IPL("");
			STREAMING::REQUEST_IPL("chop_props");
			STREAMING::REQUEST_IPL("chophillskennel");
			STREAMING::REQUEST_IPL("CJ_IOABoat");
			STREAMING::REQUEST_IPL("coronertrash");
			STREAMING::REQUEST_IPL("Coroner_Int_on");
			STREAMING::REMOVE_IPL("Coroner_Int_off");
			STREAMING::REQUEST_IPL("crashed_cargoplane");
			STREAMING::REQUEST_IPL("CS1_02_cf_offmission");
			STREAMING::REQUEST_IPL("CS1_02_cf_onmission1");
			STREAMING::REQUEST_IPL("CS1_02_cf_onmission2");
			STREAMING::REQUEST_IPL("CS1_02_cf_onmission3");
			STREAMING::REQUEST_IPL("CS1_02_cf_onmission4 ");

			STREAMING::REQUEST_IPL("CS2_06_TriAf02");
			STREAMING::REQUEST_IPL("CS3_05_water_grp1");
			STREAMING::REQUEST_IPL("CS3_05_water_grp2 ");
			STREAMING::REQUEST_IPL("cs3_07_mpgates ");
			STREAMING::REQUEST_IPL("CS4_08_TriAf02");
			STREAMING::REQUEST_IPL("CS4_04_TriAf03");
			STREAMING::REQUEST_IPL("CS5_04_MazeBillboardGraffiti");
			STREAMING::REQUEST_IPL("cs5_4_trains");
			STREAMING::REQUEST_IPL("CS5_Roads_RonOilGraffiti");

			STREAMING::REQUEST_IPL("des_farmhouse");
			STREAMING::REQUEST_IPL("des_farmhs_endimap");
			STREAMING::REQUEST_IPL("des_farmhs_end_occl");

			STREAMING::REMOVE_IPL("des_farmhs_startimap");
			STREAMING::REMOVE_IPL("des_farmhs_start_occl");

			STREAMING::REQUEST_IPL("DES_ProTree_start");
			STREAMING::REQUEST_IPL("DES_ProTree_start_lod");

			STREAMING::REQUEST_IPL("DES_Smash2_startimap ");
			STREAMING::REQUEST_IPL("DES_StiltHouse_imapstart");
			STREAMING::REMOVE_IPL("DES_Smash2_endimap ");
			STREAMING::REMOVE_IPL("DES_StiltHouse_imapend");
			STREAMING::REMOVE_IPL("des_stilthouse_rebuild ");

			STREAMING::REQUEST_IPL("DT1_05_HC_REQ");
			STREAMING::REMOVE_IPL("DT1_05_HC_REMOVE");
			STREAMING::REQUEST_IPL("DT1_05_REQUEST");
			STREAMING::REQUEST_IPL("DT1_05_rubble");
			STREAMING::REMOVE_IPL("DT1_17_NewBill");
			STREAMING::REQUEST_IPL("DT1_17_OldBill");
			STREAMING::REQUEST_IPL("DT1_21_prop_lift_on");
			STREAMING::REQUEST_IPL("fakeint");

			STREAMING::REMOVE_IPL("farm");
			STREAMING::REQUEST_IPL("farm_burnt");
			STREAMING::REQUEST_IPL("farm_burnt_props ");

			STREAMING::REQUEST_IPL("farmint_cap");
			STREAMING::REQUEST_IPL("farmint");
			STREAMING::REQUEST_IPL("farm_props");

			STREAMING::REQUEST_IPL("FBI_colPLUG");
			STREAMING::REQUEST_IPL("FIBlobby");
			STREAMING::REMOVE_IPL("FIBlobbyfake");
			STREAMING::REMOVE_IPL("FBI_repair");
			STREAMING::REQUEST_IPL("ferris_finale_Anim");
			STREAMING::REQUEST_IPL("FruitBB");
			STREAMING::REQUEST_IPL("gasparticle_grp2");
			STREAMING::REQUEST_IPL("gasstation_ipl_group1");
			STREAMING::REQUEST_IPL("gasstation_ipl_group2");

			STREAMING::REQUEST_IPL("hei_carrier");
			STREAMING::REQUEST_IPL("hei_carrier_DistantLights");
			STREAMING::REQUEST_IPL("hei_Carrier_int1");
			STREAMING::REQUEST_IPL("hei_Carrier_int2");
			STREAMING::REQUEST_IPL("hei_Carrier_int3");
			STREAMING::REQUEST_IPL("hei_Carrier_int4");
			STREAMING::REQUEST_IPL("hei_Carrier_int5");
			STREAMING::REQUEST_IPL("hei_Carrier_int6");
			STREAMING::REQUEST_IPL("hei_carrier_LODLights");

			STREAMING::REQUEST_IPL("hei_yacht_heist");
			STREAMING::REQUEST_IPL("hei_yacht_heist_Bar");
			STREAMING::REQUEST_IPL("hei_yacht_heist_Bedrm");
			STREAMING::REQUEST_IPL("hei_yacht_heist_Bridge");
			STREAMING::REQUEST_IPL("hei_yacht_heist_DistantLights");
			STREAMING::REQUEST_IPL("hei_yacht_heist_enginrm");
			STREAMING::REQUEST_IPL("hei_yacht_heist_LODLights");
			STREAMING::REQUEST_IPL("hei_yacht_heist_Lounge");

			STREAMING::REMOVE_IPL("id2_14_during_door");
			STREAMING::REMOVE_IPL("id2_14_during1");
			STREAMING::REMOVE_IPL("id2_14_during2");
			STREAMING::REQUEST_IPL("id2_14_on_fire");
			STREAMING::REMOVE_IPL("id2_14_post_no_int");
			STREAMING::REMOVE_IPL("id2_14_pre_no_int");

			STREAMING::REQUEST_IPL("ID2_21_G_Night");
			STREAMING::REQUEST_IPL("Jetsteal_ipl_grp1");
			STREAMING::REQUEST_IPL("Jetsteal_ipl_grp2");
			STREAMING::REQUEST_IPL("jetstealtunnel");
			STREAMING::REQUEST_IPL("jewel2fake");
			STREAMING::REQUEST_IPL("Jewel_Gasmasks");
			STREAMING::REQUEST_IPL("layer_sextoys_a");
			STREAMING::REQUEST_IPL("layer_torture");
			STREAMING::REQUEST_IPL("ld_rail_02_track");

			STREAMING::REQUEST_IPL("MG-Flight School 5");
			STREAMING::REQUEST_IPL("Michael_premier");
			STREAMING::REQUEST_IPL("occl_meth_grp1");
			STREAMING::REQUEST_IPL("Plane_crash_trench");
			STREAMING::REQUEST_IPL("post_hiest_unload");

			STREAMING::REQUEST_IPL("prologue01"); //North Yankton
			STREAMING::REQUEST_IPL("prologue01c");
			STREAMING::REQUEST_IPL("prologue01d");
			STREAMING::REQUEST_IPL("prologue01e");
			STREAMING::REQUEST_IPL("prologue01f");
			STREAMING::REQUEST_IPL("prologue01g");
			STREAMING::REQUEST_IPL("prologue01h");
			STREAMING::REQUEST_IPL("prologue01i");
			STREAMING::REQUEST_IPL("prologue01j");
			STREAMING::REQUEST_IPL("prologue01k");
			STREAMING::REQUEST_IPL("prologue01z");
			STREAMING::REQUEST_IPL("prologue02");
			STREAMING::REQUEST_IPL("prologue03");
			STREAMING::REQUEST_IPL("prologue03b");
			STREAMING::REQUEST_IPL("prologue03_grv_fun");
			STREAMING::REQUEST_IPL("prologue04");
			STREAMING::REQUEST_IPL("prologue04b");
			STREAMING::REQUEST_IPL("prologue05");
			STREAMING::REQUEST_IPL("prologue05b");
			STREAMING::REQUEST_IPL("prologue06");
			STREAMING::REQUEST_IPL("prologue06b");
			STREAMING::REQUEST_IPL("prologue06_int");
			STREAMING::REQUEST_IPL("prologuerd");
			STREAMING::REQUEST_IPL("prologuerdb");
			STREAMING::REQUEST_IPL("prologue_DistantLights");
			STREAMING::REQUEST_IPL("prologue_LODLights");
			STREAMING::REQUEST_IPL("prologue_m2_door");
			STREAMING::REQUEST_IPL("facelobby");

			STREAMING::REQUEST_IPL("prop_cheetah_covered");
			STREAMING::REQUEST_IPL("prop_entityXF_covered");
			STREAMING::REQUEST_IPL("prop_jb700_covered");
			STREAMING::REQUEST_IPL("prop_ztype_covered");

			STREAMING::REMOVE_IPL("RC12B_Default");
			STREAMING::REQUEST_IPL("RC12B_Destroyed");
			STREAMING::REMOVE_IPL("RC12B_Fixed");
			STREAMING::REMOVE_IPL("RC12B_HospitalInterior");

			//STREAMING::REQUEST_IPL("REMOVE_ALL_STATES");
			STREAMING::REQUEST_IPL("refit_unload");
			STREAMING::REMOVE_IPL("SC1_01_NewBill");
			STREAMING::REQUEST_IPL("SC1_01_OldBill");
			STREAMING::REQUEST_IPL("SC1_30_Keep_Closed");

			STREAMING::REQUEST_IPL("ship_occ_grp1");
			STREAMING::REQUEST_IPL("ship_occ_grp2");
			STREAMING::REQUEST_IPL("shr_int");
			STREAMING::REQUEST_IPL("smboat");
			STREAMING::REQUEST_IPL("SM_15_BldGRAF1");
			STREAMING::REQUEST_IPL("sunkcargoship");
			STREAMING::REQUEST_IPL("tankerexp_grp0");
			STREAMING::REQUEST_IPL("tankerexp_grp1");
			STREAMING::REQUEST_IPL("tankerexp_grp2");
			STREAMING::REQUEST_IPL("tankerexp_grp3");
			STREAMING::REQUEST_IPL("TrevorsMP");
			STREAMING::REQUEST_IPL("TrevorsTrailer");
			STREAMING::REQUEST_IPL("TrevorsTrailerTidy");
			STREAMING::REQUEST_IPL("TrevorsTrailerTrash");
			STREAMING::REQUEST_IPL("triathlon2_VBprops");

			STREAMING::REQUEST_IPL("TRV1_Trail_Finish");
			STREAMING::REMOVE_IPL("TRV1_Trail_end");
			STREAMING::REMOVE_IPL("TRV1_Trail_start");

			STREAMING::REQUEST_IPL("UFO");
			STREAMING::REQUEST_IPL("V_35_Fireman");
			STREAMING::REQUEST_IPL("VB_08_TriAf01");

			STREAMING::REQUEST_IPL("v_carshowroom");
			STREAMING::REQUEST_IPL("shutter_open");
			STREAMING::REMOVE_IPL("shutter_closed");
			STREAMING::REQUEST_IPL("shr_int");
			STREAMING::REQUEST_IPL("csr_inMission");
			STREAMING::REQUEST_IPL("fakeint");

			STREAMING::REQUEST_IPL("V_Michael_Garage");
			STREAMING::REQUEST_IPL("V_Michael_FameShame");
			STREAMING::REQUEST_IPL("V_Michael_JewelHeist");
			STREAMING::REQUEST_IPL("V_Michael_plane_ticket");
			STREAMING::REQUEST_IPL("V_Michael_Scuba");
			STREAMING::REQUEST_IPL("v_tunnel_hole");
			STREAMING::REQUEST_IPL("v_tunnel_hole_swap");
			STREAMING::REQUEST_IPL("yogagame");
			STREAMING::REQUEST_IPL("FINBANK");
			STREAMING::REQUEST_IPL("DT1_03_Shutter");
			STREAMING::REQUEST_IPL("DT1_03_Gr_Closed");
			STREAMING::REQUEST_IPL("DES_tankercrash");

			PRINT("~g~Loaded IPL");

		}
		else
		{
			STREAMING::REMOVE_IPL("airfield");
			STREAMING::REMOVE_IPL("AP1_04_TriAf01");
			STREAMING::REMOVE_IPL("bh1_16_refurb");
			STREAMING::REMOVE_IPL("BH1_47_JoshHse_Burnt");
			STREAMING::REQUEST_IPL("BH1_47_JoshHse_UnBurnt");
			STREAMING::REMOVE_IPL("bnkheist_apt_dest");
			STREAMING::REQUEST_IPL("bnkheist_apt_norm");
			STREAMING::REMOVE_IPL("burgershot_yoga");
			STREAMING::REMOVE_IPL("burnt_switch_off");
			STREAMING::REMOVE_IPL("cargoship");
			STREAMING::REQUEST_IPL("canyonriver01");//train
			STREAMING::REQUEST_IPL("railing_start");
			STREAMING::REMOVE_IPL("canyonriver01_traincrash");
			STREAMING::REMOVE_IPL("railing_end");
			STREAMING::REMOVE_IPL("chemgrill_grp1");
			STREAMING::REMOVE_IPL("CH1_07_TriAf03");
			STREAMING::REMOVE_IPL("CH3_RD2_BishopsChickenGraffiti");
			//STREAMING::REMOVE_IPL("");
			STREAMING::REMOVE_IPL("chop_props");
			STREAMING::REMOVE_IPL("chophillskennel");
			STREAMING::REQUEST_IPL("Coroner_Int_off");
			STREAMING::REMOVE_IPL("Coroner_Int_on");
			STREAMING::REMOVE_IPL("coronertrash");
			STREAMING::REMOVE_IPL("crashed_cargoplane");
			STREAMING::REMOVE_IPL("CS1_02_cf_offmission");
			STREAMING::REMOVE_IPL("CS1_02_cf_onmission1");
			STREAMING::REMOVE_IPL("CS1_02_cf_onmission2");
			STREAMING::REMOVE_IPL("CS1_02_cf_onmission3");
			STREAMING::REMOVE_IPL("CS1_02_cf_onmission4");
			STREAMING::REMOVE_IPL("CJ_IOABoat");
			STREAMING::REMOVE_IPL("CS2_06_TriAf02");
			STREAMING::REMOVE_IPL("CS3_05_water_grp1");
			STREAMING::REMOVE_IPL("CS3_05_water_grp2");
			STREAMING::REMOVE_IPL("cs3_07_mpgates");
			STREAMING::REMOVE_IPL("CS4_08_TriAf02");
			STREAMING::REMOVE_IPL("CS4_04_TriAf03");
			STREAMING::REMOVE_IPL("CS5_04_MazeBillboardGraffiti");
			STREAMING::REMOVE_IPL("cs5_4_trains");
			STREAMING::REMOVE_IPL("CS5_Roads_RonOilGraffiti");
			STREAMING::REMOVE_IPL("des_farmhouse");
			STREAMING::REQUEST_IPL("des_farmhs_startimap");
			STREAMING::REQUEST_IPL("des_farmhs_startimap");
			STREAMING::REMOVE_IPL("des_farmhs_endimap");
			STREAMING::REMOVE_IPL("des_farmhs_end_occl");
			STREAMING::REMOVE_IPL("DES_Smash2_startimap ");
			STREAMING::REMOVE_IPL("DES_StiltHouse_imapstart");
			STREAMING::REQUEST_IPL("DES_Smash2_endimap ");
			STREAMING::REQUEST_IPL("DES_StiltHouse_imapend");
			STREAMING::REQUEST_IPL("des_stilthouse_rebuild ");
			STREAMING::REMOVE_IPL("DT1_05_HC_REQ");
			STREAMING::REQUEST_IPL("DT1_05_HC_REMOVE");
			STREAMING::REMOVE_IPL("DT1_05_REQUEST");
			STREAMING::REMOVE_IPL("DT1_05_rubble");
			STREAMING::REQUEST_IPL("DT1_17_NewBill");
			STREAMING::REMOVE_IPL("DT1_17_OldBill");
			STREAMING::REMOVE_IPL("DT1_21_prop_lift_on");
			STREAMING::REMOVE_IPL("fakeint");
			STREAMING::REQUEST_IPL("farm");
			STREAMING::REMOVE_IPL("farm_burnt");
			STREAMING::REMOVE_IPL("farm_burnt_props ");
			STREAMING::REMOVE_IPL("farmint_cap");
			STREAMING::REMOVE_IPL("farmint");
			STREAMING::REMOVE_IPL("farm_props");
			STREAMING::REMOVE_IPL("FBI_colPLUG");
			STREAMING::REMOVE_IPL("FIBlobby");
			STREAMING::REQUEST_IPL("FIBlobbyfake");
			STREAMING::REQUEST_IPL("FBI_repair");
			STREAMING::REMOVE_IPL("ferris_finale_Anim");
			STREAMING::REMOVE_IPL("FruitBB");
			STREAMING::REMOVE_IPL("gasparticle_grp2");
			STREAMING::REMOVE_IPL("gasstation_ipl_group1");
			STREAMING::REMOVE_IPL("gasstation_ipl_group2");
			STREAMING::REMOVE_IPL("hei_carrier");
			STREAMING::REMOVE_IPL("hei_carrier_DistantLights");
			STREAMING::REMOVE_IPL("hei_Carrier_int1");
			STREAMING::REMOVE_IPL("hei_Carrier_int2");
			STREAMING::REMOVE_IPL("hei_Carrier_int3");
			STREAMING::REMOVE_IPL("hei_Carrier_int4");
			STREAMING::REMOVE_IPL("hei_Carrier_int5");
			STREAMING::REMOVE_IPL("hei_Carrier_int6");
			STREAMING::REMOVE_IPL("hei_carrier_LODLights");
			STREAMING::REMOVE_IPL("hei_yacht_heist");
			STREAMING::REMOVE_IPL("hei_yacht_heist_Bar");
			STREAMING::REMOVE_IPL("hei_yacht_heist_Bedrm");
			STREAMING::REMOVE_IPL("hei_yacht_heist_Bridge");
			STREAMING::REMOVE_IPL("hei_yacht_heist_DistantLights");
			STREAMING::REMOVE_IPL("hei_yacht_heist_enginrm");
			STREAMING::REMOVE_IPL("hei_yacht_heist_LODLights");
			STREAMING::REMOVE_IPL("hei_yacht_heist_Lounge");
			STREAMING::REQUEST_IPL("id2_14_during_door");
			STREAMING::REQUEST_IPL("id2_14_during1");
			STREAMING::REQUEST_IPL("id2_14_during2");
			STREAMING::REMOVE_IPL("id2_14_on_fire");
			STREAMING::REQUEST_IPL("id2_14_post_no_int");
			STREAMING::REQUEST_IPL("id2_14_pre_no_int");
			STREAMING::REMOVE_IPL("ID2_21_G_Night");
			STREAMING::REMOVE_IPL("Jetsteal_ipl_grp1");
			STREAMING::REMOVE_IPL("Jetsteal_ipl_grp2");
			STREAMING::REMOVE_IPL("jetstealtunnel");
			STREAMING::REMOVE_IPL("jewel2fake");
			STREAMING::REMOVE_IPL("Jewel_Gasmasks");
			STREAMING::REMOVE_IPL("layer_sextoys_a");
			STREAMING::REMOVE_IPL("layer_torture");
			STREAMING::REMOVE_IPL("ld_rail_02_track");
			STREAMING::REMOVE_IPL("MG-Flight School 5");
			STREAMING::REMOVE_IPL("Michael_premier");
			STREAMING::REMOVE_IPL("occl_meth_grp1");
			STREAMING::REMOVE_IPL("Plane_crash_trench");
			STREAMING::REMOVE_IPL("post_hiest_unload");
			STREAMING::REMOVE_IPL("prologue01"); //North Yankton
			STREAMING::REMOVE_IPL("prologue01c");
			STREAMING::REMOVE_IPL("prologue01d");
			STREAMING::REMOVE_IPL("prologue01e");
			STREAMING::REMOVE_IPL("prologue01f");
			STREAMING::REMOVE_IPL("prologue01g");
			STREAMING::REMOVE_IPL("prologue01h");
			STREAMING::REMOVE_IPL("prologue01i");
			STREAMING::REMOVE_IPL("prologue01j");
			STREAMING::REMOVE_IPL("prologue01k");
			STREAMING::REMOVE_IPL("prologue01z");
			STREAMING::REMOVE_IPL("prologue02");
			STREAMING::REMOVE_IPL("prologue03");
			STREAMING::REMOVE_IPL("prologue03b");
			STREAMING::REMOVE_IPL("prologue03_grv_fun");
			STREAMING::REMOVE_IPL("prologue04");
			STREAMING::REMOVE_IPL("prologue04b");
			STREAMING::REMOVE_IPL("prologue05");
			STREAMING::REMOVE_IPL("prologue05b");
			STREAMING::REMOVE_IPL("prologue06");
			STREAMING::REMOVE_IPL("prologue06b");
			STREAMING::REMOVE_IPL("prologue06_int");
			STREAMING::REMOVE_IPL("prologuerd");
			STREAMING::REMOVE_IPL("prologuerdb");
			STREAMING::REMOVE_IPL("prologue_DistantLights");
			STREAMING::REMOVE_IPL("prologue_LODLights");
			STREAMING::REMOVE_IPL("prologue_m2_door");
			STREAMING::REMOVE_IPL("facelobby");
			STREAMING::REQUEST_IPL("RC12B_Default");
			STREAMING::REMOVE_IPL("RC12B_Destroyed");
			STREAMING::REMOVE_IPL("RC12B_Fixed");
			STREAMING::REMOVE_IPL("RC12B_HospitalInterior");
			STREAMING::REMOVE_IPL("refit_unload");
			STREAMING::REQUEST_IPL("SC1_01_NewBill");
			STREAMING::REMOVE_IPL("SC1_01_OldBill");
			STREAMING::REMOVE_IPL("SC1_30_Keep_Closed");
			STREAMING::REMOVE_IPL("ship_occ_grp1");
			STREAMING::REMOVE_IPL("ship_occ_grp2");
			STREAMING::REMOVE_IPL("shr_int");
			STREAMING::REMOVE_IPL("smboat");
			STREAMING::REMOVE_IPL("SM_15_BldGRAF1");
			STREAMING::REMOVE_IPL("sunkcargoship");
			STREAMING::REMOVE_IPL("tankerexp_grp0");
			STREAMING::REMOVE_IPL("tankerexp_grp1");
			STREAMING::REMOVE_IPL("tankerexp_grp2");
			STREAMING::REMOVE_IPL("tankerexp_grp3");
			STREAMING::REMOVE_IPL("TrevorsMP");
			STREAMING::REMOVE_IPL("TrevorsTrailer");
			STREAMING::REMOVE_IPL("TrevorsTrailerTidy");
			STREAMING::REMOVE_IPL("TrevorsTrailerTrash");
			STREAMING::REMOVE_IPL("triathlon2_VBprops");
			STREAMING::REMOVE_IPL("TRV1_Trail_Finish");
			STREAMING::REMOVE_IPL("TRV1_Trail_end");
			STREAMING::REQUEST_IPL("TRV1_Trail_start");
			STREAMING::REMOVE_IPL("v_carshowroom");
			STREAMING::REMOVE_IPL("shutter_open");
			STREAMING::REQUEST_IPL("shutter_closed");
			STREAMING::REMOVE_IPL("shr_int");
			STREAMING::REMOVE_IPL("csr_inMission");
			STREAMING::REMOVE_IPL("fakeint");
			STREAMING::REMOVE_IPL("V_Michael_Garage");
			STREAMING::REMOVE_IPL("V_Michael_FameShame");
			STREAMING::REMOVE_IPL("V_Michael_JewelHeist");
			STREAMING::REMOVE_IPL("V_Michael_plane_ticket");
			STREAMING::REMOVE_IPL("V_Michael_Scuba");
			STREAMING::REMOVE_IPL("v_tunnel_hole");
			STREAMING::REMOVE_IPL("v_tunnel_hole_swap");
			STREAMING::REMOVE_IPL("yogagame");
			STREAMING::REMOVE_IPL("FINBANK");
			STREAMING::REMOVE_IPL("DT1_03_Shutter");
			STREAMING::REMOVE_IPL("DT1_03_Gr_Closed");
			STREAMING::REMOVE_IPL("DES_tankercrash");
			PRINT("~r~Removed IPL");
		}
	}
}
#pragma endregion

#pragma region ------Mod------

union ScriptArg { unsigned int ui; float f; int i; char *str; };
namespace SCRIPTL {
	static void TRIGGER_SCRIPT_EVENT_3(int always1, ScriptArg *argsStruct, int argsStructCount, int playerbitset) { invoke<Void>(0x54763B35, always1, argsStruct, argsStructCount, playerbitset); } // 0x54763B35
};
namespace PlayerMods {
	namespace eventCreator {
		void callScriptEvent(ScriptArg *args, int argsCount, unsigned int playerBits) {
			SCRIPTL::TRIGGER_SCRIPT_EVENT_3(1, args, argsCount, playerBits);
		}
	};
};

void drawneons(int R, int G, int B)
{
	int VehicleID = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(VehicleID, true);
	GRAPHICS::DRAW_LIGHT_WITH_RANGE_1(Coords.x, Coords.y, Coords.z, R, G, B, 4.0f, 2.0f);
}


void AIWanderCar(Ped playerPed)
{
	Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	AI::TASK_VEHICLE_DRIVE_WANDER(playerPed, playerVeh, 100.0f, 0);
}

void doKeyboardFunc(char* Result, int KBIndex, int Client)
{
	char buf[100];
	switch (KBIndex)
	{
	case 1: TextMessages(PLAYER::GET_PLAYER_PED(selectedPlayer), Result, 0); break;
	}
	KeyboardIndex = 0;// y
}

void BossMode()
{
	bool exploded = false;
	int LocalID = PLAYER::PLAYER_ID();
	int LocalPED = PLAYER::PLAYER_PED_ID();
	for (int i = 0; i < 18; i++)
	{
		if (PLAYER::IS_PLAYER_TARGETTING_ENTITY(i, LocalPED) && !exploded)
		{
			Vector3 Pos = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(i), true);
			FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z, type_, 5.0, 1, 0, 0);
			exploded = true;
		}
	}
}

void dropPickup(Vector3 Location, char* PickupModel, char* PickupBehavior, int PickupAmount, char* DisplayName, int Client)
{
	int Model = GAMEPLAY::GET_HASH_KEY(PickupModel);
	int Behavior = GAMEPLAY::GET_HASH_KEY(PickupBehavior);
	STREAMING::REQUEST_MODEL(Model);
	if (STREAMING::HAS_MODEL_LOADED(Model))
	{
		OBJECT::CREATE_AMBIENT_PICKUP(Behavior, Location.x, Location.y, Location.z, 0, PickupAmount, Model, 0, 1);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Model);
	}
}


void TeleportToPLayer(int client)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
	{
		Vector3 Coords12 = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(client), 1);
		int vehid = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
		ENTITY::SET_ENTITY_COORDS_1(vehid, Coords12.x, Coords12.y, Coords12.z, 1, 0, 0, 1);
	}
	else
	{
		Vector3 Coords12 = ENTITY::GET_ENTITY_COORDS(PLAYER::GET_PLAYER_PED(client), 1);
		ENTITY::SET_ENTITY_COORDS_1(PLAYER::PLAYER_PED_ID(), Coords12.x, Coords12.y, Coords12.z, 1, 0, 0, 1);
	}
}

char *dict = "laddersbase";
char *anim = "base_left_hand_up";

void teleportToMeFoot(int player)
{
	int targ_ped = PLAYER::GET_PLAYER_PED(player);
	STREAMING::REQUEST_ANIM_DICT(dict);
	if (STREAMING::HAS_ANIM_DICT_LOADED(dict))
	{
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
		int sceneID = NETWORK::NETWORK_CREATE_SYNCHRONISED_SCENE_3(coords.x, coords.y, coords.z, 0, 0, 0, 2, 0, 1, 1.0f, 0.0f, 1.0f);
		NETWORK::NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE_3(targ_ped, sceneID, dict, anim, 1000, -1000, 269, 0, 1000, 1);
		NETWORK::NETWORK_START_SYNCHRONISED_SCENE(sceneID);
	}
}

bool freezeclear;
void telefoot(int playerid, char* anim, char* animid, float X, float Y, float Z)
{
	if (freezeclear)
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(playerid);
		Vector3 pedpos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerid, 0.0f, 0.0f, 0.0f);
		int scene2 = NETWORK::NETWORK_CREATE_SYNCHRONISED_SCENE_3(X, Y, Z, 0.0, 0.0, 0.0, 9, 0, 0, 0, 0, 0);
		NETWORK::NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE_3(playerid, scene2, anim, animid, 8.0f, -8.0, 5, 0, 30, 0);
		NETWORK::NETWORK_START_SYNCHRONISED_SCENE(scene2);
	}
	else
	{
		int targ_ped = playerid;
		STREAMING::REQUEST_ANIM_DICT(anim);
		if (STREAMING::HAS_ANIM_DICT_LOADED(anim))
		{
			Vector3 coords = ENTITY::GET_ENTITY_COORDS(targ_ped, 1);
			int scene2 = NETWORK::NETWORK_CREATE_SYNCHRONISED_SCENE_3(X, Y, Z, 0.0, 0.0, 0.0, 9, 0, 0, 0, 0, 0);
			NETWORK::NETWORK_ADD_PED_TO_SYNCHRONISED_SCENE_3(playerid, scene2, anim, animid, 8.0f, -8.0, 5, 0, 30, 0);
			NETWORK::NETWORK_START_SYNCHRONISED_SCENE(scene2);
		}
	}
}

static void TriggerScriptEvent(int* Args, int ArgCount, Player BitsetPlayer)
{
	int Bitset = 0; GAMEPLAY::SET_BIT(&Bitset, BitsetPlayer);
	if (Bitset != 0)
	{
		SCRIPT::TRIGGER_SCRIPT_EVENT(1, Args, ArgCount, Bitset);
	}
}

void TSEvent(int arg1, int arg2, int arg3, int arg4, int arg5, int player)
{
	int args[5];
	args[0] = arg1;
	args[1] = arg2;
	args[2] = arg3;
	args[3] = arg4;
	args[4] = arg5;
	unsigned int bits = (1 << player);
	TriggerScriptEvent(args, 5, bits);
}
void TSEvent2(int arg1, int arg2, int arg3, int arg4, int arg5, int player)
{
	int args[5];
	args[0] = arg1;
	args[1] = arg2;
	args[2] = arg3;
	args[3] = arg4;
	args[4] = arg5;
	unsigned int bits = (1 << player);
	callScriptEvent(args, 5, bits);
}

void RollingInfinite(int player)
{
	setPlayerStatInt(player, 65, -30000);
	setPlayerStatInt(player, 70, -30000);
}
void RollingInfiniteNot(int player)
{
	setPlayerStatInt(player, 65, 30000);
	setPlayerStatInt(player, 70, 30000);
}
void PlayerStatMAX(int player)
{
	setPlayerStatInt(player, 72, -200);
	setPlayerStatInt(player, 73, -200);
	setPlayerStatInt(player, 74, -200);
	setPlayerStatInt(player, 75, -200);
	setPlayerStatInt(player, 76, -200);
	setPlayerStatInt(player, 77, -200);
	setPlayerStatInt(player, 78, -200);
	setPlayerStatInt(player, 79, -200);
}

void sendCash(int player, int amount) {
	int args[5];
	args[0] = 219;//0xDB
	args[1] = 40;//player...   I think you set to 40 to make it work post-patch. I haven't tested
	args[2] = amount;
	args[3] = -1;
	args[4] = 1;
	unsigned int bits = (1 << player);
	callScriptEvent(args, 5, bits);
}
void resetPlayerView(int player) {
	int args[3];
	args[0] = 0x2;
	args[1] = player;
	args[2] = 0x2e;//46
	unsigned int bits = (1 << player);
	callScriptEvent(args, 3, bits);
}
void removePlayerWantedLevel(int player)
{
	int args[2];
	args[0] = 356;
	args[1] = player;
	unsigned int bits = (1 << player);
	callScriptEvent(args, 3, bits);
}


void toggleMobileRadio()
{
	int station = AUDIO::GET_PLAYER_RADIO_STATION_INDEX();
	if (mobileRadio)
	{
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		AUDIO::SET_MOBILE_PHONE_RADIO_STATE(0);
		mobileRadio = false;
	}
	else
	{
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		AUDIO::SET_MOBILE_PHONE_RADIO_STATE(1);
		AUDIO::SET_RADIO_TO_STATION_INDEX(station);
		mobileRadio = true;
	}
}

void doForceField(int client)
{
	Vector3 Pos = ENTITY::GET_ENTITY_COORDS(client, true);
	FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z, 29, FLT_MAX, false, true, 0.0f);
	FIRE::ADD_EXPLOSION(Pos.x, Pos.y, Pos.z, 26, FLT_MAX, false, true, 0.0f);
}


#pragma endregion

#pragma region ------Functions------
void Functions()
{
	if (carinvstext) 
	{ 
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0)); 
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), false); 
	}
	else 
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0)); 
		ENTITY::SET_ENTITY_VISIBLE(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), true); 
	}

	if (hasNoCollision)
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0));
		ENTITY::SET_ENTITY_COLLISION1(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), false);
	}
	else
	{
		NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0));
		ENTITY::SET_ENTITY_COLLISION1(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), true);
	}
	if (VehicleGodMode)
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), 1);
	}
	else
	{
		ENTITY::SET_ENTITY_INVINCIBLE(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), 0);
	}
	if (Tunable_Snow == false) { SetTunable(4715, 0); }
	else if (Tunable_Snow == true) { SetTunable(4715, 1); }
	if (RunSpeed)
	{
		PLAYER::_SET_MOVE_SPEED_MULTIPLIER(PLAYER::PLAYER_ID(), 1.49f);
	}
	else
	{
		PLAYER::_SET_MOVE_SPEED_MULTIPLIER(PLAYER::PLAYER_ID(), 1);

	}
	if (NoRagdoll)
	{
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), false);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), false);
		SET_PED_RAGDOLL_ON_COLLISION(PLAYER::PLAYER_PED_ID(), false);
		SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), false);
	}
	else
	{
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(PLAYER::PLAYER_PED_ID(), true);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(PLAYER::PLAYER_PED_ID(), true);
		SET_PED_RAGDOLL_ON_COLLISION(PLAYER::PLAYER_PED_ID(), true);
		SET_PED_CAN_RAGDOLL(PLAYER::PLAYER_PED_ID(), true);
	}
	if (RevealPlayers)
	{
		Write_Global(2394218 + 56, 2147483647);
		Write_Global(2390201 + PLAYER::PLAYER_ID() * 223 + 172, 1);
	}
	else
	{
		Write_Global(2390201 + PLAYER::PLAYER_ID() * 223 + 172, 0);
	}
	if (Copsturnblindeye)
	{
		Write_Global(2410912 + 4141, 5);
		Write_Global(2410912 + 4144, 2147483647);
	}
	else
	{
		Write_Global(2410912 + 4141, 0);
	}
	if (OffRadar)
	{
		WriteInt32(ReadInt32(0x1E70394) + 0x22284, 2147483647);
		Write_Global(2390201 + PLAYER::PLAYER_ID() * 223 + 170, 1);
		Write_Global(2394218 + 55, 2147483647);
	}
	else
	{
		WriteInt32(ReadInt32(0x1E70394) + 0x22284, 0);
	}

	if (NoTarget)
	{
		PED::SET_PED_CAN_BE_TARGETTED(PLAYER::PLAYER_PED_ID(), false);
	}
	else
	{
		PED::SET_PED_CAN_BE_TARGETTED(PLAYER::PLAYER_PED_ID(), true);
	}


}
#pragma endregion

#pragma region Protections
void PTFXimport()
{

	WriteBytes(0x12D4FF8, ON, 4);
	WriteBytes(0x12C6950, ON, 4);//Lazer
	PRINT("PTFX Protection: ~g~ADDED");

}
void PTFXDefault()
{

	WriteBytes(0x12D4FF8, OFF, 4);
	WriteBytes(0x12C6950, OFF, 4);//Lazer
	PRINT("PTFX Protection: ~r~Deleted");
}
void PTFXs()
{
	if (PTFX)
	{
		PTFXimport();
	}
	else
	{
		PTFXDefault();
	}

}
void Taskimport()
{
	WriteBytes(0x12CB0F8, ON, 4);
	WriteBytes(0x12CB4BC, ON, 4);
	WriteBytes(0x12CB66C, ON, 4);
	WriteBytes(0x12CB890, ON, 4);
	PRINT("Task Protection: ~g~ADDED");

}
void TaskDefault()
{
	WriteBytes(0x12CB0F8, OFF, 4);
	WriteBytes(0x12CB4BC, OFF, 4);
	WriteBytes(0x12CB66C, OFF, 4);
	WriteBytes(0x12CB890, OFF, 4);
	PRINT("Task Protection: ~r~Deleted");
}
void SendFreezeProtect()
{
	if (freezecar)
	{
		PRINT("Freeze Car Protection : ~g~ADDED");
	}
	else
	{
		PRINT("Freeze Car Protection : ~r~DELETED");
	}
}
void Tasks()
{
	if (TaskP)
	{
		Taskimport();
	}
	else
	{

		TaskDefault();
	}

}
void CTaskPimport()
{
	WriteBytes(0x12CC8B8, ON, 4);
	PRINT("Clear Task Protection: ~g~ADDED");
}
void CTaskPDefault()
{
	WriteBytes(0x12CC8B8, OFF, 4);
	PRINT("Clear Task Protection: ~r~Deleted");
}
void ClearTaskPs()
{
	if (ClearTaskP)
	{
		CTaskPimport();
	}
	else
	{
		CTaskPDefault();
	}

}
void ExplosionPimport()
{
	WriteBytes(0x12C49A4, ON, 4);
	WriteBytes(0x12C5598, ON, 4);
	PRINT("Explosion Protection: ~g~ADDED");

}
void ExplosionPDefault()
{
	WriteBytes(0x12C49A4, OFF, 4);
	WriteBytes(0x12C5598, OFF, 4);
	PRINT("Explosion Protection: ~r~Deleted");

}
void ExplosionPs()
{
	if (ExplosionP)
	{
		ExplosionPimport();
	}
	else
	{

		ExplosionPDefault();
	}

}
void KickVoteImport()
{
	WriteBytes(0x12D1D88, ON, 4);
	WriteBytes(0x12C2B98, ON, 4);
	PRINT("Kick Vote Protection: ~g~ADDED");
}
void KickVoteDefault()
{
	WriteBytes(0x12D1D88, OFF, 4);
	WriteBytes(0x12C2B98, OFF, 4);
	PRINT("Kick Vote Protection: ~r~Deleted");
}
void KickVotes()
{
	if (KickVote)
	{
		KickVoteImport();

	}
	else
	{
		KickVoteDefault();
	}

}
void VSpeedPImport()
{
	WriteBytes(0x12BD2CC, ON, 4);
	PRINT("Vehicle Speed Control Protection: ~g~ADDED");

}
void VSpeedPDefault()
{
	WriteBytes(0x12BD2CC, OFF, 4);
	PRINT("Vehicle Speed Control Protection: ~r~Deleted");

}
void VSpeedPs()
{
	if (VSpeedP)
	{
		VSpeedPImport();

	}
	else
	{
		VSpeedPDefault();

	}
}
void RemoveWeaponImport()
{
	WriteBytes(0x12C3BD4, ON, 4);
	WriteBytes(0x12C3A2C, ON, 4);
	PRINT("Remove Weapon Protection: ~g~ADDED");

}
void RemoveWeaponDefault()
{
	WriteBytes(0x12C3BD4, OFF, 4);
	WriteBytes(0x12C3A2C, OFF, 4);
	PRINT("Remove Weapon Protection: ~r~Deleted");
}
void RemoveWeaponProtect()
{
	if (RemoveWeaponP)
	{
		RemoveWeaponImport();

	}
	else
	{
		RemoveWeaponDefault();
	}

}
void FreezeCarImport()
{
	WriteBytes(0x13BF850, ON, 4);
	WriteBytes(0xAF2580, ON, 4);
}
void FreezeCarDefault()
{
	WriteBytes(0x13BF850, OFF, 4);
	WriteBytes(0xAF2580, OFF, 4);
}

void RankProtect()
{
	WriteBytes(0x12CBD94, ON, 4);
	WriteBytes(0x12D1D88, ON, 4);
	WriteBytes(0x172D78C, ONN, 4);
	PRINT("SCRIPT Protection ~g~ADDED", 1000);
}
void NonProtect()
{
	WriteBytes(0x12CBD94, ON, 4);
	WriteBytes(0x12D1D88, ON, 4);
	WriteBytes(0x172D78C, OF, 4);
	PRINT("SCRIPT Protection ~r~DELETED", 1000);
}

void RPprotection2()
{
	if (RPprotection)
	{
		RankProtect();
	}
	else
	{
		NonProtect();
	}
}
void DisableIdleKick()
{
	SetTunable(73, 0x3B9ACA00);
	SetTunable(74, 0x3B9ACA00);
	SetTunable(75, 0x3B9ACA00);
	SetTunable(76, 0x3B9ACA00);
	PRINT("Idle Kick ~r~Disabled", 3000);
}
void SessionCheck()
{
	if (NETWORK::NETWORK_IS_IN_SESSION())
	{

		if (freezecar)
		{
			FreezeCarImport();
		}
	}
	else
	{
		if (freezecar)
		{
			FreezeCarDefault();
		}
	}
}
void FreezeCarProtect()
{
	if (freezecar)
	{
		SessionCheck();
	}
	else
	{
		FreezeCarDefault();
	}
}
#pragma endregion

#pragma region WeaponEdtior
void UnlimitedAmmo2()
{
	if (UnlimitedAmmo1)
	{
		char AAA[] = { 59, 160, 3, 231 };
		char BBB[] = { 56, 224, 0, 99 };
		WriteBytes(0xFD2A44, AAA, 4);
		WriteBytes(0xFDDDC0, BBB, 4);
	}
	else
	{
		char CCC[] = { 99,253,0,0 };
		char DDD[] = { 96,135,0,0 };
		WriteBytes(0xFD2A44, CCC, 4);
		WriteBytes(0xFDDDC0, DDD, 4);
	}
}

void GiveMoneyGunMe()
{
	if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
	{
		float Loc[3];
		if (__GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), Loc))
		{
			Vector3 Pos;
			Pos.x = Loc[0]; Pos.y = Loc[1]; Pos.z = Loc[2];
			STREAMING::REQUEST_MODEL(0x113FD533);
			if (STREAMING::HAS_MODEL_LOADED(0x113FD533) == 1)
			{
				OBJECT::CREATE_AMBIENT_PICKUP(0xCE6FDD6B, Pos.x, Pos.y, Pos.z, 0, 40000, 0x113FD533, 0, 1);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(0x113FD533);
			}
		}
		else
		{
			Vector3 AAX = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			if (STREAMING::HAS_MODEL_LOADED(0x113FD533) == 1)
			{
				OBJECT::CREATE_AMBIENT_PICKUP(0xCE6FDD6B, AAX.x, AAX.y, AAX.z, 0, 40000, 0x113FD533, 0, 1);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(0x113FD533);
			}
		}

	}
}

void MeTeleportGun()
{
	float Loc[3];
	if (PED::IS_PED_SHOOTING(PLAYER::PLAYER_PED_ID()))
	{
		if (__GET_PED_LAST_WEAPON_IMPACT_COORD(PLAYER::PLAYER_PED_ID(), Loc))
		{
			Vector3 Pos;
			Pos.x = Loc[0]; Pos.y = Loc[1]; Pos.z = Loc[2];
			ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), Pos.x, Pos.y, Pos.z, 0, 0, 0, 1);
		}
		else
		{
			PRINT("~r~No Coords found", 1000);
		}
	}
}
#pragma endregion

#pragma region TeleportToLoc
void TeleportToLoc(float x, float y, float z)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
	{
		int vehid = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
		ENTITY::SET_ENTITY_COORDS_1(vehid, x, y, z, 1, 0, 0, 1);
	}
	else
	{
		ENTITY::SET_ENTITY_COORDS_1(PLAYER::PLAYER_PED_ID(), x, y, z, 1, 0, 0, 1);
		STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
		GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_rcbarry2");
		GRAPHICS::_START_PARTICLE_FX_NON_LOOPED_ON_PED_BONE_2("scr_clown_appears", PLAYER::PLAYER_PED_ID(), 0, 0, 0, 0, 0, 0, 24818, 1.0f, 0, 0, 0);
	}
}
#pragma endregion

#pragma region ESP
void ESPing2(int Client)
{
	NameESP(Client);
	Linie(Client);
	BoxESP(Client);
}
#pragma endregion

#pragma region
Hash WepArray[58] = { 0x99B507EA, 0x678B81B1, 0x4E875F73, 0x958A4A8F, 0x440E4788, 0x84BD7BFD, 0x1B06D571, 0x5EF9FEC4, 0x22D8FE39, 0x99AEEB3B, 0x13532244, 0x2BE6766B,
0xEFE7E2DF, 0xBFEFFF6D, 0x83BF0278, 0xAF113F99, 0x9D07F764, 0x7FD62962, 0x1D073A89, 0x7846A318, 0xE284C527, 0x9D61E50F, 0x3656C8C1, 0x5FC3C11, 0xC472FE2,
0xA284510B, 0x4DD2DC56, 0xB1CA77B1, 0x687652CE, 0x42BF8A85, 0x93E220BD, 0x2C3731D9, 0xFDBC8A50, 0xA0973D5E, 0x24B17070, 0x60EC506, 0x34A67B97, 0x23C9F95C,
0x497FACC3, 0xF9E6AA4B, 0x61012683, 0xD205520E, 0xBFD21232, 0x92A27487, 0x83839C4, 0xA89CB99E, 0xC0A3098D, 0x7F229F94, 0x7F7497E5, 0x787F0BB, 0xAB564B93,
0x63AB0442, 0x47757124, 0xA3D4D34, 0x3AABBBAA, 0xC734385A, 0xA3D4D34, 0xDC4DB296 };
#pragma endregion

#pragma region Animation
bool doAnimation(char* AnimDict, char* AnimID)
{
	int pPlayer = PLAYER::PLAYER_PED_ID();
	RequestNetworkControl(pPlayer);
	STREAMING::REQUEST_ANIM_DICT(AnimDict);
	if (upperbody)
	{
		if (STREAMING::HAS_ANIM_DICT_LOADED(AnimDict))
		{
			AI::TASK_PLAY_ANIM(pPlayer, AnimDict, AnimID, 8.0f, 0.0f, -1, 49, 0, 0, 0, 0);
			return true;
		}
	}
	else
	{
		if (STREAMING::HAS_ANIM_DICT_LOADED(AnimDict))
		{
			AI::TASK_PLAY_ANIM(pPlayer, AnimDict, AnimID, 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
			return true;
		}
	}
}
char *Animation[] = { "PoleDance","PoleDance2","Sitting","Coffee" };
char *Animation1[] = { "mini@strip_club@pole_dance@pole_a_2_stage","mini@strip_club@pole_dance@pole_b_2_stage","amb@code_human_in_bus_passenger_idles@female@tablet@base","amb@code_human_in_bus_passenger_idles@male@coffee@base" };
char *Animation2[] = { "pole_a_2_stage","pole_b_2_stage","base","base" };
int AnimationV;
#pragma endregion

#pragma region Costume
int Familyingvar;
char *Familying[] = { "帽子","メガネ","耳","顔","マスク","髪の毛","上半身","ズボン","パラシュート","足","スペシャル1","スペシャル2","スペシャル3","テクスチャ","上半身2" };
char *Family[] = { "HATS","GLASSES","EARS","FACE","MASK","HAIR","TORSO","LEGS","HANDS","SHOES","SPECIAL1","SPECIAL2","SPECIAL3","TEXTURE","TORSO2" };
int HATSTexturevar;
int HATSvar;
int fam;
void Modelo(char* family, int model, int texture, int palette)
{
	if (cstrcmp(family, "HATS") || cstrcmp(family, "GLASSES") || cstrcmp(family, "EARS"))
	{
		if (cstrcmp(family, "HATS"))
			fam = 0;
		else if (cstrcmp(family, "GLASSES"))
			fam = 1;
		else if (cstrcmp(family, "EARS"))
			fam = 2;
		PED::SET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), fam, model, texture, palette);
	}
	else
	{
		if (cstrcmp(family, "FACE"))
			fam = 0;
		else if (cstrcmp(family, "MASK"))
			fam = 1;
		else if (cstrcmp(family, "HAIR"))
			fam = 2;
		else if (cstrcmp(family, "TORSO"))
			fam = 3;
		else if (cstrcmp(family, "LEGS"))
			fam = 4;
		else if (cstrcmp(family, "HANDS"))
			fam = 5;
		else if (cstrcmp(family, "SHOES"))
			fam = 6;
		else if (cstrcmp(family, "SPECIAL1"))
			fam = 7;
		else if (cstrcmp(family, "SPECIAL2"))
			fam = 8;
		else if (cstrcmp(family, "SPECIAL3"))
			fam = 9;
		else if (cstrcmp(family, "TEXTURE"))
			fam = 10;
		else if (cstrcmp(family, "TORSO2"))
			fam = 11;
		PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), fam, model, texture, palette);
	}
}

void CopyingOutfit()
{
	Modelo("HATS", PED::GET_PED_PROP_INDEX(PLAYER::GET_PLAYER_PED(selectedPlayer), 0), PED::GET_PED_PROP_TEXTURE_INDEX(PLAYER::GET_PLAYER_PED(selectedPlayer), 0), 0);
	Modelo("GALLSSES", PED::GET_PED_PROP_INDEX(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), PED::GET_PED_PROP_TEXTURE_INDEX(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), 0);
	Modelo("EARS", PED::GET_PED_PROP_INDEX(PLAYER::GET_PLAYER_PED(selectedPlayer), 2), PED::GET_PED_PROP_TEXTURE_INDEX(PLAYER::GET_PLAYER_PED(selectedPlayer), 2), 0);

	Modelo("FACE", PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 0), PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 0), PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 0));
	Modelo("MASK", PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 1), PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 1));
	Modelo("HAIR",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 2),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 2),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 2));
	Modelo("TORSO",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 3),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 3),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 3));
	Modelo("LEGS",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 4),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 4),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 4));
	Modelo("HANDS",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 5),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 5),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 5));
	Modelo("SHOES",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 6),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 6),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 6));
	Modelo("SPECIAL1",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 7),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 7),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 7));
	Modelo("SPECIAL2",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 8),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 8),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 8));
	Modelo("SPECIAL3",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 9),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 9),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 9));
	Modelo("TEXTURE",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 10),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 10),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 10));
	Modelo("TORSO2",
		PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 11),
		PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 11),
		PED::GET_PED_PALETTE_VARIATION(PLAYER::GET_PLAYER_PED(selectedPlayer), 11));
}
int DRAWABLE_VAR[12];
int TEXTURE_VAR[12];
int PALETTE_VAR[12];
bool SET_PLAYER_MODEL(Hash ModelHash)
{
	Vehicle CurrentVeh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
	STREAMING::REQUEST_MODEL(ModelHash);
	if (STREAMING::HAS_MODEL_LOADED(ModelHash))
	{

		Hash WEAPONINFO_Current;
		bool WEAPONINFO_GotWeapon[58];
		int WEAPONINFO_Ammo[58];
		bool IsGood = WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &WEAPONINFO_Current, false);
		for (int i = 0; i < 58; i++)
		{
			if (WEAPON::HAS_PED_GOT_WEAPON(PLAYER::PLAYER_PED_ID(), WepArray[i], false))
			{
				WEAPONINFO_GotWeapon[i] = true;
				WEAPONINFO_Ammo[i] = WEAPON::GET_AMMO_IN_PED_WEAPON(PLAYER::PLAYER_PED_ID(), WepArray[i]);
			}
		}
		PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), ModelHash);
		PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 200);
		if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == ModelHash)
		{
			for (int i = 0; i < 58; i++)
			{
				if (WEAPONINFO_GotWeapon[i])
				{
					WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), WepArray[i], WEAPONINFO_Ammo[i], false, true);
				}
			}
			if (Godmode)
				WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), WEAPONINFO_Current, true);
			if (ModelHash == 0xB881AEE)//hc_gunman
			{
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 0, 3, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 1, 0, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 2, 0, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 3, 1, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 4, 1, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 6, 1, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 9, 1, 0, 0);
			}
			else if (ModelHash == 0xD86B5A95)//a_c_deer
				ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), true, true);
			else if (ModelHash == 0x1250D7BA)//a_c_mtlion
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), false);
			else if (ModelHash == 0x705E61F2 ||
				ModelHash == 0x1615AD62 ||
				ModelHash == 0x9C9EFFD8 ||
				ModelHash == 0xD174E983)//mp_m_freemode_01 mp_m_freemode_02 mp_f_freemode_01 mp_f_freemode_02
			{
				for (int i = 0; i < 12; i++)
					PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, DRAWABLE_VAR[i], TEXTURE_VAR[i], PALETTE_VAR[i]);
			}

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(ModelHash);
			if (CurrentVeh != 0)
				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), CurrentVeh, -1);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

void SET_STEAL_OUTFIT()
{
	Vehicle CurrentVeh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
	STREAMING::REQUEST_MODEL(ModelHash);
	if (STREAMING::HAS_MODEL_LOADED(ModelHash))
	{

		Hash WEAPONINFO_Current;
		bool WEAPONINFO_GotWeapon[58];
		int WEAPONINFO_Ammo[58];
		bool IsGood = WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &WEAPONINFO_Current, false);
		for (int i = 0; i < 58; i++)
		{
			if (WEAPON::HAS_PED_GOT_WEAPON(PLAYER::PLAYER_PED_ID(), WepArray[i], false))
			{
				WEAPONINFO_GotWeapon[i] = true;
				WEAPONINFO_Ammo[i] = WEAPON::GET_AMMO_IN_PED_WEAPON(PLAYER::PLAYER_PED_ID(), WepArray[i]);
			}
		}
		PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), ModelHash);
		PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 200);
		if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == ModelHash)
		{
			for (int i = 0; i < 58; i++)
			{
				if (WEAPONINFO_GotWeapon[i])
				{
					WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), WepArray[i], WEAPONINFO_Ammo[i], false, true);
				}
			}
			if (Godmode)
				WEAPON::SET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), WEAPONINFO_Current, true);
			if (ModelHash == 0xB881AEE)//hc_gunman
			{
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 0, 3, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 1, 0, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 2, 0, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 3, 1, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 4, 1, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 6, 1, 0, 0);
				PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), 9, 1, 0, 0);
			}
			else if (ModelHash == 0xD86B5A95)//a_c_deer
				ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), true, true);
			else if (ModelHash == 0x1250D7BA)//a_c_mtlion
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), false);
			else if (ModelHash == 0x705E61F2 ||
				ModelHash == 0x1615AD62 ||
				ModelHash == 0x9C9EFFD8 ||
				ModelHash == 0xD174E983)//mp_m_freemode_01 mp_m_freemode_02 mp_f_freemode_01 mp_f_freemode_02
			{
				for (int i = 0; i < 12; i++)
					PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), i, DRAWABLE_VAR[i], TEXTURE_VAR[i], PALETTE_VAR[i]);
			}

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(ModelHash);
			if (CurrentVeh != 0)
				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), CurrentVeh, -1);

			CopyingOutfit();
		}
		else
		{
			PRINT("エラーが発生しました。", 1000);
			CopyingOutfit();
		}
	}
	else
	{
		PRINT("エラーが発生しました。", 1000);
		CopyingOutfit();
	}
}
#pragma endregion