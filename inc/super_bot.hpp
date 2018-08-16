#ifndef SUPER_BOT_HPP
#define SUPER_BOT_HPP

#include <sc2api/sc2_api.h>
#include <sc2renderer/sc2_renderer.h>
#include <glog/logging.h>

#include <algorithm>

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

public:
	CustomRenderer() {
		// Default values
		IntOpts[MAP_X] = 800;
		IntOpts[MAP_Y] = 600;
		IntOpts[MINI_MAP_X] = 300;
		IntOpts[MINI_MAP_Y] = 300;
		On = false;
	}

	void Init(void);

	void Close(void) { 
		if (On == false) return;

		sc2::renderer::Shutdown(); 
	}
	void Render(const SC2APIProtocol::Observation* observation);

	int GetSettings(sc2::RenderSettings &settings) {
		if (On == false) return 0;

		settings.map_x = IntOpts[MAP_X];
		settings.map_y = IntOpts[MAP_Y];
		settings.minimap_x = IntOpts[MINI_MAP_X];
		settings.minimap_y = IntOpts[MINI_MAP_Y];
		return 1;
	}
	bool LoadOpts(void);
};

class SuperBot : public sc2::Agent {
private:
	const sc2::Race DEFAULT_RACE = sc2::Race::Terran;

	CustomRenderer renderer;
	int num_workers;
	sc2::Race my_race;

	SuperBot() :
		sc2::Agent(), 
		num_workers(0),
		my_race(DEFAULT_RACE)
	{}

	// TODO-[RM]-(Sun Aug 12 2018 22:16):
	// - Implement this function
	void BuildMoreWorkers(void);
public:

	static SuperBot& GetSuperBot() {
		static SuperBot rc;
		return rc;
	}

	SuperBot(SuperBot const&) = delete;
	void operator= (SuperBot const&) = delete;

	virtual void OnGameStart() final;

	virtual void OnStep() final;

	virtual void OnGameEnd() final {
		renderer.Close();
	}

	virtual void OnUnitIdle(const sc2::Unit* unit) final;

	int LoadRendererConfigAndSettings(sc2::RenderSettings &settings);
};


#endif
