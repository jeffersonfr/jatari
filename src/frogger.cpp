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

#define TIMEOUT 1500
#define SBOUNDS 16
#define KEYDELAY -3

struct sprite_t {
  jgui::jsize_t<int> size;
  uint8_t data[];
} truck = {
    .size = {
      14, 8
    },
    .data = {
      0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x0f, 0x00, 0x00,
      0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
      0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
      0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
      0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
      0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
      0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
      0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x0f, 0x00, 0x00,

    },
  },
  aligator = {
    .size = {
      24, 7
    },
    .data = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00,
      0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
      0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
  },
  car_01 = {
    .size = {
      9, 7
    },
    .data = {
      0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
      0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x05, 0x05, 0x05,
      0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0x00
    }
  },
  car_02 = {
    .size = {
      9, 7
    },
    .data = {
      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x02,
      0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x02,
      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
      0x02, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00, 0x02,
      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
      0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x02,
      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x02
    }
  },
  car_03 = {
    .size = {
      9, 7
    },
    .data = {
      0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00,
      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
      0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00,
      0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
      0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
      0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00
    }
  },
  trunk = {
    .size = {
      12, 8
    },
    .data = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x06, 0x00, 0x06, 0x06, 0x06,
      0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x06, 0x00, 0x06, 0x06, 0x06,
      0x00, 0x06, 0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
      0x06, 0x06, 0x06, 0x06, 0x00, 0x06, 0x06, 0x00, 0x06, 0x00, 0x06, 0x06,
      0x06, 0x06, 0x06, 0x06, 0x00, 0x06, 0x06, 0x00, 0x06, 0x00, 0x06, 0x06,
      0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x06, 0x00, 0x06, 0x06, 0x06,
      0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06
    }
  },
  frogger = {
    .size = {
      7, 7
    },
    .data = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x0f, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f,
      0x0f, 0x00, 0x0f, 0x0f, 0x0f, 0x00, 0x0f,
      0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00,
      0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
      0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
      0x0f, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f,
    }
  },
  frogger_dead = {
    .size = {
      7, 7
    },
    .data = {
      0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
      0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00,
      0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,
      0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00,
      0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
    }
  },
  frogger_house = {
    .size = {
      8, 11
    },
    .data = {
      0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
      0x06, 0x00, 0x06, 0x00, 0x00, 0x06, 0x00, 0x06,
      0x06, 0x06, 0x06, 0x00, 0x00, 0x06, 0x06, 0x06,
      0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00,
      0x06, 0x00, 0x06, 0x06, 0x06, 0x06, 0x00, 0x06,
      0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
      0x06, 0x06, 0x00, 0x06, 0x06, 0x00, 0x06, 0x06,
      0x00, 0x06, 0x06, 0x00, 0x00, 0x06, 0x06, 0x00,
      0x00, 0x00, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00,
      0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06,
      0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06
    }
  },
  turtle = {
    .size = {
      8, 6
    },
    .data = {
      0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00,
      0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00,
      0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
      0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
      0x00, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00,
      0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00
    }
  };

struct object_t {
  struct sprite_t *sprite;
  jgui::jpoint_t<float> pos;
};

struct frogger_t {
  struct sprite_t *sprite;
  jgui::jpoint_t<float> pos;
  bool alive;
};

class Frogger : public Atari {

  private:
    std::vector<struct object_t>
      _objects;
    jgui::jsize_t<int>
      _block {
        .width = 8, 
        .height = 12
      };
    struct frogger_t
      _frogger {
        .sprite = &frogger,
        .pos = {
          .x = (SW - frogger.size.width)/2.0f, 
          .y = 13
        },
        .alive = true,
      };
    float _velocity[16];
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

      _velocity[0] = 0.0f;
      _velocity[1] = 0.0f;
      _velocity[2] = 1.5f;
      _velocity[3] = -2.0f;
      _velocity[4] = 1.0f;
      _velocity[5] = 2.5f;
      _velocity[6] = -1.0f;
      _velocity[7] = 0.0f;
      _velocity[8] = -3.0f;
      _velocity[9] = 1.0f;
      _velocity[10] = 2.0f;
      _velocity[11] = -1.0f;
      _velocity[12] = 3.0f;
      _velocity[13] = 0.0f;
      _velocity[14] = 0.0f;
      _velocity[15] = 0.0f;

      // lake objects
      _objects.push_back({
          .sprite = &aligator,
          .pos = {
            .x = 0.0f*aligator.size.width,
            .y = 2
          }
      });
      _objects.push_back({
          .sprite = &aligator,
          .pos = {
            .x = 3.0f*aligator.size.width,
            .y = 2
          }
      });
      // 3 turtles
      _objects.push_back({
          .sprite = &turtle,
          .pos = {
            .x = 0*turtle.size.width,
            .y = 3
          }
      });
      _objects.push_back({
          .sprite = &turtle,
          .pos = {
            .x = 2.0f*turtle.size.width,
            .y = 3
          }
      });
      _objects.push_back({
          .sprite = &turtle,
          .pos = {
            .x = 4.0f*turtle.size.width,
            .y = 3
          }
      });
      // 3 joined trunks
      _objects.push_back({
          .sprite = &trunk,
          .pos = {
            .x = 0.0f*trunk.size.width,
            .y = 4
          }
      });
      _objects.push_back({
          .sprite = &trunk,
          .pos = {
            .x = 1.0f*trunk.size.width,
            .y = 4
          }
      });
      _objects.push_back({
          .sprite = &trunk,
          .pos = {
            .x = 2.0f*trunk.size.width,
            .y = 4
          }
      });

      // 3 spaced trunks
      _objects.push_back({
          .sprite = &trunk,
          .pos = {
            .x = 0.0f*trunk.size.width,
            .y = 5
          }
      });
      _objects.push_back({
          .sprite = &trunk,
          .pos = {
            .x = 2.0f*trunk.size.width,
            .y = 5
          }
      });
      _objects.push_back({
          .sprite = &trunk,
          .pos = {
            .x = 4.0f*trunk.size.width,
            .y = 5
          }
      });
      
      // 3 spaced turtles
      _objects.push_back({
          .sprite = &turtle,
          .pos = {
            .x = 0.0f*turtle.size.width,
            .y = 6
          }
      });
      _objects.push_back({
          .sprite = &turtle,
          .pos = {
            .x = 2.0f*turtle.size.width,
            .y = 6
          }
      });
      _objects.push_back({
          .sprite = &turtle,
          .pos = {
            .x = 4.0f*turtle.size.width,
            .y = 6
          }
      });

      // road objects
      _objects.push_back({
          .sprite = &truck,
          .pos = {
            .x = 0,
            .y = 8
          }
      });
      _objects.push_back({
          .sprite = &car_01,
          .pos = {
            .x = 10,
            .y = 9
          }
      });
      _objects.push_back({
          .sprite = &car_02,
          .pos = {
            .x = 0,
            .y = 10
          }
      });
			// double cars 01
      _objects.push_back({
          .sprite = &car_03,
          .pos = {
            .x = 0.0f*car_03.size.width,
            .y = 11
          }
      });
      _objects.push_back({
          .sprite = &car_03,
          .pos = {
            .x = 2.0f*car_03.size.width,
            .y = 11
          }
      });
			// double cars 02
      _objects.push_back({
          .sprite = &car_03,
          .pos = {
            .x = 8.0f*car_03.size.width,
            .y = 11
          }
      });
      _objects.push_back({
          .sprite = &car_03,
          .pos = {
            .x = 10.0f*car_03.size.width,
            .y = 11
          }
      });
      _objects.push_back({
          .sprite = &car_01,
          .pos = {
            .x = 0,
            .y = 12
          }
      });
    }

    virtual ~Frogger()
    {
    }

    virtual void loop(int64_t timestamp)
    {
      if (_frogger.alive == true) {
				if (_frogger.pos.x > _block.width and _frogger.pos.x < (SW - _block.width)) {
					if (_delay++ > 0) {
						if (key(KEY_LEFT)) {
							_frogger.pos.x = _frogger.pos.x - _block.width/2;

							if (_frogger.pos.y < 2 or _frogger.pos.y > 6) {
								if (_frogger.pos.x < _block.width) {
									_frogger.pos.x = _block.width;
								}
							}

							_delay = KEYDELAY;
						}

						if (key(KEY_RIGHT)) {
							_frogger.pos.x = _frogger.pos.x + _block.width/2;

							if (_frogger.pos.y < 2 or _frogger.pos.y > 6) {
								if (_frogger.pos.x > (SW -  _block.width - _frogger.sprite->size.width)) {
									_frogger.pos.x = SW - _block.width - _frogger.sprite->size.width;
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
			ctx.rect({_block.width, 1*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 7*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 13*_block.height, SW - 2*_block.width, _block.height});

			// houses
			int x = (SW - 2*_block.width)/9;

			for (int i=0; i<9; i++) {
				ctx.color(6);

				if ((i%2) == 0) {
					ctx.color(1);
					ctx.rect({i*x + _block.width, 1*_block.height, x, _block.height});

					if (_houses[i/2] == true) {
						ctx.sprite(frogger_house.data, {i*x + _block.width + (x - 8)/2, _block.height + (_block.height - 11)/2, 8, 11});
					}
				}
			}

			// lake
			ctx.color(1);
			ctx.rect({_block.width, 2*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 3*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 4*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 5*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 6*_block.height, SW - 2*_block.width, _block.height});

			// road
			ctx.color(0);
			ctx.rect({_block.width, 8*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 9*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 10*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 11*_block.height, SW - 2*_block.width, _block.height});
			ctx.rect({_block.width, 12*_block.height, SW - 2*_block.width, _block.height});

			for (auto &object : _objects) {
        if (_velocity[(int)object.pos.y] < 0) {
          ctx.blit(BLIT_FLIP_HORIZONTAL);
        }

				ctx.sprite(object.sprite->data, {(int)object.pos.x, (int)(object.pos.y*_block.height + (_block.height - object.sprite->size.height)/2), object.sprite->size.width, object.sprite->size.height});

        ctx.blit(BLIT_NONE);

				object.pos.x = object.pos.x + _velocity[(int)object.pos.y];

				if (_velocity[(int)object.pos.y] < 0) {
					if (object.pos.x < -SBOUNDS) {
            object.pos.x = SW;
          }
        } else {
          if ((object.pos.x + object.sprite->size.width) > (SW + SBOUNDS)) {
            object.pos.x = -object.sprite->size.width;
          }
        }
      }

      jgui::jrect_t<int> fregion = {
        .point = {
          .x = (int)_frogger.pos.x,
          .y = (int)(_frogger.pos.y*_block.height + (_block.height - _frogger.sprite->size.height)/2)
        },
        .size = {
          .width = (int)_frogger.sprite->size.width,
          .height = (int)_frogger.sprite->size.height
        }
      };

      // frogger
      ctx.sprite(_frogger.sprite->data, fregion);

      if (_frogger.alive == true and _frogger.pos.y >= 2 and _frogger.pos.y <= 6) {
        _frogger.pos.x = _frogger.pos.x + _velocity[(int)_frogger.pos.y];

        if (_velocity[(int)_frogger.pos.y] < 0) {
          if (_frogger.pos.x < -SBOUNDS) {
            _frogger.pos.x = SW;
          }
        } else {
          if ((_frogger.pos.x + _frogger.sprite->size.width) > (SW + SBOUNDS)) {
            _frogger.pos.x = -_frogger.sprite->size.width;
          }
        }
      }
      
      // colision
      if (_frogger.alive == true and _frogger.pos.x > _block.width and _frogger.pos.x < (SW - _block.width)) {
        for (auto &object : _objects) {
          jgui::jrect_t<int> oregion = {
            .point = {
              .x = (int)object.pos.x,
              .y = (int)(object.pos.y*_block.height + (_block.height - object.sprite->size.height)/2)
            },
            .size = {
              .width = (int)object.sprite->size.width,
              .height = (int)object.sprite->size.height
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
          int x = (SW - 2*_block.width)/9;
          int house = -1;

          for (int i=0; i<9; i++) {
            jgui::jrect_t<int> oregion = {
              .point = {
                .x = (int)i*x + _block.width,
                .y = (int)(_block.height)
              },
              .size = {
                .width = (int)x,
                .height = (int)_block.height
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
                .x = (SW - frogger.size.width)/2.0f, 
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
      ctx.rect({0, 0, _block.width, SH});
      ctx.rect({SW - _block.width, 0, _block.width, SH});
      
      ctx.fill(true);
      ctx.color(0x0f);

      // lives
      for (int i=0; i<_lives; i++) {
        ctx.rect({_block.width + i*(4 + 2), (int)(14*_block.height + 4), 4, 5});
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

      ctx.rect({SW - _block.width - length, (int)(14*_block.height + 4), length, 5});

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
						.x = (SW - frogger.size.width)/2.0f, 
						.y = 13
					};
				}
			}
    }

};


int main(int argc, char **argv)
{
	jgui::Application::Init(argc, argv);

	Frogger app;

	jgui::Application::Loop();

	return 0;
}
