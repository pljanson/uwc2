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
#include "ExportArchive.h"
#include<QFileInfo>
#include<QDir>

#include "cWebshots\Picture.h"
#include <QRegExp>
#include <QString>
#include <QDebug>
#include <iostream>

bool createDir(QString newPath)
{
    bool succes = true;

    QDir dir(newPath);
    if (!dir.exists())
    {
        succes = dir.mkpath(".");
    }

    return succes;
}

void ExportArchive::LogMessage(const QString &msg, const ExportOptions &options)
{
    options.parent->logMessage(msg);
    qDebug() << msg;
}

bool ExportArchive::ExportWebshotsArchive(const ExportOptions &options)
{
    bool succes = true;

    //loop subdirs from source
    QDir dir(options.source);
    if (dir.exists())
    {
        foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllDirs ))
        {
            if (info.isDir())
            {
                TXT txt(info.absoluteFilePath().toStdString() + "/");

                if (txt.Album.count)
                {
                    QString albumPath = info.absoluteFilePath();
                    QString msg2 = "AlbumPath[" + info.absoluteFilePath() + "]";
                    LogMessage(msg2, options);

                    succes &= ProcessAlbum(albumPath, txt, options);
                }
                else
                {
                    QString dir = info.absoluteFilePath();
                    QString msg2 = "WARNING: Not a Album[" + dir + "]";
                    LogMessage(msg2, options);
                }
            }
            if ((!succes) && (options.stopOnError))
            {
                QString msg2 = "Stopped on error (ExportWebshotsArchive)";
                LogMessage(msg2, options);
                break;
            }
        }
    }
    else
    {
        succes = false;
        QString msg2 = "Error Source dir not exists: [" + options.source + "]";
        LogMessage(msg2, options);
    }
    return succes;
}

bool ExportArchive::CopyFile(const QString &albumPath,
                             const QString &exportPath,
                             const QString &fileName,
                             const ExportOptions &options )
{
    bool succes = true;

    QString pathAlbumIn = albumPath  + "\\" + fileName;
    QString pathAlbumOut = exportPath  + "\\" + fileName;

    QFile fileAlbum(pathAlbumOut);
    if (!fileAlbum.exists())
    {
        succes &= QFile::copy(pathAlbumIn, pathAlbumOut);
    }
    else
    {
        //warning :) succes stay true;
        QString msg2 = "warning: " + fileName + " already exists:" + pathAlbumOut;
        LogMessage(msg2, options);
    }

    return succes;
}

bool ExportArchive::ExportImage(const sPhotosTXT &photo,
                                const ExportOptions &options,
                                const QString &albumPath,
                                const QString &exportPath,
                                ImageType type)
{
    bool succes = true;

    //setup
    QString url;
    QString typeString;
    if (type == ImageType::PHOTO)
    {
        url = QString::fromStdString(photo.url_photo);
        typeString = "PHOTO";
    }
    else
    {
        url = QString::fromStdString(photo.url_thumb);
        typeString = "THUMB";
    }
    QString pathPhoto = albumPath  + "\\" + url;
    QString exportPhoto = exportPath + "\\" + url;
    QString photoTitle = QString::fromStdString(photo.title).simplified();//strip whitespace
    photoTitle.replace( QRegExp( "[" + QRegExp::escape( "\\/:*?\"<>| " ) + "]" ), QString( "_" ) );
    int lastPoint = exportPhoto.lastIndexOf(".");
    QString newFileName = exportPhoto.left(lastPoint) + "-" + photoTitle + ".jpg";

    //export this image using cWebshots Picture
    QString msg2 = "picture " + typeString + "[" + url + "]";
    LogMessage(msg2, options);

    try
    {
        Picture picture(pathPhoto.toStdString());
        picture.saveToFile(newFileName.toStdString(), cWebshots::JPG);
    }
    catch(Exceptions::FileSystemError err)
    {
        QString msg2 = "Exceptions::FileSystemError (picture:" + typeString + ") error:" + QString::fromStdString(err.Description());
        LogMessage(msg2, options);
        succes = false;
    }
    catch (Exceptions::UnknownFormat err)
    {
        QString msg2 = "Exceptions::UnknownFormat (picture:" + typeString + ") error:" + QString::fromStdString(err.Description());
        LogMessage(msg2, options);
        succes = false;
    }
    catch (...)
    {
        QString msg2 = "Exceptions (picture:" + typeString + ")";
        LogMessage(msg2, options);
        succes = false;
    }

    return succes;
}

bool ExportArchive::ProcessAlbum(const QString &albumPath, TXT &txt, const ExportOptions &options)
{
    bool succes = true;

    QDir dir(albumPath);
    if (dir.exists())
    {
        //export dir
        QString cleanTopic= QString::fromStdString(txt.Album.topic_name).simplified(); //strip whitespace
        cleanTopic.replace( QRegExp( "[" + QRegExp::escape( "\\/:*?\"<>|" ) + "]" ), QString( "_" ) );

        QString cleanTitle= QString::fromStdString(txt.Album.title).simplified(); //strip whitespace
        cleanTitle.replace( QRegExp( "[" + QRegExp::escape( "\\/:*?\"<>|" ) + "]" ), QString( "_" ) );

        QString exportPath = options.destination + "\\" + cleanTopic + "\\" + cleanTitle;
        QString msg2 = "exportPath = [" + exportPath + "]";
        LogMessage(msg2, options);

        createDir(exportPath);

        //for each picture do:
        for (unsigned int i = 0; i < txt.pictureCount(); ++i)
        {
            sPhotosTXT photo = txt.picture(i);

            succes &= ExportImage(photo, options, albumPath, exportPath, ImageType::PHOTO);

            if (options.doThumbs)
            {
                succes &= ExportImage(photo, options, albumPath, exportPath, ImageType::THUMB);
            }

            if ((!succes) && (options.stopOnError))
            {
                QString msg2 = "Stopped on error (ProcessAlbum)";
                LogMessage(msg2, options);
                break;
            }
        }

        if ((options.doAlbumPhoto) && (txt.pictureCount() > 0))
        {
            //copy the album and photo file
            QString msg2 = "copy[album.txt & photos.txt]";
            LogMessage(msg2, options);

            succes &= ExportArchive::CopyFile(albumPath,
                                              exportPath,
                                              "album.txt",
                                              options);

            succes &= ExportArchive::CopyFile(albumPath,
                                              exportPath,
                                              "photos.txt",
                                              options);
        }
    }
    else
    {
        //should not happen since it was read from the directory itself
        QString msg2 = "Album dir does not exist";
        LogMessage(msg2, options);

        succes = false;
    }

    LogMessage("", options);
    return succes;
}
