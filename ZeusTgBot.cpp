/*
 * ZeusTgBot.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: Mazarei
 */

#include "ZeusTgBot.h"

Zeus_TgBot::Zeus_TgBot(String token)
{
	botkey = "bot"+token;
	UpdateID = 0;
	Func_Message_Event = NULL;
}

Zeus_TgBot::~Zeus_TgBot()
{

}

void Zeus_TgBot::Set_Message_Event(Message_Event Event_Call)
{
	Func_Message_Event = Event_Call;
}

String Zeus_TgBot::SendCommand(String command)
{
#define Max_MessageSize		1024
    String mess="";
    long now;
    bool avail;

    // Connect with api.telegram.org
    IPAddress server(149,154,167,198);
    if (client.connect(server, 443))
    {
        //Serial.println(".... connected to Telegram server");
        int ch_count=0;
        client.println("GET /"+command);
        now=millis();
        avail=false;
        while (millis()-now<1500)
        {
            while (client.available())
            {
            	  String Answer = client.readString();
				  if (ch_count<Max_MessageSize)
				  {
					 mess +=Answer;
					 ch_count+=Answer.length();
				  }
				  avail=true;
			}

            if (avail)
            {
            	//Serial.println();
            	//Serial.println(mess);
            	//Serial.println();
            	break;
			}
        }
    }
    return mess;
}


bool Zeus_TgBot::GetMe(Zeus_TgBot::User_t *User)
{
	String Command = botkey + "/getme";
	String ServerResult = SendCommand(Command);
	jsonBuffer.clear();
	JsonObject& root = jsonBuffer.parseObject(ServerResult);

	if(root["ok"]==true)
	{
		String Result = root["result"];
		JsonObject& JUser = jsonBuffer.parseObject(Result);
		User->id = JUser["id"];

		String first_name =JUser["first_name"];
		String last_name = JUser["last_name"];
		String username =  JUser["username"];

		User->first_name = first_name;
		User->last_name =last_name;
		User->username =username;

		return true;
	}
	return false;
}

void Zeus_TgBot::ProssessOneMessage(String msg)
{
	StaticJsonBuffer<1024> jsBuf;
	JsonObject& Jmsg = jsBuf.parseObject(msg);
	String Message_Text = Jmsg["text"];
	String Message_From	= Jmsg["from"];
	uint32_t Message_ID	= Jmsg["message_id"];
	jsBuf.clear();
	JsonObject& JFrom = jsBuf.parseObject(Message_From);
	uint32_t Sender_ID	= JFrom["id"];


	jsBuf.clear();
	JsonObject& root = jsonBuffer.createObject();
	root["inline_keyboard"] = "For Test";
	root["url"] = "https://sisoog.com";
	root.printTo(Serial);

	//Serial.println(msg);
	//Serial.println(Message_Text);
	//Serial.println(Message_ID);
	//Serial.println(Sender_ID);
	if(Func_Message_Event!=NULL)
		Func_Message_Event(Message_ID,Sender_ID,Message_Text);
}

bool Zeus_TgBot::GetUpdates()
{
	String Command = botkey + "/getupdates?offset="+UpdateID;
	//Serial.println(Command);
	String ServerResult = SendCommand(Command);
	jsonBuffer.clear();
	JsonObject& root = jsonBuffer.parseObject(ServerResult);
	if(root["ok"]==true)
	{
		int Message_Number = root["result"].size();
		for(int MeaageID = 0;MeaageID<Message_Number;MeaageID++)
		{
			String Message = root["result"][MeaageID];
			//Serial.print(MeaageID);
			//Serial.println(" : "+Message);
			JsonObject& JMsg = jsonBuffer.parseObject(Message);
			uint32_t ID =JMsg["update_id"];
			String Msg = JMsg["message"];
			if(JMsg.size()==2)
			{
				UpdateID = ID+1;
				ProssessOneMessage(Msg);
			}
		}
		return true;
	}
	return false;
}

bool Zeus_TgBot::sendMessage(uint32_t chat_id, String text)
{
	if(text=="")
		return false;

	String command= botkey + "/sendMessage?chat_id="+chat_id+"&text="+text;
	Serial.println(command);
	long StartTime=millis() + 8000;
	while ((long)millis()<(StartTime))   // loop for a while to send the message
	{
		String smess=SendCommand(command);
		jsonBuffer.clear();
		JsonObject& root = jsonBuffer.parseObject(smess);
		if(root["ok"]==true)
		{
			return true;
		}
		delay(1000);
	}

	return false;
}
