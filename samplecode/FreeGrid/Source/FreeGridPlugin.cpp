//========================================================================================
//  
//  $File: //ai/ai15/devtech/sdk/public/samplecode/FreeGrid/Source/FreeGridPlugin.cpp $
//
//  $Revision: #2 $
//
//  Copyright 2010 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "IllustratorSDK.h"
#include "AICSXS.h"
#include "FreeGridPlugin.h"
#include "FreeGridDialogController.h"

FreeGridPlugin *gPlugin = NULL;

/*
*/
Plugin* AllocatePlugin(SPPluginRef pluginRef)
{
	return new FreeGridPlugin(pluginRef);
}

/*
*/
void FixupReload(Plugin* plugin)
{
	FreeGridPlugin::FixupVTable((FreeGridPlugin*) plugin);
}

FreeGridPlugin::FreeGridPlugin(SPPluginRef pluginRef) :
	Plugin(pluginRef),
	fAboutPluginMenu(NULL),
	fMoveToNewAIMenu(NULL),
	fRegisterEventNotifierHandle(NULL),
	fFreeGridChecked(false),
	freeGridDialogController(NULL)
{
	strncpy(fPluginName, kFreeGridPluginName, kMaxStringLength);
}

FreeGridPlugin::~FreeGridPlugin(void)
{
}

/*
*/
ASErr FreeGridPlugin::Message(char* caller, char* selector, void *message) 
{
	ASErr error = kNoErr;

	try {
		error = Plugin::Message(caller, selector, message);
	}
	catch (ai::Error& ex) {
		error = ex;
	}
	catch (...) {
		error = kCantHappenErr;
	}
	if (error) {
		if (error == kUnhandledMsgErr) {
			// Defined by Plugin.hpp and used in Plugin::Message - ignore.
			error = kNoErr;
		}
		else {
			Plugin::ReportError(error, caller, selector, message);
		}
	}	
	return error;
}

ASErr FreeGridPlugin::SetGlobal(Plugin *plugin)
{
	gPlugin = (FreeGridPlugin *) plugin;
	return kNoErr;
}

/*
*/
ASErr FreeGridPlugin::StartupPlugin(SPInterfaceMessage* message) 
{
	ASErr error = Plugin::StartupPlugin(message);
	if (error) { return error; }

	error = this->AddMenus(message);
	if (error) { return error; }

	if (NULL == freeGridDialogController)
	{
		freeGridDialogController = new FreeGridDialogController();

		error = Plugin::LockPlugin(true);
		if (error) { return error; }
	}

	error = sAINotifier->AddNotifier( fPluginRef, "Register Event Notify",
		kAICSXSPlugPlugSetupCompleteNotifier, &fRegisterEventNotifierHandle);
	if (error) { return error; }
	return error;
}

/*
*/
ASErr FreeGridPlugin::ShutdownPlugin(SPInterfaceMessage* message)
{	
	if (freeGridDialogController)
	{
		freeGridDialogController->RemoveEventListeners();
		delete freeGridDialogController;
		freeGridDialogController = NULL;
		Plugin::LockPlugin(false);
	}
	message->d.globals = NULL;	
	return Plugin::ShutdownPlugin(message);
}


ASErr FreeGridPlugin::ReloadPlugin(SPInterfaceMessage *message)
{
	return Plugin::ReloadPlugin(message);
}

ASErr FreeGridPlugin::UnloadPlugin(SPInterfaceMessage *message)
{
	return Plugin::UnloadPlugin(message);
}

/*
*/
ASErr FreeGridPlugin::PostStartupPlugin()
{
	return kNoErr;
}

/*
*/
ASErr FreeGridPlugin::AddMenus(SPInterfaceMessage* message) 
{
	ASErr resust = kNoErr;

	// Add a menu item to the About SDK Plug-ins menu group.
	SDKAboutPluginsHelper aboutPluginsHelper;
	aboutPluginsHelper.AddAboutPluginsMenuItem(message, 
				kSDKDefAboutSDKCompanyPluginsGroupName, 
				ai::UnicodeString(kSDKDefAboutSDKCompanyPluginsGroupNameString), 
				"FreeGrid...", 
				&fAboutPluginMenu);

	// Add an SDK menu group to the Windows menu.
	const char* kSDKWindowsMenuGroup = "SDKWindowGroup";
	bool exists = false;
	resust = this->MenuGroupExists(kSDKWindowsMenuGroup, exists);
	if (!exists) {
		AIPlatformAddMenuItemDataUS menuItemData;
		menuItemData.groupName = kOtherPalettesMenuGroup;
		menuItemData.itemText = ai::UnicodeString("SDK");	
		AIMenuItemHandle menuItemHandle = nil;
		resust = sAIMenu->AddMenuItem(message->d.self, NULL, &menuItemData, kMenuItemNoOptions, &menuItemHandle);
		AIMenuGroup menuGroup = nil;
		resust = sAIMenu->AddMenuGroupAsSubMenu(kSDKWindowsMenuGroup, kMenuGroupSortedAlphabeticallyOption, menuItemHandle, &menuGroup);
	}
		// Add menu item for this plug-in under the company's about plug-ins menu group.
	AIPlatformAddMenuItemDataUS showHidePanelMenuData;
	showHidePanelMenuData.groupName = kSDKWindowsMenuGroup;
	showHidePanelMenuData.itemText = ai::UnicodeString("Free Grid");
	resust = sAIMenu->AddMenuItem(message->d.self, NULL, &showHidePanelMenuData, kMenuItemWantsUpdateOption, &fMoveToNewAIMenu);
	
	if ( resust )
		goto error;
	this->fFreeGridChecked = false;	
error:
	return resust;
}

/*
*/
AIErr FreeGridPlugin::MenuGroupExists(const char* targetGroupName, bool& groupAlreadyMade)
{
	AIErr error = kNoErr;
	groupAlreadyMade = false;
	ai::int32 count = 0;
	AIMenuGroup dummyGroup = nil;
	error = sAIMenu->CountMenuGroups( &count );
	if ( error ) return error;
	for (ai::int32 i = 0; i < count; i++) {
		error = sAIMenu->GetNthMenuGroup( i, &dummyGroup );

		const char* name;
		error = sAIMenu->GetMenuGroupName( dummyGroup, &name );

		if ( std::strcmp(name, targetGroupName ) == 0 )
		{
			groupAlreadyMade = true;
			break;
		}
	}
	return error;
}

/*
*/
ASErr FreeGridPlugin::GoMenuItem(AIMenuMessage* message) 
{
	ASErr result = kNoErr;

	// Compare the menuItem selected (in the message) with our stored values to see if we do anything
	if (message->menuItem == this->fAboutPluginMenu)
	{
		// Pop this plug-in's about box.
		SDKAboutPluginsHelper aboutPluginsHelper;
		aboutPluginsHelper.PopAboutBox(message, "About FreeGrid", kSDKDefAboutSDKCompanyPluginsAlertString);
	}	
	else if (message->menuItem == this->fMoveToNewAIMenu) 
	{
		if (freeGridDialogController)
		{
			ASBoolean visible = freeGridDialogController->GetPanelReady();	
			if(visible)
			{
				result = freeGridDialogController->UnloadExtension();
				freeGridDialogController->SetPanelReady(false);
			}
			else
			{
				result = freeGridDialogController->LoadExtension();
			}
		}
	}
	return result;
}

ASErr FreeGridPlugin::UpdateMenuItem(AIMenuMessage* message) 
{
	ASBoolean visible = false;
	if (freeGridDialogController)
	{
		visible = freeGridDialogController->GetPanelReady();
	}
	sAIMenu->CheckItem(fMoveToNewAIMenu, visible);
	return kNoErr;

}

ASErr FreeGridPlugin::Notify(AINotifierMessage *message )
{
	if ( message->notifier == fRegisterEventNotifierHandle)
	{
		freeGridDialogController->RegisterCSXSEventListeners();
	}
	return kNoErr;
}
