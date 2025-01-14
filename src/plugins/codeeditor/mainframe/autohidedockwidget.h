// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AUTOHIDEDOCKWIDGET_H
#define AUTOHIDEDOCKWIDGET_H

#include <QDockWidget>

class AutoHideDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    AutoHideDockWidget(const QString &title, QWidget *parent = nullptr,
                       Qt::WindowFlags flags = Qt::WindowFlags());
    explicit AutoHideDockWidget(QWidget *parent = nullptr,
                                Qt::WindowFlags flags = Qt::WindowFlags());

    QWidget *widget() const;

    void setWidget(QWidget *widget);

protected:
    bool eventFilter(QObject *obj, QEvent *e) override;
};

#endif // AUTOHIDEDOCKWIDGET_H
