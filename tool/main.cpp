/*
 * main.cpp
 *
 *  Created on: 2013-10-06
 *      Author: creak
 */

#include <iostream>
#include "libbenchwindow.h"
#include "bench/libbench.h"

int main(int argc, char ** argv)
{
    // Print libbench version.
    std::cout << "libbench version: " << libbench::GetMajorVersion() << "." << libbench::GetMinorVersion() << std::endl;

	Glib::RefPtr<Gtk::Application> app =
			Gtk::Application::create(argc, argv,
					"org.gtkmm.libbenchgtk");
	LibBenchGtk window;
	return app->run(window);
}
