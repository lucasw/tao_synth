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

#ifndef TAOGRAPHICSENGINE_H
#define TAOGRAPHICSENGINE_H

#include <GLFW/glfw3.h>
#include <iomanip>
#include <memory>
#include <sstream>

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

namespace tao {
class Manager;
class Instrument;
class AccessPoint;
class Bow;
class Hammer;
class Connector;
class Stop;
class Output;

// The following functions are registered as GLFW callback functions
void tao_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void tao_mouse(GLFWwindow* window, int button, int action, int mods);
void tao_motion(GLFWwindow* window, double x, double y);
// void tao_display();
// void tao_reshape(int w, int h);

class DLLEXPORT GraphicsEngine {
  friend class Manager;
  friend class Instrument;
  friend class AccessPoint;
  friend class Bow;
  friend class Hammer;
  friend class Connector;
  friend class Stop;
  friend class Output;
  friend void tao_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
  friend void tao_mouse(GLFWwindow* window, int button, int action, int mods);
  friend void tao_motion(GLFWwindow* window, double x, double y);
  // friend void tao_display();
  // friend void tao_reshape(int w, int h);

public:
  GraphicsEngine(std::shared_ptr<Manager> manager);
  ~GraphicsEngine();

  void activate();
  void deactivate();
  //  or TAO_JAGGED;
  void init(const std::string win_name="tao_synth", int lineMode=TAO_ANTIALIAS);
  void reshape(int w, int h);
  void mouse(int button, int action, int mods);
  void motion(double x, double y);
  void setInstrDisplayResolution();
  void calculateOriginForRotations();
  void clearBackBuffer();
  void pushModelViewMatrix();
  void popModelViewMatrix();
  void rotateAndTranslate();
  void display();
  void flushGraphics();
  void swapBuffers();
  void mainLoop();

  void setDrawColour(int colour);
  void setClearColour(int colour);
  void displayCharString(GLfloat x, GLfloat y, GLfloat z, const std::string text);
  void displayCharString(GLfloat x, GLfloat y, GLfloat z, const std::string text,
                         GLfloat r, GLfloat g, GLfloat b);
  void displayPoint(GLfloat x, GLfloat y, int colour);
  void displayInstruments();
  void displayDevices();
  void displayInstrument(Instrument &instr);
  float screenX(Instrument &instr, float x, float y);
  float screenY(Instrument &instr, float x, float y);
  float screenY(Instrument &instr, float x, float y, float z);
  void displayPointInInstrumentSpace(Instrument &instr, float instrx,
                                     float instry, float instrz);
  void label(Instrument &instr, float x, float y, GLfloat labelXOffset,
             GLfloat labelYOffset, const std::string caption, GLfloat r, GLfloat g,
             GLfloat b);
  void label(Instrument &instr, float x, GLfloat labelXOffset,
             GLfloat yOffset, const std::string caption, GLfloat r, GLfloat g, GLfloat b);
  void label(Instrument &instr, float x, float y, float z,
             GLfloat labelXOffset, GLfloat labelYOffset, const std::string caption,
             GLfloat r, GLfloat g, GLfloat b);
  void displayAccessPoint(Instrument &instr, int i, int j);
  void displayAccessPoint(AccessPoint &p);

private:
  std::shared_ptr<Manager> manager_;
  int displayInstrumentNames, displayDeviceNames;
  int active;

  std::shared_ptr<GLFWwindow> window_;

  int viewportWidth, viewportHeight;
  GLfloat xOffset, yOffset, zOffset, xAngle, yAngle, zAngle;
  int jstep;
  int refreshRate;
  int refreshRateToRestore;
  float globalMagnification;
  std::ostringstream timestream; // used to create a string of characters
  // representing the elapsed time.
  enum { TAO_PERSPECTIVE, TAO_ORTHO, TAO_ANTIALIAS, TAO_JAGGED };
  int projectionMode;
  double lastMouseX, lastMouseY, zoomInitialMouseY;
  // TODO(lucasw) convert to bools
  int drag;
  int dolly;
  int rotate;
  GLfloat minWorldX, maxWorldX, minWorldY, maxWorldY;
  GLfloat translateX, translateY, translateZ, scaleBy;
};
}
#endif
