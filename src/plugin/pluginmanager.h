/* * ScreenCloud - An easy to use screenshot sharing application
 * Copyright (C) 2016 Olav Sortland Thoresen <olav.s.th@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 */

#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

// https://github.com/NixOS/nixpkgs/issues/42811
#ifdef slots
#undef slots
#endif

#include <Python.h>

#include <QObject>
#include <uploaders/uploader.h>
#include <QApplication>
#include <QDir>
#include <QDesktopServices>
#include <utils/OS.h>
#include <uploaders/pythonuploader.h>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <quazipfile.h>
#include <quazip.h>
#include <utils/network.h>
#include <QDomDocument>
#include <QInputDialog>
#include <PythonQt.h>

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = 0);
    ~PluginManager();
    void loadPlugins();
    void unloadPlugins();
    void reloadPlugins();
    void initStdinHandler();
    bool isLoaded(QString shortname);
    void installPlugins(QStringList& urls);
    bool uninstallPlugins(QStringList &shortnames);
    void cancelInstallation();
    void extractPlugin(QString zipFilename);
    void analyzeAndMovePlugin(QString exctractedDirPath);
    bool removeDir(const QString & dirName);
    QUrl checkUrlForRedirect(QUrl checkUrl);
    bool copyFolder(QString sourceFolder, QString destFolder);
    bool isInstalling();
    int getCurrentProgress();

private:
    QNetworkAccessManager netManager;
    QList<QNetworkReply*> netReplies;
    QFile* tmpFile;
    bool busy;
    int currentProgress;
    QString lastPythonStdOut, lastPythonStdErr;

public:
    static bool isInstalled(QString shortname);
    static QString installedVersion(QString shortname);
    static int countInstalledPlugins();
    static QString pluginPath() {
        return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/plugins";
    }
    static QString pythonQtInputCallback(void *callData);

    QHash<QString, Uploader*> uploaderPlugins; //List of available uploader plugins

Q_SIGNALS:
    void installationProgress(int);
    void pluginInstalled(QString);
    void installationError(QString);
    void installationCanceled();

public Q_SLOTS:
    void fileDownloaded(QNetworkReply* reply);
    void pythonStdOut(QString out);
    void pythonStdErr(QString err);

    
};

#endif // PLUGINLOADER_H
