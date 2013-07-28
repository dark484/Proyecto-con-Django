/*
 * $Id: exemaker.c 290 2004-10-11 22:46:26Z fredrik $
 *
 * python script loader
 *
 * history:
 * 2002-01-25 fl  created (based on scriptToolbox sample)
 * 2004-10-10 fl  minor tweaks for the effbot.org release
 * 2004-10-11 fl  added #!exe support
 *
 * Copyright (c) 2002-2004 by Fredrik Lundh
 * Copyright (c) 2000-2004 by Secret Labs AB
 */

char copyright[] = " Copyright (c) Secret Labs AB 2000-2004 ";

#if defined(WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include <stdio.h>
#include <stdlib.h>

/* TODO: add pythonw support! */

/* -------------------------------------------------------------------- */
/* The following function is taken from Python's Python/dynload_win.c
   module, where it is used to check what Python a given extension DLL
   is linked against (see that module for more information on how this
   works) */

#define DWORD_AT(mem) (*(DWORD *)(mem))
#define WORD_AT(mem)  (*(WORD *)(mem))

static char *
GetPythonImport(HINSTANCE hModule)
{
    unsigned char *dllbase, *import_data, *import_name;
    DWORD pe_offset, opt_offset;
    WORD opt_magic;
    int num_dict_off, import_off;

    if (!hModule)
        return NULL;

    dllbase = (unsigned char *) hModule;

    pe_offset = DWORD_AT(dllbase + 0x3C);

    if (memcmp(dllbase + pe_offset, "PE\0\0", 4))
        return NULL;

    opt_offset = pe_offset + 4 + 20;
    opt_magic = WORD_AT(dllbase+opt_offset);
    if (opt_magic == 0x10B) {
        /* PE32 */
        num_dict_off = 92;
        import_off   = 104;
    } else if (opt_magic == 0x20B) {
        /* PE32+ */
        num_dict_off = 108;
        import_off   = 120;
    } else {
        /* Unsupported */
        return NULL;
    }

    if (DWORD_AT(dllbase + opt_offset + num_dict_off) >= 2) {
        import_data = dllbase + DWORD_AT(dllbase + opt_offset + import_off);
        while (DWORD_AT(import_data)) {
            import_name = dllbase + DWORD_AT(import_data + 12);
            if (strlen(import_name) >= 6 && !memcmp(import_name,"python",6)) {
                char *pch;
                pch = import_name + 6;
                while (*pch && *pch != '.') {
                    if (*pch >= '0' && *pch <= '9')
                        pch++;
                    else {
                        pch = NULL;
                        break;
                    }
                }
	    
                if (pch)
                    return import_name;
            }
            import_data += 20;
        }
    }

    return NULL;
}

/* -------------------------------------------------------------------- */

int __cdecl
main(int ac, char **av)
{
    HINSTANCE dll;
    HANDLE file;
    DWORD n;
    char* argv[100];
    int (__cdecl * Py_Main)(int argc, char *argv[]);
    void (__cdecl * Py_SetPythonHome)(char* home);
    char dllfile[256];
    char exefile[256];
    char scriptfile[256];
    char message[512];
    DWORD i;

    /* set script name */
    GetModuleFileName(NULL, scriptfile, sizeof(scriptfile));
    for (i = strlen(scriptfile); i > 0 && scriptfile[i] != '.'; i--)
        ;
    scriptfile[i++] = '.';
    scriptfile[i++] = 'p';
    scriptfile[i++] = 'y';
    scriptfile[i] = '\0';

    /* read first line from script */
    file = CreateFile(scriptfile, GENERIC_READ, FILE_SHARE_READ, NULL,
                      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == (HANDLE) -1) {
        strcpy(message, "Cannot open "); strcat(message, scriptfile);
        goto error;
    }
    ReadFile(file, dllfile, sizeof(dllfile)-1, &n, NULL);
    CloseHandle(file);

    for (i = 0; i < n; i++) {
        if (dllfile[i] == '/')
            dllfile[i] = '\\';
        else if (dllfile[i] == '\r' || dllfile[i] == '\n')
            break;
    }
    dllfile[i] = '\0';

    /* look for #! header; if not present, assume #!python.exe */
    if (dllfile[0] != '#' || dllfile[1] != '!') {
        strcpy(dllfile, "#!python.exe");
        i = strlen(dllfile);
    }

    /* search the exe directory first */
    dll = LoadLibraryEx(dllfile+2, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!dll) {
        strcpy(message, "Cannot find "); strcat(message, dllfile+2);
        goto error;
    }

    if (!memcmp(dllfile+i-4, ".exe", 4)) {
        /* it's an EXE file; look for a Python DLL */
        char* realdllfile = GetPythonImport(dll);
        if (!realdllfile) {
            strcpy(message, "Cannot find DLL in "); strcat(message, dllfile+2);
            goto error;
        }
        /* try loading the DLL file from the EXE file's directory */
        for (i = strlen(dllfile); i > 0 && dllfile[i] != '\\'; i--)
            ;
        dllfile[i++] = '\\';
        strcpy(dllfile + i, realdllfile);
        dll = LoadLibraryEx(
            dllfile+2, NULL, LOAD_WITH_ALTERED_SEARCH_PATH
            );
        if (!dll)
            /* if that doesn't work, search for the DLL */
            dll = LoadLibraryEx(
                realdllfile, NULL, LOAD_WITH_ALTERED_SEARCH_PATH
                );
        if (!dll) {
            strcpy(message, "Cannot find "); strcat(message, realdllfile);
            goto error;
        }
    }

    /* get entry points */
    Py_Main = (void*) GetProcAddress(dll, "Py_Main");
    if (!Py_Main)
        ExitProcess(1);

    /* get paths */
    GetModuleFileName(dll, dllfile, sizeof(dllfile));
    GetModuleFileName(dll, exefile, sizeof(exefile));

    /* check if we have a Python library relative to the DLL home */
    for (i = strlen(dllfile); i > 0 && dllfile[i] != '\\'; i--)
        ;
    strcpy(dllfile + i, "\\lib\\os.py");

    file = CreateFile(dllfile, GENERIC_READ, FILE_SHARE_READ, NULL,
                      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (file != (HANDLE) -1) {
        /* lib/os.py exists; set PYTHONHOME to the DLL directory */
        Py_SetPythonHome = (void*) GetProcAddress(dll, "Py_SetPythonHome");
        if (Py_SetPythonHome) {
            dllfile[i] = '\0';
            Py_SetPythonHome(dllfile); /* SetPythonHome keeps a reference! */
        }
        CloseHandle(file);
    }

    /* zap environment variables (is this necessary?) */
    SetEnvironmentVariable("PYTHONDEBUG", NULL);
    SetEnvironmentVariable("PYTHONHOME", NULL);
    SetEnvironmentVariable("PYTHONOPTIMIZE", NULL);
    SetEnvironmentVariable("PYTHONPATH", NULL);
    SetEnvironmentVariable("PYTHONVERBOSE", NULL);

    argv[0] = exefile;
    argv[1] = "-S"; /* don't import site just yet */
    argv[2] = scriptfile;
    for (i = 1; i < (DWORD) ac; i++)
        argv[2+i] = av[i];
    return Py_Main(2+i, argv);

error:
    MessageBox(NULL, message, "Internal error", MB_ICONERROR | MB_OK);
    return 1;
}

#if 0
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow)
{
    return main(__argc, __argv);
}
#endif
