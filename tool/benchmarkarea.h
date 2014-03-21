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

#ifndef TOOL_BENCHMARKAREA_H
#define TOOL_BENCHMARKAREA_H

#include <gtkmm/drawingarea.h>
#include <cairomm/context.h>
#include "drawcore.h"
#include "bench/xmlcommon.h"

namespace bench
{
    class BenchMark;
}

class BenchMarkArea : public Gtk::DrawingArea
{
public:
    BenchMarkArea();

    void SetBenchMark(bench::Document const* document);

protected:
    // Override default signal handler.
    virtual bool on_draw(Cairo::RefPtr<Cairo::Context> const& cr) override;
    virtual bool on_scroll_event(GdkEventScroll* event) override;

private:
    // Create draw structures.
    DrawCore* CreateCore(bench::DocumentCore const& docCore) const;
    DrawThread* CreateThread(bench::DocumentThread const& docThread) const;
    DrawBench* CreateBenchRec(bench::DocumentBench const& docBench, DrawBench* drawParent) const;

    // Compute data for UI.
    void ComputeUiData();
    void ResizeWidget();

private:
    DrawCoreVector m_drawCores;
    bench::Document const* m_document;

    double m_timeScale;
    Glib::RefPtr<Pango::Layout> m_layout;
};

#endif // TOOL_BENCHMARKAREA_H
