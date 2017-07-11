#include <stdint.h>

#ifndef APA102_H_
#define APA102_H_

#include "APA102_imp.hpp"

class APA102_led {
	public:
		APA102_led() {
			reset();
		}
		
		void reset(void) {
			red = 0;
			green = 0;
			blue = 0;
		}
		
		void setRed(uint8_t newRed) {
			red = newRed;
		}
		void setGreen(uint8_t newGreen) {
			green = newGreen;
		}
		void setBlue(uint8_t newBlue) {
			blue = newBlue;
		}
		void setRGB(uint8_t newRed, uint8_t newGreen, uint8_t newBlue) {
			red = newRed;
			green = newGreen;
			blue = newBlue;
		}
		
		uint8_t getRed(void) {
			return red;
		}
		uint8_t getGreen(void) {
			return green;
		}
		uint8_t getBlue(void) {
			return blue;
		}
	private:
		uint8_t red;
		uint8_t green;
		uint8_t blue;
};

enum class APA102_ORDER {
	GRB,
	BRG,
	RGB,
	BGR,
	GBR,
	RBG,
};

template <uint8_t _numberOfLEDs>
class APA102 {
	public:
		APA102() {
			reset();
		}
		
		//Call this if something goes wrong.
		void reset(void) {
			//Initialize everything to "off".
			changed = true;
			colorOrder = APA102_ORDER::GRB;
			globalBrightness = 0xE0;
			for(uint8_t i = 0;i < _numberOfLEDs;i++) {
				LEDs[i].reset();
			}
		}
		
		//Set color order of the LEDs
		void setOrder(APA102_ORDER newColorOrder) {
			colorOrder = newColorOrder;
		}

		//Set global brightness of all LEDs
		void setBrightness(uint8_t brightness) {
			changed = true;
			globalBrightness = 0xE0 | brightness;
		}
		
		//Set all LEDs to new colors
		void setRed(uint8_t newRed) {
			changed = true;
			for(uint8_t i = 0;i < _numberOfLEDs;i++) {
				LEDs[i].setRed(newRed);
			}
		}
		void setGreen(uint8_t newGreen) {
			changed = true;
			for(uint8_t i = 0;i < _numberOfLEDs;i++) {
				LEDs[i].setGreen(newGreen);
			}
		}
		void setBlue(uint8_t newBlue) {
			changed = true;
			for(uint8_t i = 0;i < _numberOfLEDs;i++) {
				LEDs[i].setBlue(newBlue);
			}
		}
		void setRGB(uint8_t newRed, uint8_t newGreen, uint8_t newBlue) {
			changed = true;
			for(uint8_t i = 0;i < _numberOfLEDs;i++) {
				LEDs[i].setRGB(newRed, newGreen, newBlue);
			}
		}

		//Set individual LEDs to new colors
		void setRed(uint8_t whichLED, uint8_t newRed) {
			changed = true;
			LEDs[whichLED].setRed(newRed);
		}
		void setGreen(uint8_t whichLED, uint8_t newGreen) {
			changed = true;
			LEDs[whichLED].setGreen(newGreen);
		}
		void setBlue(uint8_t whichLED, uint8_t newBlue) {
			changed = true;
			LEDs[whichLED].setBlue(newBlue);
		}
		void setRGB(uint8_t whichLED, uint8_t newRed, uint8_t newGreen, uint8_t newBlue) {
			changed = true;
			LEDs[whichLED].setRGB(newRed, newGreen, newBlue);
		}
		
		//Set several contiguous LEDs to the same new color
		void setRed(uint8_t startLED, uint8_t endLED, uint8_t newRed) {
			if(endLED > startLED) {
				changed = true;
				for(uint8_t i = startLED;i < (endLED-startLED);i++) {
					LEDs[i].setRed(newRed);
				}
			}
		}
		void setGreen(uint8_t startLED, uint8_t endLED, uint8_t newGreen) {
			if(endLED > startLED) {
				changed = true;
				for(uint8_t i = startLED;i < (endLED-startLED);i++) {
					LEDs[i].setGreen(newGreen);
				}
			}
		}
		void setBlue(uint8_t startLED, uint8_t endLED, uint8_t newBlue) {
			if(endLED > startLED) {
				changed = true;
				for(uint8_t i = startLED;i < (endLED-startLED);i++) {
					LEDs[i].setBlue(newBlue);
				}
			}
		}
		void setRGB(uint8_t startLED, uint8_t endLED, uint8_t newRed, uint8_t newGreen, uint8_t newBlue) {
			if(endLED > startLED) {
				changed = true;
				for(uint8_t i = startLED;i < (endLED-startLED);i++) {
					LEDs[i].setRGB(newRed, newGreen, newBlue);
				}
			}
		}
		
		/*
            Dual-purpose hardware interface function.
            Call this when the SPI port is available to see if this class has anything to send.
            If it does, it will turn around and call APA102_SPI_TX() with what data it needs to send, and then return TRUE to indicate that it had something to send.
            If it doesn't, it will return FALSE.
        */
		bool doSPI(void) {
			if(changed) {
				uint8_t temp[4] = { 0 };

				//Send start bits
				APA102_SPI_TX(temp, 4, true);

				//For each LED, send globalBrightness and the three color bytes.
				temp[0] = globalBrightness;
				for(uint8_t i = 0;i < _numberOfLEDs;i++) {
					switch(colorOrder) {
						case APA102_ORDER::GRB:
							temp[1] = LEDs[i].getGreen();
							temp[2] = LEDs[i].getRed();
							temp[3] = LEDs[i].getBlue();
							break;
						case APA102_ORDER::GBR:
							temp[1] = LEDs[i].getGreen();
							temp[2] = LEDs[i].getBlue();
							temp[3] = LEDs[i].getRed();
							break;
						case APA102_ORDER::BRG:
							temp[1] = LEDs[i].getBlue();
							temp[2] = LEDs[i].getRed();
							temp[3] = LEDs[i].getGreen();
							break;
						case APA102_ORDER::BGR:
							temp[1] = LEDs[i].getBlue();
							temp[2] = LEDs[i].getGreen();
							temp[3] = LEDs[i].getRed();
							break;
						case APA102_ORDER::RGB:
							temp[1] = LEDs[i].getRed();
							temp[2] = LEDs[i].getGreen();
							temp[3] = LEDs[i].getBlue();
							break;
						case APA102_ORDER::RBG:
							temp[1] = LEDs[i].getRed();
							temp[2] = LEDs[i].getBlue();
							temp[3] = LEDs[i].getGreen();
							break;
					}
					APA102_SPI_TX(temp, 4, true);
				}

				//Send a bunch of empty clock pulses to let the LEDs do the right thing, see https://cpldcpu.wordpress.com/2014/11/30/understanding-the-apa102-superled/
					temp[0] = 0;

					//We might need to send more than one byte.
					uint8_t i = 0;
					while( i < ((_numberOfLEDs / 2) / 8) ) {
						APA102_SPI_TX(temp, 1, true);
						i++;
					}

					//But we'll definitely send one byte.
					APA102_SPI_TX(temp, 1, false);

				changed = false;
				return true;
			}
			return false;
		}
		
	private:
		bool changed;
		APA102_ORDER colorOrder;
		uint8_t globalBrightness;
		APA102_led LEDs[_numberOfLEDs];
};

#endif
