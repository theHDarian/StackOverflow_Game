#include "camera_system.hpp"
#include <glm/gtx/compatibility.hpp>

CameraSystem::CameraSystem() {

}
CameraSystem::~CameraSystem() {

}

void CameraSystem::step(float elapsed_ms) {
	Camera& camera = registry.cameras.components[0];

	// fail-safe for now -- default to player as camera target
	if (!registry.motions.has(camera.target) || registry.deleteds.has(camera.target)) {
		//std::cout << "not in motions" << std::endl;
		camera.target = registry.players.entities[0];

		// completely unnecessary, but can also transition camera from old location where target used to be -> player
		CameraRequest& cameraReq = registry.cameraRequests.emplace_with_duplicates(registry.maps.entities[0]);
		cameraReq.type = CameraRequestType::ChangeTarget;
		cameraReq.newTarget = registry.players.entities[0];
		camera.startPos = camera.lookAtPos;
		camera.startZoom = camera.zoom;
	}

	if (registry.cameraRequests.components.size() > 0) {
		CameraRequest& currReq = registry.cameraRequests.components[0];
		camera.elapsedTime += elapsed_ms;
		if (currReq.type == CameraRequestType::ChangeTarget) {
			assert(registry.motions.has(currReq.newTarget));
			camera.lookAtPos = glm::lerp(camera.startPos, registry.motions.get(currReq.newTarget).position, camera.elapsedTime / currReq.transitionTime);
		}
		
		if (currReq.type == CameraRequestType::ChangeZoom || currReq.type == CameraRequestType::ChangeLookAtAndZoom) {
			camera.zoom = glm::lerp(camera.startZoom, currReq.newZoom, camera.elapsedTime / currReq.transitionTime);
		}
		if (currReq.type == CameraRequestType::ChangeLookAt || currReq.type == CameraRequestType::ChangeLookAtAndZoom) {
			camera.lookAtPos = glm::lerp(camera.startPos, currReq.newLookAt, camera.elapsedTime / currReq.transitionTime);
		}
		
		if (camera.elapsedTime >= currReq.transitionTime) { // remove the first item
			if (currReq.type == CameraRequestType::ChangeTarget) {
				camera.target = currReq.newTarget;
			}
			registry.cameraRequests.components.erase(registry.cameraRequests.components.begin());
			camera.elapsedTime = 0;
			camera.startPos = camera.lookAtPos;
			camera.startZoom = camera.zoom;
		}
	}
	else {
		camera.lookAtPos = registry.motions.get(camera.target).position;
		camera.startPos = camera.lookAtPos;
		camera.startZoom = camera.zoom;
	}
}

