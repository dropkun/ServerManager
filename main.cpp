#include <dpp/dpp.h>
#include <dotenv.h>

#include <iostream>
#include <thread>
#include <future>
#include <string>

#include "gce.h"

// GCE起動処理
std::string PROJECT_ID = "droprealms";
std::string ZONE = "asia-northeast1-a";
std::string INSTANCE_NAME = "nasi-minecraft";

int main()
{
    const char *token = std::getenv("DISCORD_TOKEN");
    if (token == nullptr)
    {
        std::cerr << "環境変数 DISCORD_TOKEN が見つかりません。\n";
        return 1; // エラー終了
    }
    std::string str(token);
    dpp::cluster bot(str);
    bot.on_log(dpp::utility::cout_logger());

    InstanceController instance(PROJECT_ID,
                                ZONE,
                                INSTANCE_NAME);

    bot.on_slashcommand([&bot, &instance](const dpp::slashcommand_t &event)
                        {
        const auto name = event.command.get_command_name();

        if (name == "ping") {
            event.reply("Pong!");
        } else if (name == "mc") {
            const std::string subcommand = std::get<std::string>(event.get_parameter("action"));

            // 非同期で処理（重い処理をブロックしないように）
            std::thread([event, subcommand, instance]() mutable {
                if (subcommand == "start") {
                    event.reply("Starting a minecraft server...");
                    std::string result = instance.Start();
                    std::string ip = instance.GetExternalIP();
                    event.edit_response(result + "\n" + ip);
                } else if (subcommand == "stop") {
                    event.reply("Stopping a minecraft server...");
                    std::string result = instance.Stop();
                    event.edit_response(result);
                } else {
                    event.reply("未知のサブコマンドです");
                }
            }).detach();
        } });

    // スラッシュコマンド登録
    bot.on_ready([&bot](const dpp::ready_t &event)
                 {
        if (dpp::run_once<struct register_commands>()) {
            dpp::slashcommand ping("ping", "Ping pong!", bot.me.id);
            dpp::slashcommand mc("mc", "Minecraft server controll", bot.me.id);

            mc.add_option(dpp::command_option(dpp::co_string, "action", "start or stop", true)
                          .add_choice(dpp::command_option_choice("start", "start"))
                          .add_choice(dpp::command_option_choice("stop", "stop")));

            bot.global_command_create(ping);
            bot.global_command_create(mc);
        } });

    bot.start(dpp::st_wait);
}