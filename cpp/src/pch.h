#ifndef PCH_H
#define PCH_H

#include <Qt>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#include <QWebFrame>
#include <QtConcurrent/QtConcurrent>

#else
#include <QtWebKit/QWebFrame>
#include <QtConcurrentRun>

#endif

#include <QtGui>
#include <QtSql>

#include <dquest.h>
#include "dqmodel.h"

#include <QtDebug>

#endif // PCH_H
