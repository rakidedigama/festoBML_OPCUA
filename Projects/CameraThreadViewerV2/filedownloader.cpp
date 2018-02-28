#include "filedownloader.h"

#include <iostream>

FileDownloader::FileDownloader( QObject *parent) :
 QObject(parent)
{
 connect(
  &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
  this, SLOT (fileDownloaded(QNetworkReply*))
  );


}

void FileDownloader::sendRequest(QUrl imageUrl)
{
    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
 m_DownloadedData = pReply->readAll();
 std::cout << QString::fromLatin1(m_DownloadedData).toStdString() << std::endl;
 //emit a signal
 pReply->deleteLater();
 emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
 return m_DownloadedData;
}
