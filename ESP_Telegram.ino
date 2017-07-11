#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "ZeusTgBot.h"



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


void Tg_Message_Prossess(uint32_t mgs_id,uint32_t sender_id,String msg);

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

void Tg_Message_Prossess(uint32_t mgs_id,uint32_t sender_id,String msg)
{

	Serial.println("XXX - "+msg);
	Serial.println(mgs_id);
	Bot.sendMessage(sender_id, msg);
}


// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	digitalWrite(GREEN, true);
	Bot.GetUpdates();
	digitalWrite(GREEN, false);
	delay(1000);

}
