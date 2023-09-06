#pragma once
#include <GameForms.h>

void putRaceVoiceOVerride(TESRace* race, TESRace* overrideMale, TESRace* overrideFemale);

void InitializeConfigurationOverrides(const char* overrideFile);

void putRaceOverride(const char* editorID, const char* name);
bool getOverrideFor(const char* RaceToOverride, const char* input, char* output);
std::vector<TESRace*>&& getRaceVoiceOverride(TESRace* race, bool isFemale);
void ApplyTransform(std::function<TESRace* (TESRace*)> transform);
void printMap();