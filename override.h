#pragma once

void InitializeOverrides(const char* overrideFile);

const char* getOverrideFor(char* RaceToOverride, const char* input, char* output);