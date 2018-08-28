/* Tao - A software package for sound synthesis with physical models
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

#ifdef WIN32
#include <strstrea.h>
#else
#include <sstream>
#endif
#include <tao/tao.h>
#include <tao/output.h>
#include <fstream>
#include <string.h>

const int TaoOutput::buffersize = 500;
float TaoOutput::displayPosition = 0.0f;

TaoOutput::TaoOutput(std::shared_ptr<Tao> tao) : TaoDevice(tao) {}

TaoOutput::~TaoOutput() {
  delete outputfile;
  delete displayStream;

  displayStream = NULL;
  outputfile = NULL;
}

TaoOutput::TaoOutput(std::shared_ptr<Tao> tao,
    const std::string filename, const size_t channels) : TaoDevice(tao) {
  deviceType = TaoDevice::OUTPUT;
  index = 0;
  first_write = 1;
  displayString = "";
  buffer.resize(buffersize);
  next = NULL;
  samples.resize(channels);
  maxSample = 0.0;
  outputfile = new std::ofstream;
  // why are we initialising the stream with an empty string?
  displayStream =
      new std::ostringstream(displayString, std::ostringstream::out);

  std::ostringstream tempname(std::ostringstream::out);
  tempname << filename << ".dat" << std::ends;
  fullfilename = tempname.str();

  // This code is used to display output sample values being sent to TaoOutput
  // devices in the graphics window

  myDisplayPosition = TaoOutput::displayPosition;
  TaoOutput::displayPosition += 10.0f;

  addToSynthesisEngine();
}

TaoOutput::TaoOutput(std::shared_ptr<Tao> tao,
    const std::string outputName, const std::string filename, size_t channels)
    : TaoDevice(tao, outputName) {
  deviceType = TaoDevice::OUTPUT;
  index = 0;
  first_write = 1;
  displayString = "";
  buffer.resize(buffersize);
  next = NULL;
  samples.resize(channels);
  maxSample = 0.0;
  outputfile = new std::ofstream;
  // why are we initialising the stream with an empty string?
  displayStream =
      new std::ostringstream(displayString, std::ostringstream::out);

  std::ostringstream tempname(std::ostringstream::out);
  tempname << filename << ".dat" << std::ends;
  fullfilename = tempname.str();

  // This code is used to display output sample values being sent to TaoOutput
  // devices in the graphics window

  myDisplayPosition = TaoOutput::displayPosition;
  TaoOutput::displayPosition += 10.0f;

  addToSynthesisEngine();
}

void TaoOutput::update() {
  if (tao_->synthesisEngine.tick % tao_->synthesisEngine.throwAway)
    return; // Throw away samples

  for (size_t i = 0; i < samples.size(); ++i) {
    if (index < buffer.size()) {
      buffer[index++] = samples[i];
      if (samples[i] > maxSample)
        maxSample = samples[i];
    }
  #if 0
    if (samples.size() == 2) {
      buffer[index++] = samples[0];
      if (samples[0] > maxSample)
        maxSample = samples[0];
      buffer[index++] = samples[1];
      if (samples[1] > maxSample)
        maxSample = samples[1];
    }
    if (samples.size() == 1) {
      buffer[index++] = samples[0];
      if (samples[0] > maxSample)
        maxSample = samples[0];
    }
  #endif

    if (index == buffersize) {
      if (first_write) {
        first_write = 0;
        outputfile->open(fullfilename, std::ios::out);
        outputfile->write("TAO OUTPUT FILE", 15);
        int audio_rate = tao_->synthesisEngine.audioSampleRate;
        outputfile->write((char *)&audio_rate,
                          (int)sizeof(audio_rate));
        const size_t numChannels = samples.size();
        outputfile->write((char *)&numChannels, (int)sizeof(int));
        outputfile->close();
      }

      outputfile->open(fullfilename, std::ios::app);
      outputfile->write((char *)(&buffer[0]), (int)(buffersize * sizeof(float)));
      outputfile->close();
      index = 0;
    }
  }
}

void TaoOutput::display() {
  if (!tao_->graphics_engine_)
    return;
  if (!tao_->graphics_engine_->active)
    return;
  if (tao_->synthesisEngine.tick % tao_->graphics_engine_->refreshRate != 0)
    return;

  displayStream->seekp(std::ios::beg);

  *displayStream << name;

  // TODO(lucasw) for loop through all samples
  if (samples.size() == 1) {
    *displayStream << " 1:    " << samples[0] << "   Max:     " << maxSample
                   << std::ends;
  }
  if (samples.size() == 2) {
    *displayStream << " 1:    " << samples[0] << "   2:    " << samples[1]
                   << "   Max:     " << maxSample << std::ends;
  }

  // tao_->graphics_engine_->displayCharString(10.0, myDisplayPosition,
  // displayString);

  // mic1 -  L:    1338.0 R:    0.932  Max:    1340.0
  // not yet implemented
  // Just had a good idea:
  //    Could display a list of outputs and their positions in text
  //    form at top left of graphics window. Could also display current
  //    sample value and max sample value.
}
