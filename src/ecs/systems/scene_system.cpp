#include "scene_system.hpp"
#include "sound_system.hpp"
#include "text_system.hpp"
#include <fstream>		// for reading text files
#include <iostream>
#include <sstream>


SceneSystem::SceneSystem( SoundSystem* soundSystem) {
	this->soundSystem = soundSystem;
	storyDialogue = std::unordered_map<Scene, std::vector<Dialogue>>();
	interactibleDialogue = std::unordered_map<InteractibleDialogue, std::vector<Dialogue>>();
	loadDialogue("story");
	loadDialogue("interactable");
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
				if (callScientist) {
					InteractibleDialogue dialogueObject = { "CallScientist", gameState.dialogueChoice, 0 };
					if (interactibleDialogue.count(dialogueObject) > 0) { // if there's more lines of dialogue to be played after choice
						DialogueLines& lines = registry.dialogueLines.components[0];
						lines = DialogueLines();
						lines.lines = interactibleDialogue[dialogueObject];
					}
				}
				else {
					assert(registry.interactableInDialogue.components.size() > 0); // not ENTIRELY sure how long this is valid for, so assume it will always be for now
					Entity objectEntity = registry.interactableInDialogue.entities[0];
					registry.interactableReactions.emplace_with_duplicates(objectEntity, objectEntity, gameState.dialogueChoice); // emplace with dupes for now, in case some other system needs this
					InteractableObject& object = registry.interactables.get(objectEntity);
					InteractibleDialogue dialogueObject = { object.name, gameState.dialogueChoice, object.dialogueCount };
					if (interactibleDialogue.count(dialogueObject) > 0) { // if there's more lines of dialogue to be played after choice
						DialogueLines& lines = registry.dialogueLines.components[0];
						lines = DialogueLines();
						lines.lines = interactibleDialogue[dialogueObject];
					}
					else {
						registry.interactableInDialogue.clear();
					}
				}
			}
			else {
				// figure out where to react to story changes -- probably here for now
				handleStoryChoices();
				Scene scene = { map.currRoom.type, map.currRoom.dialogueCount, map.currRoom.cutsceneCount, map.currRoom.cleared, gameState.dialogueChoice };
				if (storyDialogue.count(scene) > 0) { // if there's more lines of dialogue to be played after choice
					DialogueLines& lines = registry.dialogueLines.components[0];
					lines = DialogueLines();
					lines.lines = storyDialogue[scene];
					map.currRoom.dialogueCount++; // increment here for now, consider removing auto-increment
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
		Scene scene = { map.currRoom.type, map.currRoom.dialogueCount, map.currRoom.cutsceneCount, map.currRoom.cleared, gameState.dialogueChoice };

		// just check for that 1 tutorial room for now
		if (map.currRoom.type == RoomType::TutorialRoom1 && map.currRoom.dialogueCount == 2 && !map.currRoom.cleared) {
			map.currRoom.cleared = true;
		}
		if (map.currRoom.type == RoomType::TutorialRoom2 && map.currRoom.dialogueCount == 1 && !map.currRoom.cleared) {
			input.lockControls = true;
			map.currRoom.dialogueCount++;
		}

		if (storyDialogue.count(scene) > 0) {
			DialogueLines& lines = registry.dialogueLines.components[0];
			lines = DialogueLines();
			lines.lines = storyDialogue[scene];
			summonDialogue();
			isStoryDialogue = true;
			callScientist = false;
		}

		for (Entity entity : registry.dialogueRequests.entities) { // IDEALLY should only be one at a time
			DialogueRequest& req = registry.dialogueRequests.get(entity);
			if (req.type == DialogueRequestType::InteractableDialogue) {
				InteractableObject& object = registry.interactables.get(entity);
				InteractibleDialogue dialogueObject = { object.name, gameState.dialogueChoice, object.dialogueCount };
				try {
					if (object.dialogueCount == 0) {
						// for objects that need to be constantly interacted with, we skip the first dialogue if it's not the first time they're interacted with
						dialogueObject.dialogueCount += (int)hasInteracted.at(object.item);
						hasInteracted.at(object.item) = 2;
					}
				}
				catch (std::out_of_range& e) {
					std::cout << "Error: " << e.what() << std::endl;
				}

				if (req.choice > -1) {
					dialogueObject.choice = req.choice;
				}
				if (interactibleDialogue.count(dialogueObject) > 0) {
					DialogueLines& lines = registry.dialogueLines.components[0];
					lines = DialogueLines();
					lines.lines = interactibleDialogue[dialogueObject];
					for (int i = 0; i < lines.lines.size(); i++) {
                        std::cout << std::to_string(i)<< "th text: "<< lines.lines[i].text << std::endl;
                    }
					registry.interactableInDialogue.clear();
					registry.interactableInDialogue.emplace(entity);
					summonInteractibleDialogue(entity);
					isStoryDialogue = false;
					callScientist = false;
				}
				else {
					std::cout << "No dialogue found for item: " << object.name << std::endl;
				}
			}
			else if (req.type == DialogueRequestType::StoryDialogue) {
				scene.dialogueCount++; // check if have match for +1 over
				if (storyDialogue.count(scene) > 0) {
					// if so, make sure to add + 1 to current room too
					map.currRoom.dialogueCount++;

					// hard code notifs for now
					if (map.currRoom.dialogueCount == 1 && map.currRoom.type == TutorialRoom1) {
						registry.uiRequests.insert(registry.players.entities[0], { UIRequestType::CallNotif });
					}

					DialogueLines& lines = registry.dialogueLines.components[0];
					lines = DialogueLines();
					lines.lines = storyDialogue[scene];
					summonDialogue();
					isStoryDialogue = true;
					callScientist = false;
				}
			}
			// mock in this way for now, may regret later
			else if (req.type == DialogueRequestType::CallDialogue && map.currRoom.type != RoomType::TutorialRoom1 
				&& (map.currRoom.type != RoomType::TutorialRoom2 || map.currRoom.cleared) && map.currRoom.type != RoomType::BossRoom) {
				InteractibleDialogue dialogueObject = { "CallScientist", gameState.dialogueChoice, 0 };
				DialogueLines& lines = registry.dialogueLines.components[0];
				lines = DialogueLines();
				lines.lines = interactibleDialogue[dialogueObject];
				summonDialogue(); 
				callScientist = true;
				isStoryDialogue = false;
				registry.uiRequests.insert(registry.players.entities[0], { UIRequestType::CallNotif });
			}
			else if (req.type == DialogueRequestType::ResetDialogue) {
				// actually not needed right now, but still leave here just incase
			}
		}
		
		registry.dialogueRequests.clear();
	}

}

void SceneSystem::playCutscene() {
	// idea: wait until all animation sequences are done
	if (registry.animationSequences.components.size() == 0) {
		GameState& gameState = registry.gameStates.components[0];
		gameState.cutScene = false;
		Map& map = registry.maps.components[0];
		map.currRoom.cutsceneCount++;
		map.currRoom.cutSceneDone = true;
	}
}

// ref: A0 template
void SceneSystem::loadDialogue(std::string dialogueType) {
	std::string filename = dialogue_path(dialogueType + ".txt").c_str();
	std::ifstream entity_file(filename);
	InteractibleDialogue item;
	std::vector<Dialogue> lines;
	Scene scene;

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
					int choice;

					std::stringstream ss_line(line);
					ss_line >> action >> roomName >> dialogueCount >> cutsceneCount >> roomCleared >> choice;

					// lazy way to deal with room enums for now, fix later
					RoomType room;
					if (roomName.compare("TutorialRoom1") == 0) {
						room = RoomType::TutorialRoom1;
					}
					else {
						room = RoomType::TutorialRoom2;
					}

					scene = { room, dialogueCount, cutsceneCount, roomCleared, choice };

					//std::cout << roomName << dialogueCount << cutsceneCount << roomCleared << std::endl;
				}
				else if (action.compare("ITEM") == 0) {
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
					try {
						lines.back().sfx = soundEffectNames.at(soundName);
					} catch (const std::out_of_range& oor) {
						std::cout << "ERROR: sound effect not found: " << soundName << std::endl;
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
					size_t start = 0U;
					size_t end = line.find(delim);
					while (end != std::string::npos)
					{
						dialogueBody += line.substr(start, end - start) + '\n';
						start = end + delim.length();
						end = line.find(delim, start);
					}
					dialogueBody += line.substr(start, end);

					// need to parse for text decoration spans
					std::string parsedBody = "";
					std::string openDelim = "<";
					std::string closedDelim = ">";
					std::string colorDecoration = "color";
					std::string animationDecoration = "animation";
					std::string equals = "=";
					std::string endDecoration = "/";
					char parameterSep = ',';
					std::vector<TextDecorationSpan> decorationSpans;
					std::stack<TextDecorationSpan> processingAnimations;
					std::stack<TextDecorationSpan> processingColors;
					start = 0U;
					end = dialogueBody.find(openDelim);
					while (end != std::string::npos)
					{
						std::string decoration = "";
						std::vector<std::string> decorations;

						parsedBody += dialogueBody.substr(start, end - start);
						decoration = dialogueBody.substr(end, dialogueBody.find(closedDelim, end) - end + 1);
						// parse each decoration separated by space
						// very messy with minimal error checking
						decorations = getTokenizedText(decoration);
						for (std::string dec : decorations) {
							//std::cout << dec << std::endl;
							assert(dec.length() > 0);

							// color span
							if (dec.find(colorDecoration) != std::string::npos) {
								if (dec.find(endDecoration) == std::string::npos) {
									std::string color = dec.substr(dec.find(colorDecoration) + colorDecoration.length() + equals.length());
									color = color.substr(0, min(color.find(closedDelim), color.find(" ")));
									processingColors.push(TextDecorationSpan{ parsedBody.length(), 0, colorNames.at(color) });
								}
								else {
									decorationSpans.push_back(processingColors.top());
									decorationSpans.back().endIndex = parsedBody.substr(0, parsedBody.find_last_not_of(' ')).length();
									processingColors.pop();
								}
							}

							// animation span
							if (dec.find(animationDecoration) != std::string::npos) {
								if (dec.find(endDecoration) == std::string::npos) {
									std::string animation = dec.substr(dec.find(animationDecoration) + animationDecoration.length() + equals.length());
									animation = animation.substr(0, min(animation.find(closedDelim), animation.find(" ")));
									// need to also parse any additional parameters
									std::string animationType = animation.substr(0, animation.find_first_of(parameterSep));
									std::vector<float> parameters;

									// ref for sstream an numbers: https://stackoverflow.com/questions/48528893/stringstream-parse-comma-separated-integers
									std::istringstream animation_ss(animation.substr(animation.find_first_of(parameterSep) + 1));
									float param;
									char commaDummy;
									while (animation_ss >> param) {
										parameters.push_back(param);
										animation_ss >> commaDummy; // eat up the comma in the stream
 									}

									switch (textAnimationNames.at(animationType)) {
										case TextAnimationType::WobblyText:
											if (parameters.empty()) {
												processingAnimations.push(TextDecorationSpan{ parsedBody.length(), 0, vec3(-1), textAnimationNames.at(animationType), std::make_shared<WobblyTextAnimation>()});
											}
											else {
												processingAnimations.push(TextDecorationSpan{ parsedBody.length(), 0, vec3(-1), textAnimationNames.at(animationType), 
													std::make_shared<WobblyTextAnimation>(parameters), parameters[parameters.size() - 1], parameters[parameters.size() - 1]});
												// remove the time parameter that is added to the end of wobbly in parsing
												processingAnimations.top().animation->parameters.erase(processingAnimations.top().animation->parameters.end() - 1);
											}
											break;
										case TextAnimationType::WavyText:
											if (parameters.empty()) {
												processingAnimations.push(TextDecorationSpan{ parsedBody.length(), 0, vec3(-1), textAnimationNames.at(animationType), std::make_shared<WavyTextAnimation>() });
											}
											else {
												processingAnimations.push(TextDecorationSpan{ parsedBody.length(), 0, vec3(-1), textAnimationNames.at(animationType), std::make_shared<WavyTextAnimation>(parameters) });
											}
											break;
										default:
											processingAnimations.push(TextDecorationSpan{ parsedBody.length(), 0, vec3(-1), textAnimationNames.at(animationType), std::make_shared<TextAnimation>()});
									}
									// make sure have the required number of parameters
									assert(processingAnimations.top().animation->parameters.size() == processingAnimations.top().animation->requiredParameters);
								}
								else {
									decorationSpans.push_back(processingAnimations.top());
									decorationSpans.back().endIndex = parsedBody.length() - 1;
									processingAnimations.pop();
								}
								
							}
							

						}
						start = dialogueBody.find(closedDelim, end) + closedDelim.length();
						end = dialogueBody.find(openDelim, start);
					}
					parsedBody += dialogueBody.substr(start, end);

					// make sure spans are closed properly
					assert(processingAnimations.empty());
					assert(processingColors.empty());

					lines.push_back(Dialogue{ parsedBody, decorationSpans });
					//std::cout << "text: " << dialogueBody << std::endl;
				}

			}
		}
		entity_file.close();
		if (dialogueType.compare("story") == 0) {
			storyDialogue.insert({ {scene}, {lines} });
		}
		else if (dialogueType.compare("interactable") == 0) {
			interactibleDialogue.insert({ {item}, {lines} });
		}
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

	// make player stop moving during dialogue
	if (gameState.dialogueScene) {
		RenderRequest& rr = registry.renderRequests.get(registry.players.entities[0]);
		rr.used_effect = EFFECT_ASSET_ID::TEXTURED;
		rr.texture_name = registry.sprites.get(registry.players.entities[0]).sprites[SPRITE_STATE::BASE];
	}
}

// shows dialogue for interactible objects immediately
void SceneSystem::summonInteractibleDialogue(Entity object) {
	IOState& iostate = registry.ioStates.components[0];
	GameState& gameState = registry.gameStates.components[0];
	Map& map = registry.maps.components[0];
	iostate.nextDialogue = true;
	gameState.dialogueScene = true;
	map.currRoom.dialogueDone = false;

	// make player stop moving during dialogue
	if (gameState.dialogueScene) {
		RenderRequest& rr = registry.renderRequests.get(registry.players.entities[0]);
		rr.used_effect = EFFECT_ASSET_ID::TEXTURED;
		rr.texture_name = registry.sprites.get(registry.players.entities[0]).sprites[SPRITE_STATE::BASE];
	}
}

void SceneSystem::handleStoryChoices() {
	// hard code reactions for now
	Map& map = registry.maps.components[0];
	GameState& gameState = registry.gameStates.components[0];
	if (map.currRoom.type == RoomType::TutorialRoom2 && map.currRoom.dialogueCount == 2 && map.currRoom.cleared) {
		std::cout << "The player has chosen ";
		if (gameState.dialogueChoice == 0) {
			std::cout << "apples!" << std::endl;
		}
		else {
			std::cout << "cherries!" << std::endl;
		}
	}
}
