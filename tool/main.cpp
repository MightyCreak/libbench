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
#include "libbenchwindow.h"
#include "bench/bench.h"

int main(int argc, char ** argv)
{
    // Print libbench version.
    std::cout << "libbench v" << bench::GetVersionString() << std::endl;

    Glib::RefPtr<Gtk::Application> app =
            Gtk::Application::create(argc, argv,
                                     "org.gtkmm.libbenchgtk");
    LibBenchGtk window;
    return app->run(window);
}
