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

#ifndef BENCHMARKAREA_H_
#define BENCHMARKAREA_H_

#include <gtkmm/drawingarea.h>

namespace libbench
{
	class BenchMark;
}

class BenchMarkArea : public Gtk::DrawingArea
{
public:
	BenchMarkArea();

	void SetBenchMark(libbench::BenchMark const* benchmark);

protected:
	// Override default signal handler.
	virtual bool on_draw(Cairo::RefPtr<Cairo::Context> const& cr) override;
	virtual bool on_scroll_event(GdkEventScroll* event) override;

    // Draw methods.
    void DrawCoreText(Cairo::RefPtr<Cairo::Context> const& cr,
                      int rectangle_x, int rectangle_y,
                      int rectangle_width, int rectangle_height,
                      Glib::ustring const& text);
    void DrawThreadName(Cairo::RefPtr<Cairo::Context> const& cr,
                        int rectangle_x, int rectangle_y,
                        int rectangle_width, int rectangle_height,
                        Glib::ustring const& text);

private:
    struct DrawBench
    {
        DrawBench* m_parent;
        double m_startTime;
        double m_stopTime;
        Glib::ustring m_name;
        std::vector<DrawBench> m_children;
    };

    struct DrawCore
    {
        Glib::ustring m_name;
        std::vector<DrawBench> m_benches;
    };

private:
    std::vector<DrawCore> m_drawCores;
	libbench::BenchMark const* m_benchmark;

	double m_timeToPx;
    Glib::RefPtr<Pango::Layout> m_coreLayout;
    Glib::RefPtr<Pango::Layout> m_threadLayout;
};

#endif /* BENCHMARKAREA_H_ */
