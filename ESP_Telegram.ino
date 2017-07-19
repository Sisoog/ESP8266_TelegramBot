#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "ZeusTgBot.h"
#include <EEPROM.h>


ESP8266WiFiMulti WiFiMulti;
#define Ssid	 	"Your_WIFI_SSID"
#define Password	"Your_WIFI_PASS"
#define Bot_Key		"Your_BOT_Key"

Zeus_TgBot	Bot(Bot_Key);

// Witty Cloud Board specifc pins
const int LDR = A0;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;

void Tg_Message_Prossess(String mgs_id,String sender_id,String msg,bool is_callback,String Call_ID);
uint32_t Command_Run = 0;

void setup()
{
// Add your initialization code here
	Serial.begin(115200);
	Serial.print("\n");
	//Serial.setDebugOutput(true);

	// We start by connecting to a WiFi network
	WiFiMulti.addAP(Ssid, Password);

	Serial.println();
	Serial.println();
	Serial.print("Wait for WiFi... ");

	/*Wait For Connect to Server*/
	while(WiFiMulti.run() != WL_CONNECTED)
	{
	    Serial.print(".");
	    delay(500);
	}
	Serial.println();
	Serial.println("Connect to NetWork .... OK");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	// Initialize LDR, Button and RGB LED
	pinMode(LDR, INPUT);
	pinMode(BUTTON, INPUT);
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);

	Bot.Set_Message_Event(&Tg_Message_Prossess);
	Zeus_TgBot::User_t	User;
	if(Bot.GetMe(&User))
	{
		Serial.println();
		Serial.println("Bot Information");
		Serial.println("first name: " + User.first_name);
		Serial.println("last_name: " + User.last_name);
		Serial.println("username: " + User.username);
		Serial.println();
	}
	else
	{
		Serial.println("Can Not Get Bot Info");
	}
}

void Tg_Message_Prossess(String mgs_id,String sender_id,String msg,bool is_callback,String Call_ID)
{

	String Intor = "ربات تلگرام سیسوگ \n\
این ربات توسط تیم سیسوگ برنامه ریزی و با استفاده از آردوینو راه اندازی شده است \n\
با استفاده از این ربات قادر خواهید بود لوازم منزل را از راه دور و با استفاده از تلگرام کنترل کنید.\n\
از منوی زیر یکی از کلید ها را انتخاب کنید.";

	String keyboardJson = "[[\{ \"text\" : \"مشاهده سایت سیسوگ\", \"url\" : \"https://sisoog.com\" \} ],"
						  "[\{ \"text\" : \"عضویت در تلگرام سیسوگ\", \"url\" : \"https://t.me/joinchat/AAAAAEBAGr-XNdPMm3bTIg\" \} ],"
						  "[\{ \"text\" : \"تغییر وضعیت لامپ سبز\", \"callback_data\" : \"Gtg\" \} ],"
						  "[\{ \"text\" : \"تغییر وضعیت لامپ قرمز\", \"callback_data\" : \"Rtg\" \} ],"
						  "[\{ \"text\" : \"تغییر وضعیت لامپ آبی\", \"callback_data\" : \"Btg\" \} ]]";


	if(is_callback==true)
	{
		/*it's Call Back Data*/
		Serial.println("Get CallBack - " + msg);
		if(msg=="Gtg") /*Command For Green LED*/
		{
			if(digitalRead(GREEN))
			{
				digitalWrite(GREEN, false);
				Bot.answerCallbackQuery(Call_ID,"لامپ سبز خاموش شد");
			}
			else
			{
				digitalWrite(GREEN, true);
				Bot.answerCallbackQuery(Call_ID,"لامپ سبز روشن شد");
			}
		}

		if(msg=="Rtg") /*Command For Green LED*/
		{
			if(digitalRead(RED))
			{
				digitalWrite(RED, false);
				Bot.answerCallbackQuery(Call_ID,"لامپ قرمز خاموش شد");
			}
			else
			{
				digitalWrite(RED, true);
				Bot.answerCallbackQuery(Call_ID,"لامپ قرمز روشن شد");
			}
		}

		if(msg=="Btg") /*Command For Green LED*/
		{
			if(digitalRead(BLUE))
			{
				digitalWrite(BLUE, false);
				Bot.answerCallbackQuery(Call_ID,"لامپ آبی خاموش شد");
			}
			else
			{
				digitalWrite(BLUE, true);
				Bot.answerCallbackQuery(Call_ID,"لامپ آبی روشن شد");
			}
		}

		Intor += "\n\n وضعیت لامپ ها\n";
		Intor += "\n سبز: ";
		Intor += (digitalRead(GREEN)) ? "روشن":"خاموش";
		Intor += "\n قرمز: ";
		Intor += (digitalRead(RED)) ? "روشن":"خاموش";
		Intor += "\n آبی: ";
		Intor += (digitalRead(BLUE)) ? "روشن":"خاموش";
		Intor += "\n\nتعداد درخواست های پردازش شده: " + String(Command_Run);
		Intor += "\n\nسیسوگ";
		Command_Run++;

		Bot.EditMessage(mgs_id, sender_id, Intor, keyboardJson);
	}
	else
	{
		Serial.println("Get Message - " + msg);
		Bot.sendMessage(sender_id, Intor, keyboardJson);
	}

}


// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	Bot.GetUpdates();
	delay(100);

}
