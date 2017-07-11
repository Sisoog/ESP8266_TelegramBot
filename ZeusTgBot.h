/*
 * ZeusTgBot.h
 *
 *  Created on: Jul 9, 2017
 *      Author: Mazarei
 */

#ifndef ZEUSTGBOT_H_
#define ZEUSTGBOT_H_
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "ArduinoJson.h"


typedef void (*Message_Event) (uint32_t mgs_id,uint32_t sender_id,String msg);

class Zeus_TgBot {
public:

	typedef struct
	{
		uint32_t	id;
		String		first_name;
		String		last_name;
		String		username;
	}User_t;

	Zeus_TgBot(String token);
	virtual ~Zeus_TgBot();

	void Set_Message_Event(Message_Event Event_Call);
	bool GetMe(User_t *User);
	bool GetUpdates();
	bool sendMessage(uint32_t chat_id, String text);

private:
	String botkey;
	StaticJsonBuffer<4096> jsonBuffer;
	WiFiClientSecure client;
	volatile uint32_t	UpdateID;

	String SendCommand(String command);
	void ProssessOneMessage(String msg);
	Message_Event Func_Message_Event;
};

#endif /* ZEUSTGBOT_H_ */
