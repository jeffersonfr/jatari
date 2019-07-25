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

    void line(jgui::jpoint_t<int> p0, jgui::jpoint_t<int> p1)
    {
      _raster.DrawLine(p0, p1);
    }

    void rect(jgui::jrect_t<int> rect)
    {
      if (_fill == false) {
        _raster.DrawRectangle(rect);
      } else {
        _raster.FillRectangle(rect);
      }
    }

    void arc(jgui::jpoint_t<int> point, int radius, float ang0, float ang1)
    {
      if (_fill == false) {
        _raster.DrawArc(point, {radius, radius}, (float)ang0, (float)ang1);
      } else {
        _raster.FillArc(point, {radius, radius}, (float)ang0, (float)ang1);
      }
    }

    void palette(uint32_t *pal)
    {
      _palette = pal;
    }

    void pixel(jgui::jpoint_t<int> point)
    {
      _raster.SetPixel(point);
    }

    void color(uint8_t index)
    {
      _raster.SetColor(_palette[index]);
    }

    void blit(jatari_blit_t blit)
    {
      _blit = blit;
    }

    void sprite(uint8_t obj[], jgui::jrect_t<int> region)
    {
      if (_blit == BLIT_ROTATE_90) {
        for (int j=0; j<region.size.width; j++) {
          for (int i=0; i<region.size.height; i++) {
            int c = obj[i*region.size.width + (region.size.width - j - 1)];

            if (c != 0) {
              color(c);
              pixel({region.point.x + i, region.point.y + j});
            }
          }
        }
      } else if (_blit == BLIT_ROTATE_180) {
        for (int j=0; j<region.size.height; j++) {
          for (int i=0; i<region.size.width; i++) {
            int c = obj[(region.size.height - j - 1)*region.size.width + (region.size.width - i - 1)];

            if (c != 0) {
              color(c);
              pixel({region.point.x + i, region.point.y + j});
            }
          }
        }
      } else if (_blit == BLIT_ROTATE_270) {
        for (int j=0; j<region.size.width; j++) {
          for (int i=0; i<region.size.height; i++) {
            int c = obj[(region.size.height - i - 1)*region.size.width + j];

            if (c != 0) {
              color(c);
              pixel({region.point.x + i, region.point.y + j});
            }
          }
        }
      } else {
        for (int j=0; j<region.size.height; j++) {
          for (int i=0; i<region.size.width; i++) {
            int k = j*region.size.width + i;

            if (_blit == BLIT_FLIP_HORIZONTAL) {
              k = j*region.size.width + (region.size.width - i - 1);
            } else if (_blit == BLIT_FLIP_VERTICAL) {
              k = (region.size.height - j - 1)*region.size.width + i;
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
    
    void sprite(uint8_t obj[], jgui::jrect_t<int> region, float angle)
    {
      angle = fmod(angle, 2*M_PI);

      int 
        precision = 1024,
        sinTheta = (int)(precision*sin(angle)),
        cosTheta = (int)(precision*cos(angle)),
        iw = (abs(region.size.width*cosTheta)+abs(region.size.height*sinTheta))/precision,
        ih = (abs(region.size.width*sinTheta)+abs(region.size.height*cosTheta))/precision;
      int 
        sxc = region.size.width/2,
        syc = region.size.height/2,
        dxc = iw/2,
        dyc = ih/2,
        xo, yo, t1, t2;

      for (int j=0; j<ih; j++) {
        t1 = (j - dyc)*sinTheta;
        t2 = (j - dyc)*cosTheta;

        for (int i=0; i<iw; i++) {
          xo = ((i - dxc)*cosTheta - t1)/precision;
          yo = ((i - dxc)*sinTheta + t2)/precision;

          if (xo >= -sxc && xo <= sxc && yo >= -syc && yo <= syc) {
            color(obj[(yo + syc)*region.size.width + (xo + sxc)]);
            pixel({i + region.point.x, j + region.point.y});
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
      jgui::Window(0, 0, 640, 480)
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
        return (ev->IsKeyDown(jevent::JKS_SPACE));
      }

      return false;
    }

    bool collide(jgui::jrect_t<int> r1, jgui::jrect_t<int> r2)
    {
      if ((r1.point.x + r1.size.width) >= r2.point.x && r1.point.x <= (r2.point.x + r2.size.width) && (r1.point.y + r1.size.height) >= r2.point.y && r1.point.y <= (r2.point.y + r2.size.height)) {
        return true;
      }

      return false;
    }

};

