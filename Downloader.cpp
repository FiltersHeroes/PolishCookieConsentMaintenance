//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <https://www.gnu.org/licenses/>.

//In addition, as a special exception, the copyright holders give permission
//to link the code of portions of this program with the OpenSSL library under
//certain conditions as described in each individual source file, and
//distribute linked combinations including the two.
//You must obey the GNU General Public License in all respects for all of the
//code used other than OpenSSL.  If you modify file(s) with this exception,
//you may extend this exception to your version of the file(s), but you are
//not obligated to do so.  If you do not wish to do so, delete this exception
//statement from your version.  If you delete this exception statement from
//all source files in the program, then also delete it here.

#include "Downloader.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>

#include <QStandardPaths>
#include <JlCompress.h>
#include <QMessageBox>
#include "Widget.h"
#include <QWidget>

#include <QApplication>
#include <QDebug>
#include <QTextToSpeech>
#include <QTranslator>

Downloader::Downloader(QObject* parent) :
    BaseClass(parent)
{
    // Connect to the finished signal
    connect(&m_manager, &QNetworkAccessManager::finished, this, &Downloader::onReply);
}

bool Downloader::get(const QString& targetFolder, const QUrl& url)
{
    if (targetFolder.isEmpty() || url.isEmpty())
    {
        return false;
    }

    // Create object of file class for download
    // Here there is a target directory and the name of the file that is allocated from the URL
    m_file = new QFile(targetFolder + QDir::separator() + url.fileName());
    // Trying to open the file
    if (!m_file->open(QIODevice::WriteOnly))
    {
        delete m_file;
        m_file = nullptr;
        return false;
    }

    // Creating a request
    QNetworkRequest request(url);
    // Allow to go on redirects
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    // Running the download
    m_currentReply = m_manager.get(request);

    // After that, we immediately connect to signals about readiness of data to read and update the progress of downloading
    connect(m_currentReply, &QNetworkReply::readyRead, this, &Downloader::onReadyRead);
    connect(m_currentReply, &QNetworkReply::downloadProgress, this, &Downloader::updateDownloadProgress);
    return true;
}

void Downloader::onReadyRead()
{
    // If there is data and the file is open
    if (m_file)
    {
        // write them to a file
        m_file->write(m_currentReply->readAll());
    }
}


void Downloader::onReply(QNetworkReply* reply)
{
    Widget widget;
    auto updated = widget.GetUpdateMessage();
    // by completion of the request
    if (reply->error() == QNetworkReply::NoError)
    {
        // save file
        m_file->flush();
        m_file->close();

        // extract file
        JlCompress::extractDir(widget.pathDownloadExt+"/PolishCookieConsent_chromium.zip", widget.GetInstallPath());

        // cleanup
        QDir(widget.pathDownloadExt).removeRecursively();

        // voice message
        m_speech = new QTextToSpeech(this);
        m_speech -> setVolume(100.0);
        m_speech -> say(updated);

        // text message
        QMessageBox::information(nullptr, tr("Info"), updated);
    }
    else
    {
        // Delete file in case of error
        m_file->remove();

        // Show error message
        QMessageBox::critical(nullptr, tr("Info"), reply->errorString());
    }

    delete m_file;
    m_file = nullptr;
    reply->deleteLater();
}
