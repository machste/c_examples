#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>

#include "plugins.h"


static Plugin *plugin_new(void *lib_handle, const char *name)
{
    Plugin *plugin = malloc(sizeof(Plugin));
    plugin->lib_handle = lib_handle;
    plugin->name = strdup(name);
    plugin->filter_cb = NULL;
    return plugin;
}

static void plugin_free(Plugin *plugin)
{
    if (plugin->lib_handle != NULL)
        dlclose(plugin->lib_handle);
    if (plugin->name != NULL)
        free(plugin->name);
    free(plugin);
}

Plugins *plugins_new(void)
{
    Plugins *p = malloc(sizeof(Plugins));
    p->plugins = NULL;
    p->count = 0;
    return p;
}

void plugins_free(Plugins *p)
{
    if (p->plugins != NULL) {
        for (int i = 0; i < p->count; i++) {
            plugin_free(p->plugins[i]);
        }
        free(p->plugins);
    }
    free(p);
}

static char *plugin_name_from_filename(const char *filename)
{
    char *name = NULL;
    // Find where the file name starts, and where it ends before
    // the extension starts
    const char* last_slash = strrchr(filename, '/');
    const char* name_start = last_slash ? last_slash + 1 : filename;
    const char* last_dot = strrchr(filename, '.');
    // We only care about file names that have a .so extension
    if (last_dot != NULL && strcmp(last_dot, ".so") == 0) {
        // Cut out the plugin name
        size_t name_len = last_dot - name_start;
        name = malloc(name_len + 1);
        strncpy(name, name_start, name_len);
        name[name_len] = '\0';
    }
    return name;
}

static Plugin *plugin_load(const char *path, const char *name)
{
    // Attempt to open the plugin DSO
    void *lib_handle = dlopen(path, RTLD_NOW);
    if (lib_handle == NULL) {
        fprintf(stderr, "Error loading DSO: %s\n", dlerror());
        return NULL;
    }
    // Attempt to find the init function and then call it
    size_t init_cb_name_size = strlen(name) + 6;
    char *init_cb_name = malloc(init_cb_name_size);
    snprintf(init_cb_name, init_cb_name_size, "init_%s", name);
    PluginInitCb init_cb = dlsym(lib_handle, init_cb_name);
    free(init_cb_name);
    if (init_cb == NULL) {
        fprintf(stderr, "Error loading init function: %s!\n", dlerror());
        dlclose(lib_handle);
        return NULL;
    }
    // Create new plugin object
    Plugin *plugin = plugin_new(lib_handle, name);
    // Call init callback of the plugin
    if (!init_cb(plugin)) {
        fprintf(stderr, "Error: Plugin init callback failed!\n");
        plugin_free(plugin);
        return NULL;
    }
    return plugin;
}

bool plugins_discover(Plugins *p, const char *folder)
{
    DIR* dir = opendir(folder);
    if (dir == NULL) {
        fprintf(stderr, "Unable to open folder '%s'!", folder);
        return false;
    }
    bool ret = true;
    // Look at all DSOs in the plugin directory and attempt to load them.
    struct dirent* direntry;
    while ((direntry = readdir(dir))) {
        char *name = plugin_name_from_filename(direntry->d_name);
        if (name == NULL) {
            continue;
        }
        // Generate path to DSO
        int path_len = snprintf(NULL, 0, "%s/%s", folder, direntry->d_name);
        char *path = malloc(path_len + 1);
        snprintf(path, path_len + 1, "%s/%s", folder, direntry->d_name);
        // Load plugin
        Plugin *plugin = plugin_load(path, name);
        if (plugin != NULL) {
            // Add plugin to plugins
            p->plugins = realloc(p->plugins, sizeof(Plugin *) * (p->count + 1));
            p->plugins[p->count] = plugin;
            p->count++;
        } else {
            fprintf(stderr, "Plugin '%s' failed to load!\n", path);
            ret = false;
        }
        // Cleanup
        free(path);
        free(name);
    }
    closedir(dir);
    return ret;
}

Plugin *plugins_get_index(Plugins *p, size_t index)
{
    if (index < p->count) {
        return p->plugins[index];
    }
    return NULL;
}

Plugin *plugins_get_by_name(Plugins *p, const char *name)
{
    for (int i = 0; i < p->count; i++) {
        if (strcmp(p->plugins[i]->name, name) == 0) {
            return p->plugins[i];
        }
    }
    return NULL;
}
