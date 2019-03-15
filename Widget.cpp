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

#include "Widget.h"
#include "ui_Widget.h"

#include <QFileDialog>
#include <QStandardPaths>

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QSettings>
#include <QPushButton>
#include <QVersionNumber>
#include <JlCompress.h>
#include <QTranslator>

QString Widget::GetInstallPath()
{
    QSettings settings("PolishFiltersTeam", "PolishCookieConsentUpdater");
    QString pathInstallExt = "";
    if (settings.contains("pathInstallExt")) {
        pathInstallExt = settings.value("pathInstallExt").toString();
    }
    else {
        pathInstallExt = QDir::homePath() + "/" + tr("Extensions") + "/PolishCookieConsent";
    }
    return pathInstallExt;
}

void Widget::onSuccessfulGetRequest(QNetworkReply *replyR) {
    QString json_string;
    json_string = replyR->readAll();
    QByteArray json_bytes = json_string.toUtf8();
    auto json_doc=QJsonDocument::fromJson(json_bytes);

    if(json_doc.isNull()) {
        qDebug()<<"Failed to create JSON doc.";
    }
    if(!json_doc.isObject()) {
        qDebug()<<"JSON is not an object.";
    }

    QJsonObject json_obj=json_doc.object();

    if(json_obj.isEmpty()) {
        qDebug()<<"JSON object is empty.";
    }

    QVariantMap json_map = json_obj.toVariantMap();

    auto latestReleaseTag = json_map["tag_name"].toString();
    auto newVersion = QVersionNumber::fromString(latestReleaseTag.replace("v", ""));
    replyR->deleteLater();

    QFile file(GetInstallPath()+"/manifest.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString json_file;
    json_file = file.readAll();
    QByteArray json_file_bytes = json_file.toUtf8();
    auto json_doc_file=QJsonDocument::fromJson(json_file_bytes);
    QJsonObject json_file_obj=json_doc_file.object();
    QVariantMap json_file_map = json_file_obj.toVariantMap();
    auto oldVersion = QVersionNumber::fromString(json_file_map["version"].toString());

    if(newVersion>oldVersion) {
        if(!QDir(pathDownloadExt).exists()) {
            QDir().mkdir(pathDownloadExt);
        }
        m_downloader.get(pathDownloadExt, QUrl("https://github.com/PolishFiltersTeam/PolishCookieConsent/releases/download/v" +latestReleaseTag + "/PolishCookieConsent_chromium.zip"));
    }
    else if (replyR->error()) {
        QMessageBox::critical(this, tr("Info"), replyR->errorString());
    }
    else {
        QMessageBox::information(this,tr("Info"), tr("There is currently no newer version of the Polish Cookie Consent extension."));
    }

}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->changePathLbl->setText(GetInstallPath());

    QStringList args = QCoreApplication::arguments();
    if(args.contains("/u"))
    {
       ui->downloadPushButton->animateClick();
    }
    // Connect to slots
    connect(ui->downloadPushButton, &QPushButton::clicked, this, &Widget::onDownloadButtonClicked);
    connect(&m_downloader, &Downloader::updateDownloadProgress, this, &Widget::onUpdateProgress);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onDownloadButtonClicked()
{
    QNetworkAccessManager *man = new QNetworkAccessManager(this);
    connect(man, &QNetworkAccessManager::finished, this, &Widget::onSuccessfulGetRequest);
    const QUrl url = QUrl(myURL);
    QNetworkRequest request(url);
    man->get(request);
}

void Widget::onUpdateProgress(int bytesReceived, int bytesTotal)
{
    // Updating download progress
    ui->downloadProgressBar->setMaximum(bytesTotal);
    ui->downloadProgressBar->setValue(bytesReceived);

}

void Widget::on_changePathButton_clicked()
{
    QFileDialog *fd = new QFileDialog;
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    int result = fd->exec();
    QString directory;
    if (result)
    {
        QString installPath = fd->selectedFiles()[0];
        QSettings settings("PolishFiltersTeam", "PolishCookieConsentUpdater");
        settings.setValue("pathInstallExt", installPath);
        ui->changePathLbl->setText(installPath);
    }
}
