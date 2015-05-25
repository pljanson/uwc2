/*
    ExportAchive - export all files in Webshots archive (Webshots Data) to JPG
    Create a directory structure based on the album "Dest\Topic\Title\" and
    a file name based on the ID and the photo title "ph10000-Title.jpg"

    Copyright (C) 2015 PLJ

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef EXPORTARCHIVE_H
#define EXPORTARCHIVE_H

#include <QString>
#include "cWebshots\TXT.h"
#include "ExportWindow.h"

using namespace cWebshots;

namespace ExportArchive
{
    enum class ImageType { PHOTO, THUMB }; //C++11 scoped enums

    struct ExportOptions
    {
        ExportWindow* parent;
        QString source;
        QString destination;
        bool doThumbs;
        bool doAlbumPhoto;
        bool stopOnError;

        ExportOptions() :
            parent(NULL),
            source(""),
            destination(""),
            doThumbs(false),
            doAlbumPhoto(true),
            stopOnError(false)
        {}

        ExportOptions(ExportWindow* callback,
            QString src,
            QString dest,
            bool doTh,
            bool doAlb,
            bool stopOnErr)
        {
            parent       = callback;
            source       = src;
            destination  = dest;
            doThumbs     = doTh;
            doAlbumPhoto = doAlb;
            stopOnError  = stopOnErr;
        }
    };

    bool ExportWebshotsArchive(const ExportOptions &options);
    bool ProcessAlbum(const QString &albumPath,
                      TXT &txt,
                      const ExportOptions &options);

    bool CopyFile(const QString &albumPath,
                  const QString &exportPath,
                  const QString &fileName,
                  const ExportOptions &options );
    bool ExportImage(const sPhotosTXT &photo,
                     const ExportOptions &options,
                     const QString &albumPath,
                     const QString &exportPath,
                     ImageType type);
    void LogMessage(const QString &msg,
                    const ExportOptions &options);
}
#endif // EXPORTARCHIVE_H
