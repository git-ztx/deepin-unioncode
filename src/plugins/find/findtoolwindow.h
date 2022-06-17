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
#ifndef FINDTOOLWINDOW_H
#define FINDTOOLWINDOW_H

#include "searchresultwindow.h"

#include <QWidget>

class FindToolWindowPrivate;
class FindToolWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FindToolWindow(QWidget *parent = nullptr);

signals:

private:
    void setupUi();
    void search();
    void searchText();
    void replace();
    void addSearchParamWidget(QWidget *parentWidget);
    void addSearchResultWidget(QWidget *parentWidget);
    void switchSearchParamWidget();
    bool checkSelectedScopeValid();
    bool getSearchParams(SearchParams *searchParams);

    FindToolWindowPrivate *const d;
};

#endif // FINDTOOLWINDOW_H