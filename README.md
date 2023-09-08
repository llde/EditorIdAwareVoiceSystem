# Let People Speak - Voice Management Overhaul System OBSE Plugin v1.0  (aka VoiceManagementOverhaul or Editor ID Aware Voice System)

Description:
Started as a simple fork of Ponyrider0's VoiceFile Redirector, it recently become a completly different plugin.

One of the main efforts on modding Oblivion involve localized versions of the game.
As many of you already know, one of the main pain point of installing mods that either touch the Race definitions inside the ESP or add new dialogues. 
As the Dialgoue voices and lip sync informations are generated using the Race name if you use this kind of mods and they are in a different language then the base game you can have 2 results:
Either the game original dialogues stop working, or the dialogue added by the mod don't have any voice.
This is a problem also becouse dialogues without voices are quite fast, and HELLOs (the interaction between the player and npc outside the proper dialogues, and interaction between npcs too) often don't play. 

Solution to this problem was either hand editing mods (or they dialogue's voices) to better fit the overall installation system, or use Elys's Universal Silent Voices or TES4Gecko Silent file Generator  to at least have a playable game with dialgoues that can be read properly.

Some years ago Ponyrider0 improved a bit the situation with his VoiceFile redirector. In addition to Universal Silent Voice feature, it provided a generic redirect to "Imperial" (name of the Imperial race in English) and direct redirect to appease mods like Diverse Voices, that sever the link between a Race and the Races they are selected to use in it's definitions, basically restoring this link in code.

The grave problem of VoiceFile Redirector however is that it's static. Most features works only with an English game, and can't do much in case of multiple mods in a localized game.

So I decided that it was time to end this problem.


Let People Speak - Voice Management Overhaul System implement 4 different features:

- It allows Dialogues to use of the EditorID of a Race instead of it's name for their voices and lip sync.
- It search for an available file corrisponding to the playing dialogue using all the race names that appeared in the modlist in reverse order
- It search for the voice associated to the dialogue using information of the Race Voice Override records in the definitions in the entire masterlist
- If it can't find a file, it use an empty 8 second silence sound with lip syncs

As an additional bonus it forcefully and inconditionally disable bBackgroundLoadLipFiles option. It's bugged and can cause greetings and hellos to not play, expecially when redirected
 
The functionality of the plugins is entirely dynamic. The informations required are obtained at runtime at the game startup, so the data are fresh and react to Load Order changes, mod additions, updates and removal.

The plugin have a special handling for MessageLogger and REID.
If REID is installed, Voice Management Overhaul System install an internal hook for GetEditorName, compatible with REID using the MEssaging OBSE Interface. This also allow MessageLogger to properly log editor ids for entities shadowed by REID incomplete hooks (REID hook only SetEditorName)
If REID is not installed and MEssageLogger is installed it doesn't apply any hooks. MessageLogger EditorID hooks for Races (TESRace class)  must be active for this to works.
If Nor REID nor MessageLogger are installed then VMOS install it's own hooks only for TESRace.

Requirements: xOBSE 22.0 (preferred version 22.9 - 22.10 or above)

Thanks:
OBSE Teams (ianpatt, behippo, scruggsywuggsy the ferret, and shadeMe) for OBSE
Microsoft for Visual Studio 2019
Alexandre Julliard and the Wine Team for Wine
joão antónio, Mixxa77 and Ърчне (ir4ne/ir4n) for name suggestions (result is a mixup of their suggestions)
