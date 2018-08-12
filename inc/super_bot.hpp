#ifndef SUPER_BOT_HPP
#define SUPER_BOT_HPP

#include <sc2api/sc2_api.h>
#include <sc2renderer/sc2_renderer.h>
#include <glog/logging.h>

class CustomRenderer {
	char const *CONFIG_OBJECT = "render";

	static const char *CONFIG_INT_MEMBERS[];
	static const int CONFIG_INT_MEMBERS_NUM;
	char const *CONFIG_BOOL_MEMBER = "on";

	enum IntOptions {
		MAP_X,
		MAP_Y,
		MINI_MAP_X,
		MINI_MAP_Y,
		MAX
	};

	int IntOpts[IntOptions::MAX];
	bool On;

	void LoadOpts(void);

	CustomRenderer() {
		// Default values
		IntOpts[MAP_X] = 800;
		IntOpts[MAP_Y] = 600;
		IntOpts[MINI_MAP_X] = 300;
		IntOpts[MINI_MAP_Y] = 300;
		On = false;
	}
public:
	static CustomRenderer& GetRenderer() {
		static CustomRenderer rc;
		return rc;
	}

	CustomRenderer(CustomRenderer const&) = delete;
	void operator= (CustomRenderer const&) = delete;

	void Init(void) {
		LoadOpts();
		if (On == false) return;

		sc2::renderer::Initialize("Rendered", 50, 50, IntOpts[MAP_X] + IntOpts[MAP_Y],
				std::max(IntOpts[MINI_MAP_Y], IntOpts[MAP_Y]));
	}
	void Close(void) { 
		if (On == false) return;

		sc2::renderer::Shutdown(); 
	}
	void Render(const SC2APIProtocol::Observation* observation);
};

class SuperBot : public sc2::Agent {
	// Make this configurable. Maybe?
	CustomRenderer &renderer;

	SuperBot() : sc2::Agent(), renderer(CustomRenderer::GetRenderer()) {}
public:

	static SuperBot& GetSuperBot() {
		static SuperBot rc;
		return rc;
	}

	SuperBot(SuperBot const&) = delete;
	void operator= (SuperBot const&) = delete;

	virtual void OnGameStart() final {
		renderer.Init();
	}

	virtual void OnStep() final {
		renderer.Render(Observation()->GetRawObservation());
		// LOG(INFO) << "Game Loop = " << Observation()->GetGameLoop();
		// LOG(INFO) << "Minerals = " << Observation()->GetMinerals();
		// LOG(INFO) << "Vespene Gas = " << Observation()->GetVespene();
	}

	virtual void OnGameEnd() final {
		renderer.Close();
	}
};


#endif
