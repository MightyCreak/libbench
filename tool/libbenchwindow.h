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

#ifndef TOOL_LIBBENCHWINDOW_H
#define TOOL_LIBBENCHWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/scrolledwindow.h>
#include "benchmarkarea.h"
#include "bench/xmlcommon.h"

class LibBenchGtk : public Gtk::Window
{
public:
    LibBenchGtk();
    virtual ~LibBenchGtk();

private:
    // Signal handlers.
    void OnMenuFileOpen();
    void OnMenuFileClose();
    void OnMenuFileQuit();
    void OnMenuHelpAbout();

    void OpenFile(std::string const& filename);

private:
    // Member widgets.
    Gtk::Box m_box;
    Gtk::ScrolledWindow m_scrollwnd;
    BenchMarkArea m_area;

    Glib::RefPtr<Gtk::UIManager> m_refUIManager;
    Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

    bench::Document m_document;
};

#endif // TOOL_LIBBENCHWINDOW_H
