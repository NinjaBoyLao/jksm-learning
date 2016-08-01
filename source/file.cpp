#include <3ds.h>
#include <string>
#include <stdio.h>
#include <string.h>

#include "file.h"
#include "util.h"
#include "ui.h"

fsFile::fsFile(FS_Archive _arch, std::u16string _path, u32 openFlags)
{
    arch = _arch;
    Result res = FSUSER_OpenFile(&fileHandle, arch, fsMakePath(PATH_UTF16, _path.data()), openFlags, 0);
    if(res)
    {
        opened = false;
    }
    else
    {
        FSFILE_GetSize(fileHandle, &fileSize);
        opened = true;
    }
}

fsFile::fsFile(FS_Archive _arch, std::u16string _path, u32 openFlags, u64 createSize)
{
    arch = _arch;
    Result res = FSUSER_OpenFile(&fileHandle, arch, fsMakePath(PATH_UTF16, _path.data()), openFlags, 0);
    if(res)
    {
        res = FSUSER_CreateFile(arch, fsMakePath(PATH_UTF16, _path.data()), 0, createSize);
        if(res)
        {
            opened = false;
            return;
        }
        res = FSUSER_OpenFile(&fileHandle, arch, fsMakePath(PATH_UTF16, _path.data()), openFlags, 0);
        if(res)
            opened = false;
        else
            opened = true;
    }
}

fsFile::fsFile(FS_Archive _arch, const char *_path, u32 openFlags)
{
    arch = _arch;
    Result res = FSUSER_OpenFile(&fileHandle, arch, fsMakePath(PATH_ASCII, _path), openFlags, 0);
    if(res)
        opened = false;
    else
    {
        FSFILE_GetSize(fileHandle, &fileSize);
        opened = true;
    }
}


fsFile::fsFile(FS_Archive _arch, const char *_path, u32 openFlags, u64 createSize)
{
    arch = _arch;
    Result res = FSUSER_OpenFile(&fileHandle, arch, fsMakePath(PATH_ASCII, _path), openFlags, 0);
    if(res)
    {
        res = FSUSER_CreateFile(arch, fsMakePath(PATH_ASCII, _path), 0, createSize);
        if(res)
        {
            opened = false;
            return;
        }
        res = FSUSER_OpenFile(&fileHandle, arch, fsMakePath(PATH_ASCII, _path), openFlags, 0);
        if(res)
            opened = false;
        else
            opened = true;
    }
}

fsFile::fsFile(FS_ArchiveID _arch, FS_Path archPath, FS_Path filePath, u32 openFlags)
{
    Result res = FSUSER_OpenFileDirectly(&fileHandle, _arch, archPath, filePath, openFlags, 0);
    if(res == 0)
        opened = true;
    else
        opened = false;
}

bool fsFile::isOpened()
{
    return opened;
}

Result fsFile::read(void *buff, u32 *readOut, u32 max)
{
    Result res = FSFILE_Read(fileHandle, readOut, offset, buff, max);
    if(res)
    {
        if(*readOut > max)
            *readOut = max;

        writeErrorToBuff((u8 *)buff, *readOut, (unsigned)res);

    }

    offset += *readOut;

    return res;
}

Result fsFile::write(void *dat, u32 *written, u32 size)
{
    *written = 0;
    Result res = FSFILE_Write(fileHandle, written, offset, dat, size, FS_WRITE_FLUSH);

    offset += *written;

    return res;
}

void fsFile::seek(int pos, u8 seekFrom)
{
    switch(seekFrom)
    {
        case 0:
            offset = pos;
            break;
        case 1:
            offset = offset + pos;
            break;
        case 2:
            offset = fileSize + pos;
            break;
    }
}

u8 fsFile::getByte()
{
    u8 ret;
    FSFILE_Read(fileHandle, NULL, offset, &ret, 1);
    ++offset;
    return ret;
}

void fsFile::putByte(u8 put)
{
    FSFILE_Write(fileHandle, NULL, offset, &put, 1, FS_WRITE_FLUSH);
    ++offset;
}

bool fsFile::close()
{
    Result res = FSFILE_Close(fileHandle);
    if(res)
        return false;

    return true;
}

u64 fsFile::size()
{
    return fileSize;
}

u64 fsFile::getOffset()
{
    return offset;
}
