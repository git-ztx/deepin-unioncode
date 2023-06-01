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
#include "consolewidget.h"
#include <QDebug>
#include <QMenu>
static ConsoleWidget *ins{nullptr};

class ConsoleWidgetPrivate
{
public:
    QMenu *menu = nullptr;
    QAction *consoleCopy = nullptr;
    QAction *consolePaste = nullptr;
};

ConsoleWidget *ConsoleWidget::instance()
{
    if (!ins)
        ins = new ConsoleWidget;
    return ins;
}

ConsoleWidget::ConsoleWidget(QWidget *parent)
    : QTermWidget(parent),
     d(new ConsoleWidgetPrivate())
{
    setMargin(0);
    setTerminalOpacity(0);
    setForegroundRole(QPalette::ColorRole::Window);
    setAutoFillBackground(true);
    if (availableColorSchemes().contains("Linux"))
        setColorScheme("Linux");
    if (availableKeyBindings().contains("linux"))
        setKeyBindings("linux");
    setScrollBarPosition(QTermWidget::ScrollBarRight);
    setTerminalSizeHint(false);
    setAutoClose(false);
    changeDir(QDir::homePath());
    sendText("clear\n");

    d->consoleCopy = new QAction(tr("copy"));
    d->consolePaste = new QAction(tr("paste"));
    QObject::connect(d->consoleCopy, &QAction::triggered, this, &QTermWidget::copyClipboard);
    QObject::connect(d->consolePaste, &QAction::triggered, this, &QTermWidget::pasteClipboard);
}

ConsoleWidget::~ConsoleWidget()
{
    qInfo() << __FUNCTION__;
}

void ConsoleWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (nullptr == d->menu) {
        d->menu = new QMenu(this);
        d->menu->setParent(this);
        d->menu->addAction(d->consoleCopy);
        d->menu->addAction(d->consolePaste);
    }
    if (selectedText().isEmpty()) {
        d->consoleCopy->setEnabled(false);
    } else {
        d->consoleCopy->setEnabled(true);
    }
    d->menu->exec(event->globalPos());
}
