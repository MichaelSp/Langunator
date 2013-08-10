#include "GithubAPI.h"
#include <QtNetwork>
#include <QtCore>
#include "vocabel.h"


GithubAPI::GithubAPI(QWidget *parent) :
    QObject(parent),
    oauth(parent),
    mapper(this)
{
    manager  = new QNetworkAccessManager(this);
    defaultErrorHandler = [this] (QNetworkReply* reply) {
        qWarning() << "Error getting " << reply->url() << " - Error " << reply->errorString();
    };

    connect(&mapper, static_cast<void (QSignalMapper::*)(QObject*)>(&QSignalMapper::mapped), [](QObject* obj){
        ResponseHandler *handler = qobject_cast<ResponseHandler*>(obj);
        if (handler->reply->error() != QNetworkReply::NoError){
            handler->error(handler->reply);
        }
        else {
            handler->success(handler->reply);
        }
        handler->deleteLater();
    });
}

void GithubAPI::downloadPackages(const CategoriesPtr &packages, std::function<void (QList<Vocable> &packages)> callback
                                 , std::function<void (CategoryPtr, QString )> error)
{
    foreach(const CategoryPtr&pack, packages) {
        qDebug() << "download " << pack->categoryName() << " from " << pack->sourceFileName;
        request(pack->sourceFileName, [callback, error, pack](QNetworkReply* reply){
            QJsonParseError jsonError;
            QList<Vocable> lst;
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            if (doc.isNull() || jsonError.error != QJsonParseError::NoError)
            {
                error(pack, tr("Package\n\t%1 from %2\nError parsing JSON-Response\n\t%3")
                      .arg( pack->categoryName(), pack->author, jsonError.errorString()));
                return;
            }
            QJsonArray array = doc.array();
            foreach(const QJsonValue &val, array)
                lst.append(Vocable(val.toObject(), pack));
            callback(lst);
        },
        [error,pack](QNetworkReply*reply){
            error(pack, tr("Package\n\t%1 from %2\nNetwork error:\n\t%3")
                  .arg( pack->categoryName(), pack->author, reply->errorString()));
        });
    }
}

void GithubAPI::uploadPackages(const CategoriesPtr &packages,
                               std::function<void (CategoryPtr)> callback,
                               std::function<void (CategoryPtr, QString)> error,
                               std::function<void ()> uploadDone)
{
    CategoriesPtr *successfull = new CategoriesPtr();
    int *failed=new int;
	*failed=0;
    foreach(const CategoryPtr&pack, packages) {
        qDebug() << "upload " << pack->categoryName();
        QBuffer *buf = new QBuffer(this);
        buf->open(QIODevice::WriteOnly);
        QJsonArray arr;
        DQList<Vocable> vocList = pack->vocables();
        for(int i=0;i<vocList.size();i++){
            QJsonObject obj;
            obj << *vocList.at(i);
            arr.append(obj);
        }
        buf->write( QJsonDocument(arr).toJson() );
        buf->close();
        buf->open(QIODevice::ReadOnly);
        qDebug() << buf->readAll();
        buf->seek(0);

        // POST /repos/:owner/:repo/git/blobs
        request("http://raspi/~michael/blobs", [=](QNetworkReply *){
            callback(pack);
            successfull->append(pack);
            if (successfull->size() + *failed >= packages.size()) {
				delete failed;
                updateIndex(successfull);
                uploadDone();
            }
        }, [=](QNetworkReply *reply){
            error(pack, tr("Upload of %1 failed:\n\t%2").arg(pack->categoryName(), reply->errorString()));
            (*failed)++;
			if (successfull->size() + *failed >= packages.size()) {
				delete failed;
                updateIndex(successfull);
                uploadDone();
            }
        },buf);
    }
}

void GithubAPI::updateIndex(CategoriesPtr *successfull)
{
    QJsonArray packages = index.value("packages").toArray();
    foreach(CategoryPtr cat, *successfull) {
        QJsonObject obj;
        obj << *cat.data();
        packages.append(obj);
    }
    index.insert("packages", packages);
    qDebug() << QJsonDocument(index).toJson();
	delete successfull;
}

void GithubAPI::loadIndex(std::function<void (CategoriesPtr &packages)> callback, std::function<void (QNetworkReply*)> error)
{
    auto success = [this, callback] (QNetworkReply* reply) {
        index = QJsonDocument::fromJson(reply->readAll() ).object();
        QJsonArray packages = index.value("packages").toArray();
        lastAccess = QDateTime::currentDateTime();
        CategoriesPtr cats;
        foreach(QJsonValue val, packages)
        {
            cats.append( CategoryPtr(new Category(val.toObject())));
        }
        callback(cats);
    };
    // GET /repos/:owner/:repo/git/blobs/:sha
    request("http://raspi/~michael/index.json", success, error);
}

void GithubAPI::request(QString url, std::function<void (QNetworkReply*)> success, std::function<void (QNetworkReply*)> error, QIODevice *uplData)
{
    qDebug() << "request: " << url;
    QNetworkReply *reply;
    if (uplData != NULL) {
        QNetworkRequest req;
        req.setUrl(QUrl(url));
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        reply = manager->post(req, uplData);
    }
    else
        reply = manager->get(QNetworkRequest(QUrl(url)));
    ResponseHandler *handler = new ResponseHandler(reply, success, ((error==NULL)?defaultErrorHandler:error), uplData);
    mapper.setMapping(reply, handler);
    connect(reply, SIGNAL(finished()), &mapper, SLOT(map()));
}

