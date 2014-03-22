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
#ifndef TOOL_TIMELINE_H
#define TOOL_TIMELINE_H

#include <gtkmm/drawingarea.h>

class LibbenchWindow;

class TimeLine : public Gtk::DrawingArea
{
public:
    TimeLine(LibbenchWindow *window);

    void SetTimeStart(double timeStart);
    double GetTimeStart() const;

protected:
    // Override default signal handler.
    virtual bool on_draw(Cairo::RefPtr<Cairo::Context> const& cr) override;

private:
    LibbenchWindow* m_mainWindow;
    double m_timeStart;

    Glib::RefPtr<Pango::Layout> m_layout;
    Pango::FontDescription m_font;
};

#endif // TOOL_TIMELINE_H
