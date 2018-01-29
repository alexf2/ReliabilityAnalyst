#if !defined(_GETGDIMGR_)
#define _GETGDIMGR_

#include "stdafx.h"

class TDrToolsManeger;

TDrToolsManeger* GDIMgr();

void GL_SetMsgStr( LPCSTR );
void GL_RestoreMsg();

#endif
