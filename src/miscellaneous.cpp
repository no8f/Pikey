#include <Pikey/miscellaneous.h>
#include <dpp/dpp.h>
#include <Pikey/daily.h>

void Pikey::createMsgListeners(dpp::cluster *bot) {
	bot->on_message_create([bot](const dpp::message_create_t &event) {
		if (event.msg.content.find("!cat") != std::string::npos) {
			bot->request("https://cataas.com/cat", dpp::m_get,
						 [event](const dpp::http_request_completion_t &httpRequestCompletion) {
							 dpp::message msg("Here's your cat: /ᐠ-˕-マⳊ");

							 if (httpRequestCompletion.status == 200)
								 msg.add_file("cat.png", httpRequestCompletion.body);

							 event.reply(msg, true);
						 });
		} else if (event.msg.content.find("!testDaily") != std::string::npos) {
			Pikey::sendDaily(bot);
		}
	});
}
