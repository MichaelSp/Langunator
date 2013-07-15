#include "Backend.h"

Backend::Backend(QObject *parent) :
    QObject(parent),
    cacheIsDirty(true)
{
    QTimer::singleShot(0, this, SLOT(delayedInit()));
}

void Backend::delayedInit()
{
    dbInstance = new DB();
    connect(dbInstance, &DB::dbLoaded, this, &Backend::dbLoaded);
    if (dbInstance->isLoaded())
        dbLoaded(); // signal is emitted b4 we reach that line...
}

void Backend::addCategory(QString languageFrom,QString languageTo)
{
    CategoryPtr cat(new Category(languageFrom,languageTo));
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

void Backend::setCurrentCategory(CategoryPtr arg)
{
    if (m_currentCategory != arg) {
        m_currentCategory = arg;
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
