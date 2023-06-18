#include "utils.h"
#include "finder.h"
#include "override.h"
#include "path.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

static std::unordered_map<std::string, std::vector<std::string>>  otheroverrides;

static std::unordered_map<std::string, std::vector<std::string>>  raceOverrides;

void putRaceOverride(const char* editorID, const char* name){
	std::string namestr = name;
	std::string edid = editorID;
	std::string namestr_lower = "";
	std::string edid_low = "";
	std::transform(namestr.begin(), namestr.end(), std::back_inserter(namestr_lower), ::tolower);
	std::transform(edid.begin(), edid.end(), std::back_inserter(edid_low), ::tolower);
	auto& iter = raceOverrides.find(edid_low);
	if(edid_low != namestr_lower && iter != raceOverrides.end()){
		if (std::find(iter->second.begin(), iter->second.end(), namestr_lower) == iter->second.end())
			iter->second.push_back(namestr_lower);
	}
	else{
		auto raceOverrideVector = std::vector<std::string>();
		if(edid_low != namestr_lower) raceOverrideVector.push_back(namestr_lower);
		raceOverrides[edid_low] = raceOverrideVector;
	}
}

void InitializeConfigurationOverrides(const char* overrideFile){
	
}

void printMap(){
	std::string print = "";
	for(auto& iter = raceOverrides.begin(); iter != raceOverrides.end(); iter++){
		print += iter->first + ":";
		for(auto& iter1 = iter->second.begin(); iter1 != iter->second.end(); iter1++){
			print += *iter1 + ", ";
		}
		print += "\n";
	}
	_MESSAGE("%s", print);
}

//Will need to test for overhead.
bool getOverrideFor(const char* RaceToOverride, const char* input, char* output){
	if (RaceToOverride == nullptr) return nullptr;
	std::string race = RaceToOverride;
	std::string race_low;
	std::transform(race.begin(), race.end(), std::back_inserter(race_low), ::tolower);
	auto& vec_ref = raceOverrides.find(race_low);
	if (vec_ref == raceOverrides.end()) return false;
	auto& val = vec_ref->second;
	for each (std::string over in val){
		replacePathComponent(Component::Race, input, over.c_str(), output);
		if (FileExists(output)) return true;
	}
	return false;
}