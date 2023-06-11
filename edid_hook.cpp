#include <GameForms.h>
#include <obse_common/SafeWrite.h>
#include <PluginAPI.h>
#include "edid_hook.h"
#include "constants.h"

OBSEMessagingInterface* g_msgIntfc;

REIDInteropData* msg = new REIDInteropData();

const char* __stdcall DispatchREIDMessage(TESForm* form){
	msg->FormID = form->refID;
	g_msgIntfc->Dispatch(g_pluginHandle, 'EDID' , msg , sizeof(REIDInteropData) ,"REID");
	if(msg->HasEditorID) return msg->EditorIDBuffer;
	return "";
}

void __stdcall TESForm_GetEditorIDREID()
{
	__asm
	{
		push	ecx
		call	DispatchREIDMessage
	}
}

std::map<UInt32, const char*>	FormIDReferenceMap;


void __stdcall SetEditorID(TESForm* form, const char* EditorID){
	char* edid = (char*) FormHeap_Allocate(strlen(EditorID) + 1);
	strcpy(edid, EditorID);
//	UInt32* boh = NULL;
//	*boh=form->refID;
	const char* old_edid =  FormIDReferenceMap[form->refID]; 
	FormIDReferenceMap[form->refID] = edid;
	if(old_edid) FormHeap_Free((void*)old_edid);
}

const char* __stdcall GetEditorID(TESForm* form){
	const char* old_edid =  FormIDReferenceMap[form->refID]; 
	if(old_edid) return old_edid;
	return "";
}

void __stdcall TESForm_SetEditorID(const char* EditorID)
{
	__asm
	{
		push	EditorID
		push	ecx
		call SetEditorID
	}
}


void __stdcall TESForm_GetEditorID()
{
	__asm
	{
		push	ecx
		call	GetEditorID
	}
}

void (__cdecl* TESFullName_Load)(TESFullName*, UInt32*) = (void (__cdecl*)(TESFullName*, UInt32*))kTESFullNameLoad;

void __cdecl TESFullNameHook(TESFullName* name, UInt32* unk01){
	TESForm* currentForm;
	__asm {
		mov currentForm, ebx
	}
	TESFullName_Load(name, unk01);
	_MESSAGE("%s %s", name->name.m_data, currentForm->GetEditorName());
}

void ApplyEdidHooks(const OBSEInterface* obse){
	if(obse->GetPluginLoaded("REID")){
		_MESSAGE("Detected REID. Apply Messaging interop");
		g_msgIntfc = (OBSEMessagingInterface*)obse->QueryInterface(kInterface_Messaging);
		if(!g_msgIntfc) _MESSAGE("[ERROR] Cannot get Messaging Interface from OBSE");
		for(UInt32 i = 0; i < VTBLTableSizeREID; i++){
			UInt32 PatchAddressGet = g_VTBLTableREID[i].Address + kTESForm_GetEditorID_VTBLOffset;
			SafeWrite32(PatchAddressGet, (UInt32)TESForm_GetEditorIDREID);
		}
	}
	else if(obse->GetPluginLoaded("MessageLogger")){
		_MESSAGE("Detected MessageLogger. Do nothing");
	}
	else{
		_MESSAGE("No EDID plugin found, install own hooks.");
		UInt32 PatchAddressGet = g_VTBLTableNoREID[0].Address + kTESForm_GetEditorID_VTBLOffset;
		UInt32 PatchAddress = g_VTBLTableNoREID[0].Address + kTESForm_SetEditorID_VTBLOffset;
		SafeWrite32(PatchAddressGet, (UInt32)TESForm_GetEditorID);
		SafeWrite32(PatchAddress,    (UInt32)TESForm_SetEditorID);
	}
	WriteRelCall(kTESRaceFullNameLoad1, (UInt32)&TESFullNameHook);
}


