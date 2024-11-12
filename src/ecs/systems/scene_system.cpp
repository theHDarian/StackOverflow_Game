#include "scene_system.hpp"
#include "sound_system.hpp"
#include <fstream>		// for reading text files
#include <iostream>
#include <sstream>	

SceneSystem::SceneSystem( SoundSystem* soundSystem) {
	this->soundSystem = soundSystem;
	storyDialogue = std::unordered_map<Scene, std::vector<Dialogue>>();
	interactibleDialogue = std::unordered_map<InteractibleDialogue, std::vector<Dialogue>>();
	currentObject = Entity();
	loadStoryDialogue();
	loadInteractableDialogue();
}

SceneSystem::~SceneSystem() {

}

void SceneSystem::step(float elapsed_ms) {
	Map& map = registry.maps.components[0];
	GameState& gameState = registry.gameStates.components[0];
	IOState& input = registry.ioStates.components[0];

	if (gameState.dialogueScene && input.nextDialogue) {
		// update choice
		if (registry.dialogueChoices.entities.size() > 0) {
			gameState.dialogueChoice = input.hoveringDialogueChoice; // commit player choice
			if (!isStoryDialogue) {
				assert(registry.interactables.has(currentObject)); // not ENTIRELY sure how long this is valid for, so assume it will always be for now

				Entity objectEntity = currentObject;
				registry.interactableReactions.emplace_with_duplicates(objectEntity, objectEntity, gameState.dialogueChoice); // emplace with dupes for now, in case some other system needs this
				InteractableObject& object = registry.interactables.get(objectEntity);
				InteractibleDialogue dialogueObject = { object.name, gameState.dialogueChoice, object.dialogueCount };
				if (interactibleDialogue.count(dialogueObject) > 0) { // if there's more lines of dialogue to be played after choice
					DialogueLines& lines = registry.dialogueLines.components[0];
					lines = DialogueLines();
					lines.lines = interactibleDialogue[dialogueObject];
				}
			}
			else {
				// figure out where to react to story changes -- probably here for now
				handleStoryChoices();
				Scene scene = { map.currRoom.type, map.currRoom.dialogueCount, map.currRoom.cutsceneCount, map.currRoom.cleared };
				if (storyDialogue.count(scene) > 0) { // if there's more lines of dialogue to be played after choice
					DialogueLines& lines = registry.dialogueLines.components[0];
					lines = DialogueLines();
					lines.lines = storyDialogue[scene];
				}
			}

			gameState.dialogueChoice = -1; 
			// enforce it to be valid only for duration of request
			// hope no other system needs to look at it rn...
		}

		// clear choices here for now
		for (int i = registry.dialogueChoices.size() - 1; i >= 0; i--) {
			Entity e = registry.dialogueChoices.entities[i];
			registry.deleteEntityAndRelatedEntities(e);
		}
	}
	
	if (map.currRoom.cutSceneDone) {
		if (map.currRoom.type == RoomType::TutorialRoom1 && map.currRoom.cutsceneCount == 0) {
			// beginning animation sequence - mc wakes up
			Entity player = registry.players.entities[0];
			AnimationSequence& as = registry.animationSequences.emplace(player);
			as.nextEffect = EFFECT_ASSET_ID::TEXTURED;
			as.nextSprite = "mcv1_base.png";
			RenderRequest& rr = registry.renderRequests.get(player);
			rr.used_effect = EFFECT_ASSET_ID::ANIMATE;
			rr.texture_name = "mc_startup";
			registry.animations.get(player).max_frames = 40; // hard code for now

			// play cutscene
			gameState.cutScene = true;
			map.currRoom.cutSceneDone = false;
		}
	}

	// make sure we only have 1 dialogue going on at a time
	if (map.currRoom.dialogueDone) {
		// construct current scene object
		Scene scene = { map.currRoom.type, map.currRoom.dialogueCount, map.currRoom.cutsceneCount, map.currRoom.cleared };

		// again, just check for that 1 tutorial room for now
		if (map.currRoom.type == RoomType::TutorialRoom1 && map.currRoom.dialogueCount == 2 && !map.currRoom.cleared) {
			map.currRoom.cleared = true;
		}

		if (storyDialogue.count(scene) > 0) {
			DialogueLines& lines = registry.dialogueLines.components[0];
			lines = DialogueLines();
			lines.lines = storyDialogue[scene];
			// right now, just check for that one scene
			if (scene == Scene{RoomType::TutorialRoom1, 1, 1, false}) {
				playerInputDialogue();
			}
			else {
				summonDialogue();
			}
			isStoryDialogue = true;
		}

		for (Entity entity : registry.dialogueRequests.entities) { // IDEALLY should only be one at a time
			InteractableObject& object = registry.interactables.get(entity);
			InteractibleDialogue dialogueObject = { object.name, gameState.dialogueChoice, object.dialogueCount };
			if (interactibleDialogue.count(dialogueObject) > 0) {
				DialogueLines& lines = registry.dialogueLines.components[0];
				lines = DialogueLines();
				lines.lines = interactibleDialogue[dialogueObject];
				currentObject = entity; // need to keep track of current speaking object
				summonInteractibleDialogue(entity);
				isStoryDialogue = false;
			}
		}
		registry.dialogueRequests.clear();
	}

}

// ref: A0 template
void SceneSystem::loadInteractableDialogue() {
	std::string filename = dialogue_path("interactable.txt").c_str();
	std::ifstream entity_file(filename);
	//Scene scene;
	// for now, consider using just item name. In future may consider other conditions
	//std::string item;
	InteractibleDialogue item;
	std::vector<Dialogue> lines;

	if (entity_file.is_open())
	{
		std::string line;

		while (!entity_file.eof())
		{
			std::getline(entity_file, line);

			if (line.length() > 0 && line[0] != '#')
			{
				// split line different based on what first word is
				std::string action = line.substr(0, line.find_first_of(" "));

				if (action.compare("ITEM") == 0) {
					// new item, so place all prev lines into map, unless this is the first item
					if (lines.size() > 0) {
						interactibleDialogue.insert({ {item}, {lines} });
						lines = std::vector<Dialogue>();
					}

					std::string itemName;
					int choice;
					int dialogueCount;

					std::stringstream ss_line(line);
					ss_line >> action >> itemName >> choice >> dialogueCount;

					item = { itemName, choice, dialogueCount };
				}
				else if (action.compare("SPEAKER") == 0) {
					std::string speakerName;
					std::string speakerAvatar;
					std::string reqBody;

					std::stringstream ss_line(line);

					// to allow spaces in speaker name
					reqBody = line.substr(line.find_first_of(" ") + 1);
					speakerAvatar = reqBody.substr(reqBody.find_last_of(" ") + 1);
					speakerName = reqBody.substr(0, reqBody.find_last_of(" "));

					lines.back().speakerName = speakerName;
					lines.back().speakerAvatar = speakerAvatar;
				}
				else if (action.compare("SOUND") == 0) {
					std::string soundName;

					std::stringstream ss_line(line);
					ss_line >> action >> soundName;

					// lazy way to deal with sound enums for now, fix later
					if (soundName.compare("DoorOpen") == 0) {
						lines.back().sfx = SoundType::DoorOpen;
					}
					else {
						lines.back().sfx = SoundType::IncomingDialogue;
					}
				}
				else if (action.compare("CHOICES") == 0) {
					std::string choiceLine = line.substr(line.find_first_of(" ") + 1);
					std::stringstream ss_choices(choiceLine);

					std::string choice;
					std::vector<std::string> choices;

					// ref: https://stackoverflow.com/questions/5888022/split-string-by-single-spaces
					while (std::getline(ss_choices, choice, '\\')) {
						choices.push_back(choice);
					}

					lines.back().choices = choices;
				}
				else if (action.compare("CUT") == 0) {
					std::string cutInTexture;
					std::stringstream ss_line(line);
					ss_line >> action >> cutInTexture;
					lines.back().cutInTexture = cutInTexture;
				}
				else { // this is just a body of text
					// need to manually add \n back into strings... use this until can think of better way
					//ref: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c?page=1&tab=scoredesc#tab-top
					std::string delim = "\\n";
					std::string dialogueBody = "";
					auto start = 0U;
					auto end = line.find(delim);
					while (end != std::string::npos)
					{
						dialogueBody += line.substr(start, end - start) + '\n';
						start = end + delim.length();
						end = line.find(delim, start);
					}
					dialogueBody += line.substr(start, end);

					lines.push_back(Dialogue{ dialogueBody });
					//std::cout << "text: " << dialogueBody << std::endl;
				}

			}
		}
		entity_file.close();
		interactibleDialogue.insert({ {item}, {lines} });
	}
	else
	{
		std::cout << "ERROR: failed to open file: " << filename << std::endl;
	}
}

// ref: A0 template
void SceneSystem::loadStoryDialogue() {
	std::string filename = dialogue_path("story.txt").c_str();
	std::ifstream entity_file(filename);
	Scene scene;
	std::vector<Dialogue> lines;

	if (entity_file.is_open())
	{
		std::string line;

		while (!entity_file.eof())
		{
			std::getline(entity_file, line);

			if (line.length() > 0 && line[0] != '#')
			{
				// split line different based on what first word is
				std::string action = line.substr(0, line.find_first_of(" "));
				//std::cout << action << std::endl;

				if (action.compare("SCENE") == 0) {
					// new scene, so place all prev lines into map, unless this is the first scene
					if (lines.size() > 0) {
						storyDialogue.insert({ {scene}, {lines} });
						lines = std::vector<Dialogue>();
					}

					std::string roomName;
					int dialogueCount;
					int cutsceneCount;
					bool roomCleared;

					std::stringstream ss_line(line);
					ss_line >> action >> roomName >> dialogueCount >> cutsceneCount >> roomCleared;

					// lazy way to deal with room enums for now, fix later
					RoomType room;
					if (roomName.compare("TutorialRoom1") == 0) {
						room = RoomType::TutorialRoom1;
					}
					else {
						room = RoomType::TutorialRoom2;
					}

					scene = { room, dialogueCount, cutsceneCount, roomCleared };

					//std::cout << roomName << dialogueCount << cutsceneCount << roomCleared << std::endl;
				}
				else if (action.compare("SPEAKER") == 0) {
					std::string speakerName;
					std::string speakerAvatar;

					std::stringstream ss_line(line);
					ss_line >> action >> speakerName >> speakerAvatar;

					lines.back().speakerName = speakerName;
					lines.back().speakerAvatar = speakerAvatar;
				}
				else if (action.compare("SOUND") == 0) {
					std::string soundName;

					std::stringstream ss_line(line);
					ss_line >> action >> soundName;

					// lazy way to deal with sound enums for now, fix later
					if (soundName.compare("DoorOpen") == 0) {
						lines.back().sfx = SoundType::DoorOpen;
					}
					else {
						lines.back().sfx = SoundType::IncomingDialogue;
					}
				}
				else if (action.compare("CHOICES") == 0) {
					std::string choiceLine = line.substr(line.find_first_of(" ") + 1);
					std::stringstream ss_choices(choiceLine);

					std::string choice;
					std::vector<std::string> choices;

					// ref: https://stackoverflow.com/questions/5888022/split-string-by-single-spaces
					while (std::getline(ss_choices, choice, '\\')) {
						choices.push_back(choice);
					}

					lines.back().choices = choices;
				}
				else if (action.compare("CUT") == 0) {
					std::string cutInTexture;
					std::stringstream ss_line(line);
					ss_line >> action >> cutInTexture;
					lines.back().cutInTexture = cutInTexture;
				}
				else { // this is just a body of text
					// need to manually add \n back into strings... use this until can think of better way
					//ref: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c?page=1&tab=scoredesc#tab-top
					std::string delim = "\\n";
					std::string dialogueBody = "";
					auto start = 0U;
					auto end = line.find(delim);
					while (end != std::string::npos)
					{
						dialogueBody += line.substr(start, end - start) + '\n';
						start = end + delim.length();
						end = line.find(delim, start);
					}
					dialogueBody += line.substr(start, end);

					lines.push_back(Dialogue{ dialogueBody });
					//std::cout << "text: " << dialogueBody << std::endl;
				}

			}
		}
		entity_file.close();
		storyDialogue.insert({ {scene}, {lines} });
	}
	else
	{
		std::cout << "ERROR: failed to open file: " << filename << std::endl;
	}
}

// shows dialogue immediately
void SceneSystem::summonDialogue() {
	IOState& iostate = registry.ioStates.components[0];
	GameState& gameState = registry.gameStates.components[0];
	Map& map = registry.maps.components[0];
	iostate.nextDialogue = true;
	gameState.dialogueScene = true;
	map.currRoom.dialogueDone = false;
	map.currRoom.dialogueCount++;
}

// shows dialogue for interactible objects immediately
void SceneSystem::summonInteractibleDialogue(Entity object) {
	IOState& iostate = registry.ioStates.components[0];
	GameState& gameState = registry.gameStates.components[0];
	Map& map = registry.maps.components[0];
	iostate.nextDialogue = true;
	gameState.dialogueScene = true;
	map.currRoom.dialogueDone = false;
}

// shows dialogue only when player presses E (for now)
void SceneSystem::playerInputDialogue() {
	Map& map = registry.maps.components[0];
	map.currRoom.dialogueDone = false;
	map.currRoom.dialogueCount++;
}

void SceneSystem::handleStoryChoices() {
	// hard code reactions for now
}