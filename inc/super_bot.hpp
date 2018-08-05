#ifndef SUPER_BOT_HPP
#define SUPER_BOT_HPP

#include <sc2api/sc2_api.h>
#include <glog/logging.h>

// - Also make it singleton
class SuperBot : public sc2::Agent {

	SuperBot() : sc2::Agent() {}

public:

	static SuperBot& GetSuperBot()
	{
		static SuperBot rc;
		return rc;
	}

	SuperBot(SuperBot const&) = delete;
	void operator= (SuperBot const&) = delete;

    virtual void OnGameStart() final {
        sc2::renderer::Initialize("Rendered", 50, 50, kMiniMapX + kMapX, std::max(kMiniMapY, kMapY));
    }

	virtual void OnGameEnd() final {
		sc2::renderer::Shutdown();
	}

	virtual void OnStep() final {
		LOG(INFO) << "Game Loop = " << Observation()->GetGameLoop();
		LOG(INFO) << "Minerals = " << Observation()->GetMinerals();
		LOG(INFO) << "Vespene Gas = " << Observation()->GetVespene();
	}
};

#endif
