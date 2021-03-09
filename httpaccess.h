#ifndef HTTPACCESS_H
#define HTTPACCESS_H

#include <QSslSocket>
#include <QNetworkAccessManager>
#include <QUrl>
#include <condition_variable>
#include <QWaitCondition>
#include <QSemaphore>
#include <memory>
#include <mutex>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSslError>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QAction>
#include <QMenu>
#include <QPointer>
#include <QMessageBox>
#include <memory>
#include "qmllistobject.h"


class HttpWindow;
class QSslError;
class QAuthenticator;
class QNetworkReply;
class LogView;
class SearchWindow;

#define FILE_SEARCH_KEYWORD     "file://"
#define HTTP_SEARCH_KEYWORD     "http://"
#define HTTPS_SEARCH_KEYWORD    "https://"
#define EXTERNAL_LOC_KEYWORD    "external"
#define NETWORK_TIMEOUT         16000000
#define SERVICE_URL             "https://www.erbull.me/mobicine.php"

class HttpAccess
{
//    Q_OBJECT

public:
    HttpAccess();

    QByteArray makeJsonObject(const QString &FunctionName, const QStringList &Params);
    QStringList makeHttpJsonRequest(const QString &FunctionName, const QStringList &Params);
    void makeHttpSearch(std::vector<QStringList> Request, std::vector<QStringList> &ResultList);
    void searchRequest(std::vector<QStringList> Request, std::vector<QStringList> &ResultList);

private:



};

#endif // HTTPACCESS_H
