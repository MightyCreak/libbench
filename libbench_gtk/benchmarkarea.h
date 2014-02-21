/*
 * benchmarkarea.h
 *
 *  Created on: 2013-10-13
 *      Author: creak
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
