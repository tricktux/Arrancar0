#include <sc2api/sc2_api.h>

#include <glog/logging.h>

#include <rapidjson/document.h>

#include <iostream>


class Bot : public sc2::Agent {
public:
    virtual void OnGameStart() final { LOG(INFO) << "Hello Word!!\n"; }

    virtual void OnStep() final
	{
		LOG(INFO) << "Game Loop = " << Observation()->GetGameLoop() << "\n";
		LOG(INFO) << "Minerals = " << Observation()->GetMinerals() << "\n";
		LOG(INFO) << "Vespene Gas = " << Observation()->GetVespene() << "\n";
    }
};

int main(int argc, char* argv[]) {
	sc2::Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

	rapidjson::Document document;
	document.Parse("~/.ao.json");

	// Initialize Google's logging library.
	google::InitGoogleLogging(argv[0]);

    Bot bot;
    coordinator.SetParticipants({
		sc2::CreateParticipant(sc2::Race::Terran, &bot),
        sc2::CreateComputer(sc2::Race::Zerg)
    });

    coordinator.LaunchStarcraft();
    coordinator.StartGame(sc2::kMapBelShirVestigeLE);

    while (coordinator.Update()) {}

    return 0;
}
