#include <inttypes.h>
//Audio Units entrypoint/factory

//https://developer.apple.com/documentation/coreservices/componentresult
typedef int32_t ComponentResult;

extern "C" ComponentResult StaticAUExportEntry(void * params, void* obj);
extern "C" void* StaticAUExportFactory(const void * desc);

#define COMPONENT_ENTRYX(Name, Suffix) \
    extern "C" __attribute__((visibility("default"))) ComponentResult Name ## Suffix (void* params, void* obj); \
    extern "C" __attribute__((visibility("default"))) ComponentResult Name ## Suffix (void* params, void* obj) \
    { \
        return StaticAUExportEntry(params, obj); \
    }

#define FACTORY_ENTRYX(Name) \
    extern "C" __attribute__((visibility("default"))) void* Name ## Factory (const void* desc); \
    extern "C" __attribute__((visibility("default"))) void* Name ## Factory (const void* desc) \
    { \
        return StaticAUExportFactory(desc); \
    }

#define COMPONENT_ENTRY(Name, Suffix)   COMPONENT_ENTRYX(Name, Suffix)
#define FACTORY_ENTRY(Name)             FACTORY_ENTRYX(Name)

COMPONENT_ENTRY (RNBO_Plugin_AU, Entry)
FACTORY_ENTRY (RNBO_Plugin_AU)
