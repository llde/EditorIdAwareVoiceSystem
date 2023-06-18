#pragma once

void InitializeConfigurationOverrides(const char* overrideFile);

void putRaceOverride(const char* editorID, const char* name);
bool getOverrideFor(const char* RaceToOverride, const char* input, char* output);

void printMap();