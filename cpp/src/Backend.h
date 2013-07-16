#pragma once

#include <QObject>
#include "db.h"
#include "category.h"
#include "vocabel.h"
#include "VocableList.h"

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(CategoryPtr currentCategory READ currentCategory WRITE setCurrentCategory NOTIFY currentCategoryChanged)
public:
    explicit Backend(QObject *parent = 0);
    ~Backend();

    void addCategory(QString languageFrom, QString languageTo, QString layout1, QString layout2);
    void removeCategory(CategoryPtr &cat);

    const CategoriesPtr &categories();
    CategoryPtr currentCategory() const;

    void addVocable(QString lang1, QString lang2);
    QAbstractItemModel *currentVocabularyModel() const;

    void prepareTrainingSet();
    Vocable *currentVocable();
    void showNextVocable();
public slots:
    void setCurrentCategory(CategoryPtr arg);

signals:
    void categoriesUpdated(CategoriesPtr);
    void currentCategoryChanged(CategoryPtr newCurrentCategory);
    void newVocable(Vocable *);

private slots:
    void dbLoaded();
    void delayedInit();

private:
    void fetchCategories();
    DB *dbInstance;
    CategoriesPtr _categories;
    CategoryPtr m_currentCategory;
    bool cacheIsDirty;

    VocableListPtr vocListModel;
    DQList<Vocable> trainingSet;
};

