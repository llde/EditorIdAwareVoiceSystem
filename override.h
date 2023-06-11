#pragma once

void InitializeOverrides();
void InitializeConfigurationOverrides(const char* overrideFile);

const char* getOverrideFor(char* RaceToOverride, const char* input, char* output);

const char* getMappingEditor(char* file);