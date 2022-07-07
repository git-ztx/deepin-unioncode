/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     huangyu<huangyub@uniontech.com>
 *
 * Maintainer: huangyu<huangyub@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "language.h"
#include "util/custompaths.h"
#include "dialog/contextdialog.h"

#include <QJsonDocument>
#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QMimeDatabase>

namespace support_file {

namespace documents {
inline static QJsonDocument languageGlobal;
inline static QJsonDocument languageUser;
bool languageIsLoaded();
} //namespace documents

bool documents::languageIsLoaded()
{
    return !languageGlobal.isEmpty() && !languageUser.isEmpty();
}

QString Language::globalPath()
{
    auto result = CustomPaths::endSeparator(CustomPaths::global(CustomPaths::Configures));
    return result + "language.support";
}

QString Language::userPath()
{
    auto result = CustomPaths::endSeparator(CustomPaths::user(CustomPaths::Configures));
    return result + "language.support";
}

void Language::initialize()
{
    if (!documents::languageGlobal.isEmpty()) {
        return;
    }

    QJsonParseError err;
    QFile file(globalPath());
    if (file.open(QFile::ReadOnly)) {
        auto data = file.readAll();
        documents::languageGlobal = QJsonDocument::fromJson(data, &err);
        file.close();
    }

    if (documents::languageGlobal.isEmpty()) {
        ContextDialog::ok(QObject::tr("The format of the language configuration file is incorrect or damaged. "
                                      "Check that the file is released correctly. "
                                      "If it cannot be solved, reinstall the software to solve the problem"));
        qCritical() << QString("Failed, %0 jsonDoc is Empty. ").arg(globalPath())
                       + "errorString: " + err.errorString();
        abort();
    }
}

bool Language::recovery()
{
    return true;
}

QStringList Language::ids()
{
    Language::initialize();
    return documents::languageGlobal.object().keys();
}

QString Language::id(const QString &filePath)
{
    Language::initialize();
    auto jsonObj = documents::languageGlobal.object();
    QFileInfo info(filePath);
    for (auto id : ids()) {
        auto langObjChild = jsonObj.value(id).toObject();
        QJsonArray suffixArray = langObjChild.value(Key_2::get()->suffix).toArray();
        QJsonArray baseArray = langObjChild.value(Key_2::get()->base).toArray();
        QJsonArray mimeArray = langObjChild.value(Key_2::get()->mimeType).toArray();

        for (auto suffix : suffixArray) {
            if (info.fileName().endsWith(suffix.toString()))
                return id;

            if (info.suffix() == suffix.toString())
                return id;
        }

        for (auto base : baseArray) {

            if (info.fileName() == base.toString()
                    || info.fileName().toLower() == base.toString().toLower()) {
                return id;
            }
        }

        QMimeDatabase mimeDB;
        for (auto mime : mimeArray) {
            if (mimeDB.mimeTypeForFile(filePath).name() == mime.toString()) {
                return id;
            }
        }
    }

    return "";
}

QMap<int, QString> Language::tokenWords(const QString &id)
{
    QMap<int, QString> result;
    auto idObj = documents::languageGlobal.object().value(id).toObject();
    auto tokenChildObj = idObj.value(Key_2::get()->tokenWords).toObject();

    for (auto key: tokenChildObj.keys()) {
        result[key.toInt()] = tokenChildObj.value(key).toString();
    }

    return result;
}

QSet<QString> Language::suffixs(const QString &id)
{
    QSet<QString> result;
    auto idObj = documents::languageGlobal.object().value(id).toObject();
    auto suffixArray = idObj.value(Key_2::get()->suffix).toArray();
    for (auto value : suffixArray) {
        result.insert(value.toString());
    }
    return result;
}

QSet<QString> Language::bases(const QString &id)
{
    QSet<QString> result;
    auto idObj = documents::languageGlobal.object().value(id).toObject();
    auto baseArray = idObj.value(Key_2::get()->base).toArray();
    for (auto value : baseArray) {
        result.insert(value.toString());
    }
    return result;
}

QSet<QString> Language::mimeTypes(const QString &id)
{
    QSet<QString> result;
    auto idObj = documents::languageGlobal.object().value(id).toObject();
    auto baseArray = idObj.value(Key_2::get()->mimeType).toArray();
    for (auto value : baseArray) {
        result.insert(value.toString());
    }
    return result;
}

} // namespace support_file
