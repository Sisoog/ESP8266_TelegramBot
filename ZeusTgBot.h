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

#define TG_HOST 		"api.telegram.org"
#define SSL_PORT 		443
#define LIMIT_Message	1

typedef void (*Message_Event) (String Message_id,String Message_From,String Message_Text,bool is_callback,String Call_ID);

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
	bool sendMessage(String chat_id, String text);
	bool sendMessage(String chat_id, String text, String reply_markup);
	bool answerCallbackQuery(String callback_query_id,String Text);
	bool EditMessage(String msg_id,String chat_id, String text, String reply_markup);

private:
	String botkey;
	WiFiClientSecure client;
	volatile uint32_t	UpdateID;

	String SendCommand(String command, JsonObject& payload);
	void ProssessOneMessage(JsonObject& Message);
	void ProssessOneCallBack(JsonObject& Message);

	Message_Event Func_Message_Event;
	bool is_debug = false;
};

#endif /* ZEUSTGBOT_H_ */
