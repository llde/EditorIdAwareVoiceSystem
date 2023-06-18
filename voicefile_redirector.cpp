#include <StdAfx.h>
#include "obse/PluginAPI.h"
#include "obse/CommandTable.h"
#include "obse_common/SafeWrite.h"
#include "obse/GameData.h"
#include "obse/ParamInfos.h"
#include "obse/Script.h"
#include "obse/GameObjects.h"

#include <string>
#include <vector>

#include "utils.h"
#include "path.h"
#include "override.h"
#include "finder.h"
#include "edid_hook.h"

PluginHandle				g_pluginHandle = kPluginHandle_Invalid;
OBSESerializationInterface* g_serialization = NULL;
OBSEArrayVarInterface*      g_arrayIntfc = NULL;
OBSEScriptInterface	*       g_scriptIntfc = NULL;
OBSEMessagingInterface*		g_msg;

IDebugLog		gLog("voicefile_redirector.log");

static const UInt32 loc_59EC65 = 0x0059EC65;
static const UInt32 SilentVoiceHookPatchAddr = 0x006A9EB7;
static const UInt32 SilentVoiceHookRetnAddr = 0x006A9ECE;
static const UInt32 DialogSubtitleHookPatchAddr = 0x0059EC5F;
static const UInt32 DialogSubtitleHookRetnAddr = 0x0059EC65;
static const UInt32 GeneralSubtitleHookPatchAddr = 0x0062AE3C;
static const UInt32 GeneralSubtitleHookRetnAddr = 0x0062AE43;
static const UInt32 LipsHookPatchAddr = 0x0049411F;
static const UInt32 LipsHookRetnAddr = 0x00494125;

//static const UInt32 CheckFileSub = 0x00431020;
//typedef UInt32 (*_CheckFileSub)(char*, byte, byte, int);
//static const _CheckFileSub CheckFileSub = (_CheckFileSub) 0x00431020;

#define MAX_VOICENAME 256
/*
static void OverWriteSoundFile(void);
static char *pSoundFile;
*/
static char* ConfigurationFile = "Data\\OBSE\\Plugins\\voice_redirector.ini";
static char* OverrideFile = "Data\\OBSE\\Plugins\\voice_redirector.over";
static char* SilentVoiceMp3 = "Data\\OBSE\\Plugins\\elys_usv.mp3";
//static char* SilentVoiceLip = "Data\\OBSE\\Plugins\\Elys_USV";
static char NewSoundFile[MAX_VOICENAME];
/*std::string g_NewVoiceFilename;
//static char *SoundFile = "Data\\sound\\voice\\morrowind_ob.esm\\breton\\m\\fbmwchargen_greeting_00000f48_1.mp3";

static byte DialogSubtitle;
static byte GeneralSubtitle;

static char *pLipFile;
static char TmpLip[MAX_VOICENAME];
static char *pTmpLip;

static __declspec(naked) void SilentVoiceHook(void)
{
	__asm
	{
		mov ecx, esp
		add ecx, 0x64
		mov pSoundFile, ecx
		push eax
		push ebx
		push edx
		call OverWriteSoundFile
		pop edx
		pop ebx
		pop eax
		jmp[SilentVoiceHookRetnAddr]
	}
}

//static char rawdata1[] = { 0x38, 0x0D, 0x00, 0x32, 0xB1, 0x00 };
static __declspec(naked) void DialogSubtitleHook(void)
{
	__asm
	{
		cmp DialogSubtitle, cl
		je No_change
		jmp Jump_back

No_change:
//		db 0x38, 0x0D, 0x00, 0x32, 0xB1, 0x00
		_emit 0x38
		_emit 0x0D
		_emit 0x00
		_emit 0x32
		_emit 0xB1
		_emit 0x00

Jump_back:
		mov DialogSubtitle, 0
		mov GeneralSubtitle, 0
		jmp [DialogSubtitleHookRetnAddr]
	}
}

//static char rawdata2[] = { 0x80, 0x3D, 0x08, 0x32, 0xB1, 0x00, 0x00 };
static __declspec(naked) void GeneralSubtitleHook(void)
{
	__asm
	{
		cmp GeneralSubtitle, 0
		je No_change
		jmp Jump_back

No_change:
//		db 0x80, 0x3D, 0x08, 0x32, 0xB1, 0x00, 0x00
		_emit 0x80
		_emit 0x3D
		_emit 0x08
		_emit 0x32
		_emit 0xB1
		_emit 0x00
		_emit 0x00

Jump_back:
		mov DialogSubtitle, 0
		mov GeneralSubtitle, 0
		jmp[GeneralSubtitleHookRetnAddr]
	}
}

/*
static int CheckFile(void)
{
	_MESSAGE("voicefile_redirector: Calling internal function CheckFile() with '%s'", pTmpLip);

// calling CheckFileSub crashes to desktop
	_asm
	{
		mov ecx, [0xB33A04];
		push -1
		push 0
		push 0
		push pTmpLip
		call CheckFileSub
	}
}
*/
/*
static void CheckLipFile(void)
{
	removeExtension(pLipFile);
	appendToPath(pLipFile, ".lip");
	if (FileExists(pLipFile) == true) return;
	memset(TmpLip, 0, MAX_VOICENAME);
	_MESSAGE("voicefile_redirector: raw Lip file = '%s'", pLipFile);
	char* race = getSingleComponent(Component::Race, pLipFile);
	const char* new_race = getOverrideFor(race, pLipFile, TmpLip);
	delete[] race;
	memset(pLipFile, 0, MAX_VOICENAME);
	if (new_race == nullptr) {
		// no fallbacks found, replace with silent voice mp3
		strcpy(pLipFile, SilentVoiceLip);
	}
	else{
		removeExtension(TmpLip);
//		strcpy(pLipFile, TmpLip);
		pLipFile = &TmpLip[0];
	}
    // BugFix for lip files: for some reason, doing strcpy is not working, so reverting back to old method of
    // redirecting to DLL's internal global variable for lipfilename.
//	_MESSAGE("voicefile_redirector: Lips Hook: replacing '%s' with '%s'", pLipFile, TmpLip);
// strcpy(pLipFile, TmpLip);
	_MESSAGE("voicefile_redirector: Lips Hook: replaced to  '%s.lip'", pLipFile);

}

static __declspec(naked) void LipsHook(void)
{
	__asm
	{
		mov pLipFile, ecx
		push eax
		push ebx
		push edx
		call CheckLipFile
		pop edx
		pop ebx
		pop eax
		mov ecx, pLipFile

		push ecx
		push 0x00A3D9A8
		jmp[LipsHookRetnAddr]
	}
}

static void OverWriteSoundFile(void)
{
	memset(NewSoundFile, 0, MAX_VOICENAME);
	// voice file not found, proceed with fallback mechanism
	_MESSAGE("voicefile_redirector: Silent Voice Hook activated on '%s'", pSoundFile);
	char* race = getSingleComponent(Component::Race, pSoundFile);
	const char* new_race = getOverrideFor(race, pSoundFile, NewSoundFile);
	delete[] race;
	memset(pSoundFile, 0, MAX_VOICENAME);
	if (new_race != nullptr) {
		strcpy(pSoundFile, NewSoundFile);
	}
	else {
		// no other fallbacks found, replace with silent voice mp3
		strcpy(pSoundFile, SilentVoiceMp3);
	}
	_MESSAGE("voicefile_redirector: Silent Voice Hook:  Replaced to '%s'",  pSoundFile);
	DialogSubtitle = 1;
	GeneralSubtitle = 1;
}
*/

/*String already created here*/
static  const UInt32 GetStringHookRetn  = 0x005F737C;
static  const UInt32 GetStringHookAddr = 0x005F7377;
static bool Print = true;
static BOOL  __fastcall GetStringOver(Actor* actor , UInt32 edx ,BSStringT*  string, char* str, UInt32 boh){
	memset(NewSoundFile, 0, MAX_VOICENAME);
	TESNPC* npc = OBLIVION_CAST(actor->baseForm, TESForm, TESNPC);
	if(npc){
		_MESSAGE("voicefile_redirector: Got '%s'  for '%s'",  str, npc->race.race->GetEditorName());
		const char* edid = npc->race.race->GetEditorName();
		replacePathComponent(Component::Race, str, edid, NewSoundFile);
		strcpy(str, NewSoundFile);
		_MESSAGE("voicefile_redirector: After Edid override '%s'",  str);
		if (!FileExists(str)){ /* Test for path with editor id */
			memset(NewSoundFile, 0, MAX_VOICENAME);
			bool override_found = getOverrideFor(edid, str, NewSoundFile); /*First level override*/
			if(override_found)
				strcpy(str, NewSoundFile); //Set back as sound use this, not the BSStringT
			else
				strcpy(str, SilentVoiceMp3); //Silent file
		}
		_MESSAGE("voicefile_redirector: Override '%s'",  str);
	}
	if(Print) printMap();
	Print = false;
	return ThisStdCall(0x004028D0, string, str, boh);
} 

static __declspec(naked) void GetStringHook(void)
{
	__asm
	{
		push ecx
		mov  ecx, esi
		call GetStringOver
		jmp GetStringHookRetn
	}
}
extern "C" {

	bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
	{
		_MESSAGE("voicefile_redirector: OBSE calling plugin's Query function. <Voicefile_Redirector v1.3 alpha>");

		// fill out the info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "voicefile_redirector";
		info->version = 1;

		// version checks
		if (!obse->isEditor)
		{
			if (obse->obseVersion < OBSE_VERSION_INTEGER)
			{
				_ERROR("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER);
				return false;
			}

#if OBLIVION
			if (obse->oblivionVersion != OBLIVION_VERSION)
			{
				_ERROR("incorrect Oblivion version (got %08X need %08X)", obse->oblivionVersion, OBLIVION_VERSION);
				return false;
			}
#endif

			g_serialization = (OBSESerializationInterface *)obse->QueryInterface(kInterface_Serialization);
			if (!g_serialization)
			{
				_ERROR("serialization interface not found");
				return false;
			}

			if (g_serialization->version < OBSESerializationInterface::kVersion)
			{
				_ERROR("incorrect serialization version found (got %08X need %08X)", g_serialization->version, OBSESerializationInterface::kVersion);
				return false;
			}

			g_arrayIntfc = (OBSEArrayVarInterface*)obse->QueryInterface(kInterface_ArrayVar);
			if (!g_arrayIntfc)
			{
				_ERROR("Array interface not found");
				return false;
			}

			g_scriptIntfc = (OBSEScriptInterface*)obse->QueryInterface(kInterface_Script);
		}
		else
		{
			// no version checks needed for editor
			_MESSAGE("voicefile_redirector: not compatible with editor, exiting.");
			return false;

		}

		// version checks pass

		return true;
	}

	bool OBSEPlugin_Load(const OBSEInterface * obse)
	{
		_MESSAGE("voicefile_redirector: OBSE calling plugin's Load function.");

		g_pluginHandle = obse->GetPluginHandle();

		// Hook memory address for silent voice
//		WriteRelJump(SilentVoiceHookPatchAddr, (UInt32)&SilentVoiceHook);
//		_MESSAGE("voicefile_redirector: memory address for SilentVoiceHook patched.");

		// Dialog Subtitle Hook
//		WriteRelJump(DialogSubtitleHookPatchAddr, (UInt32)&DialogSubtitleHook);
//		_MESSAGE("voicefile_redirector: memory address for DialogSubtitlesHook patched.");

		// General Subtitle Hook
//		WriteRelJump(GeneralSubtitleHookPatchAddr, (UInt32)&GeneralSubtitleHook);
//		_MESSAGE("voicefile_redirector: memory address for GeneralSubtitlesHook patched.");

		// Lips Hook
//		WriteRelJump(LipsHookPatchAddr, (UInt32)&LipsHook);
//		_MESSAGE("voicefile_redirector: memory address for LipsHook patched.");
		
		WriteRelJump(GetStringHookAddr, (UInt32)&GetStringHook);
		_MESSAGE("voicefile_redirector: memory address for a BSStringT::Set.");
		
		ApplyEdidHooks(obse);
		

		return true;
	}

};
