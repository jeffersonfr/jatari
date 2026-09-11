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
#include "include/frogger.h"

#include <iostream>
#include <string>
#include <vector>
#include <ranges>
#include <charconv>

#define TIMEOUT 1500
#define SBOUNDS 16
#define KEYDELAY -3

struct object_t {
  sprite_t *sprite;
  jcanvas::jpoint_t<float> pos;
};

struct frogger_t {
  struct sprite_t *sprite;
  jcanvas::jpoint_t<float> pos;
  bool alive;
};

struct Fase {
  std::vector<float> velocities;
  std::vector<object_t> objects;

  static auto split(std::string const &value) {
    return value
      | std::views::split(' ')
      | std::views::transform([](auto&& part) {
          return std::string_view(&*part.begin(), std::ranges::distance(part));
        });
  }

  static std::unique_ptr<Fase> load_from_file(std::string filename) {
    std::ifstream ifs(filename);

    if (!ifs.is_open()) {
      return nullptr;
    }

    std::unique_ptr<Fase> fase = std::make_unique<Fase>();
    std::string line;

    while (std::getline(ifs, line)) {
      std::istringstream iss(line);

      std::cout << line << std::endl;

      if (line[0] != '#') {
        fase->velocities = split(line)
          | std::views::transform([](std::string_view value) {
              std::string::size_type sz;
              return std::stof(std::string(value), &sz);
            })
          | std::ranges::to<std::vector<float>>();

        break;
      }
    }

    while (std::getline(ifs, line)) {
      std::istringstream iss(line);

      std::cout << line << std::endl;

      if (line[0] != '#') {
        auto values = split(line)
          | std::ranges::to<std::vector<std::string>>();

        sprite_t *sprite = nullptr;

        int size = std::stoi(values[2]);

        if (values[1] == "aligator") {
          sprite = &aligator;
        } else if (values[1] == "turtle") {
          sprite = &turtle;
        } else if (values[1] == "trunk") {
          sprite = &trunk;
        } else if (values[1] == "truck") {
          sprite = &truck;
        } else if (values[1] == "car_01") {
          sprite = &car_01;
        } else if (values[1] == "car_02") {
          sprite = &car_02;
        } else if (values[1] == "car_03") {
          sprite = &car_03;
        }

        if (!sprite) {
          continue;
        }

        int y = std::stoi(values[0]);

        for (int i = 3; i < 3 + size; i++) {
          fase->objects.emplace_back(sprite, jcanvas::jpoint_t{std::stoi(values[i]), y});
        }
      }
    }

    return fase;
  }
};

class Frogger : public Atari {

  private:
    std::unique_ptr<Fase> _fase;
    jcanvas::jpoint_t<int>
      _block {
        .x = 8, 
        .y = 12
      };
    struct frogger_t
      _frogger {
        .sprite = &frogger,
        .pos = {
          .x = (SW - frogger.size.x)/2.0f, 
          .y = 13
        },
        .alive = true,
      };
    bool _houses[5];
    int _delay = 0;
    int _lives = 5;
    int _timeout = TIMEOUT;

  public:
    Frogger():
      Atari()
    {
      for (int i=0; i<5; i++) {
        _houses[i] = false;
      }

      _fase = Fase::load_from_file("fases/1");
    }

    virtual ~Frogger()
    {
    }

    virtual void loop(int64_t timestamp)
    {
      if (_frogger.alive == true) {
				if (_frogger.pos.x > _block.x and _frogger.pos.x < (SW - _block.x)) {
					if (_delay++ > 0) {
						if (key(KEY_LEFT)) {
							_frogger.pos.x = _frogger.pos.x - _block.x/2;

							if (_frogger.pos.y < 2 or _frogger.pos.y > 6) {
								if (_frogger.pos.x < _block.x) {
									_frogger.pos.x = _block.x;
								}
							}

							_delay = KEYDELAY;
						}

						if (key(KEY_RIGHT)) {
							_frogger.pos.x = _frogger.pos.x + _block.x/2;

							if (_frogger.pos.y < 2 or _frogger.pos.y > 6) {
								if (_frogger.pos.x > (SW -  _block.x - _frogger.sprite->size.x)) {
									_frogger.pos.x = SW - _block.x - _frogger.sprite->size.x;
								}
							}

							_delay = KEYDELAY;
						}

						if (key(KEY_UP)) {
							_frogger.pos.y = _frogger.pos.y - 1;

							if (_frogger.pos.y < 1) {
								_frogger.pos.y = 1;
							}

							_delay = KEYDELAY;
						}

						if (key(KEY_DOWN)) {
							_frogger.pos.y = _frogger.pos.y + 1;

							if (_frogger.pos.y > 13) {
								_frogger.pos.y = 13;
							}

							_delay = KEYDELAY;
						}
					}
				}
			} 
		}

		virtual void draw(context &ctx)
		{
			bool alive = _frogger.alive;

			ctx.fill(true);

			// background
			ctx.color(2);
			ctx.rect({0, 0, SW, SH});

			// sidewalk
			ctx.color(6);
			ctx.rect({_block.x, 1*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 7*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 13*_block.y, SW - 2*_block.x, _block.y});

			// houses
			int x = (SW - 2*_block.x)/9;

			for (int i=0; i<9; i++) {
				ctx.color(6);

				if ((i%2) == 0) {
					ctx.color(1);
					ctx.rect({i*x + _block.x, 1*_block.y, x, _block.y});

					if (_houses[i/2] == true) {
						ctx.sprite(frogger_house.data, {i*x + _block.x + (x - 8)/2, _block.y + (_block.y - 11)/2, 8, 11});
					}
				}
			}

			// lake
			ctx.color(1);
			ctx.rect({_block.x, 2*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 3*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 4*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 5*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 6*_block.y, SW - 2*_block.x, _block.y});

			// road
			ctx.color(0);
			ctx.rect({_block.x, 8*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 9*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 10*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 11*_block.y, SW - 2*_block.x, _block.y});
			ctx.rect({_block.x, 12*_block.y, SW - 2*_block.x, _block.y});

			for (auto &object : _fase->objects) {
        if (_fase->velocities[(int)object.pos.y] < 0) {
          ctx.blit(BLIT_FLIP_HORIZONTAL);
        }

				ctx.sprite(object.sprite->data, {(int)object.pos.x, (int)(object.pos.y*_block.y + (_block.y - object.sprite->size.y)/2), object.sprite->size.x, object.sprite->size.y});

        ctx.blit(BLIT_NONE);

				object.pos.x = object.pos.x + _fase->velocities[(int)object.pos.y];

				if (_fase->velocities[(int)object.pos.y] < 0) {
					if (object.pos.x < -SBOUNDS) {
            object.pos.x = SW;
          }
        } else {
          if ((object.pos.x + object.sprite->size.x) > (SW + SBOUNDS)) {
            object.pos.x = -object.sprite->size.x;
          }
        }
      }

      jcanvas::jrect_t<int> fregion = {
        .point = {
          .x = (int)_frogger.pos.x,
          .y = (int)(_frogger.pos.y*_block.y + (_block.y - _frogger.sprite->size.y)/2)
        },
        .size = {
          .x = (int)_frogger.sprite->size.x,
          .y = (int)_frogger.sprite->size.y
        }
      };

      // fases
      ctx.sprite(_frogger.sprite->data, fregion);

      if (_frogger.alive == true and _frogger.pos.y >= 2 and _frogger.pos.y <= 6) {
        _frogger.pos.x = _frogger.pos.x + _fase->velocities[(int)_frogger.pos.y];

        if (_fase->velocities[(int)_frogger.pos.y] < 0) {
          if (_frogger.pos.x < -SBOUNDS) {
            _frogger.pos.x = SW;
          }
        } else {
          if ((_frogger.pos.x + _frogger.sprite->size.x) > (SW + SBOUNDS)) {
            _frogger.pos.x = -_frogger.sprite->size.x;
          }
        }
      }
      
      // colision
      if (_frogger.alive == true and _frogger.pos.x > _block.x and _frogger.pos.x < (SW - _block.x)) {
        for (auto &object : _fase->objects) {
          jcanvas::jrect_t<int> oregion = {
            .point = {
              .x = (int)object.pos.x,
              .y = (int)(object.pos.y*_block.y + (_block.y - object.sprite->size.y)/2)
            },
            .size = {
              .x = (int)object.sprite->size.x,
              .y = (int)object.sprite->size.y
            }
          };

          if (_frogger.pos.y >= 2 and _frogger.pos.y <= 6) { // lake
            _frogger.alive = false;
          }

          if (collide(fregion, oregion) == true) {
            if (_frogger.pos.y >= 2 and _frogger.pos.y <= 6) { // lake
              _frogger.sprite = &frogger;
              _frogger.alive = true;
            } else if (_frogger.pos.y >= 8 and _frogger.pos.y <= 12) { // road
              _frogger.alive = false;
            }

            break;
          }
        }
      
        // houses
        if (_frogger.pos.y == 1) {
          int x = (SW - 2*_block.x)/9;
          int house = -1;

          for (int i=0; i<9; i++) {
            jcanvas::jrect_t<int> oregion = {
              .point = {
                .x = (int)i*x + _block.x,
                .y = (int)(_block.y)
              },
              .size = {
                .x = (int)x,
                .y = (int)_block.y
              }
            };

            if (collide(fregion, oregion) == true) {
              house = i;
            
              if ((house%2) != 0) {
                house = -1;

                _frogger.alive = false;

                break;
              }
            }
          }
          
          if (house < 0) {
            _frogger.alive = false;
          } else {
            house = house/2;

            if (_houses[house] == false) {
              _houses[house] = true;

              // reset position
					    _timeout = TIMEOUT;
              _frogger.pos = {
                .x = (SW - frogger.size.x)/2.0f, 
                .y = 13
              };
            } else {
              _frogger.alive = false;
            }
          }
        }
      }

      // sides
      ctx.color(2);
      ctx.rect({0, 0, _block.x, SH});
      ctx.rect({SW - _block.x, 0, _block.x, SH});
      
      ctx.fill(true);
      ctx.color(0x0f);

      // lives
      for (int i=0; i<_lives; i++) {
        ctx.rect({_block.x + i*(4 + 2), (int)(14*_block.y + 4), 4, 5});
      }
      
      ctx.color(0x00);

      // timeout
			if (_frogger.alive == true) {
				_timeout = _timeout - 1;
			
				if (_timeout < 0) {
					_timeout = 0;
					_frogger.alive = false;
				}
			}

      int length = 3*_timeout/100;

      ctx.rect({SW - _block.x - length, (int)(14*_block.y + 4), length, 5});

			if (_frogger.alive == false and alive == true) {
				_lives = _lives - 1;

				if (_lives > 0) {
					_delay = 2*25; // fps (use another counter)
				}

				_frogger.sprite = &frogger_dead;
			}

			if (_frogger.alive == false and _delay > 0) {
				_delay = _delay - 1;

				if (_delay == 0) {
					// reset
					_timeout = TIMEOUT;
          _frogger.sprite = &frogger;
          _frogger.alive = true;
					_frogger.pos = {
						.x = (SW - frogger.size.x)/2.0f, 
						.y = 13
					};
				}
			}
    }

};


int main(int argc, char **argv)
{
	jcanvas::Application::Init(argc, argv);

	Frogger app;

	jcanvas::Application::Loop();

	return 0;
}
