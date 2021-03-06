#ifndef __ETHERNET_H_
#define __ETHERNET_H_

#include "board.h"
#include "private.h"
#include "ttydata.h"

#ifdef ESP8266
#   include <WiFiUdp.h>
#   include <ESP8266WiFi.h>
#   define TCP_MAX 5
	// von http://marcotuliogm.github.io/mult-UIP/docs/html/group__uip.html
	typedef struct uip_eth_addr {
		uint8_t addr[6];
	};
	typedef uint16_t 	uip_ip4addr_t [2];
	typedef uip_ip4addr_t 	uip_ipaddr_t;
	typedef struct dhcpc_state {
	};
  class WiFiServerExt : public WiFiServer {
    public:
      WiFiServerExt():WiFiServer(0){}
	};
#endif

class EthernetClass {
public:
	EthernetClass();
	void close(char *);
	void reset(bool);
	void init(void);
	void Task(void);
#ifndef ESP8266
	// NOTE:
	// typedef struct tcplink_state uip_tcp_appstate_t;
	// typedef struct dhcpc_state uip_udp_appstate_t;
	extern uint8_t eth_debug;
	extern uint8_t eth_initialized;
    struct timer periodic_timer, arp_timer;
#else
    void putChar(char data); //von tcp_link.h uebernommen
	uip_ipaddr_t 	uip_hostaddr;
	struct uip_eth_addr uip_ethaddr = {{0,0,0,0,0,0}};
	uint8_t eth_debug;
	uint8_t eth_initialized;
#endif //ESP8266
    static struct uip_eth_addr mac;       // static for dhcpc

	void erip(void *ip, uint8_t *addr);      // EEprom read IP
    void ewip(const uint16_t ip[2], uint8_t *addr); // EEprom write IP
	void udp_appcall(void);
	void tcp_appcall(void);
	void func(char *);
private:
	const char* ssid = STASSID;
	const char* password = STAPSK;
	void display_mac(uint8_t *a);
	void display_ip4(uint8_t*);
	void set_eeprom_addr();
	void dumppkt();
	void ip_initialized(void);
	void dhcpc_configured(const dhcpc_state*);
#ifdef ESP8266
	// buffers for receiving and sending data
	char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
	char  ReplyBuffer[80];// = "acknowledged\r\n";       // a string to send back
	uint8_t ReplyPos;
	WiFiUDP Udp;
	WiFiServerExt server;
	WiFiClient Tcp[TCP_MAX];  // 
	uint8_t tcp_initialized;
	int ip_active;
#endif //ESP8266
};

#ifndef ESP8266
#   define UIP_UDP_APPCALL udp_appcall
#   define UIP_APPCALL     tcp_appcall
#endif //ESP8266

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_Ethernet)
extern EthernetClass Ethernet;
#endif

#endif
