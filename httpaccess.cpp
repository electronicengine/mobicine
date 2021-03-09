/****************************************************************************

****************************************************************************/
#include <QtNetwork>
#include <QUrl>
#include <iostream>
#include <QResource>
#include <QStandardPaths>
#include <QPoint>
#include <QByteArray>
#include "httpaccess.h"



HttpAccess::HttpAccess()
{
}



QByteArray HttpAccess::makeJsonObject(const QString &FunctionName, const QStringList &Params)
{
    QJsonObject obj;
    QJsonArray params;
    obj.insert("FunctionName", FunctionName);

    foreach (QString param, Params)
    {
        params.append(QJsonValue(param));
    }

    obj.insert("params", params);

    QJsonDocument doc(obj);

    qDebug() << doc.toJson();

    return doc.toJson();
}



QStringList HttpAccess::makeHttpJsonRequest(const QString &FunctionName, const QStringList &Params)
{
    QStringList query_result;
    QNetworkReply *network_reply;
    QNetworkAccessManager network_access;
    QNetworkRequest network_request(QUrl(SERVICE_URL));
    network_request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    int time_out_counter = 0;

    network_reply = network_access.post(network_request, makeJsonObject(FunctionName, Params));

    while(!network_reply->isFinished())
    {
        if(time_out_counter++ >= NETWORK_TIMEOUT)
        {
            return QStringList("Connection Error");
        }

        qApp->processEvents();
    }

    QByteArray response = network_reply->readAll();

    query_result = QString(response).split("<br>", QString::SkipEmptyParts);
    query_result = query_result.toSet().toList();
    if(query_result.empty())
        query_result.append("No Result");

    return query_result;
}



void HttpAccess::makeHttpSearch(std::vector<QStringList> Request, std::vector<QStringList> &ResultList)
{

    QUrl Url_;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;

    QString html_content;

    Url_ = Request[0].at(0);

    reply = qnam.get(QNetworkRequest(Url_));

    while(!reply->isFinished())
    {
        qApp->processEvents();

    }

    const QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (!redirectionTarget.isNull())
    {
        const QUrl redirectedUrl = Url_.resolved(redirectionTarget.toUrl());

        Url_= redirectedUrl.toString();

        makeHttpSearch(Request, ResultList);
    }
    else
    {
        html_content = reply->readAll();

    }




}



void HttpAccess::searchRequest(std::vector<QStringList> Request, std::vector<QStringList> &ResultList)
{

    if(Request.at(0).at(0).indexOf(HTTPS_SEARCH_KEYWORD) >= 0 ||
            Request.at(0).at(0).indexOf(HTTP_SEARCH_KEYWORD) >= 0)
    {
        makeHttpSearch(Request, ResultList);
    }
    else if(Request.at(0).at(0).indexOf(FILE_SEARCH_KEYWORD) >= 0)
    {
    }

}



