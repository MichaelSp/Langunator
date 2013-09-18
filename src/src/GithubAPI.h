#pragma once

#include <QObject>
#include <functional>
#include "oauth2.h"
#include "category.h"

class QNetworkReply;
class QNetworkAccessManager;

class GithubAPI : public QObject
{
    Q_OBJECT
public:
    explicit GithubAPI(QWidget *parent);

signals:
    void errorLoading(const CategoriesPtr &packages, QString message);
    
public slots:
    void downloadPackages(const CategoriesPtr &packages, std::function<void (QList<Vocable> &)>    callback, std::function<void (CategoryPtr, QString)> error);
    void uploadPackages  (const CategoriesPtr &packages,
                          std::function<void (CategoryPtr)> callback,
                          std::function<void (CategoryPtr, QString)> error,
                          std::function<void ()> uploadDone);
    void loadIndex(std::function<void (CategoriesPtr &packages)> callback, std::function<void (QNetworkReply*)> error);

private:
    struct SuccessfullUploaded {
        QString sha;
        CategoryPtr cat;
    };

    QBuffer *prepareUploadFileBuffer(const CategoryPtr &pack);
    void updateIndex(QList<SuccessfullUploaded> *successfull);
    void request(QString url, std::function<void (QNetworkReply *)> success, std::function<void (QNetworkReply*)> error = NULL, QIODevice *uplData=NULL);
    OAuth2 oauth;
    QNetworkAccessManager *manager;
    std::function<void (QNetworkReply *)> defaultErrorHandler;
    QDateTime lastAccess;
    QSignalMapper mapper;
    QJsonObject index;
    QString indexBlobSHA;
};

class ResponseHandler: public QObject
{
    Q_OBJECT
public:
    ResponseHandler(QNetworkReply *reply, std::function<void (QNetworkReply*)> success,
                    std::function<void (QNetworkReply*)> error, QIODevice* uplData=NULL)
        :reply(reply),success(success),error(error),uploadIODevice(uplData){}
    ~ResponseHandler() { if (this!= NULL && uploadIODevice != NULL) delete uploadIODevice;}
    QNetworkReply *reply;
    std::function<void (QNetworkReply*)> success;
    std::function<void (QNetworkReply*)> error;
    QIODevice *uploadIODevice;

};
