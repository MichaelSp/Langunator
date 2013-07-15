#pragma once

#include <QObject>
#include "db.h"
#include "category.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CategoryPtr currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)
public:
    explicit Backend(QObject *parent = 0);

    void addCategory(QString languageFrom, QString languageTo);
    void removeCategory(CategoryPtr &cat);

    const CategoriesPtr &categories();
    CategoryPtr currentCategory() const;

public slots:
    void setCurrentCategory(CategoryPtr arg);

signals:
    void categoriesUpdated(CategoriesPtr);
    void currentCategoryChanged(CategoryPtr newCurrentCategory);

private slots:
    void dbLoaded();
    void delayedInit();

private:
    void fetchCategories();
    DB *dbInstance;
    CategoriesPtr _categories;
    CategoryPtr m_currentCategory;
    bool cacheIsDirty;
};

