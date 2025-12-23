#include <Ext/ini.h>
#include <Pikey/daily.h>

#include <dpp/unicode_emoji.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

bool isSunday() {
    std::time_t now = std::time(0); 
    std::tm *ltm = std::localtime(&now); 
    return (ltm->tm_wday == 0);  // Sunday is 0
}

bool isTimeToRun(int target_hour, int target_minute) {
    std::time_t now = std::time(0); 
    std::tm *ltm = std::localtime(&now);
    
    return (ltm->tm_hour == target_hour && ltm->tm_min == target_minute);
}

void Pikey::sendDaily(dpp::cluster *bot) {
	ini::File iniFile = ini::open("settings.ini");

	dpp::embed daily = dpp::embed();
	daily.set_color(dpp::colors::red_blood);
	daily.set_title(iniFile["message"].get<std::string>("Title"));
	daily.add_field(iniFile["message"].get<std::string>("Field_Header"), iniFile["message"].get<std::string>("Field_Content"));
	daily.set_footer(dpp::embed_footer().set_text(iniFile["message"].get<std::string>("Footer")));

	dpp::snowflake channelID = iniFile["general"].get<uint64_t>("DailyChannelID");
	dpp::message msg(channelID, daily);

	bot->message_create(msg, [bot](const dpp::confirmation_callback_t& cl) {
		if (!cl.is_error()) {
			bot->message_add_reaction(cl.get<dpp::message>(), dpp::unicode_emoji::x);
		}
	});
}

void Pikey::setupDaily(int target_hour, int target_minute, dpp::cluster *bot) {
	while (true) {
        if (!isSunday() && isTimeToRun(target_hour, target_minute)) {
            sendDaily(bot);
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}
