/**
*  Copyright (C) 2016 Rafal Janicki
*
*  This software is provided 'as-is', without any express or implied
*  warranty.  In no event will the authors be held liable for any damages
*  arising from the use of this software.
*
*  Permission is granted to anyone to use this software for any purpose,
*  including commercial applications, and to alter it and redistribute it
*  freely, subject to the following restrictions:
*
*  1. The origin of this software must not be misrepresented; you must not
*     claim that you wrote the original software. If you use this software
*     in a product, an acknowledgment in the product documentation would be
*     appreciated but is not required.
*  2. Altered source versions must be plainly marked as such, and must not be
*     misrepresented as being the original software.
*  3. This notice may not be removed or altered from any source distribution.
*
*  Rafal Janicki
*  ravarcade@gmail.com
*/

#include <locale.h>
#include "Direct3DDevice9Hooks.h"
#include "BAM_for_TPA.h"
#include "BAM_ViewPortSetup.h"
#include "BAM_Tracker.h"

void ProcessUserInput();

struct TablePreset {
	float Angle;
	float Length;
	float ZLevel;
	float YTranslation;
};

struct GlobalSettings {
	float StaticCamPosY;
	float StaticCamPosZ;
	int Rotation;
	float DisplaySize;
	int NativeResWidth;
	int NativeResHeight;
};

std::vector<TablePreset> Presets;
TablePreset CurrentPreset = { 9.055f, 6100.0f, 1771.25f, 0.0f }; // test values
TablePreset CurrentPresetBackup = CurrentPreset;

GlobalSettings Settings = { -500, 800, 3, 42, 1920, 1080 }; // test values

enum {
	HEAD_TRACKING = 0,
	STATIC = 1
} CamMode;

enum {
	DISABLED = 0,
	NORMAL = 1,
	HELPER = 2,
} SetupMode;

float CamLev = 0.001f; // for SetupHelper

// ======= BAM-TRACKER STUFFS BEGIN HERE ====================

BAM_Tracker::BAM_Tracker_Client BAM;

void CreateProjectionAndViewMatrix(float *P, float *V, int resolutionWidth, int resolutionHeight)
{
	// Data from config file (Settings):
	float DisplaySize;
	float DisplayNativeWidth;
	float DisplayNativeHeight;
	int Rotation;
	float AboveScreen;
	float InsideScreen;

	// Data from head tracking
	float ViewerPositionX, ViewerPositionY, ViewerPositionZ;

	// Get data from BAM Tracker
	if (BAM.IsBAMTrackerPresent()) {
		// we use Screen Width & Height as Native Resolution. Only aspect ration is important
		DisplayNativeWidth = (float)BAM.GetScreenWidth(); // [mm]
		DisplayNativeHeight = (float)BAM.GetScreenHeight(); // [mm]

		double w = DisplayNativeWidth, h = DisplayNativeHeight;
		DisplaySize = (float)(sqrt(w*w + h*h) / 25.4); // [mm] -> [inchs]
		CamMode = HEAD_TRACKING;
	}
	else { // or if there is no BAM Tracker use Settings
		DisplayNativeWidth = (float)Settings.NativeResWidth;
		DisplayNativeHeight = (float)Settings.NativeResHeight;
		DisplaySize = Settings.DisplaySize;
		CamMode = STATIC;
	}

	// i should keep is = 0
	Rotation = Settings.Rotation;

	// we have to decide, if we draw on rotated screen or not:
	bool isRotated = resolutionWidth < resolutionHeight;

	// and if FP has rotated screen
	bool isFPRotated = DisplayNativeWidth < DisplayNativeHeight;
	if (isRotated != isFPRotated) {
		float tmp = DisplayNativeWidth;
		DisplayNativeWidth = DisplayNativeHeight;
		DisplayNativeHeight = tmp;
	}

	// constant params for this project
	AboveScreen = 2000.0; // 0.2m
	InsideScreen = 2000.0; // 2.0m

	switch (CamMode) {
	case HEAD_TRACKING:
		double x, y, z;
		BAM.GetPosition(x, y, z);
		ViewerPositionX = (float)x;
		ViewerPositionY = (float)y;
		ViewerPositionZ = (float)z;
		break;

	default:
		ViewerPositionX = 0;
		ViewerPositionY = Settings.StaticCamPosY;
		ViewerPositionZ = Settings.StaticCamPosZ;
		break;
	}

	if (SetupMode == HELPER) {
		// Lets do "not so crazy" thing as you expect.
		// Move Camera to in to range 1 - 128 millimeters away from screen surface
		ViewerPositionZ = CamLev;

		// ... and move on big circle around screen;
		float angle = (float)(BAM.GetTime() * 0.090f * 0.01745329251f);
		float range = (float)(0.25*(DisplayNativeWidth < DisplayNativeHeight ? DisplayNativeWidth : DisplayNativeHeight));
		range *= (CamLev > 10.0) ? 0.01f*CamLev : 0.2f;
		ViewerPositionX = sin(angle)*range;
		ViewerPositionY = cos(angle)*range;
		AboveScreen = ViewerPositionZ - 0.5f;
	}

	// Data build projection matrix
	BuildProjectionMatrix(P,
		DisplaySize,
		DisplayNativeWidth, DisplayNativeHeight,
		(float)resolutionWidth, (float)resolutionHeight,
		0.0f, 0.0f,
		(float)resolutionWidth, (float)resolutionHeight,
		ViewerPositionX, ViewerPositionY, ViewerPositionZ,
		-AboveScreen, InsideScreen,
		Rotation);

	// Build View matrix from parts: Translation, Scale, Rotation
	// .. but first View Matrix has camera position
	float VT[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-ViewerPositionX, -ViewerPositionY, -ViewerPositionZ, 1
	};


	// --- Scale, first some math
	float pixelsToMillimeters = (float)(25.4*DisplaySize / sqrt(DisplayNativeWidth*DisplayNativeWidth + DisplayNativeHeight*DisplayNativeHeight));
	float pixelsToMillimetersX = pixelsToMillimeters * DisplayNativeWidth / resolutionWidth;
	float pixelsToMillimetersY = pixelsToMillimeters * DisplayNativeHeight / resolutionHeight;
	float ptm = Rotation & 1 ? pixelsToMillimetersX : pixelsToMillimetersY;
	float tableLengthInMillimeters = ptm * CurrentPreset.Length;
	float displayLengthInMillimeters = ptm * (Rotation & 1 ? pixelsToMillimeters * DisplayNativeWidth : pixelsToMillimeters * DisplayNativeHeight);

	// --- Scale world to fit in screen
	float scale = displayLengthInMillimeters / tableLengthInMillimeters; // calc here scale
	float S[16] = {
		scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, scale, 0,
		0, 0, 0, 1
	};
	/// ===

	// --- Translation to desired world element (playfield center or glass center)
	float T[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, -CurrentPreset.YTranslation*scale, -CurrentPreset.ZLevel * scale, 1
	};
	/// ===

	// --- Rotate world to make playfield or glass parallel to screen
	const float degToRad = 0.01745329251f;
	float angle = CurrentPreset.Angle * degToRad;
	float _S = sinf(angle);
	float _C = cosf(angle);
	float R[16] = {
		1, 0, 0, 0,
		0, _C, -_S, 0,
		0, _S, _C, 0,
		0, 0, 0, 1
	};
	/// ===

	// combine all to one matrix
	D3DXMATRIX View = D3DXMATRIX(S) * D3DXMATRIX(R) * D3DXMATRIX(T) * D3DXMATRIX(VT);

	// move result to output
	memcpy_s(V, sizeof(float[16]), View, sizeof(float[16]));

	// This function is called once per frame.
	//  use it to process also user input
	ProcessUserInput();
}

// ======= END OF BAM-TRACKER STUFFS ==========
// Part below is done quick and dirty way.

// ======= USER INPUT =========================

int ReadKeyboard(int key)
{
	const double repeat = 20.0; // ms
	const double firstRepeat = 200.0; // ms

	int ret = 0;

	key &= 0xff; // 256 keys
	static SHORT old_states[256];
	static double nextTime[256];
	static bool once = true;
	if (once)
	{
		for (unsigned int i = 0; i<256; ++i)
		{
			old_states[i] = 0;
		}

		once = false;
	}

	double t = BAM.GetTime();

	SHORT state = GetAsyncKeyState(key);
	if (((state << 8) != 0) && ((old_states[key] << 8) == 0)) {
		ret = 1;
		nextTime[key] = t + firstRepeat;
	}
	if (((state << 8) != 0) && ((old_states[key] << 8) != 0)) {
		// continue pressed
		if (nextTime[key] < t) {
			nextTime[key] = t + repeat;
			ret = 1;
		}
	}

	old_states[key] = state;
	return ret;
}

// --- Few globals for user input ----
float UI_Step = 1.0;
bool SetupEnabled = false;

const char *GetFileNameWithPath(const char *FileName)
{
	static char FileNameWithPath[1024];
	const size_t fnLen = sizeof(FileNameWithPath) - 1;

	// get current path to BAM_for_TPA
	HMODULE hm;
	if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)&GetFileNameWithPath, &hm))
	{
		GetModuleFileNameA(hm, FileNameWithPath, fnLen);
		char *p = strrchr(FileNameWithPath, '\\');
		strcpy_s(p, fnLen - (p - FileNameWithPath), "\\");
		++p;
		strcpy_s(p, fnLen - (p - FileNameWithPath), FileName);
	}
	return FileNameWithPath;
}

void SaveCurrentPreset()
{
	// save to file
	FILE *f;
	fopen_s(&f, GetFileNameWithPath("CurrentPreset.txt"), "w");
	if (f)
	{
		fprintf(f, "Angle = %.3f\n", CurrentPreset.Angle);
		fprintf(f, "Length = %.3f\n", CurrentPreset.Length);
		fprintf(f, "ZLevel = %.3f\n", CurrentPreset.ZLevel);
		fprintf(f, "YTranslation = %.3f\n", CurrentPreset.YTranslation);
		fclose(f);
	}
}

const char *ReadTilEq(FILE *&f)
{
	static char buf[1024];
	const size_t bufLen = sizeof(buf) - 1;
	char *p = buf;
	*p = 0;

	if (f)
	{
		if (feof(f)) {
			fclose(f);
			f = NULL;
		}
		else
		{
			char c = 0;
			for (;;)
			{
				fread_s(&c, 1, 1, 1, f);
				if (feof(f)) {
					fclose(f);
					f = NULL;
					break;
				}
				if (c == '\n' || c == '\r')
					p = buf;

				if (::isspace(c))
					continue;

				if (c == '=')
					break;

				if (p - buf < bufLen)
				{
					*p++ = c;
					*p = 0;
				}
			}
		}
	}

	return buf;
}

double ReadF(FILE *&f)
{
	double v = 0;
	static char buf[1024];
	const size_t bufLen = sizeof(buf) - 1;
	char *p = buf;
	*p = 0;

	while (f && !feof(f)) {
		char c = 0;
		fread_s(&c, 1, 1, 1, f);

		if (c == '\n' || c == '\r' || c == '\t')
			break;

		if (isspace(c))
			continue;

		if (p - buf < bufLen)
		{
			*p++ = c;
			*p = 0;
		}
	}

	v = atof(buf);
	return v;
}

void LoadPresetsAndSettings()
{
	setlocale(LC_MONETARY, "English");

	FILE *f;
	fopen_s(&f, GetFileNameWithPath("Settings.txt"), "r");
	while (f) {
		const char *txt = ReadTilEq(f);

		if (_stricmp(txt, "DisplaySize") == 0) Settings.DisplaySize = (float)ReadF(f);
		else if (_stricmp(txt, "Rotation") == 0) Settings.Rotation = (int)ReadF(f);
		else if (_stricmp(txt, "NativeResWidth") == 0) Settings.NativeResWidth = (int)ReadF(f);
		else if (_stricmp(txt, "NativeResHeight") == 0) Settings.NativeResHeight = (int)ReadF(f);
		else if (_stricmp(txt, "StaticCamPosY") == 0) Settings.StaticCamPosY = (float)ReadF(f);
		else if (_stricmp(txt, "StaticCamPosZ") == 0) Settings.StaticCamPosZ = (float)ReadF(f);
	}

	TablePreset tmp;
	int Set = 0;
	fopen_s(&f, GetFileNameWithPath("Presets.txt"), "r");
	while (f) {
		const char *txt = ReadTilEq(f);

		if (_stricmp(txt, "Angle") == 0) {
			tmp.Angle = (float)ReadF(f);
			Set |= 1;
		}
		else if (_stricmp(txt, "Length") == 0) {
			tmp.Length = (float)ReadF(f);
			Set |= 2;
		}
		else if (_stricmp(txt, "YTranslation") == 0) {
			tmp.YTranslation = (float)ReadF(f);
			Set |= 4;
		}
		else if (_stricmp(txt, "ZLevel") == 0) {
			tmp.ZLevel = (float)ReadF(f);
			Set |= 8;
		}

		if (Set == 15) {
			Set = 0;
			Presets.push_back(tmp);
		}
	}

	if (Presets.size()) {
		CurrentPreset = Presets[0];
		CurrentPresetBackup = CurrentPreset;
	}
	else
	{
		Presets.push_back(CurrentPreset);
	}
}

void ProcessUserInput()
{
	static bool once = true;
	if (once)
	{
		once = false;
		LoadPresetsAndSettings();
	}

	if (ReadKeyboard(VK_F1) && UI_Step < 99.0) UI_Step *= 10.0f;
	if (ReadKeyboard(VK_F2)) UI_Step = 1.0f;
	if (ReadKeyboard(VK_F3) && UI_Step > 0.000001) UI_Step *= 0.1f;



	if (ReadKeyboard(VK_F4)) { // reset
		CurrentPreset = CurrentPresetBackup;
		SetupMode = DISABLED;
	}

	if (ReadKeyboard(VK_F5)) { // save preset
		SetupMode = DISABLED;
		SaveCurrentPreset();
	}

	if (ReadKeyboard(VK_F6)) SetupMode = DISABLED;
	if (ReadKeyboard(VK_F7)) SetupMode = NORMAL;
	if (ReadKeyboard(VK_F8)) {
		if (SetupMode != HELPER) {
			CamLev = 0.0001f;
		}
		SetupMode = HELPER;
		CamLev *= 0.5;
		if (CamLev < 0.9)
		{
			CamLev = 128.0f;
		}
	}

	if (ReadKeyboard(VK_F9)) {
		SetupMode = DISABLED;
		CamMode = CamMode == STATIC ? HEAD_TRACKING : STATIC;
	}

	static unsigned int currentPresetIdx = 0;
	if (ReadKeyboard(VK_F10)) { // prev preset
		SetupMode = DISABLED;
		if (currentPresetIdx > 0)
			--currentPresetIdx;

		CurrentPreset = Presets[currentPresetIdx];
		CurrentPresetBackup = CurrentPreset;
	}

	if (ReadKeyboard(VK_F11)) { // next preset
		SetupMode = DISABLED;
		if (currentPresetIdx < Presets.size() - 1)
			++currentPresetIdx;

		CurrentPreset = Presets[currentPresetIdx];
		CurrentPresetBackup = CurrentPreset;
	}


	if (SetupMode == NORMAL || SetupMode == HELPER) {
		if (ReadKeyboard(VK_LEFT)) CurrentPreset.Angle -= UI_Step;
		if (ReadKeyboard(VK_RIGHT)) CurrentPreset.Angle += UI_Step;

		if (ReadKeyboard(VK_DOWN)) CurrentPreset.ZLevel -= 10.0f*UI_Step;
		if (ReadKeyboard(VK_UP)) CurrentPreset.ZLevel += 10.0f*UI_Step;

		if (ReadKeyboard(VK_PRIOR)) CurrentPreset.YTranslation -= 10.0f*UI_Step;
		if (ReadKeyboard(VK_NEXT)) CurrentPreset.YTranslation += 10.0f*UI_Step;

		if (ReadKeyboard(VK_HOME)) CurrentPreset.Length -= 100.0f*UI_Step;
		if (ReadKeyboard(VK_END)) CurrentPreset.Length += 100.0f*UI_Step;
	}
	else if (CamMode == STATIC) {
		if (ReadKeyboard(VK_NEXT)) Settings.StaticCamPosZ -= 10.0f*UI_Step;
		if (ReadKeyboard(VK_PRIOR)) Settings.StaticCamPosZ += 10.0f*UI_Step;

		if (ReadKeyboard(VK_END)) Settings.StaticCamPosY -= 10.0f*UI_Step;
		if (ReadKeyboard(VK_HOME)) Settings.StaticCamPosY += 10.0f*UI_Step;
	}
}
