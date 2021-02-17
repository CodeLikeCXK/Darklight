// Monster_flying_cacodemon.h
//

#pragma once

class rvmRobotSE2 : public idAI {
	CLASS_PROTOTYPE(rvmRobotSE2);
public:
	virtual void				Init(void) override;
	virtual void				AI_Begin(void) override;

	virtual int					check_attacks() override;
	virtual void				do_attack(int attack_flags) override;
private:
	stateResult_t				state_Begin(stateParms_t* parms);
	stateResult_t				state_Idle(stateParms_t* parms);
	stateResult_t				combat_range(stateParms_t* parms);
private:
	float		nextAttack;
	float		nextNoFOVAttack;
	idEntity	*combat_node;
};