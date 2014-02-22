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
#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/aboutdialog.h>
#include "libbenchwindow.h"
#include "bench/xmlreader.h"

LibBenchGtk::LibBenchGtk()
    : m_box(Gtk::ORIENTATION_VERTICAL)
{
    set_title("LibBench UI");
//    set_default_size(400, 200);

    add(m_box);

    // Create actions for menus and toolbars.
    m_refActionGroup = Gtk::ActionGroup::create();

    // File menu.
    m_refActionGroup->add(Gtk::Action::create("FileMenu", Gtk::Stock::FILE));

    // File sub menu.
    m_refActionGroup->add(Gtk::Action::create("FileOpen", Gtk::Stock::OPEN),
                          sigc::mem_fun(*this, &LibBenchGtk::OnMenuFileOpen));
    m_refActionGroup->add(Gtk::Action::create("FileClose", Gtk::Stock::CLOSE),
                          sigc::mem_fun(*this, &LibBenchGtk::OnMenuFileClose));
    m_refActionGroup->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
                          sigc::mem_fun(*this, &LibBenchGtk::OnMenuFileQuit));

    // Help menu.
    m_refActionGroup->add(Gtk::Action::create("HelpMenu", Gtk::Stock::HELP));

    // Help sub menu.
    m_refActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT),
                          sigc::mem_fun(*this, &LibBenchGtk::OnMenuHelpAbout));

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

    // Get the menubar and toolbar widgets, and add them to a container widget.
    Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
    if(pMenubar)
        m_box.pack_start(*pMenubar, Gtk::PACK_SHRINK);

    Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar");
    if(pToolbar)
        m_box.pack_start(*pToolbar, Gtk::PACK_SHRINK);

    // Add the drawing area.
    m_area.set_size_request(1000, 600);
    m_box.pack_start(m_area, Gtk::PACK_EXPAND_WIDGET);

    // The final step is to display all the widgets.
    show_all_children();
}

LibBenchGtk::~LibBenchGtk()
{
}

void LibBenchGtk::OnMenuFileOpen()
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

void LibBenchGtk::OnMenuFileClose()
{
//    m_benchmark.Clear();
    m_area.SetBenchMark(nullptr);
    m_area.get_window()->invalidate(true);
}

void LibBenchGtk::OnMenuFileQuit()
{
    hide();
}

void LibBenchGtk::OnMenuHelpAbout()
{
    Gtk::AboutDialog dialog;
    dialog.set_transient_for(*this);
    dialog.set_program_name("LibBench UI");
    dialog.set_version("0.0.1");
    dialog.set_copyright("Romain \"Creak\" Failliot");
    dialog.set_comments("This is just an example application.");
    dialog.set_license("LGPL v3");
    dialog.set_website("http://foolstep.com");
    dialog.set_website_label("LibBench UI website");
    std::vector<Glib::ustring> list_authors;
    list_authors.push_back("Romain \"Creak\" Failliot");
    dialog.set_authors(list_authors);
    dialog.run();
}

void LibBenchGtk::OpenFile(std::string const& filename)
{
//    m_document.Clear();

    bench::XmlReader xmlReader;
    xmlReader.Read(filename.c_str(), m_document);

    m_area.SetBenchMark(&m_document);
    m_area.get_window()->invalidate(true);
}
