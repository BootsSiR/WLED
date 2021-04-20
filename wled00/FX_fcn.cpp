/*
  WS2812FX_fcn.cpp contains all utility functions
  Harm Aldick - 2016
  www.aldick.org
  LICENSE
  The MIT License (MIT)
  Copyright (c) 2016  Harm Aldick
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

  Modified heavily for WLED
*/

#include "FX.h"
#include "palettes.h"

//enable custom per-LED mapping. This can allow for better effects on matrices or special displays
#define WLED_CUSTOM_LED_MAPPING

#ifdef WLED_CUSTOM_LED_MAPPING
//this is just an example (30 LEDs). It will first set all even, then all uneven LEDs.
const uint16_t customMappingTable[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 
  51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 
  101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 
  151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 
  201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 
  251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 
  301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 
  351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 
  801, 800, 799, 798, 797, 796, 795, 794, 793, 792, 791, 790, 789, 788, 787, 786, 785, 784, 783, 782, 781, 780, 779, 778, 777, 776, 775, 774, 773, 772, 771, 770, 769, 768, 767, 766, 765, 764, 763, 762, 761, 760, 759, 758, 757, 756, 755, 754, 753, 752, 
  751, 750, 749, 748, 747, 746, 745, 744, 743, 742, 741, 740, 739, 738, 737, 736, 735, 734, 733, 732, 731, 730, 729, 728, 727, 726, 725, 724, 723, 722, 721, 720, 719, 718, 717, 716, 715, 714, 713, 712, 711, 710, 709, 708, 707, 706, 705, 704, 703, 702, 
  701, 700, 699, 698, 697, 696, 695, 694, 693, 692, 691, 690, 689, 688, 687, 686, 685, 684, 683, 682, 681, 680, 679, 678, 677, 676, 675, 674, 673, 672, 671, 670, 669, 668, 667, 666, 665, 664, 663, 662, 661, 660, 659, 658, 657, 656, 655, 654, 653, 652, 
  651, 650, 649, 648, 647, 646, 645, 644, 643, 642, 641, 640, 639, 638, 637, 636, 635, 634, 633, 632, 631, 630, 629, 628, 627, 626, 625, 624, 623, 622, 621, 620, 619, 618, 617, 616, 615, 614, 613, 612, 611, 610, 609, 608, 607, 606, 605, 604, 603, 602, 
  601, 600, 599, 598, 597, 596, 595, 594, 593, 592, 591, 590, 589, 588, 587, 586, 585, 584, 583, 582, 581, 580, 579, 578, 577, 576, 575, 574, 573, 572, 571, 570, 569, 568, 567, 566, 565, 564, 563, 562, 561, 560, 559, 558, 557, 556, 555, 554, 553, 552, 
  551, 550, 549, 548, 547, 546, 545, 544, 543, 542, 541, 540, 539, 538, 537, 536, 535, 534, 533, 532, 531, 530, 529, 528, 527, 526, 525, 524, 523, 522, 521, 520, 519, 518, 517, 516, 515, 514, 513, 512, 511, 510, 509, 508, 507, 506, 505, 504, 503, 502, 
  501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 491, 490, 489, 488, 487, 486, 485, 484, 483, 482, 481, 480, 479, 478, 477, 476, 475, 474, 473, 472, 471, 470, 469, 468, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 457, 456, 455, 454, 453, 452, 
  451, 450, 449, 448, 447, 446, 445, 444, 443, 442, 441, 440, 439, 438, 437, 436, 435, 434, 433, 432, 431, 430, 429, 428, 427, 426, 425, 424, 423, 422, 421, 420, 419, 418, 417, 416, 415, 414, 413, 412, 411, 410, 409, 408, 407, 406, 405, 404, 403, 402
};
//another example. Switches direction every 5 LEDs.
/*const uint16_t customMappingTable[] = {
  0, 1, 2, 3, 4, 9, 8, 7, 6, 5, 10, 11, 12, 13, 14,
  19, 18, 17, 16, 15, 20, 21, 22, 23, 24, 29, 28, 27, 26, 25};*/

const uint16_t customMappingSize = sizeof(customMappingTable)/sizeof(uint16_t); //30 in example
#endif

#ifndef PWM_INDEX
#define PWM_INDEX 0
#endif

void WS2812FX::init(bool supportWhite, uint16_t countPixels, bool skipFirst)
{
  if (supportWhite == _useRgbw && countPixels == _length && _skipFirstMode == skipFirst) return;
  RESET_RUNTIME;
  _useRgbw = supportWhite;
  _length = countPixels;
  _skipFirstMode = skipFirst;

  uint8_t ty = 1;
  if (supportWhite) ty = 2;
  _lengthRaw = _length;
  if (_skipFirstMode) {
    _lengthRaw += LED_SKIP_AMOUNT;
  }

  bus->Begin((NeoPixelType)ty, _lengthRaw);

  _segments[0].start = 0;
  _segments[0].stop = _length;

  setBrightness(_brightness);
}

void WS2812FX::service() {
  uint32_t nowUp = millis(); // Be aware, millis() rolls over every 49 days
  now = nowUp + timebase;
  if (nowUp - _lastShow < MIN_SHOW_DELAY) return;
  bool doShow = false;

  for(uint8_t i=0; i < MAX_NUM_SEGMENTS; i++)
  {
    _segment_index = i;

    // reset the segment runtime data if needed, called before isActive to ensure deleted
    // segment's buffers are cleared
    SEGENV.resetIfRequired();

    if (SEGMENT.isActive())
    {
      if(nowUp > SEGENV.next_time || _triggered || (doShow && SEGMENT.mode == 0)) //last is temporary
      {
        if (SEGMENT.grouping == 0) SEGMENT.grouping = 1; //sanity check
        doShow = true;
        uint16_t delay = FRAMETIME;

        if (!SEGMENT.getOption(SEG_OPTION_FREEZE)) { //only run effect function if not frozen
          _virtualSegmentLength = SEGMENT.virtualLength();
          handle_palette();
          delay = (this->*_mode[SEGMENT.mode])(); //effect function
          if (SEGMENT.mode != FX_MODE_HALLOWEEN_EYES) SEGENV.call++;
        }

        SEGENV.next_time = nowUp + delay;
      }
    }
  }
  _virtualSegmentLength = 0;
  if(doShow) {
    yield();
    show();
  }
  _triggered = false;
}

void WS2812FX::setPixelColor(uint16_t n, uint32_t c) {
  uint8_t w = (c >> 24);
  uint8_t r = (c >> 16);
  uint8_t g = (c >>  8);
  uint8_t b =  c       ;
  setPixelColor(n, r, g, b, w);
}

#define REV(i) (_length - 1 - (i))

//used to map from segment index to physical pixel, taking into account grouping, offsets, reverse and mirroring
uint16_t WS2812FX::realPixelIndex(uint16_t i) {
  int16_t iGroup = i * SEGMENT.groupLength();

  /* reverse just an individual segment */
  int16_t realIndex = iGroup;
  if (IS_REVERSE) {
    if (IS_MIRROR) {
      realIndex = (SEGMENT.length() -1) / 2 - iGroup;  //only need to index half the pixels
    } else {
      realIndex = SEGMENT.length() - iGroup - 1;
    }
  }

  realIndex += SEGMENT.start;
  /* Reverse the whole string */
  if (reverseMode) realIndex = REV(realIndex);

  return realIndex;
}

void WS2812FX::setPixelColor(uint16_t i, byte r, byte g, byte b, byte w)
{
  //auto calculate white channel value if enabled
  if (_useRgbw) {
    if (rgbwMode == RGBW_MODE_AUTO_BRIGHTER || (w == 0 && (rgbwMode == RGBW_MODE_DUAL || rgbwMode == RGBW_MODE_LEGACY)))
    {
      //white value is set to lowest RGB channel
      //thank you to @Def3nder!
      w = r < g ? (r < b ? r : b) : (g < b ? g : b);
    } else if (rgbwMode == RGBW_MODE_AUTO_ACCURATE && w == 0)
    {
      w = r < g ? (r < b ? r : b) : (g < b ? g : b);
      r -= w; g -= w; b -= w;
    }
  }

  RgbwColor col;
  col.R = r; col.G = g; col.B = b; col.W = w;

  uint16_t skip = _skipFirstMode ? LED_SKIP_AMOUNT : 0;
  if (SEGLEN) {//from segment

    //color_blend(getpixel, col, SEGMENT.opacity); (pseudocode for future blending of segments)
    if (IS_SEGMENT_ON)
    {
      if (SEGMENT.opacity < 255) {
        col.R = scale8(col.R, SEGMENT.opacity);
        col.G = scale8(col.G, SEGMENT.opacity);
        col.B = scale8(col.B, SEGMENT.opacity);
        col.W = scale8(col.W, SEGMENT.opacity);
      }
    } else {
      col = BLACK;
    }

    /* Set all the pixels in the group, ensuring _skipFirstMode is honored */
    bool reversed = reverseMode ^ IS_REVERSE;
    uint16_t realIndex = realPixelIndex(i);

    for (uint16_t j = 0; j < SEGMENT.grouping; j++) {
      int16_t indexSet = realIndex + (reversed ? -j : j);
      int16_t indexSetRev = indexSet;
      if (reverseMode) indexSetRev = REV(indexSet);
      #ifdef WLED_CUSTOM_LED_MAPPING
      if (indexSet < customMappingSize) indexSet = customMappingTable[indexSet];
      #endif
      if (indexSetRev >= SEGMENT.start && indexSetRev < SEGMENT.stop) {
        bus->SetPixelColor(indexSet + skip, col);
        if (IS_MIRROR) { //set the corresponding mirrored pixel
          if (reverseMode) {
            bus->SetPixelColor(REV(SEGMENT.start) - indexSet + skip + REV(SEGMENT.stop) + 1, col);
          } else {
            bus->SetPixelColor(SEGMENT.stop - indexSet + skip + SEGMENT.start - 1, col);
          }
        }
      }
    }
  } else { //live data, etc.
    if (reverseMode) i = REV(i);
    #ifdef WLED_CUSTOM_LED_MAPPING
    if (i < customMappingSize) i = customMappingTable[i];
    #endif
    bus->SetPixelColor(i + skip, col);
  }
  if (skip && i == 0) {
    for (uint16_t j = 0; j < skip; j++) {
      bus->SetPixelColor(j, RgbwColor(0, 0, 0, 0));
    }
  }
}


//DISCLAIMER
//The following function attemps to calculate the current LED power usage,
//and will limit the brightness to stay below a set amperage threshold.
//It is NOT a measurement and NOT guaranteed to stay within the ablMilliampsMax margin.
//Stay safe with high amperage and have a reasonable safety margin!
//I am NOT to be held liable for burned down garages!

//fine tune power estimation constants for your setup
#define MA_FOR_ESP        100 //how much mA does the ESP use (Wemos D1 about 80mA, ESP32 about 120mA)
                              //you can set it to 0 if the ESP is powered by USB and the LEDs by external

void WS2812FX::show(void) {

  // avoid race condition, caputre _callback value
  show_callback callback = _callback;
  if (callback) callback();

  //power limit calculation
  //each LED can draw up 195075 "power units" (approx. 53mA)
  //one PU is the power it takes to have 1 channel 1 step brighter per brightness step
  //so A=2,R=255,G=0,B=0 would use 510 PU per LED (1mA is about 3700 PU)
  bool useWackyWS2815PowerModel = false;
  byte actualMilliampsPerLed = milliampsPerLed;

  if(milliampsPerLed == 255) {
    useWackyWS2815PowerModel = true;
    actualMilliampsPerLed = 12; // from testing an actual strip
  }

  if (ablMilliampsMax > 149 && actualMilliampsPerLed > 0) //0 mA per LED and too low numbers turn off calculation
  {
    uint32_t puPerMilliamp = 195075 / actualMilliampsPerLed;
    uint32_t powerBudget = (ablMilliampsMax - MA_FOR_ESP) * puPerMilliamp; //100mA for ESP power
    if (powerBudget > puPerMilliamp * _length) //each LED uses about 1mA in standby, exclude that from power budget
    {
      powerBudget -= puPerMilliamp * _length;
    } else
    {
      powerBudget = 0;
    }

    uint32_t powerSum = 0;

    for (uint16_t i = 0; i < _length; i++) //sum up the usage of each LED
    {
      RgbwColor c = bus->GetPixelColorRaw(i);

      if(useWackyWS2815PowerModel)
      {
        // ignore white component on WS2815 power calculation
        powerSum += (MAX(MAX(c.R,c.G),c.B)) * 3;
      }
      else
      {
        powerSum += (c.R + c.G + c.B + c.W);
      }
    }


    if (_useRgbw) //RGBW led total output with white LEDs enabled is still 50mA, so each channel uses less
    {
      powerSum *= 3;
      powerSum = powerSum >> 2; //same as /= 4
    }

    uint32_t powerSum0 = powerSum;
    powerSum *= _brightness;

    if (powerSum > powerBudget) //scale brightness down to stay in current limit
    {
      float scale = (float)powerBudget / (float)powerSum;
      uint16_t scaleI = scale * 255;
      uint8_t scaleB = (scaleI > 255) ? 255 : scaleI;
      uint8_t newBri = scale8(_brightness, scaleB);
      bus->SetBrightness(newBri);
      currentMilliamps = (powerSum0 * newBri) / puPerMilliamp;
    } else
    {
      currentMilliamps = powerSum / puPerMilliamp;
      bus->SetBrightness(_brightness);
    }
    currentMilliamps += MA_FOR_ESP; //add power of ESP back to estimate
    currentMilliamps += _length; //add standby power back to estimate
  } else {
    currentMilliamps = 0;
    bus->SetBrightness(_brightness);
  }

  // some buses send asynchronously and this method will return before
  // all of the data has been sent.
  // See https://github.com/Makuna/NeoPixelBus/wiki/ESP32-NeoMethods#neoesp32rmt-methods
  bus->Show();
  _lastShow = millis();
}

/**
 * Returns a true value if any of the strips are still being updated.
 * On some hardware (ESP32), strip updates are done asynchronously.
 */
bool WS2812FX::isUpdating() {
  return !bus->CanShow();
}

/**
 * Forces the next frame to be computed on all active segments.
 */
void WS2812FX::trigger() {
  _triggered = true;
}

void WS2812FX::setMode(uint8_t segid, uint8_t m) {
  if (segid >= MAX_NUM_SEGMENTS) return;

  if (m >= MODE_COUNT) m = MODE_COUNT - 1;

  if (_segments[segid].mode != m)
  {
    _segment_runtimes[segid].reset();
    _segments[segid].mode = m;
  }
}

uint8_t WS2812FX::getModeCount()
{
  return MODE_COUNT;
}

uint8_t WS2812FX::getPaletteCount()
{
  return 13 + GRADIENT_PALETTE_COUNT;
}

//TODO transitions

bool WS2812FX::setEffectConfig(uint8_t m, uint8_t s, uint8_t in, uint8_t f1, uint8_t f2, uint8_t f3, uint8_t p) {

  uint8_t mainSeg = getMainSegmentId();
  Segment& seg = _segments[getMainSegmentId()];
  uint8_t modePrev = seg.mode, speedPrev = seg.speed, intensityPrev = seg.intensity, fft1Prev = seg.fft1, fft2Prev = seg.fft2, fft3Prev = seg.fft3, palettePrev = seg.palette;

  bool applied = false;

  if (applyToAllSelected) {
    for (uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++)
    {
      if (_segments[i].isSelected())
      {
        _segments[i].speed = s;
        _segments[i].intensity = in;
        _segments[i].fft1 = f1;
        _segments[i].fft2 = f2;
        _segments[i].fft3 = f3;
        _segments[i].palette = p;
        setMode(i, m);
        applied = true;
      }
    }
  }

  if (!applyToAllSelected || !applied) {
    seg.speed = s;
    seg.intensity = in;
    seg.fft1 = f1;
    seg.fft2 = f2;
    seg.fft3 = f3;
    seg.palette = p;
    setMode(mainSegment, m);
  }

  if (seg.mode != modePrev || seg.speed != speedPrev || seg.intensity != intensityPrev || seg.fft1 != fft1Prev || seg.fft2 != fft2Prev || seg.fft3 != fft3Prev || seg.palette != palettePrev) return true;
  return false;
}

void WS2812FX::setColor(uint8_t slot, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  setColor(slot, ((uint32_t)w << 24) |((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

void WS2812FX::setColor(uint8_t slot, uint32_t c) {
  if (slot >= NUM_COLORS) return;

  bool applied = false;

  if (applyToAllSelected) {
    for (uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++)
    {
      if (_segments[i].isSelected()) _segments[i].colors[slot] = c;
    }
  }

  if (!applyToAllSelected || !applied) {
    _segments[getMainSegmentId()].colors[slot] = c;
  }
}

void WS2812FX::setBrightness(uint8_t b) {
  if (gammaCorrectBri) b = gamma8(b);
  if (_brightness == b) return;
  _brightness = b;
  _segment_index = 0;
  if (_brightness == 0) { //unfreeze all segments on power off
    for (uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++)
    {
      _segments[i].setOption(SEG_OPTION_FREEZE, false);
    }
    #if LEDPIN == LED_BUILTIN
      shouldStartBus = true;
    #endif
  } else {
    #if LEDPIN == LED_BUILTIN
      if (shouldStartBus) {
        shouldStartBus = false;
        const uint8_t ty = _useRgbw ? 2 : 1;
        bus->Begin((NeoPixelType)ty, _lengthRaw);
      }
    #endif
  }
  if (SEGENV.next_time > millis() + 22 && millis() - _lastShow > MIN_SHOW_DELAY) show();//apply brightness change immediately if no refresh soon
}

uint8_t WS2812FX::getMode(void) {
  return _segments[getMainSegmentId()].mode;
}

uint8_t WS2812FX::getSpeed(void) {
  return _segments[getMainSegmentId()].speed;
}

uint8_t WS2812FX::getBrightness(void) {
  return _brightness;
}

uint8_t WS2812FX::getMaxSegments(void) {
  return MAX_NUM_SEGMENTS;
}

/*uint8_t WS2812FX::getFirstSelectedSegment(void)
{
  for (uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++)
  {
    if (_segments[i].isActive() && _segments[i].isSelected()) return i;
  }
  for (uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++) //if none selected, get first active
  {
    if (_segments[i].isActive()) return i;
  }
  return 0;
}*/

uint8_t WS2812FX::getMainSegmentId(void) {
  if (mainSegment >= MAX_NUM_SEGMENTS) return 0;
  if (_segments[mainSegment].isActive()) return mainSegment;
  for (uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++) //get first active
  {
    if (_segments[i].isActive()) return i;
  }
  return 0;
}

uint32_t WS2812FX::getColor(void) {
  return _segments[getMainSegmentId()].colors[0];
}

uint32_t WS2812FX::getPixelColor(uint16_t i)
{
  i = realPixelIndex(i);

  #ifdef WLED_CUSTOM_LED_MAPPING
  if (i < customMappingSize) i = customMappingTable[i];
  #endif

  if (_skipFirstMode) i += LED_SKIP_AMOUNT;

  if (i >= _lengthRaw) return 0;

  return bus->GetPixelColorRgbw(i);
}

WS2812FX::Segment& WS2812FX::getSegment(uint8_t id) {
  if (id >= MAX_NUM_SEGMENTS) return _segments[0];
  return _segments[id];
}

WS2812FX::Segment_runtime WS2812FX::getSegmentRuntime(void) {
  return SEGENV;
}

WS2812FX::Segment* WS2812FX::getSegments(void) {
  return _segments;
}

uint32_t WS2812FX::getLastShow(void) {
  return _lastShow;
}

uint8_t WS2812FX::getColorOrder(void) {
  return bus->GetColorOrder();
}

void WS2812FX::setColorOrder(uint8_t co) {
  bus->SetColorOrder(co);
}

void WS2812FX::setSegment(uint8_t n, uint16_t i1, uint16_t i2, uint8_t grouping, uint8_t spacing) {
  if (n >= MAX_NUM_SEGMENTS) return;
  Segment& seg = _segments[n];

  //return if neither bounds nor grouping have changed
  if (seg.start == i1 && seg.stop == i2 && (!grouping || (seg.grouping == grouping && seg.spacing == spacing))) return;

  if (seg.stop) setRange(seg.start, seg.stop -1, 0); //turn old segment range off
  if (i2 <= i1) //disable segment
  {
    seg.stop = 0;
    if (n == mainSegment) //if main segment is deleted, set first active as main segment
    {
      for (uint8_t i = 0; i < MAX_NUM_SEGMENTS; i++)
      {
        if (_segments[i].isActive()) {
          mainSegment = i;
          return;
        }
      }
      mainSegment = 0; //should not happen (always at least one active segment)
    }
    return;
  }
  if (i1 < _length) seg.start = i1;
  seg.stop = i2;
  if (i2 > _length) seg.stop = _length;
  if (grouping) {
    seg.grouping = grouping;
    seg.spacing = spacing;
  }
  _segment_runtimes[n].reset();
}

void WS2812FX::resetSegments() {
  mainSegment = 0;
  memset(_segments, 0, sizeof(_segments));
  //memset(_segment_runtimes, 0, sizeof(_segment_runtimes));
  _segment_index = 0;
  _segments[0].mode = DEFAULT_MODE;
  _segments[0].colors[0] = DEFAULT_COLOR;
  _segments[0].start = 0;
  _segments[0].speed = DEFAULT_SPEED;
  _segments[0].intensity = DEFAULT_INTENSITY;
  _segments[0].stop = _length;
  _segments[0].grouping = 1;
  _segments[0].setOption(SEG_OPTION_SELECTED, 1);
  _segments[0].setOption(SEG_OPTION_ON, 1);
  _segments[0].opacity = 255;

  for (uint16_t i = 1; i < MAX_NUM_SEGMENTS; i++)
  {
    _segments[i].colors[0] = color_wheel(i*51);
    _segments[i].grouping = 1;
    _segments[i].setOption(SEG_OPTION_ON, 1);
    _segments[i].opacity = 255;
    _segments[i].speed = DEFAULT_SPEED;
    _segments[i].intensity = DEFAULT_INTENSITY;
    _segment_runtimes[i].reset();
  }
  _segment_runtimes[0].reset();
}

//After this function is called, setPixelColor() will use that segment (offsets, grouping, ... will apply)
void WS2812FX::setPixelSegment(uint8_t n)
{
  if (n < MAX_NUM_SEGMENTS) {
    _segment_index = n;
    _virtualSegmentLength = SEGMENT.length();
  } else {
    _segment_index = 0;
    _virtualSegmentLength = 0;
  }
}

void WS2812FX::setRange(uint16_t i, uint16_t i2, uint32_t col)
{
  if (i2 >= i)
  {
    for (uint16_t x = i; x <= i2; x++) setPixelColor(x, col);
  } else
  {
    for (uint16_t x = i2; x <= i; x++) setPixelColor(x, col);
  }
}

void WS2812FX::setShowCallback(show_callback cb)
{
  _callback = cb;
}

void WS2812FX::setTransitionMode(bool t)
{
  unsigned long waitMax = millis() + 20; //refresh after 20 ms if transition enabled
  for (uint16_t i = 0; i < MAX_NUM_SEGMENTS; i++)
  {
    _segment_index = i;
    SEGMENT.setOption(SEG_OPTION_TRANSITIONAL, t);

    if (t && SEGMENT.mode == FX_MODE_STATIC && SEGENV.next_time > waitMax) SEGENV.next_time = waitMax;
  }
}

/*
 * color blend function
 */
uint32_t WS2812FX::color_blend(uint32_t color1, uint32_t color2, uint8_t blend) {
  if(blend == 0)   return color1;
  if(blend == 255) return color2;

  uint32_t w1 = (color1 >> 24) & 0xff;
  uint32_t r1 = (color1 >> 16) & 0xff;
  uint32_t g1 = (color1 >>  8) & 0xff;
  uint32_t b1 =  color1        & 0xff;

  uint32_t w2 = (color2 >> 24) & 0xff;
  uint32_t r2 = (color2 >> 16) & 0xff;
  uint32_t g2 = (color2 >>  8) & 0xff;
  uint32_t b2 =  color2        & 0xff;

  uint32_t w3 = ((w2 * blend) + (w1 * (255 - blend))) >> 8;
  uint32_t r3 = ((r2 * blend) + (r1 * (255 - blend))) >> 8;
  uint32_t g3 = ((g2 * blend) + (g1 * (255 - blend))) >> 8;
  uint32_t b3 = ((b2 * blend) + (b1 * (255 - blend))) >> 8;

  return ((w3 << 24) | (r3 << 16) | (g3 << 8) | (b3));
}

/*
 * Fills segment with color
 */
void WS2812FX::fill(uint32_t c) {
  for(uint16_t i = 0; i < SEGLEN; i++) {
    setPixelColor(i, c);
  }
}

/*
 * Blends the specified color with the existing pixel color.
 */
void WS2812FX::blendPixelColor(uint16_t n, uint32_t color, uint8_t blend)
{
  setPixelColor(n, color_blend(getPixelColor(n), color, blend));
}

/*
 * fade out function, higher rate = quicker fade
 */
void WS2812FX::fade2black(uint8_t rate) {
  uint32_t color;

  //rate = rate >> 1;
  float mappedRate = (float) map(rate, 0, 255, 1, 100) ;

  mappedRate = mappedRate / 100;

  for(uint16_t i = 0; i < SEGLEN; i++) {
    color = getPixelColor(i);
    int w1 = (color >> 24) & 0xff;
    int r1 = (color >> 16) & 0xff;
    int g1 = (color >>  8) & 0xff;
    int b1 =  color        & 0xff;

    int w = w1 * mappedRate;
    int r = r1 * (mappedRate * 1.05);      // acount for the fact that leds stay red on much lower intensities
    int g = g1 * mappedRate;
    int b = b1 * mappedRate;

    setPixelColor(i, r, g, b, w);
  }
}

/*
 * fade out function, higher rate = quicker fade
 */
void WS2812FX::fade_out(uint8_t rate) {
  rate = (255-rate) >> 1;
  float mappedRate = float(rate) +1.1;

  uint32_t color = SEGCOLOR(1); // target color
  int w2 = (color >> 24) & 0xff;
  int r2 = (color >> 16) & 0xff;
  int g2 = (color >>  8) & 0xff;
  int b2 =  color        & 0xff;

  for(uint16_t i = 0; i < SEGLEN; i++) {
    color = getPixelColor(i);
    int w1 = (color >> 24) & 0xff;
    int r1 = (color >> 16) & 0xff;
    int g1 = (color >>  8) & 0xff;
    int b1 =  color        & 0xff;

    int wdelta = (w2 - w1) / mappedRate;
    int rdelta = (r2 - r1) / mappedRate;
    int gdelta = (g2 - g1) / mappedRate;
    int bdelta = (b2 - b1) / mappedRate;

    // if fade isn't complete, make sure delta is at least 1 (fixes rounding issues)
    wdelta += (w2 == w1) ? 0 : (w2 > w1) ? 1 : -1;
    rdelta += (r2 == r1) ? 0 : (r2 > r1) ? 1 : -1;
    gdelta += (g2 == g1) ? 0 : (g2 > g1) ? 1 : -1;
    bdelta += (b2 == b1) ? 0 : (b2 > b1) ? 1 : -1;

    setPixelColor(i, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
  }
}

/*
 * blurs segment content, source: FastLED colorutils.cpp
 */
void WS2812FX::blur(uint8_t blur_amount)
{
  uint8_t keep = 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;
  CRGB carryover = CRGB::Black;
  for(uint16_t i = 0; i < SEGLEN; i++)
  {
    CRGB cur = col_to_crgb(getPixelColor(i));
    CRGB part = cur;
    part.nscale8(seep);
    cur.nscale8(keep);
    cur += carryover;
    if(i > 0) {
      uint32_t c = getPixelColor(i-1);
      uint8_t r = (c >> 16 & 0xFF);
      uint8_t g = (c >> 8  & 0xFF);
      uint8_t b = (c       & 0xFF);
      setPixelColor(i-1, qadd8(r, part.red), qadd8(g, part.green), qadd8(b, part.blue));
    }
    setPixelColor(i,cur.red, cur.green, cur.blue);
    carryover = part;
  }
}

uint16_t WS2812FX::triwave16(uint16_t in)
{
  if (in < 0x8000) return in *2;
  return 0xFFFF - (in - 0x8000)*2;
}

/*
 * Generates a tristate square wave w/ attac & decay
 * @param x input value 0-255
 * @param pulsewidth 0-127
 * @param attdec attac & decay, max. pulsewidth / 2
 * @returns signed waveform value
 */
int8_t WS2812FX::tristate_square8(uint8_t x, uint8_t pulsewidth, uint8_t attdec) {
  int8_t a = 127;
  if (x > 127) {
    a = -127;
    x -= 127;
  }

  if (x < attdec) { //inc to max
    return (int16_t) x * a / attdec;
  }
  else if (x < pulsewidth - attdec) { //max
    return a;
  }
  else if (x < pulsewidth) { //dec to 0
    return (int16_t) (pulsewidth - x) * a / attdec;
  }
  return 0;
}

/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
uint32_t WS2812FX::color_wheel(uint8_t pos) {
  if (SEGMENT.palette) return color_from_palette(pos, false, true, 0);
  pos = 255 - pos;
  if(pos < 85) {
    return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
  } else if(pos < 170) {
    pos -= 85;
    return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
  } else {
    pos -= 170;
    return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
  }
}

/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
uint8_t WS2812FX::get_random_wheel_index(uint8_t pos) {
  uint8_t r = 0, x = 0, y = 0, d = 0;

  while(d < 42) {
    r = random8();
    x = abs(pos - r);
    y = 255 - x;
    d = MIN(x, y);
  }
  return r;
}


uint32_t WS2812FX::crgb_to_col(CRGB fastled)
{
  return (((uint32_t)fastled.red << 16) | ((uint32_t)fastled.green << 8) | fastled.blue);
}


CRGB WS2812FX::col_to_crgb(uint32_t color)
{
  CRGB fastled_col;
  fastled_col.red =   (color >> 16 & 0xFF);
  fastled_col.green = (color >> 8  & 0xFF);
  fastled_col.blue =  (color       & 0xFF);
  return fastled_col;
}


void WS2812FX::load_gradient_palette(uint8_t index)
{
  byte i = constrain(index, 0, GRADIENT_PALETTE_COUNT -1);
  byte tcp[72]; //support gradient palettes with up to 18 entries
  memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[i])), 72);
  targetPalette.loadDynamicGradientPalette(tcp);
}


/*
 * FastLED palette modes helper function. Limitation: Due to memory reasons, multiple active segments with FastLED will disable the Palette transitions
 */
void WS2812FX::handle_palette(void)
{
  bool singleSegmentMode = (_segment_index == _segment_index_palette_last);
  _segment_index_palette_last = _segment_index;

  byte paletteIndex = SEGMENT.palette;
  if (paletteIndex == 0) //default palette. Differs depending on effect
  {
    switch (SEGMENT.mode)
    {
      case FX_MODE_FIRE_2012  : paletteIndex = 35; break; //heat palette
      case FX_MODE_COLORWAVES : paletteIndex = 26; break; //landscape 33
      case FX_MODE_FILLNOISE8 : paletteIndex =  9; break; //ocean colors
      case FX_MODE_NOISE16_1  : paletteIndex = 20; break; //Drywet
      case FX_MODE_NOISE16_2  : paletteIndex = 43; break; //Blue cyan yellow
      case FX_MODE_NOISE16_3  : paletteIndex = 35; break; //heat palette
      case FX_MODE_NOISE16_4  : paletteIndex = 26; break; //landscape 33
      case FX_MODE_GLITTER    : paletteIndex = 11; break; //rainbow colors
      case FX_MODE_SUNRISE    : paletteIndex = 35; break; //heat palette
      case FX_MODE_FLOW       : paletteIndex =  6; break; //party
    }
  }
  if (SEGMENT.mode >= FX_MODE_METEOR && paletteIndex == 0) paletteIndex = 4;

  switch (paletteIndex)
  {
    case 0: //default palette. Exceptions for specific effects above
      targetPalette = PartyColors_p; break;
    case 1: {//periodically replace palette with a random one. Doesn't work with multiple FastLED segments
      if (!singleSegmentMode)
      {
        targetPalette = PartyColors_p; break; //fallback
      }
      if (millis() - _lastPaletteChange > 1000 + ((uint32_t)(255-SEGMENT.intensity))*100)
      {
        targetPalette = CRGBPalette16(
                        CHSV(random8(), 255, random8(128, 255)),
                        CHSV(random8(), 255, random8(128, 255)),
                        CHSV(random8(), 192, random8(128, 255)),
                        CHSV(random8(), 255, random8(128, 255)));
        _lastPaletteChange = millis();
      } break;}
    case 2: {//primary color only
      CRGB prim = col_to_crgb(SEGCOLOR(0));
      targetPalette = CRGBPalette16(prim); break;}
    case 3: {//primary + secondary
      CRGB prim = col_to_crgb(SEGCOLOR(0));
      CRGB sec  = col_to_crgb(SEGCOLOR(1));
      targetPalette = CRGBPalette16(prim,prim,sec,sec); break;}
    case 4: {//primary + secondary + tertiary
      CRGB prim = col_to_crgb(SEGCOLOR(0));
      CRGB sec  = col_to_crgb(SEGCOLOR(1));
      CRGB ter  = col_to_crgb(SEGCOLOR(2));
      targetPalette = CRGBPalette16(ter,sec,prim); break;}
    case 5: {//primary + secondary (+tert if not off), more distinct
      CRGB prim = col_to_crgb(SEGCOLOR(0));
      CRGB sec  = col_to_crgb(SEGCOLOR(1));
      if (SEGCOLOR(2)) {
        CRGB ter = col_to_crgb(SEGCOLOR(2));
        targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,ter,ter,ter,ter,ter,prim);
      } else {
        targetPalette = CRGBPalette16(prim,prim,prim,prim,prim,prim,prim,prim,sec,sec,sec,sec,sec,sec,sec,sec);
      }
      break;}
    case 6: //Party colors
      targetPalette = PartyColors_p; break;
    case 7: //Cloud colors
      targetPalette = CloudColors_p; break;
    case 8: //Lava colors
      targetPalette = LavaColors_p; break;
    case 9: //Ocean colors
      targetPalette = OceanColors_p; break;
    case 10: //Forest colors
      targetPalette = ForestColors_p; break;
    case 11: //Rainbow colors
      targetPalette = RainbowColors_p; break;
    case 12: //Rainbow stripe colors
      targetPalette = RainbowStripeColors_p; break;
    default: //progmem palettes
      load_gradient_palette(paletteIndex -13);
  }

  if (singleSegmentMode && paletteFade && SEGENV.call > 0) //only blend if just one segment uses FastLED mode
  {
    nblendPaletteTowardPalette(currentPalette, targetPalette, 48);
  } else
  {
    currentPalette = targetPalette;
  }
}


/*
 * Gets a single color from the currently selected palette.
 * @param i Palette Index (if mapping is true, the full palette will be SEGLEN long, if false, 255). Will wrap around automatically.
 * @param mapping if true, LED position in segment is considered for color
 * @param wrap FastLED palettes will usally wrap back to the start smoothly. Set false to get a hard edge
 * @param mcol If the default palette 0 is selected, return the standard color 0, 1 or 2 instead. If >2, Party palette is used instead
 * @param pbri Value to scale the brightness of the returned color by. Default is 255. (no scaling)
 * @returns Single color from palette
 */
uint32_t WS2812FX::color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri)
{
  if (SEGMENT.palette == 0 && mcol < 3) {
    uint32_t color = SEGCOLOR(mcol);
    if (pbri != 255) {
      CRGB crgb_color = col_to_crgb(color);
      crgb_color.nscale8_video(pbri);
      return crgb_to_col(crgb_color);
    } else {
      return color;
    }
  }

  uint8_t paletteIndex = i;
  if (mapping) paletteIndex = (i*255)/(SEGLEN -1);
  if (!wrap) paletteIndex = scale8(paletteIndex, 240); //cut off blend at palette "end"
  CRGB fastled_col;
  fastled_col = ColorFromPalette( currentPalette, paletteIndex, pbri, (paletteBlend == 3)? NOBLEND:LINEARBLEND);

  return crgb_to_col(fastled_col);
}

//@returns `true` if color, mode, speed, intensity and palette match
bool WS2812FX::segmentsAreIdentical(Segment* a, Segment* b)
{
  //if (a->start != b->start) return false;
  //if (a->stop != b->stop) return false;
  for (uint8_t i = 0; i < NUM_COLORS; i++)
  {
    if (a->colors[i] != b->colors[i]) return false;
  }
  if (a->mode != b->mode) return false;
  if (a->speed != b->speed) return false;
  if (a->intensity != b->intensity) return false;
  if (a->fft1 != b->fft1) return false;
  if (a->fft2 != b->fft2) return false;
  if (a->fft3 != b->fft3) return false;
  if (a->palette != b->palette) return false;
  //if (a->getOption(SEG_OPTION_REVERSED) != b->getOption(SEG_OPTION_REVERSED)) return false;
  return true;
}

#ifdef WLED_USE_ANALOG_LEDS
void WS2812FX::setRgbwPwm(void) {
  uint32_t nowUp = millis(); // Be aware, millis() rolls over every 49 days
  if (nowUp - _analogLastShow < MIN_SHOW_DELAY) return;

  _analogLastShow = nowUp;

  RgbwColor c;
  uint32_t col = bus->GetPixelColorRgbw(PWM_INDEX);
  c.R = col >> 16; c.G = col >> 8; c.B = col; c.W = col >> 24;

  byte b = getBrightness();
  if (c == _analogLastColor && b == _analogLastBri) return;

  // check color values for Warm / Cold white mix (for RGBW)  // EsplanexaDevice.cpp
  #ifdef WLED_USE_5CH_LEDS
    if        (c.R == 255 && c.G == 255 && c.B == 255 && c.W == 255) {
      bus->SetRgbwPwm(0, 0, 0,                  0, c.W * b / 255);
    } else if (c.R == 127 && c.G == 127 && c.B == 127 && c.W == 255) {
      bus->SetRgbwPwm(0, 0, 0, c.W * b / 512, c.W * b / 255);
    } else if (c.R ==   0 && c.G ==   0 && c.B ==   0 && c.W == 255) {
      bus->SetRgbwPwm(0, 0, 0, c.W * b / 255,                  0);
    } else if (c.R == 130 && c.G ==  90 && c.B ==   0 && c.W == 255) {
      bus->SetRgbwPwm(0, 0, 0, c.W * b / 255, c.W * b / 512);
    } else if (c.R == 255 && c.G == 153 && c.B ==   0 && c.W == 255) {
      bus->SetRgbwPwm(0, 0, 0, c.W * b / 255,                  0);
    } else {  // not only white colors
      bus->SetRgbwPwm(c.R * b / 255, c.G * b / 255, c.B * b / 255, c.W * b / 255);
    }
  #else
    bus->SetRgbwPwm(c.R * b / 255, c.G * b / 255, c.B * b / 255, c.W * b / 255);
  #endif
  _analogLastColor = c;
  _analogLastBri = b;
}
#else
void WS2812FX::setRgbwPwm() {}
#endif

//gamma 2.8 lookup table used for color correction
byte gammaT[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

uint8_t WS2812FX::gamma8_cal(uint8_t b, float gamma) {
  return (int)(pow((float)b / 255.0, gamma) * 255 + 0.5);
}

void WS2812FX::calcGammaTable(float gamma)
{
  for (uint16_t i = 0; i < 256; i++) {
    gammaT[i] = gamma8_cal(i, gamma);
  }
}

uint8_t WS2812FX::gamma8(uint8_t b)
{
  return gammaT[b];
}

uint32_t WS2812FX::gamma32(uint32_t color)
{
  if (!gammaCorrectCol) return color;
  uint8_t w = (color >> 24);
  uint8_t r = (color >> 16);
  uint8_t g = (color >>  8);
  uint8_t b =  color;
  w = gammaT[w];
  r = gammaT[r];
  g = gammaT[g];
  b = gammaT[b];
  return ((w << 24) | (r << 16) | (g << 8) | (b));
}

uint16_t WS2812FX::_usedSegmentData = 0;
