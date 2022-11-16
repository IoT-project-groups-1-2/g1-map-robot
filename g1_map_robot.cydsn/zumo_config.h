#ifndef ZUMO_CONFIG_H_
#define ZUMO_CONFIG_H_

/* Change the following definition to 0 to run in normal mode and to 1 to
 * enable simulator mode  */
#define ZUMO_SIMULATOR 0
#define LIDAR 1

/* Change the following definition to 0 to disable MQTT and to 1 to enable MQTT
 */
#define START_MQTT 1

/* The following definitions must all be in double quotes */
#define NETWORK_SSID "DavyJones"

#define NETWORK_PASSWORD "63PaltsiPhone"

#define MQTT_BROKER "172.20.10.9"

#define MQTT_CLIENT_ID "Zumo101"

#endif