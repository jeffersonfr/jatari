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

#define LIMIT 8
#define SIZE 25

uint8_t numbers[10][8*16] = {
  { // 0
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00
  },
  { // 1
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00
  },
  { // 2
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00
  },
  { // 3
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00
  },
  { // 4
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0f, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00
  },
  { // 5
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00
  },
  { // 6
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00
  },
  { // 7
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00
  },
  { // 8
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00
  },
  { // 9
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00
  }
};

class Pong : public Atari {

  private:
    jcanvas::jpoint_t<int> _ball_pos = {SW/2, SH/2};
    jcanvas::jpoint_t<int> _ball_vel = {-3, 3};
    jcanvas::jpoint_t<int> _player0_pos {3*LIMIT, SH/2 - SIZE/2};
    jcanvas::jpoint_t<int> _player1_pos {SW - 3*LIMIT, SH/2 - SIZE/2};
    int _player0_score = 0;
    int _player1_score = 0;
    int _delay = 0;

  public:
    Pong():
      Atari()
    {
    }

    virtual ~Pong()
    {
    }

    virtual void loop(int64_t timestamp)
    {
      _ball_pos.x = _ball_pos.x + _ball_vel.x;
      _ball_pos.y = _ball_pos.y + _ball_vel.y;

      if (_ball_pos.x < 2*LIMIT) {
        _player1_score = _player1_score + 1;
        _ball_pos = {SW/2, SH/2};
        _ball_vel.y = random()%6 - 3 + 1;
      } else if (_ball_pos.x > SW - 2*LIMIT) {
        _player0_score = _player0_score + 1;
        _ball_pos = {SW/2, SH/2};
        _ball_vel.y = random()%6 - 3 + 1;
      } else if (
          (_ball_pos.x >= _player0_pos.x and _ball_pos.x <= _player0_pos.x + 3 and (_ball_pos.y + 2) > _player0_pos.y and (_ball_pos.y - 2*2) < _player0_pos.y + SIZE)) {
        _ball_vel.x = -_ball_vel.x;
        _ball_vel.y = -((_player0_pos.y + SIZE/2) - _ball_pos.y)/2;
      } else if (
          (_ball_pos.x >= _player1_pos.x - 3 and _ball_pos.x <= _player1_pos.x and (_ball_pos.y + 2) > _player1_pos.y and (_ball_pos.y - 2*2) < _player1_pos.y + SIZE)) {
        _ball_vel.x = -_ball_vel.x;
        _ball_vel.y = -((_player1_pos.y + SIZE/2) - _ball_pos.y)/2;
      }
        
      if (_ball_pos.y < 2*LIMIT or _ball_pos.y > SH - 2*LIMIT) {
        _ball_vel.y = -_ball_vel.y;
      }

      int y = _player1_pos.y + SIZE/2;

      if (y < _ball_pos.y) {
        _player1_pos.y = _player1_pos.y + 3;
      } else if (y > _ball_pos.y) {
        _player1_pos.y = _player1_pos.y - 3;
      }

      if (key(KEY_UP)) {
        _player0_pos.y = _player0_pos.y - 5;
      } else if (key(KEY_DOWN)) {
        _player0_pos.y = _player0_pos.y + 5;
      }

      if (_player0_pos.y < 3*LIMIT) {
        _player0_pos.y = 3*LIMIT;
      }

      if (_player1_pos.y < 3*LIMIT) {
        _player1_pos.y = 3*LIMIT;
      }

      if (_player0_pos.y > SH - 3*LIMIT - SIZE) {
        _player0_pos.y = SH - 3*LIMIT - SIZE;
      }

      if (_player1_pos.y > SH - 3*LIMIT - SIZE) {
        _player1_pos.y = SH - 3*LIMIT - SIZE;
      }
    }

		virtual void draw(context &ctx)
		{
			// background
			ctx.fill(true);
			ctx.color(0x00);
			ctx.rect({0, 0, SW, SH});
			
      // players
			ctx.color(0x0f);
			ctx.rect({_player0_pos.x, _player0_pos.y, 2, SIZE});
			ctx.rect({_player1_pos.x, _player1_pos.y, 2, SIZE});
      
      // line
      for (int i=0; i<11; i++) {
			  ctx.rect({SW/2, 3*LIMIT + i*(10 + 4), 1, 10});
      }

      // ball
			ctx.rect({_ball_pos.x-2, _ball_pos.y-2, 4, 4});
      
      // borders
			ctx.fill(false);
      for (int i=0; i<5; i++) {
			  ctx.rect({LIMIT + i, LIMIT + i, SW - 2*LIMIT - 2*i, SH - 2*LIMIT - 2*i});
      }

      // score
      ctx.sprite(numbers[_player0_score/10], {50 + 0*(8 + 1), 3*LIMIT, 8, 16});
      ctx.sprite(numbers[_player0_score%10], {50 + 1*(8 + 1), 3*LIMIT, 8, 16});
      
      ctx.sprite(numbers[_player1_score/10], {SW - 66 + 0*(8 + 1), 3*LIMIT, 8, 16});
      ctx.sprite(numbers[_player1_score%10], {SW - 66 + 1*(8 + 1), 3*LIMIT, 8, 16});
    }

};

int main(int argc, char **argv)
{
	jcanvas::Application::Init(argc, argv);

	Pong app;

	jcanvas::Application::Loop();

	return 0;
}
