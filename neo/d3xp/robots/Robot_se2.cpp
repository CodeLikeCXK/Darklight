// Monster_flying_cacodemon.cpp
//

#pragma hdrstop
#include "precompiled.h"

#include "../Game_local.h"

#define CACO_ATTACK_RATE	3
#define CACO_NOFOVTIME		4

CLASS_DECLARATION(idAI, rvmRobotSE2)
END_CLASS

/*
=================
rvmRobotSE2::Init
=================
*/
void rvmRobotSE2::Init(void) {
}

/*
=================
rvmRobotSE2::AI_Begin
=================
*/
void rvmRobotSE2::AI_Begin(void) {
	Event_SetState("state_Begin");	
}

/*
=====================
rvmRobotSE2::state_Begin
=====================
*/
stateResult_t rvmRobotSE2::state_Begin(stateParms_t* parms) {	
	Event_SetMoveType(MOVETYPE_FLY);
	Event_AnimState(ANIMCHANNEL_TORSO, "Torso_Idle", 8);	
	Event_SetState("state_Idle");
	return SRESULT_DONE;
}

/*
=====================
rvmRobotSE2::state_Idle
=====================
*/
stateResult_t rvmRobotSE2::state_Idle(stateParms_t* parms) {
	if (parms->stage == 0)
	{
		if (wait_for_enemy(parms) == SRESULT_DONE)
		{
			parms->stage = 1;
		}

		return SRESULT_WAIT;
	}

	nextAttack = 0;
	nextNoFOVAttack = 0;

	Event_SetState("state_Combat");
	return SRESULT_DONE;
}


/*
=====================
rvmRobotSE2::do_attack
=====================
*/
void rvmRobotSE2::do_attack(int attack_flags) {
	nextNoFOVAttack = gameLocal.SysScriptTime() + CACO_NOFOVTIME;
	if (attack_flags & ATTACK_MISSILE) {
		SetState("combat_range");
	}
}

/*
=====================
rvmRobotSE2::check_attacks
=====================
*/
int rvmRobotSE2::check_attacks() {
	float currentTime;
	int attack_flags;

	attack_flags = 0;

	if (((gameLocal.SysScriptTime() > nextNoFOVAttack) && AI_ENEMY_VISIBLE) || AI_ENEMY_IN_FOV) {
		if (!CanReachEnemy() || (currentTime >= nextAttack)) {
			if (CanHitEnemyFromAnim("range_attack")) {
				attack_flags |= ATTACK_MISSILE;
			}
		}
	}

	return attack_flags;
}

/*
=====================
monster_zombie::combat_range
=====================
*/
stateResult_t rvmRobotSE2::combat_range(stateParms_t* parms) {
	if (parms->stage == 0)
	{
		Event_FaceEnemy();
	//	Event_AnimState(ANIMCHANNEL_TORSO, "Torso_RangeAttack", 4);
		SetWaitState("range_attack");
		parms->stage = 1;
		return SRESULT_WAIT;
	}

	if (parms->stage == 1)
	{
		if (waitState == "")
		{
			parms->stage = 2;
		}
		return SRESULT_WAIT;
	}

	// don't attack for a bit
	nextAttack = gameLocal.DelayTime(CACO_ATTACK_RATE);
	nextNoFOVAttack = gameLocal.SysScriptTime() + CACO_NOFOVTIME;

	return SRESULT_DONE;
}

