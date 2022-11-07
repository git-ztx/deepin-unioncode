/*
 * Copyright (C) 2022 Uniontech Software Technology Co., Ltd.
 *
 * Author:     luzhen<luzhen@uniontech.com>
 *
 * Maintainer: zhengyouge<zhengyouge@uniontech.com>
 *             luzhen<huangyub@uniontech.com>
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
#ifndef REVERSEDEBUGPLUGIN_H
#define REVERSEDEBUGPLUGIN_H

#include <framework/framework.h>

namespace dpfservice {
class WindowService;
}

namespace ReverseDebugger {
namespace Internal {
class ReverseDebuggerMgr;
}
}

class ReverseDebugPlugin : public dpf::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.deepin.plugin.unioncode" FILE "reversedebug.json")
public:
    virtual void initialize() override;
    virtual bool start() override;
    virtual dpf::Plugin::ShutdownFlag stop() override;

private:
    dpfservice::WindowService *windowService = nullptr;
    ReverseDebugger::Internal::ReverseDebuggerMgr *reverseDebug = nullptr;
};

#endif   // REVERSEDEBUGPLUGIN_H