/**
 * @file
 *
 * @brief Header for csvstorage plugin
 *
 * @copyright BSD License (see doc/COPYING or http://www.libelektra.org)
 *
 */

#ifndef ELEKTRA_PLUGIN_CSVSTORAGE_H
#define ELEKTRA_PLUGIN_CSVSTORAGE_H

#include <kdbplugin.h>


int elektraCsvstorageGet (Plugin * handle, KeySet * ks, Key * parentKey);
int elektraCsvstorageSet (Plugin * handle, KeySet * ks, Key * parentKey);

Plugin * ELEKTRA_PLUGIN_EXPORT (csvstorage);

#endif
