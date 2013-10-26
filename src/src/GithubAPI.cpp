#include "GithubAPI.h"
#include <QtNetwork>
#include <QtCore>
#include "Vocable.h"

#define ACCESS_TOKEN "?access_token=fb6cc1e8ed683e414df9c8837ecfa62ae1c76a6d"
#define BASE_URL "https://api.github.com/gists"
#define INDEX_URL "https://api.github.com/users/Langunator/gists"

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
        qDebug() << "callback for " << handler;
        if (handler->reply->error() != QNetworkReply::NoError){
            handler->error(handler->reply);
        }
        else {
            handler->success(handler->reply);
        }
        handler->deleteLater();
    });
}

GithubAPI::~GithubAPI()
{
}

void GithubAPI::downloadPackages(const CategoriesPtr &packages, std::function<void (QList<Vocable> &packages, const CategoryPtr &cat)> callback
                                 , std::function<void (CategoryPtr, QString )> error)
{
    foreach(const CategoryPtr&pack, packages) {
        qDebug() << "download " << pack->categoryName() << " from " << pack->sourceFileName;
        request(pack->sourceFileName, [callback, error, pack, this](QNetworkReply* reply){
            QJsonParseError jsonError;
            QList<Vocable> lst;
            QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromBase64(reply->readAll()), &jsonError);
            if (doc.isNull() || jsonError.error != QJsonParseError::NoError)
            {
                error(pack, tr("Package\n\t%1 from %2\nError parsing JSON-Response\n\t%3")
                      .arg( pack->categoryName(), pack->author, jsonError.errorString()));
                return;
            }
            QJsonArray array = doc.array();
            foreach(const QJsonValue &val, array)
                lst.append(Vocable(val.toObject(), pack));
            callback(lst, pack);
        },
        [error,pack,this](QNetworkReply*reply){
            error(pack, tr("Package\n\t%1 from %2\nNetwork error:\n\t%3")
                  .arg( pack->categoryName(), pack->author, reply->errorString()));
        });
    }
}

QBuffer* GithubAPI::prepareUploadFileBuffer(const CategoryPtr&pack) {
    QBuffer *buf = new QBuffer(this);
    buf->open(QIODevice::WriteOnly);
    QJsonArray arr;
    DQList<Vocable> vocList = pack->vocables();
    for(int i=0;i<vocList.size();i++){
        QJsonObject obj;
        obj << *vocList.at(i);
        arr.append(obj);
    }
    QJsonObject data;
    data << *pack.data();
    buf->write("{\n\"description\": \"");
    buf->write( QJsonDocument(data).toJson().replace("\"","\\\"").replace("\n"," ") );
    buf->write("\",\n   \"files\" : {\n");
    buf->write("    \"\" : {");
    buf->write("\"content\" : \"");
    buf->write( QJsonDocument(arr).toJson().toBase64() );
    buf->write("\"}\n   }\n}");
    buf->close();
    buf->open(QIODevice::ReadOnly);
    buf->seek(0);
    return buf;
}

void GithubAPI::uploadPackages(const CategoriesPtr &packages,
                               std::function<void (CategoryPtr)> callback,
                               std::function<void (CategoryPtr, QString)> error,
                               std::function<void ()> uploadDone)
{
    QList<SuccessfullUploaded> *successfull = new QList<SuccessfullUploaded>();
    int *failed=new int;
	*failed=0;
    foreach(const CategoryPtr&pack, packages) {
        QBuffer *buf = prepareUploadFileBuffer(pack);
        qDebug() << "upload " << pack->categoryName() << ": \n" << buf->data();

        request(BASE_URL ACCESS_TOKEN, [=](QNetworkReply *reply){
            callback(pack);
            QString sha = QJsonDocument::fromJson( reply->readAll() ).object().value("sha").toString();
            qDebug() << "upload resonse " << sha;
            successfull->append( { sha, pack } );
            if (successfull->size() + *failed >= packages.size()) {
				delete failed;
                uploadDone();
            }
        }, [=](QNetworkReply *reply){
            error(pack, tr("Upload of %1 failed:\n\t%2").arg(pack->categoryName(), reply->errorString()));
            (*failed)++;
			if (successfull->size() + *failed >= packages.size()) {
				delete failed;
                uploadDone();
            }
        },buf);
    }
}

void GithubAPI::loadIndex(std::function<void (CategoriesPtr &packages)> callback, std::function<void (QNetworkReply*)> error)
{
    auto success = [this, callback] (QNetworkReply* reply) {
        QJsonDocument content = QJsonDocument::fromJson(reply->readAll());
        if (content.isArray() && content.array().isEmpty()) {
            qDebug() << "Index is empty";
            return;
        }

        lastAccess = QDateTime::currentDateTime();
        index = content.array();
        CategoriesPtr cats;
        foreach(QJsonValue obj, index) {
            QString metadata = obj.toObject().value("description").toString();
            QJsonObject val = QJsonDocument::fromJson(metadata.toUtf8()).object();
            CategoryPtr cat =  CategoryPtr(new Category(val));
            cat->sourceFileName = obj.toObject().value("files").toObject().constBegin().value().toObject().value("raw_url").toString();
            qDebug() << "Metadata "<< metadata << " @ " << cat->sourceFileName;
            cats.append(cat);
        }
        callback(cats);
    };

    request(INDEX_URL ACCESS_TOKEN, success, error);
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

