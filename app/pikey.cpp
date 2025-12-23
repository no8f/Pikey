#include <Ext/ini.h>
#include <dpp/dpp.h>
#include <string>

#include <Pikey/miscellaneous.h>
#include <Pikey/daily.h>

int main(int argc, char *argv[]) {

	std::cout << "Starting Pikey..." << std::endl;
	ini::File iniFile;

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			std::string s (argv[i]);
			if ( s == "--gen_ini" ) {
				std::cout << "Generating default ini file..." << std::endl;
				iniFile["general"].set<std::string>("Token", "");
				iniFile["general"].set<std::string>("DailyChannelID", "");

				iniFile["time"].set<int>("Hour", 11);
				iniFile["time"].set<int>("Minute", 0);

				iniFile["message"].set<std::string>("Title", "Daily Daily reminder!");
				iniFile["message"].set<std::string>("Field_Header", "What are you guys/gurls doing today?");
				iniFile["message"].set<std::string>("Field_Content", "Send a quick update or react if you are doing nothing :)");
				iniFile["message"].set<std::string>("Footer", "Your friendly slave master - Pikey");

				iniFile.write("settings.ini");
				return 0;
			}
		}
	}

	try {
		iniFile = ini::open("settings.ini");
	} catch (std::invalid_argument& e) {
		std::cout << "ERROR: no ini file found. Generate one by passing the '--gen_ini' argument" << std::endl;
		return -1;
	};

	dpp::cluster bot(iniFile["general"].get<std::string>("Token"), dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());

	Pikey::createMsgListeners(&bot);

	std::thread t(Pikey::setupDaily, iniFile["time"].get<int>("Hour"), iniFile["time"].get<int>("Minute"), &bot);
	t.detach();

	bot.on_ready([&bot](const dpp::ready_t &event) {
		if (dpp::run_once<struct register_bot_commands>()) {
		}

		if (dpp::run_once<struct clear_bot_commands>()) {
			bot.global_bulk_command_delete();
		}
	});

	bot.start();

	return 0;
}
