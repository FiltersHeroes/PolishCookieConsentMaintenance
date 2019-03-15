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

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QNetworkAccessManager>
#include <QTextToSpeech>

class QNetworkReply;
class QFile;

class Downloader : public QObject
{
    Q_OBJECT
    using BaseClass = QObject;

public:
    explicit Downloader(QObject* parent = nullptr);

    // Method for starting the download
    bool get(const QString& targetFolder, const QUrl& url);

public slots:


signals:
    // A signal that sends information about the progress of the download
    void updateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    // Slot for gradual reading of downloaded data
    void onReadyRead();

    // Slot for processing request completion
    void onReply(QNetworkReply* reply);


private:
    QNetworkReply* m_currentReply {nullptr};    // Current request being processed
    QFile* m_file                 {nullptr};    // The current file to which the entry is being written
    QNetworkAccessManager m_manager;            // Network manager for downloading files
    QTextToSpeech* m_speech;                    // Speech synthesizer
};

#endif // DOWNLOADER_H
