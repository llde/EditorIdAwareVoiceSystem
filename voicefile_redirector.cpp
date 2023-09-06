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
OBSEMessagingInterface*		g_msg;
OBSETasksInterface*			g_Task;

IDebugLog		gLog("EditorIDAwareVoiceSounds.log");
#define MAX_VOICENAME 256
const char* name = "EditorID Aware Vocie Sounds";
static const char* ConfigurationFile = "Data\\OBSE\\Plugins\\voice_redirector.ini";
static const char* OverrideFile = "Data\\OBSE\\Plugins\\voice_redirector.over";
//static const char* SilentVoiceMp3 = "Data\\OBSE\\Plugins\\Elys_USV.mp3";
//static char* SilentVoiceLip = "Data\\OBSE\\Plugins\\Elys_USV";
static const char * SilentVoiceMp3 = "Data\\sound\\voice\\Oblivion.esm\\imperial\\m\\Generic_HELLO_0003AA99_1.mp3";

static char NewSoundFile[MAX_VOICENAME];
static void __stdcall  HookedCreateSoundString(Actor* actor, BSStringT* path) {
	_MESSAGE("Call %s   %s", actor->GetEditorName(), path->m_data);
	TESNPC* npc = actor->baseForm->typeID == FormType::kFormType_NPC ? (TESNPC*)actor->baseForm : nullptr;
	char* str = path->m_data;
	if (npc) {
		const char* edid;
		_MESSAGE("%s: Got '%s'  for '%s'",name, str, npc->race.race->GetEditorName());
		auto&& overr = getRaceVoiceOverride(npc->race.race, npc->actorBaseData.IsFemale());
		//	if (overr.empty()) overr.push_back(npc->race); //NO override specified for race
#ifdef DEBUG
		if (!overr.empty()) std::for_each(overr.begin(), overr.end(), [](TESRace* n) { _MESSAGE("Got race override %s", n ? n->GetEditorName() : "<THIS>"); });
#endif
		for (auto& override_race : overr) {
			if (override_race) {
				edid = override_race->GetEditorName();
			}
			else {
				edid = npc->race.race->GetEditorName();
			}
			replacePathComponent(Component::Race, str, edid, NewSoundFile);
			path->Set(NewSoundFile);
			_MESSAGE("voicefile_redirector: After Edid override '%s'", str);
			if (!FileExists(str)) { /* Test for path with editor id */
				memset(NewSoundFile, 0, MAX_VOICENAME);
				bool override_found = getOverrideFor(edid, str, NewSoundFile); /*First level override*/
				if (override_found) {
					path->Set(NewSoundFile);
					_MESSAGE("Override '%s'", str);
					return;
				}
				_MESSAGE("No Voice found for %s", edid);
			}
			else {
				_MESSAGE("Override '%s'", str);
				return;
			}
		}
		path->Set(SilentVoiceMp3);
	}
}

static UInt32 kHookSaveActor = 0x0052E4CA;
static UInt32 kHookSaveActorRetn = 0x0052E4D0;
static Actor* ActorContext = nullptr;

static __declspec(naked) void HookSaveActorContext() {
	__asm {
		push eax 
		mov eax,  [esp + 0x38 + 0x4]
		mov ActorContext, eax
		pop eax
		mov [esp + 0x34 + 0x4], eax
		cmp ebp, ebx 
		jmp [kHookSaveActorRetn]
	}
}
static UInt32 kHookCreateSoundString = 0x0052E64E;
static __declspec(naked) void HookCreateSoundString() {
	__asm {
		push[esp + 0x0 + 0x14]
		push ActorContext
		call HookedCreateSoundString
		mov eax, 1
		retn 0x14
	}
}


static UInt32* kBackgroundLoadLip = (UInt32*)0x00B1490C;
static void EventMessageCallback(OBSEMessagingInterface::Message* msg) {
	switch (msg->type) {

	case OBSEMessagingInterface::kMessage_GameInitialized:
		_MESSAGE("Fixups Race Voice Overrides");
		*kBackgroundLoadLip = 0;  //Disable LipAsyncTask, force the setting, as jumping cause the subtitle to not appear. 
		//The original task seems to have an issue where redirected hello (except changing only the race field apparently) doesn't play
		ApplyTransform([](TESRace* refID) { return (TESRace*) LookupFormByID((UInt32)refID); });
		printMap();
		break;
	default:
		return;
	}
}


static void TaskFunction() {
	static bool DoOnce = 0;
	if (DoOnce == 0) {
		_MESSAGE("Fixups Race Voice Overrides");
		ApplyTransform([](TESRace* refID) { return (TESRace*)LookupFormByID((UInt32)refID); });
		*kBackgroundLoadLip = 0;
		printMap();
		DoOnce = 1;
	}
}

extern "C" {

	bool OBSEPlugin_Query(const OBSEInterface * obse, PluginInfo * info)
	{
		_MESSAGE("%s: OBSE calling plugin's Query function. <%s v1.0 beta>", name, name);

		// fill out the info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = name;
		info->version = 1;
		g_pluginHandle = obse->GetPluginHandle();

		// version checks
		if (!obse->isEditor)
		{
			if (obse->obseVersion < OBSE_VERSION_INTEGER)
			{
				_ERROR("OBSE version too old (got %08X expected at least %08X)", obse->obseVersion, OBSE_VERSION_INTEGER);
				return false;
			}

			if (!(OBSETasks2Interface*)obse->QueryInterface(kInterface_Tasks2)) {
				_MESSAGE("Currrent OBSE version doesnt support GameInitialized messages. Use Task method");
				g_Task = (OBSETasksInterface*)obse->QueryInterface(kInterface_Tasks);
				if (!g_Task) {
					_MESSAGE("[ERROR] Cannot load Task Interface. OBSE version too old. Use At Least xOBSE 22.0 (xOBSE 22.9 or above preferred)");
					return false;
				}
				g_Task->EnqueueTask(TaskFunction);
			}
			else {
				g_msg = (OBSEMessagingInterface*)obse->QueryInterface(kInterface_Messaging);
				if (!g_msg) _MESSAGE("[ERROR] Cannot Initialize OBSE Messaging Interface");
				g_msg->RegisterListener(g_pluginHandle, "OBSE", &EventMessageCallback);
			}
		}
		else
		{
			_MESSAGE("Not compatible with editor, exiting.");
			return false;

		}

		return true;
	}

	bool OBSEPlugin_Load(const OBSEInterface * obse)
	{
		_MESSAGE("EditorID Aware Voice Sound: OBSE calling plugin's Load function.");

		WriteRelJump(kHookCreateSoundString, (UInt32)&HookCreateSoundString);
		WriteRelJump(kHookSaveActor, (UInt32) &HookSaveActorContext);
		_MESSAGE("Hook address for Voice path generation function.");
	//	WriteRelCall(0x0052E5BF, (UInt32)&Asseghimma); //This create the path using the last override (mod list) //Keep the hook address 
	//	WriteRelCall(0x0052E612, (UInt32)&Asseghimma); // This with the first masterfile. 
	//	WriteRelJump(0x005F7489, 0x005F74D4);
		ApplyEdidHooks(obse);
		

		return true;
	}

};
