//========================================================================================
//  
//  $File: //ai/ai15/devtech/sdk/public/samplecode/FreeGrid/Source/FreeGridPlugin.h $
//
//  $Revision: #1 $
//
//  Copyright 2010 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#pragma once

#ifndef __FREEGRIDPLUGIN_H__
#define __FREEGRIDPLUGIN_H__

#include "FreeGridSuites.h"
#include "Plugin.hpp"
#include "AIMenuGroups.h"
#include "SDKDef.h"
#include "SDKAboutPluginsHelper.h"
#include "FreeGridID.h"
#include "FreeGridTransformArt.h"

class FreeGridDialogController;

/**	Creates a new FreeGridPlugin.
	@param pluginRef IN unique reference to this plugin.
	@return pointer to new FreeGridPlugin.
*/
Plugin* AllocatePlugin(SPPluginRef pluginRef);

/**	Reloads the FreeGridPlugin class state when the plugin is 
	reloaded by the application.
	@param plugin IN pointer to plugin being reloaded.
*/
void FixupReload(Plugin* plugin);

/**	Provides a plugin which demonstrates adding menu items.
*/
class FreeGridPlugin : public Plugin
{
public:

	FreeGridPlugin(SPPluginRef pluginRef);

	virtual ~FreeGridPlugin(void);
		
	FreeGridDialogController* GetFreeGridDialogController() const { return freeGridDialogController; }

	/**	Restores state of FreeGridPlugin during reload.
	*/
	FIXUP_VTABLE_EX(FreeGridPlugin, Plugin);

protected:
	/** Calls Plugin::Message and handles any errors returned.
		@param caller IN sender of the message.
		@param selector IN nature of the message.
		@param message IN pointer to plugin and call information.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr Message(char* caller, char* selector, void *message);

	/**	Calls Plugin::Startup and initialisation functions, such as 
		AddMenus.
		@param message IN pointer to plugin and call information.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr SetGlobal(Plugin* plugin);

	virtual ASErr StartupPlugin(SPInterfaceMessage* message);

	/**	Sets global data to NULL and calls Plugin::Shutdown.
		@param message IN pointer to plugin and call information.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr ShutdownPlugin(SPInterfaceMessage* message);

	/** Performs plugin tasks that could not be performed until
		the application was started.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr PostStartupPlugin();

	/**	Performs actions required for menu item selected.
		@param message IN pointer to plugin and call information.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr GoMenuItem(AIMenuMessage* message);

	/**	Updates state of menu items.
		@param message IN pointer to plugin and call information.
		@return kNoErr on success, other ASErr otherwise.
	*/
	virtual ASErr UpdateMenuItem(AIMenuMessage* message);

	virtual ASErr ReloadPlugin(SPInterfaceMessage *message);

	virtual ASErr UnloadPlugin(SPInterfaceMessage *message);

	virtual ASErr Notify(AINotifierMessage* message);

private:

	FreeGridDialogController *freeGridDialogController;
	
	/** Menu item handle for the TransformButtons About menu item.
	*/
	AIMenuItemHandle fAboutPluginMenu;

	/**	Menu item handle for toggling the menu item check icon.
	*/
	AIMenuItemHandle fMoveToNewAIMenu;	

	AINotifierHandle			fRegisterEventNotifierHandle;

	/** Used to toggle the Free Grid menu item check icon.
	*/
	ASBoolean fFreeGridChecked;

	/**	Adds the menu items for this plugin to the application UI.
	@param message IN pointer to plugin and call information.
	@return kNoErr on success, other ASErr otherwise.
	*/
	ASErr AddMenus(SPInterfaceMessage* message);

	/** Check if a menu group has already been created.
		@param targetGroupName IN name of the menu group to look for.
		@param groupAlreadyMade OUT true if group already exists, false if does not exist.
		@return kNoErr on success, other ASErr otherwise.
	*/
	AIErr MenuGroupExists(const char* targetGroupName, bool& groupAlreadyMade);

};

#endif


