/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2011 Myth Project <http://bitbucket.org/sun/myth-core/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You can't share Myth Project's sources! Only for personal use.
 */

#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <ace/Version.h>

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Configuration/Config.h"

#include "Log.h"
#include "Master.h"

#ifndef _TRINITY_CORE_CONFIG
# define _TRINITY_CORE_CONFIG  "worldserver.conf"
#endif //_TRINITY_CORE_CONFIG

#ifdef _WIN32
#include "ServiceWin32.h"
char serviceName[] = "Worldserver";
char serviceLongName[] = "MythCore 'World' service";
char serviceDescription[] = "MythCore World of Warcraft 'World' Service Emulator";
/*
 * -1 - not in service mode
 *  0 - stopped
 *  1 - running
 *  2 - paused
 */
int m_ServiceStatus = -1;
#endif

WorldDatabaseWorkerPool WorldDatabase;                      ///< Accessor to the world database
CharacterDatabaseWorkerPool CharacterDatabase;              ///< Accessor to the character database
LoginDatabaseWorkerPool LoginDatabase;                      ///< Accessor to the realm/login database

uint32 realmID;                                             ///< Id of the realm

/// Print out the usage string for this program on the console.
void usage(const char *prog)
{
    sLog->outString("Usage: \n %s [<options>]\n"
        "    -c config_file           use config_file as configuration file\n\r"
        #ifdef _WIN32
        "    Running as service functions:\n\r"
        "    --service                run as service\n\r"
        "    -s install               install service\n\r"
        "    -s uninstall             uninstall service\n\r"
        #endif
        ,prog);
}

/// Launch the Trinity server
extern int main(int argc, char **argv)
{
    ///- Command line parsing to get the configuration file name
    char const* cfg_file = _TRINITY_CORE_CONFIG;
    int c = 1;
    while( c < argc )
    {
        if (strcmp(argv[c], "-c") == 0)
        {
            if (++c >= argc)
            {
                sLog->outError("Runtime-Error: -c option requires an input argument");
                usage(argv[0]);
                return 1;
            }
            else
                cfg_file = argv[c];
        }

        #ifdef _WIN32
        ////////////
        //Services//
        ////////////
        if (strcmp(argv[c], "-s") == 0)
        {
            if (++c >= argc)
            {
                sLog->outError("Runtime-Error: -s option requires an input argument");
                usage(argv[0]);
                return 1;
            }
            if (strcmp(argv[c], "install") == 0)
            {
                if (WinServiceInstall())
                    sLog->outString("Installing service");
                return 1;
            }
            else if (strcmp(argv[c], "uninstall") == 0)
            {
                if (WinServiceUninstall())
                    sLog->outString("Uninstalling service");
                return 1;
            }
            else
            {
                sLog->outError("Runtime-Error: unsupported option %s", argv[c]);
                usage(argv[0]);
                return 1;
            }
        }
        if (strcmp(argv[c], "--service") == 0)
        {
            WinServiceRun();
        }
        ////
        #endif
        ++c;
    }

    if (!sConfig->SetSource(cfg_file))
    {
        sLog->outError("Invalid or missing configuration file : %s", cfg_file);
        sLog->outError("Verify that the file exists and has \'[worldserver]' written in the top of the file!");
        return 1;
    }
    sLog->outString("Using configuration file %s.", cfg_file);

    sLog->outDetail("%s (Library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));
    sLog->outDetail("Using ACE: %s", ACE_VERSION);

    ///- and run the 'Master'
    /// \todo Why do we need this 'Master'? Can't all of this be in the Main as for Realmd?
    int ret = sMaster->Run();

    // at sMaster return function exist with codes
    // 0 - normal shutdown
    // 1 - shutdown at error
    // 2 - restart command used, this code can be used by restarter for restart Trinityd

    return ret;
}