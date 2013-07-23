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

    void addCategory(QString languageFrom, QString languageTo, int layout1, int layout2);
    void removeCategory(CategoryPtr &cat);

    const CategoriesPtr &categories();
    CategoryPtr currentCategory() const;

    void addVocable(QString lang1, QString lang2, int lektion);
    void updateVocable(const QModelIndex &idx, QString lang1, QString lang2, int lektion);
    void removeVocable(const QModelIndex &idx);
    QAbstractItemModel *currentVocabularyModel() const;

    void prepareTrainingSet();
    Vocable *currentVocable();
    void showNextVocable();
    void refreshVocListTable();
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

    //VocableListPtr vocListModel;
    QSqlQuery *vocabularyListQuery;
    QSharedPointer<QSqlQueryModel> vocListModel;
    DQList<Vocable> trainingSet;
};

