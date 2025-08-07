#include "Common.h"

#define FL_BUFFER ((LPWSTR) 0x66FC60)

inline bool AppendXmlIdsToRdl(UINT ids, RenderDisplayList& rdl)
{
    #define APPEND_XML_IDS_TO_RDL_ADDR 0x57DA40
    typedef bool AppendXmlIdsToRdlFunc(UINT, RenderDisplayList&);
    return ((AppendXmlIdsToRdlFunc*) APPEND_XML_IDS_TO_RDL_ADDR)(ids, rdl);
}

inline void AppendXmlWstrToRdl(LPCWSTR wstr, RenderDisplayList& rdl)
{
    #define APPEND_WSTR_TO_RDL_ADDR 0x57E2C0
    typedef void AppendXmlWstrToRdlFunc(LPCWSTR, UINT strLen, RenderDisplayList&, DWORD flags);
    ((AppendXmlWstrToRdlFunc*) APPEND_WSTR_TO_RDL_ADDR)(wstr, wcslen(wstr), rdl, NULL);
}
