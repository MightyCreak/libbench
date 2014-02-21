/*
 * libbenchwindow.h
 *
 *  Created on: 2013-10-13
 *      Author: creak
 */

#ifndef LIBBENCHWINDOW_H_
#define LIBBENCHWINDOW_H_

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/actiongroup.h>
#include "benchmarkarea.h"
#include "libbench/benchmark.h"

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
	BenchMarkArea m_area;

	Glib::RefPtr<Gtk::UIManager> m_refUIManager;
	Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

	libbench::BenchMark m_benchmark;
};

#endif /* LIBBENCHWINDOW_H_ */
