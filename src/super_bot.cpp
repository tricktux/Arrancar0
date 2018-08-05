
#include "super_bot.hpp"

#include <sc2renderer/sc2_renderer.h>


void SuperBot::Render() {
	const SC2APIProtocol::Observation* observation = Observation()->GetRawObservation();
	const SC2APIProtocol::ObservationRender& render =  observation->render_data();

	const SC2APIProtocol::ImageData& minimap = render.minimap();
	sc2::renderer::ImageRGB(&minimap.data().data()[0], minimap.size().x(), minimap.size().y(), 0, std::max(kMiniMapY, kMapY) - kMiniMapY);

	const SC2APIProtocol::ImageData& map = render.map();
	sc2::renderer::ImageRGB(&map.data().data()[0], map.size().x(), map.size().y(), kMiniMapX, 0);

	sc2::renderer::Render();
}
