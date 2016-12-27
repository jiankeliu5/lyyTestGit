#include "load_tddl.h"
//#ifdef WIN32


#define GETPROCADDRESS(f, n)            \
{                                       \
    void ** p = (void **)&f;            \
    *p = GetProcAddress(m->hModule, n); \
    if(NULL == *p)                      \
    {                                   \
        m->free(m);                     \
        return TCSERR(TSM_E_FAIL);      \
    }                                   \
}                                       \


//#endif

long free_tddl(TDDL_MODULE* m)
{
    if((void *)0 == m)
    {
        return TCSERR(TSM_E_BAD_PARAMETER);
    }
    if((void *)0 != m->hModule)
    {
        FreeLibrary(m->hModule);
    }

    m->Tddli_Open = 0;
    m->Tddli_Close = 0;
    m->Tddli_Cancel = 0;
    m->Tddli_GetCapability = 0;
    m->Tddli_SetCapability = 0;
    m->Tddli_GetStatus = 0;
    m->Tddli_TransmitData = 0;

    return TSM_SUCCESS;
}

long load_tddl(TDDL_MODULE* m, const char* name)
{
    int len = 0;
    if((NULL == m) || (0 == name))
    {
        return TCSERR(TSM_E_BAD_PARAMETER);
    }
    len = strlen(name);
    if((0 == len) || (255 < len))
    {
        return TCSERR(TSM_E_BAD_PARAMETER);
    }

    m->load = load_tddl;
    m->free = free_tddl;

    memset(m->name, 0, sizeof(m->name));
    memcpy(m->name, name, len);
//#ifdef WIN32

    m->hModule = LoadLibraryA(name);
    if((void *)0 == m->hModule)
    {
        return TCSERR(TSM_E_FAIL);
    }

    GETPROCADDRESS(m->Tddli_Open, "Tddli_Open");
    GETPROCADDRESS(m->Tddli_Close, "Tddli_Close");
    GETPROCADDRESS(m->Tddli_Cancel, "Tddli_Cancel");
    GETPROCADDRESS(m->Tddli_GetCapability, "Tddli_GetCapability");
    GETPROCADDRESS(m->Tddli_SetCapability, "Tddli_SetCapability");
    GETPROCADDRESS(m->Tddli_GetStatus, "Tddli_GetStatus");
    GETPROCADDRESS(m->Tddli_TransmitData, "Tddli_TransmitData");

    return TSM_SUCCESS;
}

TDDL_MODULE gTddlModule = {
    (void *)0, load_tddl, free_tddl,
};

