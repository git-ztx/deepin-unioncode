/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     luzhen<luzhen@uniontech.com>
 *
 * Maintainer: zhengyouge<zhengyouge@uniontech.com>
 *             luzhen<luzhen@uniontech.com>
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

#include "serverproxy.h"
#include "setting.h"
#include "log.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDir>
#include <QDialog>
#include <QMessageBox>

#include <iostream>

namespace OptionNames {
const QString port {"prot"};
const QString session {"session"};
const QString language {"language"};
const QString generate {"generate"};
};

namespace OptionValNames {
const QString mode {"mode"};
const QString path {"path"};
};

namespace OptionValues {
const QString single {"single"};
const QString many {"many"};
const QString cxx = {"cxx"};
const QString java = {"java"};
const QString python = {"python"};
};

namespace DefaultValues {
const QString port {"3307"};
}

// 默认为
const QList<QCommandLineOption> options
{
    {OptionNames::port, "Server open port, default 3307.\n", "number", DefaultValues::port},
    {OptionNames::session, "[" + OptionValues::single + "]" + " Use this program just as you would with Clangd.\n" +
                "[" + OptionValues::many + "]" + " Start multiple backend at the same time.\n",
                OptionValNames::mode,
                OptionValues::many
    },
    {OptionNames::language , "with session mode " + OptionValues::single + " to used,\n" +
                "   [" + OptionValues::cxx + "]" +" lauch C++ language lsp server.\n" +
                "   [" + OptionValues::java + "]" +" lauch Java language lsp server.\n" +
                "   [" + OptionValues::python + "]" + " lauch Python language lsp server.\n"
    },
    {OptionNames::generate, "generate config file to path.\n", OptionValNames::path}
};

void implAllLSPServer()
{

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Log log;
    QCommandLineParser parser;

    parser.addOptions(options);
    parser.addHelpOption();
    parser.process(a);

    auto list = parser.optionNames();
    if (list.size() != 1 && list.contains(OptionNames::generate)) {
        std::cerr << "Failed, Command generate cannot be mixed with other parameters"
                  << std::endl;
        return -1;
    } else {
        auto genPath = parser.value(OptionNames::generate);
        if (!genPath.isEmpty()) {
            return Setting::genConfigFile(genPath);
        }
    }

    qInfo() << Setting::getInfo("C/C++");
    qInfo() << Setting::getInfo("Java");

    quint16 port = DefaultValues::port.toUShort();
    if (list.contains(OptionNames::port)) {
        port = parser.value(OptionNames::port).toUShort();
    }

    jsonrpc::TcpSocketServer server("127.0.0.1", port);
    ServerProxy proxy(server);
    if (proxy.StartListening()) {
        std::cout << "Server started successfully" << std::endl;
        QObject::connect(qApp, &QCoreApplication::aboutToQuit, [&proxy](){
            proxy.StopListening();
        });
    } else {
        QMessageBox messBox;
        messBox.setText(QString("There may be port occupation from %0, "
                        "please check whether the "
                        "languageadapter has been started").arg(port));
        messBox.exec();
        proxy.StopListening();
        std::cout << "Error starting Server" << std::endl;
        exit(-1);
    }

    return a.exec();
}
