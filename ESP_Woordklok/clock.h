#ifndef CLOCK_H_
#define CLOCK_H_

#include "helpers.h"
#include "log.h"

class Clock {
	public:
		static void setTime() {
			setTime(hour(), minute(), second());
		}

		static void setTime(int hour, int minute, int second) {
			String cmd = "SET TIME " + FormatTime(hour) + ":" + FormatTime(minute) + ":" + FormatTime(second);
			runCommand(cmd);
		}

		static void getTime() {
			runCommand("GET TIME");
		}

		static void getTouch() {
			runCommand("GET TOUCH");
		}

		static void getSound() {
			runCommand("GET SOUND");
		}

		static void getLight() {
			runCommand("GET LIGHT");
		}

		static void getMode() {
			runCommand("GET MODE");
		}

		static void setMode(int mode) {
			runCommand("SET MODE " + (String) mode);
		}

		static void getTComp()  {
			runCommand("GET TCOMP");
		}

		static void customCmd(const String &cmd) {
			runCommand(cmd);
		}

		static void setNotation(int notation) {
			runCommand("SET NOTAT " + (String)notation);
		}

		static void setLMin(int min) {
			runCommand("SET LMIN " + (String) min);
		}

		static void setLMax(int max) {
			runCommand("SET LMAX " + (String) max);
		}

		static void enableSound(bool enable) {
			String cmd = String("SET SOUND ") + (enable ? String("1") : String("0"));
			runCommand(cmd);
		}

		static void enableTouch(bool enable) {
			String cmd = String("SET TOUCH ") + (enable ? String("1") : String("0"));
			runCommand(cmd);
		}

		static void setTouch(int touchFil, int touchTrH, int touchTrL, int touchTiS, int touchTiL) {
			String cmd = "SET TOUCH " + (String) touchFil + " "
					+ (String) touchTrH + " " + (String) touchTrL + " "
					+ (String) touchTiS + " " + (String) touchTiL;
			runCommand(cmd);
		}

	private:
		static void runCommand(const String &cmd) {
			Serial.println(cmd);
			WriteLogLine(cmd);
		}
};



#endif /* CLOCK_H_ */
