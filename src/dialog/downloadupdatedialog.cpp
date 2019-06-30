//
// ScreenCloud - An easy to use screenshot sharing application
// Copyright (C) 2016 Olav Sortland Thoresen <olav.s.th@gmail.com>
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//

#include "downloadupdatedialog.h"
#include "ui_downloadupdatedialog.h"

DownloadUpdateDialog::DownloadUpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadUpdateDialog)
{
    ui->setupUi(this);
    netManager = new QNetworkAccessManager();
    connect(netManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
    connect(netManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(handleSslErrors(QNetworkReply*,QList<QSslError>)));
    connect(ui->button_install, SIGNAL(pressed()), this, SLOT(launchInstaller()));
    setupUi();
}

DownloadUpdateDialog::~DownloadUpdateDialog()
{
    delete ui;
    delete netManager;
}
void DownloadUpdateDialog::setupUi()
{
    ui->button_install->setEnabled(false);
}
void DownloadUpdateDialog::startDownload(QString version)
{
#ifdef Q_OS_WIN
    tmpFile = new QFile(QDir::tempPath() + "/" + "ScreenCloud-" + version + "-" + QString(ARCH) + ".msi", this);
    if(tmpFile->exists())
    {
        INFO(tr("Removing existing installer ") + tmpFile->fileName());
        tmpFile->remove();
    }
    INFO(tr("Saving installer to ") + tmpFile->fileName());
    QNetworkRequest downloadRequest("https://api.screencloud.net/files/windows/ScreenCloud-" + version + "-" + QString(ARCH) + ".msi");
    QNetworkReply* r = netManager->get(downloadRequest);
    connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateDataTransferProgress(qint64,qint64)));
    INFO(tr("Downloading ") + downloadRequest.url().toString());
#endif
#ifdef Q_OS_MACX
    tmpFile = new QFile(QDir::tempPath() + "/" + "ScreenCloud-" + version + ".dmg", this);
    if(tmpFile->exists())
    {
        INFO(tr("Removing existing dmg ") + tmpFile->fileName());
        tmpFile->remove();
    }
    INFO(tr("Saving dmg to ") + tmpFile->fileName());
    QNetworkRequest downloadRequest("https://api.screencloud.net/files/mac/ScreenCloud-" + version + ".dmg");
    QNetworkReply* r = netManager->get(downloadRequest);
    connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateDataTransferProgress(qint64,qint64)));
    INFO(tr("Downloading ") + downloadRequest.url().toString());
#endif
}
void DownloadUpdateDialog::displayErrorMessage(QString error)
{
    WARNING(error);
    QMessageBox::critical(this, tr("Failed to download update"), error);
    this->close();
}
void DownloadUpdateDialog::downloadFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        tmpFile->open(QIODevice::WriteOnly);
        tmpFile->write(reply->readAll());
        tmpFile->flush();
        tmpFile->close();
        INFO(tr("Successfully downloaded update"));
        ui->button_install->setEnabled(true);
        ui->button_install->setDefault(true);
    }else
    {
        WARNING(tr("Error while downloading update. ") + reply->errorString());
        displayErrorMessage(tr("Error while downloading update. ") + reply->errorString());
    }
}

void DownloadUpdateDialog::updateDataTransferProgress(qint64 done, qint64 total)
{
    ui->progress_download->setMaximum(100);
    ui->progress_download->setValue((int)((done * 100) / total));
    QString doneString = QString::number(done / 1000) + "K";
    QString totalString = QString::number(total /1000) + "K";
    ui->label_progress->setText(doneString + "/" + totalString);
}

void DownloadUpdateDialog::launchInstaller()
{
    QProcess* process = new QProcess();
#ifdef Q_OS_WIN
    QString program = "msiexec /i " + QDir::toNativeSeparators(tmpFile->fileName()) + " /qr";
#else
    QString program = "open " + QDir::toNativeSeparators(tmpFile->fileName());
#endif
    INFO(tr("Starting new process: ") + program);
    process->start(program);
    QCoreApplication::exit(0);
}

void DownloadUpdateDialog::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    QString errorString;
    for(int i = 0; i < errors.count(); i++)
    {
        errorString.append(errors.at(i).errorString() + ", ");
    }
    WARNING(tr("Got SSL errors while connecting to ") + reply->request().url().host() + "." + errorString);
    displayErrorMessage(tr("Got SSL errors while connecting to ") + reply->request().url().host() + "." + errorString);
}





