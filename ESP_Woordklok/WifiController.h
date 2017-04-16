#ifndef WIFICONTROLLER_H
#define WIFICONTROLLER_H

#include <ESP8266WiFi.h>
#include <WString.h>

/**
 * Helper class for the Wifi WordClock
 * This class will guide the WiFi class to the correct state
 *
 * @note This class is based on the WiFiManager by tzapu
 *       https://github.com/tzapu/WiFiManager
 */
class WifiController {
	public:
		enum State {
			WifiController_Idle,
			WifiController_AP,
			WifiController_AP_Extended,
			WifiController_STA
		};
		WifiController(const char *apName, const char *apPassword);
		~WifiController();

		void setSTAStaticIPConfig(const IPAddress &ip, const IPAddress &gw, const IPAddress &sn);
		void autoConnect(const char *apName, const char *apPassword);
		void startConfigPortal(const char *apName, const char *apPassword);
		void update();

		void setConfigPortalTimeout(unsigned long seconds);
		void setExtendedConfigPortalTimeout(unsigned long seconds);
		void setConnectTimeout(unsigned long seconds);
		void setCreateAPOnFailure(bool enable) {
			m_setupAPOnSTAFailure = enable;
		}

	private:
		void connectWifi(const String &ssid, const String &pass);

		const char *m_apName;
		const char *m_apPassword;

		IPAddress m_staStaticIp;
		IPAddress m_staStaticGw;
		IPAddress m_staStaticSn;
		String m_staSSID;
		String m_staPasswd;

		unsigned long m_configPortalTimeout;
		unsigned long m_configPortalExtendedTimeout;
		unsigned long m_connectTimeout;
		unsigned long m_start;
		bool m_setupAPOnSTAFailure;
		State m_state;

};

#endif // WIFICONTROLLER_H
