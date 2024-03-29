#ifndef __[!output PROJECT_NAME]Plugin_h__
#define __[!output PROJECT_NAME]Plugin_h__

#include "Plugin.hpp"
#include "[!output PROJECT_NAME]ID.h"

/**	Creates a new [!output PROJECT_NAME]Plugin.
	@param pluginRef IN unique reference to this plugin.
	@return pointer to new [!output PROJECT_NAME]Plugin.
*/
Plugin* AllocatePlugin(SPPluginRef pluginRef);

/**	Reloads the [!output PROJECT_NAME]Plugin class state when the plugin is 
	reloaded by the application.
	@param plugin IN pointer to plugin being reloaded.
*/
void FixupReload(Plugin* plugin);

/**	Hooks [!output PROJECT_NAME] up as an Illustrator plug-in.
	@ingroup [!output PROJECT_NAME]
*/
class [!output PROJECT_NAME]Plugin : public Plugin
{
public:
	/** Constructor.
		@param pluginRef IN reference to this plugin.
	*/
	[!output PROJECT_NAME]Plugin(SPPluginRef pluginRef);

	/** Destructor.
	*/
	virtual ~[!output PROJECT_NAME]Plugin();
  
  /**	Restores state of [!output PROJECT_NAME]Plugin during reload.
	*/
	FIXUP_VTABLE_EX([!output PROJECT_NAME]Plugin, Plugin);
  
  /**	Initializes the plugin.
		@param message IN message sent by the plugin manager.
		@return kNoErr on success, other ASErr otherwise.
	*/
	ASErr StartupPlugin( SPInterfaceMessage * message );

	/**	Removes the plugin.
		@param message IN message sent by the plugin manager.
		@return kNoErr on success, other ASErr otherwise.
	*/
	ASErr ShutdownPlugin( SPInterfaceMessage * message );
};

#endif
