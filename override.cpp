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


/*
For every race there is a pair (one for sex) of TESRace linking the override for the voices
nullptr means it use the key race (so it doesn't enable an override)
*/
static std::unordered_map<TESRace*, std::pair<std::vector<TESRace*>,std::vector<TESRace*>>> useVoiceOfRaceOverride;

void putRaceOverride(const char* editorID, const char* name){
	std::string namestr = name;
	std::string edid = editorID;
	std::string namestr_lower = "";
	std::string edid_low = "";
	std::transform(namestr.begin(), namestr.end(), std::back_inserter(namestr_lower), ::tolower);
	std::transform(edid.begin(), edid.end(), std::back_inserter(edid_low), ::tolower);
	auto iter = raceOverrides.find(edid_low);
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

void putRaceVoiceOVerride(TESRace* race, TESRace* overrideMale, TESRace* overrideFemale) {
	auto iter = useVoiceOfRaceOverride.find(race);
	if (iter == useVoiceOfRaceOverride.end()) {
		auto maleOverride = std::vector<TESRace*>();
		auto femaleOverride = std::vector<TESRace*>();
		maleOverride.push_back(overrideMale);
		femaleOverride.push_back(overrideFemale);
		auto pair = std::make_pair(maleOverride, femaleOverride);
		useVoiceOfRaceOverride[race] = pair;
	}
	else {
		auto& pair = iter->second;
		auto& maleOverride = pair.first;
		auto& femaleOverride = pair.second;
		if (std::find(maleOverride.cbegin(), maleOverride.cend(), overrideMale) == maleOverride.cend()) {
			maleOverride.push_back(overrideMale);
		}
		if (std::find(femaleOverride.cbegin(), femaleOverride.cend(), overrideFemale) == femaleOverride.cend()) {
			femaleOverride.push_back(overrideFemale);
		}
	}
}

std::vector<TESRace*>&& getRaceVoiceOverride(TESRace* race, bool isFemale) {
	auto raceOver = useVoiceOfRaceOverride.find(race);
	if (raceOver == useVoiceOfRaceOverride.end()) return std::vector<TESRace*>();
	return isFemale ? std::forward<std::vector<TESRace*>>(raceOver->second.second) : std::forward<std::vector<TESRace*>>(raceOver->second.first);
}

void InitializeConfigurationOverrides(const char* overrideFile){
	
}

void printMap(){
	std::string print = "";
	for(auto iter = raceOverrides.begin(); iter != raceOverrides.end(); iter++){
		print += iter->first + ":";
		for(auto&& iter1 = iter->second.begin(); iter1 != iter->second.end(); iter1++){
			print += *iter1 + ", ";
		}
		print += "\n";
	}
	_MESSAGE("Race name overrides:\n%s", print);
	print.clear();
	for (auto iter = useVoiceOfRaceOverride.begin(); iter != useVoiceOfRaceOverride.end(); iter++) {
		print += iter->first->GetEditorName();
		print += ":";
		std::string print_inner = "";
		for (auto&& iter1 = iter->second.first.begin(); iter1 != iter->second.first.end(); iter1++) {
			print_inner += ", ";
			print_inner += (*iter1) ? (*iter1)->GetEditorName() : "<THIS>";
		}
		print += &print_inner[2];
		print += " ||| ";
		print_inner.clear();
		for (auto&& iter1 = iter->second.second.begin(); iter1 != iter->second.second.end(); iter1++) {
			print_inner += ", ";
			print_inner += (*iter1) ? (*iter1)->GetEditorName() : "<THIS>";
		}
		print += &print_inner[2];
		print += "\n";
	}
	_MESSAGE("Race overrides:\n%s\n", print);
}

//Will need to test for overhead.
bool getOverrideFor(const char* RaceToOverride, const char* input, char* output){
	if (RaceToOverride == nullptr) return false;
	std::string race = RaceToOverride;
	std::string race_low;
	std::transform(race.begin(), race.end(), std::back_inserter(race_low), ::tolower);
	auto vec_ref = raceOverrides.find(race_low);
	if (vec_ref == raceOverrides.end()) return false;
	auto& val = vec_ref->second;
	for (std::string& over : val){
		replacePathComponent(Component::Race, input, over.c_str(), output);
		if (FileExists(output)) return true;
	}
	return false;
}

void ApplyTransform(std::function<TESRace*(TESRace*)> transform) {
	for (auto& ref : useVoiceOfRaceOverride) {
		auto& pairOver = ref.second;
		std::transform(pairOver.first.cbegin(), pairOver.first.cend(), pairOver.first.begin(), transform);
		std::transform(pairOver.second.cbegin(), pairOver.second.cend(), pairOver.second.begin(), transform);
	}
}