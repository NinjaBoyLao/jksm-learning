#include <3ds.h>

#include "titledata.h"
#include "archive.h"
#include "ui.h"
#include "global.h"

//All the information used here was found on 3dbrew.org
//thank them too.
bool openSaveArch(FS_Archive *out, const titleData dat, bool showError)
{
    //binary path
    u32 path[3] = {dat.media, dat.low, dat.high};

    FS_Path binPath = {PATH_BINARY, 12, path};

    Result res = FSUSER_OpenArchive(out, ARCHIVE_USER_SAVEDATA, binPath);
    if(res)
    {
        if(showError)
            showMessage("Error opening save archive!");
        return false;
    }

    return true;
}

bool openExtdata(FS_Archive *out, const titleData dat, bool showError)
{
    u32 path[] = {MEDIATYPE_SD, dat.extdata, 0};

    FS_Path binPath = {PATH_BINARY, 12, path};

    Result res = FSUSER_OpenArchive(out, ARCHIVE_EXTDATA, binPath);
    if(res)
    {
        if(showError)
            showMessage("Error opening ExtData! Title may not use it.");
        return false;
    }

    return true;
}

bool openSharedExt(FS_Archive *out, u32 id)
{
    u32 path[3] = {MEDIATYPE_NAND, id, 0x00048000};

    FS_Path binPath = {PATH_BINARY, 12, path};

    Result res = FSUSER_OpenArchive(out, ARCHIVE_SHARED_EXTDATA, binPath);
    if(res)
    {
        showMessage("Error opening Shared Extdata!");
        return false;
    }

    return true;
}

bool openBossExt(FS_Archive *out, const titleData dat)
{
    u32 path[3] = {MEDIATYPE_SD, dat.extdata, 0};

    FS_Path binPath = {PATH_BINARY, 12, path};

    Result res = FSUSER_OpenArchive(out, ARCHIVE_BOSS_EXTDATA, binPath);
    if(res)
    {
        showMessage("Error opening Boss Extdata! Title may not use it.");
        return false;
    }

    return true;
}

bool openSysModule(FS_Archive *out, const titleData dat)
{
    u32 path[2] = {MEDIATYPE_NAND, (0x00010000 | dat.unique)};

    FS_Path binPath = {PATH_BINARY, 8, path};

    Result res = FSUSER_OpenArchive(out, ARCHIVE_SYSTEM_SAVEDATA, binPath);
    if(res)
    {
        return false;
    }

    return true;
}

bool openSysSave(FS_Archive *out, const titleData dat)
{
    u32 path[2] = {MEDIATYPE_NAND, (0x00020000 | dat.unique)};

    FS_Path binPath = {PATH_BINARY, 8, path};

    Result res = FSUSER_OpenArchive(out, ARCHIVE_SYSTEM_SAVEDATA, binPath);
    if(res)
    {
        //try opening as module instead
        if(openSysModule(out, dat))
            return true;
        else
        {
            showMessage("Error opening system save data. Title may not use it!");
            return false;
        }
    }

    return true;
}
