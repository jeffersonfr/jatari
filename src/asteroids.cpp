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
	jgui::jpoint_t<float> pos;
	jgui::jpoint_t<float> dir;
	std::vector<int> vertices;
  float angle;
	int level;
	bool alive;
};

struct player_t {
	jgui::jpoint_t<float> pos;
	float angle;
	float vel;
	int fire = 0;
};

struct fire_t {
  jgui::jpoint_t<float> pos;
  jgui::jpoint_t<float> dir;
  bool alive;
};

class Asteroids : public Atari {

  private:
		std::vector<asteroid_t> _asteroids;
		std::vector<fire_t> _fires;
		player_t _player;
    bool _game_over;

  public:
    Asteroids():
      Atari()
    {
			srandom(time(NULL));

      _game_over = false;

			_player = {
				.pos = {SW/2.0f, SH/2.0f},
				.angle = 0.0f,
				.vel = 0.0f,
        .fire = 0
			};

			for (int i=0; i<ASTEROIDS; i++) {
				jgui::jpoint_t<int> 
					pos {(int)(random()%SW), (int)(random()%SH)};

				if (pos.Distance(jgui::jpoint_t<int>{SW/2, SH/2}) < 32) {
					i = i - 1;

					continue;
				}

				asteroid_t 
					t {
						.pos = pos,
            .angle = 0.0f,
						.alive = true
					};

				do {
					t.dir = jgui::jpoint_t<int>{(int)(random()%3 - 1), (int)(random()%3 - 1)};
				} while (t.dir.x == 0 and t.dir.y == 0);

				t.level = 1;

				for (int j=0; j<SIDES/t.level; j++) {
					t.vertices.push_back(random()%3);
				}

				_asteroids.push_back(t);
			}
    }

    virtual ~Asteroids()
    {
    }

    void player_event()
    {
      if (_game_over == false and key(KEY_UP)) {
				_player.vel = _player.vel + 0.2f;

				if (_player.vel > 2.0f) {
					_player.vel = 2.0f;
				}
			}

      if (_game_over == false and key(KEY_DOWN)) {
				_player.vel = _player.vel - 0.2f;

				if (_player.vel < -2.0f) {
					_player.vel = -2.0f;
				}
			}

      if (_game_over == false and key(KEY_LEFT)) {
				_player.angle = _player.angle - 0.1f;
			}

      if (_game_over == false and key(KEY_RIGHT)) {
				_player.angle = _player.angle + 0.1f;
			}

      if (_game_over == false and key(KEY_ACTION)) {
        if (_player.fire > 0) {
          fire_t t {
            .pos = _player.pos, 
            .dir {2.0f*cosf(_player.angle), 2.0f*sinf(_player.angle)},
            .alive = true
          };

          // INFO:: fire from the front of space ship
          t.pos = t.pos + SCALE*t.dir;

          _fires.push_back(t);

          _player.fire = -3;
        }
			}

			_player.pos.x = _player.pos.x + _player.vel*cos(_player.angle);
			_player.pos.y = _player.pos.y + _player.vel*sin(_player.angle);
      _player.fire = _player.fire + 1;

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
    }

    void colide_ship()
    {
      for (int i=0; i<16; i++) {
        fire_t t {
          .pos = _player.pos, 
          .dir {2.0f*cosf(i*2.0f*M_PI/16.0f), 2.0f*sinf(i*2.0f*M_PI/16.0f)},
          .alive = true
        };

        _fires.push_back(t);
      }
    }

    virtual void loop(int64_t timestamp)
    {
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

			for (auto &asteroid : _asteroids) {
        if (_game_over == false and _player.pos.Distance(asteroid.pos) < 2*SCALE/asteroid.level) {
          colide_ship();

          _player.vel = 0.0f;
          _game_over = true;

          break;
        }

        // INFO:: verify if collide with fire
        for (auto &fire : _fires) {
          if (fire.pos.Distance(asteroid.pos) < SCALE/asteroid.level) {
            asteroid.alive = false;
            fire.alive = false;

            if (asteroid.level == 2) {
              break;
            }

            // INFO:: add some small asteroids
            for (int i=0; i<3; i++) {
              asteroid_t 
                t {
                  .pos = asteroid.pos,
                  .angle = 0.0f,
                  .alive = true
                };

              do {
                t.dir = jgui::jpoint_t<int>{(int)(random()%3 - 1), (int)(random()%3 - 1)};
              } while (t.dir.x == 0 and t.dir.y == 0);

              t.level = 2;

              for (int j=0; j<SIDES/t.level; j++) {
                t.vertices.push_back(random()%3);
              }

              _asteroids.push_back(t);
            }

            break;
          }
        }
			}
      
      for (auto &fire : _fires) {
        fire.pos = fire.pos + fire.dir;
              
        if (fire.pos.x < 0 or fire.pos.y < 0 or fire.pos.x > SW or fire.pos.y > SH) {
          fire.alive = false;
        }
      }

      _asteroids.erase(std::remove_if(_asteroids.begin(), _asteroids.end(), 
            [](asteroid_t &asteroid) {
              return asteroid.alive == false;
            }), _asteroids.end());

      _fires.erase(std::remove_if(_fires.begin(), _fires.end(), 
            [](fire_t &fire) {
              return fire.alive == false;
            }), _fires.end());

      player_event();
    }

		virtual void draw(context &ctx)
		{
			// background
			ctx.fill(true);
			ctx.color(0x00);
			ctx.rect(0, 0, SW, SH);
			
      // player
      jgui::jpoint_t<float>
        p0 {SCALE, 0},
			  p1 {-SCALE/2, SCALE/2},
			  p2 {-SCALE/2, -SCALE/2};

      p0 = p0.Rotate(_player.angle) + _player.pos;
      p1 = p1.Rotate(_player.angle) + _player.pos;
      p2 = p2.Rotate(_player.angle) + _player.pos;

			ctx.color(0x0f);

      if (_game_over == true) {
			  ctx.color(0x04);
      }

			ctx.line(p0.x, p0.y, p1.x, p1.y);
			ctx.line(p1.x, p1.y, p2.x, p2.y);
			ctx.line(p2.x, p2.y, p0.x, p0.y);

			// asteroids
			ctx.color(0x01);

			for (auto &t : _asteroids) {
				jgui::jpoint_t<int> 
					p0;

				for (int j=0; j<=t.vertices.size(); j++) {
					int
						k = j%t.vertices.size();
					float 
						angle = k*2.0f*M_PI/t.vertices.size() + t.angle;

					jgui::jpoint_t<int> 
						p1 {(int)(t.pos.x + (SCALE/t.level + t.vertices[k])*cosf(angle)), (int)(t.pos.y + (SCALE/t.level + t.vertices[k])*sinf(angle))};

					if (j != 0) {
						ctx.line(p0.x, p0.y, p1.x, p1.y);
					}

					p0 = p1;
				}

        t.angle = t.angle + (random()%100)/1000.0f;
			}
			
      ctx.fill(false);
      ctx.color(0x0f);

      for (auto &t : _fires) {
        ctx.arc(t.pos.x, t.pos.y, 1, 0.0f, 2.0f*M_PI);
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
