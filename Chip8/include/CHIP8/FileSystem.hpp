/*********************************************************************
Matt Marchant 2016
http://trederia.blogspot.com

CHIP8 - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

//taken from the xygnine source https://github.com/fallahn/xygine

#ifndef CH_FILESYSTEM_HPP_
#define CH_FILESYSTEM_HPP_

#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <vector>

//TODO check this macro works on all windows compilers
//(only tested in VC right now)
#ifdef _WIN32

#include <Windows.h>

#else
#include <libgen.h>
#include <dirent.h>

#endif //_WIN32

static std::vector<std::string> listFiles(std::string path)
{
    std::vector<std::string> results;

#ifdef _WIN32
    if (path.back() != '/')
        path.append("/*");
    else
        path.append("*");

    //convert to wide chars for windows
    std::basic_string<TCHAR> wPath;
    wPath.assign(path.begin(), path.end());

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(wPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to find file data, invalid file handle returned" << std::endl;
        return results;
    }

    do
    {
        if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) //not a directory
        {
            //convert from wide char
            std::basic_string<TCHAR> wName(findData.cFileName);
            std::string fileName;
            fileName.assign(wName.begin(), wName.end());
            results.push_back(fileName);
        }

    } while (FindNextFile(hFind, &findData) != 0);
    FindClose(hFind);

    return std::move(results);
#else
    if (path.back() != '/')
        path.append("/.");
    else
        path.append(".");

    struct dirent* dp;
    DIR* dir = opendir(path.c_str());

    if (dir)
    {
        while ((dp = readdir(dir)) != nullptr)
        {
            std::string workingPath(path);
            workingPath.append("/");
            workingPath.append((dp->d_name));

            struct stat buf;
            if (!stat(workingPath.c_str(), &buf))
            {
                if (!S_ISDIR(buf.st_mode))
                {
                    results.emplace_back(dp->d_name);
                }
            }
        }
        closedir(dir);
    }
    return std::move(results);
#endif //_WIN32
}

#endif //CH_FILESYSTEM_HPP_