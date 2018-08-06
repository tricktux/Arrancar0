
#include "super_bot.hpp"

#include <sc2renderer/sc2_renderer.h>

#include "config.hpp"

const char *CustomRenderer::CONFIG_INT_MEMBERS[] = {
	"map_x",
	"map_y",
	"mini_map_x",
	"mini_map_y"
};
const int CustomRenderer::CONFIG_INT_MEMBERS_NUM =
	sizeof(CustomRenderer::CONFIG_INT_MEMBERS)/sizeof(char *);

void CustomRenderer::Render() {
	if (On == false) return;

	// TODO-Sun Aug 05 2018 20:29: Not working 
	// const SC2APIProtocol::Observation* observation = Observation()->GetRawObservation();
	// const SC2APIProtocol::ObservationRender& render =  observation->render_data();

	// const SC2APIProtocol::ImageData& minimap = render.minimap();
	// sc2::renderer::ImageRGB(&minimap.data().data()[0], minimap.size().x(), minimap.size().y(), 0, std::max(kMiniMapY, kMapY) - kMiniMapY);

	// const SC2APIProtocol::ImageData& map = render.map();
	// sc2::renderer::ImageRGB(&map.data().data()[0], map.size().x(), map.size().y(), kMiniMapX, 0);

	// sc2::renderer::Render();
}

void CustomRenderer::LoadOpts(void) {
	const Config &cfg = Config::GetConfig();

	cfg.GetValue(CONFIG_OBJECT, CONFIG_BOOL_MEMBER, On);

	if (On == false) return;


	for (int k=0; k < IntOptions::MAX; k++) {
		cfg.GetValue(CONFIG_OBJECT, CONFIG_INT_MEMBERS[k], IntOpts[k]);
		LOG(INFO) << "[CustomRenderer::LoadOpts]: Got: '"
			<< CONFIG_INT_MEMBERS[k] << "' = " << IntOpts[k];
	}
}
