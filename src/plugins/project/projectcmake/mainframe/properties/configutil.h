/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     zhouyi<zhouyi1@uniontech.com>
 *
 * Maintainer: zhouyi<zhouyi1@uniontech.com>
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

#ifndef CONFIGUTIL_H
#define CONFIGUTIL_H

#include "services/project/projectservice.h"

#include <QObject>
#include <QVector>
#include <QMap>

namespace config {

enum ConfigType {
    Unknown = 0,
    Debug,
    Release
};

enum StepType {
    Build = 0,
    Clean,
    StepCount
};

struct StepItem {
    StepType type = Build;
    QString targetName;
    QStringList targetList;
    QString arguments;

    friend QDataStream &operator<<(QDataStream &stream, const StepItem &data)
    {
        stream << data.type;
        stream << data.targetName;
        stream << data.targetList;
        stream << data.arguments;

        return stream;
    }

    friend QDataStream &operator>>(QDataStream &stream, StepItem &data)
    {
        int type = 0;
        stream >> type;
        data.type = static_cast<StepType>(type);
        stream >> data.targetName;
        stream >> data.targetList;
        stream >> data.arguments;

        return stream;
    }
};

struct EnvironmentItem {
    bool enable;
    QMap<QString, QString> environments;

    friend QDataStream &operator<<(QDataStream &stream, const EnvironmentItem &data)
    {
        stream << data.enable;
        stream << data.environments;

        return stream;
    }

    friend QDataStream &operator>>(QDataStream &stream, EnvironmentItem &data)
    {
        stream >> data.enable;
        stream >> data.environments;

        return stream;
    }
};

struct RunParam {
    QString targetName;
    QString targetPath;
    QString arguments;
    QString workDirectory;
    EnvironmentItem env;

    friend QDataStream &operator<<(QDataStream &stream, const RunParam &data)
    {
        stream << data.targetName;
        stream << data.targetPath;
        stream << data.arguments;
        stream << data.workDirectory;
        stream << data.env;

        return stream;
    }

    friend QDataStream &operator>>(QDataStream &stream, RunParam &data)
    {
        stream >> data.targetName;
        stream >> data.targetPath;
        stream >> data.arguments;
        stream >> data.workDirectory;
        stream >> data.env;

        return stream;
    }
};

struct RunConfigure {
    QString defaultTargetName;
    QVector<RunParam> params;

    friend QDataStream &operator<<(QDataStream &stream, const RunConfigure &data)
    {
        stream << data.defaultTargetName;
        stream << data.params;

        return stream;
    }

    friend QDataStream &operator>>(QDataStream &stream, RunConfigure &data)
    {
        stream >> data.defaultTargetName;
        stream >> data.params;

        return stream;
    }
};

struct BuildConfigure {
    ConfigType type = Unknown;
    QString directory;
    QVector<StepItem> steps;
    EnvironmentItem env;
    RunConfigure runConfigure;

    friend QDataStream &operator<<(QDataStream &stream, const BuildConfigure &data)
    {
        stream << data.type;
        stream << data.directory;
        stream << data.steps;
        stream << data.env;
        stream << data.runConfigure;

        return stream;
    }

    friend QDataStream &operator>>(QDataStream &stream, BuildConfigure &data)
    {
        int type = 0;
        stream >> type;
        data.type = static_cast<ConfigType>(type);
        stream >> data.directory;
        stream >> data.steps;
        stream >> data.env;
        stream >> data.runConfigure;

        return stream;
    }
};

struct ConfigureParam {
    QString kit;
    QString language;
    QString projectPath;
    ConfigType defaultType = Unknown;
    ConfigType tempSelType = Unknown;
    QVector<BuildConfigure> buildConfigures;

    friend QDataStream &operator<<(QDataStream &stream, const ConfigureParam &data)
    {
        stream << data.kit;
        stream << data.language;
        stream << data.projectPath;
        int type = data.defaultType;
        stream << type;
        stream << data.buildConfigures;

        return stream;
    }

    friend QDataStream &operator>>(QDataStream &stream, ConfigureParam &data)
    {
        stream >> data.kit;
        stream >> data.language;
        stream >> data.projectPath;
        int type = 0;
        stream >> type;
        data.defaultType = static_cast<ConfigType>(type);
        stream >> data.buildConfigures;

        return stream;
    }

    void clear()
    {
        kit.clear();
        language.clear();
        projectPath.clear();
        defaultType = Unknown;
        tempSelType = Unknown;
        buildConfigures.clear();
    }
};

class ConfigUtilPrivate;
class ConfigUtil final: public QObject
{
    Q_OBJECT
public:
    static ConfigUtil *instance();

    ConfigUtil(const ConfigUtil &) = delete;
    ConfigUtil &operator=(const ConfigUtil &) = delete;

    QString getConfigPath(const QString &projectPath);

    ConfigureParam *getConfigureParamPointer();

    QString getNameFromType(ConfigType type);
    ConfigType getTypeFromName(QString name);

    bool getProjectInfo(const ConfigureParam *param, dpfservice::ProjectInfo &info);
    bool isNeedConfig(const QString &projectPath, ConfigureParam &param);
    void checkConfigInfo(const QString &buildType, const QString &directory);
    void configProject(const ConfigureParam *param);

    void readConfig(const QString &filePath, ConfigureParam &param);
    void saveConfig(const QString &filePath, const ConfigureParam &param);
signals:
    void configureDone(const dpfservice::ProjectInfo &info);

private:
    explicit ConfigUtil(QObject *parent = nullptr);
    ~ConfigUtil();

    ConfigUtilPrivate *const d;
};

} //namespace config

#endif // CONFIGUTIL_H
