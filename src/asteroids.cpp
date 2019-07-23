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
 *   59 Temple Place - Suite 330x00, Boston, MA  02111-1307, USA.          *
 ***************************************************************************/
#include "atari.h"

#define ASTEROIDS 8
#define SIDES 16
#define SCALE 8

struct asteroid_t {
	jgui::jpoint_t<int> pos;
	jgui::jpoint_t<int> dir;
	std::vector<int> vertices;
	int level;
	bool alive;
};

struct player_t {
	jgui::jpoint_t<float> pos;
	float angle;
	float vel;
	int fire = 0;
};

class Asteroids : public Atari {

  private:
		std::vector<asteroid_t> _asteroids;
		player_t _player;

  public:
    Asteroids():
      Atari()
    {
			srandom(time(NULL));

			_player = {
				.pos = {SW/2.0f, SH/2.0f},
				.angle = 0.0f,
				.vel = 0.0f
				};

			for (int i=0; i<ASTEROIDS; i++) {
				jgui::jpoint_t<int> 
					pos {random()%SW, random()%SH};

				if (pos.Distance(jgui::jpoint_t<int>{SW/2, SH/2}) < 32) {
					i = i - 1;

					continue;
				}

				asteroid_t 
					t {
						.pos = pos,
						.alive = true
					};

				do {
					t.dir = jgui::jpoint_t<int>{random()%3 - 1, random()%3 - 1};
				} while (t.dir.x == 0 and t.dir.y == 0);

				t.level = 2;

				for (int j=0; j<SIDES; j++) {
					t.vertices.push_back(random()%3);
				}

				_asteroids.push_back(t);
			}
    }

    virtual ~Asteroids()
    {
    }

    virtual void loop(int64_t timestamp)
    {
      if (key(KEY_UP)) {
				_player.vel = _player.vel + 0.1f;

				if (_player.vel > 5.0f) {
					_player.vel = 5.0f;
				}
			}

      if (key(KEY_DOWN)) {
				_player.vel = _player.vel - 1.0f;

				if (_player.vel < -5.0f) {
					_player.vel = -5.0f;
				}
			}

      if (key(KEY_LEFT)) {
				_player.angle = _player.angle - 0.1f;
			}

      if (key(KEY_RIGHT)) {
				_player.angle = _player.angle + 0.1f;
			}

			_player.pos.x = _player.pos.x + _player.vel*cos(_player.angle);
			_player.pos.y = _player.pos.y + _player.vel*sin(_player.angle);

			if ((_player.pos.x + 5) < 0) {
				_player.pos.x = SW + 5;
			}

			if ((_player.pos.x - 5) > SW) {
				_player.pos.x = -5;
			}

			if ((_player.pos.y + 5) < 0) {
				_player.pos.y = SH + 5;
			}

			if ((_player.pos.y - 5) > SH) {
				_player.pos.y = -5;
			}

			for (auto &t : _asteroids) {
				t.pos = t.pos + t.dir;

				if ((t.pos.x + SCALE) < 0) {
					t.pos.x = SW + SCALE;
				}

				if ((t.pos.x - SCALE) > SW) {
					t.pos.x = -SCALE;
				}

				if ((t.pos.y + SCALE) < 0) {
					t.pos.y = SH + SCALE;
				}
				
				if ((t.pos.y - SCALE) > SH) {
					t.pos.y = -SCALE;
				}

			}
    }

		virtual void draw(context &ctx)
		{
			// background
			ctx.fill(true);
			ctx.color(0x00);
			ctx.rect({0, 0, SW, SH});
			
      // player
			ctx.color(0x0f);
			ctx.fill(false);
			ctx.arc(_player.pos.x, _player.pos.y, 3, 0.0f, 2.0f*M_PI);
			ctx.line(
				_player.pos.x + 3*cos(_player.angle), _player.pos.y + 3*sin(_player.angle),
				_player.pos.x + 5*cos(_player.angle), _player.pos.y + 5*sin(_player.angle));

			// asteroids
			for (int i=0; i<ASTEROIDS; i++) {
				asteroid_t t = _asteroids[i];

				if (t.alive == false) {
					continue;
				}

				jgui::jpoint_t<int> 
					p0;

				for (int j=0; j<=t.vertices.size(); j++) {
					int
						k = j%t.vertices.size();
					float 
						angle = k*2.0f*M_PI/t.vertices.size();

					jgui::jpoint_t<int> 
						p1 {t.pos.x + (SCALE + t.vertices[k])*cos(angle), t.pos.y + (SCALE + t.vertices[k])*sin(angle)};

					if (j != 0) {
						ctx.line(p0.x, p0.y, p1.x, p1.y);
					}

					p0 = p1;
				}
			}
    }

};

int main(int argc, char **argv)
{
	jgui::Application::Init(argc, argv);

	Asteroids app;

	jgui::Application::Loop();

	return 0;
}
