#include "hooks.h"

#include "../Utils/util.h"
#include "../Utils/util_sdk.h"
#include "../settings.h"

#include "../GUI/gui.h"

#include <csignal>

typedef void (* SetKeyCodeStateFn)( IInputInternal*, ButtonCode_t, bool );

void Hooks::SetKeyCodeState(IInputInternal* thisptr, ButtonCode_t code, bool pressed)
{
    //if( bPressed )
    //    cvar->ConsoleColorPrintf( ColorRGBA(150, 150, 255, 255), "Key Pressed: %s\n", inputSystem->ButtonCodeToString( code ) );


	/*
	if (SetKeyCodeState::shouldListen && bPressed)
	{
		SetKeyCodeState::shouldListen = false;
		*SetKeyCodeState::keyOutput = code;
		UI::UpdateWeaponSettings();
	}
	*/

	/*
	if (!SetKeyCodeState::shouldListen)
		Shortcuts::SetKeyCodeState(code, bPressed);
	 */
    if( !pressed )
        return inputInternalVMT->GetOriginalMethod<SetKeyCodeStateFn>(96)(thisptr, code, pressed);


    int max;
    CBaseEntity* entity;
    CDotaPlayer* localPlayer;
    panorama::IUIPanel* panel;
    char bytes[256];
    switch( code ){
        case ButtonCode_t::INSERT:
            UI::ToggleUI();
            break;
        case ButtonCode_t::HOME:
            for( int i = 0; i <= entitySystem->GetHighestEntityIndex(); i++ ){
                entity = entitySystem->GetBaseEntity(i);
                if( entity ){
                    cvar->ConsoleDPrintf("Entity#%d(%p) - Name:(%s) \n", i, (void*)entity, entity->Schema_DynamicBinding()->bindingName );
                }
            }
            break;
        case ButtonCode_t::PGUP:
            //vscriptSystem->RunScript("bazinga", false);
            if( UI::hudRoot ){
                panel = UI::hudRoot->FindChildTraverse("ItemList");
            } else {
                cvar->ConsoleDPrintf("Hudroot NULL!\n");
            }
            if( panel ){
                cvar->ConsoleDPrintf("Found panel: %s\n", panel->GetID());
                panoramaEngine->AccessUIEngine()->RunScript(panel, "$.DispatchEvent( 'DOTAItemBuildPlusOptionSelected', -1, true );", "panorama/layout/hud/dota_hud_item_build.xml", 8, 10, false );
            } else {
                cvar->ConsoleDPrintf("Panel not found\n");
            }
            break;
        case ButtonCode_t::PGDN:
            Util::SpewDataMap( entitySystem->GetBaseEntity( mc_ent_select->GetInt() )->C_DOTAPlayer__GetPredDescMap() );
            std::raise(SIGINT);
            break;
        case ButtonCode_t::END:
            for( int i = 0; i < 1024; i++ ){
                panorama::IUIPanel *panel = panoramaEngine->AccessUIEngine()->GetPanelArray()->slots[i].panel;
                if( panoramaEngine->AccessUIEngine()->IsValidPanelPointer(panel) ){
                    cvar->ConsoleDPrintf("Panel %d: (%s)\n", i, panel->GetID());
                }
            }
            cvar->ConsoleDPrintf("Done Showing panels.\n");
            break;
        case ButtonCode_t::DELETE:
            networkClientService->GetIGameClient()->ForceFullUpdate("unnamed");
            break;
        case ButtonCode_t::PAUSE:
            cvar->ConsoleDPrintf("pause key pressed.\n");
            engine->ServerCmd(mc_cmd_src->GetInt(), mc_raw_command->strValue);
            break;
        case ButtonCode_t::SCROLLLOCK:
            //snprintf(bytes, 256, "say \"I got a whole lotta money\x0D\x0D\nswag nigga\nswag\"");
            //engine->ClientCmd_Unrestricted(bytes);
            Util::DumpLinkMaps();
            break;
        default:
            break;
    }
	inputInternalVMT->GetOriginalMethod<SetKeyCodeStateFn>(96)(thisptr, code, pressed);
}
