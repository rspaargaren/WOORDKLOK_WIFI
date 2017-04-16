#include "WifiController.h"
#include "log.h"

#include <ESP8266WiFi.h>

extern "C" {
	#include "user_interface.h"
}

WifiController::WifiController(const char *apName, const char *apPassword)
	: m_apName(apName)
	, m_apPassword(apPassword)
	, m_configPortalTimeout(0)
	, m_configPortalExtendedTimeout(0)
	, m_connectTimeout(0)
	, m_start(0)
	, m_setupAPOnSTAFailure(true)
	, m_state(WifiController_Idle) {
}

WifiController::~WifiController() {
}

void WifiController::setSTAStaticIPConfig(const IPAddress &ip, const IPAddress &gw, const IPAddress &sn) {
	m_staStaticIp = ip;
	m_staStaticGw = gw;
	m_staStaticSn = sn;
}

void WifiController::autoConnect(const char *apName, const char *apPassword) {
	debug_wc_print("AutoConnect");

	// attempt to connect; should it fail, fall back to AP
	WiFi.mode(WIFI_STA);
	connectWifi(apName, apPassword);
}

void WifiController::startConfigPortal(const char *apName, const char *apPassword) {
	debug_wc_print("starting AP");
	WiFi.disconnect();
	WiFi.setAutoReconnect(false);
	m_state = WifiController_AP;
	m_start = millis();

	//setup AP
	WiFi.mode(WIFI_AP_STA);

	m_apName = apName;
	m_apPassword = apPassword;

	debug_wc_print("Configuring access point... ");
	debug_wc_print(m_apName);

	if (m_apPassword != NULL) {
		WiFi.softAP(m_apName, m_apPassword);
	} else {
		WiFi.softAP(m_apName);
	}
	delay(500); // Without delay I've seen the IP address blank
	debug_wc_print("AP IP address: ");
	debug_wc_print(WiFi.softAPIP());
}

void WifiController::connectWifi(const String &ssid, const String &pass) {
	debug_wc_print("Connecting as wifi client...");

	m_staSSID = String(ssid);
	m_staPasswd = String(pass);

	// check if we've got static_ip settings, if we do, use those.
	if (m_staStaticIp) {
		debug_wc_print("Custom STA IP/GW/Subnet");
		WiFi.config(m_staStaticIp, m_staStaticGw, m_staStaticSn);
		debug_wc_print(WiFi.localIP());
	}
	//fix for auto connect racing issue
	if (WiFi.status() == WL_CONNECTED) {
		debug_wc_print("Already connected. Bailing out.");
		return;
	}
	//check if we have ssid and pass and force those, if not, try with last saved values
	if (ssid != "") {
		m_start = millis();
		m_state = WifiController_STA;
		WiFi.begin(ssid.c_str(), pass.c_str());
	} else {
		if (WiFi.SSID()) {
			debug_wc_print("Using last saved values, should be faster");
			//trying to fix connection in progress hanging
			ETS_UART_INTR_DISABLE();
			wifi_station_disconnect();
			ETS_UART_INTR_ENABLE();
			m_start = millis();
			m_state = WifiController_STA;
			WiFi.begin();
		} else {
			debug_wc_print("No saved credentials");
		}
	}
}

void WifiController::update() {
	if (m_state == WifiController_STA) {
		// are we trying to connect as a client
		uint8_t status = WiFi.status();
		if (status == WL_CONNECTED) {
			debug_wc_print("Client connection succeeded");
			m_state = WifiController_Idle;
			WiFi.mode(WIFI_STA);
			WiFi.setAutoReconnect(true);
			m_setupAPOnSTAFailure = false;
		} else {
			if ((status == WL_CONNECT_FAILED) || (millis() > m_start + m_connectTimeout)) {
				if (status == WL_CONNECT_FAILED) {
					debug_wc_print("Client connection failure");
				} else {
					debug_wc_print("Client connection timeout");
				}
				m_state = WifiController_Idle;
				WiFi.setAutoReconnect(true);
				if (m_setupAPOnSTAFailure) {
					m_setupAPOnSTAFailure = false;
					startConfigPortal(m_apName, m_apPassword);
				} else {
					WiFi.reconnect();
				}
			}
		}
	} else if (m_state == WifiController_AP || m_state == WifiController_AP_Extended) {
		// are we in access point mode
		bool disconnect = false;
		unsigned long ms = millis();

		if (m_state == WifiController_AP) {
			if (ms > m_start + m_configPortalTimeout) {
				debug_wc_print("Portal timeout");
				if (wifi_softap_get_station_num()) {
					debug_wc_print("Extending portal timeout since a client is still connected.");
					m_state = WifiController_AP_Extended;
					m_start = ms;
				} else {
					disconnect = true;
				}
			}
		}
		if (m_state == WifiController_AP_Extended) {
			if (ms >= m_start + m_configPortalExtendedTimeout) {
				disconnect = true;
			}
		}
		if (disconnect) {
			debug_wc_print("Portal timeout, disconnecting");
			WiFi.setAutoReconnect(true);
			m_state = WifiController_Idle;
			// AP timeout
			WiFi.mode(WIFI_STA);
			if (m_staSSID.length()) {
				if (m_staStaticIp) {
					debug_wc_print("Custom STA IP/GW/Subnet");
					WiFi.config(m_staStaticIp, m_staStaticGw, m_staStaticSn);
					debug_wc_print(WiFi.localIP());
				}
				WiFi.begin(m_staSSID.c_str(), m_staPasswd.c_str());
			}
			m_setupAPOnSTAFailure = false;
			WiFi.reconnect();
		}
	}
}

void WifiController::setConfigPortalTimeout(unsigned long seconds) {
	m_configPortalTimeout = seconds * 1000;
}

void WifiController::setExtendedConfigPortalTimeout(unsigned long seconds) {
	m_configPortalExtendedTimeout = seconds * 1000;
}

void WifiController::setConnectTimeout(unsigned long seconds) {
	m_connectTimeout = seconds * 1000;
}
