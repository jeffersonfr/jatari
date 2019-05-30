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
#include "jgui/japplication.h"
#include "jgui/jwindow.h"
#include "jgui/jbufferedimage.h"
#include "jgui/jraster.h"
#include "jmath/jmath.h"

#include <iostream>
#include <string>
#include <map>
#include <chrono>

static const int SW = 160;
static const int SH = 192;

uint32_t jpalette_t[] = {
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
  0xffffffff, // 15. white
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
    jgui::Raster
      _raster;
    uint32_t 
      *_palette;
    jatari_blit_t
      _blit;
    bool 
      _fill;

  public:
    context(jgui::Graphics *g):
      _raster((uint32_t *)cairo_image_surface_get_data(g->GetCairoSurface()), jgui::jsize_t<int>{SW, SH})
    {
      _fill = false;
      _palette = jpalette_t;
    }

    virtual ~context()
    {
    }

    void fill(bool b)
    {
      _fill = b;
    }

    void line(int x0, int y0, int x1, int y1)
    {
      _raster.DrawLine({{x0, y0}, {x1, y1}});
    }

    void rect(jgui::jregion_t<int> region)
    {
      if (_fill == false) {
        _raster.DrawRectangle(region);
      } else {
        _raster.FillRectangle(region);
      }
    }

    void arc(int x, int y, int rad, int ang0, int ang1)
    {
      if (_fill == false) {
        _raster.DrawArc({x, y}, {rad, rad}, (float)ang0, (float)ang1);
      } else {
        _raster.FillArc({x, y}, {rad, rad}, (float)ang0, (float)ang1);
      }
    }

    void palette(uint32_t *pal)
    {
      _palette = pal;
    }

    void pixel(int x, int y)
    {
      _raster.SetPixel({x, y});
    }

    void color(uint8_t index)
    {
      _raster.SetColor(_palette[index]);
    }

    void blit(jatari_blit_t blit)
    {
      _blit = blit;
    }

    void sprite(uint8_t obj[], jgui::jregion_t<int> region)
    {
      if (_blit == BLIT_ROTATE_90) {
        for (int j=0; j<region.width; j++) {
          for (int i=0; i<region.height; i++) {
            int c = obj[i*region.width + (region.width - j - 1)];

            if (c != 0) {
              color(c);
              pixel(region.x + i, region.y + j);
            }
          }
        }
      } else if (_blit == BLIT_ROTATE_180) {
        for (int j=0; j<region.height; j++) {
          for (int i=0; i<region.width; i++) {
            int c = obj[(region.height - j - 1)*region.width + (region.width - i - 1)];

            if (c != 0) {
              color(c);
              pixel(region.x + i, region.y + j);
            }
          }
        }
      } else if (_blit == BLIT_ROTATE_270) {
        for (int j=0; j<region.width; j++) {
          for (int i=0; i<region.height; i++) {
            int c = obj[(region.height - i - 1)*region.width + j];

            if (c != 0) {
              color(c);
              pixel(region.x + i, region.y + j);
            }
          }
        }
      } else {
        for (int j=0; j<region.height; j++) {
          for (int i=0; i<region.width; i++) {
            int k = j*region.width + i;

            if (_blit == BLIT_FLIP_HORIZONTAL) {
              k = j*region.width + (region.width - i - 1);
            } else if (_blit == BLIT_FLIP_VERTICAL) {
              k = (region.height - j - 1)*region.width + i;
            }

            int c = obj[k];

            if (c != 0) {
              color(c);
              pixel(region.x + i, region.y + j);
            }
          }
        }
      }
    }

};

class Atari : public jgui::Window {

  private:
    std::chrono::time_point<std::chrono::steady_clock> 
      _start_time = std::chrono::steady_clock::now();
    jgui::Image 
      *_screen;

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

    virtual void Paint(jgui::Graphics *g)
    {
      std::chrono::milliseconds 
        diff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _start_time);
      context 
        ctx(_screen->GetGraphics());

      loop(diff.count());
      draw(ctx);

      _screen->GetGraphics()->SetAntialias(jgui::JAM_NONE);

      jgui::Image 
        *scaled = _screen->Scale(GetSize());

      g->DrawImage(scaled, jgui::jpoint_t<int>{0, 0});

      delete scaled;

      Repaint();
      Framerate(25);
    }

  public:
    Atari():
      jgui::Window(0, 0, 720, 480)
    {
      _screen = new jgui::BufferedImage(jgui::JPF_RGB32, {SW, SH});
    }

    virtual ~Atari()
    {
      delete _screen;
      _screen = nullptr;
    }

    void viewport(jgui::jsize_t<int> size)
    {
      SetSize(size);
    }

    jgui::jsize_t<int> size()
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
      jgui::EventManager *ev = GetEventManager();

      if (k == KEY_LEFT) {
        return (ev->IsKeyDown(jevent::JKS_CURSOR_LEFT));
      } else if (k == KEY_RIGHT) {
        return (ev->IsKeyDown(jevent::JKS_CURSOR_RIGHT));
      } else if (k == KEY_UP) {
        return (ev->IsKeyDown(jevent::JKS_CURSOR_UP));
      } else if (k == KEY_DOWN) {
        return (ev->IsKeyDown(jevent::JKS_CURSOR_DOWN));
      } else if (k == KEY_ACTION) {
        return (ev->IsKeyDown(jevent::JKS_ENTER));
      }

      return false;
    }

    bool collide(jgui::jregion_t<int> r1, jgui::jregion_t<int> r2)
    {
      if ((r1.x + r1.width) >= r2.x && r1.x <= (r2.x + r2.width) && (r1.y + r1.height) >= r2.y && r1.y <= (r2.y + r2.height)) {
        return true;
      }

      return false;
    }

};


