/* TaoSynth - A software package for sound synthesis with physical models
 * Copyright (C) 1993-1999 Mark Pearson
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef TAODEVICE_H
#define TAODEVICE_H

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#include <tao/access_point.h>

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

namespace tao {
class SynthEngine;
class Instrument;

class DLLEXPORT Device {
  friend class SynthEngine;
  friend class GraphicsEngine;

public:
  Device(std::shared_ptr<Manager> tao);
  ~Device();
  Device(std::shared_ptr<Manager> manager, const std::string deviceName);
  std::string getName();
  float getX();
  float getY();
  virtual void apply(AccessPoint &a);
  void remove();
  float x, y;

protected:
  std::shared_ptr<Manager> manager_;
  void addToSynthesisEngine();
  void removeFromSynthesisEngine();
  void activate();
  void deactivate();
  virtual void update() = 0;
  virtual void display() = 0;

  enum { BOW, HAMMER, CONNECTOR, STOP, OUTPUT };

  int deviceType;
  int active;
  std::string name;
  Instrument *targetInstrument;
  AccessPoint interfacePoint;
  Device *next;
};
}
#endif
