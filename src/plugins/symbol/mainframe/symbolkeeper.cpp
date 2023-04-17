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
#include "symbolkeeper.h"

#include "common/common.h"
#include "services/builder/builderservice.h"

#include <QStandardItem>

namespace {
static SymbolTreeView *tree{nullptr};
static SymbolParser *parser{nullptr};
static SymbolParseArgs parserArgs;
}

SymbolKeeper::SymbolKeeper()
{

}

SymbolKeeper *SymbolKeeper::instance()
{
    static SymbolKeeper ins;
    return &ins;
}

SymbolTreeView *SymbolKeeper::treeView()
{
    if (!tree) {
        tree = new SymbolTreeView();
        QObject::connect(tree, &SymbolTreeView::jumpToLine,
                         this, &SymbolKeeper::jumpToLine);
    }
    return tree;
}

void SymbolKeeper::doParse(const SymbolParseArgs &args)
{
    parserArgs = args;
    if (parser) {
        QObject::disconnect(parser, &SymbolParser::parseDone,
                            this, &SymbolKeeper::doParseDone);
        parser->kill();
        parser->waitForFinished();
        delete parser;
        parser = nullptr;
    }
    parser = new SymbolParser();
    parser->setArgs(args);
    parser->SymbolParser::start();
    QObject::connect(parser, &SymbolParser::parseDone,
                     this, &SymbolKeeper::doParseDone);
}

void SymbolKeeper::jumpToLine(const QString &filePath, const QString &fileLine)
{
    editor.jumpToLine(filePath, fileLine.toInt());
}

void SymbolKeeper::doParseDone(bool result)
{
    SymbolParseArgs args = parser->args();
    symbol.parseDone(args.workspace, args.language, args.storage, result);
    this->treeView()->expandAll();
    parser->kill();
    delete parser;
    parser = nullptr;
}
