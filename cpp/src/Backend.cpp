#include "Backend.h"

Backend::Backend(QObject *parent) :
    QObject(parent),
    cacheIsDirty(true),
    vocabularyListQuery(NULL)
{
    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

Backend::~Backend()
{
    if (!currentCategory().isNull())
        currentCategory()->save();
    delete vocabularyListQuery;
}

void Backend::delayedInit()
{
    dbInstance = new DB();

    vocabularyListQuery = new QSqlQuery();
    vocabularyListQuery->prepare("SELECT language1,language2,lektion FROM " + Vocable::TableName() + " WHERE category = :cat" );

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    connect(dbInstance, &DB::dbLoaded, this, &Backend::dbLoaded);
#else
    connect(dbInstance, SIGNAL(dbLoaded()), this, SLOT(dbLoaded()));
#endif
    if (dbInstance->isLoaded())
        dbLoaded(); // signal is emitted b4 we reach that line...
    prepareTrainingSet();
}

void Backend::addCategory(QString languageFrom,QString languageTo)
{
    CategoryPtr cat(new Category(languageFrom,languageTo));
    if (cat->save(false,false))
    {
        cacheIsDirty = true;
        emit categoriesUpdated(categories());
    }
    else
        qWarning() << "Cannot add category " << cat->categoryName() <<": "
                   << Vocable::objects().lastQuery().lastError();
}

void Backend::removeCategory(CategoryPtr &cat)
{
    if (!Vocable::objects().filter(DQWhere( "language1 = ", cat->id ) || DQWhere("language2 = ",cat->id)).remove()) {
        qWarning() << "Cannot remove vocab for category " << cat->categoryName() <<": "
                   << Vocable::objects().lastQuery().executedQuery();
        return;
    }
    if (cat->remove())
    {
        cat.clear();
        cacheIsDirty=true;
        emit categoriesUpdated(categories());
    }
    else
        qWarning() << "Cannot remove category " << cat->categoryName()<<": " << Category::objects().lastQuery().executedQuery();
}

const CategoriesPtr& Backend::categories()
{
    if (_categories.isEmpty() || cacheIsDirty)
        fetchCategories();
    return _categories;
}

CategoryPtr Backend::currentCategory() const
{
    return m_currentCategory;
}

void Backend::refreshVocListTable()
{
    vocabularyListQuery->exec();
    vocListModel->setQuery(*vocabularyListQuery);
}

void Backend::addVocable(QString lang1, QString lang2, int lektion)
{
    bool ok = currentCategory()->addVocable(lang1, lang2, lektion);
    if (ok && !vocListModel.isNull())
    {
        refreshVocListTable();
    }
}

void Backend::updateVocable(const QModelIndex &idx, QString lang1, QString lang2, int lektion)
{
    if (!idx.isValid())
        return;
    Vocable voc;
    voc.load(DQWhere("language1 = ", idx.model()->index(idx.row(),0).data().toString()) &&
             DQWhere("language2 = ", idx.model()->index(idx.row(),1).data().toString()));
    voc.language1 = lang1;
    voc.language2 = lang2;
    voc.lesson = lektion;
    bool ok = voc.save();
    if (ok && !vocListModel.isNull())
    {
        refreshVocListTable();
    }
}

void Backend::removeVocable(const QModelIndex &idx)
{
    if (!idx.isValid())
        return;
    bool ok = Vocable::objects().filter(DQWhere("language1 = ", idx.model()->index(idx.row(),0).data().toString()) &&
                                        DQWhere("language2 = ", idx.model()->index(idx.row(),1).data().toString())).remove();
    if (ok && !vocListModel.isNull())
    {
        refreshVocListTable();
    }
}

QAbstractItemModel *Backend::currentVocabularyModel()const
{
    return vocListModel.data();
}

void Backend::prepareTrainingSet()
{
    DQSharedQuery qry(Vocable::objects());
    trainingSet = qry.filter( DQWhere("(julianday(Date('now')) - julianday(lastAsked))") >= DQWhere("rightInRow*rightInRow") ).all();
    emit newVocable(currentVocable());
}

Vocable* Backend::currentVocable()
{
    if (trainingSet.size() <= 0)
        return NULL;
    return trainingSet.at(0);
}

void Backend::showNextVocable()
{
    if (trainingSet.size()>0)
        trainingSet.removeAt(0);
    emit newVocable(currentVocable());
}

void Backend::setCurrentCategory(CategoryPtr arg)
{
    if (m_currentCategory != arg) {
        if (!m_currentCategory.isNull())
            m_currentCategory->save();
        m_currentCategory = arg;
        if (!arg.isNull()){
            //vocListModel = VocableListPtr(new VocableList(m_currentCategory));
            vocListModel = QSharedPointer<QSqlQueryModel>(new QSqlQueryModel(this));
            vocabularyListQuery->bindValue(":cat", m_currentCategory->id.get().toInt() );
            refreshVocListTable();
            vocListModel->setHeaderData(0, Qt::Horizontal, m_currentCategory->languageFrom());
            vocListModel->setHeaderData(1, Qt::Horizontal, m_currentCategory->languageTo());
            vocListModel->setHeaderData(2, Qt::Horizontal, tr("Lektion"));
        }
        emit currentCategoryChanged(arg);
    }
}

void Backend::dbLoaded()
{
    emit categoriesUpdated(categories());
}

void Backend::fetchCategories()
{
    _categories.clear();
    DQList<Category> allCategories = Category::objects().all();
    for(int i=0;i<allCategories.size();i++) {
        CategoryPtr item(allCategories.at(i));
        _categories.append(item);
    }
    allCategories.detach();
}
