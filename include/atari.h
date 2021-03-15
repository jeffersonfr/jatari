/***************************************************************************
 *   Copyright (C) 2005 by Jeff Ferr                                       *
 *   root@sat                                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "jcanvas/core/japplication.h"
#include "jcanvas/core/jwindow.h"
#include "jcanvas/core/jbufferedimage.h"
#include "jcanvas/core/jraster.h"

#include <iostream>
#include <string>
#include <map>
#include <chrono>

static const int SW = 160;
static const int SH = 192;

struct jpalette_t {
  size_t size;
  uint32_t colors[];
} jatari_palette = {
  .size = 15,
  .colors = {
    0xff000000, // 00. black
    0xff00008f, // 01. blue
    0xff008f00, // 02. green
    0xff008f8f, // 03. cyan
    0xff8f0000, // 04. red
    0xff8f008f, // 05. magenta
    0xff8f8f00, // 06. brown
    0xffcfcfcf, // 07. light gray
    0xff8f8f8f, // 08. dark gray
    0xff0000ff, // 09. light blue
    0xff00ff00, // 10. light green
    0xff00ffff, // 11. light cyan
    0xffff0000, // 12. light red
    0xffff00ff, // 13. light magenta
    0xffffff00, // 14. yellow
    0xffffffff // 15. white
  }
};

enum jatari_key_t {
  KEY_UP = 0,
  KEY_DOWN,
  KEY_LEFT,
  KEY_RIGHT,
  KEY_ACTION,
  KEY_LAST
};

enum jatari_blit_t {
  BLIT_NONE,
  BLIT_FLIP_HORIZONTAL,
  BLIT_FLIP_VERTICAL,
  BLIT_ROTATE_90,
  BLIT_ROTATE_180,
  BLIT_ROTATE_270,
};

class context {

  private:
    jcanvas::Raster
      _raster;
    jpalette_t
      *_palette;
    jatari_blit_t
      _blit;
    bool 
      _fill;

  public:
    context(jcanvas::Graphics *g):
      _raster((uint32_t *)cairo_image_surface_get_data(g->GetCairoSurface()), jcanvas::jpoint_t<int>{SW, SH})
    {
      _fill = false;
      _palette = &jatari_palette;
    }

    virtual ~context()
    {
    }

    void fill(bool b)
    {
      _fill = b;
    }

    void line(jcanvas::jpoint_t<int> p0, jcanvas::jpoint_t<int> p1)
    {
      _raster.DrawLine(p0, p1);
    }

    void rect(jcanvas::jrect_t<int> rect)
    {
      if (_fill == false) {
        _raster.DrawRectangle(rect);
      } else {
        _raster.FillRectangle(rect);
      }
    }

    void arc(jcanvas::jpoint_t<int> point, int radius, float ang0, float ang1)
    {
      if (_fill == false) {
        _raster.DrawArc(point, {radius, radius}, (float)ang0, (float)ang1);
      } else {
        _raster.FillArc(point, {radius, radius}, (float)ang0, (float)ang1);
      }
    }

    void palette(jpalette_t *palette)
    {
      _palette = palette;
    }

    void pixel(jcanvas::jpoint_t<int> point)
    {
      _raster.SetPixel(point);
    }

    void color(uint8_t index)
    {
      if (index > _palette->size) {
        return;
      }

      _raster.SetColor(_palette->colors[index]);
    }

    void blit(jatari_blit_t blit)
    {
      _blit = blit;
    }

    void sprite(uint8_t obj[], jcanvas::jrect_t<int> region)
    {
      if (_blit == BLIT_ROTATE_90) {
        for (int j=0; j<region.size.x; j++) {
          for (int i=0; i<region.size.y; i++) {
            int c = obj[i*region.size.x + (region.size.x - j - 1)];

            if (c != 0) {
              color(c);
              pixel({region.point.x + i, region.point.y + j});
            }
          }
        }
      } else if (_blit == BLIT_ROTATE_180) {
        for (int j=0; j<region.size.y; j++) {
          for (int i=0; i<region.size.x; i++) {
            int c = obj[(region.size.y - j - 1)*region.size.x + (region.size.x - i - 1)];

            if (c != 0) {
              color(c);
              pixel({region.point.x + i, region.point.y + j});
            }
          }
        }
      } else if (_blit == BLIT_ROTATE_270) {
        for (int j=0; j<region.size.x; j++) {
          for (int i=0; i<region.size.y; i++) {
            int c = obj[(region.size.y - i - 1)*region.size.x + j];

            if (c != 0) {
              color(c);
              pixel({region.point.x + i, region.point.y + j});
            }
          }
        }
      } else {
        for (int j=0; j<region.size.y; j++) {
          for (int i=0; i<region.size.x; i++) {
            int k = j*region.size.x + i;

            if (_blit == BLIT_FLIP_HORIZONTAL) {
              k = j*region.size.x + (region.size.x - i - 1);
            } else if (_blit == BLIT_FLIP_VERTICAL) {
              k = (region.size.y - j - 1)*region.size.x + i;
            }

            int c = obj[k];

            if (c != 0) {
              color(c);
              pixel({region.point.x + i, region.point.y + j});
            }
          }
        }
      }
    }
    
    void sprite(uint8_t obj[], jcanvas::jrect_t<int> region, float angle)
    {
      float
        sinTheta = sinf(angle),
        cosTheta = cosf(angle);
      int 
        iw = region.size.x,
        ih = region.size.y;

      for (int j=0; j<ih; j++) {
        for (int i=0; i<iw; i++) {
					float
						px = i - iw/2.0f,
						py = j - ih/2.0f;
					float
						rx = px*cosTheta - py*sinTheta,
						ry = px*sinTheta + py*cosTheta;

          color(obj[j*iw + i]);
          pixel(jcanvas::jpoint_t<float>{roundf(region.point.x + rx + iw/2.0f), roundf(region.point.y - ry + ih/2.0f)});
        }
      }
    }

};

class Atari : public jcanvas::Window, public jcanvas::KeyListener {

  private:
    std::chrono::time_point<std::chrono::steady_clock> 
      _start_time = std::chrono::steady_clock::now();
    std::shared_ptr<jcanvas::Image>
      _screen;

  private:
    void Framerate(int fps)
    {
      static int index = 0;

      std::chrono::time_point<std::chrono::steady_clock> timestamp = _start_time + std::chrono::milliseconds(index++*(1000/fps));
      std::chrono::time_point<std::chrono::steady_clock> current = std::chrono::steady_clock::now();
      std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp - current);

      if (diff.count() < 0) {
        return;
      }

      std::this_thread::sleep_for(diff);
    }

    virtual void Paint(jcanvas::Graphics *g)
    {
      std::chrono::milliseconds 
        diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start_time);
      context 
        ctx(_screen->GetGraphics());

      loop(diff.count());
      draw(ctx);

      _screen->GetGraphics()->SetAntialias(jcanvas::jantialias_mode_t::None);

      std::shared_ptr<jcanvas::Image>
        scaled = _screen->Scale(GetSize());

      g->DrawImage(scaled, jcanvas::jpoint_t<int>{0, 0});

      Repaint();
      Framerate(25);
    }

  public:
    Atari():
      jcanvas::Window({640, 480})
    {
      _screen = std::make_shared<jcanvas::BufferedImage>(jcanvas::jpixelformat_t::RGB32, jcanvas::jpoint_t<int>{SW, SH});
    }

    virtual ~Atari()
    {
      _screen = nullptr;
    }

    void viewport(jcanvas::jpoint_t<int> size)
    {
      SetSize(size);
    }

    jcanvas::jpoint_t<int> size()
    {
      return {SW, SH};
    }

    virtual void loop(int64_t timestamp)
    {
    }

    virtual void draw(context &ctx)
    {
    }

    bool key(jatari_key_t k)
    {
      jcanvas::EventManager &ev = GetEventManager();

      if (k == KEY_LEFT) {
        return (ev.IsKeyDown(jcanvas::jkeyevent_symbol_t::CursorLeft));
      } else if (k == KEY_RIGHT) {
        return (ev.IsKeyDown(jcanvas::jkeyevent_symbol_t::CursorRight));
      } else if (k == KEY_UP) {
        return (ev.IsKeyDown(jcanvas::jkeyevent_symbol_t::CursorUp));
      } else if (k == KEY_DOWN) {
        return (ev.IsKeyDown(jcanvas::jkeyevent_symbol_t::CursorDown));
      } else if (k == KEY_ACTION) {
        return (ev.IsKeyDown(jcanvas::jkeyevent_symbol_t::Space));
      }

      return false;
    }

    bool collide(jcanvas::jrect_t<int> r1, jcanvas::jrect_t<int> r2)
    {
      if ((r1.point.x + r1.size.x) >= r2.point.x && r1.point.x <= (r2.point.x + r2.size.x) && (r1.point.y + r1.size.y) >= r2.point.y && r1.point.y <= (r2.point.y + r2.size.y)) {
        return true;
      }

      return false;
    }

};

