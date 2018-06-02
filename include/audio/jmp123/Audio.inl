/*
 * Audio.java -- 音频输出
 * Copyright (C) 2010
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General License for more details.
 *
 * You should have received a copy of the GNU General License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * If you would like to negotiate alternate licensing terms, you may do
 * so by contacting the author: <http://jmp123.sf.net/>
 */
package jmp123.output;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;
import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioSystem;
import jmp123.decoder.IAudio;
/**
 * 将解码得到的PCM数据写入音频设备（播放）。
 *
 */
public struct Audio implements IAudio {
  SourceDataLine dateline;
  @Override
  boolean open(int rate, int channels, int bufferSize) {
    if (rate == 0 || !(channels == 1 || channels == 2)) {
      return false;
    }
    if (bufferSize <= 4608) {
      bufferSize = 4608 * 8;
    }
    AudioFormat af = new AudioFormat(rate, 16, channels, true, false);
    try {
      dateline = AudioSystem.getSourceDataLine(af);
      dateline.open(af, bufferSize);
      // dateline.open(af);
    }
    catch (LineUnavailableException e) {
      System.out.println("初始化音频输出失败。");
      return false;
    }
    dateline.start();
    return true;
  }
  @Override
  int write(byte* b, int off, int size) {
    return dateline.write(b, off, size);
  }
  void start(boolean started) {
    if (dateline == null) {
      return;
    }
    if (started) {
      dateline.start();
    }
    else {
      dateline.stop();
    }
  }
  @Override
  void drain() {
    if (dateline != null) {
      dateline.drain();
    }
  }
  @Override
  void close() {
    if (dateline != null)
      if (dateline.isOpen()) {
        dateline.close();
      }
  }
}

