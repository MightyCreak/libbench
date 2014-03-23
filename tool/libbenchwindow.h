/*
 * Copyright (C) 2014 Romain "Creak" Failliot.
 *
 * This file is part of libbench.
 *
 * libbench is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libbench is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libbench. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef TOOL_LIBBENCHWINDOW_H
#define TOOL_LIBBENCHWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include "benchmarkarea.h"
#include "timeline.h"
#include "bench/xmlcommon.h"

namespace Gtk
{
    class UIManager;
    class ActionGroup;
}

class LibbenchWindow : public Gtk::Window
{
public:
    LibbenchWindow();
    virtual ~LibbenchWindow();

    bool IsFileOpened() const;

    void SetTimeScale(double scale);
    double GetTimeScale() const;

private:
    // Signal handlers.
    void OnMenuFileOpen();
    void OnMenuFileClose();
    void OnMenuFileQuit();
    void OnMenuHelpAbout();
    bool OnAreaScrollEvent(GdkEventScroll* event);
    void OnHScrollValueChanged();
    void OnVScrollValueChanged();

    void OpenFile(std::string const& filename);

private:
    // Member widgets.
    Gtk::Box m_box;
    TimeLine m_timeline;
    Gtk::ScrolledWindow m_scrollwnd;
    BenchMarkArea m_benchArea;

    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

    Glib::ustring m_filepath;
    double m_timeScale;
};

#endif // TOOL_LIBBENCHWINDOW_H
