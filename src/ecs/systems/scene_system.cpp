// this deals with the game's story scenes
// "scene" = list of dialogue to show next? what about smaller-scale branching?

// idea 1 - polling: every X frames, scene system checks states of interested objects in world
//			and decide if scene should update/what next scene should be
//			each scene as a "start scene" condition. 
//			When scene ends, updates list of "should check" scene conditions

// idea 2 - callbacks: whenever an object of interest changes, object lets scene system know
//			then scene system will wake up and investigate

// example scenes:
// S1: tutorial intro dialogue (conditions to check before: nothing, start immediately)
// S2: after player beats tutorial room dialogue (conditions to check: if first room is cleared/all enemies died in room)
// S3: checkpoint 1, after player defeats first boss and scientist calls player (conditions: first boss dead)
// S4: upon entering boss room
// S5: after defeating boss 

// simplistic implementation: hard code scenes, after 1 scene ends move onto next
// slightly more complex: parse condition from external script for scene, translate condition into code
// eg: if boss1 == dead

// better idea: each scene is tied to a room. story doesn't happen outside of specific rooms anyway.
// room system is very vertsatile!!!
