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

#include <iostream>
#include <gtkmm/aboutdialog.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/uimanager.h>
#include "libbenchwindow.h"
#include "bench/xmlreader.h"

LibbenchWindow::LibbenchWindow()
    : m_box(Gtk::ORIENTATION_VERTICAL)
    , m_timeline(this)
    , m_benchArea(this)
    , m_timeScale(1e3) // 1s -> 1000px
{
    set_title("LibBench UI");
    set_default_size(600, 400);

    // Create actions for menus and toolbars.
    m_refActionGroup = Gtk::ActionGroup::create();

    // File menu.
    m_refActionGroup->add(Gtk::Action::create("FileMenu", Gtk::Stock::FILE));

    // File sub menu.
    m_refActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
                          sigc::mem_fun(*this, &LibbenchWindow::OnMenuFileOpen));
    m_refActionGroup->add(Gtk::Action::create("FileClose", Gtk::Stock::CLOSE),
                          sigc::mem_fun(*this, &LibbenchWindow::OnMenuFileClose));
    m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
                          sigc::mem_fun(*this, &LibbenchWindow::OnMenuFileQuit));

    // Help menu.
    m_refActionGroup->add(Gtk::Action::create("HelpMenu", Gtk::Stock::HELP));

    // Help sub menu.
    m_refActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT),
                          sigc::mem_fun(*this, &LibbenchWindow::OnMenuHelpAbout));

    m_refUIManager = Gtk::UIManager::create();
    m_refUIManager->insert_action_group(m_refActionGroup);
    add_accel_group(m_refUIManager->get_accel_group());

    // Layout the actions in a menubar and toolbar.
    Glib::ustring ui_info =
            "<ui>"
            "  <menubar name='MenuBar'>"
            "    <menu action='FileMenu'>"
            "      <menuitem action='FileOpen'/>"
            "      <menuitem action='FileClose'/>"
            "      <separator/>"
            "      <menuitem action='FileQuit'/>"
            "    </menu>"
            "    <menu action='HelpMenu'>"
            "      <menuitem action='HelpAbout'/>"
            "    </menu>"
            "  </menubar>"
            "  <toolbar name='ToolBar'>"
            "    <toolitem action='FileOpen'/>"
            "    <toolitem action='FileClose'/>"
            "  </toolbar>"
            "</ui>";

    try
    {
        m_refUIManager->add_ui_from_string(ui_info);
    }
    catch(const Glib::Error& ex)
    {
        std::cerr << "building menus failed: " << ex.what();
    }

    m_benchArea.signal_scroll_event().connect(sigc::mem_fun(*this, &LibbenchWindow::OnAreaScrollEvent));
    m_scrollwnd.get_hadjustment()->signal_value_changed().connect(sigc::mem_fun(*this, &LibbenchWindow::OnHScrollValueChanged));
    m_scrollwnd.get_vadjustment()->signal_value_changed().connect(sigc::mem_fun(*this, &LibbenchWindow::OnVScrollValueChanged));

    // Get the menubar and toolbar widgets, and add them to a container widget.
    Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
    if(pMenubar)
        m_box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

    Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar");
    if(pToolbar)
        m_box.pack_start(*pToolbar, Gtk::PACK_SHRINK);

    // Add the drawing area.
    m_box.pack_start(m_timeline, Gtk::PACK_SHRINK);
    m_scrollwnd.add(m_benchArea);
    m_box.pack_start(m_scrollwnd, Gtk::PACK_EXPAND_WIDGET);
    add(m_box);

    // The final step is to display all the widgets.
    show_all_children();
}

LibbenchWindow::~LibbenchWindow()
{
}

bool LibbenchWindow::IsFileOpened() const
{
    return !m_filepath.empty();
}

void LibbenchWindow::SetTimeScale(double scale)
{
    if(scale > 1.0e6)
        scale = 1.0e6;
    else if(scale < 1.0)
        scale = 1.0;
    if(m_timeScale != scale)
    {
        m_timeScale = scale;
        m_timeline.SetTimeStart(m_scrollwnd.get_hadjustment()->get_value() / GetTimeScale());
        m_timeline.queue_draw();
        m_benchArea.ComputeSize();
        m_benchArea.queue_draw();
    }
}

double LibbenchWindow::GetTimeScale() const
{
    return m_timeScale;
}

void LibbenchWindow::OnMenuFileOpen()
{
    Gtk::FileChooserDialog dialog("Please choose a file",
                                  Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    // Add response buttons the the dialog.
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    int result = dialog.run();
    if(result == Gtk::RESPONSE_OK)
        OpenFile(dialog.get_filename());
}

void LibbenchWindow::OnMenuFileClose()
{
    m_filepath.clear();
    m_benchArea.SetBenchMark(nullptr);
    m_timeline.queue_draw();
    m_benchArea.queue_draw();
}

void LibbenchWindow::OnMenuFileQuit()
{
    hide();
}

void LibbenchWindow::OnMenuHelpAbout()
{
    Gtk::AboutDialog dialog;
    dialog.set_transient_for(*this);
    dialog.set_program_name("LibBench UI");
    dialog.set_version("0.0.1");
    dialog.set_copyright("Romain \"Creak\" Failliot");
    dialog.set_comments("This is just an example application.");
    dialog.set_license("LGPL v3");
    dialog.set_website("https://github.com/MightyCreak/libbench");
    dialog.set_website_label("LibBench UI website");
    std::vector<Glib::ustring> list_authors;
    list_authors.push_back("Romain \"Creak\" Failliot");
    dialog.set_authors(list_authors);
    dialog.run();
}

bool LibbenchWindow::OnAreaScrollEvent(GdkEventScroll *event)
{
    if(event->state & GDK_CONTROL_MASK)
    {
        switch(event->direction)
        {
        case GDK_SCROLL_UP:
            SetTimeScale(GetTimeScale() * 1.5);
            return true;

        case GDK_SCROLL_DOWN:
            SetTimeScale(GetTimeScale() / 1.5);
            return true;

        default:
            break;
        }
    }

    return false;
}

void LibbenchWindow::OnHScrollValueChanged()
{
    double startTime = m_scrollwnd.get_hadjustment()->get_value() / GetTimeScale();
    m_benchArea.SetStartTime(startTime);
    m_timeline.SetTimeStart(startTime);
    m_timeline.queue_draw();
    m_benchArea.queue_draw();
}

void LibbenchWindow::OnVScrollValueChanged()
{
    m_benchArea.queue_draw();
}

void LibbenchWindow::OpenFile(std::string const& filename)
{
    m_filepath = filename;

    bench::XmlReader xmlReader;
    bench::Document document;
    xmlReader.Read(filename.c_str(), document);

    m_benchArea.SetBenchMark(&document);
    m_timeline.SetTimeStart(m_scrollwnd.get_hadjustment()->get_value() / GetTimeScale());
    m_timeline.queue_draw();
    m_benchArea.queue_draw();
}
