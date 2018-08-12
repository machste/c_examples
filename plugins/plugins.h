#ifndef _PLUGINS_H_
#define _PLUGINS_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct Plugin_t Plugin;

typedef bool (*PluginInitCb)(Plugin *plugin);

typedef int (*PluginFilterCb)(const char *src, char *dest, size_t dest_size);

struct Plugin_t {
    void *lib_handle;
    char *name;
    PluginFilterCb filter_cb;
};

typedef struct Plugins_t {
    Plugin **plugins;
    size_t count;
} Plugins;


Plugins *plugins_new(void);
void plugins_free(Plugins *p);

bool plugins_discover(Plugins *p, const char *folder);

Plugin *plugins_get_index(Plugins *p, size_t index);
Plugin *plugins_get_by_name(Plugins *p, const char *name);

#endif /* _PLUGINS_H_ */
