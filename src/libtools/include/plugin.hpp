/**
 * \file
 *
 * \brief Header file of plugin
 *
 * \copyright BSD License (see doc/COPYING or http://www.libelektra.org)
 *
 */

#ifndef TOOLS_PLUGIN_HPP
#define TOOLS_PLUGIN_HPP

#include <kdb.hpp>
#include <toolexception.hpp>

#include <map>
#include <vector>
#include <string>

namespace ckdb
{
	typedef struct _Plugin Plugin;
}

namespace kdb
{

/**
 * This is a C++ representation of a plugin.
 *
 * It will load an Elektra plugin using the module loader
 * from Elektra.
 *
 * Then you can either check the plugins configuration
 * using loadInfo(), parse() and check.
 * Symbols can then be retrieved with getSymbol().
 *
 * Or you can use the normal open(), close(), get(),
 * set() and error() API which every plugin exports.
 */
class Plugin
{
private:
	typedef void (*func_t)();

private:
	ckdb::Plugin *plugin;
	std::string pluginName;
	kdb::KeySet info;

	std::map<std::string, func_t> symbols;
	std::map<std::string, std::string> infos;

	bool firstRef;

	void uninit();

public:
	Plugin(std::string const& pluginName, kdb::KeySet &modules, kdb::KeySet const& testConfig);

	Plugin(Plugin const& other);
	Plugin& operator = (Plugin const& other);
	~Plugin();

	/**
	 * Gets the configuration for the plugin.
	 */
	void loadInfo();

	/**
	 * Creates symbol and info table.
	 */
	void parse();

	/**
	 * Does various checks on the Plugin and throws exceptions
	 * if something is not ok.
	 *
	 * - Check if Plugin is compatible to current Version of Backend-API.
	 *
	 * @throw PluginCheckException if there are errors
	 * @param warnings for warnings
	 *
	 * @pre parse()
	 */
	void check(std::vector<std::string> & warnings);

	ckdb::Plugin *operator->();

	/**
	 * Gets the whole string of an information item.
	 * @pre loadInfo()
	 */
	std::string lookupInfo(std::string item, std::string section = "infos");

	/**
	 * Searches within a string of an information item.
	 * @pre loadInfo()
	 */
	bool findInfo(std::string check, std::string item, std::string section = "infos");

	/**
	 * Returns the whole keyset of information.
	 * @pre loadInfo()
	 */
	kdb::KeySet getInfo() {return info;}

	/**
	 * In the plugin's contract there is a description of which
	 * config is needed in order to work together with a backend
	 * properly.
	 *
	 * @return the keyset with the config needed for the backend.
	 * @pre loadInfo()
	 */
	kdb::KeySet getNeededConfig();

	/**
	 * Returns symbol to a function.
	 * @pre parse()
	 */
	func_t getSymbol (std::string which)
	{
		if (symbols.find (which) == symbols.end()) throw MissingSymbol(which);
		return symbols[which];
	}

	/**
	 * Calls the open function of the plugin
	 * @pre parse()
	 */
	int open (kdb::Key & errorKey);

	/**
	 * Calls the close function of the plugin
	 * @pre parse()
	 */
	int close (kdb::Key & errorKey);

	/**
	 * Calls the get function of the plugin
	 * @pre parse()
	 */
	int get (kdb::KeySet & ks, kdb::Key & parentKey);

	/**
	 * Calls the set function of the plugin
	 * @pre parse()
	 */
	int set (kdb::KeySet & ks, kdb::Key & parentKey);

	/**
	 * Calls the error function of the plugin
	 * @pre parse()
	 */
	int error (kdb::KeySet & ks, kdb::Key & parentKey);

	/**
	 * @return the name of the plugin 
	 */
	std::string name();

	/**
	 * @return the name how it would be referred to in mountpoint
	 */
	std::string refname();
};

#if __cplusplus > 199711L
typedef std::unique_ptr<Plugin> PluginPtr;
#else
typedef std::auto_ptr<Plugin> PluginPtr;
#endif
}

#endif
