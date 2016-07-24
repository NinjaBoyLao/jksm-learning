#include <3ds.h>
#include <string>
#include <sf2d.h>
#include <sftd.h>

#include "restore.h"
#include "archive.h"
#include "slot.h"
#include "global.h"
#include "dir.h"
#include "util.h"
#include "ui.h"
#include "sdpath.h"

void copyFiletoArch(FS_Archive arch, const std::u16string from, const std::u16string to, int mode)
{
    Handle sdFile, archFile;
    FSUSER_OpenFile(&sdFile, sdArch, fsMakePath(PATH_UTF16, from.data()), FS_OPEN_READ, 0);
    //OpenFile will fail FS_OPEN_CREATE in ExtData
    if(!modeExtdata(mode))
        FSUSER_OpenFile(&archFile, arch, fsMakePath(PATH_UTF16, to.data()), FS_OPEN_CREATE | FS_OPEN_WRITE, 0);
    else
    {
        Result chk = FSUSER_OpenFile(&archFile, arch, fsMakePath(PATH_UTF16, to.data()), FS_OPEN_WRITE, 0);
        if(chk)
        {
            //try to create it
            u64 size = 0;
            FSFILE_GetSize(sdFile, &size);
            chk = FSUSER_CreateFile(arch, fsMakePath(PATH_UTF16, to.data()), 0, size);
            if(chk)
            {
                showMessage("Error creating extData file!");
            }
            else
            {
                FSUSER_OpenFile(&archFile, arch, fsMakePath(PATH_UTF16, to.data()), FS_OPEN_WRITE, 0);
            }
        }
    }

    u32 read = 0;
    u64 offset = 0;

    u64 size;
    FSFILE_GetSize(sdFile, &size);

    u8 *buff = new u8[buff_size];
    std::string copyString = "Copying " + toString(from) + "...";
    evenString(&copyString);
    progressBar fileProg((float)size, copyString.c_str());
    do
    {
        FSFILE_Read(sdFile, &read, offset, buff, buff_size);
        FSFILE_Write(archFile, NULL, offset, buff, read, FS_WRITE_FLUSH);
        offset += read;

        //I only do this so people don't think it froze on larger files.
        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            fileProg.draw(offset);
        sf2d_end_frame();

        sf2d_swapbuffers();
    }while(offset < size);

    delete[] buff;

    FSFILE_Close(sdFile);
    FSFILE_Close(archFile);
}

void copyDirToArch(FS_Archive arch, const std::u16string from, const std::u16string to, int mode)
{
    dirList list(sdArch, from);
    for(unsigned i = 0; i < list.count(); i++)
    {
        if(list.isDir(i))
        {
            std::u16string newFrom = from;
            newFrom += list.retItem(i);
            newFrom += L'/';

            std::u16string newTo = to;
            newTo += list.retItem(i);
            FSUSER_CreateDirectory(arch, fsMakePath(PATH_UTF16, newTo.data()), 0);
            newTo += L'/';

            copyDirToArch(arch, newFrom, newTo, mode);
        }
        else
        {
            std::u16string sdPath = from;
            sdPath += list.retItem(i);

            std::u16string archPath = to;
            archPath += list.retItem(i);

            copyFiletoArch(arch, sdPath, archPath, mode);
        }
    }
}

bool restoreData(const titleData dat, FS_Archive arch, int mode)
{
    std::u16string sdPath;

    std::string keepName = GetSlot(false, dat, mode);
    if(keepName=="")
        return false;

    std::string ask = "Really restore " + keepName + "?";
    evenString(&ask);

    if(!confirm(ask.c_str()))
        return false;

    sdPath = getPath(mode) + dat.nameSafe + (char16_t)'/' + tou16(keepName.c_str()) + (char16_t)'/';

    std::u16string archPath;
    archPath += L'/';

    if(!modeExtdata(mode))
        FSUSER_DeleteDirectoryRecursively(arch, fsMakePath(PATH_ASCII, "/"));


    copyDirToArch(arch, sdPath, archPath, mode);

    //If we're not restoring some kind of extdata, commit save data
    if(!modeExtdata(mode))
    {
        Result res = FSUSER_ControlArchive(arch, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);
        if(res)
        {
            showMessage("Error committing save data!");
        }
        //If we're running under something from the hbl, end the session, delete the SV and start it again.
        if(hbl)
            fsEnd();

        deleteSV(dat);

        if(hbl)
            fsStart();
    }


    showMessage("Complete!");

    return true;
}

bool restoreDataSDPath(const titleData dat, FS_Archive arch, int mode)
{
    std::u16string sdPath = getSDPath();
    if(sdPath.length() < 2)
        return false;

    std::u16string archPath = (char16_t *)"/";

    if(!modeExtdata(mode))
        FSUSER_DeleteDirectoryRecursively(arch, fsMakePath(PATH_ASCII, "/"));

    copyDirToArch(arch, sdPath, archPath, mode);

    if(!modeExtdata(mode))
        FSUSER_ControlArchive(arch, ARCHIVE_ACTION_COMMIT_SAVE_DATA, NULL, 0, NULL, 0);

    showMessage("Complete!");

    return true;
}
