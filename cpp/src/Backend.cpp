#include "Backend.h"

Backend::Backend(QObject *parent) :
    QObject(parent),
    cacheIsDirty(true)
{
    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

Backend::~Backend()
{
    currentCategory()->save();
}

void Backend::delayedInit()
{
    dbInstance = new DB();
    connect(dbInstance, &DB::dbLoaded, this, &Backend::dbLoaded);
    if (dbInstance->isLoaded())
        dbLoaded(); // signal is emitted b4 we reach that line...
    prepareTrainingSet();
}

void Backend::addCategory(QString languageFrom,QString languageTo, QString layout1, QString layout2)
{
    CategoryPtr cat(new Category(languageFrom,languageTo));
    cat->setKeyboardLayoutFrom(layout1);
    cat->setKeyboardLayoutTo(layout2);
    if (cat->save(false,false))
    {
        cacheIsDirty = true;
        emit categoriesUpdated(categories());
    }
}

void Backend::removeCategory(CategoryPtr &cat)
{
    if (cat->remove())
    {
        cacheIsDirty=true;
        emit categoriesUpdated(categories());
    }
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

void Backend::addVocable(QString lang1, QString lang2)
{
    bool ok = currentCategory()->addVocable(lang1, lang2);
    if (ok && !vocListModel.isNull())
    {
        vocListModel->refreshCache();
    }
}

QAbstractItemModel *Backend::currentVocabularyModel()const
{
    return vocListModel.data();
}

void Backend::prepareTrainingSet()
{
    DQSharedQuery qry(Vocable::objects());
    //trainingSet = qry.filter( DQWhere("lastAsked - CURRENT_TIMESTAMP") > "rightInRow" ).all();
    trainingSet = qry.all();
    qDebug() << "query " << qry.lastQuery().lastError();
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
            vocListModel = VocableListPtr(new VocableList(m_currentCategory));
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
